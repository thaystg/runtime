// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB.H
//

#ifndef __MONO_DEBUGGER_CORDB_H__
#define __MONO_DEBUGGER_CORDB_H__

#include "cor.h"
#include "cordebug.h"
#include "corhdr.h"
#include "xcordebug.h"

#include <mono/component/debugger-protocol.h>
#include <mono/utils/mono-publib.h>
#include <shash.h>

#include "arraylist.h"
#include "utsem.h"
#include "ex.h"
#include "log.h"

#ifdef HOST_WIN32
#include <windows.h>
#include <ws2tcpip.h>
#define DIR_SEPARATOR '\\'
#else
#define DIR_SEPARATOR '/'
#endif

#define return_if_nok(error)                                                                                           \
    do                                                                                                                 \
    {                                                                                                                  \
        if (!is_ok((error)))                                                                                           \
            return S_FALSE;                                                                                            \
    } while (0)

//#define _DEV_LOG_
#ifdef _DEV_LOG_
    #define LOG_METHOD_ENTRY if (conn) conn->logMethodEntry(__FUNCSIG__)
    #define LOG_METHOD_ENTRY
#else
    #define LOG_METHOD_ENTRY
#endif

#ifdef _DEBUG
#define LOGGING
#endif

#ifdef TARGET_AMD64
#define POS_RSP 0x98
#else
#define POS_RSP 0 //TODO fix for other platforms
#endif


#define CreateProcess CreateProcessW

class Socket;
class Cordb;
class CordbProcess;
class CordbAppDomain;
class CordbAssembly;
class CordbModule;
class CordbCode;
class CordbThread;
class CordbFunction;
class CordbStepper;
class RegMeta;
class CordbRegisterSet;
class CordbClass;
class CordbNativeFrame;
class CordbAppDomainEnum;
class CordbTypeEnum;
class CordbBlockingObjectEnum;
class CordbFunctionBreakpoint;
class CordbEval;
class CordbType;
class CordbStackWalk;
class CordbChainEnum;
class CordbFrameEnum;

#define MONO_TYPE_NAME_FORMAT_FULL_NAME 2

enum CordbTypeKind {
    CordbTypeKindSimpleType,
    CordbTypeKindClassType,
    CordbTypeKindArrayType,
    CordbTypeKindTotal
};

class ReceivedReplyPacket
{
    int             error;
    int             error_2;
    int             id;
    MdbgProtBuffer* buf;

public:
    ReceivedReplyPacket(int error, int error_2, int id, MdbgProtBuffer* buf);
    ~ReceivedReplyPacket();
    MdbgProtBuffer* Buffer()
    {
        return buf;
    }
    int Error()
    {
        return error;
    }
    int Error2()
    {
        return error_2;
    }
    int Id()
    {
        return id;
    }
};
template <typename KEY, typename VALUE>
class MapSHashWithRemoveThreadSafe : public MapSHashWithRemove<KEY, VALUE>
{
    UTSemReadWrite*     m_pSemReadWrite;
public:
    MapSHashWithRemoveThreadSafe()
    {
        m_pSemReadWrite  = new UTSemReadWrite();
        m_pSemReadWrite->Init();
    }
    ~MapSHashWithRemoveThreadSafe()
    {
        delete m_pSemReadWrite;
    }
    void Add(KEY key, VALUE value)
    {
        m_pSemReadWrite->LockWrite();
        MapSHashWithRemove::Add(key, value);
        m_pSemReadWrite->UnlockWrite();
    }
    BOOL Lookup(KEY key, VALUE* pValue) const
    {
        m_pSemReadWrite->LockRead();
        BOOL ret = MapSHashWithRemove::Lookup(key, pValue);
        m_pSemReadWrite->UnlockRead();
        return ret;
    }
    void Remove(KEY key)
    {
        m_pSemReadWrite->LockWrite();
        MapSHashWithRemove::Remove(key);
        m_pSemReadWrite->UnlockWrite();
    }
};
class ArrayListThreadSafe : public ArrayList
{
    UTSemReadWrite*     m_pSemReadWrite;
public:
    ArrayListThreadSafe()
    {
        m_pSemReadWrite  = new UTSemReadWrite();
        m_pSemReadWrite->Init();
    }
    ~ArrayListThreadSafe()
    {
        delete m_pSemReadWrite;
    }
    HRESULT Append(void *element)
    {
        m_pSemReadWrite->LockWrite();
        HRESULT hr = ArrayList::Append(element);
        m_pSemReadWrite->UnlockWrite();
        return hr;
    }

