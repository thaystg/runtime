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


CordbValue::CordbValue(CorElementType type, int value, int size)
{
	this->type = type;
	this->value = value;
	this->size = size;
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - GetAddress - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - CreateBreakpoint - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - GetExactType - IMPLEMENTED" << std::endl;
	CordbType *tp = new CordbType(type);
	*ppType = static_cast<ICorDebugType*>(tp);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize64(ULONG64* pSize)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - GetSize64 - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetValue(void* pTo)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - GetValue - NOT IMPLEMENTED" << std::endl; \
	memcpy(pTo, &value, size);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::SetValue(void* pFrom)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbValue - SetValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
