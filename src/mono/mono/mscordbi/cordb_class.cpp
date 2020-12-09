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

using namespace std;

CordbClass::CordbClass(Connection *conn, mdToken token)
{
	this->token = token;
	this->conn = conn;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetModule(ICorDebugModule** pModule)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetModule - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetToken(mdTypeDef* pTypeDef)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetToken - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue)
{
	CordbContent content_value;
	content_value.booleanValue = 0;
	CordbValue* value = new CordbValue(conn, ELEMENT_TYPE_BOOLEAN, content_value, 1);
	*ppValue = static_cast<ICorDebugValue*>(value);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugClass)
	{
		*pInterface = static_cast<ICorDebugClass*>(this);
	}
	else if (id == IID_ICorDebugClass2)
	{
		*pInterface = static_cast<ICorDebugClass2*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugClass*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbClass::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbClass::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetParameterizedType(CorElementType elementType, ULONG32 nTypeArgs, ICorDebugType* ppTypeArgs[], ICorDebugType** ppType)
{
	DEBUG_PRINTF(1, "CordbClass - GetParameterizedType - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::SetJMCStatus(BOOL bIsJustMyCode)
{
	DEBUG_PRINTF(1, "CordbClass - SetJMCStatus - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
