#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_function.hpp>
#include <cordb_code.hpp>

using namespace std;

CordbFunction::CordbFunction(mdToken token, int id, CordbModule* module)
{
	this->token = token;
	this->id = id;
	code = NULL;
	this->module = module;
}

HRESULT __stdcall CordbFunction::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugFunction)
	{
		*pInterface = static_cast<ICorDebugFunction*>(this);
	}
	else if (id == IID_ICorDebugFunction2)
	{
		*pInterface = static_cast<ICorDebugFunction2*>(this);
	}
	else if (id == IID_ICorDebugFunction3)
	{
		*pInterface = static_cast<ICorDebugFunction3*>(this);
	}
	else if (id == IID_ICorDebugFunction4)
	{
		*pInterface = static_cast<ICorDebugFunction4*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugFunction*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG __stdcall CordbFunction::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbFunction::Release(void)
{
	return 0;
}

HRESULT __stdcall CordbFunction::GetModule(ICorDebugModule** ppModule)
{
	*ppModule = static_cast<ICorDebugModule*>(this->module);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetModule - IMPLEMENTED" << std::endl;
	file.close();
	if (!*ppModule)
		return S_FALSE;
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetClass(ICorDebugClass** ppClass)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetClass - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetToken(mdMethodDef* pMethodDef)
{
	*pMethodDef = this->token;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetToken - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetILCode(ICorDebugCode** ppCode)
{
	if (code == NULL)
		code = new CordbCode(this); 
	*ppCode = static_cast<ICorDebugCode*>(code);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetILCode - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetNativeCode(ICorDebugCode** ppCode)
{
	*ppCode = static_cast<ICorDebugCode*>(code);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetNativeCode - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunction::CreateBreakpoint(ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - CreateBreakpoint - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetLocalVarSigToken(mdSignature* pmdSig)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetLocalVarSigToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetCurrentVersionNumber(ULONG32* pnCurrentVersion)
{
	*pnCurrentVersion = 1;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetCurrentVersionNumber - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunction::SetJMCStatus(BOOL bIsJustMyCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - SetJMCStatus - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetJMCStatus(BOOL* pbIsJustMyCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetJMCStatus - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::EnumerateNativeCode(ICorDebugCodeEnum** ppCodeEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - EnumerateNativeCode - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetVersionNumber(ULONG32* pnVersion)
{
	*pnVersion = 1;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetVersionNumber - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetActiveReJitRequestILCode(ICorDebugILCode** ppReJitedILCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - GetActiveReJitRequestILCode - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::CreateNativeBreakpoint(ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunction - CreateNativeBreakpoint - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}
