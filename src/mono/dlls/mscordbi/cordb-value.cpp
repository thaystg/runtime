// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-VALUE.CPP
//

#include <cordb-breakpoint.h>
#include <cordb-class.h>
#include <cordb-type.h>
#include <cordb-value.h>
#include <cordb-process.h>
#include <cordb.h>

using namespace std;

CordbValue::CordbValue(Connection* conn, CorElementType type, CordbContent value, int size) : CordbBaseMono(conn), CordbValueMono(type)
{
    LOG_METHOD_ENTRY;
    this->m_value = value;
    this->m_size  = size;
    this->conn    = conn;
    m_pType       = NULL;
}

CordbValue::~CordbValue()
{
    LOG_METHOD_ENTRY;
    if (m_pType)
        m_pType->InternalRelease();
}

void CordbValue::WriteToBuffer(MdbgProtBuffer* buf)
{
    LOG_METHOD_ENTRY;
    m_dbgprot_buffer_add_byte(buf, m_type);
    switch (m_type)
    {
        case ELEMENT_TYPE_BOOLEAN:
        case ELEMENT_TYPE_I1:
        case ELEMENT_TYPE_U1:
            m_dbgprot_buffer_add_int(buf, (int)m_value.booleanValue);
            break;
        case ELEMENT_TYPE_CHAR:
        case ELEMENT_TYPE_I2:
        case ELEMENT_TYPE_U2:
            m_dbgprot_buffer_add_int(buf, (int)m_value.charValue);
            break;
        case ELEMENT_TYPE_I4:
        case ELEMENT_TYPE_U4:
        case ELEMENT_TYPE_R4:
            m_dbgprot_buffer_add_int(buf, m_value.intValue);
            break;
        case ELEMENT_TYPE_I8:
        case ELEMENT_TYPE_U8:
        case ELEMENT_TYPE_R8:
            m_dbgprot_buffer_add_long(buf, m_value.longValue);
            break;
        default:
            printf("not expected");
    }
}

