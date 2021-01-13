#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-function.hpp>
#include <cordb-process.hpp>
#include <cordb-assembly.hpp>
#include <cordb-appdomain.hpp>
#include <cordb-class.hpp>
#include <cordb-symbol.hpp>

using namespace std;


CordbAssembly::CordbAssembly(CordbProcess* process, CordbAppDomain* appDomain, int id_assembly)
{
	pProcess = process;
	pAppDomain = appDomain;
	id = id_assembly;
}

HRESULT CordbAssembly::IsFullyTrusted(
/* [out] */ BOOL* pbFullyTrusted)
{
	*pbFullyTrusted = true;
	DEBUG_PRINTF(1, "CorDebugAssembly - IsFullyTrusted - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbAssembly::GetAppDomain(
/* [out] */ ICorDebugAppDomain** ppAppDomain)
{
	DEBUG_PRINTF(1, "CorDebugAssembly - GetAppDomain - IMPLEMENTED\n");
*ppAppDomain = static_cast<ICorDebugAppDomain*>(pAppDomain);
	return S_OK;
}

	HRESULT CordbAssembly::EnumerateModules(
	/* [out] */ ICorDebugModuleEnum** ppModules)
{
	DEBUG_PRINTF(1, "CorDebugAssembly - EnumerateModules - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::GetCodeBase(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	DEBUG_PRINTF(1, "CorDebugAssembly - GetCodeBase - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	DEBUG_PRINTF(1, "CorDebugAssembly - GetName - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
{
	if (id == IID_ICorDebugAssembly)
		*ppInterface = static_cast<ICorDebugAssembly*>(this);
	else if (id == IID_ICorDebugAssembly2)
		*ppInterface = static_cast<ICorDebugAssembly2*>(this);
	else if (id == IID_IUnknown)
		*ppInterface = static_cast<IUnknown*>(static_cast<ICorDebugAssembly*>(this));
	else
	{
		DEBUG_PRINTF(1, "CordbAssembly - QueryInterface - E_NOTIMPL\n");
	

		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG CordbAssembly::AddRef(void)
{
	return S_OK;
}

ULONG CordbAssembly::Release(void)
{
	return S_OK;
}



CordbModule::CordbModule(CordbProcess* process, CordbAssembly* assembly, int id_assembly)
{
	pProcess = process;
	pCordbSymbol = NULL;
	pAssembly = assembly;
	id = id_assembly;
	dwFlags = 0;
}

HRESULT CordbModule::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugModule)
	{
		*pInterface = static_cast<ICorDebugModule*>(this);
	}
	else if (id == IID_ICorDebugModule2)
	{
		*pInterface = static_cast<ICorDebugModule2*>(this);
	}
	else if (id == IID_ICorDebugModule3)
	{
		*pInterface = static_cast<ICorDebugModule3*>(this);
	}
	else if (id == IID_ICorDebugModule4)
	{
		*pInterface = static_cast<ICorDebugModule4*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugModule*>(this));
	}
	else
	{
		DEBUG_PRINTF(1, "CordbModule - QueryInterface - E_NOTIMPL\n");
	

		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG CordbModule::AddRef(void)
{
	return S_OK;
}

ULONG CordbModule::Release(void)
{
	return S_OK;
}
HRESULT CordbModule::IsMappedLayout(
	/* [out] */ BOOL* pIsMapped)
{
	*pIsMapped = FALSE;
	DEBUG_PRINTF(1, "CordbModule - IsMappedLayout - IMPLEMENTED\n");
	return S_OK;
}
HRESULT CordbModule::CreateReaderForInMemorySymbols(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void** ppObj)
{
	DEBUG_PRINTF(1, "CordbModule - CreateReaderForInMemorySymbols - NOT IMPLEMENTED\n");
	return S_OK;
}
HRESULT CordbModule::SetJMCStatus(
	/* [in] */ BOOL bIsJustMyCode,
	/* [in] */ ULONG32 cTokens,
	/* [size_is][in] */ mdToken pTokens[])
{
	DEBUG_PRINTF(1, "CordbModule - SetJMCStatus - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::ApplyChanges(
	/* [in] */ ULONG cbMetadata,
	/* [size_is][in] */ BYTE pbMetadata[],
	/* [in] */ ULONG cbIL,
	/* [size_is][in] */ BYTE pbIL[])
{
	DEBUG_PRINTF(1, "CordbModule - ApplyChanges - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::SetJITCompilerFlags(
	/* [in] */ DWORD dwFlags)
{
	this->dwFlags = dwFlags;
	DEBUG_PRINTF(1, "CordbModule - SetJITCompilerFlags - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetJITCompilerFlags(
	/* [out] */ DWORD* pdwFlags)
{
	*pdwFlags = dwFlags;
	DEBUG_PRINTF(1, "CordbModule - GetJITCompilerFlags - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::ResolveAssembly(
	/* [in] */ mdToken tkAssemblyRef,
	/* [out] */ ICorDebugAssembly** ppAssembly)
{
	DEBUG_PRINTF(1, "CordbModule - ResolveAssembly - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	DEBUG_PRINTF(1, "CordbModule - GetProcess - NOT IMPLEMENTED\n");
// *ppProcess = pProcess;
	return S_OK;
}

HRESULT CordbModule::GetBaseAddress(
	/* [out] */ CORDB_ADDRESS* pAddress)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	int cmdId = pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METADATA_BLOB, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pProcess->connection->get_answer(cmdId);
	assembly_metadata_blob = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &assembly_metadata_len);

	DEBUG_PRINTF(1, "CordbModule - GetBaseAddress\n");

	*pAddress = (CORDB_ADDRESS)assembly_metadata_blob;
	return S_OK;
}

HRESULT CordbModule::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	DEBUG_PRINTF(1, "CordbModule - GetName - IMPLEMENTED\n");
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	int cmdId = pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_LOCATION, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pProcess->connection->get_answer(cmdId);
	char* assembly_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	
	DEBUG_PRINTF(1, "CordbModule - assembly_name - %s\n", assembly_name);

	if (cchName < strlen(assembly_name) + 1) {
		*pcchName = strlen(assembly_name) + 1;
		g_free(assembly_name);
		return S_OK;
	}
	mbstowcs(szName, assembly_name, strlen(assembly_name)+1);
	*pcchName = strlen(assembly_name) + 1;
	g_free(assembly_name);
	return S_OK;
}












HRESULT CordbModule::EnableJITDebugging(
	/* [in] */ BOOL bTrackJITInfo,
	/* [in] */ BOOL bAllowJitOpts)
{
	DEBUG_PRINTF(1, "CordbModule - EnableJITDebugging - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::EnableClassLoadCallbacks(
	/* [in] */ BOOL bClassLoadCallbacks)
{
	DEBUG_PRINTF(1, "CordbModule - EnableClassLoadCallbacks - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromToken(
	/* [in] */ mdMethodDef methodDef,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	//check in a cache before talk to mono runtime to get info
	DEBUG_PRINTF(1, "CordbModule - GetFunctionFromToken - IMPLEMENTED\n");
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	buffer_add_int(&localbuf, methodDef);
	int cmdId = pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pProcess->connection->get_answer(cmdId);
	int id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	CordbFunction* func = NULL;
	func = pProcess->cordb->findFunction(id);
	if (func == NULL)
	{
		func = new CordbFunction(methodDef, id, this, pProcess->connection);
		g_ptr_array_add(pProcess->cordb->functions, func);
	}
	*ppFunction = func;
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromRVA(
	/* [in] */ CORDB_ADDRESS rva,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	DEBUG_PRINTF(1, "CordbModule - GetFunctionFromRVA - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetClassFromToken(
	/* [in] */ mdTypeDef typeDef,
	/* [out] */ ICorDebugClass** ppClass)
{
	CordbClass* pClass = new CordbClass(pProcess->connection, typeDef, id);
	*ppClass = static_cast<ICorDebugClass*>(pClass);
	return S_OK;
}

HRESULT CordbModule::CreateBreakpoint(
	/* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint)
{
	DEBUG_PRINTF(1, "CordbModule - CreateBreakpoint - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetEditAndContinueSnapshot(
	/* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot)
{
	DEBUG_PRINTF(1, "CordbModule - GetEditAndContinueSnapshot - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetMetaDataInterface(
	/* [in] */ REFIID riid,
	/* [out] */ IUnknown** ppObj)
{
	if (pCordbSymbol == NULL)
		pCordbSymbol = new CordbSymbol(pAssembly);
	pCordbSymbol->QueryInterface(riid, (void**)ppObj);
	DEBUG_PRINTF(1, "CordbModule - GetMetaDataInterface - IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetToken(
	/* [out] */ mdModule* pToken)
{
	DEBUG_PRINTF(1, "CordbModule - GetToken - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::IsDynamic(
	/* [out] */ BOOL* pDynamic)
{
	DEBUG_PRINTF(1, "CordbModule - IsDynamic - IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetGlobalVariableValue(
	/* [in] */ mdFieldDef fieldDef,
	/* [out] */ ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbModule - GetGlobalVariableValue - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetSize(
	/* [out] */ ULONG32* pcBytes)
{
	DEBUG_PRINTF(1, "CordbModule - GetSize -IMPLEMENTED\n");
	*pcBytes = assembly_metadata_len;
	return S_OK;
}

HRESULT CordbModule::IsInMemory(
	/* [out] */ BOOL* pInMemory)
{
	DEBUG_PRINTF(1, "CordbModule - IsInMemory - IMPLEMENTED\n");
	return S_OK;
}
