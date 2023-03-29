// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-PROCESS.CPP
//

#include <cordb-appdomain.h>
#include <cordb-assembly.h>
#include <cordb-breakpoint.h>
#include <cordb-class.h>
#include <cordb-code.h>
#include <cordb-eval.h>
#include <cordb-function.h>
#include <cordb-process.h>
#include <cordb-thread.h>
#include <cordb-stepper.h>
#include <cordb-type.h>
#include <cordb.h>

using namespace std;

CordbProcess::CordbProcess(Cordb* cordb) : CordbBaseMono(NULL)
{
    LOG_METHOD_ENTRY;
    m_pAppDomainEnum = NULL;
    m_pBreakpoints   = new ArrayListThreadSafe();
    m_pThreads       = new ArrayListThreadSafe();
    m_pFunctions     = new ArrayListThreadSafe();
    m_pModules       = new ArrayListThreadSafe();
    m_pAddDomains    = new ArrayListThreadSafe();
    m_pSteppers      = new ArrayListThreadSafe();
    m_pCompletedEval = new ArrayListThreadSafe();
    this->m_pCordb   = cordb;
    m_bIsJustMyCode  = false;
    m_pTypeMapArray  = new ArrayListThreadSafe();
    for (DWORD i = 0; i < CordbTypeKindTotal; i++)
    {
        m_pTypeMapArray->Append(new MapSHashWithRemove<long, CordbType*>());
    }
    suspend_count = 0;
}

void CordbProcess::SetPaused()
{
    LOG_METHOD_ENTRY;
    int id = dbg_rt_atomic_inc_int32_t ((volatile int32_t *)&suspend_count);
}

int CordbProcess::GetObjectIdByAddress(CORDB_ADDRESS address)
{
    LOG_METHOD_ENTRY;
    MdbgProtBuffer localbuf;
    m_dbgprot_buffer_init(&localbuf, 128);
    m_dbgprot_buffer_add_long(&localbuf, address);
    int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_GET_OBJECT_ID_BY_ADDRESS, &localbuf);
    m_dbgprot_buffer_free(&localbuf);

    ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
    CHECK_ERROR_RETURN_FALSE(received_reply_packet);
    MdbgProtBuffer* pReply = received_reply_packet->Buffer();
    return m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
}

CordbProcess::~CordbProcess()
{
    LOG_METHOD_ENTRY;
    if (m_pAppDomainEnum)
        m_pAppDomainEnum->InternalRelease();

    for (DWORD i = 0; i < m_pBreakpoints->GetCount(); i++)
    {
        CordbFunctionBreakpoint* breakpoint = (CordbFunctionBreakpoint*)m_pBreakpoints->Get(i);
        if (breakpoint)
            breakpoint->InternalRelease();
    }

    for (DWORD i = 0; i < m_pSteppers->GetCount(); i++)
    {
        CordbStepper* stepper = (CordbStepper*)m_pSteppers->Get(i);
        if (stepper)
            stepper->InternalRelease();
    }

    for (DWORD i = 0; i < m_pThreads->GetCount(); i++)
    {
        CordbThread* thread = (CordbThread*)m_pThreads->Get(i);
        thread->InternalRelease();
    }

    for (DWORD i = 0; i < m_pFunctions->GetCount(); i++)
    {
        CordbFunction* function = (CordbFunction*)m_pFunctions->Get(i);
        function->InternalRelease();
    }

    for (DWORD i = 0; i < m_pAddDomains->GetCount(); i++)
    {
        CordbAppDomain* appdomain = (CordbAppDomain*)m_pAddDomains->Get(i);
        appdomain->InternalRelease();
    }

    for (DWORD i = 0; i < m_pModules->GetCount(); i++)
    {
        CordbModule* module = (CordbModule*)m_pModules->Get(i);
        module->InternalRelease();
    }

    for (DWORD i = 0; i < m_pCompletedEval->GetCount(); i++)
    {
        CordbEval* eval = (CordbEval*)m_pCompletedEval->Get(i);
        eval->InternalRelease();
    }

    for (MapSHashWithRemove<int, CordbEval*>::Iterator iter = m_pPendingEval.Begin(), end = m_pPendingEval.End(); iter != end; iter++)
    {
        iter->Value()->InternalRelease();
    }

    for (MapSHashWithRemove<int, CordbClass*>::Iterator iter = m_classMap.Begin(), end = m_classMap.End(); iter != end; iter++)
    {
        iter->Value()->InternalRelease();
    }
    

    for (DWORD i = 0; i < m_pTypeMapArray->GetCount(); i++)
    {
        MapSHashWithRemove<long, CordbType*>* typeMap = (MapSHashWithRemove<long, CordbType*>*)m_pTypeMapArray->Get(i);
        for (MapSHashWithRemove<long, CordbType*>::Iterator iter = typeMap->Begin(), end = typeMap->End(); iter != end; iter++)
        {
            if (iter->Value())
                iter->Value()->InternalRelease();
        }
    }

    delete m_pBreakpoints;
    delete m_pThreads;
    delete m_pFunctions;
    delete m_pModules;
    delete m_pCompletedEval;
    delete m_pAddDomains;
    delete m_pTypeMapArray;
    delete conn;
}

