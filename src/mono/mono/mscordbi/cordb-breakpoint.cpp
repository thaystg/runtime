#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-breakpoint.hpp>
#include <cordb-function.hpp>
#include <cordb-code.hpp>
#include <cordb-process.hpp>

using namespace std;

CordbFunctionBreakpoint::CordbFunctionBreakpoint(CordbCode* code, ULONG32 offset)
{
	this->code = code;
	this->offset = offset;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetFunction(ICorDebugFunction** ppFunction)
{
	*ppFunction = static_cast<ICorDebugFunction*>(code->func) ;
	DEBUG_PRINTF(1, "CordbFunctionBreakpoint - GetFunction - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetOffset(ULONG32* pnOffset)
{
	DEBUG_PRINTF(1, "CordbFunctionBreakpoint - GetOffset - NOT IMPLEMENTED\n");
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
		DEBUG_PRINTF(1, "CordbFunctionBreakpoint - Activate\n");
	}
	else
	{
		DEBUG_PRINTF(1, "CordbFunctionBreakpoint - Activate - FALSE - NOT IMPLEMENTED\n");
	}
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::IsActive(BOOL* pbActive)
{
	DEBUG_PRINTF(1, "CordbFunctionBreakpoint - IsActive - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunctionBreakpoint::QueryInterface(REFIID id, void** pInterface)
{
	DEBUG_PRINTF(1, "CordbFunctionBreakpoint - QueryInterface - IMPLEMENTED\n");

	if (id == IID_ICorDebugFunctionBreakpoint)
	{
		*pInterface = static_cast<ICorDebugFunctionBreakpoint*>(this);
	}
	else
	{
		// Not looking for a function breakpoint? See if the base class handles
		// this interface. (issue 143976)
		DEBUG_PRINTF(1, "return CordbBreakpoint::QueryInterface(id, pInterface);\n");
	
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
