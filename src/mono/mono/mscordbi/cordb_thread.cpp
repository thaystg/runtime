#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_appdomain.hpp>
#include <cordb_register.hpp>
#include <cordb_blocking_obj.hpp>
#include <cordb_chain.hpp>
#include <cordb_process.hpp>

using namespace std;

CordbThread::CordbThread(CordbProcess* ppProcess, long thread_id)
{
	this->ppProcess = ppProcess;
	this->thread_id = thread_id;
	stepper = NULL;
	registerset = NULL;
}

HRESULT STDMETHODCALLTYPE CordbThread::HasUnhandledException(void)
{
	DEBUG_PRINTF(1, "CordbThread - HasUnhandledException - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetBlockingObjects(
	/* [out] */ ICorDebugBlockingObjectEnum** ppBlockingObjectEnum)
{
	DEBUG_PRINTF(1, "CordbThread - GetBlockingObjects - IMPLEMENTED\n");
	CordbBlockingObjectEnum* blockingObject = new CordbBlockingObjectEnum();
	*ppBlockingObjectEnum = static_cast<ICorDebugBlockingObjectEnum*>(blockingObject);
	
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentCustomDebuggerNotification(
	/* [out] */ ICorDebugValue** ppNotificationObject)
{
	DEBUG_PRINTF(1, "CordbThread - GetCurrentCustomDebuggerNotification - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStackWalk(
	/* [out] */ ICorDebugStackWalk** ppStackWalk)
{
	DEBUG_PRINTF(1, "CordbThread - CreateStackWalk - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveInternalFrames(
	/* [in] */ ULONG32 cInternalFrames,
	/* [out] */ ULONG32* pcInternalFrames,
	/* [length_is][size_is][out][in] */ ICorDebugInternalFrame2* ppInternalFrames[])
{
	DEBUG_PRINTF(1, "CordbThread - GetActiveInternalFrames - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFunctions(
	/* [in] */ ULONG32 cFunctions,
	/* [out] */ ULONG32* pcFunctions,
	/* [length_is][size_is][out][in] */ COR_ACTIVE_FUNCTION pFunctions[])
{
	DEBUG_PRINTF(1, "CordbThread - GetActiveFunctions - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetConnectionID(
	/* [out] */ CONNID* pdwConnectionId)
{
	DEBUG_PRINTF(1, "CordbThread - GetConnectionID - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetTaskID(
	/* [out] */ TASKID* pTaskId)
{
	DEBUG_PRINTF(1, "CordbThread - GetTaskID - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetVolatileOSThreadID(
	/* [out] */ DWORD* pdwTid)
{
	DEBUG_PRINTF(1, "CordbThread - GetVolatileOSThreadID - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::InterceptCurrentException(

	/* [in] */ ICorDebugFrame* pFrame)
{
	DEBUG_PRINTF(1, "CordbThread - InterceptCurrentException - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThread::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	DEBUG_PRINTF(1, "CordbThread - GetProcess - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetID(
	/* [out] */ DWORD* pdwThreadId)
{
	*pdwThreadId = thread_id;
	DEBUG_PRINTF(1, "CordbThread - GetID - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetHandle(
	/* [out] */ HTHREAD* phThreadHandle)
{
	DEBUG_PRINTF(1, "CordbThread - GetHandle - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetAppDomain(
	/* [out] */ ICorDebugAppDomain** ppAppDomain)
{
	*ppAppDomain = static_cast<ICorDebugAppDomain*>(this->ppProcess->connection->pCorDebugAppDomain);
	DEBUG_PRINTF(1, "CordbThread - GetAppDomain - IMPLEMENTED\n");
	return S_OK;

}

HRESULT STDMETHODCALLTYPE CordbThread::SetDebugState(
	/* [in] */ CorDebugThreadState state)
{
	DEBUG_PRINTF(1, "CordbThread - SetDebugState - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetDebugState(
	/* [out] */ CorDebugThreadState* pState)
{
	DEBUG_PRINTF(1, "CordbThread - GetDebugState - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetUserState(
	/* [out] */ CorDebugUserState* pState)
{
	DEBUG_PRINTF(1, "CordbThread - GetUserState - NOT IMPLEMENTED\n");

	*pState = USER_SUSPENDED;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentException(
	/* [out] */ ICorDebugValue** ppExceptionObject)
{
	DEBUG_PRINTF(1, "CordbThread - GetCurrentException - IMPLEMENTED\n");

	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbThread::ClearCurrentException(void)
{
	DEBUG_PRINTF(1, "CordbThread - ClearCurrentException - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStepper(
	/* [out] */ ICorDebugStepper** ppStepper)
{
	if (!this->stepper)
	{
		CordbStepper* stepper = new CordbStepper(this);
		this->stepper = stepper;
	}
	*ppStepper = static_cast<ICorDebugStepper*>(stepper);

	DEBUG_PRINTF(1, "CordbThread - CreateStepper - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::EnumerateChains(
	/* [out] */ ICorDebugChainEnum** ppChains)
{
	CordbChainEnum* pChains = new CordbChainEnum(this);
	*ppChains = static_cast<ICorDebugChainEnum*>(pChains);
	DEBUG_PRINTF(1, "CordbThread - EnumerateChains - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveChain(
	/* [out] */ ICorDebugChain** ppChain)
{
	DEBUG_PRINTF(1, "CordbThread - GetActiveChain - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFrame(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	DEBUG_PRINTF(1, "CordbThread - GetActiveFrame - IMPLEMENTED\n");Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread_id);
	buffer_add_int(&localbuf, 0);
	buffer_add_int(&localbuf, -1);

	int cmdId = this->ppProcess->connection->send_event(CMD_SET_THREAD, CMD_THREAD_GET_FRAME_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		this->ppProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->ppProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int nframes = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	for (int i = 0; i < nframes; i++)
	{
		int frameid = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int methoid = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int il_offset = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int flags = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		if (i == 0)
		{
			CordbNativeFrame* frame = new CordbNativeFrame(frameid, methoid, il_offset, flags, this);
			*ppFrame = static_cast<ICorDebugFrame*>(frame);
		}
	}
	guint32 ctx_len;
	guint8 *ctx = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &ctx_len);
	registerset = new CordbRegisteSet(ctx, ctx_len);

	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetRegisterSet(
	/* [out] */ ICorDebugRegisterSet** ppRegisters)
{
	DEBUG_PRINTF(1, "CordbThread - GetRegisterSet - IMPLEMENTED - %x\n", registerset);

	if (!registerset)
		registerset = new CordbRegisteSet(0, 0);

	

	*ppRegisters = static_cast<ICorDebugRegisterSet*>(registerset);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateEval(
	/* [out] */ ICorDebugEval** ppEval)
{
	DEBUG_PRINTF(1, "CordbThread - CreateEval - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetObject(
	/* [out] */ ICorDebugValue** ppObject)
{
	DEBUG_PRINTF(1, "CordbThread - GetObject - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThread::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
{
	if (id == IID_ICorDebugThread)
	{
		*ppInterface = static_cast<ICorDebugThread*>(this);
	}
	else if (id == IID_ICorDebugThread2)
	{
		*ppInterface = static_cast<ICorDebugThread2*>(this);
	}
	else if (id == IID_ICorDebugThread3)
	{
		*ppInterface = static_cast<ICorDebugThread3*>(this);
	}
	else if (id == IID_ICorDebugThread4)
	{
		*ppInterface = static_cast<ICorDebugThread4*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*ppInterface = static_cast<IUnknown*>(static_cast<ICorDebugThread*>(this));
	}
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}
	DEBUG_PRINTF(1, "CordbThread - QueryInterface - IMPLEMENTED\n");
	return S_OK;
}
ULONG STDMETHODCALLTYPE CordbThread::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbThread::Release(void)
{
	return 0;
}
