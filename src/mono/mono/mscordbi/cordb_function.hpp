#ifndef __MONO_DEBUGGER_CORDB_FUNCTION_H__
#define __MONO_DEBUGGER_CORDB_FUNCTION_H__

#include <cordb.hpp>
#include <cordb_assembly.hpp>

class CordbFunction :
    public ICorDebugFunction,
    public ICorDebugFunction2,
    public ICorDebugFunction3,
    public ICorDebugFunction4
{
public:
    int id;
    mdToken token;
    CordbCode* code;
    CordbModule* module;
    Connection *connection;

    CordbFunction(mdToken token, int id, CordbModule *module, Connection *connection);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetModule(/* [out] */ ICorDebugModule** ppModule);
    HRESULT STDMETHODCALLTYPE GetClass(/* [out] */ ICorDebugClass** ppClass);
    HRESULT STDMETHODCALLTYPE GetToken(/* [out] */ mdMethodDef* pMethodDef);
    HRESULT STDMETHODCALLTYPE GetILCode(/* [out] */ ICorDebugCode** ppCode);
    HRESULT STDMETHODCALLTYPE GetNativeCode(/* [out] */ ICorDebugCode** ppCode);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(/* [out] */ ICorDebugFunctionBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE GetLocalVarSigToken(/* [out] */ mdSignature* pmdSig);
    HRESULT STDMETHODCALLTYPE GetCurrentVersionNumber(/* [out] */ ULONG32* pnCurrentVersion);
    HRESULT STDMETHODCALLTYPE SetJMCStatus(/* [in] */ BOOL bIsJustMyCode);
    HRESULT STDMETHODCALLTYPE GetJMCStatus(/* [out] */ BOOL* pbIsJustMyCode);
    HRESULT STDMETHODCALLTYPE EnumerateNativeCode(/* [out] */ ICorDebugCodeEnum** ppCodeEnum);
    HRESULT STDMETHODCALLTYPE GetVersionNumber(/* [out] */ ULONG32* pnVersion);
    HRESULT STDMETHODCALLTYPE GetActiveReJitRequestILCode(ICorDebugILCode** ppReJitedILCode);
    HRESULT STDMETHODCALLTYPE CreateNativeBreakpoint(ICorDebugFunctionBreakpoint** ppBreakpoint);
};

#endif
