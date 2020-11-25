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
#include <cordb_blocking_obj.hpp>

using namespace std;


HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Next(ULONG celt, CorDebugBlockingObject values[], ULONG* pceltFetched)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbBlockingObjectEnum - Next - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Skip(ULONG celt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbBlockingObjectEnum - Skip - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Reset(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbBlockingObjectEnum - Reset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Clone(ICorDebugEnum** ppEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbBlockingObjectEnum - Clone - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::GetCount(ULONG* pcelt)
{
	pcelt = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::QueryInterface(REFIID riid, void** ppvObject)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbBlockingObjectEnum - QueryInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::Release(void)
{
	return 0;
}


