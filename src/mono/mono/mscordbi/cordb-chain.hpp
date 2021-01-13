#ifndef __MONO_DEBUGGER_CORDB_CHAIN_H__
#define __MONO_DEBUGGER_CORDB_CHAIN_H__

#include <cordb.hpp>


class CordbChainEnum: 
public ICorDebugChainEnum
{
public:
    CordbThread* thread;
    CordbChainEnum(CordbThread* thread);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE Skip(/* [in] */ ULONG celt);
    HRESULT STDMETHODCALLTYPE Reset(void);
    HRESULT STDMETHODCALLTYPE Clone(/* [out] */ ICorDebugEnum** ppEnum);
    HRESULT STDMETHODCALLTYPE GetCount(/* [out] */ ULONG* pcelt);
    HRESULT STDMETHODCALLTYPE Next(/* [in] */ ULONG celt,/* [length_is][size_is][out] */ ICorDebugChain* chains[],/* [out] */ ULONG* pceltFetched);
};

class CordbChain :
    public ICorDebugChain
{
public:
    CordbThread* thread;
    CorDebugChainReason chain_reason;
    bool is_managed;
    CordbChain(CordbThread* thread, CorDebugChainReason chain_reason, bool is_managed);
    HRESULT STDMETHODCALLTYPE GetThread(/* [out] */ ICorDebugThread** ppThread);
    HRESULT STDMETHODCALLTYPE GetStackRange(/* [out] */ CORDB_ADDRESS* pStart,/* [out] */ CORDB_ADDRESS* pEnd);
    HRESULT STDMETHODCALLTYPE GetContext(/* [out] */ ICorDebugContext** ppContext);
    HRESULT STDMETHODCALLTYPE GetCaller(/* [out] */ ICorDebugChain** ppChain);
    HRESULT STDMETHODCALLTYPE GetCallee(/* [out] */ ICorDebugChain** ppChain);
    HRESULT STDMETHODCALLTYPE GetPrevious(/* [out] */ ICorDebugChain** ppChain);
    HRESULT STDMETHODCALLTYPE GetNext(/* [out] */ ICorDebugChain** ppChain);
    HRESULT STDMETHODCALLTYPE IsManaged(/* [out] */ BOOL* pManaged);
    HRESULT STDMETHODCALLTYPE EnumerateFrames(/* [out] */ ICorDebugFrameEnum** ppFrames);
    HRESULT STDMETHODCALLTYPE GetActiveFrame(/* [out] */ ICorDebugFrame** ppFrame);
    HRESULT STDMETHODCALLTYPE GetRegisterSet(/* [out] */ ICorDebugRegisterSet** ppRegisters);
    HRESULT STDMETHODCALLTYPE GetReason(/* [out] */ CorDebugChainReason* pReason);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

#endif