// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-THREAD.CPP
//

#include <cordb-appdomain.h>
#include <cordb-blocking-obj.h>
#include <cordb-chain.h>
#include <cordb-eval.h>
#include <cordb-frame.h>
#include <cordb-process.h>
#include <cordb-register.h>
#include <cordb-stepper.h>
#include <cordb-thread.h>
#include <cordb-stackwalk.h>
#include <cordb.h>

CordbThread::CordbThread(Connection* conn, CordbProcess* ppProcess, long thread_id) : CordbBaseMono(conn)
{
    LOG_METHOD_ENTRY;
    this->m_pProcess  = ppProcess;
    this->m_threadId  = thread_id;
    m_pRegisterSet    = NULL;
    m_pCurrentFrame   = NULL;
    m_pBlockingObject = NULL;
    m_pChains = NULL;
    ppProcess->AddThread(this);
}

CordbThread::~CordbThread()
{
    LOG_METHOD_ENTRY;
    if (m_pCurrentFrame)
        m_pCurrentFrame->InternalRelease();
    if (m_pBlockingObject)
        m_pBlockingObject->InternalRelease();
    if (m_pRegisterSet)
        m_pRegisterSet->InternalRelease();
    if (m_pChains)
        m_pChains->InternalRelease();
}

void CordbThread::Reset()
{
    LOG_METHOD_ENTRY;
    if (m_pChains)
        m_pChains->InternalRelease();
    if (m_pRegisterSet)
        m_pRegisterSet->InternalRelease();
    m_pChains = NULL;
    m_pRegisterSet = NULL;
}

void CordbThread::SetRegisterSet(CordbRegisterSet* rs)
{
    LOG_METHOD_ENTRY;
    if (m_pRegisterSet != NULL)
        m_pRegisterSet->InternalRelease();
    m_pRegisterSet = rs;
    m_pRegisterSet->InternalAddRef();
}

