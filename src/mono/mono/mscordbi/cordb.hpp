#ifndef __MONO_DEBUGGER_CORDB_H__
#define __MONO_DEBUGGER_CORDB_H__

#define MONO_DLL_EXPORT

#include <mono\utils\mono-publib.h>
#include <mono\mini\debugger-protocol.h>
#include <mono\mini\debugger-engine.h>
#include <stdio.h>


#ifdef WIN32
#include <windows.h>
#endif
#include "cor.h"
#include "corhdr.h"
#include "cordebug.h"
#include "xcordebug.h"
#include <WS2tcpip.h>

static MonoCoopMutex debug_mutex;

#define dbg_lock() mono_os_mutex_lock (&debug_mutex.m)
#define dbg_unlock() mono_os_mutex_unlock (&debug_mutex.m)

#define return_if_nok(error) do { if (!is_ok ((error))) return S_FALSE; } while (0)

/*1   0x00007DA0    CoreCLRCreateCordbObject
2   0x00007CA0    CoreCLRCreateCordbObjectEx
3   0x00007C60    CreateCordbObject
4   0x00007DC0    DllGetClassObjectInternal
5   0x0000D0C0    OpenProcess
6   0x0000D070    OpenProcess2
7   0x0000CE30    OpenProcessImpl
8   0x0000CFF0    OpenProcessImpl2*/

class Cordb;
class CordbProcess;
class CordbAppDomain;
class CordbAssembly;
class CordbCode;
class CordbThread;
class CordbFunction;
class CordbStepper;
class CordbRegisteSet;
class CordbJITILFrame;

struct M128BIT {
    uint64_t Low;
    int64_t  High;
};


struct AMD64_XMM_SAVE_AREA32 {
    uint16_t ControlWord;
    uint16_t StatusWord;
    uint8_t  TagWord;
    uint8_t  Reserved1;
    uint16_t ErrorOpcode;
    uint32_t ErrorOffset;
    uint16_t ErrorSelector;
    uint16_t Reserved2;
    uint32_t DataOffset;
    uint16_t DataSelector;
    uint16_t Reserved3;
    uint32_t MxCsr;
    uint32_t MxCsr_Mask;
    M128BIT  FloatRegisters[8];
    M128BIT  XmmRegisters[16];
    uint8_t  Reserved4[96];
};

struct AMD64_CONTEXT {

    //
    // Register parameter home addresses.
    //

    uint64_t P1Home;
    uint64_t P2Home;
    uint64_t P3Home;
    uint64_t P4Home;
    uint64_t P5Home;
    uint64_t P6Home;

    //
    // Control flags.
    //

    uint32_t ContextFlags;
    uint32_t MxCsr;

    //
    // Segment Registers and processor flags.
    //

    uint16_t SegCs;
    uint16_t SegDs;
    uint16_t SegEs;
    uint16_t SegFs;
    uint16_t SegGs;
    uint16_t SegSs;
    uint32_t EFlags;

    //
    // Debug registers
    //

    uint64_t Dr0;
    uint64_t Dr1;
    uint64_t Dr2;
    uint64_t Dr3;
    uint64_t Dr6;
    uint64_t Dr7;

    //
    // Integer registers.
    //

    uint64_t Rax;
    uint64_t Rcx;
    uint64_t Rdx;
    uint64_t Rbx;
    uint64_t Rsp;
    uint64_t Rbp;
    uint64_t Rsi;
    uint64_t Rdi;
    uint64_t R8;
    uint64_t R9;
    uint64_t R10;
    uint64_t R11;
    uint64_t R12;
    uint64_t R13;
    uint64_t R14;
    uint64_t R15;

    //
    // Program counter.
    //

    uint64_t Rip;

    //
    // Floating point state.
    //

    union {
        AMD64_XMM_SAVE_AREA32 FltSave;
        struct {
            M128BIT Header[2];
            M128BIT Legacy[8];
            M128BIT Xmm0;
            M128BIT Xmm1;
            M128BIT Xmm2;
            M128BIT Xmm3;
            M128BIT Xmm4;
            M128BIT Xmm5;
            M128BIT Xmm6;
            M128BIT Xmm7;
            M128BIT Xmm8;
            M128BIT Xmm9;
            M128BIT Xmm10;
            M128BIT Xmm11;
            M128BIT Xmm12;
            M128BIT Xmm13;
            M128BIT Xmm14;
            M128BIT Xmm15;
        };
    };

    //
    // Vector registers.
    //

    M128BIT VectorRegister[26];
    uint64_t VectorControl;

    //
    // Special debug control registers.
    //

    uint64_t DebugControl;
    uint64_t LastBranchToRip;
    uint64_t LastBranchFromRip;
    uint64_t LastExceptionToRip;
    uint64_t LastExceptionFromRip;
};

class Cordb : public ICorDebug, public ICorDebugRemote
{
public:
    GPtrArray* breakpoints;
    GPtrArray* threads;
    GPtrArray* functions;
    ICorDebugManagedCallback* pCallback;
    Cordb();

    CordbFunction* findFunction(int id);
    CordbFunction* findFunctionByToken(int token);
    HRESULT Initialize(void);

    HRESULT Terminate(void);

    HRESULT SetManagedHandler(
        /* [in] */ ICorDebugManagedCallback* pCallback);

    HRESULT SetUnmanagedHandler(
        /* [in] */ ICorDebugUnmanagedCallback* pCallback);