    PTR_VOID Get(DWORD index) const
    {
        m_pSemReadWrite->LockRead();
        PTR_VOID ptr = ArrayList::Get(index);
        m_pSemReadWrite->UnlockRead();
        return ptr;
    }

    void Set(DWORD index, PTR_VOID element)
    {
        m_pSemReadWrite->LockWrite();
        ArrayList::Set(index, element);
        m_pSemReadWrite->UnlockWrite();
    }
    /*void Clear()
    {
        m_pSemReadWrite->LockWrite();
        ArrayList::Clear();
        m_pSemReadWrite->UnlockWrite();
    }*/
};

class Connection
{
    Socket*       m_socket;
    CordbProcess* m_pProcess;
    Cordb*        m_pCordb;
    MapSHashWithRemoveThreadSafe<int, ReceivedReplyPacket*> m_pReceiveReplies;
    ArrayListThreadSafe*    m_pReceivedPacketsToProcess;
    void          ProcessPacketInternal(MdbgProtBuffer* recvbuf);
    void          ProcessPacketFromQueue();
    void          EnableEvent(MdbgProtEventKind eventKind);
    void          SendPacket(MdbgProtBuffer& sendbuf);
    int           ProcessPacket(bool is_answer = false);

public:
#ifdef _DEV_LOG_
    FILE *fptr; //mylog
    void logMethodEntry(const char* methodName)
    {
        fprintf(fptr, "%s - %d\n", methodName, GetCurrentThreadId());
        fflush(fptr);
    }
#endif    
    CordbProcess* GetProcess() const
    {
        return m_pProcess;
    }
    Cordb* GetCordb() const
    {
        return m_pCordb;
    }
    Connection(CordbProcess* proc, Cordb* cordb);
    ~Connection();

    void LoopSendReceive();
    void CloseConnection();
    void StartConnection();
    void TransportHandshake();
    void Receive();

    int                  SendEvent(int cmd_set, int cmd, MdbgProtBuffer* sendbuf);
    ReceivedReplyPacket* GetReplyWithError(int cmdId);
    ReceivedReplyPacket* TryGetReplyWithError(int cmdId);
    CordbAppDomain*      GetCurrentAppDomain();
};

class CordbBaseMono
{
protected:
    Connection* conn;
    ULONG       m_cRef;         // Ref count.
public:
    CordbBaseMono(Connection* conn);
    virtual ~CordbBaseMono();
    void                SetConnection(Connection* conn);
    ULONG               BaseAddRef(void);
    ULONG               BaseRelease(void);
    ULONG               InternalAddRef(void);
    ULONG               InternalRelease(void);
    virtual const char* GetClassName()
    {
        return "CordbBaseMono";
    }
};

class Cordb : public ICorDebug, public ICorDebugRemote, public CordbBaseMono
{
    ICorDebugManagedCallback* m_pCallback;
    CordbProcess*             m_pProcess;
    DWORD                     m_nPort;
public:
    DWORD Port()
    {
        return m_nPort;
    }
    ICorDebugManagedCallback* GetCallback() const
    {
        return m_pCallback;
    }
    Cordb(DWORD port);
    ULONG STDMETHODCALLTYPE AddRef(void)
    {
        return (BaseAddRef());
    }
    ULONG STDMETHODCALLTYPE Release(void)
    {
        return (BaseRelease());
    }
    const char* GetClassName()
    {
        return "Cordb";
    }
    ~Cordb();

