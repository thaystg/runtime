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
        if (cEntries - 1 > (SIZE_MAX - bytes) / sizeof(COR_DEBUG_EXCEPTION_FILTER_ENTRY))
        {
            ThrowOutOfMemory();
        }
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

ULONG32 ExceptionFilterTable::GetTypeMatchDepth(
    const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
    MethodTable* pExceptionMT) const
{
    if ((entry.flags & (COR_DEBUG_EXCEPTION_FILTER_MODULE_DEFAULT |
                        COR_DEBUG_EXCEPTION_FILTER_PROCESS_DEFAULT)) != 0)
    {
        return ULONG_MAX;
    }

    // Walk the MethodTable's type identity. Concord pushes
    // (moduleAddress, mdTypeDef) pairs that resolve at the EE side to a
    // unique MethodTable per loaded type. moduleAddress is the runtime
    // PTR_Module value Concord obtained via the ICorDebugModule mapping.
    if (pExceptionMT == nullptr)
    {
        return ULONG_MAX;
    }

    MethodTable* pCurrentMT = pExceptionMT;
    bool includeSubclasses = (entry.flags & COR_DEBUG_EXCEPTION_FILTER_SUBCLASSES) != 0;
    ULONG32 depth = 0;

    do
    {
        Module* pCurrentModule = pCurrentMT->GetModule();
        bool moduleMatches = pCurrentModule != nullptr &&
            entry.moduleAddress == static_cast<CORDB_ADDRESS>(PTR_TO_TADDR(pCurrentModule));
        if (moduleMatches && entry.typeDef == pCurrentMT->GetCl())
        {
            return depth;
        }

        if (!includeSubclasses)
        {
            return ULONG_MAX;
        }

        pCurrentMT = pCurrentMT->GetParentMethodTable();
        depth++;
    } while (pCurrentMT != nullptr);

    return ULONG_MAX;
}

ExceptionNotificationAction ExceptionFilterTable::GetEntryAction(
    const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry,
    CorDebugExceptionNotificationStage stage) const
{
    ULONG32 stageMask = static_cast<ULONG32>(stage);
    if ((entry.stoppingMask & stageMask) != 0)
    {
        return ExceptionNotificationAction::Stop;
    }
    if ((entry.notificationMask & stageMask) != 0)
    {
        return ExceptionNotificationAction::Notify;
    }
    return ExceptionNotificationAction::Ignore;
}

ExceptionNotificationAction ExceptionFilterTable::GetAction(
    MethodTable* pExceptionMT,
    CorDebugExceptionNotificationStage stage) const
{
    if (m_cEntries == 0)
    {
        return ExceptionNotificationAction::Stop;
    }

    const COR_DEBUG_EXCEPTION_FILTER_ENTRY* pModuleDefault = nullptr;
    const COR_DEBUG_EXCEPTION_FILTER_ENTRY* pProcessDefault = nullptr;
    const COR_DEBUG_EXCEPTION_FILTER_ENTRY* pBestTypeMatch = nullptr;
    ULONG32 bestTypeMatchDepth = ULONG_MAX;
    Module* pExceptionModule = pExceptionMT == nullptr ? nullptr : pExceptionMT->GetModule();
    CORDB_ADDRESS exceptionModuleAddress = pExceptionModule == nullptr
        ? 0
        : static_cast<CORDB_ADDRESS>(PTR_TO_TADDR(pExceptionModule));

    for (ULONG32 i = 0; i < m_cEntries; i++)
    {
        const COR_DEBUG_EXCEPTION_FILTER_ENTRY& entry = m_entries[i];
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_PROCESS_DEFAULT) != 0)
        {
            pProcessDefault = &entry;
            continue;
        }
        if ((entry.flags & COR_DEBUG_EXCEPTION_FILTER_MODULE_DEFAULT) != 0)
        {
            if (entry.moduleAddress == exceptionModuleAddress)
            {
                pModuleDefault = &entry;
            }
            continue;
        }
        ULONG32 matchDepth = GetTypeMatchDepth(entry, pExceptionMT);
        if (matchDepth < bestTypeMatchDepth)
        {
            pBestTypeMatch = &entry;
            bestTypeMatchDepth = matchDepth;
        }
    }

    if (pBestTypeMatch != nullptr)
    {
        return GetEntryAction(*pBestTypeMatch, stage);
    }
    if (pModuleDefault != nullptr)
    {
        return GetEntryAction(*pModuleDefault, stage);
    }
    if (pProcessDefault != nullptr)
    {
        return GetEntryAction(*pProcessDefault, stage);
    }
    return ExceptionNotificationAction::Ignore;
}