    HRESULT CreateProcess(
        /* [in] */ LPCWSTR lpApplicationName,
        /* [in] */ LPWSTR lpCommandLine,
        /* [in] */ LPSECURITY_ATTRIBUTES lpProcessAttributes,
        /* [in] */ LPSECURITY_ATTRIBUTES lpThreadAttributes,
        /* [in] */ BOOL bInheritHandles,
        /* [in] */ DWORD dwCreationFlags,
        /* [in] */ PVOID lpEnvironment,
        /* [in] */ LPCWSTR lpCurrentDirectory,
        /* [in] */ LPSTARTUPINFOW lpStartupInfo,
        /* [in] */ LPPROCESS_INFORMATION lpProcessInformation,
        /* [in] */ CorDebugCreateProcessFlags debuggingFlags,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT DebugActiveProcess(
        /* [in] */ DWORD id,
        /* [in] */ BOOL win32Attach,
        /* [out] */ ICorDebugProcess** ppProcess);
    HRESULT EnumerateProcesses(
        /* [out] */ ICorDebugProcessEnum** ppProcess);

    HRESULT GetProcess(
        /* [in] */ DWORD dwProcessId,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT CanLaunchOrAttach(
        /* [in] */ DWORD dwProcessId,
        /* [in] */ BOOL win32DebuggingEnabled);
    HRESULT QueryInterface(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject);

    ULONG AddRef(void);
    ULONG Release(void);
    HRESULT CreateProcessEx(
        /* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
        /* [in] */ LPCWSTR lpApplicationName,
        /* [annotation][in] */
        _In_  LPWSTR lpCommandLine,
        /* [in] */ LPSECURITY_ATTRIBUTES lpProcessAttributes,
        /* [in] */ LPSECURITY_ATTRIBUTES lpThreadAttributes,
        /* [in] */ BOOL bInheritHandles,
        /* [in] */ DWORD dwCreationFlags,
        /* [in] */ PVOID lpEnvironment,
        /* [in] */ LPCWSTR lpCurrentDirectory,
        /* [in] */ LPSTARTUPINFOW lpStartupInfo,
        /* [in] */ LPPROCESS_INFORMATION lpProcessInformation,
        /* [in] */ CorDebugCreateProcessFlags debuggingFlags,
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT DebugActiveProcessEx(
        /* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
        /* [in] */ DWORD dwProcessId,
        /* [in] */ BOOL fWin32Attach,
        /* [out] */ ICorDebugProcess** ppProcess);
};

class Connection
{
	SOCKET connect_socket;
	CordbProcess* ppProcess;
	Cordb* ppCordb;
    bool is_answer_pending;
public:
    CordbAppDomain* pCorDebugAppDomain;
	GHashTable* received_replies;
	GPtrArray* received_replies_to_process;
	Connection(CordbProcess* proc, Cordb* cordb);
	void loop_send_receive();
	void process_packet_internal(Buffer* recvbuf);
	void process_packet_from_queue();
	void enable_event(EventKind eventKind);
	void close_connection();
	void start_connection();
	void transport_handshake();
	void send_packet(Buffer& sendbuf);
	void receive_packet(Buffer& b, int len);
	void receive_header(Header* header);
	int send_event(int cmd_set, int cmd, Buffer* sendbuf);
	int process_packet(bool is_answer = false);
    CordbThread* findThread(GPtrArray *threads, long thread_id);
};

static Connection* connection;



class CordbSymbol :
    public IMetaDataImport,
    public IMetaDataImport2,
    public IMetaDataAssemblyImport
{
    CordbAssembly* pCordbAssembly;
    int module_id;
public:

    CordbSymbol(CordbAssembly* cordbAssembly);

    STDMETHOD(EnumGenericParams)(
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdToken      tk,                    // [IN] TypeDef or MethodDef whose generic parameters are requested
        mdGenericParam rGenericParams[],    // [OUT] Put GenericParams here.
        ULONG       cMax,                   // [IN] Max GenericParams to put.
        ULONG* pcGenericParams);

    STDMETHOD(GetGenericParamProps)(        // S_OK or error.
        mdGenericParam gp,                  // [IN] GenericParam
        ULONG* pulParamSeq,          // [OUT] Index of the type parameter
        DWORD* pdwParamFlags,        // [OUT] Flags, for future use (e.g. variance)
        mdToken* ptOwner,              // [OUT] Owner (TypeDef or MethodDef)
        DWORD* reserved,              // [OUT] For future use (e.g. non-type parameters)
        _Out_writes_to_opt_(cchName, *pchName)
        LPWSTR       wzname,                // [OUT] Put name here
        ULONG        cchName,               // [IN] Size of buffer
        ULONG* pchName); // [OUT] Put size of name (wide chars) here.

    STDMETHOD(GetMethodSpecProps)(
        mdMethodSpec mi,                    // [IN] The method instantiation
        mdToken* tkParent,                  // [OUT] MethodDef or MemberRef
        PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
        ULONG* pcbSigBlob);    // [OUT] actual size of signature blob

    STDMETHOD(EnumGenericParamConstraints)(
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdGenericParam tk,                  // [IN] GenericParam whose constraints are requested
        mdGenericParamConstraint rGenericParamConstraints[],    // [OUT] Put GenericParamConstraints here.
        ULONG       cMax,                   // [IN] Max GenericParamConstraints to put.
        ULONG* pcGenericParamConstraints);// [OUT] Put # put here.

    STDMETHOD(GetGenericParamConstraintProps)( // S_OK or error.
        mdGenericParamConstraint gpc,       // [IN] GenericParamConstraint
        mdGenericParam* ptGenericParam,     // [OUT] GenericParam that is constrained
        mdToken* ptkConstraintType);// [OUT] TypeDef/Ref/Spec constraint

    STDMETHOD(GetPEKind)(                   // S_OK or error.
        DWORD* pdwPEKind,                   // [OUT] The kind of PE (0 - not a PE)
        DWORD* pdwMAchine);       // [OUT] Machine as defined in NT header

    STDMETHOD(GetVersionString)(            // S_OK or error.
        _Out_writes_to_opt_(ccBufSize, *pccBufSize)
        LPWSTR      pwzBuf,                 // [OUT] Put version string here.
        DWORD       ccBufSize,              // [IN] size of the buffer, in wide chars
        DWORD* pccBufSize);   // [OUT] Size of the version string, wide chars, including terminating nul.

    STDMETHOD(EnumMethodSpecs)(
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdToken      tk,                    // [IN] MethodDef or MemberRef whose MethodSpecs are requested
        mdMethodSpec rMethodSpecs[],        // [OUT] Put MethodSpecs here.
        ULONG       cMax,                   // [IN] Max tokens to put.
        ULONG* pcMethodSpecs);  // [OUT] Put actual count here.

    STDMETHOD(GetAssemblyProps)(            // S_OK or error.
        mdAssembly  mda,                    // [IN] The Assembly for which to get the properties.
        const void** ppbPublicKey,         // [OUT] Pointer to the public key.
        ULONG* pcbPublicKey,          // [OUT] Count of bytes in the public key.
        ULONG* pulHashAlgId,          // [OUT] Hash Algorithm.
        _Out_writes_to_opt_(cchName, *pchName) LPWSTR  szName, // [OUT] Buffer to fill with assembly's simply name.
        ULONG       cchName,                // [IN] Size of buffer in wide chars.
        ULONG* pchName,               // [OUT] Actual # of wide chars in name.
        ASSEMBLYMETADATA* pMetaData,        // [OUT] Assembly MetaData.
        DWORD* pdwAssemblyFlags); // [OUT] Flags.

    STDMETHOD(GetAssemblyRefProps)(         // S_OK or error.
    mdAssemblyRef mdar,                 // [IN] The AssemblyRef for which to get the properties.
    const void** ppbPublicKeyOrToken,  // [OUT] Pointer to the public key or token.
    ULONG* pcbPublicKeyOrToken,   // [OUT] Count of bytes in the public key or token.
    _Out_writes_to_opt_(cchName, *pchName)LPWSTR szName, // [OUT] Buffer to fill with name.
    ULONG       cchName,                // [IN] Size of buffer in wide chars.
    ULONG* pchName,               // [OUT] Actual # of wide chars in name.
    ASSEMBLYMETADATA* pMetaData,        // [OUT] Assembly MetaData.
    const void** ppbHashValue,         // [OUT] Hash blob.
    ULONG* pcbHashValue,          // [OUT] Count of bytes in the hash blob.
    DWORD* pdwAssemblyRefFlags);// [OUT] Flags.

    STDMETHOD(GetFileProps)(                // S_OK or error.
        mdFile      mdf,                    // [IN] The File for which to get the properties.
        _Out_writes_to_opt_(cchName, *pchName) LPWSTR      szName, // [OUT] Buffer to fill with name.
        ULONG       cchName,                // [IN] Size of buffer in wide chars.
        ULONG* pchName,               // [OUT] Actual # of wide chars in name.
        const void** ppbHashValue,         // [OUT] Pointer to the Hash Value Blob.
        ULONG* pcbHashValue,          // [OUT] Count of bytes in the Hash Value Blob.
        DWORD* pdwFileFlags);// [OUT] Flags.

    STDMETHOD(GetExportedTypeProps)(        // S_OK or error.
        mdExportedType   mdct,              // [IN] The ExportedType for which to get the properties.
        _Out_writes_to_opt_(cchName, *pchName) LPWSTR      szName, // [OUT] Buffer to fill with name.
        ULONG       cchName,                // [IN] Size of buffer in wide chars.
        ULONG* pchName,               // [OUT] Actual # of wide chars in name.
        mdToken* ptkImplementation,     // [OUT] mdFile or mdAssemblyRef or mdExportedType.
        mdTypeDef* ptkTypeDef,            // [OUT] TypeDef token within the file.
        DWORD* pdwExportedTypeFlags);// [OUT] Flags.

    STDMETHOD(GetManifestResourceProps)(    // S_OK or error.
        mdManifestResource  mdmr,           // [IN] The ManifestResource for which to get the properties.
        _Out_writes_to_opt_(cchName, *pchName)LPWSTR      szName,  // [OUT] Buffer to fill with name.
        ULONG       cchName,                // [IN] Size of buffer in wide chars.
        ULONG* pchName,               // [OUT] Actual # of wide chars in name.
        mdToken* ptkImplementation,     // [OUT] mdFile or mdAssemblyRef that provides the ManifestResource.
        DWORD* pdwOffset,             // [OUT] Offset to the beginning of the resource within the file.
        DWORD* pdwResourceFlags);// [OUT] Flags.

    STDMETHOD(EnumAssemblyRefs)(            // S_OK or error
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdAssemblyRef rAssemblyRefs[],      // [OUT] Put AssemblyRefs here.
        ULONG       cMax,                   // [IN] Max AssemblyRefs to put.
        ULONG* pcTokens);// [OUT] Put # put here.

    STDMETHOD(EnumFiles)(                   // S_OK or error
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdFile      rFiles[],               // [OUT] Put Files here.
        ULONG       cMax,                   // [IN] Max Files to put.
        ULONG* pcTokens);// [OUT] Put # put here.

    STDMETHOD(EnumExportedTypes)(           // S_OK or error
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdExportedType   rExportedTypes[],  // [OUT] Put ExportedTypes here.
        ULONG       cMax,                   // [IN] Max ExportedTypes to put.
        ULONG* pcTokens);// [OUT] Put # put here.

    STDMETHOD(EnumManifestResources)(       // S_OK or error
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdManifestResource  rManifestResources[],   // [OUT] Put ManifestResources here.
        ULONG       cMax,                   // [IN] Max Resources to put.
        ULONG* pcTokens);// [OUT] Put # put here.

	STDMETHOD(GetAssemblyFromScope)(        // S_OK or error
		mdAssembly* ptkAssembly);

    STDMETHOD(FindExportedTypeByName)(      // S_OK or error
        LPCWSTR     szName,                 // [IN] Name of the ExportedType.
        mdToken     mdtExportedType,        // [IN] ExportedType for the enclosing class.
        mdExportedType* ptkExportedType);// [OUT] Put the ExportedType token here.

    STDMETHOD(FindManifestResourceByName)(  // S_OK or error
        LPCWSTR     szName,                 // [IN] Name of the ManifestResource.
        mdManifestResource* ptkManifestResource);// [OUT] Put the ManifestResource token here.

    STDMETHOD(FindAssembliesByName)(        // S_OK or error
        LPCWSTR  szAppBase,                 // [IN] optional - can be NULL
        LPCWSTR  szPrivateBin,              // [IN] optional - can be NULL
        LPCWSTR  szAssemblyName,            // [IN] required - this is the assembly you are requesting
        IUnknown* ppIUnk[],                 // [OUT] put IMetaDataAssemblyImport pointers here
        ULONG    cMax,                      // [IN] The max number to put
        ULONG* pcAssemblies);// [OUT] The number of assemblies returned.

	// IUnknown methods
     HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObj);

     ULONG STDMETHODCALLTYPE AddRef();
     ULONG STDMETHODCALLTYPE Release();


	// IMetaDataImport functions

    void STDMETHODCALLTYPE CloseEnum(HCORENUM hEnum);
     HRESULT STDMETHODCALLTYPE CountEnum(HCORENUM hEnum, ULONG* pulCount);
     HRESULT STDMETHODCALLTYPE ResetEnum(HCORENUM hEnum, ULONG ulPos);
     HRESULT STDMETHODCALLTYPE EnumTypeDefs(HCORENUM* phEnum, mdTypeDef rTypeDefs[],
        ULONG cMax, ULONG* pcTypeDefs);
     HRESULT STDMETHODCALLTYPE EnumInterfaceImpls(HCORENUM* phEnum, mdTypeDef td,
        mdInterfaceImpl rImpls[], ULONG cMax,
        ULONG* pcImpls);
     HRESULT STDMETHODCALLTYPE EnumTypeRefs(HCORENUM* phEnum, mdTypeRef rTypeRefs[],
        ULONG cMax, ULONG* pcTypeRefs);

     HRESULT STDMETHODCALLTYPE FindTypeDefByName(           // S_OK or error.
        LPCWSTR     szTypeDef,              // [IN] Name of the Type.
        mdToken     tkEnclosingClass,       // [IN] TypeDef/TypeRef for Enclosing class.
        mdTypeDef* ptd);             // [OUT] Put the TypeDef token here.
	

     HRESULT STDMETHODCALLTYPE GetScopeProps(               // S_OK or error.
        __out_ecount_part_opt(cchName, *pchName)
        LPWSTR      szName,                 // [OUT] Put the name here.
        ULONG       cchName,                // [IN] Size of name buffer in wide chars.
        ULONG* pchName,               // [OUT] Put size of name (wide chars) here.
        GUID* pmvid);           // [OUT, OPTIONAL] Put MVID here.
	
     HRESULT STDMETHODCALLTYPE GetModuleFromScope(          // S_OK.
        mdModule* pmd);            // [OUT] Put mdModule token here.
	


     HRESULT STDMETHODCALLTYPE GetTypeDefProps(             // S_OK or error.
        mdTypeDef   td,                     // [IN] TypeDef token for inquiry.
        __out_ecount_part_opt(cchTypeDef, *pchTypeDef)
        LPWSTR      szTypeDef,              // [OUT] Put name here.
        ULONG       cchTypeDef,             // [IN] size of name buffer in wide chars.
        ULONG* pchTypeDef,            // [OUT] put size of name (wide chars) here.
        DWORD* pdwTypeDefFlags,       // [OUT] Put flags here.
        mdToken* ptkExtends);     // [OUT] Put base class TypeDef/TypeRef here.

	

     HRESULT STDMETHODCALLTYPE GetInterfaceImplProps(       // S_OK or error.
        mdInterfaceImpl iiImpl,             // [IN] InterfaceImpl token.
        mdTypeDef* pClass,                // [OUT] Put implementing class token here.
        mdToken* ptkIface);        // [OUT] Put implemented interface token here.

     HRESULT STDMETHODCALLTYPE GetTypeRefProps(             // S_OK or error.
        mdTypeRef   tr,                     // [IN] TypeRef token.
        mdToken* ptkResolutionScope,    // [OUT] Resolution scope, ModuleRef or AssemblyRef.
        __out_ecount_part_opt(cchName, *pchName)
        LPWSTR      szName,                 // [OUT] Name of the TypeRef.
        ULONG       cchName,                // [IN] Size of buffer.
        ULONG* pchName);         // [OUT] Size of Name.


     HRESULT STDMETHODCALLTYPE ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown** ppIScope, mdTypeDef* ptd);

     HRESULT STDMETHODCALLTYPE EnumMembers(                 // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        mdToken     rMembers[],             // [OUT] Put MemberDefs here.
        ULONG       cMax,                   // [IN] Max MemberDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumMembersWithName(         // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        LPCWSTR     szName,                 // [IN] Limit results to those with this name.
        mdToken     rMembers[],             // [OUT] Put MemberDefs here.
        ULONG       cMax,                   // [IN] Max MemberDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.



     HRESULT STDMETHODCALLTYPE EnumMethods(                 // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        mdMethodDef rMethods[],             // [OUT] Put MethodDefs here.
        ULONG       cMax,                   // [IN] Max MethodDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumMethodsWithName(         // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        LPCWSTR     szName,                 // [IN] Limit results to those with this name.
        mdMethodDef rMethods[],             // [OU] Put MethodDefs here.
        ULONG       cMax,                   // [IN] Max MethodDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumFields(                  // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        mdFieldDef  rFields[],              // [OUT] Put FieldDefs here.
        ULONG       cMax,                   // [IN] Max FieldDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.



     HRESULT STDMETHODCALLTYPE EnumFieldsWithName(          // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
        LPCWSTR     szName,                 // [IN] Limit results to those with this name.
        mdFieldDef  rFields[],              // [OUT] Put MemberDefs here.
        ULONG       cMax,                   // [IN] Max MemberDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.



     HRESULT STDMETHODCALLTYPE EnumParams(                  // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
        mdParamDef  rParams[],              // [OUT] Put ParamDefs here.
        ULONG       cMax,                   // [IN] Max ParamDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumMemberRefs(              // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdToken     tkParent,               // [IN] Parent token to scope the enumeration.
        mdMemberRef rMemberRefs[],          // [OUT] Put MemberRefs here.
        ULONG       cMax,                   // [IN] Max MemberRefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumMethodImpls(             // S_OK, S_FALSE, or error
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
        mdToken     rMethodBody[],          // [OUT] Put Method Body tokens here.
        mdToken     rMethodDecl[],          // [OUT] Put Method Declaration tokens here.
        ULONG       cMax,                   // [IN] Max tokens to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumPermissionSets(          // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdToken     tk,                     // [IN] if !NIL, token to scope the enumeration.
        DWORD       dwActions,              // [IN] if !0, return only these actions.
        mdPermission rPermission[],         // [OUT] Put Permissions here.
        ULONG       cMax,                   // [IN] Max Permissions to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE FindMember(
        mdTypeDef   td,                     // [IN] given typedef
        LPCWSTR     szName,                 // [IN] member name
        PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
        ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
        mdToken* pmb);             // [OUT] matching memberdef


     HRESULT STDMETHODCALLTYPE FindMethod(
        mdTypeDef   td,                     // [IN] given typedef
        LPCWSTR     szName,                 // [IN] member name
        PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
        ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
        mdMethodDef* pmb);             // [OUT] matching memberdef


     HRESULT STDMETHODCALLTYPE FindField(
        mdTypeDef   td,                     // [IN] given typedef
        LPCWSTR     szName,                 // [IN] member name
        PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
        ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
        mdFieldDef* pmb);             // [OUT] matching memberdef


     HRESULT STDMETHODCALLTYPE FindMemberRef(
        mdTypeRef   td,                     // [IN] given typeRef
        LPCWSTR     szName,                 // [IN] member name
        PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
        ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
        mdMemberRef* pmr);             // [OUT] matching memberref



     HRESULT STDMETHODCALLTYPE GetMethodProps(
        mdMethodDef mb,                     // The method for which to get props.
        mdTypeDef* pClass,                // Put method's class here.
        __out_ecount_part_opt(cchMethod, *pchMethod)
        LPWSTR      szMethod,               // Put method's name here.
        ULONG       cchMethod,              // Size of szMethod buffer in wide chars.
        ULONG* pchMethod,             // Put actual size here
        DWORD* pdwAttr,               // Put flags here.
        PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
        ULONG* pcbSigBlob,            // [OUT] actual size of signature blob
        ULONG* pulCodeRVA,            // [OUT] codeRVA
        DWORD* pdwImplFlags);    // [OUT] Impl. Flags



     HRESULT STDMETHODCALLTYPE GetMemberRefProps(           // S_OK or error.
        mdMemberRef mr,                     // [IN] given memberref
        mdToken* ptk,                   // [OUT] Put classref or classdef here.
        __out_ecount_part_opt(cchMember, *pchMember)
        LPWSTR      szMember,               // [OUT] buffer to fill for member's name
        ULONG       cchMember,              // [IN] the count of char of szMember
        ULONG* pchMember,             // [OUT] actual count of char in member name
        PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to meta data blob value
        ULONG* pbSig);           // [OUT] actual size of signature blob


     HRESULT STDMETHODCALLTYPE EnumProperties(              // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
        mdProperty  rProperties[],          // [OUT] Put Properties here.
        ULONG       cMax,                   // [IN] Max properties to put.
        ULONG* pcProperties);    // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE EnumEvents(                  // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
        mdEvent     rEvents[],              // [OUT] Put events here.
        ULONG       cMax,                   // [IN] Max events to put.
        ULONG* pcEvents);        // [OUT] Put # put here.



     HRESULT STDMETHODCALLTYPE GetEventProps(               // S_OK, S_FALSE, or error.
        mdEvent     ev,                     // [IN] event token
        mdTypeDef* pClass,                // [OUT] typedef containing the event declarion.
        LPCWSTR     szEvent,                // [OUT] Event name
        ULONG       cchEvent,               // [IN] the count of wchar of szEvent
        ULONG* pchEvent,              // [OUT] actual count of wchar for event's name
        DWORD* pdwEventFlags,         // [OUT] Event flags.
        mdToken* ptkEventType,          // [OUT] EventType class
        mdMethodDef* pmdAddOn,              // [OUT] AddOn method of the event
        mdMethodDef* pmdRemoveOn,           // [OUT] RemoveOn method of the event
        mdMethodDef* pmdFire,               // [OUT] Fire method of the event
        mdMethodDef rmdOtherMethod[],       // [OUT] other method of the event
        ULONG       cMax,                   // [IN] size of rmdOtherMethod
        ULONG* pcOtherMethod);   // [OUT] total number of other method of this event


     HRESULT STDMETHODCALLTYPE EnumMethodSemantics(         // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
        mdToken     rEventProp[],           // [OUT] Put Event/Property here.
        ULONG       cMax,                   // [IN] Max properties to put.
        ULONG* pcEventProp);     // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE GetMethodSemantics(          // S_OK, S_FALSE, or error.
        mdMethodDef mb,                     // [IN] method token
        mdToken     tkEventProp,            // [IN] event/property token.
        DWORD* pdwSemanticsFlags); // [OUT] the role flags for the method/propevent pair



     HRESULT STDMETHODCALLTYPE GetClassLayout(
        mdTypeDef   td,                     // [IN] give typedef
        DWORD* pdwPackSize,           // [OUT] 1, 2, 4, 8, or 16
        COR_FIELD_OFFSET rFieldOffset[],    // [OUT] field offset array
        ULONG       cMax,                   // [IN] size of the array
        ULONG* pcFieldOffset,         // [OUT] needed array size
        ULONG* pulClassSize);        // [OUT] the size of the class


     HRESULT STDMETHODCALLTYPE GetFieldMarshal(
        mdToken     tk,                     // [IN] given a field's memberdef
        PCCOR_SIGNATURE* ppvNativeType,     // [OUT] native type of this field
        ULONG* pcbNativeType);   // [OUT] the count of bytes of *ppvNativeType


     HRESULT STDMETHODCALLTYPE GetRVA(                      // S_OK or error.
        mdToken     tk,                     // Member for which to set offset
        ULONG* pulCodeRVA,            // The offset
        DWORD* pdwImplFlags);    // the implementation flags


     HRESULT STDMETHODCALLTYPE GetPermissionSetProps(
        mdPermission pm,                    // [IN] the permission token.
        DWORD* pdwAction,             // [OUT] CorDeclSecurity.
        void const** ppvPermission,        // [OUT] permission blob.
        ULONG* pcbPermission);   // [OUT] count of bytes of pvPermission.


     HRESULT STDMETHODCALLTYPE GetSigFromToken(             // S_OK or error.
        mdSignature mdSig,                  // [IN] Signature token.
        PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to token.
        ULONG* pcbSig);         // [OUT] return size of signature.


     HRESULT STDMETHODCALLTYPE GetModuleRefProps(           // S_OK or error.
        mdModuleRef mur,                    // [IN] moduleref token.
        __out_ecount_part_opt(cchName, *pchName)
        LPWSTR      szName,                 // [OUT] buffer to fill with the moduleref name.
        ULONG       cchName,                // [IN] size of szName in wide characters.
        ULONG* pchName);         // [OUT] actual count of characters in the name.


     HRESULT STDMETHODCALLTYPE EnumModuleRefs(              // S_OK or error.
        HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
        mdModuleRef rModuleRefs[],          // [OUT] put modulerefs here.
        ULONG       cmax,                   // [IN] max memberrefs to put.
        ULONG* pcModuleRefs);    // [OUT] put # put here.



     HRESULT STDMETHODCALLTYPE GetTypeSpecFromToken(        // S_OK or error.
        mdTypeSpec typespec,                // [IN] TypeSpec token.
        PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to TypeSpec signature
        ULONG* pcbSig);          // [OUT] return size of signature.


     HRESULT STDMETHODCALLTYPE GetNameFromToken(            // Not Recommended! May be removed!
        mdToken     tk,                     // [IN] Token to get name from.  Must have a name.
        MDUTF8CSTR* pszUtf8NamePtr);  // [OUT] Return pointer to UTF8 name in heap.



     HRESULT STDMETHODCALLTYPE EnumUnresolvedMethods(       // S_OK, S_FALSE, or error.
        HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
        mdToken     rMethods[],             // [OUT] Put MemberDefs here.
        ULONG       cMax,                   // [IN] Max MemberDefs to put.
        ULONG* pcTokens);        // [OUT] Put # put here.


     HRESULT STDMETHODCALLTYPE GetUserString(               // S_OK or error.
        mdString    stk,                    // [IN] String token.
        __out_ecount_part_opt(cchString, *pchString)
        LPWSTR      szString,               // [OUT] Copy of string.
        ULONG       cchString,              // [IN] Max chars of room in szString.
        ULONG* pchString);       // [OUT] How many chars in actual string.


     HRESULT STDMETHODCALLTYPE GetPinvokeMap(               // S_OK or error.
        mdToken     tk,                     // [IN] FieldDef or MethodDef.
        DWORD* pdwMappingFlags,       // [OUT] Flags used for mapping.
        __out_ecount_part_opt(cchImportName, *pchImportName)
        LPWSTR      szImportName,           // [OUT] Import name.
        ULONG       cchImportName,          // [IN] Size of the name buffer.
        ULONG* pchImportName,         // [OUT] Actual number of characters stored.
        mdModuleRef* pmrImportDLL);    // [OUT] ModuleRef token for the target DLL.


     HRESULT STDMETHODCALLTYPE EnumSignatures(              // S_OK or error.
        HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
        mdSignature rSignatures[],          // [OUT] put signatures here.
        ULONG       cmax,                   // [IN] max signatures to put.
        ULONG* pcSignatures);    // [OUT] put # put here.


     HRESULT STDMETHODCALLTYPE EnumTypeSpecs(               // S_OK or error.
        HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
        mdTypeSpec  rTypeSpecs[],           // [OUT] put TypeSpecs here.
        ULONG       cmax,                   // [IN] max TypeSpecs to put.
        ULONG* pcTypeSpecs);     // [OUT] put # put here.


     HRESULT STDMETHODCALLTYPE EnumUserStrings(             // S_OK or error.
        HCORENUM* phEnum,                // [IN/OUT] pointer to the enum.
        mdString    rStrings[],             // [OUT] put Strings here.
        ULONG       cmax,                   // [IN] max Strings to put.
        ULONG* pcStrings);       // [OUT] put # put here.

     HRESULT STDMETHODCALLTYPE GetParamForMethodIndex(      // S_OK or error.
        mdMethodDef md,                     // [IN] Method token.
        ULONG       ulParamSeq,             // [IN] Parameter sequence.
        mdParamDef* ppd);             // [IN] Put Param token here.


     HRESULT STDMETHODCALLTYPE EnumCustomAttributes(        // S_OK or error.
        HCORENUM* phEnum,                // [IN, OUT] COR enumerator.
        mdToken     tk,                     // [IN] Token to scope the enumeration, 0 for all.
        mdToken     tkType,                 // [IN] Type of interest, 0 for all.
        mdCustomAttribute rCustomAttributes[], // [OUT] Put custom attribute tokens here.
        ULONG       cMax,                   // [IN] Size of rCustomAttributes.
        ULONG* pcCustomAttributes);  // [OUT, OPTIONAL] Put count of token values here.


     HRESULT STDMETHODCALLTYPE GetCustomAttributeProps(     // S_OK or error.
        mdCustomAttribute cv,               // [IN] CustomAttribute token.
        mdToken* ptkObj,                // [OUT, OPTIONAL] Put object token here.
        mdToken* ptkType,               // [OUT, OPTIONAL] Put AttrType token here.
        void const** ppBlob,               // [OUT, OPTIONAL] Put pointer to data here.
        ULONG* pcbSize);         // [OUT, OPTIONAL] Put size of date here.


     HRESULT STDMETHODCALLTYPE FindTypeRef(
        mdToken     tkResolutionScope,      // [IN] ModuleRef, AssemblyRef or TypeRef.
        LPCWSTR     szName,                 // [IN] TypeRef Name.
        mdTypeRef* ptr);             // [OUT] matching TypeRef.


     HRESULT STDMETHODCALLTYPE GetMemberProps(
        mdToken     mb,                     // The member for which to get props.
        mdTypeDef* pClass,                // Put member's class here.
        __out_ecount_part_opt(cchMember, *pchMember)
        LPWSTR      szMember,               // Put member's name here.
        ULONG       cchMember,              // Size of szMember buffer in wide chars.
        ULONG* pchMember,             // Put actual size here
        DWORD* pdwAttr,               // Put flags here.
        PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
        ULONG* pcbSigBlob,            // [OUT] actual size of signature blob
        ULONG* pulCodeRVA,            // [OUT] codeRVA
        DWORD* pdwImplFlags,          // [OUT] Impl. Flags
        DWORD* pdwCPlusTypeFlag,      // [OUT] flag for value type. selected ELEMENT_TYPE_*
        UVCP_CONSTANT* ppValue,             // [OUT] constant value
        ULONG* pcchValue);       // [OUT] size of constant string in chars, 0 for non-strings.
	

     HRESULT STDMETHODCALLTYPE GetFieldProps(
        mdFieldDef  mb,                     // The field for which to get props.
        mdTypeDef* pClass,                // Put field's class here.
        __out_ecount_part_opt(cchField, *pchField)
        LPWSTR      szField,                // Put field's name here.
        ULONG       cchField,               // Size of szField buffer in wide chars.
        ULONG* pchField,              // Put actual size here
        DWORD* pdwAttr,               // Put flags here.
        PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
        ULONG* pcbSigBlob,            // [OUT] actual size of signature blob
        DWORD* pdwCPlusTypeFlag,      // [OUT] flag for value type. selected ELEMENT_TYPE_*
        UVCP_CONSTANT* ppValue,             // [OUT] constant value
        ULONG* pcchValue);       // [OUT] size of constant string in chars, 0 for non-strings.


     HRESULT STDMETHODCALLTYPE GetPropertyProps(            // S_OK, S_FALSE, or error.
        mdProperty  prop,                   // [IN] property token
        mdTypeDef* pClass,                // [OUT] typedef containing the property declarion.
        LPCWSTR     szProperty,             // [OUT] Property name
        ULONG       cchProperty,            // [IN] the count of wchar of szProperty
        ULONG* pchProperty,           // [OUT] actual count of wchar for property name
        DWORD* pdwPropFlags,          // [OUT] property flags.
        PCCOR_SIGNATURE* ppvSig,            // [OUT] property type. pointing to meta data internal blob
        ULONG* pbSig,                 // [OUT] count of bytes in *ppvSig
        DWORD* pdwCPlusTypeFlag,      // [OUT] flag for value type. selected ELEMENT_TYPE_*
        UVCP_CONSTANT* ppDefaultValue,      // [OUT] constant value
        ULONG* pcchDefaultValue,      // [OUT] size of constant string in chars, 0 for non-strings.
        mdMethodDef* pmdSetter,             // [OUT] setter method of the property
        mdMethodDef* pmdGetter,             // [OUT] getter method of the property
        mdMethodDef rmdOtherMethod[],       // [OUT] other method of the property
        ULONG       cMax,                   // [IN] size of rmdOtherMethod
        ULONG* pcOtherMethod);   // [OUT] total number of other method of this property


     HRESULT STDMETHODCALLTYPE GetParamProps(               // S_OK or error.
        mdParamDef  tk,                     // [IN]The Parameter.
        mdMethodDef* pmd,                   // [OUT] Parent Method token.
        ULONG* pulSequence,           // [OUT] Parameter sequence.
        __out_ecount_part_opt(cchName, *pchName)
        LPWSTR      szName,                 // [OUT] Put name here.
        ULONG       cchName,                // [OUT] Size of name buffer.
        ULONG* pchName,               // [OUT] Put actual size of name here.
        DWORD* pdwAttr,               // [OUT] Put flags here.
        DWORD* pdwCPlusTypeFlag,      // [OUT] Flag for value type. selected ELEMENT_TYPE_*.
        UVCP_CONSTANT* ppValue,             // [OUT] Constant value.
        ULONG* pcchValue);       // [OUT] size of constant string in chars, 0 for non-strings.


     HRESULT STDMETHODCALLTYPE GetCustomAttributeByName(    // S_OK or error.
        mdToken     tkObj,                  // [IN] Object with Custom Attribute.
        LPCWSTR     szName,                 // [IN] Name of desired Custom Attribute.
        const void** ppData,               // [OUT] Put pointer to data here.
        ULONG* pcbData);         // [OUT] Put size of data here.


     BOOL STDMETHODCALLTYPE IsValidToken(         // True or False.
        mdToken     tk);               // [IN] Given token.


     HRESULT STDMETHODCALLTYPE GetNestedClassProps(         // S_OK or error.
        mdTypeDef   tdNestedClass,          // [IN] NestedClass token.
        mdTypeDef* ptdEnclosingClass); // [OUT] EnclosingClass token.


     HRESULT STDMETHODCALLTYPE GetNativeCallConvFromSig(    // S_OK or error.
        void const* pvSig,                 // [IN] Pointer to signature.
        ULONG       cbSig,                  // [IN] Count of signature bytes.
        ULONG* pCallConv);       // [OUT] Put calling conv here (see CorPinvokemap).


     HRESULT STDMETHODCALLTYPE IsGlobal(                    // S_OK or error.
        mdToken     pd,                     // [IN] Type, Field, or Method token.
        int* pbGlobal);        // [OUT] Put 1 if global, 0 otherwise.

};


class CordbAppDomain :
	public ICorDebugAppDomain,
	public ICorDebugAppDomain2,
	public ICorDebugAppDomain3,
	public ICorDebugAppDomain4
{
public:
    CordbAppDomain(ICorDebugProcess* ppProcess);
private:
	ICorDebugProcess* pProcess;

     HRESULT STDMETHODCALLTYPE Stop(
        /* [in] */ DWORD dwTimeoutIgnored);

     HRESULT STDMETHODCALLTYPE Continue(
        /* [in] */ BOOL fIsOutOfBand);

     HRESULT STDMETHODCALLTYPE IsRunning(
        /* [out] */ BOOL* pbRunning);

     HRESULT STDMETHODCALLTYPE HasQueuedCallbacks(
        /* [in] */ ICorDebugThread* pThread,
        /* [out] */ BOOL* pbQueued);

     HRESULT STDMETHODCALLTYPE EnumerateThreads(
        /* [out] */ ICorDebugThreadEnum** ppThreads);

     HRESULT STDMETHODCALLTYPE SetAllThreadsDebugState(
        /* [in] */ CorDebugThreadState state,
        /* [in] */ ICorDebugThread* pExceptThisThread);

     HRESULT STDMETHODCALLTYPE Detach(void);

     HRESULT STDMETHODCALLTYPE Terminate(
        /* [in] */ UINT exitCode);

     HRESULT STDMETHODCALLTYPE CanCommitChanges(
        /* [in] */ ULONG cSnapshots,
        /* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
        /* [out] */ ICorDebugErrorInfoEnum** pError);

     HRESULT STDMETHODCALLTYPE CommitChanges(
        /* [in] */ ULONG cSnapshots,
        /* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
        /* [out] */ ICorDebugErrorInfoEnum** pError);

     HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface);

     ULONG STDMETHODCALLTYPE AddRef(void);

     ULONG STDMETHODCALLTYPE Release(void);

     HRESULT STDMETHODCALLTYPE GetProcess(
        /* [out] */ ICorDebugProcess** ppProcess);

     HRESULT STDMETHODCALLTYPE EnumerateAssemblies(
        /* [out] */ ICorDebugAssemblyEnum** ppAssemblies);


     HRESULT STDMETHODCALLTYPE GetModuleFromMetaDataInterface(
        /* [in] */ IUnknown* pIMetaData,
        /* [out] */ ICorDebugModule** ppModule);


     HRESULT STDMETHODCALLTYPE EnumerateBreakpoints(
        /* [out] */ ICorDebugBreakpointEnum** ppBreakpoints);

     HRESULT STDMETHODCALLTYPE EnumerateSteppers(
        /* [out] */ ICorDebugStepperEnum** ppSteppers);


     HRESULT STDMETHODCALLTYPE IsAttached(
        /* [out] */ BOOL* pbAttached);


     HRESULT STDMETHODCALLTYPE GetName(
        /* [in] */ ULONG32 cchName,
        /* [out] */ ULONG32* pcchName,
        /* [length_is][size_is][out] */ WCHAR szName[]);


     HRESULT STDMETHODCALLTYPE GetObject(
        /* [out] */ ICorDebugValue** ppObject);

     HRESULT STDMETHODCALLTYPE Attach(void);


     HRESULT STDMETHODCALLTYPE GetID(
        /* [out] */ ULONG32* pId);

     HRESULT STDMETHODCALLTYPE GetArrayOrPointerType(
        /* [in] */ CorElementType elementType,
        /* [in] */ ULONG32 nRank,
        /* [in] */ ICorDebugType* pTypeArg,
        /* [out] */ ICorDebugType** ppType);


     HRESULT STDMETHODCALLTYPE GetFunctionPointerType(
        /* [in] */ ULONG32 nTypeArgs,
        /* [size_is][in] */ ICorDebugType* ppTypeArgs[],
        /* [out] */ ICorDebugType** ppType);

     HRESULT STDMETHODCALLTYPE GetCachedWinRTTypesForIIDs(
        /* [in] */ ULONG32 cReqTypes,
        /* [size_is][in] */ GUID* iidsToResolve,
        /* [out] */ ICorDebugTypeEnum** ppTypesEnum);


     HRESULT STDMETHODCALLTYPE GetCachedWinRTTypes(
        /* [out] */ ICorDebugGuidToTypeEnum** ppGuidToTypeEnum);

     HRESULT STDMETHODCALLTYPE GetObjectForCCW(
        /* [in] */ CORDB_ADDRESS ccwPointer,
        /* [out] */ ICorDebugValue** ppManagedObject);

};


class CordbModule : 
	public ICorDebugModule,
	public ICorDebugModule2,
	public ICorDebugModule3,
	public ICorDebugModule4
{
public:
	int id; //id on mono side;
	CordbProcess* pProcess;
	CordbSymbol* pCordbSymbol;
	CordbAssembly* pAssembly;
    guint8* assembly_metadata_blob;
    guint32 assembly_metadata_len;

    CordbModule(CordbProcess* process, CordbAssembly* assembly, int id_assembly);

    HRESULT CordbModule::QueryInterface(REFIID id, void** pInterface);

     ULONG STDMETHODCALLTYPE AddRef(void);

     ULONG STDMETHODCALLTYPE Release(void);
     HRESULT STDMETHODCALLTYPE IsMappedLayout(
        /* [out] */ BOOL* pIsMapped);
     HRESULT STDMETHODCALLTYPE CreateReaderForInMemorySymbols(
        /* [in] */ REFIID riid,
        /* [iid_is][out] */ void** ppObj);

     HRESULT STDMETHODCALLTYPE SetJMCStatus(
        /* [in] */ BOOL bIsJustMyCode,
        /* [in] */ ULONG32 cTokens,
        /* [size_is][in] */ mdToken pTokens[]);

     HRESULT STDMETHODCALLTYPE ApplyChanges(
        /* [in] */ ULONG cbMetadata,
        /* [size_is][in] */ BYTE pbMetadata[],
        /* [in] */ ULONG cbIL,
        /* [size_is][in] */ BYTE pbIL[]);

     HRESULT STDMETHODCALLTYPE SetJITCompilerFlags(
        /* [in] */ DWORD dwFlags);

     HRESULT STDMETHODCALLTYPE GetJITCompilerFlags(
        /* [out] */ DWORD* pdwFlags);

     HRESULT STDMETHODCALLTYPE ResolveAssembly(
        /* [in] */ mdToken tkAssemblyRef,
        /* [out] */ ICorDebugAssembly** ppAssembly);

     HRESULT STDMETHODCALLTYPE GetProcess(
        /* [out] */ ICorDebugProcess** ppProcess);

     HRESULT STDMETHODCALLTYPE GetBaseAddress(
        /* [out] */ CORDB_ADDRESS* pAddress);

	 HRESULT STDMETHODCALLTYPE GetAssembly(
		/* [out] */ ICorDebugAssembly** ppAssembly);

     HRESULT STDMETHODCALLTYPE GetName(
        /* [in] */ ULONG32 cchName,
        /* [out] */ ULONG32* pcchName,
        /* [length_is][size_is][out] */ WCHAR szName[]);

     HRESULT STDMETHODCALLTYPE EnableJITDebugging(
        /* [in] */ BOOL bTrackJITInfo,
        /* [in] */ BOOL bAllowJitOpts);

     HRESULT STDMETHODCALLTYPE EnableClassLoadCallbacks(
        /* [in] */ BOOL bClassLoadCallbacks);

     HRESULT STDMETHODCALLTYPE GetFunctionFromToken(
        /* [in] */ mdMethodDef methodDef,
        /* [out] */ ICorDebugFunction** ppFunction);

     HRESULT STDMETHODCALLTYPE GetFunctionFromRVA(
        /* [in] */ CORDB_ADDRESS rva,
        /* [out] */ ICorDebugFunction** ppFunction);

     HRESULT STDMETHODCALLTYPE GetClassFromToken(
        /* [in] */ mdTypeDef typeDef,
        /* [out] */ ICorDebugClass** ppClass);

     HRESULT STDMETHODCALLTYPE CreateBreakpoint(
        /* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint);

     HRESULT STDMETHODCALLTYPE GetEditAndContinueSnapshot(
        /* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot);

     HRESULT STDMETHODCALLTYPE GetMetaDataInterface(
        /* [in] */ REFIID riid,
        /* [out] */ IUnknown** ppObj);

     HRESULT STDMETHODCALLTYPE GetToken(
        /* [out] */ mdModule* pToken);
     HRESULT STDMETHODCALLTYPE IsDynamic(
        /* [out] */ BOOL* pDynamic);

     HRESULT STDMETHODCALLTYPE GetGlobalVariableValue(
        /* [in] */ mdFieldDef fieldDef,
        /* [out] */ ICorDebugValue** ppValue);

     HRESULT STDMETHODCALLTYPE GetSize(
        /* [out] */ ULONG32* pcBytes);

     HRESULT STDMETHODCALLTYPE IsInMemory(
        /* [out] */ BOOL* pInMemory);

};

class CordbAssembly :
	public ICorDebugAssembly, 
	public ICorDebugAssembly2
{
public:
	CordbProcess* pProcess;
	CordbAppDomain* pAppDomain;
	int id;
    CordbAssembly(CordbProcess* process, CordbAppDomain* appDomain, int id_assembly);
     HRESULT STDMETHODCALLTYPE IsFullyTrusted(
        /* [out] */ BOOL* pbFullyTrusted);
	 HRESULT STDMETHODCALLTYPE GetProcess(
		/* [out] */ ICorDebugProcess** ppProcess);

     HRESULT STDMETHODCALLTYPE GetAppDomain(
        /* [out] */ ICorDebugAppDomain** ppAppDomain);

     HRESULT STDMETHODCALLTYPE EnumerateModules(
        /* [out] */ ICorDebugModuleEnum** ppModules);

     HRESULT STDMETHODCALLTYPE GetCodeBase(
        /* [in] */ ULONG32 cchName,
        /* [out] */ ULONG32* pcchName,
        /* [length_is][size_is][out] */ WCHAR szName[]);
     HRESULT STDMETHODCALLTYPE GetName(
        /* [in] */ ULONG32 cchName,
        /* [out] */ ULONG32* pcchName,
        /* [length_is][size_is][out] */ WCHAR szName[]);
     HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface);