HRESULT STDMETHODCALLTYPE CordbThread::HasUnhandledException(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - HasUnhandledException - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetBlockingObjects(ICorDebugBlockingObjectEnum** ppBlockingObjectEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetBlockingObjects - IMPLEMENTED\n"));
    if (m_pBlockingObject == NULL)
    {
        m_pBlockingObject = new CordbBlockingObjectEnum(conn);
        m_pBlockingObject->InternalAddRef();
    }
    m_pBlockingObject->QueryInterface(IID_ICorDebugBlockingObjectEnum, (void**)ppBlockingObjectEnum);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentCustomDebuggerNotification(ICorDebugValue** ppNotificationObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000,
         "CordbThread - GetCurrentCustomDebuggerNotification - NOT "
         "IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStackWalk(ICorDebugStackWalk** ppStackWalk)
{
    LOG_METHOD_ENTRY;
    ICorDebugStackWalk* stackWalk = new CordbStackWalk(conn, this);
    return stackWalk->QueryInterface(IID_ICorDebugStackWalk, (void**)ppStackWalk);
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveInternalFrames(ULONG32                  cInternalFrames,
                                                               ULONG32*                 pcInternalFrames,
                                                               ICorDebugInternalFrame2* ppInternalFrames[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetActiveInternalFrames - NOT IMPLEMENTED\n"));
    *pcInternalFrames = 0;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFunctions(ULONG32             cFunctions,
                                                          ULONG32*            pcFunctions,
                                                          COR_ACTIVE_FUNCTION pFunctions[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetActiveFunctions - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetConnectionID(CONNID* pdwConnectionId)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetConnectionID - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetTaskID(TASKID* pTaskId)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetTaskID - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetVolatileOSThreadID(DWORD* pdwTid)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetActiveFrame - IMPLEMENTED\n"));
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, GetThreadId());

        int cmdId = this->conn->SendEvent(MDBGPROT_CMD_SET_THREAD, MDBGPROT_CMD_THREAD_GET_TID, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();

        *pdwTid = (DWORD) m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbThread::InterceptCurrentException(

    ICorDebugFrame* pFrame)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - InterceptCurrentException - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetProcess(ICorDebugProcess** ppProcess)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetProcess - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetID(DWORD* pdwThreadId)
{
    LOG_METHOD_ENTRY;
    *pdwThreadId = GetThreadId();
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetID - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetHandle(HTHREAD* phThreadHandle)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetHandle - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetAppDomain(ICorDebugAppDomain** ppAppDomain)
{
    LOG_METHOD_ENTRY;
    conn->GetCurrentAppDomain()->QueryInterface(IID_ICorDebugAppDomain, (void**)ppAppDomain);
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetAppDomain - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::SetDebugState(CorDebugThreadState state)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - SetDebugState - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetDebugState(CorDebugThreadState* pState)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetDebugState - NOT IMPLEMENTED\n"));
    *pState = THREAD_RUN;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetUserState(CorDebugUserState* pState)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetUserState - NOT IMPLEMENTED\n"));

    *pState = (CorDebugUserState)0;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentException(ICorDebugValue** ppExceptionObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetCurrentException - IMPLEMENTED\n"));

    return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbThread::ClearCurrentException(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - ClearCurrentException - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStepper(ICorDebugStepper** ppStepper)
{
    LOG_METHOD_ENTRY;
    m_pStepper = new CordbStepper(conn, this);
    m_pStepper->InternalAddRef();
    m_pStepper->QueryInterface(IID_ICorDebugStepper, (void**)ppStepper);

    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - CreateStepper - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::EnumerateChains(ICorDebugChainEnum** ppChains)
{
    LOG_METHOD_ENTRY;
    if (m_pChains == NULL)
    {
        m_pChains = new CordbChainEnum(conn, this);
        m_pChains->AddRef();
    }
    m_pChains->AddRef();
    *ppChains = static_cast<ICorDebugChainEnum*>(m_pChains);
    
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - EnumerateChains - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveChain(ICorDebugChain** ppChain)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetActiveChain - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFrame(ICorDebugFrame** ppFrame)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - GetActiveFrame - IMPLEMENTED\n"));
    HRESULT hr = S_OK;
    EX_TRY 
    {    
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, GetThreadId());
        m_dbgprot_buffer_add_int(&localbuf, 0);
        m_dbgprot_buffer_add_int(&localbuf, -1);

        int cmdId = this->conn->SendEvent(MDBGPROT_CMD_SET_THREAD, MDBGPROT_CMD_THREAD_GET_FRAME_INFO, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();

        int nframes = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
        if (nframes > 0)
        {
            int frameid   = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int methodId  = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
            int il_offset = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int flags     = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            if (m_pCurrentFrame)
                m_pCurrentFrame->InternalRelease();
            m_pCurrentFrame = new CordbNativeFrame(conn, frameid, methodId, il_offset, flags, this, 0);
            m_pCurrentFrame->InternalAddRef();
            m_pCurrentFrame->QueryInterface(IID_ICorDebugFrame, (void**)ppFrame);
        }
        SetRegisterSet(new CordbRegisterSet(conn, 0));
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetRegisterSet(ICorDebugRegisterSet** ppRegisters)
{
    LOG_METHOD_ENTRY;
    if (m_pRegisterSet != NULL)
    {
        m_pRegisterSet->QueryInterface(IID_ICorDebugRegisterSet, (void**)ppRegisters);
        return S_OK;
    }
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, GetThreadId());
        m_dbgprot_buffer_add_int(&localbuf, 0);

        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_THREAD, MDBGPROT_CMD_THREAD_GET_CONTEXT, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();

        int contextSizeReceived = 0;
        int64_t stack_pointer = m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
        m_pRegisterSet = new CordbRegisterSet(conn, stack_pointer);
        m_pRegisterSet->InternalAddRef();
    }
    EX_CATCH_HRESULT(hr);

    m_pRegisterSet->QueryInterface(IID_ICorDebugRegisterSet, (void**)ppRegisters);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateEval(ICorDebugEval** ppEval)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbThread - CreateEval - IMPLEMENTED\n"));
    CordbEval* eval = new CordbEval(this->conn, this);
    eval->QueryInterface(IID_ICorDebugEval, (void**)ppEval);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetObject(ICorDebugValue** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbThread - GetObject - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::QueryInterface(REFIID id, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugThread)
    {
        *ppInterface = static_cast<ICorDebugThread*>(this);
    }
    else if (id == IID_ICorDebugThread2)
    {
        *ppInterface = static_cast<ICorDebugThread2*>(this);
    }
    else if (id == IID_ICorDebugThread3)
    {
        *ppInterface = static_cast<ICorDebugThread3*>(this);
    }
    else if (id == IID_ICorDebugThread4)
    {
        *ppInterface = static_cast<ICorDebugThread4*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *ppInterface = static_cast<IUnknown*>(static_cast<ICorDebugThread*>(this));
    }
    else
    {
        *ppInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

CordbThreadEnum::CordbThreadEnum(Connection* conn) : CordbBaseMono(conn)
{
    LOG_METHOD_ENTRY;
}

HRESULT STDMETHODCALLTYPE CordbThreadEnum::Next(ULONG celt, ICorDebugThread* values[], ULONG* pceltFetched)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThreadEnum::Skip(ULONG celt)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThreadEnum::Reset(void)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThreadEnum::Clone(ICorDebugEnum** ppEnum)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThreadEnum::GetCount(ULONG* pcelt)
{
    LOG_METHOD_ENTRY;
    *pcelt = 0;
    return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbThreadEnum::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugThreadEnum*>(this));
    else if (id == IID_ICorDebugThreadEnum)
        *pInterface = static_cast<ICorDebugThreadEnum*>(this);
    else if (id == IID_ICorDebugEnum)
        *pInterface = static_cast<ICorDebugEnum*>(this);
    else
    {
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}
