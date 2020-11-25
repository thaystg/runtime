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
CordbType::CordbType(CorElementType type)
{
	this->type = type;
}

HRESULT STDMETHODCALLTYPE CordbType::GetType(CorElementType* ty)
{
	*ty = type;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetType - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetClass(ICorDebugClass** ppClass)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetClass - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbType::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
	CordbTypeEnum* tp = new CordbTypeEnum();
	*ppTyParEnum = static_cast<ICorDebugTypeEnum*>(tp);

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - EnumerateTypeParameters - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetFirstTypeParameter(ICorDebugType** value)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetFirstTypeParameter - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetBase(ICorDebugType** pBase)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetBase - IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetStaticFieldValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetRank(ULONG32* pnRank)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetRank - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugType)
		*pInterface = static_cast<ICorDebugType*>(this);
	else if (id == IID_ICorDebugType2)
		*pInterface = static_cast<ICorDebugType2*>(this);
	else if (id == IID_IUnknown)
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugType*>(this));
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbType::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbType::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbType::GetTypeID(COR_TYPEID* id)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbType - GetTypeID - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Next(ULONG celt, ICorDebugType* values[], ULONG* pceltFetched)
{
	*pceltFetched = celt;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbTypeEnum - Next - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Skip(ULONG celt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbTypeEnum - Skip - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Reset(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbTypeEnum - Reset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Clone(ICorDebugEnum** ppEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbTypeEnum - Clone - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::GetCount(ULONG* pcelt)
{
	*pcelt = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugEnum)
		*pInterface = static_cast<ICorDebugEnum*>(this);
	else if (id == IID_ICorDebugTypeEnum)
		*pInterface = static_cast<ICorDebugTypeEnum*>(this);
	else if (id == IID_IUnknown)
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugTypeEnum*>(this));
	else
	{
		fstream file;
		file.open("c:\\thays\\example.txt", ios::out | ios::in | ios::app);
		file << "CordbTypeEnum - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbTypeEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbTypeEnum::Release(void)
{
	return 0;
}
