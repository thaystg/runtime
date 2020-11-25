#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_function.hpp>
#include <cordb_process.hpp>
#include <cordb_assembly.hpp>
#include <cordb_register.hpp>

using namespace std;

HRESULT __stdcall CordbRegisteSet::GetRegistersAvailable(ULONG64* pAvailable)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - GetRegistersAvailable - NOT IMPLEMENTED" << endl;
	file.close();

	return E_NOTIMPL;
}

CordbRegisteSet::CordbRegisteSet(guint8* ctx, guint32 ctx_len)
{
	this->ctx = ctx;
	this->ctx_len = ctx_len;
}

HRESULT __stdcall CordbRegisteSet::QueryInterface(REFIID id, void** pInterface)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - QueryInterface - NOT IMPLEMENTED" << endl;
	file.close();

	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbRegisteSet::GetRegisters(ULONG64 mask, ULONG32 regCount, CORDB_REGISTER regBuffer[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - GetRegisters - NOT IMPLEMENTED" << endl;
	file.close();
	return E_NOTIMPL;
}

ULONG __stdcall CordbRegisteSet::Release(void)
{
	return 0;
}

ULONG __stdcall CordbRegisteSet::AddRef(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetRegisters(
	/* [in] */ ULONG64 mask,
	/* [in] */ ULONG32 regCount,
	/* [size_is][in] */ CORDB_REGISTER regBuffer[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - SetRegisters - NOT IMPLEMENTED" << endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::GetThreadContext(
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][out][in] */ BYTE context[])
{
	AMD64_CONTEXT ctx_amd64;
	memcpy(&ctx_amd64.Rax, ctx, ctx_len);
	memcpy(context, &ctx_amd64, contextSize);
	
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - GetThreadContext - NOT IMPLEMENTED" << endl;
	file.close();

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetThreadContext(
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][in] */ BYTE context[])
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbRegisteSet - SetThreadContext - NOT IMPLEMENTED" << endl;
	file.close();

	return E_NOTIMPL;
}