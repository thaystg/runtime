#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_breakpoint.hpp>
#include <cordb_function.hpp>
#include <cordb_code.hpp>
#include <cordb_process.hpp>

using namespace std;

CordbFunctionBreakpoint::CordbFunctionBreakpoint(CordbCode* code, ULONG32 offset)
{
	this->code = code;
	this->offset = offset;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetFunction(ICorDebugFunction** ppFunction)
{
	*ppFunction = static_cast<ICorDebugFunction*>(code->func) ;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunctionBreakpoint - GetFunction - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetOffset(ULONG32* pnOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunctionBreakpoint - GetOffset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunctionBreakpoint::Activate(BOOL bActive)
{
	if (bActive)
	{
		Buffer sendbuf;
		int buflen = 128;
		buffer_init(&sendbuf, buflen);
		buffer_add_byte(&sendbuf, EVENT_KIND_BREAKPOINT);
		buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
		buffer_add_byte(&sendbuf, 1); //modifiers
		buffer_add_byte(&sendbuf, MOD_KIND_LOCATION_ONLY);
		buffer_add_id(&sendbuf, this->code->func->id);
		buffer_add_long(&sendbuf, offset);
		code->func->module->pProcess->connection->send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
		buffer_free(&sendbuf);

		fstream file;
		file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
		file << "CordbFunctionBreakpoint - Activate" << std::endl;
		file.close();
	}
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::IsActive(BOOL* pbActive)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunctionBreakpoint - IsActive - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunctionBreakpoint::QueryInterface(REFIID id, void** pInterface)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFunctionBreakpoint - QueryInterface - IMPLEMENTED" << std::endl;
	file.close();
	if (id == IID_ICorDebugFunctionBreakpoint)
	{
		*pInterface = static_cast<ICorDebugFunctionBreakpoint*>(this);
	}
	else
	{
		// Not looking for a function breakpoint? See if the base class handles
		// this interface. (issue 143976)
		fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "return CordbBreakpoint::QueryInterface(id, pInterface);" << std::endl;
		file.close();
		//return CordbBreakpoint::QueryInterface(id, pInterface);
	}
	return S_OK;
}

ULONG __stdcall CordbFunctionBreakpoint::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbFunctionBreakpoint::Release(void)
{
	return 0;
}
