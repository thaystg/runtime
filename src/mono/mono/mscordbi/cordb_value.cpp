#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_breakpoint.hpp>
#include <cordb_function.hpp>
#include <cordb_class.hpp>
#include <cordb_value.hpp>
#include <cordb_type.hpp>

using namespace std;


CordbValue::CordbValue(Connection* conn, CorElementType type, CordbContent value, int size)
{
	this->type = type;
	this->value = value;
	if (type == MONO_TYPE_I8 || type == MONO_TYPE_U8 || type == MONO_TYPE_R8)
	{
		DEBUG_PRINTF(1, "CordbValue - LONG %ld - %d\n", this->value.longValue, size);
	}
	this->size = size;
	this->conn = conn;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetType(CorElementType* pType)
{
	*pType = type;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize(ULONG32* pSize)
{
	*pSize = size;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetAddress(CORDB_ADDRESS* pAddress)
{
	*pAddress = (CORDB_ADDRESS)&value;
	DEBUG_PRINTF(1, "CordbValue - GetAddress - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
	DEBUG_PRINTF(1, "CordbValue - CreateBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugValue)
	{
		*pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugGenericValue*>(this));
	}
	else if (id == IID_ICorDebugValue2)
	{
		*pInterface = static_cast<ICorDebugValue2*>(this);
	}
	else if (id == IID_ICorDebugValue3)
	{
		*pInterface = static_cast<ICorDebugValue3*>(this);
	}
	else if (id == IID_ICorDebugGenericValue)
	{
		*pInterface = static_cast<ICorDebugGenericValue*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugGenericValue*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbValue::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbValue::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetExactType(ICorDebugType** ppType)
{
	DEBUG_PRINTF(1, "CordbValue - GetExactType - IMPLEMENTED\n");
	CordbType *tp = new CordbType(type);
	*ppType = static_cast<ICorDebugType*>(tp);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize64(ULONG64* pSize)
{
	DEBUG_PRINTF(1, "CordbValue - GetSize64 - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetValue(void* pTo)
{
	DEBUG_PRINTF(1, "CordbValue - GetValue - IMPLEMENTED\n");
	memcpy(pTo, &value, size);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::SetValue(void* pFrom)
{
	DEBUG_PRINTF(1, "CordbValue - SetValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetType(CorElementType* pType)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - GetType - IMPLEMENTED\n");
	*pType = type;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetSize(ULONG32* pSize)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - GetSize - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetAddress(CORDB_ADDRESS* pAddress)
{
	*pAddress = (CORDB_ADDRESS)&object_id;
	DEBUG_PRINTF(1, "CordbReferenceValue - GetAddress - IMPLEMENTED\n");
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - CreateBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugValue)
	{
		*pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugReferenceValue*>(this));
	}
	else if (id == IID_ICorDebugValue2)
	{
		*pInterface = static_cast<ICorDebugValue2*>(this);
	}
	else if (id == IID_ICorDebugValue3)
	{
		*pInterface = static_cast<ICorDebugValue3*>(this);
	}
	else if (id == IID_ICorDebugReferenceValue)
	{
		*pInterface = static_cast<ICorDebugReferenceValue*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugReferenceValue*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
ULONG STDMETHODCALLTYPE CordbReferenceValue::AddRef(void)
{
	return 1;
}
ULONG STDMETHODCALLTYPE CordbReferenceValue::Release(void)
{
	return 1;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetExactType(ICorDebugType** ppType)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - GetExactType - IMPLEMENTED\n");
	CordbType* tp = new CordbType(type);
	*ppType = static_cast<ICorDebugType*>(tp);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetSize64(ULONG64* pSize)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - GetSize64 - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetValue(void* pTo)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - GetValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::SetValue(void* pFrom)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - SetValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::IsNull(/* [out] */ BOOL* pbNull)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - IsNull - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::GetValue(/* [out] */ CORDB_ADDRESS* pValue)
{
	*pValue = (CORDB_ADDRESS)&object_id;
	DEBUG_PRINTF(1, "CordbReferenceValue - GetValue - IMPLEMENTED\n");
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::SetValue(/* [in] */ CORDB_ADDRESS value)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - SetValue CORDB_ADDRESS - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::Dereference(/* [out] */ ICorDebugValue** ppValue)
{
	CordbObjectValue* objectValue = new CordbObjectValue(conn, type, object_id);
	objectValue->QueryInterface(IID_ICorDebugValue, (void**)ppValue);
	DEBUG_PRINTF(1, "CordbReferenceValue - Dereference - IMPLEMENTED\n");
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbReferenceValue::DereferenceStrong(/* [out] */ ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbReferenceValue - DereferenceStrong - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

CordbReferenceValue::CordbReferenceValue(Connection* conn, CorElementType type, int object_id) {
	this->type = type;
	this->object_id = object_id;
	this->conn = conn;
}

CordbObjectValue::CordbObjectValue(Connection* conn, CorElementType type, int object_id) {
	this->type = type;
	this->object_id = object_id;
	this->conn = conn;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetType(CorElementType* pType){
	DEBUG_PRINTF(1, "CordbObjectValue - GetType - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbObjectValue::GetSize(ULONG32* pSize){
	DEBUG_PRINTF(1, "CordbObjectValue - GetSize - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetAddress(CORDB_ADDRESS* pAddress){
	*pAddress = (CORDB_ADDRESS)&object_id;
	DEBUG_PRINTF(1, "CordbObjectValue - GetAddress - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint) {
	DEBUG_PRINTF(1, "CordbObjectValue - CreateBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbObjectValue::AddRef(void) {
	return 1;
}

ULONG STDMETHODCALLTYPE CordbObjectValue::Release(void) {
	return 1;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetExactType(ICorDebugType** ppType) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetExactType - IMPLEMENTED\n");
	CordbType* tp = new CordbType(type);
	*ppType = static_cast<ICorDebugType*>(tp);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetSize64(ULONG64* pSize) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetSize64 - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetValue(void* pTo) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::SetValue(void* pFrom) {
	DEBUG_PRINTF(1, "CordbObjectValue - SetValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetVirtualMethodAndType(mdMemberRef memberRef, ICorDebugFunction** ppFunction, ICorDebugType** ppType) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetVirtualMethodAndType - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetLength(ULONG32* pcchString) {
	if (type == ELEMENT_TYPE_STRING) 
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, object_id);
		buffer_add_int(&localbuf, 0);
		buffer_add_int(&localbuf, -1);

		int cmdId = conn->send_event(CMD_SET_STRING_REF, CMD_STRING_REF_GET_LENGTH, &localbuf);
		buffer_free(&localbuf);
		Buffer* localbuf2 = conn->get_answer(cmdId);
		*pcchString = decode_long(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetString(ULONG32 cchString, ULONG32* pcchString, WCHAR szString[]) {
	if (type == ELEMENT_TYPE_STRING) 
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, object_id);
		buffer_add_int(&localbuf, 0);
		buffer_add_int(&localbuf, -1);

		int cmdId = conn->send_event(CMD_SET_STRING_REF, CMD_STRING_REF_GET_VALUE, &localbuf);
		buffer_free(&localbuf);
		Buffer* localbuf2 = conn->get_answer(cmdId);
		*pcchString = cchString;
		char *value = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbObjectValue - GetString - %s\n", value);
		if (cchString >= strlen(value)) {
			mbstowcs(szString, value, strlen(value)+1);
			*pcchString = strlen(value);
		}
		return S_OK;
	}
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateHandle(CorDebugHandleType type, ICorDebugHandleValue** ppHandle) {
	DEBUG_PRINTF(1, "CordbObjectValue - CreateHandle - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetThreadOwningMonitorLock(ICorDebugThread** ppThread, DWORD* pAcquisitionCount) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetThreadOwningMonitorLock - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetMonitorEventWaitList(ICorDebugThreadEnum** ppThreadEnum) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetMonitorEventWaitList - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::EnumerateExceptionCallStack(ICorDebugExceptionObjectCallStackEnum** ppCallStackEnum) {
	DEBUG_PRINTF(1, "CordbObjectValue - EnumerateExceptionCallStack - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetCachedInterfaceTypes(BOOL bIInspectableOnly, ICorDebugTypeEnum** ppInterfacesEnum) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetCachedInterfaceTypes - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetCachedInterfacePointers(BOOL bIInspectableOnly, ULONG32 celt, ULONG32* pcEltFetched, CORDB_ADDRESS* ptrs) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetCachedInterfacePointers - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetTarget(ICorDebugReferenceValue** ppObject) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetTarget - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetFunction(ICorDebugFunction** ppFunction) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetFunction - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetClass(/* [out] */ ICorDebugClass** ppClass) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetClass - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetFieldValue(ICorDebugClass* pClass, mdFieldDef fieldDef, ICorDebugValue** ppValue) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetFieldValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetVirtualMethod(mdMemberRef memberRef, ICorDebugFunction** ppFunction) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetVirtualMethod - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetContext(ICorDebugContext** ppContext) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetContext - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::IsValueClass(BOOL* pbIsValueClass) {
	DEBUG_PRINTF(1, "CordbObjectValue - IsValueClass - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::GetManagedCopy(IUnknown** ppObject) {
	DEBUG_PRINTF(1, "CordbObjectValue - GetManagedCopy - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbObjectValue::SetFromManagedCopy(IUnknown* pObject) {
	DEBUG_PRINTF(1, "CordbObjectValue - SetFromManagedCopy - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::IsValid(BOOL* pbValid) {
	DEBUG_PRINTF(1, "CordbObjectValue - IsValid - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::CreateRelocBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint) {
	DEBUG_PRINTF(1, "CordbObjectValue - CreateRelocBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbObjectValue::QueryInterface(REFIID id, void** pInterface) {
	if (id == IID_ICorDebugValue)
	{
		*pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugObjectValue*>(this));
	}
	else if (id == IID_ICorDebugValue2)
	{
		*pInterface = static_cast<ICorDebugValue2*>(this);
	}
	else if (id == IID_ICorDebugValue3)
	{
		*pInterface = static_cast<ICorDebugValue3*>(this);
	}
	else if (id == IID_ICorDebugObjectValue)
	{
		*pInterface = static_cast<ICorDebugObjectValue*>(this);
	}
	else if (id == IID_ICorDebugObjectValue2)
	{
		*pInterface = static_cast<ICorDebugObjectValue2*>(this);
	}
	else if (id == IID_ICorDebugGenericValue)
	{
		*pInterface = static_cast<ICorDebugGenericValue*>(this);
	}
	else if (id == IID_ICorDebugHeapValue)
	{
		*pInterface = static_cast<ICorDebugHeapValue*>(this);
	}
	else if (id == IID_ICorDebugHeapValue2)
	{
		*pInterface = static_cast<ICorDebugHeapValue2*>(this);
	}
	else if (id == IID_ICorDebugHeapValue3)
	{
		*pInterface = static_cast<ICorDebugHeapValue3*>(this);
	}
	else if ((id == IID_ICorDebugStringValue) &&
		(type == ELEMENT_TYPE_STRING))
	{
		*pInterface = static_cast<ICorDebugStringValue*>(this);
	}
	else /*if (id == IID_ICorDebugExceptionObjectValue && m_fIsExceptionObject)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugExceptionObjectValue*>(this));
	}
	else if (id == IID_ICorDebugComObjectValue && m_fIsRcw)
	{
		*pInterface = static_cast<ICorDebugComObjectValue*>(this);
	}
	else if (id == IID_ICorDebugDelegateObjectValue && m_fIsDelegate)
	{
		*pInterface = static_cast<ICorDebugDelegateObjectValue*>(this);
	}
	else*/ if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugObjectValue*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
