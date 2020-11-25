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

using namespace std;


HRESULT __stdcall CordbChainEnum::Next(ULONG celt, ICorDebugChain* chains[], ULONG* pceltFetched)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - Next - IMPLEMENTED" << std::endl;
	file.close();
	chains[0] = new CordbChain(thread, CHAIN_PROCESS_START, false);
	chains[1] = new CordbChain(thread, CHAIN_ENTER_MANAGED, true);
	*pceltFetched = celt;
	return S_OK;
}

CordbChainEnum::CordbChainEnum(CordbThread* thread)
{
	this->thread = thread;
}

HRESULT __stdcall CordbChainEnum::QueryInterface(REFIID id, void** pInterface)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - QueryInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT __stdcall CordbChainEnum::Skip(ULONG celt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - Skip - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbChainEnum::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbChainEnum::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::Reset(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - Reset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::Clone(
	/* [out] */ ICorDebugEnum** ppEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - Clone - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::GetCount(
	/* [out] */ ULONG* pcelt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChainEnum - GetCount - IMPLEMENTED" << std::endl;
	file.close();
	*pcelt = 2;
	return S_OK;
}

CordbChain::CordbChain(CordbThread* thread, CorDebugChainReason chain_reason, bool is_managed)
{
	this->thread = thread;
	this->chain_reason = chain_reason;
	this->is_managed = is_managed;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetThread(/* [out] */ ICorDebugThread** ppThread)
{
	*ppThread = static_cast<ICorDebugThread*>(thread);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetThread - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetStackRange(/* [out] */ CORDB_ADDRESS* pStart, /* [out] */ CORDB_ADDRESS* pEnd)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetStackRange - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetContext(/* [out] */ ICorDebugContext** ppContext)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetContext - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetCaller(/* [out] */ ICorDebugChain** ppChain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetCaller - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetCallee(/* [out] */ ICorDebugChain** ppChain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetCallee - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetPrevious(/* [out] */ ICorDebugChain** ppChain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetPrevious - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetNext(/* [out] */ ICorDebugChain** ppChain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetNext - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::IsManaged(/* [out] */ BOOL* pManaged)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - IsManaged - IMPLEMENTED" << std::endl;
	*pManaged = is_managed;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::EnumerateFrames(/* [out] */ ICorDebugFrameEnum** ppFrames)
{
	*ppFrames = new CordbFrameEnum(thread);
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - EnumerateFrames - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetActiveFrame(/* [out] */ ICorDebugFrame** ppFrame)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetActiveFrame - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetRegisterSet(/* [out] */ ICorDebugRegisterSet** ppRegisters)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetRegisterSet - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetReason(/* [out] */ CorDebugChainReason* pReason)
{
	*pReason = chain_reason;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - GetReason - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::QueryInterface(/* [in] */ REFIID id, /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbChain - QueryInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbChain::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbChain::Release(void)
{
	return 0;
}
