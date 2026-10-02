// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include <Windows.h>
#include <cor.h>
#include <cordebug.h>

#include <atomic>
#include <cstdio>
#include <cwchar>
#include <string>
#include <vector>

namespace
{
    using StartupCallback = void(STDMETHODCALLTYPE*)(IUnknown*, void*, HRESULT);
    using CreateProcessForLaunchFn = HRESULT(STDMETHODCALLTYPE*)(LPWSTR, BOOL, void*, LPCWSTR, DWORD*, HANDLE*);
    using ResumeProcessFn = HRESULT(STDMETHODCALLTYPE*)(HANDLE);
    using CloseResumeHandleFn = HRESULT(STDMETHODCALLTYPE*)(HANDLE);
    using RegisterForRuntimeStartupFn = HRESULT(STDMETHODCALLTYPE*)(DWORD, StartupCallback, void*, void**);
    using UnregisterForRuntimeStartupFn = HRESULT(STDMETHODCALLTYPE*)(void*);

    const GUID MetadataImportId =
        { 0x7dac8207, 0xd3ae, 0x4c75, { 0x9b, 0x67, 0x92, 0x80, 0x1a, 0x49, 0x7d, 0x44 } };

    template <typename T>
    void ReleaseInterface(T*& value)
    {
        if (value != nullptr)
        {
            value->Release();
            value = nullptr;
        }
    }

    bool Check(HRESULT hr, const wchar_t* operation)
    {
        if (SUCCEEDED(hr))
        {
            return true;
        }

        std::fwprintf(stderr, L"%ls failed: 0x%08X\n", operation, static_cast<unsigned>(hr));
        return false;
    }

