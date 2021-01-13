#ifndef __MONO_DEBUGGER_CORDB_PROCESS_H__
#define __MONO_DEBUGGER_CORDB_PROCESS_H__

#include <cordb.hpp>

//based on ManagedEvent class of src/coreclr/debug/di/shimpriv.h
class ManagedEvent
{
public:
    virtual ~ManagedEvent();
    class DispatchArgs
    {
    public:
        DispatchArgs(ICorDebugManagedCallback* pCallback1, ICorDebugManagedCallback2* pCallback2, ICorDebugManagedCallback3* pCallback3, ICorDebugManagedCallback4* pCallback4);

        ICorDebugManagedCallback* GetCallback1();
        ICorDebugManagedCallback2* GetCallback2();
        ICorDebugManagedCallback3* GetCallback3();
        ICorDebugManagedCallback4* GetCallback4();


    protected:
        ICorDebugManagedCallback* m_pCallback1;
        ICorDebugManagedCallback2* m_pCallback2;
        ICorDebugManagedCallback3* m_pCallback3;
        ICorDebugManagedCallback4* m_pCallback4;
    };
    virtual HRESULT Dispatch(DispatchArgs args) = 0;
    // Returns 0 if none.
    DWORD GetOSTid();

protected:
    // Ctor for events with thread-affinity
    ManagedEvent(ICorDebugThread* pThread);

    // Ctor for events without thread affinity.
    ManagedEvent();

    friend class ManagedEventQueue;
    ManagedEvent* m_pNext;

    DWORD m_dwThreadId;
};

//
//based on ManagedEventQueue class of src/coreclr/debug/di/shimpriv.h
class ManagedEventQueue
{
public:
    ManagedEventQueue();


    void Init();

    // Remove event from the top. Caller then takes ownership of Event and will call Delete on it.
    // Caller checks IsEmpty() first.
    ManagedEvent* Dequeue();

    // Queue owns the event and will delete it (unless it's dequeued first).
    void QueueEvent(ManagedEvent* pEvent);

    // Test if event queue is empty
    bool IsEmpty();

    // Empty event queue and delete all objects
    void DeleteAll();

    // Nothrows
    BOOL HasQueuedCallbacks(ICorDebugThread* pThread);

    // Save the current queue and start with a new empty queue
    void SuspendQueue();

    // Restore the saved queue onto the end of the current queue
    void RestoreSuspendedQueue();

protected:
    // If empty,  First + Last are both NULL.
    // Else first points to the head of the queue; and Last points to the end of the queue.
    ManagedEvent* m_pFirstEvent;
    ManagedEvent* m_pLastEvent;

};


class EvalCompleteEvent : public ManagedEvent
{
    // callbacks parameters. These are strong references
    ICorDebugAppDomain *m_pAppDomain;
    ICorDebugThread *m_pThread;
    ICorDebugEval  *m_pEval;

public:
    // Ctor
    EvalCompleteEvent(ICorDebugAppDomain* pAppDomain, ICorDebugThread* pThread, ICorDebugEval* pEval);

    HRESULT Dispatch(DispatchArgs args);
};

