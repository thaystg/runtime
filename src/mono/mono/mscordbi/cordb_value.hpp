#ifndef __MONO_DEBUGGER_CORDB_VALUE_H__
#define __MONO_DEBUGGER_CORDB_VALUE_H__

#include <cordb.hpp>

union CordbContent
{
    gint16 charValue;
    gint8 booleanValue;
    gint32 intValue;
    gint64 longValue;
    void* pointerValue;
};

class CordbValue :
    public ICorDebugValue2, 
    public ICorDebugValue3,
    public ICorDebugGenericValue
{
    CorElementType type;
    CordbContent value;
    int size;
    Connection* conn;
public:
    CordbValue(Connection* conn, CorElementType type, CordbContent value, int size);
    HRESULT STDMETHODCALLTYPE GetType(CorElementType* pType);
    HRESULT STDMETHODCALLTYPE GetSize(ULONG32* pSize);
    HRESULT STDMETHODCALLTYPE GetAddress(CORDB_ADDRESS* pAddress);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetExactType(ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetSize64(ULONG64* pSize);
    HRESULT STDMETHODCALLTYPE GetValue(void* pTo);
    HRESULT STDMETHODCALLTYPE SetValue(void* pFrom);
};

class CordbReferenceValue : public ICorDebugReferenceValue, public ICorDebugValue2, public ICorDebugValue3, public ICorDebugGenericValue
{
    CorElementType type;
    int object_id;
    Connection* conn;
    CordbClass* klass;
public:
    CordbReferenceValue(Connection *conn, CorElementType type, int object_id);
    HRESULT STDMETHODCALLTYPE GetType(CorElementType* pType);
    HRESULT STDMETHODCALLTYPE GetSize(ULONG32* pSize);
    HRESULT STDMETHODCALLTYPE GetAddress(CORDB_ADDRESS* pAddress);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetExactType(ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetSize64(ULONG64* pSize);
    HRESULT STDMETHODCALLTYPE GetValue(void* pTo);
    HRESULT STDMETHODCALLTYPE SetValue(void* pFrom);
    HRESULT STDMETHODCALLTYPE IsNull(BOOL* pbNull);
    HRESULT STDMETHODCALLTYPE GetValue(CORDB_ADDRESS* pValue);
    HRESULT STDMETHODCALLTYPE SetValue(CORDB_ADDRESS value);
    HRESULT STDMETHODCALLTYPE Dereference(ICorDebugValue** ppValue);
    HRESULT STDMETHODCALLTYPE DereferenceStrong(ICorDebugValue** ppValue);
};

class CordbObjectValue : public ICorDebugObjectValue,
    public ICorDebugObjectValue2,
    public ICorDebugGenericValue,
    public ICorDebugStringValue,
    public ICorDebugValue2,
    public ICorDebugValue3,
    public ICorDebugHeapValue2,
    public ICorDebugHeapValue3,
    public ICorDebugExceptionObjectValue,
    public ICorDebugComObjectValue,
    public ICorDebugDelegateObjectValue
{
    CorElementType type;
    int object_id;
    Connection* conn;
    CordbClass* klass;
public:
    CordbObjectValue(Connection* conn, CorElementType type, int object_id, CordbClass* klass);
    HRESULT STDMETHODCALLTYPE GetClass(/* [out] */ ICorDebugClass** ppClass);
    HRESULT STDMETHODCALLTYPE GetFieldValue(ICorDebugClass* pClass, mdFieldDef fieldDef, ICorDebugValue** ppValue);

    static HRESULT CreateCordbValue(Connection* conn, Buffer* localbuf2, ICorDebugValue** ppValue);

    HRESULT STDMETHODCALLTYPE GetVirtualMethod(mdMemberRef memberRef, ICorDebugFunction** ppFunction);
    HRESULT STDMETHODCALLTYPE GetContext(ICorDebugContext** ppContext);
    HRESULT STDMETHODCALLTYPE IsValueClass(BOOL* pbIsValueClass);
    HRESULT STDMETHODCALLTYPE GetManagedCopy(IUnknown** ppObject);
    HRESULT STDMETHODCALLTYPE SetFromManagedCopy(IUnknown* pObject);
    HRESULT STDMETHODCALLTYPE IsValid(BOOL* pbValid);
    HRESULT STDMETHODCALLTYPE CreateRelocBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE GetType(CorElementType* pType);
    HRESULT STDMETHODCALLTYPE GetSize(ULONG32* pSize);
    HRESULT STDMETHODCALLTYPE GetAddress(CORDB_ADDRESS* pAddress);
    HRESULT STDMETHODCALLTYPE CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetExactType(ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetSize64(ULONG64* pSize);
    HRESULT STDMETHODCALLTYPE GetValue(void* pTo);
    HRESULT STDMETHODCALLTYPE SetValue(void* pFrom);
    HRESULT STDMETHODCALLTYPE GetVirtualMethodAndType(mdMemberRef memberRef, ICorDebugFunction** ppFunction,ICorDebugType** ppType);
    HRESULT STDMETHODCALLTYPE GetLength(ULONG32* pcchString);
    HRESULT STDMETHODCALLTYPE GetString(ULONG32 cchString, ULONG32* pcchString, WCHAR szString[]);
    HRESULT STDMETHODCALLTYPE CreateHandle(CorDebugHandleType type, ICorDebugHandleValue** ppHandle);
    HRESULT STDMETHODCALLTYPE GetThreadOwningMonitorLock( ICorDebugThread** ppThread, DWORD* pAcquisitionCount);
    HRESULT STDMETHODCALLTYPE GetMonitorEventWaitList(ICorDebugThreadEnum** ppThreadEnum);
    HRESULT STDMETHODCALLTYPE EnumerateExceptionCallStack(ICorDebugExceptionObjectCallStackEnum** ppCallStackEnum);
    HRESULT STDMETHODCALLTYPE GetCachedInterfaceTypes(BOOL bIInspectableOnly, ICorDebugTypeEnum** ppInterfacesEnum);
    HRESULT STDMETHODCALLTYPE GetCachedInterfacePointers(BOOL bIInspectableOnly, ULONG32 celt, ULONG32* pcEltFetched, CORDB_ADDRESS* ptrs);
    HRESULT STDMETHODCALLTYPE GetTarget(ICorDebugReferenceValue** ppObject);
    HRESULT STDMETHODCALLTYPE GetFunction(ICorDebugFunction** ppFunction);
};
#endif
