// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//*****************************************************************************
// DbgShim.cpp
//
// dbgshim is responsible to start the debuggee process with debug parameters and to
// load the correct mscordbi that will connect with mono runtime.
//*****************************************************************************

#include <utilcode.h>

#include "dbgshim.h"

#include "palclr.h"
#if defined(TARGET_WINDOWS)
#include <libloaderapi.h>
#else
#include <dlfcn.h>
#define putenv _putenv
#endif

#ifndef MAX_LONGPATH
#define MAX_LONGPATH   1024
#endif

#ifdef HOST_UNIX
#define INITIALIZE_SHIM { if (PAL_InitializeDLL() != 0) return E_FAIL; }
#else
#define INITIALIZE_SHIM
#endif

// Contract for public APIs. These must be NOTHROW.
#define PUBLIC_CONTRACT \
    INITIALIZE_SHIM \
    CONTRACTL \
    { \
        NOTHROW; \
    } \
    CONTRACTL_END;

// Functions that we'll look for in the loaded Mscordbi module.
typedef HRESULT (STDAPICALLTYPE *FPCoreCLRCreateCordbObject)(
    int iDebuggerVersion,
    DWORD pid,
    HMODULE hmodTargetCLR,
    IUnknown **ppCordb);

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CreateProcessForLaunch(
    _In_ LPWSTR lpCommandLine,
    _In_ BOOL bSuspendProcess,
    _In_ LPVOID lpEnvironment,
    _In_ LPCWSTR lpCurrentDirectory,
    _Out_ PDWORD pProcessId,
    _Out_ HANDLE *pResumeHandle)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
ResumeProcess(
    _In_ HANDLE hResumeHandle)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CloseResumeHandle(
    _In_ HANDLE hResumeHandle)
{
    return S_OK;
}


HRESULT CreateCoreDbg(HMODULE hDBIModule, DWORD portId, int iDebuggerVersion, IUnknown **ppCordb)
{
    HRESULT hr = S_OK;

#if defined(TARGET_WINDOWS)
    FPCoreCLRCreateCordbObject fpCreate =
        (FPCoreCLRCreateCordbObject)GetProcAddress(hDBIModule, "CoreCLRCreateCordbObject");
#else
    FPCoreCLRCreateCordbObject fpCreate = (FPCoreCLRCreateCordbObject)dlsym (hDBIModule, "CoreCLRCreateCordbObject");
#endif

    if (fpCreate == NULL)
    {
        return CORDBG_E_INCOMPATIBLE_PROTOCOL;
    }

    return fpCreate(iDebuggerVersion, portId, NULL, ppCordb);

    return hr;
}

static IUnknown* pCordb = NULL;

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
RegisterForRuntimeStartup(
    _In_ DWORD dwPort,
    _In_ PSTARTUP_CALLBACK pfnCallback,
    _In_ PVOID parameter,
    _Out_ PVOID *ppUnregisterToken)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
RegisterForRuntimeStartupEx(
    _In_ DWORD dwProcessId,
    _In_ LPCWSTR szApplicationGroupId,
    _In_ PSTARTUP_CALLBACK pfnCallback,
    _In_ PVOID parameter,
    _Out_ PVOID *ppUnregisterToken)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
UnregisterForRuntimeStartup(
    _In_ PVOID pUnregisterToken)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
GetStartupNotificationEvent(
    _In_ DWORD debuggeePID,
    _Out_ HANDLE* phStartupEvent)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
EnumerateCLRs(DWORD debuggeePID,
    _Out_ HANDLE** ppHandleArrayOut,
    _Out_ LPWSTR** ppStringArrayOut,
    _Out_ DWORD* pdwArrayLengthOut)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CloseCLREnumeration(
    _In_ HANDLE* pHandleArray,
    _In_ LPWSTR* pStringArray,
    _In_ DWORD dwArrayLength)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CreateVersionStringFromModule(
    _In_ DWORD pidDebuggee,
    _In_ LPCWSTR szModuleName,
    _Out_writes_to_opt_(cchBuffer, *pdwLength) LPWSTR pBuffer,
    _In_ DWORD cchBuffer,
    _Out_ DWORD* pdwLength)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CreateDebuggingInterfaceFromVersionEx(
    _In_ int iDebuggerVersion,
    _In_ LPCWSTR szDebuggeeVersion,
    _Out_ IUnknown ** ppCordb)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT
HRESULT
CreateDebuggingInterfaceFromVersion2(
    _In_ int iDebuggerVersion,
    _In_ LPCWSTR szDebuggeeVersion,
    _In_ LPCWSTR szApplicationGroupId,
    _Out_ IUnknown ** ppCordb)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT HRESULT
CreateDebuggingInterfaceFromVersion(
    _In_ LPCWSTR szDebuggeeVersion,
    _Out_ IUnknown ** ppCordb)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT
HRESULT
RegisterForRuntimeStartup3(
    _In_ DWORD dwProcessId,
    _In_ LPCWSTR szApplicationGroupId,
    _In_ IUnknown* pLibraryProvider,
    _In_ PSTARTUP_CALLBACK pfnCallback,
    _In_ PVOID parameter,
    _Out_ PVOID *ppUnregisterToken)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT
HRESULT
CreateDebuggingInterfaceFromVersion3(
    _In_ int iDebuggerVersion,
    _In_ LPCWSTR szDebuggeeVersion,
    _In_ LPCWSTR szApplicationGroupId,
    _In_ IUnknown* pLibraryProvider,
    _Out_ IUnknown ** ppCordb)
{
    return S_OK;
}

MONO_EXTERN_C
MONO_API_EXPORT
HRESULT
RegisterForRuntimeStartupRemotePort(
    _In_ DWORD dwRemotePortId,
    _Out_ IUnknown ** ppCordb)
{
    if (pCordb != NULL)
        return S_OK;

    HRESULT hr = S_OK;
    HMODULE hMod = NULL;

    char* msCorDbiPath = getenv("MSCORDBI_PATH");
    if (msCorDbiPath == NULL)
        msCorDbiPath = (char*)"T:\\thays\\runtime_icordbg\\runtime\\artifacts\\bin\\mono\\windows.x64.Debug\\mscordbi.dll";
#ifdef TARGET_WINDOWS
    hMod = LoadLibraryA(msCorDbiPath);
#else
    hMod = dlopen(msCorDbiPath, RTLD_LAZY);
#endif
    if (hMod == NULL)
    {
        hr = CORDBG_E_DEBUG_COMPONENT_MISSING;
        return hr;
    }

    hr = CreateCoreDbg(hMod, dwRemotePortId, 0, &pCordb);
    *ppCordb = pCordb;
    return S_OK;
}


