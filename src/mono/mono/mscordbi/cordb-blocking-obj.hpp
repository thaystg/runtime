#ifndef __MONO_DEBUGGER_CORDB_BLOCKING_OBJ_H__
#define __MONO_DEBUGGER_CORDB_BLOCKING_OBJ_H__

#include <cordb.hpp>

class CordbBlockingObjectEnum :
    public ICorDebugBlockingObjectEnum
{

public:
    HRESULT STDMETHODCALLTYPE Next(ULONG celt, CorDebugBlockingObject values[], ULONG* pceltFetched);
    HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
    HRESULT STDMETHODCALLTYPE Reset(void);
    HRESULT STDMETHODCALLTYPE Clone(ICorDebugEnum** ppEnum);
    HRESULT STDMETHODCALLTYPE GetCount(ULONG* pcelt);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

#endif
