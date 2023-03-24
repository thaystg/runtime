// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-CLASS.CPP
//

#include <cordb-breakpoint.h>
#include <cordb-class.h>
#include <cordb-process.h>
#include <cordb-value.h>
#include <cordb-frame.h>
#include <cordb-thread.h>
#include <cordb.h>

#include "cordb-assembly.h"

using namespace std;

CordbClass::CordbClass(Connection* conn, mdToken token, int module_id, int debugger_id) : CordbBaseMono(conn)
{
    this->m_metadataToken = token;
    this->m_debuggerModuleId    = module_id;
    m_debuggerId = debugger_id;
}

int CordbClass::GetDebuggerId() {
    if (m_debuggerId != -1)
        return m_debuggerId;

    MdbgProtBuffer localbuf;
    m_dbgprot_buffer_init(&localbuf, 128);
    m_dbgprot_buffer_add_int(&localbuf, m_debuggerModuleId);
    m_dbgprot_buffer_add_int(&localbuf, m_metadataToken);

    int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ASSEMBLY, MDBGPROT_CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
    m_dbgprot_buffer_free(&localbuf);

    ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
    if (received_reply_packet->Error() > 0 || received_reply_packet->Error2() > 0)
        return -1;
    MdbgProtBuffer* pReply = received_reply_packet->Buffer();
    m_debuggerId = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
    return m_debuggerId;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetModule(ICorDebugModule** pModule)
{
    LOG((LF_CORDB, LL_INFO1000000, "CordbClass - GetModule - IMPLEMENTED\n"));
    if (pModule)
    {
        CordbModule* module = conn->GetProcess()->GetModule(m_debuggerModuleId);
        if (module)
        {
            *pModule = static_cast<ICorDebugModule*>(module);
            (*pModule)->AddRef();
            return S_OK;
        }
    }
    return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetToken(mdTypeDef* pTypeDef)
{
    LOG((LF_CORDB, LL_INFO1000000, "CordbClass - GetToken - IMPLEMENTED\n"));
    *pTypeDef = m_metadataToken;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetStaticFieldValue(mdFieldDef       fieldDef,
                                                          ICorDebugFrame*  pFrame,
                                                          ICorDebugValue** ppValue)
{
    GetDebuggerId();
    HRESULT hr = S_OK;
    EX_TRY
    {
        if (m_debuggerId == -1)
            hr = S_FALSE;
        else {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);
            m_dbgprot_buffer_add_int(&localbuf, fieldDef);
            m_dbgprot_buffer_add_int(&localbuf, pFrame ? ((CordbNativeFrame*)pFrame)->GetThread()->GetThreadId() : -1);

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_TYPE, MDBGPROT_CMD_TYPE_GET_VALUES_ICORDBG, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            hr = CordbObjectValue::CreateCordbValue(conn, pReply, ppValue);
        }
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbClass::QueryInterface(REFIID id, void** pInterface)
{
    if (id == IID_ICorDebugClass)
    {
        *pInterface = static_cast<ICorDebugClass*>(this);
    }
    else if (id == IID_ICorDebugClass2)
    {
        *pInterface = static_cast<ICorDebugClass2*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugClass*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetParameterizedType(CorElementType  elementType,
                                                           ULONG32         nTypeArgs,
                                                           ICorDebugType*  ppTypeArgs[],
                                                           ICorDebugType** ppType)
{
    int debuggerId = GetDebuggerId();
    CordbClass* klass = this;
    if (nTypeArgs > 0)
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_int(&localbuf, debuggerId);
        m_dbgprot_buffer_add_int(&localbuf, nTypeArgs);
        for (ULONG32 i = 0; i < nTypeArgs; i++)
        {
            ICorDebugClass* pClass;
            ppTypeArgs[i]->GetClass(&pClass);
            m_dbgprot_buffer_add_int(&localbuf, ((CordbClass*)pClass)->GetDebuggerId());
        }
        
        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_TYPE, MDBGPROT_CMD_TYPE_BIND_GENERIC_PARAMETERS, &localbuf);
        m_dbgprot_buffer_free(&localbuf);

        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        debuggerId = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
    }
    if (debuggerId != m_debuggerId)
    {
        klass = new CordbClass(conn, m_metadataToken, m_debuggerModuleId, debuggerId);
    }
    
    CordbType *ret = conn->GetProcess()->FindOrAddClassType(elementType, klass);
    ret->QueryInterface(IID_ICorDebugType, (void**)ppType);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::SetJMCStatus(BOOL bIsJustMyCode)
{
    LOG((LF_CORDB, LL_INFO100000, "CordbClass - SetJMCStatus - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}
