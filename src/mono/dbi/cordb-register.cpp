// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB-REGISTER.CPP
//

#include <fstream>
#include <iostream>

#include <cordb-assembly.hpp>
#include <cordb-frame.hpp>
#include <cordb-function.hpp>
#include <cordb-process.hpp>
#include <cordb-register.hpp>
#include <cordb-stepper.hpp>
#include <cordb-thread.hpp>
#include <cordb.hpp>

using namespace std;

HRESULT __stdcall CordbRegisteSet::GetRegistersAvailable(ULONG64 *pAvailable) {
  DEBUG_PRINTF(1,
               "CordbRegisteSet - GetRegistersAvailable - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

CordbRegisteSet::CordbRegisteSet(Connection *conn, guint8 *ctx, guint32 ctx_len)
    : CordbBaseMono(conn) {
  this->ctx = ctx;
  this->ctx_len = ctx_len;
}

HRESULT __stdcall CordbRegisteSet::QueryInterface(REFIID id,
                                                  void **pInterface) {
  DEBUG_PRINTF(1, "CordbRegisteSet - QueryInterface - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT __stdcall CordbRegisteSet::GetRegisters(ULONG64 mask, ULONG32 regCount,
                                                CORDB_REGISTER regBuffer[]) {
  DEBUG_PRINTF(1, "CordbRegisteSet - GetRegisters - NOT IMPLEMENTED");
  return E_NOTIMPL;
}

ULONG __stdcall CordbRegisteSet::Release(void) { return 0; }

ULONG __stdcall CordbRegisteSet::AddRef(void) { return 0; }

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetRegisters(
    /* [in] */ ULONG64 mask,
    /* [in] */ ULONG32 regCount,
    /* [size_is][in] */ CORDB_REGISTER regBuffer[]) {
  DEBUG_PRINTF(1, "CordbRegisteSet - SetRegisters - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::GetThreadContext(
    /* [in] */ ULONG32 contextSize,
    /* [size_is][length_is][out][in] */ BYTE context[]) {
  AMD64_CONTEXT ctx_amd64;
  memcpy(&ctx_amd64.Rax, ctx, ctx_len);
  memcpy(context, &ctx_amd64, contextSize);
  DEBUG_PRINTF(1, "CordbRegisteSet - GetThreadContext - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetThreadContext(
    /* [in] */ ULONG32 contextSize,
    /* [size_is][length_is][in] */ BYTE context[]) {
  DEBUG_PRINTF(1, "CordbRegisteSet - SetThreadContext - NOT IMPLEMENTED\n");
  return E_NOTIMPL;
}
