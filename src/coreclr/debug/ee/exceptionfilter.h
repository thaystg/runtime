// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

//
// File: exceptionfilter.h
//
// Runtime-side managed exception filter table installed via
// ICorDebugProcess13::SetExceptionFilter. The Debugger keeps a single live
// snapshot pointer; updates allocate a new table and atomically publish it
// under the debugger Crst. Readers acquire the Crst only briefly to AddRef
// the live snapshot, then walk the entries lock-free.
//
#ifndef _EXCEPTION_FILTER_H_
#define _EXCEPTION_FILTER_H_

#include "cordebug.h"

class MethodTable;

enum class ExceptionNotificationAction
{
    Ignore,
    Notify,
    Stop
};

class ExceptionFilterTable
{
public:
    static ExceptionFilterTable* Create(const COR_DEBUG_EXCEPTION_FILTER_ENTRY* pEntries, ULONG32 cEntries);

    LONG AddRef()  { return InterlockedIncrement(&m_refCount); }
    LONG Release()
    {
        LONG c = InterlockedDecrement(&m_refCount);
        if (c == 0)
        {
            delete this;
        }
        return c;
    }

    ExceptionNotificationAction GetAction(
        MethodTable* pExceptionMT,
        CorDebugExceptionNotificationStage stage) const;

    bool IsEmpty() const { return m_cEntries == 0; }

private:
    ExceptionFilterTable(ULONG32 cEntries);
    ~ExceptionFilterTable();

    ULONG32 GetTypeMatchDepth(const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
                              MethodTable* pExceptionMT) const;
    ExceptionNotificationAction GetEntryAction(
        const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
        CorDebugExceptionNotificationStage stage) const;

    LONG    m_refCount;
    ULONG32 m_cEntries;
    // Trailing array; allocated together with the object via operator new.
    COR_DEBUG_EXCEPTION_FILTER_ENTRY m_entries[1];
};

#endif // _EXCEPTION_FILTER_H_
