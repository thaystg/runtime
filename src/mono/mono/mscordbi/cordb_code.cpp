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
#include <cordb_chain.hpp>
#include <cordb_code.hpp>
#include <cordb_process.hpp>

using namespace std;


CordbCode::CordbCode(CordbFunction* func)
{
	this->func = func;
}

HRESULT __stdcall CordbCode::IsIL(BOOL* pbIL)
{
	DEBUG_PRINTF(1, "CordbCode - IsIL - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetFunction(ICorDebugFunction** ppFunction)
{
	DEBUG_PRINTF(1, "CordbCode - GetFunction - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetAddress(CORDB_ADDRESS* pStart)
{
	DEBUG_PRINTF(1, "CordbCode - GetAddress - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetSize(ULONG32* pcBytes)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, this->func->id);
	int cmdId = func->module->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_BODY, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = func->module->pProcess->connection->get_answer(cmdId);
	int code_size = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*pcBytes = code_size;
	DEBUG_PRINTF(1, "CordbCode - GetSize - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbCode::CreateBreakpoint(ULONG32 offset, ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	//add it in a list to not recreate a already created breakpoint
	CordbFunctionBreakpoint* bp = new CordbFunctionBreakpoint(this, offset);
	*ppBreakpoint = static_cast<ICorDebugFunctionBreakpoint*>(bp);
	g_ptr_array_add(this->func->module->pProcess->cordb->breakpoints, bp);
	DEBUG_PRINTF(1, "CordbCode - CreateBreakpoint - %ld - IMPLEMENTED\n");

	DEBUG_PRINTF(1, "CreateBreakpoint - static_cast<ICorDebugFunctionBreakpoint*>(breakpoint)\n");
	return S_OK;
}

HRESULT __stdcall CordbCode::GetCode(ULONG32 startOffset, ULONG32 endOffset, ULONG32 cBufferAlloc, BYTE buffer[], ULONG32* pcBufferSize)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, this->func->id);
	int cmdId = func->module->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_BODY, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = func->module->pProcess->connection->get_answer(cmdId);
	guint8*  code = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, pcBufferSize);
	
	memcpy(buffer, code, *pcBufferSize);
	DEBUG_PRINTF(1, "CordbCode - GetCode - IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbCode::GetVersionNumber(ULONG32* nVersion)
{
	*nVersion = 1;
	DEBUG_PRINTF(1, "CordbCode - GetVersionNumber - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT __stdcall CordbCode::GetILToNativeMapping(ULONG32 cMap, ULONG32* pcMap, COR_DEBUG_IL_TO_NATIVE_MAP map[])
{
	DEBUG_PRINTF(1, "CordbCode - GetILToNativeMapping - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetEnCRemapSequencePoints(ULONG32 cMap, ULONG32* pcMap, ULONG32 offsets[])
{
	DEBUG_PRINTF(1, "CordbCode - GetEnCRemapSequencePoints - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugCode)
	{
		*pInterface = static_cast<ICorDebugCode*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugCode*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG __stdcall CordbCode::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbCode::Release(void)
{
	return 0;
}


