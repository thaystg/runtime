#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-breakpoint.hpp>
#include <cordb-function.hpp>
#include <cordb-class.hpp>
#include <cordb-value.hpp>

using namespace std;

CordbClass::CordbClass (Connection *conn, mdToken token, int module_id) {
	this->token = token;
	this->conn = conn;
	this->module_id = module_id;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetModule (ICorDebugModule **pModule) {
	DEBUG_PRINTF (1, "CordbClass - GetModule - IMPLEMENTED - %d\n", module_id);
	if (pModule) {
		*pModule = (ICorDebugModule*)g_hash_table_lookup (conn->ppCordb->modules, GINT_TO_POINTER (module_id));
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetToken (mdTypeDef *pTypeDef) {
	DEBUG_PRINTF (1, "CordbClass - GetToken - IMPLEMENTED - %d\n", module_id);
	*pTypeDef = token;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetStaticFieldValue (mdFieldDef fieldDef, ICorDebugFrame *pFrame,
                                                           ICorDebugValue **ppValue) {
	DEBUG_PRINTF (1, "CordbClass - GetStaticFieldValue - NOT IMPLEMENTED - %d\n", fieldDef);
	CordbContent content_value;
	content_value.booleanValue = 0;
	CordbValue *value = new CordbValue (conn, ELEMENT_TYPE_BOOLEAN, content_value, 1);
	*ppValue = static_cast<ICorDebugValue*> (value);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::QueryInterface (REFIID id, void **pInterface) {
	if (id == IID_ICorDebugClass) {
		*pInterface = static_cast<ICorDebugClass*> (this);
	}
	else if (id == IID_ICorDebugClass2) {
		*pInterface = static_cast<ICorDebugClass2*> (this);
	}
	else if (id == IID_IUnknown) {
		*pInterface = static_cast<IUnknown*> (static_cast<ICorDebugClass*> (this));
	}
	else {
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbClass::AddRef (void) {
	return 0;
}

ULONG STDMETHODCALLTYPE CordbClass::Release (void) {
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetParameterizedType (CorElementType elementType, ULONG32 nTypeArgs,
                                                            ICorDebugType *ppTypeArgs[], ICorDebugType **ppType) {
	DEBUG_PRINTF (1, "CordbClass - GetParameterizedType - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::SetJMCStatus (BOOL bIsJustMyCode) {
	DEBUG_PRINTF (1, "CordbClass - SetJMCStatus - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
