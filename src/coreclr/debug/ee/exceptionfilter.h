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
// Semantics mirror the Mono soft-debugger's MOD_KIND_EXCEPTION_ONLY modifier
// (see runtime/src/mono/mono/component/debugger-agent.c lines 3380-3436):
//
//   - "EVERYTHING_ELSE" is the catch-all bucket. It matches when no specific
//     entry matched the exception type.
//   - "NOT_FILTERED" inverts the per-entry decision: an entry with this flag
//     matches when no other entry matched (used by VS to break on everything
//     except a small list of types).
//   - "SUBCLASSES" makes the entry match the listed type AND any subclass.
//   - "CAUGHT" / "UNCAUGHT" gate whether the entry matches the given
//     caught/uncaught flavor of the exception.
//

#ifndef _EXCEPTION_FILTER_H_
#define _EXCEPTION_FILTER_H_

#include "cordebug.h"

class MethodTable;

class ExceptionFilterTable
{
public:
    // Builds a new table from a contiguous span of validated entries.
    // The caller has already validated that EVERYTHING_ELSE entries carry no
    // typeDef/moduleAddress and that every entry sets at least one of
    // CAUGHT / UNCAUGHT.
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

    // Apply the Mono-style match algorithm.
    //
    // pExceptionMT is the runtime type of the thrown object. fCaught indicates
    // whether the exception currently has a handler in scope (first-chance
    // caught path) or is destined to be unhandled. Returns TRUE if the
    // configured filter wants the debugger to be notified of this exception.
    //
    // An empty table (cEntries == 0) is treated as "no filter installed" and
    // always returns TRUE so back-compat with hosts that never call
    // SetExceptionFilter is preserved.
    BOOL Match(MethodTable* pExceptionMT, BOOL fCaught) const;

    bool IsEmpty() const { return m_cEntries == 0; }

private:
    ExceptionFilterTable(ULONG32 cEntries);
    ~ExceptionFilterTable();

    // Returns true when the entry's typeDef/moduleAddress matches pExceptionMT
    // (honoring the SUBCLASSES bit) AND the entry's caught/uncaught flags
    // include fCaught.
    bool EntryMatchesType(const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
                          MethodTable* pExceptionMT,
                          BOOL fCaught) const;

    LONG    m_refCount;
    ULONG32 m_cEntries;
    // Trailing array; allocated together with the object via operator new.
    COR_DEBUG_EXCEPTION_FILTER_ENTRY m_entries[1];
};

#endif // _EXCEPTION_FILTER_H_
