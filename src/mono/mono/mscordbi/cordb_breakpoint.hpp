#ifndef __MONO_DEBUGGER_CORDB_BREAKPOINT_H__
#define __MONO_DEBUGGER_CORDB_BREAKPOINT_H__

#include <cordb.hpp>

class CordbFunctionBreakpoint :
    public ICorDebugFunctionBreakpoint
{
public:
    CordbCode* code;
    ULONG32 offset;
    CordbFunctionBreakpoint(CordbCode* code, ULONG32 offset);
    HRESULT STDMETHODCALLTYPE GetFunction(/* [out] */ ICorDebugFunction** ppFunction);
    HRESULT STDMETHODCALLTYPE GetOffset(/* [out] */ ULONG32* pnOffset);
    HRESULT STDMETHODCALLTYPE Activate(/* [in] */ BOOL bActive);
    HRESULT STDMETHODCALLTYPE IsActive(/* [out] */ BOOL* pbActive);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

#endif
