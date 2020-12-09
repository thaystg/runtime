#ifndef __MONO_DEBUGGER_CORDB_H__
#define __MONO_DEBUGGER_CORDB_H__

#define MONO_DLL_EXPORT

#include <mono\utils\mono-publib.h>
#include <mono\mini\debugger-protocol.h>
#include <mono\mini\debugger-engine.h>
#include <stdio.h>


#ifdef WIN32
#include <windows.h>
#endif
#include "cor.h"
#include "corhdr.h"
#include "cordebug.h"
#include "xcordebug.h"
#include <WS2tcpip.h>

#define return_if_nok(error) do { if (!is_ok ((error))) return S_FALSE; } while (0)

#define dbg_lock() mono_os_mutex_lock (&debug_mutex.m);
#define dbg_unlock() mono_os_mutex_unlock (&debug_mutex.m);
static MonoCoopMutex debug_mutex;

class Cordb;
class CordbProcess;
class CordbAppDomain;
class CordbAssembly;
class CordbCode;
class CordbThread;
class CordbFunction;
class CordbStepper;
class CordbSymbol;
class CordbRegisteSet;

int convert_mono_type_2_icordbg_size(int type);

struct M128BIT {
    uint64_t Low;
    int64_t  High;
};


struct AMD64_XMM_SAVE_AREA32 {
    uint16_t ControlWord;
    uint16_t StatusWord;
    uint8_t  TagWord;
    uint8_t  Reserved1;
    uint16_t ErrorOpcode;
    uint32_t ErrorOffset;
    uint16_t ErrorSelector;
    uint16_t Reserved2;
    uint32_t DataOffset;
    uint16_t DataSelector;
    uint16_t Reserved3;
    uint32_t MxCsr;
    uint32_t MxCsr_Mask;
    M128BIT  FloatRegisters[8];
    M128BIT  XmmRegisters[16];
    uint8_t  Reserved4[96];
};

struct AMD64_CONTEXT {

    //
    // Register parameter home addresses.
    //

    uint64_t P1Home;
    uint64_t P2Home;
    uint64_t P3Home;
    uint64_t P4Home;
    uint64_t P5Home;
    uint64_t P6Home;

    //
    // Control flags.
    //

    uint32_t ContextFlags;
    uint32_t MxCsr;

    //
    // Segment Registers and processor flags.
    //

    uint16_t SegCs;
    uint16_t SegDs;
    uint16_t SegEs;
    uint16_t SegFs;
    uint16_t SegGs;
    uint16_t SegSs;
    uint32_t EFlags;

    //
    // Debug registers
    //

    uint64_t Dr0;
    uint64_t Dr1;
    uint64_t Dr2;
    uint64_t Dr3;
    uint64_t Dr6;
    uint64_t Dr7;

    //
    // Integer registers.
    //

    uint64_t Rax;
    uint64_t Rcx;
    uint64_t Rdx;
    uint64_t Rbx;
    uint64_t Rsp;
    uint64_t Rbp;
    uint64_t Rsi;
    uint64_t Rdi;
    uint64_t R8;
    uint64_t R9;
    uint64_t R10;
    uint64_t R11;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;

    //
    // Program counter.
    //

    uint64_t Rip;

    //
    // Floating point state.
    //

    union {
        AMD64_XMM_SAVE_AREA32 FltSave;
        struct {
            M128BIT Header[2];
            M128BIT Legacy[8];
            M128BIT Xmm0;
            M128BIT Xmm1;
            M128BIT Xmm2;
            M128BIT Xmm3;
            M128BIT Xmm4;
            M128BIT Xmm5;
            M128BIT Xmm6;
            M128BIT Xmm7;
            M128BIT Xmm8;
            M128BIT Xmm9;
            M128BIT Xmm10;
            M128BIT Xmm11;
            M128BIT Xmm12;
            M128BIT Xmm13;
            M128BIT Xmm14;
            M128BIT Xmm15;
        };
    };

    //
    // Vector registers.
    //

    M128BIT VectorRegister[26];
    uint64_t VectorControl;

    //
    // Special debug control registers.
    //

    uint64_t DebugControl;
    uint64_t LastBranchToRip;
    uint64_t LastBranchFromRip;
    uint64_t LastExceptionToRip;
    uint64_t LastExceptionFromRip;
};

