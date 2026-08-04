// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

#include "stdafx.h"
#include "exceptionfilter.h"

#include "../../vm/methodtable.h"
#include "../../vm/ceeload.h"

ExceptionFilterTable::ExceptionFilterTable(ULONG32 cEntries)
    : m_refCount(1)
    , m_cEntries(cEntries)
{
}

ExceptionFilterTable::~ExceptionFilterTable()
{
}

ExceptionFilterTable* ExceptionFilterTable::Create(const COR_DEBUG_EXCEPTION_FILTER_ENTRY* pEntries, ULONG32 cEntries)
{
    CONTRACTL
    {
        THROWS;
        GC_NOTRIGGER;
        MODE_ANY;
    }
    CONTRACTL_END;

    // The trailing m_entries array reserves space for at least one entry;
    // grow by (cEntries - 1) for any additional entries. cEntries == 0 is
    // legal and represents "no filter installed".
    size_t bytes = sizeof(ExceptionFilterTable);
    if (cEntries > 1)
    {
        bytes += (cEntries - 1) * sizeof(COR_DEBUG_EXCEPTION_FILTER_ENTRY);
    }

    void* mem = ::operator new(bytes);
    ExceptionFilterTable* pTable = new (mem) ExceptionFilterTable(cEntries);

    if (cEntries > 0 && pEntries != nullptr)
    {
        memcpy(pTable->m_entries, pEntries, cEntries * sizeof(COR_DEBUG_EXCEPTION_FILTER_ENTRY));
    }

    return pTable;
}

bool ExceptionFilterTable::EntryMatchesType(const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
                                            MethodTable* pExceptionMT,
                                            BOOL fCaught) const
{
    // Caught / uncaught gate. An entry with neither bit set was rejected by
    // CordbProcess::SetExceptionFilter validation, but be defensive.
    if (fCaught)
    {
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_CAUGHT) == 0)
        {
            return false;
        }
    }
    else
    {
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_UNCAUGHT) == 0)
        {
            return false;
        }
    }

    // EVERYTHING_ELSE has its typeDef ignored; it's matched separately by Match()
    // as the "no specific entry matched" bucket.
    if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_EVERYTHING_ELSE) != 0)
    {
        return false;
    }

    // Walk the MethodTable's type identity. Concord pushes
    // (moduleAddress, mdTypeDef) pairs that resolve at the EE side to a
    // unique MethodTable per loaded type. moduleAddress is the runtime
    // PTR_Module value Concord obtained via the ICorDebugModule mapping.
    if (pExceptionMT == nullptr)
    {
        return false;
    }

    MethodTable* pCurrentMT = pExceptionMT;
    bool includeSubclasses = (entry.flags & COR_DEBUG_EXCEPTION_FILTER_SUBCLASSES) != 0;

    do
    {
        Module* pCurrentModule = pCurrentMT->GetModule();
        // moduleAddress == 0 is "match any module" — useful for hosts that
        // add an exception type by name before the module has been resolved
        // to a specific address (matches Mono's behavior where the agent
        // identifies types by class id, not by module).
        //
        // Otherwise, moduleAddress is the EE-internal PTR_Module identifier
        // produced by the DBI right side (CordbProcess::SetExceptionFilter
        // translates the PE image base value the host passes through
        // ICorDebugProcess13::SetExceptionFilter into this Module* TADDR).
        bool moduleMatches = (entry.moduleAddress == 0) ||
            (pCurrentModule != nullptr &&
             entry.moduleAddress == (CORDB_ADDRESS)(SIZE_T)PTR_TO_TADDR(pCurrentModule));
        if (moduleMatches && entry.typeDef == pCurrentMT->GetCl())
        {
            return true;
        }

        if (!includeSubclasses)
        {
            return false;
        }

        pCurrentMT = pCurrentMT->GetParentMethodTable();
    } while (pCurrentMT != nullptr);

    return false;
}

BOOL ExceptionFilterTable::Match(MethodTable* pExceptionMT, BOOL fCaught) const
{
    // Empty table = no filter installed = back-compat behavior (always notify).
    if (m_cEntries == 0)
    {
        return TRUE;
    }

    // Two passes:
    //   Pass 1: any EXCLUDE entry that matches the (type, caught) immediately
    //           wins — the runtime suppresses the callback. This lets hosts
    //           express "break on everything EXCEPT these types" via an
    //           EVERYTHING_ELSE bucket plus per-type EXCLUDE entries.
    //   Pass 2: mirrors the Mono not_filtered_feature evaluation (debugger-
    //           agent.c lines 3403-3436): a specific entry that matches the
    //           type + caught flavor wins, otherwise EVERYTHING_ELSE /
    //           NOT_FILTERED buckets decide.

    for (ULONG32 i = 0; i < m_cEntries; i++)
    {
        const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry = m_entries[i];
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_EXCLUDE) == 0)
        {
            continue;
        }
        // Exclude entries are always specific (no EVERYTHING_ELSE blanket
        // exclude — that would be equivalent to "install no filter").
        if (EntryMatchesType(entry, pExceptionMT, fCaught))
        {
            return FALSE;
        }
    }

    bool anySpecificMatchedCaughtFlavor = false;
    bool hasEverythingElseMatchingCaughtFlavor = false;
    bool hasNotFilteredMatchingCaughtFlavor = false;

    for (ULONG32 i = 0; i < m_cEntries; i++)
    {
        const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry = m_entries[i];

        // EXCLUDE entries already handled in pass 1.
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_EXCLUDE) != 0)
        {
            continue;
        }

        const ULONG32 caughtBit = fCaught
                                    ? COR_DEBUG_EXCEPTION_FILTER_CAUGHT
                                    : COR_DEBUG_EXCEPTION_FILTER_UNCAUGHT;
        const bool entryCoversFlavor = (entry.flags & caughtBit) != 0;

        if (!entryCoversFlavor)
        {
            continue;
        }

        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_EVERYTHING_ELSE) != 0)
        {
            hasEverythingElseMatchingCaughtFlavor = true;
            continue;
        }

        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_NOT_FILTERED) != 0)
        {
            hasNotFilteredMatchingCaughtFlavor = true;
            continue;
        }

        // Specific (typeDef, moduleAddress) entry.
        if (EntryMatchesType(entry, pExceptionMT, fCaught))
        {
            anySpecificMatchedCaughtFlavor = true;
            // Early out: a specific match in the matching caught flavor is
            // sufficient.
            return TRUE;
        }
    }

    // No specific match. Apply buckets.
    if (hasEverythingElseMatchingCaughtFlavor)
    {
        return TRUE;
    }

    if (hasNotFilteredMatchingCaughtFlavor && !anySpecificMatchedCaughtFlavor)
    {
        return TRUE;
    }

    return FALSE;
}
