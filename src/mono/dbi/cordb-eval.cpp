// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-EVAL.CPP
//

#include <cordb-eval.h>
#include <cordb-function.h>
#include <cordb-process.h>
#include <cordb-thread.h>
#include <cordb-value.h>
#include <cordb.h>

CordbEval::CordbEval(Connection *conn, CordbThread *thread)
    : CordbBaseMono(conn) {
  this->thread = thread;
  ppValue = NULL;
  cmdId = -1;
}

HRESULT STDMETHODCALLTYPE CordbEval::CallParameterizedFunction(
    ICorDebugFunction *pFunction, ULONG32 nTypeArgs,
    ICorDebugType *ppTypeArgs[], ULONG32 nArgs, ICorDebugValue *ppArgs[]) {
  DEBUG_PRINTF(1, "CordbEval - CallParameterizedFunction - IMPLEMENTED\n");

  Buffer localbuf;
  buffer_init(&localbuf, 128);
  buffer_add_id(&localbuf, thread->thread_id);
  buffer_add_int(&localbuf, 1);
  buffer_add_int(&localbuf, ((CordbFunction *)pFunction)->id);
  buffer_add_int(&localbuf, nArgs);
  cmdId = conn->send_event(CMD_SET_VM, CMD_VM_INVOKE_METHOD, &localbuf);
  buffer_free(&localbuf);
  g_ptr_array_add(conn->pending_eval, this);
  return S_OK;
}

void CordbEval::EvalComplete(Buffer *localbuf2) {

  decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
  CordbObjectValue::CreateCordbValue(conn, localbuf2, &ppValue);
  conn->ppCordb->pCallback->EvalComplete(
      static_cast<ICorDebugAppDomain *>(
          g_ptr_array_index(thread->ppProcess->appdomains, 0)),
      static_cast<ICorDebugThread *>(thread),
      static_cast<ICorDebugEval *>(this));
}

HRESULT STDMETHODCALLTYPE
CordbEval::CreateValueForType(ICorDebugType *pType, ICorDebugValue **ppValue) {
  DEBUG_PRINTF(1, "CordbEval - CreateValueForType - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewParameterizedObject(
    ICorDebugFunction *pConstructor, ULONG32 nTypeArgs,
    ICorDebugType *ppTypeArgs[], ULONG32 nArgs, ICorDebugValue *ppArgs[]) {
  DEBUG_PRINTF(1, "CordbEval - NewParameterizedObject - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewParameterizedObjectNoConstructor(
    ICorDebugClass *pClass, ULONG32 nTypeArgs, ICorDebugType *ppTypeArgs[]) {
  DEBUG_PRINTF(
      1, "CordbEval - NewParameterizedObjectNoConstructor - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE
CordbEval::NewParameterizedArray(ICorDebugType *pElementType, ULONG32 rank,
                                 ULONG32 dims[], ULONG32 lowBounds[]) {
  DEBUG_PRINTF(1, "CordbEval - NewParameterizedArray - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewStringWithLength(LPCWSTR string,
                                                         UINT uiLength) {
  DEBUG_PRINTF(1, "CordbEval - NewStringWithLength - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::RudeAbort(void) {
  DEBUG_PRINTF(1, "CordbEval - RudeAbort - NOT IMPLEMENTED\n");
  return S_OK;
}

ULONG CordbEval::AddRef(void) { return 1; }

ULONG CordbEval::Release(void) { return 1; }

HRESULT
CordbEval::QueryInterface(REFIID id,
                          _COM_Outptr_ void __RPC_FAR *__RPC_FAR *pInterface) {
  if (id == IID_ICorDebugEval) {
    *pInterface = static_cast<ICorDebugEval *>(this);
  } else if (id == IID_ICorDebugEval2) {
    *pInterface = static_cast<ICorDebugEval2 *>(this);
  } else if (id == IID_IUnknown) {
    *pInterface = static_cast<IUnknown *>(static_cast<ICorDebugEval *>(this));
  } else {
    *pInterface = NULL;
    return E_NOINTERFACE;
  }

  return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::CallFunction(ICorDebugFunction *pFunction,
                                                  ULONG32 nArgs,
                                                  ICorDebugValue *ppArgs[]) {
  DEBUG_PRINTF(1, "CordbEval - CallFunction - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewObject(ICorDebugFunction *pConstructor,
                                               ULONG32 nArgs,
                                               ICorDebugValue *ppArgs[]) {
  DEBUG_PRINTF(1, "CordbEval - NewObject - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE
CordbEval::NewObjectNoConstructor(ICorDebugClass *pClass) {
  DEBUG_PRINTF(1, "CordbEval - NewObjectNoConstructor - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewString(LPCWSTR string) {
  DEBUG_PRINTF(1, "CordbEval - NewString - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::NewArray(CorElementType elementType,
                                              ICorDebugClass *pElementClass,
                                              ULONG32 rank, ULONG32 dims[],
                                              ULONG32 lowBounds[]) {
  DEBUG_PRINTF(1, "CordbEval - NewArray - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::IsActive(BOOL *pbActive) {
  DEBUG_PRINTF(1, "CordbEval - IsActive - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::Abort(void) {
  DEBUG_PRINTF(1, "CordbEval - Abort - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::GetResult(ICorDebugValue **ppResult) {
  *ppResult = ppValue;
  DEBUG_PRINTF(1, "CordbEval - GetResult - IMPLEMENTED\n");
  return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbEval::GetThread(ICorDebugThread **ppThread) {
  DEBUG_PRINTF(1, "CordbEval - GetThread - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbEval::CreateValue(CorElementType elementType,
                                                 ICorDebugClass *pElementClass,
                                                 ICorDebugValue **ppValue) {
  CordbContent content_value;
  content_value.booleanValue = 0;
  CordbValue *value =
      new CordbValue(conn, elementType, content_value,
                     convert_mono_type_2_icordbg_size(elementType));
  DEBUG_PRINTF(1, "CordbEval - CreateValue - IMPLEMENTED\n");
  value->QueryInterface(IID_ICorDebugValue, (void **)ppValue);
  return S_OK;
}