    class ManagedCallback final :
        public ICorDebugManagedCallback,
        public ICorDebugManagedCallback2,
        public ICorDebugManagedCallback5
    {
    public:
        ManagedCallback(bool installFilter, HANDLE exitEvent)
            : _refCount(1),
              _process(nullptr),
              _installFilter(installFilter),
              _filterInstalled(false),
              _exitEvent(exitEvent),
              _firstChanceCallbacks(0),
              _notificationCallbacks(0),
              _invalidNotification(false)
        {
        }

        ~ManagedCallback()
        {
            ReleaseInterface(_process);
        }

        bool FilterInstalled() const
        {
            return _filterInstalled.load();
        }

        long FirstChanceCallbacks() const
        {
            return _firstChanceCallbacks.load();
        }

        long NotificationCallbacks() const
        {
            return _notificationCallbacks.load();
        }

        bool InvalidNotification() const
        {
            return _invalidNotification.load();
        }

        HRESULT STDMETHODCALLTYPE QueryInterface(REFIID id, void** result) override
        {
            if (result == nullptr)
            {
                return E_POINTER;
            }

            if (id == IID_IUnknown || id == IID_ICorDebugManagedCallback)
            {
                *result = static_cast<ICorDebugManagedCallback*>(this);
            }
            else if (id == IID_ICorDebugManagedCallback2)
            {
                *result = static_cast<ICorDebugManagedCallback2*>(this);
            }
            else if (id == IID_ICorDebugManagedCallback5)
            {
                *result = static_cast<ICorDebugManagedCallback5*>(this);
            }
            else
            {
                *result = nullptr;
                return E_NOINTERFACE;
            }

            AddRef();
            return S_OK;
        }

        ULONG STDMETHODCALLTYPE AddRef() override
        {
            return static_cast<ULONG>(InterlockedIncrement(&_refCount));
        }

        ULONG STDMETHODCALLTYPE Release() override
        {
            LONG count = InterlockedDecrement(&_refCount);
            if (count == 0)
            {
                delete this;
            }

            return static_cast<ULONG>(count);
        }

        HRESULT STDMETHODCALLTYPE CreateProcess(ICorDebugProcess* process) override
        {
            _process = process;
            _process->AddRef();
            return Continue();
        }

        HRESULT STDMETHODCALLTYPE ExceptionNotification(
            ICorDebugProcess*,
            ICorDebugThread*,
            CORDB_ADDRESS moduleAddress,
            mdTypeDef typeDef,
            CORDB_ADDRESS instructionAddress,
            CorDebugExceptionNotificationStage stage) override
        {
            if (moduleAddress == 0 ||
                typeDef == mdTypeDefNil ||
                instructionAddress == 0 ||
                stage != COR_DEBUG_EXCEPTION_STAGE_FIRST_CHANCE)
            {
                _invalidNotification.store(true);
            }

            _notificationCallbacks.fetch_add(1);
            return S_OK;
        }

        HRESULT STDMETHODCALLTYPE ExitProcess(ICorDebugProcess*) override
        {
            HRESULT hr = Continue();
            SetEvent(_exitEvent);
            return hr;
        }

        HRESULT STDMETHODCALLTYPE LoadModule(ICorDebugAppDomain*, ICorDebugModule* module) override
        {
            if (_installFilter && !_filterInstalled.load())
            {
                TryInstallFilter(module);
            }

            return Continue();
        }

        HRESULT STDMETHODCALLTYPE Exception(
            ICorDebugAppDomain*,
            ICorDebugThread*,
            ICorDebugFrame*,
            ULONG32,
            CorDebugExceptionCallbackType eventType,
            DWORD) override
        {
            if (eventType == DEBUG_EXCEPTION_FIRST_CHANCE)
            {
                _firstChanceCallbacks.fetch_add(1);
            }

            return Continue();
        }

#define CONTINUE_CALLBACK(name, signature) \
        HRESULT STDMETHODCALLTYPE name signature override { return Continue(); }

        CONTINUE_CALLBACK(Breakpoint, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugBreakpoint*))
        CONTINUE_CALLBACK(StepComplete, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugStepper*, CorDebugStepReason))
        CONTINUE_CALLBACK(Break, (ICorDebugAppDomain*, ICorDebugThread*))
        CONTINUE_CALLBACK(Exception, (ICorDebugAppDomain*, ICorDebugThread*, BOOL))
        CONTINUE_CALLBACK(EvalComplete, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugEval*))
        CONTINUE_CALLBACK(EvalException, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugEval*))
        CONTINUE_CALLBACK(CreateThread, (ICorDebugAppDomain*, ICorDebugThread*))
        CONTINUE_CALLBACK(ExitThread, (ICorDebugAppDomain*, ICorDebugThread*))
        CONTINUE_CALLBACK(UnloadModule, (ICorDebugAppDomain*, ICorDebugModule*))
        CONTINUE_CALLBACK(LoadClass, (ICorDebugAppDomain*, ICorDebugClass*))
        CONTINUE_CALLBACK(UnloadClass, (ICorDebugAppDomain*, ICorDebugClass*))
        CONTINUE_CALLBACK(LogMessage, (ICorDebugAppDomain*, ICorDebugThread*, LONG, WCHAR*, WCHAR*))
        CONTINUE_CALLBACK(LogSwitch, (ICorDebugAppDomain*, ICorDebugThread*, LONG, ULONG, WCHAR*, WCHAR*))
        CONTINUE_CALLBACK(CreateAppDomain, (ICorDebugProcess*, ICorDebugAppDomain*))
        CONTINUE_CALLBACK(ExitAppDomain, (ICorDebugProcess*, ICorDebugAppDomain*))
        CONTINUE_CALLBACK(LoadAssembly, (ICorDebugAppDomain*, ICorDebugAssembly*))
        CONTINUE_CALLBACK(UnloadAssembly, (ICorDebugAppDomain*, ICorDebugAssembly*))
        CONTINUE_CALLBACK(ControlCTrap, (ICorDebugProcess*))
        CONTINUE_CALLBACK(NameChange, (ICorDebugAppDomain*, ICorDebugThread*))
        CONTINUE_CALLBACK(UpdateModuleSymbols, (ICorDebugAppDomain*, ICorDebugModule*, IStream*))
        CONTINUE_CALLBACK(EditAndContinueRemap, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugFunction*, BOOL))
        CONTINUE_CALLBACK(BreakpointSetError, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugBreakpoint*, DWORD))
        CONTINUE_CALLBACK(FunctionRemapOpportunity, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugFunction*, ICorDebugFunction*, ULONG32))
        CONTINUE_CALLBACK(CreateConnection, (ICorDebugProcess*, CONNID, WCHAR*))
        CONTINUE_CALLBACK(ChangeConnection, (ICorDebugProcess*, CONNID))
        CONTINUE_CALLBACK(DestroyConnection, (ICorDebugProcess*, CONNID))
        CONTINUE_CALLBACK(ExceptionUnwind, (ICorDebugAppDomain*, ICorDebugThread*, CorDebugExceptionUnwindCallbackType, DWORD))
        CONTINUE_CALLBACK(FunctionRemapComplete, (ICorDebugAppDomain*, ICorDebugThread*, ICorDebugFunction*))
        CONTINUE_CALLBACK(MDANotification, (ICorDebugController*, ICorDebugThread*, ICorDebugMDA*))

