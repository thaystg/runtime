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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - Next - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Skip(ULONG celt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - Skip - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Reset(void)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - Reset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Clone(ICorDebugEnum** ppEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - Clone - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::GetCount(ULONG* pcelt)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - IMPLEMENTED" << endl;
	file.close();

	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum::GetCount" << std::endl;
	file.close();
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_int(&localbuf, 0);
	buffer_add_int(&localbuf, -1);

	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 1" << endl;
	file.close();

	int cmdId = thread->ppProcess->connection->send_event(CMD_SET_THREAD, CMD_THREAD_GET_FRAME_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		fstream file;
		file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
		file << "CordbFrameEnum - GetCount - 1" << endl;
		file.close();
		thread->ppProcess->connection->process_packet(true);
		file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
		file << "CordbFrameEnum - GetCount - 2" << endl;
		file.close();
		localbuf2 = (Buffer*)g_hash_table_lookup(thread->ppProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 3" << endl;
	file.close();
	nframes = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 4" << endl;
	file.close();

	frames = (CordbNativeFrame**) malloc(sizeof(CordbNativeFrame*) * nframes);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 5" << endl;
	file.close();
	for (int i = 0; i < nframes; i++)
	{
		file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
		file << "CordbFrameEnum - GetCount - 6" << endl;
		file.close();
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

	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 7" << endl;
	file.close();

	*pcelt = nframes;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - GetCount - 8 - " << nframes << endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::QueryInterface(REFIID riid, void** ppvObject)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrameEnum - QueryInterface - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetChain - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCode(
	/* [out] */ ICorDebugCode** ppCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetCode - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetFunction" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetFunctionToken(
	/* [out] */ mdMethodDef* pToken)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetFunctionToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackRange(
	/* [out] */ CORDB_ADDRESS* pStart,
	/* [out] */ CORDB_ADDRESS* pEnd)
{
	*pStart = 4096;
	*pEnd = 8192;
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetStackRange - NOT IMPLEMENTED - we need id?" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCaller(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetCaller - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCallee(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetCallee - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CreateStepper(
	/* [out] */ ICorDebugStepper** ppStepper)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - CreateStepper - NOT IMPLEMENTED" << std::endl;
	file.close();
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
		fstream file;
		file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
		file << "CordbFrame - QueryInterface - E_NOTIMPL" << std::endl;
		file.close();
		return E_NOINTERFACE;
	}
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - QueryInterface - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::RemapFunction(ULONG32 newILOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - RemapFunction - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - EnumerateTypeParameters - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetReturnValueForILOffset(ULONG32 ILoffset, ICorDebugValue** ppReturnValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetReturnValueForILOffset - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariablesEx(ILCodeKind flags, ICorDebugValueEnum** ppValueEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - EnumerateLocalVariablesEx - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetLocalVariableEx(ILCodeKind flags, DWORD dwIndex, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetLocalVariableEx - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbJITILFrame - GetCodeEx - IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetIP - IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbJITILFrame::SetIP(
	/* [in] */ ULONG32 nOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - SetIP - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariables(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - EnumerateLocalVariables - NOT IMPLEMENTED" << std::endl;
	file.close();
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

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		thread->ppProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(thread->ppProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int type = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int value = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	*ppValue = new CordbValue(ELEMENT_TYPE_I4, value, 4);


	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetArgument - IMPLEMENTED - dwIndex" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateArguments(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - EnumerateArguments - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	buffer_add_int(&localbuf, 1);
	buffer_add_int(&localbuf, -1);

	int cmdId = thread->ppProcess->connection->send_event(CMD_SET_STACK_FRAME, CMD_STACK_FRAME_GET_VALUES, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		thread->ppProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(thread->ppProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	
	int type = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int value = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	*ppValue = new CordbValue(ELEMENT_TYPE_I4, value, 4);

	
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetArgument - IMPLEMENTED - dwIndex" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackDepth(
	/* [out] */ ULONG32* pDepth)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetStackDepth - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackValue(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - GetStackValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CanSetIP(
	/* [in] */ ULONG32 nOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbFrame - CanSetIP - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}


CordbNativeFrame::CordbNativeFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread)
{
	m_JITILFrame = new CordbJITILFrame(frameid, methoid, il_offset, flags, thread);
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetIP(ULONG32* pnOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetIP - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::SetIP(ULONG32 nOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - SetIP - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetRegisterSet(ICorDebugRegisterSet** ppRegisters)
{
	return thread->GetRegisterSet(ppRegisters);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterValue(CorDebugRegister reg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetLocalRegisterValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalDoubleRegisterValue(CorDebugRegister highWordReg, CorDebugRegister lowWordReg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetLocalDoubleRegisterValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryValue(CORDB_ADDRESS address, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetLocalMemoryValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterMemoryValue(CorDebugRegister highWordReg, CORDB_ADDRESS lowWordAddress, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetLocalRegisterMemoryValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryRegisterValue(CORDB_ADDRESS highWordAddress, CorDebugRegister lowWordRegister, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetLocalMemoryRegisterValue - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CanSetIP(ULONG32 nOffset)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - CanSetIP - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetChain(ICorDebugChain** ppChain)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetChain - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCode(ICorDebugCode** ppCode)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetCode - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunction(ICorDebugFunction** ppFunction)
{
	return m_JITILFrame->GetFunction(ppFunction);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunctionToken(mdMethodDef* pToken)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetFunctionToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackRange(CORDB_ADDRESS* pStart, CORDB_ADDRESS* pEnd)
{
	return m_JITILFrame->GetStackRange(pStart, pEnd);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCaller(ICorDebugFrame** ppFrame)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetCaller - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCallee(ICorDebugFrame** ppFrame)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetCallee - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CreateStepper(ICorDebugStepper** ppStepper)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - CreateStepper - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - IsChild - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::IsMatchingParentFrame(ICorDebugNativeFrame2* pPotentialParentFrame, BOOL* pIsParent)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - IsMatchingParentFrame - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackParameterSize(ULONG32* pSize)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbNativeFrame - GetStackParameterSize - NOT IMPLEMENTED" << std::endl;
	file.close();
	return E_NOTIMPL;
}
