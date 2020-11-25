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

using namespace std;


CordbAppDomain::CordbAppDomain(ICorDebugProcess* ppProcess)
{
	pProcess = ppProcess;
}

HRESULT CordbAppDomain::Stop(/* [in] */ DWORD dwTimeoutIgnored)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - Stop - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::Continue(/* [in] */ BOOL fIsOutOfBand)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - Continue - NOT IMPLEMENTED" << std::endl;
	file.close();
	pProcess->Continue(fIsOutOfBand);
	return S_OK;
}

HRESULT CordbAppDomain::IsRunning(/* [out] */ BOOL* pbRunning)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - IsRunning - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::HasQueuedCallbacks(/* [in] */ ICorDebugThread* pThread,/* [out] */ BOOL* pbQueued)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - HasQueuedCallbacks - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::EnumerateThreads(/* [out] */ ICorDebugThreadEnum** ppThreads)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - EnumerateThreads - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::SetAllThreadsDebugState(/* [in] */ CorDebugThreadState state,/* [in] */ ICorDebugThread* pExceptThisThread)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - SetAllThreadsDebugState - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::Detach(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - Detach - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::Terminate(/* [in] */ UINT exitCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - Terminate - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::CanCommitChanges(/* [in] */ ULONG cSnapshots,/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],/* [out] */ ICorDebugErrorInfoEnum** pError)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - CanCommitChanges - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::CommitChanges(/* [in] */ ULONG cSnapshots,/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],/* [out] */ ICorDebugErrorInfoEnum** pError)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - CommitChanges - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
{
	if (id == IID_ICorDebugAppDomain)
	{
		*ppInterface = (ICorDebugAppDomain*)this;
	}
	else if (id == IID_ICorDebugAppDomain2)
	{
		*ppInterface = (ICorDebugAppDomain2*)this;
	}
	else if (id == IID_ICorDebugAppDomain3)
	{
		*ppInterface = (ICorDebugAppDomain3*)this;
	}
	else if (id == IID_ICorDebugAppDomain4)
	{
		*ppInterface = (ICorDebugAppDomain4*)this;
	}
	else if (id == IID_ICorDebugController)
		*ppInterface = (ICorDebugController*)(ICorDebugAppDomain*)this;
	else if (id == IID_IUnknown)
		*ppInterface = (IUnknown*)(ICorDebugAppDomain*)this;
	else
	{
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG CordbAppDomain::AddRef(void)
{
	return S_OK;
}

ULONG CordbAppDomain::Release(void)
{
	return S_OK;
}

HRESULT CordbAppDomain::GetProcess(
/* [out] */ ICorDebugProcess** ppProcess)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetProcess - NOT IMPLEMENTED" << std::endl;
	file.close();
	*ppProcess = pProcess;
	return S_OK;
}

HRESULT CordbAppDomain::EnumerateAssemblies(/* [out] */ ICorDebugAssemblyEnum** ppAssemblies)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - EnumerateAssemblies - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetModuleFromMetaDataInterface(/* [in] */ IUnknown* pIMetaData,/* [out] */ ICorDebugModule** ppModule)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetModuleFromMetaDataInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::EnumerateBreakpoints(/* [out] */ ICorDebugBreakpointEnum** ppBreakpoints)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - EnumerateBreakpoints - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::EnumerateSteppers(/* [out] */ ICorDebugStepperEnum** ppSteppers)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - EnumerateSteppers - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::IsAttached(/* [out] */ BOOL* pbAttached)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - IsAttached - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetName(/* [in] */ ULONG32 cchName,/* [out] */ ULONG32* pcchName,/* [length_is][size_is][out] */ WCHAR szName[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetName - IMPLEMENTED" << std::endl;
	if (cchName < strlen("DefaultDomain")) {
		*pcchName = strlen("DefaultDomain") + 1;
		return S_OK;
	}
	wcscpy(szName, L"DefaultDomain");
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetObject(/* [out] */ ICorDebugValue** ppObject)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetObject - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::Attach(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - Attach - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetID(/* [out] */ ULONG32* pId)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetID - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbAppDomain::GetArrayOrPointerType(/* [in] */ CorElementType elementType,/* [in] */ ULONG32 nRank,/* [in] */ ICorDebugType* pTypeArg,/* [out] */ ICorDebugType** ppType)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetArrayOrPointerType - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetFunctionPointerType(/* [in] */ ULONG32 nTypeArgs,/* [size_is][in] */ ICorDebugType* ppTypeArgs[],/* [out] */ ICorDebugType** ppType)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetFunctionPointerType - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbAppDomain::GetCachedWinRTTypesForIIDs(/* [in] */ ULONG32 cReqTypes,/* [size_is][in] */ GUID* iidsToResolve,/* [out] */ ICorDebugTypeEnum** ppTypesEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetCachedWinRTTypesForIIDs - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetCachedWinRTTypes(/* [out] */ ICorDebugGuidToTypeEnum** ppGuidToTypeEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetCachedWinRTTypes - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbAppDomain::GetObjectForCCW(/* [in] */ CORDB_ADDRESS ccwPointer,/* [out] */ ICorDebugValue** ppManagedObject)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbAppDomain - GetObjectForCCW - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}