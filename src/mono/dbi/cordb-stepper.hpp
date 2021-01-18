#ifndef __MONO_DEBUGGER_CORDB_STEPPER_H__
#define __MONO_DEBUGGER_CORDB_STEPPER_H__

#include <cordb.hpp>


class CordbStepper :
	public ICorDebugStepper,
	public ICorDebugStepper2
{
	CordbThread *thread;
	boolean hasStepped;
public:
	boolean isComplete;
	CordbStepper (CordbThread *thread);
	HRESULT STDMETHODCALLTYPE IsActive (BOOL *pbActive);
	HRESULT STDMETHODCALLTYPE Deactivate (void);
	HRESULT STDMETHODCALLTYPE SetInterceptMask (CorDebugIntercept mask);
	HRESULT STDMETHODCALLTYPE SetUnmappedStopMask (CorDebugUnmappedStop mask);
	HRESULT STDMETHODCALLTYPE Step (BOOL bStepIn);
	HRESULT STDMETHODCALLTYPE StepRange (BOOL bStepIn, COR_DEBUG_STEP_RANGE ranges[], ULONG32 cRangeCount);
	HRESULT STDMETHODCALLTYPE StepOut (void);
	HRESULT STDMETHODCALLTYPE SetRangeIL (BOOL bIL);
	HRESULT STDMETHODCALLTYPE QueryInterface (REFIID riid, void **ppvObject);
	ULONG STDMETHODCALLTYPE AddRef (void);
	ULONG STDMETHODCALLTYPE Release (void);
	HRESULT STDMETHODCALLTYPE SetJMC (BOOL fIsJMCStepper);
};


#endif
