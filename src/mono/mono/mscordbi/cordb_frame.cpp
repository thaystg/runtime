#include <iostream>
#include <fstream>

#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_function.hpp>
#include <cordb_process.hpp>
#include <cordb_register.hpp>
#include <cordb_code.hpp>
#include <cordb_value.hpp>

using namespace std;

CordbFrameEnum::CordbFrameEnum(CordbThread* thread)
{
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Next(ULONG celt, ICorDebugFrame* frames[], ULONG* pceltFetched)
{
	for (int i = 0; i < nframes; i++)
	{
		frames[i] = this->frames[i];
	}
	DEBUG_PRINTF(1, "CordbFrameEnum - Next - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Skip(ULONG celt)
{
	DEBUG_PRINTF(1, "CordbFrameEnum - Skip - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Reset(void)
{
	DEBUG_PRINTF(1, "CordbFrameEnum - Reset - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Clone(ICorDebugEnum** ppEnum)
{
	DEBUG_PRINTF(1, "CordbFrameEnum - Clone - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::GetCount(ULONG* pcelt)
{
	DEBUG_PRINTF(1, "CordbFrameEnum - GetCount - IMPLEMENTED\n");
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_int(&localbuf, 0);
	buffer_add_int(&localbuf, -1);

	
	int cmdId = thread->ppProcess->connection->send_event(CMD_SET_THREAD, CMD_THREAD_GET_FRAME_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = thread->ppProcess->connection->get_answer(cmdId);
	nframes = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	frames = (CordbNativeFrame**) malloc(sizeof(CordbNativeFrame*) * nframes);
	
	for (int i = 0; i < nframes; i++)
	{
		int frameid = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int methoid = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int il_offset = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int flags = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		CordbNativeFrame* frame = new CordbNativeFrame(frameid, methoid, il_offset, flags, thread);
		frames[i] = frame;
	}

	guint32 ctx_len;
	guint8 *ctx = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &ctx_len);
	
	if (!thread->registerset)
		thread->registerset = new CordbRegisteSet(ctx, ctx_len);

	*pcelt = nframes;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::QueryInterface(REFIID riid, void** ppvObject)
{
	DEBUG_PRINTF(1, "CordbFrameEnum - QueryInterface - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbFrameEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbFrameEnum::Release(void)
{
	return 0;
}


CordbJITILFrame::CordbJITILFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread)
{
	this->frameid = frameid;
	this->methoid = methoid;
	this->il_offset = il_offset;
	this->flags = flags;
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetChain(
	/* [out] */ ICorDebugChain** ppChain)
{
	DEBUG_PRINTF(1, "CordbFrame - GetChain - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCode(
	/* [out] */ ICorDebugCode** ppCode)
{
	DEBUG_PRINTF(1, "CordbFrame - GetCode - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetFunction(
	/* [out] */ ICorDebugFunction** ppFunction)
{
	CordbFunction *func = thread->ppProcess->cordb->findFunction(methoid);
	if (!func)
	{
		func = new CordbFunction(0, methoid, NULL);
		g_ptr_array_add(thread->ppProcess->cordb->functions, func);
	}

	*ppFunction = static_cast<ICorDebugFunction*>(func);
	DEBUG_PRINTF(1, "CordbFrame - GetFunction\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetFunctionToken(
	/* [out] */ mdMethodDef* pToken)
{
	DEBUG_PRINTF(1, "CordbFrame - GetFunctionToken - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackRange(
	/* [out] */ CORDB_ADDRESS* pStart,
	/* [out] */ CORDB_ADDRESS* pEnd)
{
	*pStart = 4096;
	*pEnd = 8192;
	DEBUG_PRINTF(1, "CordbFrame - GetStackRange - NOT IMPLEMENTED - we need id?\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCaller(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	DEBUG_PRINTF(1, "CordbFrame - GetCaller - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCallee(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	DEBUG_PRINTF(1, "CordbFrame - GetCallee - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CreateStepper(
	/* [out] */ ICorDebugStepper** ppStepper)
{
	DEBUG_PRINTF(1, "CordbFrame - CreateStepper - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbJITILFrame::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
	if (id == IID_ICorDebugILFrame)
	{
		*pInterface = static_cast<ICorDebugILFrame*>(this);
	}
	else if (id == IID_ICorDebugILFrame2)
	{
		*pInterface = static_cast<ICorDebugILFrame2*>(this);
	}
	else if (id == IID_ICorDebugILFrame3)
	{
		*pInterface = static_cast<ICorDebugILFrame3*>(this);
	}
	else if (id == IID_ICorDebugILFrame4)
	{
		*pInterface = static_cast<ICorDebugILFrame4*>(this);
	}
	else
	{
		*pInterface = NULL;
		
		
		DEBUG_PRINTF(1, "CordbFrame - QueryInterface - E_NOTIMPL\n");
	
		return E_NOINTERFACE;
	}
	DEBUG_PRINTF(1, "CordbFrame - QueryInterface - IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::RemapFunction(ULONG32 newILOffset)
{
	DEBUG_PRINTF(1, "CordbFrame - RemapFunction - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
	DEBUG_PRINTF(1, "CordbFrame - EnumerateTypeParameters - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetReturnValueForILOffset(ULONG32 ILoffset, ICorDebugValue** ppReturnValue)
{
	DEBUG_PRINTF(1, "CordbFrame - GetReturnValueForILOffset - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariablesEx(ILCodeKind flags, ICorDebugValueEnum** ppValueEnum)
{
	DEBUG_PRINTF(1, "CordbFrame - EnumerateLocalVariablesEx - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetLocalVariableEx(ILCodeKind flags, DWORD dwIndex, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbFrame - GetLocalVariableEx - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCodeEx(ILCodeKind flags, ICorDebugCode** ppCode)
{
	if (flags == ILCODE_REJIT_IL) {
		*ppCode = NULL;
	}
	else {
		CordbCode* code = new CordbCode(NULL);
		*ppCode = static_cast<ICorDebugCode*>(code);
	}
	DEBUG_PRINTF(1, "CordbJITILFrame - GetCodeEx - IMPLEMENTED\n");
	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbJITILFrame::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbJITILFrame::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetIP(
	/* [out] */ ULONG32* pnOffset,
	/* [out] */ CorDebugMappingResult* pMappingResult)
{
	*pnOffset = il_offset;
	*pMappingResult = MAPPING_EXACT;
	DEBUG_PRINTF(1, "CordbFrame - GetIP - IMPLEMENTED\n");
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbJITILFrame::SetIP(
	/* [in] */ ULONG32 nOffset)
{
	DEBUG_PRINTF(1, "CordbFrame - SetIP - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariables(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	DEBUG_PRINTF(1, "CordbFrame - EnumerateLocalVariables - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetLocalVariable(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_id(&localbuf, frameid);
	buffer_add_int(&localbuf, 1);
	buffer_add_int(&localbuf, dwIndex);


	int cmdId = thread->ppProcess->connection->send_event(CMD_SET_STACK_FRAME, CMD_STACK_FRAME_GET_VALUES, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = thread->ppProcess->connection->get_answer(cmdId);
	return CordbObjectValue::CreateCordbValue(thread->ppProcess->connection, localbuf2, ppValue);
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateArguments(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	DEBUG_PRINTF(1, "CordbFrame - EnumerateArguments - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetArgument(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_id(&localbuf, frameid);
	

    buffer_add_int(&localbuf, dwIndex);
	int cmdId = thread->ppProcess->connection->send_event(CMD_SET_STACK_FRAME, CMD_STACK_FRAME_GET_ARGUMENT, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = thread->ppProcess->connection->get_answer(cmdId);
	DEBUG_PRINTF(1, "CordbFrame - GetArgument - IMPLEMENTED - dwIndex - %d\n", dwIndex);
	return CordbObjectValue::CreateCordbValue(thread->ppProcess->connection, localbuf2, ppValue);
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackDepth(
	/* [out] */ ULONG32* pDepth)
{
	DEBUG_PRINTF(1, "CordbFrame - GetStackDepth - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackValue(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbFrame - GetStackValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CanSetIP(
	/* [in] */ ULONG32 nOffset)
{
	DEBUG_PRINTF(1, "CordbFrame - CanSetIP - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}


CordbNativeFrame::CordbNativeFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread)
{
	m_JITILFrame = new CordbJITILFrame(frameid, methoid, il_offset, flags, thread);
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetIP(ULONG32* pnOffset)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetIP - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::SetIP(ULONG32 nOffset)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - SetIP - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetRegisterSet(ICorDebugRegisterSet** ppRegisters)
{
	return thread->GetRegisterSet(ppRegisters);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterValue(CorDebugRegister reg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetLocalRegisterValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalDoubleRegisterValue(CorDebugRegister highWordReg, CorDebugRegister lowWordReg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetLocalDoubleRegisterValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryValue(CORDB_ADDRESS address, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetLocalMemoryValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterMemoryValue(CorDebugRegister highWordReg, CORDB_ADDRESS lowWordAddress, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetLocalRegisterMemoryValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryRegisterValue(CORDB_ADDRESS highWordAddress, CorDebugRegister lowWordRegister, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetLocalMemoryRegisterValue - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CanSetIP(ULONG32 nOffset)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - CanSetIP - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetChain(ICorDebugChain** ppChain)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetChain - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCode(ICorDebugCode** ppCode)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetCode - NOT IMPLEMENTED\n");
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunction(ICorDebugFunction** ppFunction)
{
	return m_JITILFrame->GetFunction(ppFunction);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunctionToken(mdMethodDef* pToken)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetFunctionToken - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackRange(CORDB_ADDRESS* pStart, CORDB_ADDRESS* pEnd)
{
	return m_JITILFrame->GetStackRange(pStart, pEnd);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCaller(ICorDebugFrame** ppFrame)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetCaller - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCallee(ICorDebugFrame** ppFrame)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetCallee - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CreateStepper(ICorDebugStepper** ppStepper)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - CreateStepper - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugFrame)
	{
		*pInterface = static_cast<ICorDebugFrame*>(static_cast<ICorDebugNativeFrame*>(this));
	}
	else if (id == IID_ICorDebugNativeFrame)
	{
		*pInterface = static_cast<ICorDebugNativeFrame*>(this);
	}
	else if (id == IID_ICorDebugNativeFrame2)
	{
		*pInterface = static_cast<ICorDebugNativeFrame2*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugNativeFrame*>(this));
	}
	else
	{
		// might be searching for an IL Frame. delegate that search to the
		// JITILFrame
		if (m_JITILFrame != NULL)
		{
			return m_JITILFrame->QueryInterface(id, pInterface);
		}
		else
		{
			*pInterface = NULL;
			return E_NOINTERFACE;
		}
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbNativeFrame::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbNativeFrame::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::IsChild(BOOL* pIsChild)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - IsChild - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::IsMatchingParentFrame(ICorDebugNativeFrame2* pPotentialParentFrame, BOOL* pIsParent)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - IsMatchingParentFrame - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackParameterSize(ULONG32* pSize)
{
	DEBUG_PRINTF(1, "CordbNativeFrame - GetStackParameterSize - NOT IMPLEMENTED\n");
	return E_NOTIMPL;
}