     ULONG STDMETHODCALLTYPE AddRef(void);

     ULONG STDMETHODCALLTYPE Release(void);

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
    int suspended;
	Cordb* cordb;
    CordbProcess()
    {
        suspended = false;
    }
     HRESULT STDMETHODCALLTYPE EnumerateLoaderHeapMemoryRegions(
        /* [out] */ ICorDebugMemoryRangeEnum** ppRanges);
     HRESULT STDMETHODCALLTYPE EnableGCNotificationEvents(
        BOOL fEnable);
     HRESULT STDMETHODCALLTYPE EnableExceptionCallbacksOutsideOfMyCode(
    /* [in] */ BOOL enableExceptionsOutsideOfJMC);
     HRESULT STDMETHODCALLTYPE SetWriteableMetadataUpdateMode(
        WriteableMetadataUpdateMode flags);
     HRESULT STDMETHODCALLTYPE GetGCHeapInformation(
        /* [out] */ COR_HEAPINFO* pHeapInfo);

     HRESULT STDMETHODCALLTYPE EnumerateHeap(
        /* [out] */ ICorDebugHeapEnum** ppObjects);
     HRESULT STDMETHODCALLTYPE EnumerateHeapRegions(
        /* [out] */ ICorDebugHeapSegmentEnum** ppRegions);
     HRESULT STDMETHODCALLTYPE GetObject(
        /* [in] */ CORDB_ADDRESS addr,
        /* [out] */ ICorDebugObjectValue** pObject);
     HRESULT STDMETHODCALLTYPE EnumerateGCReferences(
        /* [in] */ BOOL enumerateWeakReferences,
        /* [out] */ ICorDebugGCReferenceEnum** ppEnum);
     HRESULT STDMETHODCALLTYPE EnumerateHandles(
        /* [in] */ CorGCReferenceType types,
        /* [out] */ ICorDebugGCReferenceEnum** ppEnum);
     HRESULT STDMETHODCALLTYPE GetTypeID(
        /* [in] */ CORDB_ADDRESS obj,
        /* [out] */ COR_TYPEID* pId);
     HRESULT STDMETHODCALLTYPE GetTypeForTypeID(
        /* [in] */ COR_TYPEID id,
        /* [out] */ ICorDebugType** ppType);