void CordbProcess::CheckCompletedEval()
{
    for (DWORD i = 0; i < m_pCompletedEval->GetCount(); i++)
    {
        CordbEval* eval = (CordbEval*)m_pCompletedEval->Get(i);
        eval->EvalComplete();
    }
}

bool CordbProcess::CheckPendingEval(int commandId, MdbgProtBuffer* pReply)
{
    CordbEval *ret = NULL;
    if (m_pPendingEval.Lookup(commandId, &ret)) {
        LOG_METHOD_ENTRY;
        ret->SetResponse(pReply);
        m_pCompletedEval->Append(ret);
        return true;
    }
    return false;
}

HRESULT CordbProcess::EnumerateLoaderHeapMemoryRegions(ICorDebugMemoryRangeEnum** ppRanges)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateLoaderHeapMemoryRegions - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnableGCNotificationEvents(BOOL fEnable)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnableGCNotificationEvents - NOT IMPLEMENTED\n"));

    return S_OK;
}

HRESULT CordbProcess::EnableExceptionCallbacksOutsideOfMyCode(BOOL enableExceptionsOutsideOfJMC)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000,
         "CordbProcess - EnableExceptionCallbacksOutsideOfMyCode - "
         "NOT IMPLEMENTED\n"));

    return S_OK;
}

HRESULT CordbProcess::SetWriteableMetadataUpdateMode(WriteableMetadataUpdateMode flags)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetWriteableMetadataUpdateMode - NOT IMPLEMENTED\n"));

    return S_OK;
}

HRESULT CordbProcess::GetGCHeapInformation(COR_HEAPINFO* pHeapInfo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetGCHeapInformation - NOT IMPLEMENTED\n"));

    return S_OK;
}

