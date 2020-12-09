#ifndef __MONO_DEBUGGER_CORDB_CLASS_H__
#define __MONO_DEBUGGER_CORDB_CLASS_H__

#include <cordb.hpp>


class CordbClass :
    public ICorDebugClass,
    public ICorDebugClass2
{
    Connection* conn;
    mdToken token;
public:
    CordbClass(Connection* conn, mdToken token);
    HRESULT STDMETHODCALLTYPE GetModule(ICorDebugModule** pModule);
    HRESULT STDMETHODCALLTYPE GetToken(mdTypeDef* pTypeDef);
    HRESULT STDMETHODCALLTYPE GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetParameterizedType(CorElementType elementType, ULONG32 nTypeArgs, ICorDebugType* ppTypeArgs[], ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE SetJMCStatus(BOOL bIsJustMyCode);
};


#endif