     HRESULT STDMETHODCALLTYPE GetArrayLayout(
        /* [in] */ COR_TYPEID id,
        /* [out] */ COR_ARRAY_LAYOUT* pLayout);
     HRESULT STDMETHODCALLTYPE GetTypeLayout(
        /* [in] */ COR_TYPEID id,
        /* [out] */ COR_TYPE_LAYOUT* pLayout);
     HRESULT STDMETHODCALLTYPE GetTypeFields(
        /* [in] */ COR_TYPEID id,
        ULONG32 celt,
        COR_FIELD fields[],
        ULONG32* pceltNeeded);

     HRESULT STDMETHODCALLTYPE EnableNGENPolicy(
        /* [in] */ CorDebugNGENPolicy ePolicy);
     HRESULT STDMETHODCALLTYPE Filter(
        /* [size_is][length_is][in] */ const BYTE pRecord[],
        /* [in] */ DWORD countBytes,
        /* [in] */ CorDebugRecordFormat format,
        /* [in] */ DWORD dwFlags,
        /* [in] */ DWORD dwThreadId,
        /* [in] */ ICorDebugManagedCallback* pCallback,
        /* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus);

     HRESULT STDMETHODCALLTYPE ProcessStateChanged(
        /* [in] */ CorDebugStateChange eChange);
     HRESULT STDMETHODCALLTYPE SetEnableCustomNotification(
        ICorDebugClass* pClass,
        BOOL fEnable);
     HRESULT STDMETHODCALLTYPE GetID(
        /* [out] */ DWORD* pdwProcessId);
     HRESULT STDMETHODCALLTYPE GetHandle(
        /* [out] */ HPROCESS* phProcessHandle);
     HRESULT STDMETHODCALLTYPE GetThread(
        /* [in] */ DWORD dwThreadId,
        /* [out] */ ICorDebugThread** ppThread);
     HRESULT STDMETHODCALLTYPE EnumerateObjects(
        /* [out] */ ICorDebugObjectEnum** ppObjects);
     HRESULT STDMETHODCALLTYPE IsTransitionStub(
        /* [in] */ CORDB_ADDRESS address,
        /* [out] */ BOOL* pbTransitionStub);
     HRESULT STDMETHODCALLTYPE IsOSSuspended(
        /* [in] */ DWORD threadID,
        /* [out] */ BOOL* pbSuspended);
     HRESULT STDMETHODCALLTYPE GetThreadContext(
        /* [in] */ DWORD threadID,
        /* [in] */ ULONG32 contextSize,
        /* [size_is][length_is][out][in] */ BYTE context[]);
     HRESULT STDMETHODCALLTYPE SetThreadContext(
        /* [in] */ DWORD threadID,
        /* [in] */ ULONG32 contextSize,
        /* [size_is][length_is][in] */ BYTE context[]);
     HRESULT STDMETHODCALLTYPE ReadMemory(
        /* [in] */ CORDB_ADDRESS address,
        /* [in] */ DWORD size,
        /* [length_is][size_is][out] */ BYTE buffer[],
        /* [out] */ SIZE_T* read);
     HRESULT STDMETHODCALLTYPE WriteMemory(
        /* [in] */ CORDB_ADDRESS address,
        /* [in] */ DWORD size,
        /* [size_is][in] */ BYTE buffer[],
        /* [out] */ SIZE_T* written);
     HRESULT STDMETHODCALLTYPE ClearCurrentException(
        /* [in] */ DWORD threadID);
     HRESULT STDMETHODCALLTYPE EnableLogMessages(
        /* [in] */ BOOL fOnOff);
     HRESULT STDMETHODCALLTYPE ModifyLogSwitch(
        /* [annotation][in] */
        _In_  WCHAR* pLogSwitchName,
        /* [in] */ LONG lLevel);
     HRESULT STDMETHODCALLTYPE EnumerateAppDomains(
        /* [out] */ ICorDebugAppDomainEnum** ppAppDomains);
     HRESULT STDMETHODCALLTYPE GetObject(
        /* [out] */ ICorDebugValue** ppObject);
     HRESULT STDMETHODCALLTYPE ThreadForFiberCookie(
        /* [in] */ DWORD fiberCookie,
        /* [out] */ ICorDebugThread** ppThread);
     HRESULT STDMETHODCALLTYPE GetHelperThreadID(
        /* [out] */ DWORD* pThreadID);
     HRESULT STDMETHODCALLTYPE GetThreadForTaskID(
        /* [in] */ TASKID taskid,
        /* [out] */ ICorDebugThread2** ppThread);
     HRESULT STDMETHODCALLTYPE GetVersion(
        /* [out] */ COR_VERSION* version);
     HRESULT STDMETHODCALLTYPE SetUnmanagedBreakpoint(
        /* [in] */ CORDB_ADDRESS address,
        /* [in] */ ULONG32 bufsize,
        /* [length_is][size_is][out] */ BYTE buffer[],
        /* [out] */ ULONG32* bufLen);
     HRESULT STDMETHODCALLTYPE ClearUnmanagedBreakpoint(
        /* [in] */ CORDB_ADDRESS address);
     HRESULT STDMETHODCALLTYPE SetDesiredNGENCompilerFlags(
        /* [in] */ DWORD pdwFlags);
     HRESULT STDMETHODCALLTYPE GetDesiredNGENCompilerFlags(
        /* [out] */ DWORD* pdwFlags);
     HRESULT STDMETHODCALLTYPE GetReferenceValueFromGCHandle(
        /* [in] */ UINT_PTR handle,
        /* [out] */ ICorDebugReferenceValue** pOutValue);
     HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
     ULONG STDMETHODCALLTYPE AddRef(void);
     ULONG STDMETHODCALLTYPE Release(void);
     HRESULT STDMETHODCALLTYPE Stop(
        /* [in] */ DWORD dwTimeoutIgnored);
     HRESULT STDMETHODCALLTYPE Continue(
        /* [in] */ BOOL fIsOutOfBand);
     HRESULT STDMETHODCALLTYPE IsRunning(
        /* [out] */ BOOL* pbRunning);
     HRESULT STDMETHODCALLTYPE HasQueuedCallbacks(
        /* [in] */ ICorDebugThread* pThread,
        /* [out] */ BOOL* pbQueued);