    HRESULT STDMETHODCALLTYPE Initialize(void);

    HRESULT STDMETHODCALLTYPE Terminate(void);

    HRESULT STDMETHODCALLTYPE SetManagedHandler(ICorDebugManagedCallback* pCallback);

    HRESULT STDMETHODCALLTYPE SetUnmanagedHandler(ICorDebugUnmanagedCallback* pCallback);

    HRESULT STDMETHODCALLTYPE CreateProcess(LPCWSTR                    lpApplicationName,
                          LPWSTR                     lpCommandLine,
                          LPSECURITY_ATTRIBUTES      lpProcessAttributes,
                          LPSECURITY_ATTRIBUTES      lpThreadAttributes,
                          BOOL                       bInheritHandles,
                          DWORD                      dwCreationFlags,
                          PVOID                      lpEnvironment,
                          LPCWSTR                    lpCurrentDirectory,
                          LPSTARTUPINFOW             lpStartupInfo,
                          LPPROCESS_INFORMATION      lpProcessInformation,
                          CorDebugCreateProcessFlags debuggingFlags,
                          ICorDebugProcess**         ppProcess);

    HRESULT STDMETHODCALLTYPE DebugActiveProcess(DWORD id, BOOL win32Attach, ICorDebugProcess** ppProcess);
    HRESULT STDMETHODCALLTYPE EnumerateProcesses(ICorDebugProcessEnum** ppProcess);

    HRESULT STDMETHODCALLTYPE GetProcess(DWORD dwProcessId, ICorDebugProcess** ppProcess);

    HRESULT STDMETHODCALLTYPE CanLaunchOrAttach(DWORD dwProcessId, BOOL win32DebuggingEnabled);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);
    HRESULT STDMETHODCALLTYPE CreateProcessEx(ICorDebugRemoteTarget*     pRemoteTarget,
                            LPCWSTR                    lpApplicationName,
                            _In_ LPWSTR                lpCommandLine,
                            LPSECURITY_ATTRIBUTES      lpProcessAttributes,
                            LPSECURITY_ATTRIBUTES      lpThreadAttributes,
                            BOOL                       bInheritHandles,
                            DWORD                      dwCreationFlags,
                            PVOID                      lpEnvironment,
                            LPCWSTR                    lpCurrentDirectory,
                            LPSTARTUPINFOW             lpStartupInfo,
                            LPPROCESS_INFORMATION      lpProcessInformation,
                            CorDebugCreateProcessFlags debuggingFlags,
                            ICorDebugProcess**         ppProcess);

    HRESULT STDMETHODCALLTYPE DebugActiveProcessEx(ICorDebugRemoteTarget* pRemoteTarget,
                                 DWORD                  dwProcessId,
                                 BOOL                   fWin32Attach,
                                 ICorDebugProcess**     ppProcess);
};

#define CHECK_ERROR_RETURN_FALSE(localbuf)                                                                                             \
    do                                                                                                                                 \
    {                                                                                                                                  \
        if (localbuf->Error() > 0 || localbuf->Error2() > 0)                                                                           \
        {\
            if (localbuf->Buffer()->end > localbuf->Buffer()->p) {\
                char *error_msg = m_dbgprot_decode_string(localbuf->Buffer()->p, &localbuf->Buffer()->p, localbuf->Buffer()->end);         \
              LOG((LF_CORDB, LL_INFO100000, "ERROR RECEIVED - %s\n", error_msg));                                                         \
                free(error_msg);                                                                                                           \
            }\
            else {\
              LOG((LF_CORDB, LL_INFO100000, "ERROR RECEIVED - %d - %d\n", localbuf->Error(), localbuf->Error2()));                                                         \
            }\
            EX_THROW(HRException, (E_FAIL));                                                                                           \
        }                                                                                                                              \
    } while (0)

#endif
