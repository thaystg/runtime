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
	DEBUG_PRINTF(1, "CordbValue - GetValue - NOT IMPLEMENTED\n");
	memcpy(pTo, &value, size);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::SetValue(void* pFrom)
{
	DEBUG_PRINTF(1, "CordbValue - SetValue - NOT IMPLEMENTED\n");
	return S_OK;
}