HRESULT STDMETHODCALLTYPE CordbValue::GetType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    *pType = m_type;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize(ULONG32* pSize)
{
    LOG_METHOD_ENTRY;
    *pSize = m_size;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetAddress(CORDB_ADDRESS* pAddress)
{
    LOG_METHOD_ENTRY;
    *pAddress = (CORDB_ADDRESS)0;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValue - GetAddress - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValue - CreateBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValue)
    {
        *pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugGenericValue*>(this));
    }
    else if (id == IID_ICorDebugValue2)
    {
        *pInterface = static_cast<ICorDebugValue2*>(this);
    }
    else if (id == IID_ICorDebugValue3)
    {
        *pInterface = static_cast<ICorDebugValue3*>(this);
    }
    else if (id == IID_ICorDebugGenericValue)
    {
        *pInterface = static_cast<ICorDebugGenericValue*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugGenericValue*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetExactType(ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValue - GetExactType - IMPLEMENTED\n"));
    if (m_pType == NULL)
    {
        m_pType = conn->GetProcess()->FindOrAddPrimitiveType(m_type);
        m_pType->InternalAddRef();
    }
    m_pType->QueryInterface(IID_ICorDebugType, (void**)ppType);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize64(ULONG64* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValue - GetSize64 - IMPLEMENTED\n"));
    *pSize = m_size;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetValue(void* pTo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValue - GetValue - IMPLEMENTED\n"));
    memcpy(pTo, (void*)&m_value, m_size);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::SetValue(void* pFrom)
{
    LOG_METHOD_ENTRY;
    memcpy(&m_value, pFrom, m_size);
    LOG((LF_CORDB, LL_INFO1000000, "CordbValue - SetValue - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - GetType - IMPLEMENTED\n"));
    *pType = m_type;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetSize(ULONG32* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - GetSize - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetAddress(CORDB_ADDRESS* pAddress)
{
    LOG_METHOD_ENTRY;
    *pAddress = (CORDB_ADDRESS)0;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - GetAddress - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - CreateBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValue)
    {
        *pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugReferenceValue*>(this));
    }
    else if (id == IID_ICorDebugValue2)
    {
        *pInterface = static_cast<ICorDebugValue2*>(this);
    }
    else if (id == IID_ICorDebugValue3)
    {
        *pInterface = static_cast<ICorDebugValue3*>(this);
    }
    else if (id == IID_ICorDebugReferenceValue)
    {
        *pInterface = static_cast<ICorDebugReferenceValue*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugReferenceValue*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

CordbClass* CordbReferenceValue::GetClass(int type_id)
{
    LOG_METHOD_ENTRY;
    return conn->GetProcess()->FindOrAddClass(type_id);
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetExactType(ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - GetExactType - IMPLEMENTED\n"));
    HRESULT hr = S_OK;
    EX_TRY
    {
        if (m_pCordbType)
        {
            m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
            goto __Exit;
        }
        if (m_pClass != NULL)
        {
            m_pCordbType = conn->GetProcess()->FindOrAddClassType(m_type, m_pClass);
            m_pCordbType->InternalAddRef();
            m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
            goto __Exit;
        }
        if (m_type == ELEMENT_TYPE_CLASS && m_debuggerId != -1)
        {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_OBJECT_REF, MDBGPROT_CMD_OBJECT_REF_GET_TYPE, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            int type_id = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);

            m_pClass = conn->GetProcess()->FindOrAddClass(type_id);
            m_pClass->InternalAddRef();
            m_pCordbType = m_pClass->GetCordbType();
            m_pCordbType->InternalAddRef();
            m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
            goto __Exit;
        }
        if (m_type == ELEMENT_TYPE_SZARRAY && m_debuggerId != -1)
        {
            m_pClass = NULL;
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ARRAY_REF, MDBGPROT_CMD_ARRAY_REF_GET_TYPE, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            CorElementType type_id = (CorElementType)m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            int rank    = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            if (type_id == ELEMENT_TYPE_CLASS)
            {
                int klass_id = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                m_pClass = conn->GetProcess()->FindOrAddClass(klass_id);
                m_pClass->InternalAddRef();
                m_pCordbType = m_pClass->GetCordbType();
            }
            else
            {
                m_pCordbType = conn->GetProcess()->FindOrAddPrimitiveType(type_id);
            }
            m_pCordbType = conn->GetProcess()->FindOrAddArrayType(m_type, m_debuggerId, m_pCordbType);
            m_pCordbType->InternalAddRef();
            m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
            goto __Exit;
        }
        m_pCordbType = conn->GetProcess()->FindOrAddPrimitiveType(m_type);
        m_pCordbType->InternalAddRef();
        m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
    }
    EX_CATCH_HRESULT(hr);
__Exit:
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetSize64(ULONG64* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - GetSize64 - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetValue(void* pTo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - GetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::SetValue(void* pFrom)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - SetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::IsNull(BOOL* pbNull)
{
    LOG_METHOD_ENTRY;
    if (m_debuggerId == -1)
        *pbNull = true;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - IsNull - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetValue(CORDB_ADDRESS* pValue)
{
    LOG_METHOD_ENTRY;
    *pValue = (CORDB_ADDRESS)m_pAddress;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - GetValue - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::SetValue(CORDB_ADDRESS value)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - SetValue CORDB_ADDRESS - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::Dereference(ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    if (m_debuggerId == -1)
        return CORDBG_E_BAD_REFERENCE_VALUE;
    if (!m_pClass) {
        ICorDebugType* cordbType;
        GetExactType(&cordbType);
        cordbType->Release();
    }
    if (m_type == ELEMENT_TYPE_SZARRAY || m_type == ELEMENT_TYPE_ARRAY)
    {
        CordbArrayValue* objectValue = new CordbArrayValue(conn, m_type, m_pCordbType, m_debuggerId, m_pClass);
        objectValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
    }
    else
    {
        CordbObjectValue* objectValue = new CordbObjectValue(conn, m_type, m_debuggerId, m_pClass, m_pAddress, m_pCordbType);
        objectValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
    }
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - Dereference - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::DereferenceStrong(ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - DereferenceStrong - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

CordbReferenceValue::CordbReferenceValue(Connection* conn, CorElementType type, int object_id, CordbClass* klass, CordbType* cordbType, CORDB_ADDRESS cordbAddress)
    : CordbBaseMono(conn), CordbValueMono(type)
{
    LOG_METHOD_ENTRY;
    this->m_debuggerId = object_id;
    this->conn         = conn;
    this->m_pClass     = klass;
    this->m_pCordbType = cordbType;
    this->m_pAddress   = cordbAddress;
    if (cordbType)
        cordbType->InternalAddRef();
    if (klass)
        klass->InternalAddRef();
}

void CordbReferenceValue::WriteToBuffer(MdbgProtBuffer* buf)
{
    LOG_METHOD_ENTRY;
    m_dbgprot_buffer_add_byte(buf, m_type);
    m_dbgprot_buffer_add_int(buf, m_debuggerId);
}

CordbReferenceValue::~CordbReferenceValue()
{
    LOG_METHOD_ENTRY;
    if (m_pCordbType)
        m_pCordbType->InternalRelease();
    if (m_pClass)
        m_pClass->InternalRelease();
}

CordbObjectValue::CordbObjectValue(Connection* conn, CorElementType type, int object_id, CordbClass* klass, CORDB_ADDRESS address, CordbType* cordbType)
    : CordbBaseMono(conn), CordbValueMono(type)
{
    LOG_METHOD_ENTRY;
    this->m_debuggerId = object_id;
    this->m_pClass     = klass;
    if (klass)
        klass->InternalAddRef();
    m_pCordbType = cordbType;
    if (cordbType)
        cordbType->InternalAddRef();
    m_pAddress = address;
}

void CordbObjectValue::WriteToBuffer(MdbgProtBuffer* buf)
{
    LOG_METHOD_ENTRY;
    m_dbgprot_buffer_add_byte(buf, m_type);
    m_dbgprot_buffer_add_int(buf, m_debuggerId);
}

CordbObjectValue::~CordbObjectValue()
{
    LOG_METHOD_ENTRY;
    if (m_pClass)
        m_pClass->InternalRelease();
    if (m_pCordbType)
        m_pCordbType->InternalRelease();
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbObjectValue - GetType - IMPLEMENTED\n"));
    *pType = m_type;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetSize(ULONG32* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetSize - NOT IMPLEMENTED\n"));
    *pSize = 10;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetAddress(CORDB_ADDRESS* pAddress)
{
    LOG_METHOD_ENTRY;
    *pAddress = (CORDB_ADDRESS)m_pAddress; 
    LOG((LF_CORDB, LL_INFO1000000, "CordbObjectValue - GetAddress - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - CreateBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetExactType(ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbObjectValue - GetExactType - IMPLEMENTED\n"));
    if (m_pCordbType == NULL)
    {
        m_pCordbType = conn->GetProcess()->FindOrAddClassType(m_type, m_pClass);
        m_pCordbType->InternalAddRef();
    }
    m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetSize64(ULONG64* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetSize64 - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetValue(void* pTo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::SetValue(void* pFrom)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - SetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetVirtualMethodAndType(mdMemberRef         memberRef,
                                                                    ICorDebugFunction** ppFunction,
                                                                    ICorDebugType**     ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetVirtualMethodAndType - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetLength(ULONG32* pcchString)
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        if (m_debuggerId == -1)
            hr = S_FALSE;
        else if (m_type == ELEMENT_TYPE_STRING)
        {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_STRING_REF, MDBGPROT_CMD_STRING_REF_GET_LENGTH, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            *pcchString = (ULONG32)m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
        }
        else
            hr = E_NOTIMPL;
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetString(ULONG32 cchString, ULONG32* pcchString, WCHAR szString[])
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        if (m_debuggerId == -1)
            hr = S_FALSE;
        else if (m_type == ELEMENT_TYPE_STRING)
        {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_STRING_REF, MDBGPROT_CMD_STRING_REF_GET_VALUE, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            *pcchString   = cchString;
            int use_utf16 = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            if (use_utf16)
            {
               LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetString - NOT IMPLEMENTED - use_utf16\n"));
            }
            else
            {
                char* value = m_dbgprot_decode_string(pReply->p, &pReply->p, pReply->end);
                LOG((LF_CORDB, LL_INFO1000000, "CordbObjectValue - GetString - IMPLEMENTED\n"));
                if (cchString >= strlen(value))
                {
                    MultiByteToWideChar(CP_UTF8, 0, value, -1, szString, cchString);
                    *pcchString = cchString;
                }
                free(value);
            }
            hr =  S_OK;
        }
        else
            hr = E_NOTIMPL;
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateHandle(CorDebugHandleType type, ICorDebugHandleValue** ppHandle)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - CreateHandle - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetThreadOwningMonitorLock(ICorDebugThread** ppThread,
                                                                       DWORD*            pAcquisitionCount)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetThreadOwningMonitorLock - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetMonitorEventWaitList(ICorDebugThreadEnum** ppThreadEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetMonitorEventWaitList - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE
CordbObjectValue::EnumerateExceptionCallStack(ICorDebugExceptionObjectCallStackEnum** ppCallStackEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - EnumerateExceptionCallStack - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetCachedInterfaceTypes(BOOL                bIInspectableOnly,
                                                                    ICorDebugTypeEnum** ppInterfacesEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetCachedInterfaceTypes - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetCachedInterfacePointers(BOOL           bIInspectableOnly,
                                                                       ULONG32        celt,
                                                                       ULONG32*       pcEltFetched,
                                                                       CORDB_ADDRESS* ptrs)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetCachedInterfacePointers - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetTarget(ICorDebugReferenceValue** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetTarget - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetFunction(ICorDebugFunction** ppFunction)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetFunction - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetClass(ICorDebugClass** ppClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetClass - IMPLEMENTED\n"));
    if (m_pClass) {
        m_pClass->QueryInterface(IID_ICorDebugClass, (void**)ppClass);
        return S_OK;
    }
    return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetFieldValue(ICorDebugClass*  pClass,
                                                          mdFieldDef       fieldDef,
                                                          ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbObjectValue - GetFieldValue - IMPLEMENTED\n"));
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

            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_OBJECT_REF, MDBGPROT_CMD_OBJECT_REF_GET_VALUES_ICORDBG, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            hr = CreateCordbValue(conn, pReply, ppValue);
        }
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

int CordbObjectValue::GetTypeSize(int type)
{
    //LOG_METHOD_ENTRY;
    switch (type)
    {
        case ELEMENT_TYPE_VOID:
            return 0;
        case ELEMENT_TYPE_BOOLEAN:
        case ELEMENT_TYPE_I1:
        case ELEMENT_TYPE_U1:
            return 1;
            break;
        case ELEMENT_TYPE_CHAR:
        case ELEMENT_TYPE_I2:
        case ELEMENT_TYPE_U2:
            return 2;
        case ELEMENT_TYPE_I4:
        case ELEMENT_TYPE_U4:
        case ELEMENT_TYPE_R4:
            return 4;
        case ELEMENT_TYPE_I8:
        case ELEMENT_TYPE_U8:
        case ELEMENT_TYPE_R8:
            return 8;
    }
    return 0;
}

HRESULT CordbObjectValue::CreateCordbValue(Connection* conn, MdbgProtBuffer* pReply, ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        CorElementType type = (CorElementType)m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
        CordbContent   value;

        if ((MdbgProtValueTypeId)type == MDBGPROT_VALUE_TYPE_ID_NULL)
        {
            CorElementType type = (CorElementType)m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            if (type == ELEMENT_TYPE_CLASS || type == ELEMENT_TYPE_STRING || type == ELEMENT_TYPE_OBJECT || type == ELEMENT_TYPE_GENERICINST)
            {
                int klass_id = (CorElementType)m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);

                
                CordbClass*          klass    =  conn->GetProcess()->FindOrAddClass(klass_id);
                CordbReferenceValue* refValue = new CordbReferenceValue(conn, type, -1, klass);
                refValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
            }
            if (type == ELEMENT_TYPE_SZARRAY)
            {
                CordbClass* klass   = NULL;
                int         type_id = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
                if (type_id == ELEMENT_TYPE_CLASS)
                {
                    int klass_id = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                    klass =  conn->GetProcess()->FindOrAddClass(klass_id);
                }
                CordbType* cordbtype = CordbType::CreateArrayType(type, conn,  klass->GetCordbType());
                CordbReferenceValue* refValue = new CordbReferenceValue(conn, type, -1, klass, cordbtype);
                refValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
            }
            goto __Exit;
        }

        switch (type)
        {
            case ELEMENT_TYPE_BOOLEAN:
            case ELEMENT_TYPE_I1:
            case ELEMENT_TYPE_U1:
                value.booleanValue = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                break;
            case ELEMENT_TYPE_CHAR:
            case ELEMENT_TYPE_I2:
            case ELEMENT_TYPE_U2:
                value.charValue = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                break;
            case ELEMENT_TYPE_I4:
            case ELEMENT_TYPE_U4:
            case ELEMENT_TYPE_R4:
                value.intValue = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                break;
            case ELEMENT_TYPE_I8:
            case ELEMENT_TYPE_U8:
            case ELEMENT_TYPE_R8:
                value.longValue = m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
                break;
            case ELEMENT_TYPE_CLASS:
            case ELEMENT_TYPE_SZARRAY:
            case ELEMENT_TYPE_STRING:
            {
                int object_id = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
                CORDB_ADDRESS address = m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
                CordbReferenceValue* refValue  = new CordbReferenceValue(conn, type, object_id, NULL, NULL, address);
                refValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
                goto __Exit;
            }
            case ELEMENT_TYPE_VALUETYPE:
            {
                int isEnum = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
                int isBoxed = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
                CORDB_ADDRESS address = m_dbgprot_decode_long(pReply->p, &pReply->p, pReply->end);
                int type_id = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
                CordbValueType *valueTypeValue = new CordbValueType(conn, type, conn->GetProcess()->FindOrAddClass(type_id), address);
                int fieldCount = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                for (int i = 0 ; i < fieldCount; i++)
                {
                    int fieldToken = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                    ICorDebugValue* m_pValue;
                    CordbObjectValue::CreateCordbValue(conn, pReply, &m_pValue);
                    valueTypeValue->AddField(fieldToken, m_pValue);
                }
                valueTypeValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
                goto __Exit;
            }
            default:
               LOG((LF_CORDB, LL_INFO100000, "default value - %d", type));
                hr = E_FAIL;
                goto __Exit;
        }
        *ppValue = new CordbValue(conn, type, value, GetTypeSize(type));
        (*ppValue)->AddRef();
    }
    EX_CATCH_HRESULT(hr);
__Exit:
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetVirtualMethod(mdMemberRef memberRef, ICorDebugFunction** ppFunction)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetVirtualMethod - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetContext(ICorDebugContext** ppContext)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetContext - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::IsValueClass(BOOL* pbIsValueClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - IsValueClass - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetManagedCopy(IUnknown** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - GetManagedCopy - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::SetFromManagedCopy(IUnknown* pObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - SetFromManagedCopy - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::IsValid(BOOL* pbValid)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - IsValid - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateRelocBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbObjectValue - CreateRelocBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValue)
    {
        *pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugObjectValue*>(this));
    }
    else if (id == IID_ICorDebugValue2)
    {
        *pInterface = static_cast<ICorDebugValue2*>(this);
    }
    else if (id == IID_ICorDebugValue3)
    {
        *pInterface = static_cast<ICorDebugValue3*>(this);
    }
    else if (id == IID_ICorDebugObjectValue)
    {
        *pInterface = static_cast<ICorDebugObjectValue*>(this);
    }
    else if (id == IID_ICorDebugObjectValue2)
    {
        *pInterface = static_cast<ICorDebugObjectValue2*>(this);
    }
    else if (id == IID_ICorDebugGenericValue)
    {
        *pInterface = static_cast<ICorDebugGenericValue*>(this);
    }
    /*else if (id == IID_ICorDebugHeapValue)
    {
        *pInterface = static_cast<ICorDebugHeapValue*>(this);
    }
    else if (id == IID_ICorDebugHeapValue2)
    {
        *pInterface = static_cast<ICorDebugHeapValue2*>(this);
    }
    else if (id == IID_ICorDebugHeapValue3)
    {
        *pInterface = static_cast<ICorDebugHeapValue3*>(this);
    }*/
    else if ((id == IID_ICorDebugStringValue) && (m_type == ELEMENT_TYPE_STRING))
    {
        *pInterface = static_cast<ICorDebugStringValue*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugObjectValue*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

CordbArrayValue::CordbArrayValue(Connection* conn, CorElementType element_type, CordbType* type, int object_id, CordbClass* klass)
    : CordbBaseMono(conn), CordbValueMono(element_type)
{
    LOG_METHOD_ENTRY;
    this->m_debuggerId = object_id;
    this->m_pClass     = klass;
    this->m_pCordbType = type;
    if (klass)
        klass->InternalAddRef();
    if (m_pCordbType)
        m_pCordbType->InternalAddRef();
}

CordbArrayValue::~CordbArrayValue()
{
    LOG_METHOD_ENTRY;
    if (m_pClass)
        m_pClass->InternalRelease();
    if (m_pCordbType)
        m_pCordbType->InternalRelease();
}
HRESULT STDMETHODCALLTYPE CordbArrayValue::GetClass(ICorDebugClass** ppClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetClass - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetFieldValue(ICorDebugClass*  pClass,
                                                         mdFieldDef       fieldDef,
                                                         ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetFieldValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetVirtualMethod(mdMemberRef memberRef, ICorDebugFunction** ppFunction)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetVirtualMethod - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetContext(ICorDebugContext** ppContext)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetContext - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::IsValueClass(BOOL* pbIsValueClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - IsValueClass - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetManagedCopy(IUnknown** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetManagedCopy - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::SetFromManagedCopy(IUnknown* pObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - SetFromManagedCopy - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetType - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetSize(ULONG32* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetSize - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetAddress(CORDB_ADDRESS* pAddress)
{
    LOG_METHOD_ENTRY;
    *pAddress = (CORDB_ADDRESS)0;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetAddress - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - CreateBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValue)
    {
        *pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugArrayValue*>(this));
    }
    else if (id == IID_ICorDebugValue2)
    {
        *pInterface = static_cast<ICorDebugValue2*>(this);
    }
    else if (id == IID_ICorDebugValue3)
    {
        *pInterface = static_cast<ICorDebugValue3*>(this);
    }
    else if (id == IID_ICorDebugArrayValue)
    {
        *pInterface = static_cast<ICorDebugArrayValue*>(this);
    }
    else if (id == IID_ICorDebugGenericValue)
    {
        *pInterface = static_cast<ICorDebugGenericValue*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugArrayValue*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetVirtualMethodAndType(mdMemberRef         memberRef,
                                                                   ICorDebugFunction** ppFunction,
                                                                   ICorDebugType**     ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetVirtualMethodAndType - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetValue(void* pTo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::SetValue(void* pFrom)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - SetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetLength(ULONG32* pcchString)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetLength - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetString(ULONG32 cchString, ULONG32* pcchString, WCHAR szString[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetString - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::IsValid(BOOL* pbValid)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - IsValid - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::CreateRelocBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - CreateRelocBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetExactType(ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    m_pCordbType->QueryInterface(IID_ICorDebugType, (void**)ppType);
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetExactType - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetSize64(ULONG64* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetSize64 - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::CreateHandle(CorDebugHandleType type, ICorDebugHandleValue** ppHandle)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - CreateHandle - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetThreadOwningMonitorLock(ICorDebugThread** ppThread,
                                                                      DWORD*            pAcquisitionCount)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetThreadOwningMonitorLock - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetMonitorEventWaitList(ICorDebugThreadEnum** ppThreadEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetMonitorEventWaitList - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE
CordbArrayValue::EnumerateExceptionCallStack(ICorDebugExceptionObjectCallStackEnum** ppCallStackEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - EnumerateExceptionCallStack - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetCachedInterfaceTypes(BOOL                bIInspectableOnly,
                                                                   ICorDebugTypeEnum** ppInterfacesEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetCachedInterfaceTypes - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetCachedInterfacePointers(BOOL           bIInspectableOnly,
                                                                      ULONG32        celt,
                                                                      ULONG32*       pcEltFetched,
                                                                      CORDB_ADDRESS* ptrs)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetCachedInterfacePointers - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetTarget(ICorDebugReferenceValue** ppObject)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetTarget - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetFunction(ICorDebugFunction** ppFunction)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetFunction - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetElementType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetElementType - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetRank(ULONG32* pnRank)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbArrayValue - GetRank - IMPLEMENTED\n"));
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ARRAY_REF, MDBGPROT_CMD_ARRAY_REF_GET_LENGTH, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        int             rank   = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
        *pnRank = rank;
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetCount(ULONG32* pnCount)
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);

        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ARRAY_REF, MDBGPROT_CMD_ARRAY_REF_GET_LENGTH, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        int             rank   = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
        m_nCount               = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
        LOG((LF_CORDB, LL_INFO1000000, "CordbArrayValue - GetCount - IMPLEMENTED\n"));
        *pnCount = m_nCount;
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetDimensions(ULONG32 cdim, ULONG32 dims[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetDimensions - IMPLEMENTED\n"));
    dims[0] = m_nCount;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::HasBaseIndicies(BOOL* pbHasBaseIndicies)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - HasBaseIndicies - NOT IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetBaseIndicies(ULONG32 cdim, ULONG32 indices[])
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbArrayValue - GetBaseIndicies - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetElement(ULONG32 cdim, ULONG32 indices[], ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);
        m_dbgprot_buffer_add_int(&localbuf, indices[cdim - 1]);
        m_dbgprot_buffer_add_int(&localbuf, 1);

        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ARRAY_REF, MDBGPROT_CMD_ARRAY_REF_GET_VALUES, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        hr = CordbObjectValue::CreateCordbValue(conn, pReply, ppValue);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}

HRESULT STDMETHODCALLTYPE CordbArrayValue::GetElementAtPosition(ULONG32 nPosition, ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    HRESULT hr = S_OK;
    EX_TRY
    {
        MdbgProtBuffer localbuf;
        m_dbgprot_buffer_init(&localbuf, 128);
        m_dbgprot_buffer_add_id(&localbuf, m_debuggerId);
        m_dbgprot_buffer_add_int(&localbuf, nPosition);
        m_dbgprot_buffer_add_int(&localbuf, 1);

        int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_ARRAY_REF, MDBGPROT_CMD_ARRAY_REF_GET_VALUES, &localbuf);
        m_dbgprot_buffer_free(&localbuf);
        ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
        CHECK_ERROR_RETURN_FALSE(received_reply_packet);
        MdbgProtBuffer* pReply = received_reply_packet->Buffer();
        hr = CordbObjectValue::CreateCordbValue(conn, pReply, ppValue);
    }
    EX_CATCH_HRESULT(hr);
    return hr;
}


CordbValueEnum::CordbValueEnum(Connection* conn, long nThreadDebuggerId, long nFrameDebuggerId, bool bIsArgument, ILCodeKind nFlags):CordbBaseMono(conn), CordbValueMono(ELEMENT_TYPE_VALUETYPE)
{
    LOG_METHOD_ENTRY;
    m_nThreadDebuggerId = nThreadDebuggerId;
    m_nFrameDebuggerId = nFrameDebuggerId;
    m_nCurrentValuePos = 0;
    m_nCount = 0;
    m_nFlags = nFlags;
    m_bIsArgument = bIsArgument;
    m_pValues = NULL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::Next(ULONG celt, ICorDebugValue* values[], ULONG* pceltFetched)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValueEnum - Next - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::Skip(ULONG celt)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValueEnum - Skip - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::Reset(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValueEnum - Reset - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::Clone(ICorDebugEnum** ppEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValueEnum - Clone - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::GetCount(ULONG* pcelt)
{
    LOG_METHOD_ENTRY;
    if (m_nFlags == ILCODE_REJIT_IL)
    {
        *pcelt = 0;
        return S_OK;
    }
    if (m_bIsArgument) {
        LOG((LF_CORDB, LL_INFO1000000, "CordbFrame - GetArgument - IMPLEMENTED\n"));
        HRESULT hr = S_OK;
        EX_TRY
        {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_nThreadDebuggerId);
            m_dbgprot_buffer_add_id(&localbuf, m_nFrameDebuggerId);
            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_STACK_FRAME, MDBGPROT_CMD_STACK_FRAME_GET_ARGUMENTS, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();
            m_nCount = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            *pcelt = m_nCount;
            m_pValues = new ICorDebugValue*[m_nCount];
            for (int i = 0; i < m_nCount; i++)
            {
                hr = CordbObjectValue::CreateCordbValue(conn, pReply, &m_pValues[i]);
            }
        }
        EX_CATCH_HRESULT(hr);
        return hr;
    }
    LOG((LF_CORDB, LL_INFO100000, "CordbValueEnum - GetCount - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueEnum::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValueEnum)
    {
        *pInterface = static_cast<ICorDebugValueEnum*>(this);
    }
    else if (id == IID_ICorDebugEnum)
    {
        *pInterface = static_cast<ICorDebugEnum*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(this);
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}




HRESULT STDMETHODCALLTYPE CordbValueType::GetType(CorElementType* pType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - GetType - IMPLEMENTED\n"));
    *pType = m_type;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetSize(ULONG32* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbValueType - GetSize - NOT IMPLEMENTED\n"));
    *pSize = 10;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetAddress(CORDB_ADDRESS* pAddress)
{
    LOG_METHOD_ENTRY;
    *pAddress = (CORDB_ADDRESS)m_pAddress;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValueType - GetAddress - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - CreateBreakpoint - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugValueMono)
    {
        *pInterface = static_cast<CordbValueMono*>(this);
        return S_OK;
    }
    if (id == IID_ICorDebugValue)
    {
        *pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugObjectValue*>(this));
    }
    else if (id == IID_ICorDebugValue2)
    {
        *pInterface = static_cast<ICorDebugValue2*>(this);
    }
    else if (id == IID_ICorDebugValue3)
    {
        *pInterface = static_cast<ICorDebugValue3*>(this);
    }
    else if (id == IID_ICorDebugObjectValue)
    {
        *pInterface = static_cast<ICorDebugObjectValue*>(this);
    }
    else if (id == IID_ICorDebugObjectValue2)
    {
        *pInterface = static_cast<ICorDebugObjectValue2*>(this);
    }
    else if (id == IID_IUnknown)
    {
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugObjectValue*>(this));
    }
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetExactType(ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValueType - GetExactType - IMPLEMENTED\n"));
    if (m_pClass != NULL)
    {
        m_pClass->GetCordbType()->QueryInterface(IID_ICorDebugType, (void**)ppType);
        return S_OK;
    }
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetSize64(ULONG64* pSize)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - GetSize64 - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetValue(void* pTo)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - GetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::SetValue(void* pFrom)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - SetValue - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::IsNull(BOOL* pbNull)
{
    LOG_METHOD_ENTRY;
    *pbNull = true;
    LOG((LF_CORDB, LL_INFO1000000, "CordbReferenceValue - IsNull - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetValue(CORDB_ADDRESS* pValue)
{
    LOG_METHOD_ENTRY;
    *pValue = (CORDB_ADDRESS)m_pAddress;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValueType - GetValue - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::SetValue(CORDB_ADDRESS value)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbReferenceValue - SetValue CORDB_ADDRESS - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}


CordbValueType::CordbValueType(Connection* conn, CorElementType type, CordbClass* klass, CORDB_ADDRESS cordbAddress)
    : CordbBaseMono(conn), CordbValueMono(type)
{
    LOG_METHOD_ENTRY;
    this->m_type       = type;
    this->conn         = conn;
    this->m_pClass     = klass;
    this->m_pAddress   = cordbAddress;
    if (klass)
        klass->InternalAddRef();
}

CordbValueType::~CordbValueType()
{
    LOG_METHOD_ENTRY;
    if (m_pClass)
        m_pClass->InternalRelease();
}

void CordbValueType::AddField(int token, ICorDebugValue* value)
{
    LOG_METHOD_ENTRY;
    m_fieldMap.Add(token, value);
    m_fields.Append(value);
}

void CordbValueType::WriteToBuffer(MdbgProtBuffer* buf)
{
    LOG_METHOD_ENTRY;
    m_dbgprot_buffer_add_byte(buf, m_type);
    m_dbgprot_buffer_add_byte(buf, false); //isenum
    m_dbgprot_buffer_add_byte(buf, false); //isboxed
    m_dbgprot_buffer_add_int(buf, m_pClass->GetDebuggerId()); //type_id
    m_dbgprot_buffer_add_int(buf, m_fields.GetCount()); //num fields
    for (DWORD i = 0; i < m_fields.GetCount(); i++)
    {
        ICorDebugValue* fieldValue = (ICorDebugValue*)m_fields.Get(i);
        if (fieldValue)
        {
            CordbValueMono* fieldValueMono;
            fieldValue->QueryInterface(IID_ICorDebugValueMono, (void**)&fieldValueMono);
            fieldValueMono->WriteToBuffer(buf);
        }
    }
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetClass(ICorDebugClass** ppClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbValueType - GetClass - IMPLEMENTED\n"));
    if (m_pClass != NULL)
    {
        m_pClass->QueryInterface(IID_ICorDebugClass, (void**)ppClass);
        return S_OK;
    }
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetFieldValue(ICorDebugClass* pClass, mdFieldDef fieldDef, ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    ICorDebugValue *value = NULL;
    m_fieldMap.Lookup(fieldDef, &value);
    value->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetVirtualMethod(mdMemberRef memberRef, ICorDebugFunction** ppFunction)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetContext(ICorDebugContext** ppContext)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::IsValueClass(BOOL* pbIsValueClass)
{
    LOG_METHOD_ENTRY;
    *pbIsValueClass = true;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetManagedCopy(IUnknown** ppObject)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::SetFromManagedCopy(IUnknown* pObject)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValueType::GetVirtualMethodAndType(mdMemberRef memberRef, ICorDebugFunction** ppFunction, ICorDebugType** ppType)
{
    LOG_METHOD_ENTRY;
    return E_NOTIMPL;
}