     HRESULT STDMETHODCALLTYPE EnumerateThreads(
        /* [out] */ ICorDebugThreadEnum** ppThreads);

     HRESULT STDMETHODCALLTYPE SetAllThreadsDebugState(
        /* [in] */ CorDebugThreadState state,
        /* [in] */ ICorDebugThread* pExceptThisThread);
     HRESULT STDMETHODCALLTYPE Detach(void);

     HRESULT STDMETHODCALLTYPE Terminate(
        /* [in] */ UINT exitCode);
     HRESULT STDMETHODCALLTYPE CanCommitChanges(
        /* [in] */ ULONG cSnapshots,
        /* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
        /* [out] */ ICorDebugErrorInfoEnum** pError);
     HRESULT STDMETHODCALLTYPE CommitChanges(
        /* [in] */ ULONG cSnapshots,
        /* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
        /* [out] */ ICorDebugErrorInfoEnum** pError);
};

class CordbFunction :
    public ICorDebugFunction,
    public ICorDebugFunction2,
    public ICorDebugFunction3,
    public ICorDebugFunction4
{
public:
    int id;
    mdToken token;
    CordbCode* code;
    CordbModule* module;

    CordbFunction(mdToken token, int id, CordbModule *module);
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
    HRESULT STDMETHODCALLTYPE GetModule(
        /* [out] */ ICorDebugModule** ppModule);

    HRESULT STDMETHODCALLTYPE GetClass(
        /* [out] */ ICorDebugClass** ppClass);

    HRESULT STDMETHODCALLTYPE GetToken(
        /* [out] */ mdMethodDef* pMethodDef);

    HRESULT STDMETHODCALLTYPE GetILCode(
        /* [out] */ ICorDebugCode** ppCode);

    HRESULT STDMETHODCALLTYPE GetNativeCode(
        /* [out] */ ICorDebugCode** ppCode);

    HRESULT STDMETHODCALLTYPE CreateBreakpoint(
        /* [out] */ ICorDebugFunctionBreakpoint** ppBreakpoint);

    HRESULT STDMETHODCALLTYPE GetLocalVarSigToken(
        /* [out] */ mdSignature* pmdSig);

    HRESULT STDMETHODCALLTYPE GetCurrentVersionNumber(
        /* [out] */ ULONG32* pnCurrentVersion);
    HRESULT STDMETHODCALLTYPE SetJMCStatus(
        /* [in] */ BOOL bIsJustMyCode);

    HRESULT STDMETHODCALLTYPE GetJMCStatus(
        /* [out] */ BOOL* pbIsJustMyCode);

    HRESULT STDMETHODCALLTYPE EnumerateNativeCode(
        /* [out] */ ICorDebugCodeEnum** ppCodeEnum);

    HRESULT STDMETHODCALLTYPE GetVersionNumber(
        /* [out] */ ULONG32* pnVersion);
    HRESULT STDMETHODCALLTYPE GetActiveReJitRequestILCode(
        ICorDebugILCode** ppReJitedILCode);
    HRESULT STDMETHODCALLTYPE CreateNativeBreakpoint(
        ICorDebugFunctionBreakpoint** ppBreakpoint);
};

