// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-EVAL.CPP
//

#include <cordb-appdomain.h>
#include <cordb-eval.h>
#include <cordb-function.h>
#include <cordb-process.h>
#include <cordb-thread.h>
#include <cordb-value.h>
#include <cordb-class.h>
#include <cordb.h>

#include "corerror.h"
#include "metamodel.h"
#include "metamodelpub.h"
#include "rwutil.h"
#include "stdafx.h"

CordbEval::CordbEval(Connection* conn, CordbThread* thread) : CordbBaseMono(conn)
{
    LOG_METHOD_ENTRY;
    this->m_pThread = thread;
    if (thread)
        thread->InternalAddRef();
    m_pValue    = NULL;
    m_commandId = -1;
    m_pReply = NULL;
    isNoConstructor = false;
    m_evalStatus = NONE;
    m_pSemReadWrite  = new UTSemReadWrite();
    m_pSemReadWrite->Init();
}

void CordbEval::Cleanup()
{
    m_pReply = NULL;
    m_evalStatus = NONE;
    m_pValue = NULL;
}

CordbEval::~CordbEval()
{
    LOG_METHOD_ENTRY;
    if (m_pThread)
        m_pThread->InternalRelease();
}

int CordbEval::GetMethodFromGenericTypeBound(ICorDebugFunction* pFunction,
                         ULONG32            nTypeArgs,
                         ICorDebugType*     ppTypeArgs[])
{
    LOG_METHOD_ENTRY;
    int methodId = ((CordbFunction*)pFunction)->GetDebuggerId();
    if (nTypeArgs > 0)
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_int(&localbuf, methodId);
        m_dbgprot_buffer_add_int(&localbuf, nTypeArgs);
        for (ULONG32 i = 0; i < nTypeArgs; i++)
        {
            ICorDebugClass* pClass;
            ppTypeArgs[i]->GetClass(&pClass);
            m_dbgprot_buffer_add_int(&localbuf, ((CordbClass*)pClass)->GetDebuggerId());
        }
        
        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_METHOD, MDBGPROT_CMD_METHOD_BIND_GENERIC_TYPE_PARAMETERS, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        methodId = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
    }
    return methodId;
}
HRESULT STDMETHODCALLTYPE CordbEval::CallParameterizedFunction(ICorDebugFunction* pFunction,
                                                               ULONG32            nTypeArgs,
                                                               ICorDebugType*     ppTypeArgs[],
                                                               ULONG32            nArgs,
                                                               ICorDebugValue*    ppArgs[])
{
    LOG_METHOD_ENTRY;
    Cleanup();
    conn->GetProcess()->Stop(false);
    LOG((LF_CORDB, LL_INFO1000000, "CordbEval - CallParameterizedFunction - IMPLEMENTED\n"));
    int methodId = GetMethodFromGenericTypeBound(pFunction, nTypeArgs, ppTypeArgs);
    MdbgProtBuffer localbuf;
    m_dbgprot_buffer_init(&localbuf, 128);
    m_dbgprot_buffer_add_id(&localbuf, m_pThread->GetThreadId());
    m_dbgprot_buffer_add_int(&localbuf, 1);
    m_dbgprot_buffer_add_int(&localbuf, methodId);
    //TODO check if is static method and if it's pass the first arg which is this and then pass the nArgs
    bool isStatic = false;
    for (ULONG32 i = 0; i < nArgs; i++)
    {
        if ((!isStatic && i == 1) || isStatic == true)
            m_dbgprot_buffer_add_int(&localbuf, isStatic ? nArgs : nArgs - 1);
        CordbValueMono* valueMono;
        ppArgs[i]->QueryInterface(IID_ICorDebugValueMono, (void**)&valueMono);
        valueMono->WriteToBuffer(&localbuf);
    }
    if (nArgs == 1 && !isStatic)
        m_dbgprot_buffer_add_int(&localbuf, 0);
    m_commandId = conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_INVOKE_METHOD, &localbuf);
    m_dbgprot_buffer_free(&localbuf);
    conn->GetProcess()->AddPendingEval(m_commandId, this);
    return S_OK;
}

void CordbEval::EvalComplete()
{
    m_pSemReadWrite->LockWrite();
    if (m_evalStatus == CONTINUED && m_pReply != NULL)
    {
        m_evalStatus = COMPLETED;
        LOG_METHOD_ENTRY;
        conn->GetCordb()->GetCallback()->EvalComplete(conn->GetProcess()->GetCurrentAppDomain(), m_pThread, this);
    }
    m_pSemReadWrite->UnlockWrite();
}

void CordbEval::SetContinued()
{
    m_pSemReadWrite->LockWrite();
    if (m_evalStatus == NONE)
        m_evalStatus = CONTINUED;
    m_pSemReadWrite->UnlockWrite();
}

void CordbEval::SetResponse(MdbgProtBuffer* pReply)
{
    LOG_METHOD_ENTRY;
    m_pReply = pReply;
}

