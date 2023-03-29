// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-TYPE.CPP
//

#include <cordb-breakpoint.h>
#include <cordb-class.h>
#include <cordb-type.h>
#include <cordb-process.h>
#include <cordb.h>

using namespace std;

CordbType::CordbType(CorElementType type, Connection* conn, CordbClass* klass)
    : CordbBaseMono(conn)
{
    LOG_METHOD_ENTRY;
    if (type == ELEMENT_TYPE_CLASS && klass == NULL)
        assert(0);
    this->m_pClass         = klass;
    this->m_type           = type;
    m_pTypeParameterList   = NULL;
    m_pTypeEnum            = NULL;
    if (klass)
        klass->InternalAddRef();
}

CordbType* CordbType::CreateArrayType(CorElementType type, Connection* conn, CordbType* arrayType)
{
    LOG_METHOD_ENTRY;
    CordbType* ret = new CordbType(type, conn);
    ArrayList *m_pTypeParameterList = new ArrayList();
    arrayType->InternalAddRef();
    m_pTypeParameterList->Append(arrayType);
    ret->SetTypeParameterList(m_pTypeParameterList);
    return ret;
}

void CordbType::SetTypeParameterList(ArrayList* pTypeParameterList)
{
    LOG_METHOD_ENTRY;
    m_pTypeParameterList = pTypeParameterList;
}

CordbType::~CordbType()
{
    LOG_METHOD_ENTRY;
    if (m_pClass)
        m_pClass->InternalRelease();

    for (DWORD i = 0; i < m_pTypeParameterList->GetCount(); i++)
    {
        CordbType* m_pTypeParameter = (CordbType*)m_pTypeParameterList->Get(i);
        if (m_pTypeParameter)
            m_pTypeParameter->InternalRelease();
    }
    delete m_pTypeParameterList;
    if (m_pTypeEnum)
        m_pTypeEnum->InternalRelease();
}

