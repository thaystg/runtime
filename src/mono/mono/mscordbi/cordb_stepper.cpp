#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_process.hpp>

using namespace std;

CordbStepper::CordbStepper(CordbThread* thread)
{
	this->thread = thread;
	hasStepped = false;
}

HRESULT STDMETHODCALLTYPE CordbStepper::IsActive(BOOL* pbActive)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - IsActive - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Deactivate(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - Deactivate - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetInterceptMask(CorDebugIntercept mask)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - SetInterceptMask - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetUnmappedStopMask(CorDebugUnmappedStop mask)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - SetUnmappedStopMask - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Step(BOOL bStepIn)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - Step - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepRange(BOOL bStepIn, COR_DEBUG_STEP_RANGE ranges[], ULONG32 cRangeCount)
{
	hasStepped = true;
	//thread->ppProcess->suspended = false;
	/*
	step_thread_id = decode_id (p, &p, end);
				size = (StepSize)decode_int (p, &p, end);
				depth = (StepDepth)decode_int (p, &p, end);
				if (CHECK_PROTOCOL_VERSION (2, 16))
					filter = (StepFilter)decode_int (p, &p, end);
				req->modifiers [i].data.filter = filter;
				if (!CHECK_PROTOCOL_VERSION (2, 26) && (req->modifiers [i].data.filter & STEP_FILTER_DEBUGGER_HIDDEN))
					/* Treat STEP_THOUGH the same as HIDDEN
	req->modifiers[i].data.filter = (StepFilter)(req->modifiers[i].data.filter | STEP_FILTER_DEBUGGER_STEP_THROUGH); */

	Buffer sendbuf;
	int buflen = 128;
	buffer_init(&sendbuf, buflen);
	buffer_add_byte(&sendbuf, EVENT_KIND_STEP);
	buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
	buffer_add_byte(&sendbuf, 1); //modifiers
	buffer_add_byte(&sendbuf, MOD_KIND_STEP);

	buffer_add_id(&sendbuf, thread->thread_id);
	buffer_add_int(&sendbuf, STEP_SIZE_MIN);
	buffer_add_int(&sendbuf, bStepIn ? STEP_DEPTH_INTO : STEP_DEPTH_OVER);
	buffer_add_int(&sendbuf, STEP_FILTER_NONE);

	this->thread->ppProcess->connection->send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
	buffer_free(&sendbuf);

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - StepRange - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepOut(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - StepOut - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetRangeIL(BOOL bIL)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - SetRangeIL - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::QueryInterface(REFIID riid, void** ppvObject)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - QueryInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::AddRef(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - AddRef - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::Release(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - Release - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetJMC(BOOL fIsJMCStepper)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbStepper - SetJMC - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}
