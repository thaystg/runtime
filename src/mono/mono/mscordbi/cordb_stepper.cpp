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
	DEBUG_PRINTF(1, "CordbStepper - IsActive - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Deactivate(void)
{
	DEBUG_PRINTF(1, "CordbStepper - Deactivate - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetInterceptMask(CorDebugIntercept mask)
{
	DEBUG_PRINTF(1, "CordbStepper - SetInterceptMask - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetUnmappedStopMask(CorDebugUnmappedStop mask)
{
	DEBUG_PRINTF(1, "CordbStepper - SetUnmappedStopMask - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Step(BOOL bStepIn)
{
	DEBUG_PRINTF(1, "CordbStepper - Step - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepRange(BOOL bStepIn, COR_DEBUG_STEP_RANGE ranges[], ULONG32 cRangeCount)
{
	hasStepped = true;
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

	DEBUG_PRINTF(1, "CordbStepper - StepRange - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepOut(void)
{
	DEBUG_PRINTF(1, "CordbStepper - StepOut - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetRangeIL(BOOL bIL)
{
	DEBUG_PRINTF(1, "CordbStepper - SetRangeIL - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::QueryInterface(REFIID riid, void** ppvObject)
{
	DEBUG_PRINTF(1, "CordbStepper - QueryInterface - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::AddRef(void)
{
	DEBUG_PRINTF(1, "CordbStepper - AddRef - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::Release(void)
{
	DEBUG_PRINTF(1, "CordbStepper - Release - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetJMC(BOOL fIsJMCStepper)
{
	DEBUG_PRINTF(1, "CordbStepper - SetJMC - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
