#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-function.hpp>
#include <cordb-code.hpp>
#include <cordb-process.hpp>

using namespace std;

CordbFunction::CordbFunction (mdToken token, int id, CordbModule *module, Connection *connection) {
	this->token = token;
	this->id = id;
	code = NULL;
	this->module = module;
	this->connection = connection;
}

HRESULT __stdcall CordbFunction::QueryInterface (REFIID id, void **pInterface) {
	if (id == IID_ICorDebugFunction) {
		*pInterface = static_cast<ICorDebugFunction*> (this);
	}
	else if (id == IID_ICorDebugFunction2) {
		*pInterface = static_cast<ICorDebugFunction2*> (this);
	}
	else if (id == IID_ICorDebugFunction3) {
		*pInterface = static_cast<ICorDebugFunction3*> (this);
	}
	else if (id == IID_ICorDebugFunction4) {
		*pInterface = static_cast<ICorDebugFunction4*> (this);
	}
	else if (id == IID_IUnknown) {
		*pInterface = static_cast<IUnknown*> (static_cast<ICorDebugFunction*> (this));
	}
	else {
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG __stdcall CordbFunction::AddRef (void) {
	return 0;
}

ULONG __stdcall CordbFunction::Release (void) {
	return 0;
}

HRESULT __stdcall CordbFunction::GetModule (ICorDebugModule **ppModule) {
	if (module == NULL) {
		Buffer localbuf;
		buffer_init (&localbuf, 128);
		buffer_add_id (&localbuf, id);
		int cmdId = connection->send_event (CMD_SET_METHOD, CMD_METHOD_ASSEMBLY, &localbuf);
		buffer_free (&localbuf);

		DEBUG_PRINTF (1, "CordbFunction - GetModule - IMPLEMENTED - ENTREI NO 0.1 - %d\n", id);

		Buffer *localbuf2 = connection->get_answer (cmdId);

		int module_id = decode_id (localbuf2->buf, &localbuf2->buf, localbuf2->end);

		DEBUG_PRINTF (1, "CordbFunction - GetModule - IMPLEMENTED - ENTREI NO 0.2 - %d\n", module_id);

		module = (CordbModule*)g_hash_table_lookup (connection->ppCordb->modules, GINT_TO_POINTER (module_id));
	}

	*ppModule = static_cast<ICorDebugModule*> (this->module);
	DEBUG_PRINTF (1, "CordbFunction - GetModule - IMPLEMENTED - %p\n", this->module);

	if (!*ppModule)
		return S_FALSE;
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetClass (ICorDebugClass **ppClass) {
	DEBUG_PRINTF (1, "CordbFunction - GetClass - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetToken (mdMethodDef *pMethodDef) {
	if (this->token == 0) {
		DEBUG_PRINTF (1, "CordbFunction - GetToken - IMPLEMENTED - ENTREI NO 0 - %d\n", id);
		Buffer localbuf;
		buffer_init (&localbuf, 128);
		buffer_add_id (&localbuf, id);
		int cmdId = connection->send_event (CMD_SET_METHOD, CMD_METHOD_TOKEN, &localbuf);
		buffer_free (&localbuf);

		DEBUG_PRINTF (1, "CordbFunction - GetToken - IMPLEMENTED - ENTREI NO 0.1 - %d\n", id);

		Buffer *localbuf2 = connection->get_answer (cmdId);

		this->token = decode_int (localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}
	*pMethodDef = this->token;
	DEBUG_PRINTF (1, "CordbFunction - GetToken - IMPLEMENTED - %d\n", *pMethodDef);
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetILCode (ICorDebugCode **ppCode) {
	if (code == NULL)
		code = new CordbCode (this);
	*ppCode = static_cast<ICorDebugCode*> (code);
	DEBUG_PRINTF (1, "CordbFunction - GetILCode - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetNativeCode (ICorDebugCode **ppCode) {
	*ppCode = static_cast<ICorDebugCode*> (code);
	DEBUG_PRINTF (1, "CordbFunction - GetNativeCode - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbFunction::CreateBreakpoint (ICorDebugFunctionBreakpoint **ppBreakpoint) {
	DEBUG_PRINTF (1, "CordbFunction - CreateBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetLocalVarSigToken (mdSignature *pmdSig) {
	DEBUG_PRINTF (1, "CordbFunction - GetLocalVarSigToken - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetCurrentVersionNumber (ULONG32 *pnCurrentVersion) {
	*pnCurrentVersion = 1;
	DEBUG_PRINTF (1, "CordbFunction - GetCurrentVersionNumber - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbFunction::SetJMCStatus (BOOL bIsJustMyCode) {
	DEBUG_PRINTF (1, "CordbFunction - SetJMCStatus - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetJMCStatus (BOOL *pbIsJustMyCode) {
	DEBUG_PRINTF (1, "CordbFunction - GetJMCStatus - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::EnumerateNativeCode (ICorDebugCodeEnum **ppCodeEnum) {
	DEBUG_PRINTF (1, "CordbFunction - EnumerateNativeCode - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetVersionNumber (ULONG32 *pnVersion) {
	*pnVersion = 1;
	DEBUG_PRINTF (1, "CordbFunction - GetVersionNumber - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetActiveReJitRequestILCode (ICorDebugILCode **ppReJitedILCode) {
	DEBUG_PRINTF (1, "CordbFunction - GetActiveReJitRequestILCode - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::CreateNativeBreakpoint (ICorDebugFunctionBreakpoint **ppBreakpoint) {
	DEBUG_PRINTF (1, "CordbFunction - CreateNativeBreakpoint - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