class CordbCode :
    public ICorDebugCode
{
public:
    CordbFunction* func;
    CordbCode(CordbFunction* func);
    HRESULT STDMETHODCALLTYPE IsIL(
        /* [out] */ BOOL* pbIL);

    HRESULT STDMETHODCALLTYPE GetFunction(
        /* [out] */ ICorDebugFunction** ppFunction);

    HRESULT STDMETHODCALLTYPE GetAddress(
        /* [out] */ CORDB_ADDRESS* pStart);

    HRESULT STDMETHODCALLTYPE GetSize(
        /* [out] */ ULONG32* pcBytes);

    HRESULT STDMETHODCALLTYPE CreateBreakpoint(
        /* [in] */ ULONG32 offset,
        /* [out] */ ICorDebugFunctionBreakpoint** ppBreakpoint);

    HRESULT STDMETHODCALLTYPE GetCode(
        /* [in] */ ULONG32 startOffset,
        /* [in] */ ULONG32 endOffset,
        /* [in] */ ULONG32 cBufferAlloc,
        /* [length_is][size_is][out] */ BYTE buffer[],
        /* [out] */ ULONG32* pcBufferSize);

    HRESULT STDMETHODCALLTYPE GetVersionNumber(
        /* [out] */ ULONG32* nVersion);

    HRESULT STDMETHODCALLTYPE GetILToNativeMapping(
        /* [in] */ ULONG32 cMap,
        /* [out] */ ULONG32* pcMap,
        /* [length_is][size_is][out] */ COR_DEBUG_IL_TO_NATIVE_MAP map[]);

    HRESULT STDMETHODCALLTYPE GetEnCRemapSequencePoints(
        /* [in] */ ULONG32 cMap,
        /* [out] */ ULONG32* pcMap,
        /* [length_is][size_is][out] */ ULONG32 offsets[]);

    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

class CordbFunctionBreakpoint :
    public ICorDebugFunctionBreakpoint
{
public:
    CordbCode* code;
    ULONG32 offset;
    CordbFunctionBreakpoint(CordbCode* code, ULONG32 offset);
    HRESULT STDMETHODCALLTYPE GetFunction(
        /* [out] */ ICorDebugFunction** ppFunction);

    HRESULT STDMETHODCALLTYPE GetOffset(
        /* [out] */ ULONG32* pnOffset);

    HRESULT STDMETHODCALLTYPE Activate(
        /* [in] */ BOOL bActive);

    HRESULT STDMETHODCALLTYPE IsActive(
        /* [out] */ BOOL* pbActive);

    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

class CordbThread :
    public ICorDebugThread,
    public ICorDebugThread2,
    public ICorDebugThread3,
    public ICorDebugThread4
{
public:
    long thread_id;
    CordbProcess* ppProcess;
    CordbStepper* stepper;
    CordbRegisteSet* registerset;
    CordbThread(CordbProcess *ppProcess, long thread_id);
    HRESULT STDMETHODCALLTYPE HasUnhandledException(void);

    HRESULT STDMETHODCALLTYPE GetBlockingObjects(
        /* [out] */ ICorDebugBlockingObjectEnum** ppBlockingObjectEnum);

    HRESULT STDMETHODCALLTYPE GetCurrentCustomDebuggerNotification(
        /* [out] */ ICorDebugValue** ppNotificationObject);

    HRESULT STDMETHODCALLTYPE CreateStackWalk(
        /* [out] */ ICorDebugStackWalk** ppStackWalk);

    HRESULT STDMETHODCALLTYPE GetActiveInternalFrames(
        /* [in] */ ULONG32 cInternalFrames,
        /* [out] */ ULONG32* pcInternalFrames,
        /* [length_is][size_is][out][in] */ ICorDebugInternalFrame2* ppInternalFrames[]);

    HRESULT STDMETHODCALLTYPE GetActiveFunctions(
        /* [in] */ ULONG32 cFunctions,
        /* [out] */ ULONG32* pcFunctions,
        /* [length_is][size_is][out][in] */ COR_ACTIVE_FUNCTION pFunctions[]);

    HRESULT STDMETHODCALLTYPE GetConnectionID(
        /* [out] */ CONNID* pdwConnectionId);

    HRESULT STDMETHODCALLTYPE GetTaskID(
        /* [out] */ TASKID* pTaskId);

    HRESULT STDMETHODCALLTYPE GetVolatileOSThreadID(
        /* [out] */ DWORD* pdwTid);

    HRESULT STDMETHODCALLTYPE InterceptCurrentException(

        /* [in] */ ICorDebugFrame* pFrame);
    HRESULT STDMETHODCALLTYPE GetProcess(
        /* [out] */ ICorDebugProcess** ppProcess);

    HRESULT STDMETHODCALLTYPE GetID(
        /* [out] */ DWORD* pdwThreadId);

    HRESULT STDMETHODCALLTYPE GetHandle(
        /* [out] */ HTHREAD* phThreadHandle);

    HRESULT STDMETHODCALLTYPE GetAppDomain(
        /* [out] */ ICorDebugAppDomain** ppAppDomain);

    HRESULT STDMETHODCALLTYPE SetDebugState(
        /* [in] */ CorDebugThreadState state);

    HRESULT STDMETHODCALLTYPE GetDebugState(
        /* [out] */ CorDebugThreadState* pState);

    HRESULT STDMETHODCALLTYPE GetUserState(
        /* [out] */ CorDebugUserState* pState);

    HRESULT STDMETHODCALLTYPE GetCurrentException(
        /* [out] */ ICorDebugValue** ppExceptionObject);

    HRESULT STDMETHODCALLTYPE ClearCurrentException(void);

    HRESULT STDMETHODCALLTYPE CreateStepper(
        /* [out] */ ICorDebugStepper** ppStepper);

    HRESULT STDMETHODCALLTYPE EnumerateChains(
        /* [out] */ ICorDebugChainEnum** ppChains);

    HRESULT STDMETHODCALLTYPE GetActiveChain(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE GetActiveFrame(
        /* [out] */ ICorDebugFrame** ppFrame);

    HRESULT STDMETHODCALLTYPE GetRegisterSet(
        /* [out] */ ICorDebugRegisterSet** ppRegisters);

    HRESULT STDMETHODCALLTYPE CreateEval(
        /* [out] */ ICorDebugEval** ppEval);

    HRESULT STDMETHODCALLTYPE GetObject(
        /* [out] */ ICorDebugValue** ppObject);
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

};

class CordbNativeFrame :
    public ICorDebugNativeFrame,
    public ICorDebugNativeFrame2
{
    CordbJITILFrame* m_JITILFrame;
public:
    CordbThread* thread;
    CordbNativeFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread);

    virtual HRESULT STDMETHODCALLTYPE GetIP(ULONG32* pnOffset);


    virtual HRESULT STDMETHODCALLTYPE SetIP(ULONG32 nOffset);


    virtual HRESULT STDMETHODCALLTYPE GetRegisterSet(ICorDebugRegisterSet** ppRegisters);


    virtual HRESULT STDMETHODCALLTYPE GetLocalRegisterValue(CorDebugRegister reg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetLocalDoubleRegisterValue(CorDebugRegister highWordReg, CorDebugRegister lowWordReg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetLocalMemoryValue(CORDB_ADDRESS address, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetLocalRegisterMemoryValue(CorDebugRegister highWordReg, CORDB_ADDRESS lowWordAddress, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetLocalMemoryRegisterValue(CORDB_ADDRESS highWordAddress, CorDebugRegister lowWordRegister, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE CanSetIP(ULONG32 nOffset);


    virtual HRESULT STDMETHODCALLTYPE GetChain(ICorDebugChain** ppChain);


    virtual HRESULT STDMETHODCALLTYPE GetCode(ICorDebugCode** ppCode);


    virtual HRESULT STDMETHODCALLTYPE GetFunction(ICorDebugFunction** ppFunction);


    virtual HRESULT STDMETHODCALLTYPE GetFunctionToken(mdMethodDef* pToken);


    virtual HRESULT STDMETHODCALLTYPE GetStackRange(CORDB_ADDRESS* pStart, CORDB_ADDRESS* pEnd);


    virtual HRESULT STDMETHODCALLTYPE GetCaller(ICorDebugFrame** ppFrame);


    virtual HRESULT STDMETHODCALLTYPE GetCallee(ICorDebugFrame** ppFrame);


    virtual HRESULT STDMETHODCALLTYPE CreateStepper(ICorDebugStepper** ppStepper);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);


    virtual HRESULT STDMETHODCALLTYPE IsChild(BOOL* pIsChild);


    virtual HRESULT STDMETHODCALLTYPE IsMatchingParentFrame(ICorDebugNativeFrame2* pPotentialParentFrame, BOOL* pIsParent);


    virtual HRESULT STDMETHODCALLTYPE GetStackParameterSize(ULONG32* pSize);

};

class CordbJITILFrame :
    public ICorDebugILFrame,
    public ICorDebugILFrame2,
    public ICorDebugILFrame3,
    public ICorDebugILFrame4
{
public:
    int frameid;
    int methoid;
    int il_offset;
    int flags;
    CordbThread* thread;
    CordbJITILFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread);
    HRESULT STDMETHODCALLTYPE GetChain(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE GetCode(
        /* [out] */ ICorDebugCode** ppCode);

    HRESULT STDMETHODCALLTYPE GetFunction(
        /* [out] */ ICorDebugFunction** ppFunction);

    HRESULT STDMETHODCALLTYPE GetFunctionToken(
        /* [out] */ mdMethodDef* pToken);

    HRESULT STDMETHODCALLTYPE GetStackRange(
        /* [out] */ CORDB_ADDRESS* pStart,
        /* [out] */ CORDB_ADDRESS* pEnd);

    HRESULT STDMETHODCALLTYPE GetCaller(
        /* [out] */ ICorDebugFrame** ppFrame);

    HRESULT STDMETHODCALLTYPE GetCallee(
        /* [out] */ ICorDebugFrame** ppFrame);

    HRESULT STDMETHODCALLTYPE CreateStepper(
        /* [out] */ ICorDebugStepper** ppStepper);
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    HRESULT STDMETHODCALLTYPE GetIP(
        /* [out] */ ULONG32* pnOffset,
        /* [out] */ CorDebugMappingResult* pMappingResult);

    HRESULT STDMETHODCALLTYPE SetIP(
        /* [in] */ ULONG32 nOffset);

    HRESULT STDMETHODCALLTYPE EnumerateLocalVariables(
        /* [out] */ ICorDebugValueEnum** ppValueEnum);

    HRESULT STDMETHODCALLTYPE GetLocalVariable(
        /* [in] */ DWORD dwIndex,
        /* [out] */ ICorDebugValue** ppValue);

    HRESULT STDMETHODCALLTYPE EnumerateArguments(
        /* [out] */ ICorDebugValueEnum** ppValueEnum);

    HRESULT STDMETHODCALLTYPE GetArgument(
        /* [in] */ DWORD dwIndex,
        /* [out] */ ICorDebugValue** ppValue);

    HRESULT STDMETHODCALLTYPE GetStackDepth(
        /* [out] */ ULONG32* pDepth);

    HRESULT STDMETHODCALLTYPE GetStackValue(
        /* [in] */ DWORD dwIndex,
        /* [out] */ ICorDebugValue** ppValue);

    HRESULT STDMETHODCALLTYPE CanSetIP(
        /* [in] */ ULONG32 nOffset);


    virtual HRESULT STDMETHODCALLTYPE RemapFunction(ULONG32 newILOffset);


    virtual HRESULT STDMETHODCALLTYPE EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum);


    virtual HRESULT STDMETHODCALLTYPE GetReturnValueForILOffset(ULONG32 ILoffset, ICorDebugValue** ppReturnValue);


    virtual HRESULT STDMETHODCALLTYPE EnumerateLocalVariablesEx(ILCodeKind flags, ICorDebugValueEnum** ppValueEnum);


    virtual HRESULT STDMETHODCALLTYPE GetLocalVariableEx(ILCodeKind flags, DWORD dwIndex, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetCodeEx(ILCodeKind flags, ICorDebugCode** ppCode);

};

class CordbRegisteSet:
    public ICorDebugRegisterSet {

    guint8* ctx;
    guint32 ctx_len;
public:
    CordbRegisteSet(guint8* ctx, guint32 ctx_len);
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    HRESULT STDMETHODCALLTYPE GetRegistersAvailable(
        /* [out] */ ULONG64* pAvailable);

    HRESULT STDMETHODCALLTYPE GetRegisters(
        /* [in] */ ULONG64 mask,
        /* [in] */ ULONG32 regCount,
        /* [length_is][size_is][out] */ CORDB_REGISTER regBuffer[]);

    HRESULT STDMETHODCALLTYPE SetRegisters(
        /* [in] */ ULONG64 mask,
        /* [in] */ ULONG32 regCount,
        /* [size_is][in] */ CORDB_REGISTER regBuffer[]);

    HRESULT STDMETHODCALLTYPE GetThreadContext(
        /* [in] */ ULONG32 contextSize,
        /* [size_is][length_is][out][in] */ BYTE context[]);

    HRESULT STDMETHODCALLTYPE SetThreadContext(
        /* [in] */ ULONG32 contextSize,
        /* [size_is][length_is][in] */ BYTE context[]);
};

class CordbChainEnum: 
public ICorDebugChainEnum
{
public:
    CordbThread* thread;
    CordbChainEnum(CordbThread* thread);
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);

    HRESULT STDMETHODCALLTYPE Skip(
        /* [in] */ ULONG celt);

    HRESULT STDMETHODCALLTYPE Reset(void);

    HRESULT STDMETHODCALLTYPE Clone(
        /* [out] */ ICorDebugEnum** ppEnum);

    HRESULT STDMETHODCALLTYPE GetCount(
        /* [out] */ ULONG* pcelt);

    HRESULT STDMETHODCALLTYPE Next(
        /* [in] */ ULONG celt,
        /* [length_is][size_is][out] */ ICorDebugChain* chains[],
        /* [out] */ ULONG* pceltFetched);
};

class CordbChain :
    public ICorDebugChain
{
public:
    CordbThread* thread;
    CorDebugChainReason chain_reason;
    bool is_managed;
    CordbChain(CordbThread* thread, CorDebugChainReason chain_reason, bool is_managed);

    HRESULT STDMETHODCALLTYPE GetThread(
        /* [out] */ ICorDebugThread** ppThread);

    HRESULT STDMETHODCALLTYPE GetStackRange(
        /* [out] */ CORDB_ADDRESS* pStart,
        /* [out] */ CORDB_ADDRESS* pEnd);

    HRESULT STDMETHODCALLTYPE GetContext(
        /* [out] */ ICorDebugContext** ppContext);

    HRESULT STDMETHODCALLTYPE GetCaller(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE GetCallee(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE GetPrevious(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE GetNext(
        /* [out] */ ICorDebugChain** ppChain);

    HRESULT STDMETHODCALLTYPE IsManaged(
        /* [out] */ BOOL* pManaged);

    HRESULT STDMETHODCALLTYPE EnumerateFrames(
        /* [out] */ ICorDebugFrameEnum** ppFrames);

    HRESULT STDMETHODCALLTYPE GetActiveFrame(
        /* [out] */ ICorDebugFrame** ppFrame);

    HRESULT STDMETHODCALLTYPE GetRegisterSet(
        /* [out] */ ICorDebugRegisterSet** ppRegisters);

    HRESULT STDMETHODCALLTYPE GetReason(
        /* [out] */ CorDebugChainReason* pReason);
    
    HRESULT STDMETHODCALLTYPE QueryInterface(
        /* [in] */ REFIID id,
        /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface);
    ULONG STDMETHODCALLTYPE AddRef(void);
    ULONG STDMETHODCALLTYPE Release(void);
};

class CordbFrameEnum :
    public ICorDebugFrameEnum
{
public:
    CordbThread* thread;
    int nframes;
    CordbNativeFrame** frames;
    CordbFrameEnum(CordbThread* thread);
    virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, ICorDebugFrame* frames[], ULONG* pceltFetched);
    virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);
    virtual HRESULT STDMETHODCALLTYPE Reset(void);
    virtual HRESULT STDMETHODCALLTYPE Clone(ICorDebugEnum** ppEnum);
    virtual HRESULT STDMETHODCALLTYPE GetCount(ULONG* pcelt);
    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);
    virtual ULONG STDMETHODCALLTYPE AddRef(void);
    virtual ULONG STDMETHODCALLTYPE Release(void);
};

class CordbBlockingObjectEnum :
    public ICorDebugBlockingObjectEnum
{

public:
    virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, CorDebugBlockingObject values[], ULONG* pceltFetched);


    virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);


    virtual HRESULT STDMETHODCALLTYPE Reset(void);


    virtual HRESULT STDMETHODCALLTYPE Clone(ICorDebugEnum** ppEnum);


    virtual HRESULT STDMETHODCALLTYPE GetCount(ULONG* pcelt);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);

};

class CordbStepper :
    public ICorDebugStepper,
    public ICorDebugStepper2
{
    CordbThread* thread;
    boolean hasStepped;
public:
    CordbStepper(CordbThread* thread);
    virtual HRESULT STDMETHODCALLTYPE IsActive(BOOL* pbActive);


    virtual HRESULT STDMETHODCALLTYPE Deactivate(void);


    virtual HRESULT STDMETHODCALLTYPE SetInterceptMask(CorDebugIntercept mask);


    virtual HRESULT STDMETHODCALLTYPE SetUnmappedStopMask(CorDebugUnmappedStop mask);


    virtual HRESULT STDMETHODCALLTYPE Step(BOOL bStepIn);


    virtual HRESULT STDMETHODCALLTYPE StepRange(BOOL bStepIn, COR_DEBUG_STEP_RANGE ranges[], ULONG32 cRangeCount);


    virtual HRESULT STDMETHODCALLTYPE StepOut(void);


    virtual HRESULT STDMETHODCALLTYPE SetRangeIL(BOOL bIL);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);


    virtual HRESULT STDMETHODCALLTYPE SetJMC(BOOL fIsJMCStepper);

};

class CordbClass :
    public ICorDebugClass,
    public ICorDebugClass2
{
    mdToken token;
public:
    CordbClass(mdToken token);
    virtual HRESULT STDMETHODCALLTYPE GetModule(ICorDebugModule** pModule);


    virtual HRESULT STDMETHODCALLTYPE GetToken(mdTypeDef* pTypeDef);


    virtual HRESULT STDMETHODCALLTYPE GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);


    virtual HRESULT STDMETHODCALLTYPE GetParameterizedType(CorElementType elementType, ULONG32 nTypeArgs, ICorDebugType* ppTypeArgs[], ICorDebugType** ppType);


    virtual HRESULT STDMETHODCALLTYPE SetJMCStatus(BOOL bIsJustMyCode);

};