class CordbProcess :
	public ICorDebugProcess,
	public ICorDebugProcess2,
	public ICorDebugProcess3,
	public ICorDebugProcess4,
	public ICorDebugProcess5,
	public ICorDebugProcess7,
	public ICorDebugProcess8,
	public ICorDebugProcess10,
	public ICorDebugProcess11
{
public:
    GPtrArray* appdomains;
    Connection *connection;
    int suspended;
	Cordb* cordb;
    //int eval_count;
    ManagedEventQueue* managed_event_queue;
    CordbProcess();
    HRESULT STDMETHODCALLTYPE EnumerateLoaderHeapMemoryRegions(/* [out] */ ICorDebugMemoryRangeEnum** ppRanges);
    HRESULT STDMETHODCALLTYPE EnableGCNotificationEvents(BOOL fEnable);
    HRESULT STDMETHODCALLTYPE EnableExceptionCallbacksOutsideOfMyCode(/* [in] */ BOOL enableExceptionsOutsideOfJMC);
    HRESULT STDMETHODCALLTYPE SetWriteableMetadataUpdateMode(WriteableMetadataUpdateMode flags);
    HRESULT STDMETHODCALLTYPE GetGCHeapInformation(/* [out] */ COR_HEAPINFO* pHeapInfo);
    HRESULT STDMETHODCALLTYPE EnumerateHeap(/* [out] */ ICorDebugHeapEnum** ppObjects);
    HRESULT STDMETHODCALLTYPE EnumerateHeapRegions(/* [out] */ ICorDebugHeapSegmentEnum** ppRegions);
    HRESULT STDMETHODCALLTYPE GetObject(/* [in] */ CORDB_ADDRESS addr,/* [out] */ ICorDebugObjectValue** pObject);
    HRESULT STDMETHODCALLTYPE EnumerateGCReferences(/* [in] */ BOOL enumerateWeakReferences,/* [out] */ ICorDebugGCReferenceEnum** ppEnum);
    HRESULT STDMETHODCALLTYPE EnumerateHandles(/* [in] */ CorGCReferenceType types,/* [out] */ ICorDebugGCReferenceEnum** ppEnum);
    HRESULT STDMETHODCALLTYPE GetTypeID(/* [in] */ CORDB_ADDRESS obj,/* [out] */ COR_TYPEID* pId);
    HRESULT STDMETHODCALLTYPE GetTypeForTypeID(/* [in] */ COR_TYPEID id,/* [out] */ ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetArrayLayout(/* [in] */ COR_TYPEID id,/* [out] */ COR_ARRAY_LAYOUT* pLayout);
    HRESULT STDMETHODCALLTYPE GetTypeLayout(/* [in] */ COR_TYPEID id,/* [out] */ COR_TYPE_LAYOUT* pLayout);
    HRESULT STDMETHODCALLTYPE GetTypeFields(/* [in] */ COR_TYPEID id,ULONG32 celt,COR_FIELD fields[],ULONG32* pceltNeeded);
    HRESULT STDMETHODCALLTYPE EnableNGENPolicy(/* [in] */ CorDebugNGENPolicy ePolicy);
    HRESULT STDMETHODCALLTYPE Filter(/* [size_is][length_is][in] */ const BYTE pRecord[],/* [in] */ DWORD countBytes,/* [in] */ CorDebugRecordFormat format,/* [in] */ DWORD dwFlags,/* [in] */ DWORD dwThreadId,/* [in] */ ICorDebugManagedCallback* pCallback,/* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus);
    HRESULT STDMETHODCALLTYPE ProcessStateChanged(/* [in] */ CorDebugStateChange eChange);
    HRESULT STDMETHODCALLTYPE SetEnableCustomNotification(ICorDebugClass* pClass,BOOL fEnable);
    HRESULT STDMETHODCALLTYPE GetID(/* [out] */ DWORD* pdwProcessId);
    HRESULT STDMETHODCALLTYPE GetHandle(/* [out] */ HPROCESS* phProcessHandle);
    HRESULT STDMETHODCALLTYPE GetThread(/* [in] */ DWORD dwThreadId,/* [out] */ ICorDebugThread** ppThread);
    HRESULT STDMETHODCALLTYPE EnumerateObjects(/* [out] */ ICorDebugObjectEnum** ppObjects);
    HRESULT STDMETHODCALLTYPE IsTransitionStub(/* [in] */ CORDB_ADDRESS address,/* [out] */ BOOL* pbTransitionStub);
    HRESULT STDMETHODCALLTYPE IsOSSuspended(/* [in] */ DWORD threadID,/* [out] */ BOOL* pbSuspended);
    HRESULT STDMETHODCALLTYPE GetThreadContext(/* [in] */ DWORD threadID,/* [in] */ ULONG32 contextSize,/* [size_is][length_is][out][in] */ BYTE context[]);
    HRESULT STDMETHODCALLTYPE SetThreadContext(/* [in] */ DWORD threadID,/* [in] */ ULONG32 contextSize,/* [size_is][length_is][in] */ BYTE context[]);
    HRESULT STDMETHODCALLTYPE ReadMemory(/* [in] */ CORDB_ADDRESS address,/* [in] */ DWORD size,/* [length_is][size_is][out] */ BYTE buffer[],/* [out] */ SIZE_T* read);
    HRESULT STDMETHODCALLTYPE WriteMemory(/* [in] */ CORDB_ADDRESS address,/* [in] */ DWORD size,/* [size_is][in] */ BYTE buffer[],/* [out] */ SIZE_T* written);
    HRESULT STDMETHODCALLTYPE ClearCurrentException(/* [in] */ DWORD threadID);
    HRESULT STDMETHODCALLTYPE EnableLogMessages(/* [in] */ BOOL fOnOff);
    HRESULT STDMETHODCALLTYPE ModifyLogSwitch(/* [annotation][in] */_In_  WCHAR* pLogSwitchName,/* [in] */ LONG lLevel);
    HRESULT STDMETHODCALLTYPE EnumerateAppDomains(/* [out] */ ICorDebugAppDomainEnum** ppAppDomains);
    HRESULT STDMETHODCALLTYPE GetObject(/* [out] */ ICorDebugValue** ppObject);
    HRESULT STDMETHODCALLTYPE ThreadForFiberCookie(/* [in] */ DWORD fiberCookie,/* [out] */ ICorDebugThread** ppThread);
    HRESULT STDMETHODCALLTYPE GetHelperThreadID(/* [out] */ DWORD* pThreadID);
    HRESULT STDMETHODCALLTYPE GetThreadForTaskID(/* [in] */ TASKID taskid,/* [out] */ ICorDebugThread2** ppThread);
    HRESULT STDMETHODCALLTYPE GetVersion(/* [out] */ COR_VERSION* version);
    HRESULT STDMETHODCALLTYPE SetUnmanagedBreakpoint(/* [in] */ CORDB_ADDRESS address,/* [in] */ ULONG32 bufsize,/* [length_is][size_is][out] */ BYTE buffer[],/* [out] */ ULONG32* bufLen);
    HRESULT STDMETHODCALLTYPE ClearUnmanagedBreakpoint(/* [in] */ CORDB_ADDRESS address);
    HRESULT STDMETHODCALLTYPE SetDesiredNGENCompilerFlags(/* [in] */ DWORD pdwFlags);
    HRESULT STDMETHODCALLTYPE GetDesiredNGENCompilerFlags(/* [out] */ DWORD* pdwFlags);
    HRESULT STDMETHODCALLTYPE GetReferenceValueFromGCHandle(/* [in] */ UINT_PTR handle,/* [out] */ ICorDebugReferenceValue** pOutValue);
    HRESULT STDMETHODCALLTYPE QueryInterface(/* [in] */ REFIID id,/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE Stop(/* [in] */ DWORD dwTimeoutIgnored);
    HRESULT STDMETHODCALLTYPE Continue(/* [in] */ BOOL fIsOutOfBand);
    HRESULT STDMETHODCALLTYPE IsRunning(/* [out] */ BOOL* pbRunning);
    HRESULT STDMETHODCALLTYPE HasQueuedCallbacks(/* [in] */ ICorDebugThread* pThread,/* [out] */ BOOL* pbQueued);
    HRESULT STDMETHODCALLTYPE EnumerateThreads(/* [out] */ ICorDebugThreadEnum** ppThreads);
    HRESULT STDMETHODCALLTYPE SetAllThreadsDebugState(/* [in] */ CorDebugThreadState state,/* [in] */ ICorDebugThread* pExceptThisThread);
    HRESULT STDMETHODCALLTYPE Detach(void);
    HRESULT STDMETHODCALLTYPE Terminate(/* [in] */ UINT exitCode);
    HRESULT STDMETHODCALLTYPE CanCommitChanges(/* [in] */ ULONG cSnapshots,/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],/* [out] */ ICorDebugErrorInfoEnum** pError);
    HRESULT STDMETHODCALLTYPE CommitChanges(/* [in] */ ULONG cSnapshots,/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],/* [out] */ ICorDebugErrorInfoEnum** pError);
};

#endif
