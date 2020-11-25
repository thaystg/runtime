#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_function.hpp>
#include <cordb_process.hpp>

using namespace std;

CordbProcess::CordbProcess()
{
    suspended = false;
}

HRESULT CordbProcess::EnumerateLoaderHeapMemoryRegions(
	/* [out] */ ICorDebugMemoryRangeEnum** ppRanges) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateLoaderHeapMemoryRegions - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnableGCNotificationEvents(
	BOOL fEnable) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnableGCNotificationEvents - NOT IMPLEMENTED" << std::endl; 
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnableExceptionCallbacksOutsideOfMyCode(
	/* [in] */ BOOL enableExceptionsOutsideOfJMC) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnableExceptionCallbacksOutsideOfMyCode - NOT IMPLEMENTED" << std::endl; 
	file.close();
	return S_OK;
}

HRESULT CordbProcess::SetWriteableMetadataUpdateMode(
	WriteableMetadataUpdateMode flags) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetWriteableMetadataUpdateMode - NOT IMPLEMENTED" << std::endl; 
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetGCHeapInformation(
	/* [out] */ COR_HEAPINFO* pHeapInfo) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetGCHeapInformation - NOT IMPLEMENTED" << std::endl; 
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateHeap(
	/* [out] */ ICorDebugHeapEnum** ppObjects) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateHeap - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateHeapRegions(
	/* [out] */ ICorDebugHeapSegmentEnum** ppRegions) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateHeapRegions - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetObject(
	/* [in] */ CORDB_ADDRESS addr,
	/* [out] */ ICorDebugObjectValue** pObject) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetObject - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateGCReferences(
	/* [in] */ BOOL enumerateWeakReferences,
	/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateGCReferences - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateHandles(
	/* [in] */ CorGCReferenceType types,
	/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateHandles - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetTypeID(
	/* [in] */ CORDB_ADDRESS obj,
	/* [out] */ COR_TYPEID* pId) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetTypeID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetTypeForTypeID(
	/* [in] */ COR_TYPEID id,
	/* [out] */ ICorDebugType** ppType) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetTypeForTypeID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetArrayLayout(
	/* [in] */ COR_TYPEID id,
	/* [out] */ COR_ARRAY_LAYOUT* pLayout) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetArrayLayout - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetTypeLayout(
	/* [in] */ COR_TYPEID id,
	/* [out] */ COR_TYPE_LAYOUT* pLayout) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetTypeLayout - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetTypeFields(
	/* [in] */ COR_TYPEID id,
	ULONG32 celt,
	COR_FIELD fields[],
	ULONG32* pceltNeeded) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetTypeFields - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnableNGENPolicy(
	/* [in] */ CorDebugNGENPolicy ePolicy) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnableNGENPolicy - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbProcess::Filter(
	/* [size_is][length_is][in] */ const BYTE pRecord[],
	/* [in] */ DWORD countBytes,
	/* [in] */ CorDebugRecordFormat format,
	/* [in] */ DWORD dwFlags,
	/* [in] */ DWORD dwThreadId,
	/* [in] */ ICorDebugManagedCallback* pCallback,
	/* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Filter - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ProcessStateChanged(
	/* [in] */ CorDebugStateChange eChange) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ProcessStateChanged - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbProcess::SetEnableCustomNotification(
	ICorDebugClass* pClass,
	BOOL fEnable) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetEnableCustomNotification - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbProcess::GetID(
	/* [out] */ DWORD* pdwProcessId) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetHandle(
	/* [out] */ HPROCESS* phProcessHandle) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetHandle - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetThread(
	/* [in] */ DWORD dwThreadId,
	/* [out] */ ICorDebugThread** ppThread) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetThread - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateObjects(
	/* [out] */ ICorDebugObjectEnum** ppObjects) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateObjects - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::IsTransitionStub(
	/* [in] */ CORDB_ADDRESS address,
	/* [out] */ BOOL* pbTransitionStub) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - IsTransitionStub - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::IsOSSuspended(
	/* [in] */ DWORD threadID,
	/* [out] */ BOOL* pbSuspended) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - IsOSSuspended - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetThreadContext(
	/* [in] */ DWORD threadID,
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][out][in] */ BYTE context[]) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetThreadContext - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::SetThreadContext(
	/* [in] */ DWORD threadID,
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][in] */ BYTE context[]) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetThreadContext - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ReadMemory(
/* [in] */ CORDB_ADDRESS address,
/* [in] */ DWORD size,
/* [length_is][size_is][out] */ BYTE buffer[],
/* [out] */ SIZE_T* read) {
	memcpy(buffer, (void*)address, size);
	if (read != NULL)
		*read = size;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ReadMemory - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::WriteMemory(
	/* [in] */ CORDB_ADDRESS address,
	/* [in] */ DWORD size,
	/* [size_is][in] */ BYTE buffer[],
	/* [out] */ SIZE_T* written) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - WriteMemory - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ClearCurrentException(
	/* [in] */ DWORD threadID) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ClearCurrentException - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnableLogMessages(
	/* [in] */ BOOL fOnOff) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnableLogMessages - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ModifyLogSwitch(
	/* [annotation][in] */
	_In_  WCHAR* pLogSwitchName,
	/* [in] */ LONG lLevel) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ModifyLogSwitch - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateAppDomains(
	/* [out] */ ICorDebugAppDomainEnum** ppAppDomains) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateAppDomains - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetObject(
	/* [out] */ ICorDebugValue** ppObject) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetObject - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ThreadForFiberCookie(
	/* [in] */ DWORD fiberCookie,
	/* [out] */ ICorDebugThread** ppThread) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ThreadForFiberCookie - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetHelperThreadID(
	/* [out] */ DWORD* pThreadID) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "GetHelperThreadID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbProcess::GetThreadForTaskID(
	/* [in] */ TASKID taskid,
	/* [out] */ ICorDebugThread2** ppThread) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetHelperThreadID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetVersion(
	/* [out] */ COR_VERSION* version) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetVersion - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::SetUnmanagedBreakpoint(
	/* [in] */ CORDB_ADDRESS address,
	/* [in] */ ULONG32 bufsize,
	/* [length_is][size_is][out] */ BYTE buffer[],
	/* [out] */ ULONG32* bufLen) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetUnmanagedBreakpoint - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::ClearUnmanagedBreakpoint(
	/* [in] */ CORDB_ADDRESS address) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - ClearUnmanagedBreakpoint - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::SetDesiredNGENCompilerFlags(
	/* [in] */ DWORD pdwFlags) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetDesiredNGENCompilerFlags - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetDesiredNGENCompilerFlags(
	/* [out] */ DWORD* pdwFlags)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetDesiredNGENCompilerFlags - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::GetReferenceValueFromGCHandle(
	/* [in] */ UINT_PTR handle,
	/* [out] */ ICorDebugReferenceValue** pOutValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - GetReferenceValueFromGCHandle - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT CordbProcess::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
	if (id == IID_ICorDebugProcess)
	{
		*pInterface = static_cast<ICorDebugProcess*>(this);
	}
	else if (id == IID_ICorDebugController)
	{
		*pInterface = static_cast<ICorDebugController*>(static_cast<ICorDebugProcess*>(this));
	}
	else if (id == IID_ICorDebugProcess2)

	{
		*pInterface = static_cast<ICorDebugProcess2*>(this);
	}
	else if (id == IID_ICorDebugProcess3)
	{
		*pInterface = static_cast<ICorDebugProcess3*>(this);
	}
	else if (id == IID_ICorDebugProcess4)
	{
		*pInterface = static_cast<ICorDebugProcess4*>(this);
	}
	else if (id == IID_ICorDebugProcess5)
	{
		*pInterface = static_cast<ICorDebugProcess5*>(this);
	}
	else if (id == IID_ICorDebugProcess7)
	{
		*pInterface = static_cast<ICorDebugProcess7*>(this);
	}
	else if (id == IID_ICorDebugProcess8)
	{
		*pInterface = static_cast<ICorDebugProcess8*>(this);
	}
	else if (id == IID_ICorDebugProcess10)
	{
		*pInterface = static_cast<ICorDebugProcess10*>(this);
	}
	else if (id == IID_ICorDebugProcess11)
	{
		*pInterface = static_cast<ICorDebugProcess11*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugProcess*>(this));
	}

	else
	{
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();

		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG CordbProcess::AddRef(void)
{
	return S_OK;
}

ULONG CordbProcess::Release(void)
{
	return S_OK;
}

HRESULT CordbProcess::Stop(
/* [in] */ DWORD dwTimeoutIgnored) {
	if (suspended)
	{
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Stop - ALREADY SUSPENDED" << std::endl;
		file.close();
		return S_OK;
	}
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Stop - IMPLEMENTED" << std::endl;
	file.close();
	Buffer sendbuf;
	buffer_init(&sendbuf, 128);
	connection->send_event(CMD_SET_VM, CMD_VM_SUSPEND, &sendbuf);
	suspended = true;
	return S_OK;
}

HRESULT CordbProcess::Continue(
	/* [in] */ BOOL fIsOutOfBand)
{
	if (suspended == false) {
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "RESUME SEM ESTAR SUSPENSO - fIsOutOfBand " << std::endl;
		return S_OK;
	}
	suspended = false;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Continue - IMPLEMENTED" << std::endl;
	file.close();
	Buffer sendbuf;
	buffer_init(&sendbuf, 128);
	connection->send_event(CMD_SET_VM, CMD_VM_RESUME, &sendbuf);
	return S_OK;
}

HRESULT CordbProcess::IsRunning(
	/* [out] */ BOOL* pbRunning) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - IsRunning - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::HasQueuedCallbacks(
	/* [in] */ ICorDebugThread* pThread,
	/* [out] */ BOOL* pbQueued) {
	//connection->process_packet_from_queue();
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - HasQueuedCallbacks - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::EnumerateThreads(
	/* [out] */ ICorDebugThreadEnum** ppThreads) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - EnumerateThreads - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::SetAllThreadsDebugState(
	/* [in] */ CorDebugThreadState state,
	/* [in] */ ICorDebugThread* pExceptThisThread) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - SetAllThreadsDebugState - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::Detach(void) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Detach - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::Terminate(
	/* [in] */ UINT exitCode) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - Terminate - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::CanCommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - CanCommitChanges - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbProcess::CommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbProcess - CommitChanges - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
