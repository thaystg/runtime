#ifndef __MONO_DEBUGGER_CORDB_ASSEMBLY_H__
#define __MONO_DEBUGGER_CORDB_ASSEMBLY_H__

#include <cordb.hpp>

class CordbModule : 
	public ICorDebugModule,
	public ICorDebugModule2,
	public ICorDebugModule3,
	public ICorDebugModule4
{
public:
	int id; //id on mono side;
	CordbProcess* pProcess;
	CordbSymbol* pCordbSymbol;
	CordbAssembly* pAssembly;
    guint8* assembly_metadata_blob;
    guint32 assembly_metadata_len;

    CordbModule(CordbProcess* process, CordbAssembly* assembly, int id_assembly);

    HRESULT CordbModule::QueryInterface(REFIID id, void** pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE IsMappedLayout(/* [out] */ BOOL* pIsMapped);
    HRESULT STDMETHODCALLTYPE CreateReaderForInMemorySymbols(/* [in] */ REFIID riid,/* [iid_is][out] */ void** ppObj);
    HRESULT STDMETHODCALLTYPE SetJMCStatus(/* [in] */ BOOL bIsJustMyCode,/* [in] */ ULONG32 cTokens,/* [size_is][in] */ mdToken pTokens[]);
    HRESULT STDMETHODCALLTYPE ApplyChanges(/* [in] */ ULONG cbMetadata,/* [size_is][in] */ BYTE pbMetadata[],/* [in] */ ULONG cbIL,/* [size_is][in] */ BYTE pbIL[]);
    HRESULT STDMETHODCALLTYPE SetJITCompilerFlags(/* [in] */ DWORD dwFlags);
    HRESULT STDMETHODCALLTYPE GetJITCompilerFlags(/* [out] */ DWORD* pdwFlags);
    HRESULT STDMETHODCALLTYPE ResolveAssembly(/* [in] */ mdToken tkAssemblyRef,/* [out] */ ICorDebugAssembly** ppAssembly);
    HRESULT STDMETHODCALLTYPE GetProcess(/* [out] */ ICorDebugProcess** ppProcess);
    HRESULT STDMETHODCALLTYPE GetBaseAddress(/* [out] */ CORDB_ADDRESS* pAddress);
    HRESULT STDMETHODCALLTYPE GetAssembly(/* [out] */ ICorDebugAssembly** ppAssembly);
    HRESULT STDMETHODCALLTYPE GetName(/* [in] */ ULONG32 cchName,/* [out] */ ULONG32* pcchName,/* [length_is][size_is][out] */ WCHAR szName[]);
    HRESULT STDMETHODCALLTYPE EnableJITDebugging(/* [in] */ BOOL bTrackJITInfo,/* [in] */ BOOL bAllowJitOpts);
    HRESULT STDMETHODCALLTYPE EnableClassLoadCallbacks(/* [in] */ BOOL bClassLoadCallbacks);
    HRESULT STDMETHODCALLTYPE GetFunctionFromToken(/* [in] */ mdMethodDef methodDef,/* [out] */ ICorDebugFunction** ppFunction);
    HRESULT STDMETHODCALLTYPE GetFunctionFromRVA(/* [in] */ CORDB_ADDRESS rva,/* [out] */ ICorDebugFunction** ppFunction);
    HRESULT STDMETHODCALLTYPE GetClassFromToken(/* [in] */ mdTypeDef typeDef,/* [out] */ ICorDebugClass** ppClass);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(/* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE GetEditAndContinueSnapshot(/* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot);
    HRESULT STDMETHODCALLTYPE GetMetaDataInterface(/* [in] */ REFIID riid,/* [out] */ IUnknown** ppObj);
    HRESULT STDMETHODCALLTYPE GetToken(/* [out] */ mdModule* pToken);
    HRESULT STDMETHODCALLTYPE IsDynamic(/* [out] */ BOOL* pDynamic);
    HRESULT STDMETHODCALLTYPE GetGlobalVariableValue(/* [in] */ mdFieldDef fieldDef,/* [out] */ ICorDebugValue** ppValue);
    HRESULT STDMETHODCALLTYPE GetSize(/* [out] */ ULONG32* pcBytes);
    HRESULT STDMETHODCALLTYPE IsInMemory(/* [out] */ BOOL* pInMemory);
};


class CordbAssembly :
	public ICorDebugAssembly, 
	public ICorDebugAssembly2
{
public:
	CordbProcess* pProcess;
	CordbAppDomain* pAppDomain;
	int id;
    CordbAssembly(CordbProcess* process, CordbAppDomain* appDomain, int id_assembly);
    HRESULT STDMETHODCALLTYPE IsFullyTrusted(/* [out] */ BOOL* pbFullyTrusted);
	HRESULT STDMETHODCALLTYPE GetProcess(/* [out] */ ICorDebugProcess** ppProcess);
    HRESULT STDMETHODCALLTYPE GetAppDomain(/* [out] */ ICorDebugAppDomain** ppAppDomain);
    HRESULT STDMETHODCALLTYPE EnumerateModules(/* [out] */ ICorDebugModuleEnum** ppModules);
    HRESULT STDMETHODCALLTYPE GetCodeBase(/* [in] */ ULONG32 cchName,/* [out] */ ULONG32* pcchName,/* [length_is][size_is][out] */ WCHAR szName[]);
    HRESULT STDMETHODCALLTYPE GetName(/* [in] */ ULONG32 cchName,/* [out] */ ULONG32* pcchName,/* [length_is][size_is][out] */ WCHAR szName[]);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

#endif