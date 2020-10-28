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

#define return_if_nok(error) do { if (!is_ok ((error))) return S_FALSE; } while (0)

/*1   0x00007DA0    CoreCLRCreateCordbObject
2   0x00007CA0    CoreCLRCreateCordbObjectEx
3   0x00007C60    CreateCordbObject
4   0x00007DC0    DllGetClassObjectInternal
5   0x0000D0C0    OpenVirtualProcess
6   0x0000D070    OpenVirtualProcess2
7   0x0000CE30    OpenVirtualProcessImpl
8   0x0000CFF0    OpenVirtualProcessImpl2*/

class Cordb;
class CordbProcess;
class CordbAppDomain;
class CordbAssembly;

class Connection
{
	SOCKET connect_socket;
	CordbProcess* ppProcess;
	CordbAppDomain*pCorDebugAppDomain;
	Cordb* ppCordb;
public:
	Connection(CordbProcess* proc, Cordb* cordb);
	void loop_send_receive();
	void enable_event(EventKind eventKind);
	void close_connection();
	void start_connection();
	void transport_handshake();
	void send_packet(Buffer& sendbuf);
	void receive_packet(Buffer& b);
	void send_event(int cmd_set, int cmd, Buffer* sendbuf);
};

static Connection* connection;



class CordbSymbol :
	public IMetaDataImport
{
public:
	// IUnknown methods
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppvObj)
	{
		if (ppvObj == NULL)
			return E_POINTER;

		/*if (riid == IID_IMetaDataEmit)
			*ppvObj = static_cast<IMetaDataEmit*>(this);
		else*/
			if (riid == IID_IMetaDataImport)
				*ppvObj = static_cast<IMetaDataImport*>(this);
			else
				if (riid == IID_IUnknown)
					*ppvObj = static_cast<IMetaDataImport*>(this);
				else
				{
					printf("CordbSymbol - QueryInterface - E_NOTIMPL - %d - %d\n", riid, IID_IMetaDataEmit);
					return E_NOTIMPL;
				}

		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef()
	{
		printf("CordbSymbol - AddRef\n");
		fflush(stdout);
		return S_OK;
	}
	virtual ULONG STDMETHODCALLTYPE Release()
	{
		printf("CordbSymbol - Release\n");
		fflush(stdout);
		return S_OK;
	}


	// IMetaDataImport functions

	void STDMETHODCALLTYPE CloseEnum(HCORENUM hEnum)
	{
		printf("CordbSymbol - CloseEnum\n");
		fflush(stdout);
		return;
	}
	virtual HRESULT STDMETHODCALLTYPE CountEnum(HCORENUM hEnum, ULONG* pulCount)
	{
		printf("CordbSymbol - CountEnum\n");
		fflush(stdout);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE ResetEnum(HCORENUM hEnum, ULONG ulPos)
	{
		printf("CordbSymbol - ResetEnum\n");
		fflush(stdout);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE EnumTypeDefs(HCORENUM* phEnum, mdTypeDef rTypeDefs[],
		ULONG cMax, ULONG* pcTypeDefs)
	{
		printf("CordbSymbol - EnumTypeDefs\n");
		fflush(stdout);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE EnumInterfaceImpls(HCORENUM* phEnum, mdTypeDef td,
		mdInterfaceImpl rImpls[], ULONG cMax,
		ULONG* pcImpls)
	{
		printf("CordbSymbol - EnumInterfaceImpls\n");
		fflush(stdout);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE EnumTypeRefs(HCORENUM* phEnum, mdTypeRef rTypeRefs[],
		ULONG cMax, ULONG* pcTypeRefs)
	{
		printf("CordbSymbol - EnumTypeRefs\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindTypeDefByName(           // S_OK or error.
		LPCWSTR     szTypeDef,              // [IN] Name of the Type.
		mdToken     tkEnclosingClass,       // [IN] TypeDef/TypeRef for Enclosing class.
		mdTypeDef* ptd)             // [OUT] Put the TypeDef token here.
	{
		printf("CordbSymbol - FindTypeDefByName\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetScopeProps(               // S_OK or error.
		__out_ecount_part_opt(cchName, *pchName)
		LPWSTR      szName,                 // [OUT] Put the name here.
		ULONG       cchName,                // [IN] Size of name buffer in wide chars.
		ULONG* pchName,               // [OUT] Put size of name (wide chars) here.
		GUID* pmvid)           // [OUT, OPTIONAL] Put MVID here.
	{
		printf("CordbSymbol - GetScopeProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetModuleFromScope(          // S_OK.
		mdModule* pmd)            // [OUT] Put mdModule token here.
	{
		printf("CordbSymbol - GetModuleFromScope\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeDefProps(             // S_OK or error.
		mdTypeDef   td,                     // [IN] TypeDef token for inquiry.
		__out_ecount_part_opt(cchTypeDef, *pchTypeDef)
		LPWSTR      szTypeDef,              // [OUT] Put name here.
		ULONG       cchTypeDef,             // [IN] size of name buffer in wide chars.
		ULONG* pchTypeDef,            // [OUT] put size of name (wide chars) here.
		DWORD* pdwTypeDefFlags,       // [OUT] Put flags here.
		mdToken* ptkExtends)     // [OUT] Put base class TypeDef/TypeRef here.

	{
		printf("CordbSymbol - GetTypeDefProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetInterfaceImplProps(       // S_OK or error.
		mdInterfaceImpl iiImpl,             // [IN] InterfaceImpl token.
		mdTypeDef* pClass,                // [OUT] Put implementing class token here.
		mdToken* ptkIface)        // [OUT] Put implemented interface token here.

	{
		printf("CordbSymbol - GetInterfaceImplProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeRefProps(             // S_OK or error.
		mdTypeRef   tr,                     // [IN] TypeRef token.
		mdToken* ptkResolutionScope,    // [OUT] Resolution scope, ModuleRef or AssemblyRef.
		__out_ecount_part_opt(cchName, *pchName)
		LPWSTR      szName,                 // [OUT] Name of the TypeRef.
		ULONG       cchName,                // [IN] Size of buffer.
		ULONG* pchName)         // [OUT] Size of Name.

	{
		printf("CordbSymbol - GetTypeRefProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown** ppIScope, mdTypeDef* ptd)
	{
		printf("CordbSymbol - ResolveTypeRef\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMembers(                 // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		mdToken     rMembers[],             // [OUT] Put MemberDefs here.
		ULONG       cMax,                   // [IN] Max MemberDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMembers\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMembersWithName(         // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		LPCWSTR     szName,                 // [IN] Limit results to those with this name.
		mdToken     rMembers[],             // [OUT] Put MemberDefs here.
		ULONG       cMax,                   // [IN] Max MemberDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMembersWithName\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMethods(                 // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		mdMethodDef rMethods[],             // [OUT] Put MethodDefs here.
		ULONG       cMax,                   // [IN] Max MethodDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMethods\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMethodsWithName(         // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		LPCWSTR     szName,                 // [IN] Limit results to those with this name.
		mdMethodDef rMethods[],             // [OU] Put MethodDefs here.
		ULONG       cMax,                   // [IN] Max MethodDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMethodsWithName\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumFields(                  // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		mdFieldDef  rFields[],              // [OUT] Put FieldDefs here.
		ULONG       cMax,                   // [IN] Max FieldDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumFields\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumFieldsWithName(          // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
		LPCWSTR     szName,                 // [IN] Limit results to those with this name.
		mdFieldDef  rFields[],              // [OUT] Put MemberDefs here.
		ULONG       cMax,                   // [IN] Max MemberDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumFieldsWithName\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE EnumParams(                  // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
		mdParamDef  rParams[],              // [OUT] Put ParamDefs here.
		ULONG       cMax,                   // [IN] Max ParamDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumParams\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMemberRefs(              // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdToken     tkParent,               // [IN] Parent token to scope the enumeration.
		mdMemberRef rMemberRefs[],          // [OUT] Put MemberRefs here.
		ULONG       cMax,                   // [IN] Max MemberRefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMemberRefs\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMethodImpls(             // S_OK, S_FALSE, or error
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
		mdToken     rMethodBody[],          // [OUT] Put Method Body tokens here.
		mdToken     rMethodDecl[],          // [OUT] Put Method Declaration tokens here.
		ULONG       cMax,                   // [IN] Max tokens to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMethodImpls\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumPermissionSets(          // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdToken     tk,                     // [IN] if !NIL, token to scope the enumeration.
		DWORD       dwActions,              // [IN] if !0, return only these actions.
		mdPermission rPermission[],         // [OUT] Put Permissions here.
		ULONG       cMax,                   // [IN] Max Permissions to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumPermissionSets\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindMember(
		mdTypeDef   td,                     // [IN] given typedef
		LPCWSTR     szName,                 // [IN] member name
		PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
		ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
		mdToken* pmb)             // [OUT] matching memberdef

	{
		printf("CordbSymbol - FindMember\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindMethod(
		mdTypeDef   td,                     // [IN] given typedef
		LPCWSTR     szName,                 // [IN] member name
		PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
		ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
		mdMethodDef* pmb)             // [OUT] matching memberdef

	{
		printf("CordbSymbol - FindMethod\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindField(
		mdTypeDef   td,                     // [IN] given typedef
		LPCWSTR     szName,                 // [IN] member name
		PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
		ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
		mdFieldDef* pmb)             // [OUT] matching memberdef

	{
		printf("CordbSymbol - FindField\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindMemberRef(
		mdTypeRef   td,                     // [IN] given typeRef
		LPCWSTR     szName,                 // [IN] member name
		PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
		ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
		mdMemberRef* pmr)             // [OUT] matching memberref

	{
		printf("CordbSymbol - FindMemberRef\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetMethodProps(
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
		DWORD* pdwImplFlags)    // [OUT] Impl. Flags

	{
		printf("CordbSymbol - GetMethodProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetMemberRefProps(           // S_OK or error.
		mdMemberRef mr,                     // [IN] given memberref
		mdToken* ptk,                   // [OUT] Put classref or classdef here.
		__out_ecount_part_opt(cchMember, *pchMember)
		LPWSTR      szMember,               // [OUT] buffer to fill for member's name
		ULONG       cchMember,              // [IN] the count of char of szMember
		ULONG* pchMember,             // [OUT] actual count of char in member name
		PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to meta data blob value
		ULONG* pbSig)           // [OUT] actual size of signature blob

	{
		printf("CordbSymbol - GetMemberRefProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumProperties(              // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
		mdProperty  rProperties[],          // [OUT] Put Properties here.
		ULONG       cMax,                   // [IN] Max properties to put.
		ULONG* pcProperties)    // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumProperties\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumEvents(                  // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
		mdEvent     rEvents[],              // [OUT] Put events here.
		ULONG       cMax,                   // [IN] Max events to put.
		ULONG* pcEvents)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumEvents\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetEventProps(               // S_OK, S_FALSE, or error.
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
		ULONG* pcOtherMethod)   // [OUT] total number of other method of this event

	{
		printf("CordbSymbol - GetEventProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumMethodSemantics(         // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
		mdToken     rEventProp[],           // [OUT] Put Event/Property here.
		ULONG       cMax,                   // [IN] Max properties to put.
		ULONG* pcEventProp)     // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumMethodSemantics\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetMethodSemantics(          // S_OK, S_FALSE, or error.
		mdMethodDef mb,                     // [IN] method token
		mdToken     tkEventProp,            // [IN] event/property token.
		DWORD* pdwSemanticsFlags) // [OUT] the role flags for the method/propevent pair

	{
		printf("CordbSymbol - GetMethodSemantics\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetClassLayout(
		mdTypeDef   td,                     // [IN] give typedef
		DWORD* pdwPackSize,           // [OUT] 1, 2, 4, 8, or 16
		COR_FIELD_OFFSET rFieldOffset[],    // [OUT] field offset array
		ULONG       cMax,                   // [IN] size of the array
		ULONG* pcFieldOffset,         // [OUT] needed array size
		ULONG* pulClassSize)        // [OUT] the size of the class

	{
		printf("CordbSymbol - GetClassLayout\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFieldMarshal(
		mdToken     tk,                     // [IN] given a field's memberdef
		PCCOR_SIGNATURE* ppvNativeType,     // [OUT] native type of this field
		ULONG* pcbNativeType)   // [OUT] the count of bytes of *ppvNativeType

	{
		printf("CordbSymbol - GetFieldMarshal\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetRVA(                      // S_OK or error.
		mdToken     tk,                     // Member for which to set offset
		ULONG* pulCodeRVA,            // The offset
		DWORD* pdwImplFlags)    // the implementation flags

	{
		printf("CordbSymbol - GetRVA\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPermissionSetProps(
		mdPermission pm,                    // [IN] the permission token.
		DWORD* pdwAction,             // [OUT] CorDeclSecurity.
		void const** ppvPermission,        // [OUT] permission blob.
		ULONG* pcbPermission)   // [OUT] count of bytes of pvPermission.

	{
		printf("CordbSymbol - GetPermissionSetProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetSigFromToken(             // S_OK or error.
		mdSignature mdSig,                  // [IN] Signature token.
		PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to token.
		ULONG* pcbSig)          // [OUT] return size of signature.

	{
		printf("CordbSymbol - GetSigFromToken\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetModuleRefProps(           // S_OK or error.
		mdModuleRef mur,                    // [IN] moduleref token.
		__out_ecount_part_opt(cchName, *pchName)
		LPWSTR      szName,                 // [OUT] buffer to fill with the moduleref name.
		ULONG       cchName,                // [IN] size of szName in wide characters.
		ULONG* pchName)         // [OUT] actual count of characters in the name.

	{
		printf("CordbSymbol - GetModuleRefProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumModuleRefs(              // S_OK or error.
		HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
		mdModuleRef rModuleRefs[],          // [OUT] put modulerefs here.
		ULONG       cmax,                   // [IN] max memberrefs to put.
		ULONG* pcModuleRefs)    // [OUT] put # put here.

	{
		printf("CordbSymbol - EnumModuleRefs\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeSpecFromToken(        // S_OK or error.
		mdTypeSpec typespec,                // [IN] TypeSpec token.
		PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to TypeSpec signature
		ULONG* pcbSig)          // [OUT] return size of signature.

	{
		printf("CordbSymbol - GetTypeSpecFromToken\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetNameFromToken(            // Not Recommended! May be removed!
		mdToken     tk,                     // [IN] Token to get name from.  Must have a name.
		MDUTF8CSTR* pszUtf8NamePtr)  // [OUT] Return pointer to UTF8 name in heap.

	{
		printf("CordbSymbol - GetNameFromToken\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumUnresolvedMethods(       // S_OK, S_FALSE, or error.
		HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
		mdToken     rMethods[],             // [OUT] Put MemberDefs here.
		ULONG       cMax,                   // [IN] Max MemberDefs to put.
		ULONG* pcTokens)        // [OUT] Put # put here.

	{
		printf("CordbSymbol - EnumUnresolvedMethods\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetUserString(               // S_OK or error.
		mdString    stk,                    // [IN] String token.
		__out_ecount_part_opt(cchString, *pchString)
		LPWSTR      szString,               // [OUT] Copy of string.
		ULONG       cchString,              // [IN] Max chars of room in szString.
		ULONG* pchString)       // [OUT] How many chars in actual string.

	{
		printf("CordbSymbol - GetUserString\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPinvokeMap(               // S_OK or error.
		mdToken     tk,                     // [IN] FieldDef or MethodDef.
		DWORD* pdwMappingFlags,       // [OUT] Flags used for mapping.
		__out_ecount_part_opt(cchImportName, *pchImportName)
		LPWSTR      szImportName,           // [OUT] Import name.
		ULONG       cchImportName,          // [IN] Size of the name buffer.
		ULONG* pchImportName,         // [OUT] Actual number of characters stored.
		mdModuleRef* pmrImportDLL)    // [OUT] ModuleRef token for the target DLL.
	{
		printf("CordbSymbol - GetPinvokeMap\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumSignatures(              // S_OK or error.
		HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
		mdSignature rSignatures[],          // [OUT] put signatures here.
		ULONG       cmax,                   // [IN] max signatures to put.
		ULONG* pcSignatures)    // [OUT] put # put here.
	{
		printf("CordbSymbol - EnumSignatures\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumTypeSpecs(               // S_OK or error.
		HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
		mdTypeSpec  rTypeSpecs[],           // [OUT] put TypeSpecs here.
		ULONG       cmax,                   // [IN] max TypeSpecs to put.
		ULONG* pcTypeSpecs)     // [OUT] put # put here.
	{
		printf("CordbSymbol - EnumTypeSpecs\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE EnumUserStrings(             // S_OK or error.
		HCORENUM* phEnum,                // [IN/OUT] pointer to the enum.
		mdString    rStrings[],             // [OUT] put Strings here.
		ULONG       cmax,                   // [IN] max Strings to put.
		ULONG* pcStrings)       // [OUT] put # put here.
	{
		printf("CordbSymbol - EnumUserStrings\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetParamForMethodIndex(      // S_OK or error.
		mdMethodDef md,                     // [IN] Method token.
		ULONG       ulParamSeq,             // [IN] Parameter sequence.
		mdParamDef* ppd)             // [IN] Put Param token here.
	{
		printf("CordbSymbol - GetParamForMethodIndex\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumCustomAttributes(        // S_OK or error.
		HCORENUM* phEnum,                // [IN, OUT] COR enumerator.
		mdToken     tk,                     // [IN] Token to scope the enumeration, 0 for all.
		mdToken     tkType,                 // [IN] Type of interest, 0 for all.
		mdCustomAttribute rCustomAttributes[], // [OUT] Put custom attribute tokens here.
		ULONG       cMax,                   // [IN] Size of rCustomAttributes.
		ULONG* pcCustomAttributes)  // [OUT, OPTIONAL] Put count of token values here.
	{
		printf("CordbSymbol - EnumCustomAttributes\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCustomAttributeProps(     // S_OK or error.
		mdCustomAttribute cv,               // [IN] CustomAttribute token.
		mdToken* ptkObj,                // [OUT, OPTIONAL] Put object token here.
		mdToken* ptkType,               // [OUT, OPTIONAL] Put AttrType token here.
		void const** ppBlob,               // [OUT, OPTIONAL] Put pointer to data here.
		ULONG* pcbSize)         // [OUT, OPTIONAL] Put size of date here.
	{
		printf("CordbSymbol - GetCustomAttributeProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE FindTypeRef(
		mdToken     tkResolutionScope,      // [IN] ModuleRef, AssemblyRef or TypeRef.
		LPCWSTR     szName,                 // [IN] TypeRef Name.
		mdTypeRef* ptr)             // [OUT] matching TypeRef.
	{
		printf("CordbSymbol - FindTypeRef\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetMemberProps(
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
		ULONG* pcchValue)       // [OUT] size of constant string in chars, 0 for non-strings.
	{
		printf("CordbSymbol - GetMemberProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFieldProps(
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
		ULONG* pcchValue)       // [OUT] size of constant string in chars, 0 for non-strings.
	{
		printf("CordbSymbol - GetFieldProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetPropertyProps(            // S_OK, S_FALSE, or error.
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
		ULONG* pcOtherMethod)   // [OUT] total number of other method of this property
	{
		printf("CordbSymbol - GetPropertyProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetParamProps(               // S_OK or error.
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
		ULONG* pcchValue)       // [OUT] size of constant string in chars, 0 for non-strings.
	{
		printf("CordbSymbol - GetParamProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCustomAttributeByName(    // S_OK or error.
		mdToken     tkObj,                  // [IN] Object with Custom Attribute.
		LPCWSTR     szName,                 // [IN] Name of desired Custom Attribute.
		const void** ppData,               // [OUT] Put pointer to data here.
		ULONG* pcbData)         // [OUT] Put size of data here.
	{
		printf("CordbSymbol - GetCustomAttributeByName\n");
		fflush(stdout);
		return S_OK;
	}

	virtual BOOL STDMETHODCALLTYPE IsValidToken(         // True or False.
		mdToken     tk)               // [IN] Given token.
	{
		printf("CordbSymbol - IsValidToken\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetNestedClassProps(         // S_OK or error.
		mdTypeDef   tdNestedClass,          // [IN] NestedClass token.
		mdTypeDef* ptdEnclosingClass) // [OUT] EnclosingClass token.
	{
		printf("CordbSymbol - GetNestedClassProps\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetNativeCallConvFromSig(    // S_OK or error.
		void const* pvSig,                 // [IN] Pointer to signature.
		ULONG       cbSig,                  // [IN] Count of signature bytes.
		ULONG* pCallConv)       // [OUT] Put calling conv here (see CorPinvokemap).
	{
		printf("CordbSymbol - GetNativeCallConvFromSig\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE IsGlobal(                    // S_OK or error.
		mdToken     pd,                     // [IN] Type, Field, or Method token.
		int* pbGlobal)        // [OUT] Put 1 if global, 0 otherwise.

	{
		printf("CordbSymbol - IsGlobal\n");
		fflush(stdout);
		return S_OK;
	}
};


class CordbAppDomain :
	public ICorDebugAppDomain,
	public ICorDebugAppDomain2,
	public ICorDebugAppDomain3,
	public ICorDebugAppDomain4
{
public:
	CordbAppDomain(ICorDebugProcess* ppProcess)
	{
		pProcess = ppProcess;
	}
private:
	ICorDebugProcess* pProcess;

	virtual HRESULT STDMETHODCALLTYPE Stop(
		/* [in] */ DWORD dwTimeoutIgnored)
	{
		printf("CordbAppDomain - Stop\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Continue(
		/* [in] */ BOOL fIsOutOfBand)
	{
		printf("CordbAppDomain - Continue - %x\n", connection);
		fflush(stdout);
		pProcess->Continue(fIsOutOfBand);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE IsRunning(
		/* [out] */ BOOL* pbRunning)
	{
		printf("CordbAppDomain - IsRunning\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE HasQueuedCallbacks(
		/* [in] */ ICorDebugThread* pThread,
		/* [out] */ BOOL* pbQueued)
	{
		printf("CordbAppDomain - HasQueuedCallbacks\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateThreads(
		/* [out] */ ICorDebugThreadEnum** ppThreads)
	{
		printf("CordbAppDomain - EnumerateThreads\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE SetAllThreadsDebugState(
		/* [in] */ CorDebugThreadState state,
		/* [in] */ ICorDebugThread* pExceptThisThread)
	{
		printf("CordbAppDomain - SetAllThreadsDebugState\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Detach(void)
	{
		printf("CordbAppDomain - Detach\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE Terminate(
		/* [in] */ UINT exitCode)
	{
		printf("CordbAppDomain - Terminate\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CanCommitChanges(
		/* [in] */ ULONG cSnapshots,
		/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
		/* [out] */ ICorDebugErrorInfoEnum** pError)
	{
		printf("CordbAppDomain - CanCommitChanges\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CommitChanges(
		/* [in] */ ULONG cSnapshots,
		/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
		/* [out] */ ICorDebugErrorInfoEnum** pError)
	{
		printf("CordbAppDomain - CommitChanges\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID id,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
	{
		if (id == IID_ICorDebugAppDomain)
		{
			*ppInterface = (ICorDebugAppDomain*)this;
		}
		else if (id == IID_ICorDebugAppDomain2)
		{
			*ppInterface = (ICorDebugAppDomain2*)this;
		}
		else if (id == IID_ICorDebugAppDomain3)
		{
			*ppInterface = (ICorDebugAppDomain3*)this;
		}
		else if (id == IID_ICorDebugAppDomain4)
		{
			*ppInterface = (ICorDebugAppDomain4*)this;
		}
		else if (id == IID_ICorDebugController)
			*ppInterface = (ICorDebugController*)(ICorDebugAppDomain*)this;
		else if (id == IID_IUnknown)
			*ppInterface = (IUnknown*)(ICorDebugAppDomain*)this;
		else
		{
			*ppInterface = NULL;
			return E_NOINTERFACE;
		}

		printf("CordbAppDomain - QueryInterface\n");
		fflush(stdout);
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		printf("CordbAppDomain - AddRef\n");
		fflush(stdout);
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		printf("CordbAppDomain - Release\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetProcess(
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("CordbAppDomain - GetProcess\n");
		fflush(stdout);
		*ppProcess = pProcess;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateAssemblies(
		/* [out] */ ICorDebugAssemblyEnum** ppAssemblies)
	{
		printf("CordbAppDomain - EnumerateAssemblies\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetModuleFromMetaDataInterface(
		/* [in] */ IUnknown* pIMetaData,
		/* [out] */ ICorDebugModule** ppModule)
	{
		printf("CordbAppDomain - GetModuleFromMetaDataInterface\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE EnumerateBreakpoints(
		/* [out] */ ICorDebugBreakpointEnum** ppBreakpoints)
	{
		printf("CordbAppDomain - EnumerateBreakpoints\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE EnumerateSteppers(
		/* [out] */ ICorDebugStepperEnum** ppSteppers)
	{
		printf("CordbAppDomain - EnumerateSteppers\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE IsAttached(
		/* [out] */ BOOL* pbAttached)
	{
		printf("CordbAppDomain - IsAttached\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetName(
		/* [in] */ ULONG32 cchName,
		/* [out] */ ULONG32* pcchName,
		/* [length_is][size_is][out] */ WCHAR szName[])
	{
		printf("CordbAppDomain - GetName\n");
		if (cchName < strlen("DefaultDomain")) {
			*pcchName = strlen("DefaultDomain") + 1;
			return S_OK;
		}
		wcscpy(szName, L"DefaultDomain");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetObject(
		/* [out] */ ICorDebugValue** ppObject)
	{
		printf("CordbAppDomain - GetObject\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE Attach(void)
	{
		printf("CordbAppDomain - Attach\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetID(
		/* [out] */ ULONG32* pId)
	{
		printf("CordbAppDomain - GetID\n");
		fflush(stdout);
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetArrayOrPointerType(
		/* [in] */ CorElementType elementType,
		/* [in] */ ULONG32 nRank,
		/* [in] */ ICorDebugType* pTypeArg,
		/* [out] */ ICorDebugType** ppType)
	{
		printf("CordbAppDomain - GetArrayOrPointerType\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetFunctionPointerType(
		/* [in] */ ULONG32 nTypeArgs,
		/* [size_is][in] */ ICorDebugType* ppTypeArgs[],
		/* [out] */ ICorDebugType** ppType)
	{
		printf("CordbAppDomain - GetFunctionPointerType\n");
		fflush(stdout);
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCachedWinRTTypesForIIDs(
		/* [in] */ ULONG32 cReqTypes,
		/* [size_is][in] */ GUID* iidsToResolve,
		/* [out] */ ICorDebugTypeEnum** ppTypesEnum)
	{
		printf("CordbAppDomain - GetCachedWinRTTypesForIIDs\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetCachedWinRTTypes(
		/* [out] */ ICorDebugGuidToTypeEnum** ppGuidToTypeEnum)
	{
		printf("CordbAppDomain - GetCachedWinRTTypes\n");
		fflush(stdout);
		return S_OK;
	}


	virtual HRESULT STDMETHODCALLTYPE GetObjectForCCW(
		/* [in] */ CORDB_ADDRESS ccwPointer,
		/* [out] */ ICorDebugValue** ppManagedObject)
	{
		printf("CordbAppDomain - GetObjectForCCW\n");
		fflush(stdout);
		return S_OK;
	}

};


class CordbModule : 
	public ICorDebugModule,
	public ICorDebugModule2,
	public ICorDebugModule3,
	public ICorDebugModule4
{
public:
	CordbProcess* pProcess;
	CordbSymbol* pCordbSymbol;
	CordbAssembly* pAssembly;
	CordbModule(CordbProcess* process, CordbAssembly* assembly)
	{
		pProcess = process;
		pCordbSymbol = NULL;
		pAssembly = assembly;
	}

	HRESULT CordbModule::QueryInterface(REFIID id, void** pInterface)
	{
		if (id == IID_ICorDebugModule)
		{
			*pInterface = static_cast<ICorDebugModule*>(this);
		}
		else if (id == IID_ICorDebugModule2)
		{
			*pInterface = static_cast<ICorDebugModule2*>(this);
		}
		else if (id == IID_ICorDebugModule3)
		{
			*pInterface = static_cast<ICorDebugModule3*>(this);
		}
		else if (id == IID_ICorDebugModule4)
		{
			*pInterface = static_cast<ICorDebugModule4*>(this);
		}
		else if (id == IID_IUnknown)
		{
			*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugModule*>(this));
		}
		else
		{
			*pInterface = NULL;
			return E_NOINTERFACE;
		}
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		printf("AddRef\n");
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		printf("Release\n");
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE IsMappedLayout(
		/* [out] */ BOOL* pIsMapped)
	{
		printf("CordbModule - IsMappedLayout\n");
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE CreateReaderForInMemorySymbols(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ void** ppObj)
	{
		printf("CordbModule - CreateReaderForInMemorySymbols\n");
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE SetJMCStatus(
		/* [in] */ BOOL bIsJustMyCode,
		/* [in] */ ULONG32 cTokens,
		/* [size_is][in] */ mdToken pTokens[])
	{
		printf("CordbModule - SetJMCStatus\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ApplyChanges(
		/* [in] */ ULONG cbMetadata,
		/* [size_is][in] */ BYTE pbMetadata[],
		/* [in] */ ULONG cbIL,
		/* [size_is][in] */ BYTE pbIL[])
	{
		printf("CordbModule - ApplyChanges\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE SetJITCompilerFlags(
		/* [in] */ DWORD dwFlags)
	{
		printf("CordbModule - SetJITCompilerFlags\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetJITCompilerFlags(
		/* [out] */ DWORD* pdwFlags)
	{
		printf("CordbModule - GetJITCompilerFlags\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE ResolveAssembly(
		/* [in] */ mdToken tkAssemblyRef,
		/* [out] */ ICorDebugAssembly** ppAssembly)
	{
		printf("CordbModule - ResolveAssembly\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetProcess(
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("CordbModule - GetProcess\n");
		// *ppProcess = pProcess;
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetBaseAddress(
		/* [out] */ CORDB_ADDRESS* pAddress)
	{
		printf("CordbModule - GetBaseAddress\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetAssembly(
		/* [out] */ ICorDebugAssembly** ppAssembly);

	virtual HRESULT STDMETHODCALLTYPE GetName(
		/* [in] */ ULONG32 cchName,
		/* [out] */ ULONG32* pcchName,
		/* [length_is][size_is][out] */ WCHAR szName[])
	{
		printf("CordbModule - GetName\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableJITDebugging(
		/* [in] */ BOOL bTrackJITInfo,
		/* [in] */ BOOL bAllowJitOpts)
	{
		printf("CordbModule - EnableJITDebugging\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableClassLoadCallbacks(
		/* [in] */ BOOL bClassLoadCallbacks)
	{
		printf("CordbModule - EnableClassLoadCallbacks\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFunctionFromToken(
		/* [in] */ mdMethodDef methodDef,
		/* [out] */ ICorDebugFunction** ppFunction)
	{
		printf("CordbModule - GetFunctionFromToken\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetFunctionFromRVA(
		/* [in] */ CORDB_ADDRESS rva,
		/* [out] */ ICorDebugFunction** ppFunction)
	{
		printf("CordbModule - GetFunctionFromRVA\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetClassFromToken(
		/* [in] */ mdTypeDef typeDef,
		/* [out] */ ICorDebugClass** ppClass)
	{
		printf("CordbModule - GetClassFromToken\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateBreakpoint(
		/* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint)
	{
		printf("CordbModule - CreateBreakpoint\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetEditAndContinueSnapshot(
		/* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot)
	{
		printf("CordbModule - GetEditAndContinueSnapshot\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetMetaDataInterface(
		/* [in] */ REFIID riid,
		/* [out] */ IUnknown** ppObj)
	{
		if (pCordbSymbol == NULL)
			pCordbSymbol = new CordbSymbol();
		pCordbSymbol->QueryInterface(riid, (void**)ppObj);
		printf("CordbModule - GetMetaDataInterface\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetToken(
		/* [out] */ mdModule* pToken)
	{
		printf("CordbModule - GetToken\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE IsDynamic(
		/* [out] */ BOOL* pDynamic)
	{
		printf("CordbModule - IsDynamic\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetGlobalVariableValue(
		/* [in] */ mdFieldDef fieldDef,
		/* [out] */ ICorDebugValue** ppValue)
	{
		printf("CordbModule - GetGlobalVariableValue\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetSize(
		/* [out] */ ULONG32* pcBytes)
	{
		printf("CordbModule - GetSize\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE IsInMemory(
		/* [out] */ BOOL* pInMemory)
	{
		printf("CordbModule - IsInMemory\n");
		return S_OK;
	}

};

class CordbAssembly :
	public ICorDebugAssembly, 
	public ICorDebugAssembly2
{
public:
	CordbProcess* pProcess;
	CordbAssembly(CordbProcess* process)
	{
		pProcess = process;
	}
	virtual HRESULT STDMETHODCALLTYPE IsFullyTrusted(
		/* [out] */ BOOL* pbFullyTrusted)
	{
		printf("CorDebugAssembly - IsFullyTrusted\n");
		return S_OK;
	}
	virtual HRESULT STDMETHODCALLTYPE GetProcess(
		/* [out] */ ICorDebugProcess** ppProcess);

	virtual HRESULT STDMETHODCALLTYPE GetAppDomain(
		/* [out] */ ICorDebugAppDomain** ppAppDomain)
	{
		printf("CorDebugAssembly - GetProcess\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateModules(
		/* [out] */ ICorDebugModuleEnum** ppModules)
	{
		printf("CorDebugAssembly - GetProcess\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetCodeBase(
		/* [in] */ ULONG32 cchName,
		/* [out] */ ULONG32* pcchName,
		/* [length_is][size_is][out] */ WCHAR szName[])
	{
		printf("CorDebugAssembly - GetProcess\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE GetName(
		/* [in] */ ULONG32 cchName,
		/* [out] */ ULONG32* pcchName,
		/* [length_is][size_is][out] */ WCHAR szName[])
	{
		printf("CorDebugAssembly - GetProcess\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID id,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
	{
		if (id == IID_ICorDebugAssembly)
			*ppInterface = static_cast<ICorDebugAssembly*>(this);
		else if (id == IID_ICorDebugAssembly2)
			*ppInterface = static_cast<ICorDebugAssembly2*>(this);
		else if (id == IID_IUnknown)
			*ppInterface = static_cast<IUnknown*>(static_cast<ICorDebugAssembly*>(this));
		else
		{
			*ppInterface = NULL;
			return E_NOINTERFACE;
		}

		printf("CordbAssembly - QueryInterface\n");
		fflush(stdout);
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		printf("AddRef\n"); 
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		printf("Release\n");
		return S_OK;
	}

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
	Cordb* cordb;
	virtual HRESULT STDMETHODCALLTYPE EnumerateLoaderHeapMemoryRegions(
		/* [out] */ ICorDebugMemoryRangeEnum** ppRanges) {
		printf("EnumerateLoaderHeapMemoryRegions\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE EnableGCNotificationEvents(
		BOOL fEnable) {
		printf("EnableGCNotificationEvents\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE EnableExceptionCallbacksOutsideOfMyCode(
		/* [in] */ BOOL enableExceptionsOutsideOfJMC) {
		printf("EnableExceptionCallbacksOutsideOfMyCode\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE SetWriteableMetadataUpdateMode(
		WriteableMetadataUpdateMode flags) {
		printf("SetWriteableMetadataUpdateMode\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE GetGCHeapInformation(
		/* [out] */ COR_HEAPINFO* pHeapInfo) {
		printf("GetGCHeapInformation\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateHeap(
		/* [out] */ ICorDebugHeapEnum** ppObjects) {
		printf("EnumerateHeap\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateHeapRegions(
		/* [out] */ ICorDebugHeapSegmentEnum** ppRegions) {
		printf("EnumerateHeapRegions\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetObject(
		/* [in] */ CORDB_ADDRESS addr,
		/* [out] */ ICorDebugObjectValue** pObject) {
		printf("GetObject\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateGCReferences(
		/* [in] */ BOOL enumerateWeakReferences,
		/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
		printf("EnumerateGCReferences\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateHandles(
		/* [in] */ CorGCReferenceType types,
		/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
		printf("EnumerateHandles\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeID(
		/* [in] */ CORDB_ADDRESS obj,
		/* [out] */ COR_TYPEID* pId) {
		printf("GetTypeID\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeForTypeID(
		/* [in] */ COR_TYPEID id,
		/* [out] */ ICorDebugType** ppType) {
		printf("GetTypeForTypeID\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetArrayLayout(
		/* [in] */ COR_TYPEID id,
		/* [out] */ COR_ARRAY_LAYOUT* pLayout) {
		printf("GetArrayLayout\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeLayout(
		/* [in] */ COR_TYPEID id,
		/* [out] */ COR_TYPE_LAYOUT* pLayout) {
		printf("GetTypeLayout\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetTypeFields(
		/* [in] */ COR_TYPEID id,
		ULONG32 celt,
		COR_FIELD fields[],
		ULONG32* pceltNeeded) {
		printf("GetTypeFields\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableNGENPolicy(
		/* [in] */ CorDebugNGENPolicy ePolicy) {
		printf("EnableNGENPolicy\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE Filter(
		/* [size_is][length_is][in] */ const BYTE pRecord[],
		/* [in] */ DWORD countBytes,
		/* [in] */ CorDebugRecordFormat format,
		/* [in] */ DWORD dwFlags,
		/* [in] */ DWORD dwThreadId,
		/* [in] */ ICorDebugManagedCallback* pCallback,
		/* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus) {
		printf("Filter\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ProcessStateChanged(
		/* [in] */ CorDebugStateChange eChange) {
		printf("ProcessStateChanged\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE SetEnableCustomNotification(
		ICorDebugClass* pClass,
		BOOL fEnable) {
		printf("SetEnableCustomNotification\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE GetID(
		/* [out] */ DWORD* pdwProcessId) {
		printf("GetID\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetHandle(
		/* [out] */ HPROCESS* phProcessHandle) {
		printf("GetHandle\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetThread(
		/* [in] */ DWORD dwThreadId,
		/* [out] */ ICorDebugThread** ppThread) {
		printf("GetThread\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateObjects(
		/* [out] */ ICorDebugObjectEnum** ppObjects) {
		printf("EnumerateObjects\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE IsTransitionStub(
		/* [in] */ CORDB_ADDRESS address,
		/* [out] */ BOOL* pbTransitionStub) {
		printf("IsTransitionStub\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE IsOSSuspended(
		/* [in] */ DWORD threadID,
		/* [out] */ BOOL* pbSuspended) {
		printf("IsOSSuspended\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetThreadContext(
		/* [in] */ DWORD threadID,
		/* [in] */ ULONG32 contextSize,
		/* [size_is][length_is][out][in] */ BYTE context[]) {
		printf("GetThreadContext\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetThreadContext(
		/* [in] */ DWORD threadID,
		/* [in] */ ULONG32 contextSize,
		/* [size_is][length_is][in] */ BYTE context[]) {
		printf("SetThreadContext\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ReadMemory(
		/* [in] */ CORDB_ADDRESS address,
		/* [in] */ DWORD size,
		/* [length_is][size_is][out] */ BYTE buffer[],
		/* [out] */ SIZE_T* read) {
		printf("ReadMemory\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE WriteMemory(
		/* [in] */ CORDB_ADDRESS address,
		/* [in] */ DWORD size,
		/* [size_is][in] */ BYTE buffer[],
		/* [out] */ SIZE_T* written) {
		printf("WriteMemory\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ClearCurrentException(
		/* [in] */ DWORD threadID) {
		printf("ClearCurrentException\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnableLogMessages(
		/* [in] */ BOOL fOnOff) {
		printf("EnableLogMessages\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ModifyLogSwitch(
		/* [annotation][in] */
		_In_  WCHAR* pLogSwitchName,
		/* [in] */ LONG lLevel) {
		printf("ModifyLogSwitch\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateAppDomains(
		/* [out] */ ICorDebugAppDomainEnum** ppAppDomains) {
		printf("EnumerateAppDomains\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetObject(
		/* [out] */ ICorDebugValue** ppObject) {
		printf("GetObject\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ThreadForFiberCookie(
		/* [in] */ DWORD fiberCookie,
		/* [out] */ ICorDebugThread** ppThread) {
		printf("ThreadForFiberCookie\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetHelperThreadID(
		/* [out] */ DWORD* pThreadID) {
		printf("GetHelperThreadID\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE GetThreadForTaskID(
		/* [in] */ TASKID taskid,
		/* [out] */ ICorDebugThread2** ppThread) {
		printf("GetHelperThreadID\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetVersion(
		/* [out] */ COR_VERSION* version) {
		printf("GetVersion\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetUnmanagedBreakpoint(
		/* [in] */ CORDB_ADDRESS address,
		/* [in] */ ULONG32 bufsize,
		/* [length_is][size_is][out] */ BYTE buffer[],
		/* [out] */ ULONG32* bufLen) {
		printf("SetUnmanagedBreakpoint\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE ClearUnmanagedBreakpoint(
		/* [in] */ CORDB_ADDRESS address) {
		printf("ClearUnmanagedBreakpoint\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetDesiredNGENCompilerFlags(
		/* [in] */ DWORD pdwFlags) {
		printf("SetDesiredNGENCompilerFlags\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetDesiredNGENCompilerFlags(
		/* [out] */ DWORD* pdwFlags)
	{
		printf("GetDesiredNGENCompilerFlags\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetReferenceValueFromGCHandle(
		/* [in] */ UINT_PTR handle,
		/* [out] */ ICorDebugReferenceValue** pOutValue)
	{
		printf("GetReferenceValueFromGCHandle\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID id,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
	{
		if (id == IID_ICorDebugProcess)
		{
			*pInterface = static_cast<ICorDebugProcess*>(this);
		}
		else if (id == IID_ICorDebugController)
		{
			*pInterface = static_cast<ICorDebugController*>(static_cast<ICorDebugProcess*>(this));
		}
		else if (id == IID_ICorDebugProcess2)

		{
			*pInterface = static_cast<ICorDebugProcess2*>(this);
		}
		else if (id == IID_ICorDebugProcess3)
		{
			*pInterface = static_cast<ICorDebugProcess3*>(this);
		}
		else if (id == IID_ICorDebugProcess4)
		{
			*pInterface = static_cast<ICorDebugProcess4*>(this);
		}
		else if (id == IID_ICorDebugProcess5)
		{
			*pInterface = static_cast<ICorDebugProcess5*>(this);
		}
		else if (id == IID_ICorDebugProcess7)
		{
			*pInterface = static_cast<ICorDebugProcess7*>(this);
		}
		else if (id == IID_ICorDebugProcess8)
		{
			*pInterface = static_cast<ICorDebugProcess8*>(this);
		}
		else if (id == IID_ICorDebugProcess10)
		{
			*pInterface = static_cast<ICorDebugProcess10*>(this);
		}
		else if (id == IID_ICorDebugProcess11)
		{
			*pInterface = static_cast<ICorDebugProcess11*>(this);
		}
		else if (id == IID_IUnknown)
		{
			*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugProcess*>(this));
		}

		else
		{
			*pInterface = NULL;
			return E_NOINTERFACE;
		}

		printf("CordbProcess - QueryInterface\n");
		fflush(stdout);
		return S_OK;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		printf("AddRef\n"); return 1;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		printf("Release\n"); return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE Stop(
		/* [in] */ DWORD dwTimeoutIgnored) {
		printf("Stop\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE Continue(
		/* [in] */ BOOL fIsOutOfBand)
	{
		printf("CordbProcess - Continue\n");
		Buffer sendbuf;
		buffer_init(&sendbuf, 128);
		connection->send_event(CMD_SET_VM, CMD_VM_RESUME, &sendbuf);
		printf("CordbProcess - Enviei resume\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE IsRunning(
		/* [out] */ BOOL* pbRunning) {
		printf("IsRunning\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE HasQueuedCallbacks(
		/* [in] */ ICorDebugThread* pThread,
		/* [out] */ BOOL* pbQueued) {
		printf("HasQueuedCallbacks\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateThreads(
		/* [out] */ ICorDebugThreadEnum** ppThreads) {
		printf("EnumerateThreads\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetAllThreadsDebugState(
		/* [in] */ CorDebugThreadState state,
		/* [in] */ ICorDebugThread* pExceptThisThread) {
		printf("SetAllThreadsDebugState\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE Detach(void) {
		printf("Detach\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE Terminate(
		/* [in] */ UINT exitCode) {
		printf("CordbProcess - Terminate\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE CanCommitChanges(
		/* [in] */ ULONG cSnapshots,
		/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
		/* [out] */ ICorDebugErrorInfoEnum** pError) {
		printf("CanCommitChanges\n"); return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE CommitChanges(
		/* [in] */ ULONG cSnapshots,
		/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
		/* [out] */ ICorDebugErrorInfoEnum** pError) {
		printf("CommitChanges\n"); return 1;
	}
};

MONO_API HRESULT CoreCLRCreateCordbObjectEx(int iDebuggerVersion, DWORD pid, LPCWSTR lpApplicationGroupId, HMODULE hmodTargetCLR, void** ppCordb)
{
	printf("CoreCLRCreateCordbObjectEx - cheguei aqui hein thays\n");
	return 1;
}

static void debugger_thread(void* ppProcess)
{
	connection = new Connection((CordbProcess*)ppProcess, ((CordbProcess*)ppProcess)->cordb);
	connection->start_connection();
	connection->transport_handshake();
	connection->loop_send_receive();
	connection->close_connection();
}

class Cordb : public ICorDebug, public ICorDebugRemote
{
public:
	ICorDebugManagedCallback* pCallback;
	virtual HRESULT STDMETHODCALLTYPE Initialize(void)
	{
		printf("Initialize\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE Terminate(void)
	{
		printf("Cordb - Terminate\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetManagedHandler(
		/* [in] */ ICorDebugManagedCallback* pCallback)
	{
		printf("SetManagedHandler - %x\n", pCallback);
		this->pCallback = pCallback;
		this->pCallback->AddRef();
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE SetUnmanagedHandler(
		/* [in] */ ICorDebugUnmanagedCallback* pCallback)
	{
		printf("SetUnmanagedHandler\n");
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE CreateProcess(
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
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("CreateProcess\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE DebugActiveProcess(
		/* [in] */ DWORD id,
		/* [in] */ BOOL win32Attach,
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("DebugActiveProcess\n");
		*ppProcess = new CordbProcess;
		((CordbProcess*)*ppProcess)->cordb = this;
		
		DWORD thread_id;
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)debugger_thread, ((CordbProcess*)*ppProcess), 0, &thread_id);

		/*connection = new Connection((CordbProcess*)*ppProcess, this);
		connection->start_connection();
		connection->transport_handshake();
		connection->loop_send_receive();
		connection->close_connection();*/
		return S_OK;
	}

	virtual HRESULT STDMETHODCALLTYPE EnumerateProcesses(
		/* [out] */ ICorDebugProcessEnum** ppProcess)
	{
		printf("EnumerateProcesses\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE GetProcess(
		/* [in] */ DWORD dwProcessId,
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("GetProcess\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE CanLaunchOrAttach(
		/* [in] */ DWORD dwProcessId,
		/* [in] */ BOOL win32DebuggingEnabled)
	{
		printf("CanLaunchOrAttach\n");
		return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE QueryInterface(
		/* [in] */ REFIID riid,
		/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
	{
		printf("QueryInterface 2\n");
		fflush(stdout);
		return 1;
	}

	virtual ULONG STDMETHODCALLTYPE AddRef(void)
	{
		printf("AddRef\n");
		return 1;
	}

	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		printf("Release\n");
		return 1;
	}
	virtual HRESULT STDMETHODCALLTYPE CreateProcessEx(
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
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("CreateProcessEx\n");
		return 1;
	}

	virtual HRESULT STDMETHODCALLTYPE DebugActiveProcessEx(
		/* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
		/* [in] */ DWORD dwProcessId,
		/* [in] */ BOOL fWin32Attach,
		/* [out] */ ICorDebugProcess** ppProcess)
	{
		printf("DebugActiveProcessEx\n");
		return 1;
	}
};


Connection::Connection(CordbProcess* proc, Cordb* cordb)
{
	ppProcess = proc;
	ppCordb = cordb;
	pCorDebugAppDomain = NULL;
}

void Connection::loop_send_receive()
{
	//machine.EnableEvents(EventType.AssemblyLoad, EventType.ThreadStart, EventType.ThreadDeath, EventType.AppDomainUnload, EventType.UserBreak, EventType.UserLog);
	enable_event(EVENT_KIND_ASSEMBLY_LOAD);
	enable_event(EVENT_KIND_APPDOMAIN_CREATE);
	enable_event(EVENT_KIND_THREAD_START);
	enable_event(EVENT_KIND_THREAD_DEATH);
	enable_event(EVENT_KIND_APPDOMAIN_UNLOAD);
	enable_event(EVENT_KIND_USER_BREAK);
	enable_event(EVENT_KIND_USER_LOG);

	int buflen = 128;

	int iResult;
	Buffer sendbuf;
	Buffer recvbuf, recvbufheader;
	// Receive until the peer closes the connection
	do {
		buffer_init(&sendbuf, buflen);
		buffer_init(&recvbufheader, HEADER_LEN);

		iResult = recv(connect_socket, (char*)recvbufheader.buf, HEADER_LEN, 0);
		if (iResult > 0)
			printf("Bytes received 1: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

		Header header;
		decode_command_header(&recvbufheader, &header);

		printf("header.len == %d\n", header.len);
		printf("header.flags == %x\n", header.flags);
		printf("header.command_set == %d\n", header.command_set);
		printf("header.command == %d\n", header.command);

		buffer_init(&recvbuf, header.len - HEADER_LEN);
		iResult = recv(connect_socket, (char*)recvbuf.buf, header.len - HEADER_LEN, 0);
		if (iResult > 0)
			printf("Bytes received 2: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

		if (header.command_set == CMD_SET_EVENT && header.command == CMD_COMPOSITE) {
			int spolicy = decode_byte(recvbuf.buf, &recvbuf.buf, recvbuf.end);
			int nevents = decode_int(recvbuf.buf, &recvbuf.buf, recvbuf.end);

			for (int i = 0; i < nevents; ++i) {

				int kind = decode_byte(recvbuf.buf, &recvbuf.buf, recvbuf.end);
				int req_id = decode_int(recvbuf.buf, &recvbuf.buf, recvbuf.end);

				EventKind etype = (EventKind)kind;

				long thread_id = decode_id(recvbuf.buf, &recvbuf.buf, recvbuf.end);

				printf("Received %d events %s(%d), suspend=%d.\n", nevents, event_to_string(etype), etype, spolicy);
				fflush(stdout);
				switch (etype)
				{
					case EVENT_KIND_VM_START:
					{

						Buffer sendbuf;
						buffer_init(&sendbuf, 128);
						send_event(0, 0, &sendbuf);

						printf("vou chamar o callback do createprocess - %x - %x\n", ppCordb, ppCordb->pCallback);
						ppCordb->pCallback->CreateProcess(static_cast<ICorDebugProcess*>(ppProcess));
						printf("chamei o callback do createprocess\n");
						fflush(stdout);
					}
					break;
					case EVENT_KIND_ASSEMBLY_LOAD:
					{
						if (pCorDebugAppDomain == NULL) {
							pCorDebugAppDomain = new CordbAppDomain(static_cast<ICorDebugProcess*>(ppProcess));
							printf("vou chamar o callback do CreateAppDomain\n");
							ppCordb->pCallback->CreateAppDomain(static_cast<ICorDebugProcess*>(ppProcess), pCorDebugAppDomain);
							printf("chamei o callback do CreateAppDomain\n");
							fflush(stdout);
						}

						ICorDebugAssembly* pAssembly = new CordbAssembly(ppProcess);
						printf("vou chamar o callback do LoadAssembly\n");
						fflush(stdout);
						ppCordb->pCallback->LoadAssembly(pCorDebugAppDomain, pAssembly);
						printf("chamei o callback do LoadAssembly\n");
						fflush(stdout);

						ICorDebugModule* pModule = new CordbModule(ppProcess, (CordbAssembly*)pAssembly);
						printf("vou chamar o callback do LoadModule\n");
						fflush(stdout);
						ppCordb->pCallback->LoadModule(pCorDebugAppDomain, pModule);
						printf("chamei o callback do LoadModule\n");
						fflush(stdout);
					}
					break;
				}
			}
			//buffer_free(&recvbuf);
		}

		//buffer_free(&sendbuf);
		//buffer_free(&recvbufheader);
		printf("tentando ler\n");
		Sleep(1000);
	} while (iResult >= 0);
}

void Connection::enable_event(EventKind eventKind)
{
	Buffer sendbuf;
	int buflen = 128;
	buffer_init(&sendbuf, buflen);
	buffer_add_byte(&sendbuf, eventKind);
	buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
	buffer_add_byte(&sendbuf, 0); //modifiers
	connection->send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
	buffer_free(&sendbuf);
}

void Connection::close_connection()
{
	closesocket(connect_socket);
	WSACleanup();
}

void Connection::start_connection()
{
	Sleep(1000);
	WSADATA wsaData;
	connect_socket = INVALID_SOCKET;
	struct addrinfo* result = NULL,
		* ptr = NULL,
		hints;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", "3001", &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		connect_socket = socket(ptr->ai_family, ptr->ai_socktype,
			ptr->ai_protocol);
		if (connect_socket == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(connect_socket);
			connect_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (connect_socket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
	printf("connected\n");
}

void Connection::transport_handshake()
{
	int buflen = 128;

	Buffer sendbuf;
	buffer_init(&sendbuf, buflen);

	Buffer recvbuf;
	buffer_init(&recvbuf, buflen);

	int iResult;
	iResult = recv(connect_socket, (char*)recvbuf.buf, buflen, 0);
	if (iResult > 0)
		printf("Bytes received 3: %d\n", iResult);

	// Send an initial buffer
	buffer_add_data(&sendbuf, (guint8*)"DWP-Handshake", 13);
	send_packet(sendbuf);

	printf("Bytes Sent: %ld\n", iResult);
}

void Connection::send_packet(Buffer& sendbuf)
{
	printf("send_packet - %d\n", buffer_len(&sendbuf));
	fflush(stdout);
	int iResult = send(connect_socket, (const char*)sendbuf.buf, buffer_len(&sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connect_socket);
		WSACleanup();
		return;
	}
}

void Connection::receive_packet(Buffer& recvbuf)
{
	buffer_init(&recvbuf, 128);
	int iResult;
	iResult = recv(connect_socket, (char*)recvbuf.buf, 128, 0);
	if (iResult > 0)
		printf("Bytes received 2: %d\n", iResult);
}

void Connection::send_event(int cmd_set, int cmd, Buffer *sendbuf)
{
	Buffer outbuf;
	buffer_add_command_header(sendbuf, cmd_set, cmd, &outbuf);
	send_packet(outbuf);
	//receive_packet(outbuf);
}




MONO_API HRESULT CoreCLRCreateCordbObject(int iDebuggerVersion, DWORD pid, HMODULE hmodTargetCLR, void** ppCordb)
{
	*ppCordb = new Cordb();
	printf("CoreCLRCreateCordbObject - cheguei aqui hein thays\n");
	return 1;
}

MONO_API HRESULT CreateCordbObject(int iDebuggerVersion, void** ppCordb)
{
	printf("CreateCordbObject - cheguei aqui hein thays\n");
	return 1;
}


HRESULT STDMETHODCALLTYPE CordbModule::GetAssembly(
	/* [out] */ ICorDebugAssembly** ppAssembly)
{
	printf("CordbModule - GetAssembly\n");
	*ppAssembly = static_cast<ICorDebugAssembly*> (pAssembly);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbAssembly::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("CorDebugAssembly - GetProcess\n");
	*ppProcess = static_cast<ICorDebugProcess*>(pProcess);
	return S_OK;
}
