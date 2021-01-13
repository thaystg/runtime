#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-breakpoint.hpp>
#include <cordb-function.hpp>
#include <cordb-class.hpp>
#include <cordb-value.hpp>
#include <cordb-type.hpp>
#include <cordb-blocking-obj.hpp>

using namespace std;


HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Next (ULONG celt, CorDebugBlockingObject values[],
                                                         ULONG *pceltFetched) {
	DEBUG_PRINTF (1, "CordbBlockingObjectEnum - Next - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Skip (ULONG celt) {
	DEBUG_PRINTF (1, "CordbBlockingObjectEnum - Skip - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Reset (void) {
	DEBUG_PRINTF (1, "CordbBlockingObjectEnum - Reset - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Clone (ICorDebugEnum **ppEnum) {
	DEBUG_PRINTF (1, "CordbBlockingObjectEnum - Clone - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::GetCount (ULONG *pcelt) {
	pcelt = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::QueryInterface (REFIID riid, void **ppvObject) {
	DEBUG_PRINTF (1, "CordbBlockingObjectEnum - QueryInterface - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::AddRef (void) {
	return 0;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::Release (void) {
	return 0;
}