HRESULT STDMETHODCALLTYPE CordbEval::CreateValueForType(ICorDebugType* pType, ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - CreateValueForType - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewParameterizedObject(ICorDebugFunction* pConstructor,
                                                            ULONG32            nTypeArgs,
                                                            ICorDebugType*     ppTypeArgs[],
                                                            ULONG32            nArgs,
                                                            ICorDebugValue*    ppArgs[])
{
    LOG_METHOD_ENTRY;
    Cleanup();
    conn->GetProcess()->Stop(false);
    LOG((LF_CORDB, LL_INFO1000000, "CordbEval - NewParameterizedObject - IMPLEMENTED\n"));
    int methodId = GetMethodFromGenericTypeBound(pConstructor, nTypeArgs, ppTypeArgs);
    MdbgProtBuffer localbuf;
    m_dbgprot_buffer_init(&localbuf, 128);
    m_dbgprot_buffer_add_id(&localbuf, m_pThread->GetThreadId());
    m_dbgprot_buffer_add_int(&localbuf, 1);
    m_dbgprot_buffer_add_int(&localbuf, methodId);
    m_dbgprot_buffer_add_byte(&localbuf, MDBGPROT_VALUE_TYPE_ID_NULL); 
    m_dbgprot_buffer_add_byte(&localbuf, 0);
    m_dbgprot_buffer_add_int(&localbuf, 0);
    m_dbgprot_buffer_add_int(&localbuf, 1);
    for (ULONG32 i = 0; i < nArgs; i++)
    {
        CordbValueMono* valueMono;
        ppArgs[i]->QueryInterface(IID_ICorDebugValueMono, (void**)&valueMono);
        valueMono->WriteToBuffer(&localbuf);
    }
    m_commandId = conn->SendEvent(MDBGPROT_CMD_SET_VM, MDBGPROT_CMD_VM_INVOKE_METHOD, &localbuf);
    m_dbgprot_buffer_free(&localbuf);
    conn->GetProcess()->AddPendingEval(m_commandId, this);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewParameterizedObjectNoConstructor(ICorDebugClass* pClass,
                                                                         ULONG32         nTypeArgs,
                                                                         ICorDebugType*  ppTypeArgs[])
{
    LOG_METHOD_ENTRY;
    Cleanup();
    HRESULT hr = S_OK;
    EX_TRY
    {
        conn->GetProcess()->Stop(false);
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, ((CordbClass*)pClass)->GetDebuggerId());
        this->m_commandId = conn->SendEvent(MDBGPROT_CMD_SET_TYPE, MDBGPROT_CMD_TYPE_GET_OBJECT, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        isNoConstructor = true;
        conn->GetProcess()->AddPendingEval(m_commandId, this);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewParameterizedArray(ICorDebugType* pElementType,
                                                           ULONG32        rank,
                                                           ULONG32        dims[],
                                                           ULONG32        lowBounds[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - NewParameterizedArray - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewStringWithLength(LPCWSTR string, UINT uiLength)
{
    LOG_METHOD_ENTRY;
    Cleanup();
    HRESULT hr = S_OK;
    EX_TRY
    {
        conn->GetProcess()->Stop(false);
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, m_pThread->GetThreadId());
        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_THREAD, MDBGPROT_CMD_THREAD_GET_APPDOMAIN, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();

        int domainId = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);

        LPSTR szString;
        UTF8STR(string, szString);

        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, domainId);
        m_dbgprot_buffer_add_string(&localbuf, szString);
        this->m_commandId = conn->SendEvent(MDBGPROT_CMD_SET_APPDOMAIN, MDBGPROT_CMD_APPDOMAIN_CREATE_STRING, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        conn->GetProcess()->AddPendingEval(m_commandId, this);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbEval::RudeAbort(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - RudeAbort - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT
CordbEval::QueryInterface(REFIID id, _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugEval)
    {
        *pInterface = static_cast<ICorDebugEval*>(this);
    }
    else if (id == IID_ICorDebugEval2)
    {
        *pInterface = static_cast<ICorDebugEval2*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugEval*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::CallFunction(ICorDebugFunction* pFunction, ULONG32 nArgs, ICorDebugValue* ppArgs[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - CallFunction - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewObject(ICorDebugFunction* pConstructor, ULONG32 nArgs, ICorDebugValue* ppArgs[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - NewObject - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewObjectNoConstructor(ICorDebugClass* pClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - NewObjectNoConstructor - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewString(LPCWSTR string)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - NewString - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewArray(
    CorElementType elementType, ICorDebugClass* pElementClass, ULONG32 rank, ULONG32 dims[], ULONG32 lowBounds[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - NewArray - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::IsActive(BOOL* pbActive)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - IsActive - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::Abort(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - Abort - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::GetResult(ICorDebugValue** ppResult)
{
    LOG_METHOD_ENTRY;
    if (!isNoConstructor)
    {
        m_dbgprot_decode_byte(m_pReply->p, &m_pReply->p, m_pReply->end);
        CordbObjectValue::CreateCordbValue(conn, m_pReply, &m_pValue);
    }
    else
    {
        int object_id = m_dbgprot_decode_id(m_pReply->p, &m_pReply->p, m_pReply->end);
        CordbObjectValue::CreateCordbValue(conn, m_pReply, &m_pValue);
    }
    *ppResult = m_pValue;
    LOG((LF_CORDB, LL_INFO1000000, "CordbEval - GetResult - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::GetThread(ICorDebugThread** ppThread)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbEval - GetThread - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::CreateValue(CorElementType   elementType,
                                                 ICorDebugClass*  pElementClass,
                                                 ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    CordbContent content_value;
    content_value.booleanValue = 0;
    CordbValue* value = new CordbValue(conn, elementType, content_value, CordbObjectValue::GetTypeSize(elementType));
    LOG((LF_CORDB, LL_INFO1000000, "CordbEval - CreateValue - IMPLEMENTED\n"));
    value->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
    return S_OK;
}
