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

CordbClass::CordbClass(mdToken token)
{
	this->token = token;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetModule(ICorDebugModule** pModule)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetModule - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetToken(mdTypeDef* pTypeDef)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue)
{
	CordbValue* value = new CordbValue(ELEMENT_TYPE_BOOLEAN, 0, 1);
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbClass - GetParameterizedType - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::SetJMCStatus(BOOL bIsJustMyCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbClass - SetJMCStatus - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}