class CordbValue :
    public ICorDebugValue,
    public ICorDebugValue2, 
    public ICorDebugValue3,
    public ICorDebugGenericValue
{
    CorElementType type;
    int value;
    int size;
public:
    CordbValue(CorElementType type, int value, int size);
    virtual HRESULT STDMETHODCALLTYPE GetType(CorElementType* pType);


    virtual HRESULT STDMETHODCALLTYPE GetSize(ULONG32* pSize);


    virtual HRESULT STDMETHODCALLTYPE GetAddress(CORDB_ADDRESS* pAddress);


    virtual HRESULT STDMETHODCALLTYPE CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);


    virtual HRESULT STDMETHODCALLTYPE GetExactType(ICorDebugType** ppType);


    virtual HRESULT STDMETHODCALLTYPE GetSize64(ULONG64* pSize);


    virtual HRESULT STDMETHODCALLTYPE GetValue(void* pTo);


    virtual HRESULT STDMETHODCALLTYPE SetValue(void* pFrom);

};

class CordbType:
    public ICorDebugType,
    public ICorDebugType2
{
    CorElementType type;
public:
    CordbType(CorElementType type);
    virtual HRESULT STDMETHODCALLTYPE GetType(CorElementType* ty);


    virtual HRESULT STDMETHODCALLTYPE GetClass(ICorDebugClass** ppClass);


    virtual HRESULT STDMETHODCALLTYPE EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum);


    virtual HRESULT STDMETHODCALLTYPE GetFirstTypeParameter(ICorDebugType** value);


    virtual HRESULT STDMETHODCALLTYPE GetBase(ICorDebugType** pBase);


    virtual HRESULT STDMETHODCALLTYPE GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue);


    virtual HRESULT STDMETHODCALLTYPE GetRank(ULONG32* pnRank);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);


    virtual HRESULT STDMETHODCALLTYPE GetTypeID(COR_TYPEID* id);

};

class CordbTypeEnum :
    public ICorDebugTypeEnum
{

public:
    virtual HRESULT STDMETHODCALLTYPE Next(ULONG celt, ICorDebugType* values[], ULONG* pceltFetched);


    virtual HRESULT STDMETHODCALLTYPE Skip(ULONG celt);


    virtual HRESULT STDMETHODCALLTYPE Reset(void);


    virtual HRESULT STDMETHODCALLTYPE Clone(ICorDebugEnum** ppEnum);


    virtual HRESULT STDMETHODCALLTYPE GetCount(ULONG* pcelt);


    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** ppvObject);


    virtual ULONG STDMETHODCALLTYPE AddRef(void);


    virtual ULONG STDMETHODCALLTYPE Release(void);

};
#endif
