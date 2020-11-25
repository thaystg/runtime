#ifndef __MONO_DEBUGGER_CORDB_VALUE_H__
#define __MONO_DEBUGGER_CORDB_VALUE_H__

#include <cordb.hpp>

class CordbValue :
    public ICorDebugValue2, 
    public ICorDebugValue3,
    public ICorDebugGenericValue
{
    CorElementType type;
    int value;
    int size;
public:
    CordbValue(CorElementType type, int value, int size);
    HRESULT STDMETHODCALLTYPE GetType(CorElementType* pType);
    HRESULT STDMETHODCALLTYPE GetSize(ULONG32* pSize);
    HRESULT STDMETHODCALLTYPE GetAddress(CORDB_ADDRESS* pAddress);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetExactType(ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetSize64(ULONG64* pSize);
    HRESULT STDMETHODCALLTYPE GetValue(void* pTo);
    HRESULT STDMETHODCALLTYPE SetValue(void* pFrom);
};


#endif