HRESULT CordbProcess::EnumerateHeap(ICorDebugHeapEnum** ppObjects)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateHeap - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT
CordbProcess::EnumerateHeapRegions(ICorDebugHeapSegmentEnum** ppRegions)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateHeapRegions - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetObject(CORDB_ADDRESS addr, ICorDebugObjectValue** pObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetObject - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnumerateGCReferences(BOOL enumerateWeakReferences, ICorDebugGCReferenceEnum** ppEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateGCReferences - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnumerateHandles(CorGCReferenceType types, ICorDebugGCReferenceEnum** ppEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateHandles - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetTypeID(CORDB_ADDRESS obj, COR_TYPEID* pId)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetTypeID - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetTypeForTypeID(COR_TYPEID id, ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetTypeForTypeID - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetArrayLayout(COR_TYPEID id, COR_ARRAY_LAYOUT* pLayout)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetArrayLayout - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetTypeLayout(COR_TYPEID id, COR_TYPE_LAYOUT* pLayout)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetTypeLayout - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetTypeFields(COR_TYPEID id, ULONG32 celt, COR_FIELD fields[], ULONG32* pceltNeeded)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetTypeFields - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnableNGENPolicy(CorDebugNGENPolicy ePolicy)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnableNGENPolicy - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT
CordbProcess::Filter(const BYTE                             pRecord[],
                     DWORD                                  countBytes,
                     CorDebugRecordFormat                   format,
                     DWORD                                  dwFlags,
                     DWORD                                  dwThreadId,
                     ICorDebugManagedCallback*              pCallback,
                     /* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - Filter - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ProcessStateChanged(CorDebugStateChange eChange)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - ProcessStateChanged - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::SetEnableCustomNotification(ICorDebugClass* pClass, BOOL fEnable)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetEnableCustomNotification - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetID(DWORD* pdwProcessId)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetID - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetHandle(HPROCESS* phProcessHandle)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetHandle - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetThread(DWORD dwThreadId, ICorDebugThread** ppThread)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetThread - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnumerateObjects(ICorDebugObjectEnum** ppObjects)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnumerateObjects - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::IsTransitionStub(CORDB_ADDRESS address, BOOL* pbTransitionStub)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - IsTransitionStub - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::IsOSSuspended(DWORD threadID, BOOL* pbSuspended)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - IsOSSuspended - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetThreadContext(DWORD threadID, ULONG32 contextSize, BYTE context[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetThreadContext - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::SetThreadContext(DWORD threadID, ULONG32 contextSize, BYTE context[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetThreadContext - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ReadMemory(CORDB_ADDRESS address, DWORD size, BYTE buffer[], SIZE_T* read)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbProcess - ReadMemory - IMPLEMENTED\n"));
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_long(&localbuf, address);
        m_dbgprot_buffer_add_int(&localbuf, size);
        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_READ_MEMORY, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        int memoryReadSize = 0;
        uint8_t* memoryRead = m_dbgprot_decode_byte_array(pReply->p, &pReply->p, pReply->end, (int32_t*)&memoryReadSize);
        memcpy(buffer, (void*)memoryRead, memoryReadSize);
        if (read != NULL)
            *read = memoryReadSize;
        free(memoryRead);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT CordbProcess::WriteMemory(CORDB_ADDRESS address, DWORD size, BYTE buffer[], SIZE_T* written)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - WriteMemory - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ClearCurrentException(DWORD threadID)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - ClearCurrentException - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnableLogMessages(BOOL fOnOff)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - EnableLogMessages - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ModifyLogSwitch(
    /* [annotation][in] */
    _In_ WCHAR* pLogSwitchName,
    LONG        lLevel)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - ModifyLogSwitch - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT
CordbProcess::EnumerateAppDomains(ICorDebugAppDomainEnum** ppAppDomains)
{
    LOG_METHOD_ENTRY;
    if (!m_pAppDomainEnum)
    {
        m_pAppDomainEnum = new CordbAppDomainEnum(conn, this);
        m_pAppDomainEnum->InternalAddRef();
    }
    m_pAppDomainEnum->QueryInterface(IID_ICorDebugAppDomainEnum, (void**)ppAppDomains);
    LOG((LF_CORDB, LL_INFO1000000, "CordbProcess - EnumerateAppDomains - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetObject(ICorDebugValue** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetObject - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ThreadForFiberCookie(DWORD fiberCookie, ICorDebugThread** ppThread)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - ThreadForFiberCookie - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetHelperThreadID(DWORD* pThreadID)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "GetHelperThreadID - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetThreadForTaskID(TASKID taskid, ICorDebugThread2** ppThread)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetHelperThreadID - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetVersion(COR_VERSION* version)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetVersion - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::SetUnmanagedBreakpoint(CORDB_ADDRESS address, ULONG32 bufsize, BYTE buffer[], ULONG32* bufLen)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetUnmanagedBreakpoint - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::ClearUnmanagedBreakpoint(CORDB_ADDRESS address)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - ClearUnmanagedBreakpoint - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::SetDesiredNGENCompilerFlags(DWORD pdwFlags)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetDesiredNGENCompilerFlags - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetDesiredNGENCompilerFlags(DWORD* pdwFlags)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetDesiredNGENCompilerFlags - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::GetReferenceValueFromGCHandle(UINT_PTR handle, ICorDebugReferenceValue** pOutValue)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - GetReferenceValueFromGCHandle - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::QueryInterface(REFIID id, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugProcess)
    {
        *pInterface = static_cast<ICorDebugProcess*>(this);
    }
    else if (id == IID_ICorDebugController)
    {
        *pInterface = static_cast<ICorDebugController*>(static_cast<ICorDebugProcess*>(this));
    }
    else if (id == IID_ICorDebugProcess2)
    {
        *pInterface = static_cast<ICorDebugProcess2*>(this);
    }
    else if (id == IID_ICorDebugProcess3)
    {
        *pInterface = static_cast<ICorDebugProcess3*>(this);
    }
    else if (id == IID_ICorDebugProcess4)
    {
        *pInterface = static_cast<ICorDebugProcess4*>(this);
    }
    else if (id == IID_ICorDebugProcess5)
    {
        *pInterface = static_cast<ICorDebugProcess5*>(this);
    }
    else if (id == IID_ICorDebugProcess7)
    {
        *pInterface = static_cast<ICorDebugProcess7*>(this);
    }
    else if (id == IID_ICorDebugProcess8)
    {
        *pInterface = static_cast<ICorDebugProcess8*>(this);
    }
    else if (id == IID_ICorDebugProcess10)
    {
        *pInterface = static_cast<ICorDebugProcess10*>(this);
    }
    else if (id == IID_ICorDebugProcess11)
    {
        *pInterface = static_cast<ICorDebugProcess11*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugProcess*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT CordbProcess::Stop(DWORD dwTimeoutIgnored)
{
    LOG_METHOD_ENTRY;
    int id = dbg_rt_atomic_inc_int32_t ((volatile int32_t *)&suspend_count);
    if (id != 1)
        return S_OK;
    LOG((LF_CORDB, LL_INFO1000000, "CordbProcess - Stop - IMPLEMENTED\n"));
    MdbgProtBuffer sendbuf;
    m_dbgprot_buffer_init(&sendbuf, 128);
    conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_SUSPEND, &sendbuf);
    m_dbgprot_buffer_free(&sendbuf);
    return S_OK;
}

HRESULT CordbProcess::Continue(BOOL fIsOutOfBand)
{
    LOG_METHOD_ENTRY;
    int id = dbg_rt_atomic_dec_int32_t ((volatile int32_t *)&suspend_count);
    if (id != 0)
    {
        for (MapSHashWithRemove<int, CordbEval*>::Iterator iter = m_pPendingEval.Begin(), end = m_pPendingEval.End(); iter != end; iter++)
        {
            iter->Value()->SetContinued();
        }
        return S_OK;
    }

    CordbThread* ret = NULL;
    for (DWORD i = 0; i < m_pThreads->GetCount(); i++)
    {
        CordbThread* thread = (CordbThread*)m_pThreads->Get(i);
        thread->Reset();
    }
    for (DWORD i = 0; i < m_pCompletedEval->GetCount(); i++)
    {
        CordbEval* eval = (CordbEval*)m_pCompletedEval->Get(i);
        eval->InternalRelease();
    }
    m_pCompletedEval->Clear();
    for (MapSHashWithRemove<int, CordbEval*>::Iterator iter = m_pPendingEval.Begin(), end = m_pPendingEval.End(); iter != end; iter++)
    {
        iter->Value()->InternalRelease();
    }
    m_pPendingEval.RemoveAll();

    LOG((LF_CORDB, LL_INFO1000000, "CordbProcess - Continue - IMPLEMENTED\n"));
    MdbgProtBuffer sendbuf;
    m_dbgprot_buffer_init(&sendbuf, 128);
    conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_RESUME, &sendbuf);
    m_dbgprot_buffer_free(&sendbuf);
    return S_OK;
}

HRESULT CordbProcess::IsRunning(BOOL* pbRunning)
{
    LOG_METHOD_ENTRY;
    *pbRunning = true;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - IsRunning - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::HasQueuedCallbacks(ICorDebugThread* pThread, BOOL* pbQueued)
{
    LOG_METHOD_ENTRY;
    // conn->process_packet_from_queue();
    *pbQueued = false;
    LOG((LF_CORDB, LL_INFO1000000, "CordbProcess - HasQueuedCallbacks - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::EnumerateThreads(ICorDebugThreadEnum** ppThreads)
{
    LOG_METHOD_ENTRY;
    *ppThreads = new CordbThreadEnum(conn);
    (*ppThreads)->AddRef();
    return S_OK;
}

HRESULT
CordbProcess::SetAllThreadsDebugState(CorDebugThreadState state, ICorDebugThread* pExceptThisThread)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - SetAllThreadsDebugState - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::Detach(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - Detach - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT CordbProcess::Terminate(UINT exitCode)
{
    LOG_METHOD_ENTRY;
    MdbgProtBuffer sendbuf;
    m_dbgprot_buffer_init(&sendbuf, 128);
    m_dbgprot_buffer_add_int(&sendbuf, -1);
    conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_EXIT, &sendbuf);
    m_dbgprot_buffer_free(&sendbuf);
    return S_OK;
}

HRESULT
CordbProcess::CanCommitChanges(ULONG                             cSnapshots,
                               ICorDebugEditAndContinueSnapshot* pSnapshots[],
                               ICorDebugErrorInfoEnum**          pError)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - CanCommitChanges - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT
CordbProcess::CommitChanges(ULONG                             cSnapshots,
                            ICorDebugEditAndContinueSnapshot* pSnapshots[],
                            ICorDebugErrorInfoEnum**          pError)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbProcess - CommitChanges - NOT IMPLEMENTED\n"));
    return S_OK;
}

void CordbProcess::AddThread(CordbThread* thread)
{
    LOG_METHOD_ENTRY;
    //dbg_write_lock();
    m_pThreads->Append(thread);
    thread->InternalAddRef();
    //dbg_write_unlock();
}

void CordbProcess::AddFunction(CordbFunction* function)
{
    LOG_METHOD_ENTRY;
    //dbg_write_lock();
    m_pFunctions->Append(function);
    function->InternalAddRef();
    //dbg_write_unlock();
}

void CordbProcess::AddModule(CordbModule* module)
{
    LOG_METHOD_ENTRY;
    //dbg_write_lock();
    m_pModules->Append(module);
    module->InternalAddRef();
    //dbg_write_unlock();
}

void CordbProcess::AddAppDomain(CordbAppDomain* appDomain)
{
    LOG_METHOD_ENTRY;
    m_pAddDomains->Append(appDomain);
    appDomain->InternalAddRef();
}

void CordbProcess::AddPendingEval(int cmdId, CordbEval* eval)
{
    LOG_METHOD_ENTRY;
    m_pPendingEval.Add(cmdId, eval);
    eval->InternalAddRef();
}

void CordbProcess::AddBreakpoint(CordbFunctionBreakpoint* bp)
{
    LOG_METHOD_ENTRY;
    m_pBreakpoints->Append(bp);
    bp->InternalAddRef();
}

void CordbProcess::AddStepper(CordbStepper* step)
{
    LOG_METHOD_ENTRY;
    m_pSteppers->Append(step);
    step->InternalAddRef();
}

CordbClass* CordbProcess::FindOrAddClass(mdToken token, int type_id, int module_id)
{
    LOG_METHOD_ENTRY;
    CordbClass *ret = NULL;
    if (!m_classMap.Lookup(type_id, &ret)) {
        ret = new CordbClass(conn, token, module_id, type_id);
        m_classMap.Add(type_id, ret);
        ret->InternalAddRef();
    }
    return ret;
}

CordbClass* CordbProcess::FindOrAddClass(int type_id)
{
    LOG_METHOD_ENTRY;
    CordbClass *ret = NULL;
    if (!m_classMap.Lookup(type_id, &ret)) {
        ret = CordbClass::CreateFromSDB(conn, type_id);
        m_classMap.Add(type_id, ret);
        ret->InternalAddRef();
    }
    return ret;
}

CordbType* CordbProcess::FindOrAddPrimitiveType(CorElementType type)
{
    LOG_METHOD_ENTRY;
    CordbType* ret = NULL;
    MapSHashWithRemove<long, CordbType*>* typeMap = (MapSHashWithRemove<long, CordbType*>*) m_pTypeMapArray->Get(CordbTypeKindSimpleType);
    if (!typeMap->Lookup(type, &ret)) {
        ret = new CordbType(type, conn);
        typeMap->Add(type, ret);
        ret->InternalAddRef();
    }
    return ret;
}

CordbType* CordbProcess::FindOrAddClassType(CorElementType type, CordbClass *klass)
{
    LOG_METHOD_ENTRY;
    if (klass == NULL)
        return FindOrAddPrimitiveType(type);
    return klass->GetCordbType();
}

CordbType* CordbProcess::FindOrAddArrayType(CorElementType type, int arrayDebuggerId, CordbType* arrayType)
{
    LOG_METHOD_ENTRY;
    CordbType* ret = NULL;
    long hash = 0;
    MapSHashWithRemove<long, CordbType*>* typeMap = (MapSHashWithRemove<long, CordbType*>*) m_pTypeMapArray->Get(CordbTypeKindArrayType);
    CorElementType eleType;
    ICorDebugClass *eleClass = 0;
    mdTypeDef eleToken = 0;
    arrayType->GetType(&eleType);
    if (eleType == ELEMENT_TYPE_CLASS)
    {
        arrayType->GetClass(&eleClass);
        eleClass->GetToken(&eleToken);
    }
    if (!typeMap->Lookup(arrayDebuggerId, &ret)) {
        ret = CordbType::CreateArrayType(type, conn, arrayType);
        typeMap->Add(arrayDebuggerId, ret);
        ret->InternalAddRef();
    }
    return ret;
}

CordbFunction* CordbProcess::FindFunctionByToken(mdToken token, int module_id)
{
    LOG_METHOD_ENTRY;
    CordbFunction* ret = NULL;
    for (DWORD i = 0; i < m_pFunctions->GetCount(); i++)
    {
        CordbFunction* function = (CordbFunction*)m_pFunctions->Get(i);
        if (function->GetToken() == token && function->GetModuleId() == module_id)
        {
            ret = function;
            break;
        }
    }
    return ret;
}

CordbFunction* CordbProcess::FindFunction(int id)
{
    LOG_METHOD_ENTRY;
    CordbFunction* ret = NULL;
    for (DWORD i = 0; i < m_pFunctions->GetCount(); i++)
    {
        CordbFunction* function = (CordbFunction*)m_pFunctions->Get(i);
        if (function->GetDebuggerId() == id)
        {
            ret = function;
            break;
        }
    }
    return ret;
}

CordbStepper* CordbProcess::GetStepper(int id)
{
    LOG_METHOD_ENTRY;
    CordbStepper *ret = NULL;
    for (DWORD i = 0; i < m_pSteppers->GetCount(); i++)
    {
        CordbStepper* stepper = (CordbStepper*)m_pSteppers->Get(i);
        if (stepper->GetDebuggerId() == id)
        {
            ret = stepper;
            break;
        }
    }
    return ret;
}

CordbModule* CordbProcess::GetModule(int module_id)
{
    LOG_METHOD_ENTRY;
    CordbModule* ret = NULL;
    for (DWORD i = 0; i < m_pModules->GetCount(); i++)
    {
        CordbModule* module = (CordbModule*)m_pModules->Get(i);
        if (module->GetDebuggerId() == module_id)
        {
            ret = module;
            break;
        }
    }
    return ret;
}

CordbAppDomain* CordbProcess::GetCurrentAppDomain()
{
    LOG_METHOD_ENTRY;
    CordbAppDomain* ret = NULL;
    if (m_pAddDomains->GetCount() > 0)
        ret = (CordbAppDomain*)m_pAddDomains->Get(0);
    return ret;
}

CordbThread* CordbProcess::FindThread(long thread_id)
{
    LOG_METHOD_ENTRY;
    CordbThread* ret = NULL;
    for (DWORD i = 0; i < m_pThreads->GetCount(); i++)
    {
        CordbThread* thread = (CordbThread*)m_pThreads->Get(i);
        if (thread->GetThreadId() == thread_id)
        {
            ret = thread;
            break;
        }
    }
    return ret;
}

CordbFunctionBreakpoint* CordbProcess::GetBreakpoint(int id)
{
    LOG_METHOD_ENTRY;
    CordbFunctionBreakpoint* ret = NULL;
    for (DWORD i = 0; i < m_pBreakpoints->GetCount(); i++)
    {
        CordbFunctionBreakpoint* bp = (CordbFunctionBreakpoint*)m_pBreakpoints->Get(i);
        if (bp->GetDebuggerId() == id)
        {
            ret = bp;
            break;
        }
    }
    return ret;
}

void CordbProcess::SetJMCStatus(BOOL bIsJustMyCode)
{
    LOG_METHOD_ENTRY;
    m_bIsJustMyCode = bIsJustMyCode;
}

BOOL CordbProcess::GetJMCStatus()
{
    LOG_METHOD_ENTRY;
    return m_bIsJustMyCode;
}

