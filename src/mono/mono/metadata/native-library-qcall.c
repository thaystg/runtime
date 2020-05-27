#include "config.h"
#include "mono/metadata/assembly-internals.h"
#include "mono/metadata/class-internals.h"
#include "mono/metadata/icall-decl.h"
#include "mono/metadata/loader-internals.h"
#include "mono/metadata/loader.h"
#include "mono/metadata/object-internals.h"
#include "mono/metadata/reflection-internals.h"
#include "mono/utils/checked-build.h"
#include "mono/utils/mono-compiler.h"
#include "mono/utils/mono-logger-internals.h"
#include "mono/utils/mono-path.h"
#include "mono/metadata/native-library.h"

extern const void* gPalGlobalizationNative[];

const struct ECClass c_rgECClasses[] =
{
#define FCClassElement(name,namespace,funcs) {name, namespace, funcs},
FCClassElement("Globalization", "", gPalGlobalizationNative)
};  // c_rgECClasses[]

const int c_nECClasses = sizeof(c_rgECClasses)/sizeof(c_rgECClasses[0]);

static gboolean            IsEndOfArray(ECFunc *func)  { return !!((int)func->m_dwFlags & FCFuncFlag_EndOfArray); }
static gboolean            HasSignature(ECFunc *func)  { return !!((int)func->m_dwFlags & FCFuncFlag_HasSignature); }
static gboolean            IsUnreferenced(ECFunc *func){ return !!((int)func->m_dwFlags & FCFuncFlag_Unreferenced); }
static gboolean            IsQCall(ECFunc *func)       { return !!((int)func->m_dwFlags & FCFuncFlag_QCall); }
//CorInfoIntrinsics   IntrinsicID(ECFunc *func)   { return (CorInfoIntrinsics)((INT8)(func->m_dwFlags >> 16)); }
//int                 DynamicID(ECFunc *func)     { return (int)              ((int8)(func->m_dwFlags >> 24)); }

static ECFunc*             NextInArray(ECFunc *func)
{
    return (ECFunc*)((char*)func +sizeof(ECFunc));
        //(HasSignature(func) ? sizeof(ECFunc) : offsetof(ECFunc, func->m_pMethodSig)));
}

static int 
find_impls_index_for_class (MonoMethod* method)
{
    const char* namespace = m_class_get_name_space(method->klass);
    const char* name = m_class_get_name(method->klass);

    // Array classes get null from the above routine, but they have no ecalls.
    if (name == NULL)
        return -1;

    unsigned low  = 0;
    unsigned high = c_nECClasses;

#ifdef _DEBUG
    static bool checkedSort = false;
    if (!checkedSort) {
        checkedSort = true;
        for (unsigned i = 1; i < high; i++)  {
            // Make certain list is sorted!
            int cmp = strcmp(c_rgECClasses[i].m_szClassName, c_rgECClasses[i-1].m_szClassName);
            if (cmp == 0)
                cmp = strcmp(c_rgECClasses[i].m_szNameSpace, c_rgECClasses[i-1].m_szNameSpace);
            g_assert(cmp > 0); // Hey, you forgot to sort the new class
        }
    }
#endif // _DEBUG
    while (high > low) {
        unsigned mid  = (high + low) / 2;
        int cmp = strcmp(name, c_rgECClasses[mid].m_szClassName);
        if (cmp == 0)
            cmp = strcmp(namespace, c_rgECClasses[mid].m_szNameSpace);

        if (cmp == 0) {
            return(mid);
        }
        if (cmp > 0)
            low = mid+1;
        else
            high = mid;
    }
    return -1;
}

static int 
find_index_for_method (MonoMethod* method, const void **impls)
{
    const char* method_name = method->name;
    for (ECFunc* cur = (ECFunc*)impls; !IsEndOfArray(cur); cur = NextInArray(cur))
    {
        if (strcmp(cur->m_szMethodName, method_name) != 0)
            continue;
        return (int)((const void**)cur - impls);
    }

    return -1;
}

gpointer
mono_lookup_pinvoke_qcall_internal (MonoMethod *method, MonoLookupPInvokeStatus *status_out)
{
    int pos_class = find_impls_index_for_class (method);
    if (pos_class < 0)
        return NULL;
    int pos_method = find_index_for_method (method, c_rgECClasses[pos_class].m_pECFunc);
    if (pos_method < 0)
        return NULL;
    return  (gpointer)c_rgECClasses[pos_class].m_pECFunc[pos_method+1];
}