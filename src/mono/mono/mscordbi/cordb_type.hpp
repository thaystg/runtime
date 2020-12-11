#ifndef __MONO_DEBUGGER_CORDB_TYPE_H__
#define __MONO_DEBUGGER_CORDB_TYPE_H__

#include <cordb.hpp>

class CordbType:
    public ICorDebugType,
    public ICorDebugType2
{
    CorElementType type;
    CordbClass* klass;
public:
    CordbType(CorElementType type, CordbClass* klass = NULL);
    HRESULT STDMETHODCALLTYPE GetType(CorElementType* ty);
    HRESULT STDMETHODCALLTYPE GetClass(ICorDebugClass** ppClass);
    HRESULT STDMETHODCALLTYPE EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum);
    HRESULT STDMETHODCALLTYPE GetFirstTypeParameter(ICorDebugType** value);
    HRESULT STDMETHODCALLTYPE GetBase(ICorDebugType** pBase);
    HRESULT STDMETHODCALLTYPE GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue);
    HRESULT STDMETHODCALLTYPE GetRank(ULONG32* pnRank);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetTypeID(COR_TYPEID* id);
};

class CordbTypeEnum :
    public ICorDebugTypeEnum
{
public:
    virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, ICorDebugType* values[], ULONG* pceltFetched);
    HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
    HRESULT STDMETHODCALLTYPE Reset(void);
    HRESULT STDMETHODCALLTYPE Clone(ICorDebugEnum** ppEnum);
    HRESULT STDMETHODCALLTYPE GetCount(ULONG* pcelt);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

#endif