class Cordb : public ICorDebug, public ICorDebugRemote
{
public:
    GPtrArray* breakpoints;
    GPtrArray* threads;
    GPtrArray* functions;
    ICorDebugManagedCallback* pCallback;
    Cordb();

    CordbFunction* findFunction(int id);
    CordbFunction* findFunctionByToken(int token);
    HRESULT Initialize(void);

    HRESULT Terminate(void);

    HRESULT SetManagedHandler(
        /* [in] */ ICorDebugManagedCallback* pCallback);

    HRESULT SetUnmanagedHandler(
        /* [in] */ ICorDebugUnmanagedCallback* pCallback);

    HRESULT CreateProcess(
        /* [in] */ LPCWSTR lpApplicationName,
        /* [in] */ LPWSTR lpCommandLine,
        /* [in] */ LPSECURITY_ATTRIBUTES lpProcessAttributes,
        /* [in] */ LPSECURITY_ATTRIBUTES lpThreadAttributes,
        /* [in] */ BOOL bInheritHandles,
        /* [in] */ DWORD dwCreationFlags,
        /* [in] */ PVOID lpEnvironment,
        /* [in] */ LPCWSTR lpCurrentDirectory,
        /* [in] */ LPSTARTUPINFOW lpStartupInfo,
        /* [in] */ LPPROCESS_INFORMATION lpProcessInformation,
        /* [in] */ CorDebugCreateProcessFlags debuggingFlags,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT DebugActiveProcess(
        /* [in] */ DWORD id,
        /* [in] */ BOOL win32Attach,
        /* [out] */ ICorDebugProcess** ppProcess);
    HRESULT EnumerateProcesses(
        /* [out] */ ICorDebugProcessEnum** ppProcess);

    HRESULT GetProcess(
        /* [in] */ DWORD dwProcessId,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT CanLaunchOrAttach(
        /* [in] */ DWORD dwProcessId,
        /* [in] */ BOOL win32DebuggingEnabled);
    HRESULT QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

    ULONG AddRef(void);
    ULONG Release(void);
    HRESULT CreateProcessEx(
        /* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
        /* [in] */ LPCWSTR lpApplicationName,
        /* [annotation][in] */
        _In_  LPWSTR lpCommandLine,
        /* [in] */ LPSECURITY_ATTRIBUTES lpProcessAttributes,
        /* [in] */ LPSECURITY_ATTRIBUTES lpThreadAttributes,
        /* [in] */ BOOL bInheritHandles,
        /* [in] */ DWORD dwCreationFlags,
        /* [in] */ PVOID lpEnvironment,
        /* [in] */ LPCWSTR lpCurrentDirectory,
        /* [in] */ LPSTARTUPINFOW lpStartupInfo,
        /* [in] */ LPPROCESS_INFORMATION lpProcessInformation,
        /* [in] */ CorDebugCreateProcessFlags debuggingFlags,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT DebugActiveProcessEx(
        /* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
        /* [in] */ DWORD dwProcessId,
        /* [in] */ BOOL fWin32Attach,
        /* [out] */ ICorDebugProcess** ppProcess);
};

class Connection
{
	SOCKET connect_socket;
	CordbProcess* ppProcess;
	Cordb* ppCordb;
    bool is_answer_pending;
public:
    CordbAppDomain* pCorDebugAppDomain;
	GHashTable* received_replies;
	GPtrArray* received_replies_to_process;
	Connection(CordbProcess* proc, Cordb* cordb);
	void loop_send_receive();
	void process_packet_internal(Buffer* recvbuf);
	void process_packet_from_queue();
	void enable_event(EventKind eventKind);
	void close_connection();
	void start_connection();
	void transport_handshake();
    void receive();
	void send_packet(Buffer& sendbuf);
	void receive_packet(Buffer& b, int len);
	void receive_header(Header* header);
	int send_event(int cmd_set, int cmd, Buffer* sendbuf);
	int process_packet(bool is_answer = false);
    Buffer* get_answer(int cmdId);
    CordbThread* findThread(GPtrArray *threads, long thread_id);
};

static int log_level = 10;
static FILE* log_file = fopen("c:\\thays\\example.txt", "a+");

#define DEBUG(level,s) do { if (G_UNLIKELY ((level) <= log_level)) { s; fflush (log_file); } } while (0)
#define DEBUG_PRINTF(level, ...) do { if (G_UNLIKELY ((level) <= log_level)) { fprintf (log_file, __VA_ARGS__); fflush (log_file); } } while (0)

#endif
