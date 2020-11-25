#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_function.hpp>
#include <cordb_process.hpp>
#include <cordb_assembly.hpp>
#include <cordb_appdomain.hpp>
#include <cordb_class.hpp>
#include <cordb_symbol.hpp>

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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CorDebugAssembly - IsFullyTrusted - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbAssembly::GetAppDomain(
/* [out] */ ICorDebugAppDomain** ppAppDomain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CorDebugAssembly - GetAppDomain - IMPLEMENTED" << std::endl;
	*ppAppDomain = static_cast<ICorDebugAppDomain*>(pAppDomain);
	return S_OK;
}

	HRESULT CordbAssembly::EnumerateModules(
	/* [out] */ ICorDebugModuleEnum** ppModules)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CorDebugAssembly - EnumerateModules - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

	HRESULT CordbAssembly::GetCodeBase(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CorDebugAssembly - GetCodeBase - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

	HRESULT CordbAssembly::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CorDebugAssembly - GetName - NOT IMPLEMENTED" << std::endl;
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
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAssembly - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();

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
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();

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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - IsMappedLayout - IMPLEMENTED" << std::endl;
	return S_OK;
}
HRESULT CordbModule::CreateReaderForInMemorySymbols(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void** ppObj)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - CreateReaderForInMemorySymbols - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}
HRESULT CordbModule::SetJMCStatus(
	/* [in] */ BOOL bIsJustMyCode,
	/* [in] */ ULONG32 cTokens,
	/* [size_is][in] */ mdToken pTokens[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - SetJMCStatus - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::ApplyChanges(
	/* [in] */ ULONG cbMetadata,
	/* [size_is][in] */ BYTE pbMetadata[],
	/* [in] */ ULONG cbIL,
	/* [size_is][in] */ BYTE pbIL[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - ApplyChanges - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::SetJITCompilerFlags(
	/* [in] */ DWORD dwFlags)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - SetJITCompilerFlags - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetJITCompilerFlags(
	/* [out] */ DWORD* pdwFlags)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetJITCompilerFlags - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::ResolveAssembly(
	/* [in] */ mdToken tkAssemblyRef,
	/* [out] */ ICorDebugAssembly** ppAssembly)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - ResolveAssembly - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetProcess - NOT IMPLEMENTED" << std::endl;
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

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	assembly_metadata_blob = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &assembly_metadata_len);

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetBaseAddress" << std::endl;
	file.close();
	*pAddress = (CORDB_ADDRESS)assembly_metadata_blob;
	return S_OK;
}

HRESULT CordbModule::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	int cmdId = pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_LOCATION, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	char* assembly_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	fstream file;
	if (cchName < strlen(assembly_name) + 1) {
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetName - IMPLEMENTED - devolvi soh o tamanho" << std::endl;
		*pcchName = strlen(assembly_name) + 1;
		g_free(assembly_name);
		return S_OK;
	}
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetName - IMPLEMENTED - devolvi o tamanho e o valor" << std::endl;
	mbstowcs(szName, assembly_name, strlen(assembly_name)+1);
	*pcchName = strlen(assembly_name) + 1;
	g_free(assembly_name);
	return S_OK;
}

HRESULT CordbModule::EnableJITDebugging(
	/* [in] */ BOOL bTrackJITInfo,
	/* [in] */ BOOL bAllowJitOpts)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - EnableJITDebugging - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::EnableClassLoadCallbacks(
	/* [in] */ BOOL bClassLoadCallbacks)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - EnableClassLoadCallbacks - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromToken(
	/* [in] */ mdMethodDef methodDef,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	//check in a cache before talk to mono runtime to get info
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetFunctionFromToken - IMPLEMENTED" << std::endl;
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	buffer_add_int(&localbuf, methodDef);
	int cmdId = pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	CordbFunction* func = NULL;
	func = pProcess->cordb->findFunction(id);
	if (func == NULL)
	{
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CRIANDO FUNCAO" << std::endl;
		func = new CordbFunction(methodDef, id, this);
		g_ptr_array_add(pProcess->cordb->functions, func);
	}
	*ppFunction = func;
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromRVA(
	/* [in] */ CORDB_ADDRESS rva,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetFunctionFromRVA - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetClassFromToken(
	/* [in] */ mdTypeDef typeDef,
	/* [out] */ ICorDebugClass** ppClass)
{
	CordbClass* pClass = new CordbClass(typeDef);
	*ppClass = static_cast<ICorDebugClass*>(pClass);
	return S_OK;
}

HRESULT CordbModule::CreateBreakpoint(
	/* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - CreateBreakpoint - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetEditAndContinueSnapshot(
	/* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetEditAndContinueSnapshot - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetMetaDataInterface(
	/* [in] */ REFIID riid,
	/* [out] */ IUnknown** ppObj)
{
	if (pCordbSymbol == NULL)
		pCordbSymbol = new CordbSymbol(pAssembly);
	pCordbSymbol->QueryInterface(riid, (void**)ppObj);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetMetaDataInterface - IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetToken(
	/* [out] */ mdModule* pToken)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetToken - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::IsDynamic(
	/* [out] */ BOOL* pDynamic)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - IsDynamic - IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetGlobalVariableValue(
	/* [in] */ mdFieldDef fieldDef,
	/* [out] */ ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetGlobalVariableValue - NOT IMPLEMENTED" << std::endl;
	return S_OK;
}

HRESULT CordbModule::GetSize(
	/* [out] */ ULONG32* pcBytes)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - GetSize -IMPLEMENTED" << std::endl;
	*pcBytes = assembly_metadata_len;
	return S_OK;
}

HRESULT CordbModule::IsInMemory(
	/* [out] */ BOOL* pInMemory)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbModule - IsInMemory - IMPLEMENTED" << std::endl;
	return S_OK;
}