HRESULT STDMETHODCALLTYPE CordbType::GetType(CorElementType* ty)
{
    LOG_METHOD_ENTRY;
    *ty = m_type;
    LOG((LF_CORDB, LL_INFO1000000, "CordbType - GetType - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetClass(ICorDebugClass** ppClass)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbType - GetClass - IMPLEMENTED\n"));
    if (!m_pClass)
    {
        LOG((LF_CORDB, LL_INFO100000, "CordbType - GetClass - NO CLASS\n"));
        return S_OK;
    }
    m_pClass->QueryInterface(IID_ICorDebugClass, (void**)ppClass);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
    LOG_METHOD_ENTRY;
    if (m_pTypeEnum == NULL)
    {
        if (m_pClass)
        {
            MdbgProtBuffer localbuf;
            m_dbgprot_buffer_init(&localbuf, 128);
            m_dbgprot_buffer_add_id(&localbuf, m_pClass->GetDebuggerId());
            m_dbgprot_buffer_add_int(&localbuf, MONO_TYPE_NAME_FORMAT_FULL_NAME);
            int cmdId = conn->SendEvent(MDBGPROT_CMD_SET_TYPE, MDBGPROT_CMD_TYPE_GET_INFO, &localbuf);
            m_dbgprot_buffer_free(&localbuf);

            ReceivedReplyPacket* received_reply_packet = conn->GetReplyWithError(cmdId);
            CHECK_ERROR_RETURN_FALSE(received_reply_packet);
            MdbgProtBuffer* pReply = received_reply_packet->Buffer();

            char* namespace_str      = m_dbgprot_decode_string(pReply->p, &pReply->p, pReply->end);
            char* class_name_str     = m_dbgprot_decode_string(pReply->p, &pReply->p, pReply->end);
            char* class_fullname_str = m_dbgprot_decode_string(pReply->p, &pReply->p, pReply->end);
            int   assembly_id        = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
            int   module_id          = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
            int type_id1             = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
            int type_id2             = m_dbgprot_decode_id(pReply->p, &pReply->p, pReply->end);
            int token                = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int rank                 = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            int flags                = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int flags2               = m_dbgprot_decode_byte(pReply->p, &pReply->p, pReply->end);
            int nestedClass          = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            for (int i = 0; i < nestedClass; i++)
            {
                int typeNestedId = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            }
            int typeIdG = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int g_inst_count        = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
            int gInstId = 0;
            if (g_inst_count > 0)
                m_pTypeParameterList = new ArrayList();
            for (int i = 0; i < g_inst_count; i++)
            {
                gInstId = m_dbgprot_decode_int(pReply->p, &pReply->p, pReply->end);
                CordbClass *m_pClassParm = conn->GetProcess()->FindOrAddClass(gInstId);
                CordbType *type = conn->GetProcess()->FindOrAddClassType(ELEMENT_TYPE_CLASS, m_pClassParm);
                type->InternalAddRef();
                m_pTypeParameterList->Append(type);
            }
        }
        m_pTypeEnum = new CordbTypeEnum(conn, m_pTypeParameterList);
        m_pTypeEnum->InternalAddRef();
    }
    m_pTypeEnum->QueryInterface(IID_ICorDebugTypeEnum, (void**)ppTyParEnum);

    LOG((LF_CORDB, LL_INFO1000000, "CordbType - EnumerateTypeParameters - IMPLEMENTED\n"));
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetFirstTypeParameter(ICorDebugType** value)
{
    LOG_METHOD_ENTRY;
    if (m_pTypeParameterList && m_pTypeParameterList->GetCount() > 0)
        ((CordbType*)m_pTypeParameterList->Get(0))->QueryInterface(IID_ICorDebugType, (void**)value);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetBase(ICorDebugType** pBase)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO1000000, "CordbType - GetBase - IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetStaticFieldValue(mdFieldDef       fieldDef,
                                                         ICorDebugFrame*  pFrame,
                                                         ICorDebugValue** ppValue)
{
    LOG_METHOD_ENTRY;
    if (m_pClass) {
        return m_pClass->GetStaticFieldValue(fieldDef, pFrame, ppValue);
    }
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetRank(ULONG32* pnRank)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbType - GetRank - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugType)
        *pInterface = static_cast<ICorDebugType*>(this);
    else if (id == IID_ICorDebugType2)
        *pInterface = static_cast<ICorDebugType2*>(this);
    else if (id == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugType*>(this));
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetTypeID(COR_TYPEID* id)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbType - GetTypeID - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

CordbTypeEnum::CordbTypeEnum(Connection* conn, ArrayList* typeList) : CordbBaseMono(conn)
{
    LOG_METHOD_ENTRY;
    this->m_pTypeList = typeList;
}

CordbTypeEnum::~CordbTypeEnum()
{
    LOG_METHOD_ENTRY;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Next(ULONG celt, ICorDebugType* values[], ULONG* pceltFetched)
{
    LOG_METHOD_ENTRY;
    if (m_pTypeList != NULL)
    {
        for (DWORD i = 0; i < m_pTypeList->GetCount(); i++)
        {
            CordbType* m_pTypeParameter = (CordbType*)m_pTypeList->Get(i);
            if (m_pTypeParameter)
                m_pTypeParameter->QueryInterface(IID_ICorDebugType, (void**)&values[i]);
        }
        *pceltFetched = m_pTypeList->GetCount();
    }
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Skip(ULONG celt)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbTypeEnum - Skip - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Reset(void)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbTypeEnum - Reset - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Clone(ICorDebugEnum** ppEnum)
{
    LOG_METHOD_ENTRY;
    LOG((LF_CORDB, LL_INFO100000, "CordbTypeEnum - Clone - NOT IMPLEMENTED\n"));
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::GetCount(ULONG* pcelt)
{
    LOG_METHOD_ENTRY;
    *pcelt = m_pTypeList ? m_pTypeList->GetCount() : 0;
    return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::QueryInterface(REFIID id, void** pInterface)
{
    LOG_METHOD_ENTRY;
    if (id == IID_ICorDebugEnum)
        *pInterface = static_cast<ICorDebugEnum*>(this);
    else if (id == IID_ICorDebugTypeEnum)
        *pInterface = static_cast<ICorDebugTypeEnum*>(this);
    else if (id == IID_IUnknown)
        *pInterface = static_cast<IUnknown*>(static_cast<ICorDebugTypeEnum*>(this));
    else
    {
        *pInterface = NULL;
        return E_NOINTERFACE;
    }
    AddRef();
    return S_OK;
}