#undef CONTINUE_CALLBACK

        HRESULT STDMETHODCALLTYPE DebuggerError(ICorDebugProcess*, HRESULT error, DWORD code) override
        {
            std::fwprintf(stderr, L"DebuggerError callback: 0x%08X, code %u\n", static_cast<unsigned>(error), code);
            SetEvent(_exitEvent);
            return error;
        }

    private:
        HRESULT Continue()
        {
            return _process != nullptr ? _process->Continue(FALSE) : E_UNEXPECTED;
        }

        void TryInstallFilter(ICorDebugModule* module)
        {
            IMetaDataImport* metadata = nullptr;
            HRESULT hr = module->GetMetaDataInterface(MetadataImportId, reinterpret_cast<IUnknown**>(&metadata));
            if (FAILED(hr))
            {
                return;
            }

            mdTypeDef exceptionType = mdTypeDefNil;
            HCORENUM enumerator = nullptr;
            mdTypeDef types[64];
            ULONG fetched = 0;

            while (SUCCEEDED(metadata->EnumTypeDefs(&enumerator, types, ARRAYSIZE(types), &fetched)) && fetched != 0)
            {
                for (ULONG i = 0; i < fetched; i++)
                {
                    WCHAR name[256];
                    ULONG nameLength = 0;
                    DWORD flags = 0;
                    mdToken extends = mdTokenNil;
                    if (SUCCEEDED(metadata->GetTypeDefProps(
                            types[i], name, ARRAYSIZE(name), &nameLength, &flags, &extends)) &&
                        std::wcscmp(name, L"FilteredException") == 0)
                    {
                        exceptionType = types[i];
                        break;
                    }
                }

                if (exceptionType != mdTypeDefNil)
                {
                    break;
                }
            }

            if (enumerator != nullptr)
            {
                metadata->CloseEnum(enumerator);
            }
            metadata->Release();

            if (exceptionType == mdTypeDefNil)
            {
                return;
            }

            CORDB_ADDRESS moduleAddress = 0;
            ICorDebugProcess13* process13 = nullptr;
            if (FAILED(module->GetBaseAddress(&moduleAddress)) ||
                FAILED(_process->QueryInterface(IID_ICorDebugProcess13, reinterpret_cast<void**>(&process13))))
            {
                return;
            }

            COR_DEBUG_EXCEPTION_FILTER_ENTRY entries[2] = {};
            entries[0].moduleAddress = moduleAddress;
            entries[0].typeDef = exceptionType;
            entries[0].notificationMask = COR_DEBUG_EXCEPTION_STAGE_FIRST_CHANCE;

            entries[1].flags = COR_DEBUG_EXCEPTION_FILTER_PROCESS_DEFAULT;
            entries[1].notificationMask =
                COR_DEBUG_EXCEPTION_STAGE_FIRST_CHANCE |
                COR_DEBUG_EXCEPTION_STAGE_USER_FIRST_CHANCE;
            entries[1].stoppingMask = entries[1].notificationMask;

            hr = process13->SetExceptionFilter(ARRAYSIZE(entries), entries);
            process13->Release();

            if (SUCCEEDED(hr))
            {
                _filterInstalled.store(true);
            }
            else
            {
                std::fwprintf(stderr, L"SetExceptionFilter failed: 0x%08X\n", static_cast<unsigned>(hr));
            }
        }

        LONG _refCount;
        ICorDebugProcess* _process;
        bool _installFilter;
        std::atomic<bool> _filterInstalled;
        HANDLE _exitEvent;
        std::atomic<long> _firstChanceCallbacks;
        std::atomic<long> _notificationCallbacks;
        std::atomic<bool> _invalidNotification;
    };

    struct DebugShim
    {
        HMODULE module = nullptr;
        CreateProcessForLaunchFn createProcessForLaunch = nullptr;
        ResumeProcessFn resumeProcess = nullptr;
        CloseResumeHandleFn closeResumeHandle = nullptr;
        RegisterForRuntimeStartupFn registerForRuntimeStartup = nullptr;
        UnregisterForRuntimeStartupFn unregisterForRuntimeStartup = nullptr;

        bool Load(const wchar_t* path)
        {
            module = LoadLibraryW(path);
            if (module == nullptr)
            {
                std::fwprintf(stderr, L"LoadLibrary(%ls) failed: %u\n", path, GetLastError());
                return false;
            }

#define LOAD_EXPORT(member, name) member = reinterpret_cast<decltype(member)>(GetProcAddress(module, name))
            LOAD_EXPORT(createProcessForLaunch, "CreateProcessForLaunch");
            LOAD_EXPORT(resumeProcess, "ResumeProcess");
            LOAD_EXPORT(closeResumeHandle, "CloseResumeHandle");
            LOAD_EXPORT(registerForRuntimeStartup, "RegisterForRuntimeStartup");
            LOAD_EXPORT(unregisterForRuntimeStartup, "UnregisterForRuntimeStartup");
#undef LOAD_EXPORT

            return createProcessForLaunch != nullptr &&
                resumeProcess != nullptr &&
                closeResumeHandle != nullptr &&
                registerForRuntimeStartup != nullptr &&
                unregisterForRuntimeStartup != nullptr;
        }

        ~DebugShim()
        {
            if (module != nullptr)
            {
                FreeLibrary(module);
            }
        }
    };

    class DebugSession
    {
    public:
        DebugSession(DebugShim& shim, bool installFilter)
            : _shim(shim),
              _installFilter(installFilter),
              _processId(0),
              _unregisterToken(nullptr),
              _debug(nullptr),
              _process(nullptr),
              _callback(nullptr),
              _startupEvent(CreateEventW(nullptr, TRUE, FALSE, nullptr)),
              _exitEvent(CreateEventW(nullptr, TRUE, FALSE, nullptr)),
              _startupResult(E_PENDING)
        {
        }

        ~DebugSession()
        {
            if (_debug != nullptr)
            {
                _debug->Terminate();
            }
            ReleaseInterface(_process);
            ReleaseInterface(_debug);
            if (_callback != nullptr)
            {
                _callback->Release();
            }
            CloseHandle(_startupEvent);
            CloseHandle(_exitEvent);
        }

        bool Run(const std::wstring& commandLine, int iterations, int samples)
        {
            std::vector<wchar_t> mutableCommand(commandLine.begin(), commandLine.end());
            mutableCommand.push_back(L'\0');

            HANDLE resumeHandle = nullptr;
            if (!Check(_shim.createProcessForLaunch(
                    mutableCommand.data(), TRUE, nullptr, nullptr, &_processId, &resumeHandle),
                    L"CreateProcessForLaunch"))
            {
                return false;
            }

            HANDLE processHandle = OpenProcess(SYNCHRONIZE | PROCESS_QUERY_LIMITED_INFORMATION, FALSE, _processId);
            if (processHandle == nullptr)
            {
                std::fwprintf(stderr, L"OpenProcess failed: %u\n", GetLastError());
                return false;
            }

            if (!Check(_shim.registerForRuntimeStartup(
                    _processId, &DebugSession::StartupCallback, this, &_unregisterToken),
                    L"RegisterForRuntimeStartup") ||
                !Check(_shim.resumeProcess(resumeHandle), L"ResumeProcess"))
            {
                CloseHandle(processHandle);
                return false;
            }
            _shim.closeResumeHandle(resumeHandle);

            bool succeeded = WaitForSingleObject(_startupEvent, 30'000) == WAIT_OBJECT_0 &&
                Check(_startupResult.load(), L"DBI startup");

            if (succeeded)
            {
                succeeded = WaitForSingleObject(processHandle, 10 * 60 * 1000) == WAIT_OBJECT_0;
            }

            DWORD exitCode = 0;
            GetExitCodeProcess(processHandle, &exitCode);
            CloseHandle(processHandle);
            WaitForSingleObject(_exitEvent, 5'000);

            const long expectedCallbacks = 2'100L + 4L * samples * iterations;
            const long callbacks = _callback != nullptr ? _callback->FirstChanceCallbacks() : -1;
            const long notifications = _callback != nullptr ? _callback->NotificationCallbacks() : -1;
            if (_installFilter)
            {
                succeeded = succeeded &&
                    _callback != nullptr &&
                    _callback->FilterInstalled() &&
                    callbacks == 0 &&
                    notifications == expectedCallbacks &&
                    !_callback->InvalidNotification();
            }
            else
            {
                succeeded = succeeded &&
                    callbacks == expectedCallbacks &&
                    notifications == 0;
            }

            std::fwprintf(
                stderr,
                L"%ls: first-chance callbacks=%ld, notifications=%ld, expected=%ld, exit=%u\n",
                _installFilter ? L"filtered" : L"baseline",
                callbacks,
                notifications,
                _installFilter ? 0L : expectedCallbacks,
                exitCode);

            return succeeded && exitCode == 100;
        }

    private:
        static void STDMETHODCALLTYPE StartupCallback(IUnknown* unknown, void* parameter, HRESULT hr)
        {
            DebugSession* session = static_cast<DebugSession*>(parameter);
            session->_startupResult.store(SUCCEEDED(hr) ? session->Startup(unknown) : hr);
            if (session->_unregisterToken != nullptr)
            {
                session->_shim.unregisterForRuntimeStartup(session->_unregisterToken);
                session->_unregisterToken = nullptr;
            }
            SetEvent(session->_startupEvent);
        }

        HRESULT Startup(IUnknown* unknown)
        {
            HRESULT hr = unknown->QueryInterface(IID_ICorDebug, reinterpret_cast<void**>(&_debug));
            if (FAILED(hr))
            {
                return hr;
            }

            hr = _debug->Initialize();
            if (FAILED(hr))
            {
                return hr;
            }

            _callback = new ManagedCallback(_installFilter, _exitEvent);
            hr = _debug->SetManagedHandler(_callback);
            if (FAILED(hr))
            {
                return hr;
            }

            return _debug->DebugActiveProcess(_processId, FALSE, &_process);
        }

        DebugShim& _shim;
        bool _installFilter;
        DWORD _processId;
        void* _unregisterToken;
        ICorDebug* _debug;
        ICorDebugProcess* _process;
        ManagedCallback* _callback;
        HANDLE _startupEvent;
        HANDLE _exitEvent;
        std::atomic<HRESULT> _startupResult;
    };

    std::wstring Quote(const wchar_t* value)
    {
        return L"\"" + std::wstring(value) + L"\"";
    }
}

int wmain(int argc, wchar_t** argv)
{
    if (argc < 5)
    {
        std::fwprintf(
            stderr,
            L"Usage: RuntimeExceptionFilterDriver <dbgshim> <host> <benchmark.dll> <iterations> [samples] [baseline|filtered|both]\n");
        return 1;
    }

    int iterations = _wtoi(argv[4]);
    int samples = argc > 5 ? _wtoi(argv[5]) : 7;
    if (iterations <= 0 || samples <= 0)
    {
        std::fwprintf(stderr, L"Iterations and samples must be positive.\n");
        return 1;
    }

    std::wstring scenario = argc > 6 ? argv[6] : L"both";
    if (scenario != L"baseline" && scenario != L"filtered" && scenario != L"both")
    {
        std::fwprintf(stderr, L"Scenario must be baseline, filtered, or both.\n");
        return 1;
    }

    DebugShim shim;
    if (!shim.Load(argv[1]))
    {
        return 1;
    }

    std::printf("scenario,depth,iterations,median_ms,ns_per_throw,throws_per_second,legacy_expected_filter_evaluations,optimized_expected_filter_evaluations,evaluation_reduction_percent\n");
    std::fflush(stdout);

    for (bool filtered : { false, true })
    {
        if ((scenario == L"baseline" && filtered) ||
            (scenario == L"filtered" && !filtered))
        {
            continue;
        }

        std::wstring command =
            Quote(argv[2]) + L" " +
            Quote(argv[3]) + L" " +
            (filtered ? L"filtered" : L"baseline") + L" --no-header " +
            std::to_wstring(iterations) + L" " +
            std::to_wstring(samples);

        DebugSession session(shim, filtered);
        if (!session.Run(command, iterations, samples))
        {
            return 1;
        }
    }

    return 0;
}
