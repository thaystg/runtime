#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb-frame.hpp>
#include <cordb-thread.hpp>
#include <cordb-stepper.hpp>
#include <cordb-symbol.hpp>
#include <cordb-assembly.hpp>
#include <cordb-process.hpp>
#include <cordb-function.hpp>
#include <mono/utils/atomic.h>

using namespace std;

CordbParameter::CordbParameter(int type, char* name, int method_token){
	this->type = type;
	this->name = name;
	this->method_token = method_token;
}

CordbSymbol::CordbSymbol(CordbAssembly* cordbAssembly)
{
	module_id = -1;
	pCordbAssembly = cordbAssembly;
	parameters = g_hash_table_new (NULL, NULL);
	token_id = 0;
}

HRESULT CordbSymbol::EnumGenericParams(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken      tk,                    // [IN] TypeDef or MethodDef whose generic parameters are requested
	mdGenericParam rGenericParams[],    // [OUT] Put GenericParams here.
	ULONG       cMax,                   // [IN] Max GenericParams to put.
	ULONG* pcGenericParams)
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumGenericParams - NOT IMPLEMENTED - %d\n", tk);
	*pcGenericParams = 0;
	return S_OK;
}

HRESULT CordbSymbol::GetGenericParamProps(        // S_OK or error.
	mdGenericParam gp,                  // [IN] GenericParam
	ULONG* pulParamSeq,          // [OUT] Index of the type parameter
	DWORD* pdwParamFlags,        // [OUT] Flags, for future use (e.g. variance)
	mdToken* ptOwner,              // [OUT] Owner (TypeDef or MethodDef)
	DWORD* reserved,              // [OUT] For future use (e.g. non-type parameters)
	_Out_writes_to_opt_(cchName, *pchName)
	LPWSTR       wzname,                // [OUT] Put name here
	ULONG        cchName,               // [IN] Size of buffer
	ULONG* pchName) {
	DEBUG_PRINTF(1, "CordbSymbol - GetGenericParamProps - NOT IMPLEMENTED\n");
	return S_OK;
}       // [OUT] Put size of name (wide chars) here.

HRESULT CordbSymbol::GetMethodSpecProps(
	mdMethodSpec mi,                    // [IN] The method instantiation
	mdToken* tkParent,                  // [OUT] MethodDef or MemberRef
	PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
	ULONG* pcbSigBlob) {

	DEBUG_PRINTF(1, "CordbSymbol - GetMethodSpecProps - NOT IMPLEMENTED\n");return S_OK;
}     // [OUT] actual size of signature blob

HRESULT CordbSymbol::EnumGenericParamConstraints(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdGenericParam tk,                  // [IN] GenericParam whose constraints are requested
	mdGenericParamConstraint rGenericParamConstraints[],    // [OUT] Put GenericParamConstraints here.
	ULONG       cMax,                   // [IN] Max GenericParamConstraints to put.
	ULONG* pcGenericParamConstraints) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumGenericParamConstraints - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] Put # put here.

HRESULT CordbSymbol::GetGenericParamConstraintProps( // S_OK or error.
	mdGenericParamConstraint gpc,       // [IN] GenericParamConstraint
	mdGenericParam* ptGenericParam,     // [OUT] GenericParam that is constrained
	mdToken* ptkConstraintType) {
	DEBUG_PRINTF(1, "CordbSymbol - GetGenericParamConstraintProps - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] TypeDef/Ref/Spec constraint

HRESULT CordbSymbol::GetPEKind(                   // S_OK or error.
	DWORD* pdwPEKind,                   // [OUT] The kind of PE (0 - not a PE)
	DWORD* pdwMAchine) {
	DEBUG_PRINTF(1, "CordbSymbol - GetPEKind - NOT IMPLEMENTED\n");
	return S_OK;
}           // [OUT] Machine as defined in NT header

HRESULT CordbSymbol::GetVersionString(            // S_OK or error.
	_Out_writes_to_opt_(ccBufSize, *pccBufSize)
	LPWSTR      pwzBuf,                 // [OUT] Put version string here.
	DWORD       ccBufSize,              // [IN] size of the buffer, in wide chars
	DWORD* pccBufSize) {
	DEBUG_PRINTF(1, "CordbSymbol - GetVersionString - NOT IMPLEMENTED\n");
	return S_OK;
}     // [OUT] Size of the version string, wide chars, including terminating nul.

HRESULT CordbSymbol::EnumMethodSpecs(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken      tk,                    // [IN] MethodDef or MemberRef whose MethodSpecs are requested
	mdMethodSpec rMethodSpecs[],        // [OUT] Put MethodSpecs here.
	ULONG       cMax,                   // [IN] Max tokens to put.
	ULONG* pcMethodSpecs) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumMethodSpecs - NOT IMPLEMENTED\n");
	return S_OK;
}  // [OUT] Put actual count here.

HRESULT CordbSymbol::GetAssemblyProps(            // S_OK or error.
	mdAssembly  mda,                    // [IN] The Assembly for which to get the properties.
	const void** ppbPublicKey,         // [OUT] Pointer to the public key.
	ULONG* pcbPublicKey,          // [OUT] Count of bytes in the public key.
	ULONG* pulHashAlgId,          // [OUT] Hash Algorithm.
	_Out_writes_to_opt_(cchName, *pchName) LPWSTR  szName, // [OUT] Buffer to fill with assembly's simply name.
	ULONG       cchName,                // [IN] Size of buffer in wide chars.
	ULONG* pchName,               // [OUT] Actual # of wide chars in name.
	ASSEMBLYMETADATA* pMetaData,        // [OUT] Assembly MetaData.
	DWORD* pdwAssemblyFlags) {
	DEBUG_PRINTF(1, "CordbSymbol - GetAssemblyProps - THAYS - NOT IMPLEMENTED - %d - %d\n", cchName, mda);

	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_SIMPLE_NAME, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char* assembly_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	if (cchName > strlen(assembly_name))
	{
		mbstowcs(szName, assembly_name, strlen(assembly_name) + 1);
	}
	*pchName = strlen(assembly_name) + 1;

	DEBUG_PRINTF(1, "CordbSymbol - GetAssemblyProps - NOT IMPLEMENTED - %d - %d - %s\n", cchName, mda, assembly_name);
	return S_OK;
}   // [OUT] Flags.

HRESULT CordbSymbol::GetAssemblyRefProps(         // S_OK or error.
	mdAssemblyRef mdar,                 // [IN] The AssemblyRef for which to get the properties.
	const void** ppbPublicKeyOrToken,  // [OUT] Pointer to the public key or token.
	ULONG* pcbPublicKeyOrToken,   // [OUT] Count of bytes in the public key or token.
	_Out_writes_to_opt_(cchName, *pchName)LPWSTR szName, // [OUT] Buffer to fill with name.
	ULONG       cchName,                // [IN] Size of buffer in wide chars.
	ULONG* pchName,               // [OUT] Actual # of wide chars in name.
	ASSEMBLYMETADATA* pMetaData,        // [OUT] Assembly MetaData.
	const void** ppbHashValue,         // [OUT] Hash blob.
	ULONG* pcbHashValue,          // [OUT] Count of bytes in the hash blob.
	DWORD* pdwAssemblyRefFlags) {
	DEBUG_PRINTF(1, "CordbSymbol - GetAssemblyRefProps - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] Flags.

HRESULT CordbSymbol::GetFileProps(                // S_OK or error.
	mdFile      mdf,                    // [IN] The File for which to get the properties.
	_Out_writes_to_opt_(cchName, *pchName) LPWSTR      szName, // [OUT] Buffer to fill with name.
	ULONG       cchName,                // [IN] Size of buffer in wide chars.
	ULONG* pchName,               // [OUT] Actual # of wide chars in name.
	const void** ppbHashValue,         // [OUT] Pointer to the Hash Value Blob.
	ULONG* pcbHashValue,          // [OUT] Count of bytes in the Hash Value Blob.
	DWORD* pdwFileFlags) {
	DEBUG_PRINTF(1, "CordbSymbol - GetFileProps - NOT IMPLEMENTED\n");
	return S_OK;
}   // [OUT] Flags.

HRESULT CordbSymbol::GetExportedTypeProps(        // S_OK or error.
	mdExportedType   mdct,              // [IN] The ExportedType for which to get the properties.
	_Out_writes_to_opt_(cchName, *pchName) LPWSTR      szName, // [OUT] Buffer to fill with name.
	ULONG       cchName,                // [IN] Size of buffer in wide chars.
	ULONG* pchName,               // [OUT] Actual # of wide chars in name.
	mdToken* ptkImplementation,     // [OUT] mdFile or mdAssemblyRef or mdExportedType.
	mdTypeDef* ptkTypeDef,            // [OUT] TypeDef token within the file.
	DWORD* pdwExportedTypeFlags) {
	DEBUG_PRINTF(1, "CordbSymbol - GetExportedTypeProps - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] Flags.

HRESULT CordbSymbol::GetManifestResourceProps(    // S_OK or error.
	mdManifestResource  mdmr,           // [IN] The ManifestResource for which to get the properties.
	_Out_writes_to_opt_(cchName, *pchName)LPWSTR      szName,  // [OUT] Buffer to fill with name.
	ULONG       cchName,                // [IN] Size of buffer in wide chars.
	ULONG* pchName,               // [OUT] Actual # of wide chars in name.
	mdToken* ptkImplementation,     // [OUT] mdFile or mdAssemblyRef that provides the ManifestResource.
	DWORD* pdwOffset,             // [OUT] Offset to the beginning of the resource within the file.
	DWORD* pdwResourceFlags) {
	DEBUG_PRINTF(1, "CordbSymbol - GetManifestResourceProps - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] Flags.

HRESULT CordbSymbol::EnumAssemblyRefs(            // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdAssemblyRef rAssemblyRefs[],      // [OUT] Put AssemblyRefs here.
	ULONG       cMax,                   // [IN] Max AssemblyRefs to put.
	ULONG* pcTokens) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumAssemblyRefs - NOT IMPLEMENTED\n");
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumFiles(                   // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdFile      rFiles[],               // [OUT] Put Files here.
	ULONG       cMax,                   // [IN] Max Files to put.
	ULONG* pcTokens) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumFiles - NOT IMPLEMENTED\n");
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumExportedTypes(           // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdExportedType   rExportedTypes[],  // [OUT] Put ExportedTypes here.
	ULONG       cMax,                   // [IN] Max ExportedTypes to put.
	ULONG* pcTokens) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumExportedTypes - NOT IMPLEMENTED\n");
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumManifestResources(       // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdManifestResource  rManifestResources[],   // [OUT] Put ManifestResources here.
	ULONG       cMax,                   // [IN] Max Resources to put.
	ULONG* pcTokens) {
	DEBUG_PRINTF(1, "CordbSymbol - EnumManifestResources - NOT IMPLEMENTED\n");
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::FindExportedTypeByName(      // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ExportedType.
	mdToken     mdtExportedType,        // [IN] ExportedType for the enclosing class.
	mdExportedType* ptkExportedType) {
	DEBUG_PRINTF(1, "CordbSymbol - FindExportedTypeByName - NOT IMPLEMENTED\n");
	return S_OK;
}// [OUT] Put the ExportedType token here.

HRESULT CordbSymbol::FindManifestResourceByName(  // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ManifestResource.
	mdManifestResource* ptkManifestResource) {
	DEBUG_PRINTF(1, "CordbSymbol - FindManifestResourceByName - NOT IMPLEMENTED\n");
	return S_OK;
} // [OUT] Put the ManifestResource token here.

HRESULT CordbSymbol::FindAssembliesByName(        // S_OK or error
	LPCWSTR  szAppBase,                 // [IN] optional - can be NULL
	LPCWSTR  szPrivateBin,              // [IN] optional - can be NULL
	LPCWSTR  szAssemblyName,            // [IN] required - this is the assembly you are requesting
	IUnknown* ppIUnk[],                 // [OUT] put IMetaDataAssemblyImport pointers here
	ULONG    cMax,                      // [IN] The max number to put
	ULONG* pcAssemblies) {
	DEBUG_PRINTF(1, "CordbSymbol - FindAssembliesByName - NOT IMPLEMENTED\n");
	return S_OK;
}      // [OUT] The number of assemblies returned.

// IUnknown methods
HRESULT CordbSymbol::QueryInterface(REFIID riid, LPVOID* ppvObj)
{
	if (ppvObj == NULL)
		return E_POINTER;
	if (riid == IID_IMetaDataAssemblyImport)
		*ppvObj = static_cast<IMetaDataAssemblyImport*>(this);
	else
		if (riid == IID_IMetaDataImport2)
			*ppvObj = static_cast<IMetaDataImport2*>(this);
		else
			if (riid == IID_IMetaDataImport)
				*ppvObj = static_cast<IMetaDataImport*>(this);
			else
				if (riid == IID_IUnknown)
					*ppvObj = static_cast<IMetaDataImport*>(this);
				else
				{
					DEBUG_PRINTF(1, "CordbSymbol - QueryInterface - E_NOTIMPL\n");
					return E_NOTIMPL;
				}

	return S_OK;
}

ULONG CordbSymbol::AddRef()
{
	return S_OK;
}
ULONG CordbSymbol::Release()
{
	return S_OK;
}

// IMetaDataImport functions
void CordbSymbol::CloseEnum(HCORENUM hEnum)
{
	DEBUG_PRINTF(1, "CordbSymbol - CloseEnum - IMPLEMENTED\n");

	return;
}

HRESULT CordbSymbol::CountEnum(HCORENUM hEnum, ULONG* pulCount)
{
	DEBUG_PRINTF(1, "CordbSymbol - CountEnum - NOT IMPLEMENTED\n");
	*pulCount = 0;
	return S_OK;
}

HRESULT CordbSymbol::ResetEnum(HCORENUM hEnum, ULONG ulPos)
{
	DEBUG_PRINTF(1, "CordbSymbol - ResetEnum - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeDefs(HCORENUM* phEnum, mdTypeDef rTypeDefs[],
ULONG cMax, ULONG* pcTypeDefs)
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumTypeDefs - %d\n", this->pCordbAssembly->id);
	if (*phEnum == NULL)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPEDEFS, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE (received_reply_packet);
		Buffer *localbuf2 = received_reply_packet->buf;
		int count = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		DEBUG_PRINTF(1, "CordbSymbol - EnumTypeDefs - 1\n");
		
		GPtrArray *typedefs = g_ptr_array_new();
		for (int i = 1; i < count; i++) {
			DEBUG_PRINTF(1, "CordbSymbol - EnumTypeDefs - 2\n");
			int* typedef_id = new int;
			*typedef_id = (i + 1) | MONO_TOKEN_TYPE_DEF;
			g_ptr_array_add(typedefs, typedef_id);
		}
		HENUMInternal* enumInternal = new HENUMInternal();
		enumInternal->items = typedefs;
		enumInternal->currentIdx = 0;
		*phEnum = enumInternal;
	}
	HENUMInternal* enumInternal = static_cast<HENUMInternal*>(*phEnum);
	for (int i = 0; i < cMax; i++) {
		DEBUG_PRINTF(1, "CordbSymbol - EnumTypeDefs - to fazendo o loop - %d - %d\n", enumInternal->currentIdx, enumInternal->items->len);
		if (enumInternal->currentIdx >= enumInternal->items->len) {
			*pcTypeDefs = i;
			return S_OK;
		}
		rTypeDefs[i] = *(int*)(g_ptr_array_index(enumInternal->items, enumInternal->currentIdx));
		DEBUG_PRINTF(1, "CordbSymbol - EnumTypeDefs - to fazendo o loop - %d\n", rTypeDefs[i]);
		enumInternal->currentIdx++;
	}
	*pcTypeDefs = cMax;
	return S_OK;
}

HRESULT CordbSymbol::EnumInterfaceImpls(HCORENUM* phEnum, mdTypeDef td,
mdInterfaceImpl rImpls[], ULONG cMax,
ULONG* pcImpls)
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumInterfaceImpls - NOT IMPLEMENTED\n");
	*pcImpls = 0;
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeRefs(HCORENUM* phEnum, mdTypeRef rTypeRefs[],
ULONG cMax, ULONG* pcTypeRefs)
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumTypeRefs - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindTypeDefByName(           // S_OK or error.
LPCWSTR     szTypeDef,              // [IN] Name of the Type.
mdToken     tkEnclosingClass,       // [IN] TypeDef/TypeRef for Enclosing class.
mdTypeDef* ptd)             // [OUT] Put the TypeDef token here.
{
	int klass_id;
	if (FindTypeDefByNameInternal(szTypeDef, klass_id) != S_OK)
		return S_FALSE;


	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer *localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_fullname_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*ptd = token;
	DEBUG_PRINTF(1, "CordbSymbol - FindTypeDefByName - IMPLEMENTED - %ws - %d\n", szTypeDef, token);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbSymbol::FindTypeDefByNameInternal(LPCWSTR szTypeDef, int& klass_id)
{
	Buffer sendbuf;
	buffer_init(&sendbuf, 128);

	char output[256];
	sprintf(output, "%ws", szTypeDef);
	buffer_add_string(&sendbuf, output);
	buffer_add_byte(&sendbuf, true);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_VM, CMD_VM_GET_TYPES, &sendbuf);
	buffer_free(&sendbuf);

	ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
	CHECK_ERROR_RETURN_FALSE(received_reply_packet);
	Buffer* localbuf2 = received_reply_packet->buf;

	int count_class = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	if (count_class == 0)
		return S_FALSE;
	klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	return S_OK;
}

HRESULT CordbSymbol::GetScopeProps(               // S_OK or error.
__out_ecount_part_opt(cchName, *pchName)
LPWSTR      szName,                 // [OUT] Put the name here.
ULONG       cchName,                // [IN] Size of name buffer in wide chars.
ULONG* pchName,               // [OUT] Put size of name (wide chars) here.
GUID* pmvid)           // [OUT, OPTIONAL] Put MVID here.
{
	if (module_id < 0) {
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_MANIFEST_MODULE, &localbuf);
		buffer_free(&localbuf);

		Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

		module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}

	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, module_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_MODULE, CMD_MODULE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
	char* basename = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* module_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* guid = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* sourcelink = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	guint32 mvid_len;
	guint8 *mvid = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &mvid_len);
	memcpy(pmvid, mvid, mvid_len);
	if (cchName > strlen(name))
	{
		mbstowcs(szName, name, strlen(name) + 1);
		*pchName = strlen(name) + 1;
	}
	
	DEBUG_PRINTF(1, "CordbSymbol - GetScopeProps - IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetModuleFromScope(          // S_OK.
mdModule* pmd)            // [OUT] Put mdModule token here.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetModuleFromScope - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetTypeDefProps(             // S_OK or error.
mdTypeDef   td,                     // [IN] TypeDef token for inquiry.
__out_ecount_part_opt(cchTypeDef, *pchTypeDef)
LPWSTR      szTypeDef,              // [OUT] Put name here.
ULONG       cchTypeDef,             // [IN] size of name buffer in wide chars.
ULONG* pchTypeDef,            // [OUT] put size of name (wide chars) here.
DWORD* pdwTypeDefFlags,       // [OUT] Put flags here.
mdToken* ptkExtends)     // [OUT] Put base class TypeDef/TypeRef here.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED - %d\n", td);
	if (td == 0)
		return E_NOTIMPL;
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	buffer_add_int(&localbuf, td);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.1\n");

	ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
	CHECK_ERROR_RETURN_FALSE (received_reply_packet);
	Buffer *localbuf2 = received_reply_packet->buf;
	
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.2 - klass_id - %d\n", klass_id);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.3\n");

	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_fullname_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int rank = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int flags = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.4\n");


	if (cchTypeDef > strlen(class_fullname_str)) {
		mbstowcs(szTypeDef, class_fullname_str, strlen(class_fullname_str) + 1);
	}
	if (pchTypeDef)
		*pchTypeDef = strlen(class_fullname_str) + 1;

	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.5\n");
	DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED - %s - %d - %d  %d - %d\n", class_fullname_str, type_id, type_id2, flags, token);

	if (pdwTypeDefFlags)
		*pdwTypeDefFlags = flags;

	if (type_id != 0) {
		buffer_init(&localbuf, 128);

		buffer_add_id(&localbuf, type_id);
		cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
		buffer_free(&localbuf);

		localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
		char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.6\n");
		char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.7\n");
		char* class_fullname_str2 = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.8\n");
		int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.9\n");
		module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.10\n");
		int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.11\n");
		int type_id3 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.12\n");
		int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.13\n");
		int rank = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.14\n");
		int flags2 = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.15\n");

		if (ptkExtends)
			*ptkExtends = token;
		DEBUG_PRINTF(1, "CordbSymbol - GetTypeDefProps - IMPLEMENTED 1.16 - flags - %d - token - %d\n", flags, token);
	}
	else
		*ptkExtends = MONO_TOKEN_TYPE_REF;

	return S_OK;
}

HRESULT CordbSymbol::GetInterfaceImplProps(       // S_OK or error.
mdInterfaceImpl iiImpl,             // [IN] InterfaceImpl token.
mdTypeDef* pClass,                // [OUT] Put implementing class token here.
mdToken* ptkIface)        // [OUT] Put implemented interface token here.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetInterfaceImplProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetTypeRefProps(             // S_OK or error.
mdTypeRef   tr,                     // [IN] TypeRef token.
mdToken* ptkResolutionScope,    // [OUT] Resolution scope, ModuleRef or AssemblyRef.
__out_ecount_part_opt(cchName, *pchName)
LPWSTR      szName,                 // [OUT] Name of the TypeRef.
ULONG       cchName,                // [IN] Size of buffer.
ULONG* pchName)         // [OUT] Size of Name.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetTypeRefProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown** ppIScope, mdTypeDef* ptd)
{
	DEBUG_PRINTF(1, "CordbSymbol - ResolveTypeRef - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumMembers(                 // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdToken     rMembers[],             // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMembers - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumMembersWithName(         // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
LPCWSTR     szName,                 // [IN] Limit results to those with this name.
mdToken     rMembers[],             // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMembersWithName - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumMethods(                 // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdMethodDef rMethods[],             // [OUT] Put MethodDefs here.
ULONG       cMax,                   // [IN] Max MethodDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - %d - %d\n", cl, this->pCordbAssembly->id);
	if (*phEnum == NULL)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, cl);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);

		ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE (received_reply_packet);
		Buffer *localbuf2 = received_reply_packet->buf;

		int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - 1\n");
		
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, klass_id);

		DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - 1.1 - %d\n", klass_id);

		cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_METHODS, &localbuf);
		buffer_free(&localbuf);
		localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
		int num_methods = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		GPtrArray *methods = g_ptr_array_new();
		DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - 2 - NUMMETHODS - %d - cl - %d\n", num_methods, cl);
		for (int i = 0; i < num_methods; i++) {
			int* method_id = new int;
			*method_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			*method_id = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - 3 -  %d\n", *method_id);
			g_ptr_array_add(methods, method_id);
		}
		HENUMInternal* enumInternal = new HENUMInternal();
		enumInternal->items = methods;
		enumInternal->currentIdx = 0;
		*phEnum = enumInternal;
	}
	HENUMInternal* enumInternal = static_cast<HENUMInternal*>(*phEnum);
	for (int i = 0; i < cMax; i++) {
		DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - to fazendo o loop - %d - %d\n", enumInternal->currentIdx, enumInternal->items->len);
		if (enumInternal->currentIdx >= enumInternal->items->len) {
			*pcTokens = 0;
			return S_FALSE;
		}
		rMethods[i] = *(int*)(g_ptr_array_index(enumInternal->items, enumInternal->currentIdx));
		DEBUG_PRINTF(1, "CordbSymbol - EnumMethod - to fazendo o loop - %d\n", rMethods[i]);
		enumInternal->currentIdx++;
	}
	*pcTokens = cMax;
	return S_OK;
}

HRESULT CordbSymbol::EnumMethodsWithName(         // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
LPCWSTR     szName,                 // [IN] Limit results to those with this name.
mdMethodDef rMethods[],             // [OU] Put MethodDefs here.
ULONG       cMax,                   // [IN] Max MethodDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMethodsWithName - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumFields(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdFieldDef  rFields[],              // [OUT] Put FieldDefs here.
ULONG       cMax,                   // [IN] Max FieldDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumFields - %d - %d\n", cl, this->pCordbAssembly->id);
	if (*phEnum == NULL)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, cl);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE (received_reply_packet);
		Buffer *localbuf2 = received_reply_packet->buf;
		int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		DEBUG_PRINTF(1, "CordbSymbol - EnumFields - 1\n");
		
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, klass_id);

		DEBUG_PRINTF(1, "CordbSymbol - EnumFields - 1.1 - %d\n", klass_id);

		cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_FIELDS, &localbuf);
		buffer_free(&localbuf);
		localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
		int num_fields = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		GPtrArray *fields = g_ptr_array_new();
		for (int i = 0; i < num_fields; i++) {
			DEBUG_PRINTF(1, "CordbSymbol - EnumFields - 2\n");
			int* field_id = new int;
			*field_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			*field_id = mono_metadata_make_token(MONO_TABLE_FIELD, *field_id);
			char *field_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			DEBUG_PRINTF(1, "CordbSymbol - EnumFields - 3 -  %d - %s\n", *field_id, field_name);
			g_ptr_array_add(fields, field_id);
		}
		HENUMInternal* enumInternal = new HENUMInternal();
		enumInternal->items = fields;
		enumInternal->currentIdx = 0;
		*phEnum = enumInternal;
	}
	HENUMInternal* enumInternal = static_cast<HENUMInternal*>(*phEnum);
	for (int i = 0; i < cMax; i++) {
		DEBUG_PRINTF(1, "CordbSymbol - EnumFields - to fazendo o loop - %d - %d\n", enumInternal->currentIdx, enumInternal->items->len);
		if (enumInternal->currentIdx >= enumInternal->items->len) {
			*pcTokens = i;
			return S_OK;
		}
		rFields[i] = *(int*)(g_ptr_array_index(enumInternal->items, enumInternal->currentIdx));
		DEBUG_PRINTF(1, "CordbSymbol - EnumFields - to fazendo o loop - %d\n", rFields[i]);
		enumInternal->currentIdx++;
	}
	*pcTokens = cMax;
	return S_OK;
}

HRESULT CordbSymbol::EnumFieldsWithName(          // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
LPCWSTR     szName,                 // [IN] Limit results to those with this name.
mdFieldDef  rFields[],              // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumFieldsWithName - NOT IMPLEMENTED\n");
	return S_OK;
}


HRESULT CordbSymbol::EnumParams(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
mdParamDef  rParams[],              // [OUT] Put ParamDefs here.
ULONG       cMax,                   // [IN] Max ParamDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumParams - %d - %d\n", mb, this->pCordbAssembly->id);
	if (*phEnum == NULL)
	{
		int method_id;
		if (findMethodByToken(mb, method_id) != S_OK)
			return S_FALSE;

		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, method_id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_PARAM_INFO, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE (received_reply_packet);
		Buffer *localbuf2 = received_reply_packet->buf;
		int call_convention = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int param_count = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int generic_param_count = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int ret_type = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		GPtrArray *params = g_ptr_array_new();
		for (int i = 0; i < param_count; i++) {
			int *param_id = new int;
			*param_id = mono_atomic_inc_i32(&token_id) | ID_PARAMETER;
			int type_id = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			char *param_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			DEBUG_PRINTF(1, "CordbSymbol - EnumParams - 3 -  %d - %s\n", *param_id, param_name);
			g_ptr_array_add(params, param_id);
			CordbParameter* parm = new CordbParameter(type_id, param_name, mb);
			g_hash_table_insert (parameters, GINT_TO_POINTER(*param_id), parm);
		}

		HENUMInternal* enumInternal = new HENUMInternal();
		enumInternal->items = params;
		enumInternal->currentIdx = 0;
		*phEnum = enumInternal;
	}
	HENUMInternal* enumInternal = static_cast<HENUMInternal*>(*phEnum);
	for (int i = 0; i < cMax; i++) {
		DEBUG_PRINTF(1, "CordbSymbol - EnumParams - to fazendo o loop - %d - %d\n", enumInternal->currentIdx, enumInternal->items->len);
		if (enumInternal->currentIdx >= enumInternal->items->len) {
			*pcTokens = i;
			return S_OK;
		}
		rParams[i] = *(int*)(g_ptr_array_index(enumInternal->items, enumInternal->currentIdx));
		DEBUG_PRINTF(1, "CordbSymbol - EnumParams - to fazendo o loop - %d\n", rParams[i]);
		enumInternal->currentIdx++;
	}
	*pcTokens = cMax;
	return S_OK;
}

HRESULT CordbSymbol::EnumMemberRefs(              // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdToken     tkParent,               // [IN] Parent token to scope the enumeration.
mdMemberRef rMemberRefs[],          // [OUT] Put MemberRefs here.
ULONG       cMax,                   // [IN] Max MemberRefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMemberRefs - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumMethodImpls(             // S_OK, S_FALSE, or error
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
mdToken     rMethodBody[],          // [OUT] Put Method Body tokens here.
mdToken     rMethodDecl[],          // [OUT] Put Method Declaration tokens here.
ULONG       cMax,                   // [IN] Max tokens to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMethodImpls - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumPermissionSets(          // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdToken     tk,                     // [IN] if !NIL, token to scope the enumeration.
DWORD       dwActions,              // [IN] if !0, return only these actions.
mdPermission rPermission[],         // [OUT] Put Permissions here.
ULONG       cMax,                   // [IN] Max Permissions to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumPermissionSets - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindMember(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdToken* pmb)             // [OUT] matching memberdef
{
	DEBUG_PRINTF(1, "CordbSymbol - FindMember - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindMethod(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdMethodDef* pmb)             // [OUT] matching memberdef
{
	DEBUG_PRINTF(1, "CordbSymbol - FindMethod - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindField(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdFieldDef* pmb)             // [OUT] matching memberdef
{
	DEBUG_PRINTF(1, "CordbSymbol - FindField - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindMemberRef(
mdTypeRef   td,                     // [IN] given typeRef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdMemberRef* pmr)             // [OUT] matching memberref
{
	DEBUG_PRINTF(1, "CordbSymbol - FindMemberRef - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetMethodProps(
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
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - 1 - %d\n", mb);
	int method_id;
	if (findMethodByToken(mb, method_id) != S_OK)
		return S_FALSE;
		

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, method_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_NAME, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char* method_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	if (!stricmp(method_name, "localMethod"))
	{
		DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - 2 - %s\n", method_name);
	}
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - 2 - %s\n", method_name);
	if (cchMethod > strlen(method_name)) {
		mbstowcs(szMethod, method_name, strlen(method_name) + 1);
	}
	if (pchMethod)
		*pchMethod = strlen(method_name) + 1;

	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - 2\n");

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, method_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_DECLARING_TYPE, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);
	
	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_fullname_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*pClass = token;
		
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - 3\n");
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, method_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_SIGNATURE, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
	int signature_len = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - SIGNATURE_LEN - %d\n", signature_len);
	COR_SIGNATURE *tempSignature = new COR_SIGNATURE[signature_len];
	for (int i = 0 ; i < signature_len; i++) {
		tempSignature[i] = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}

	if (ppvSigBlob) {
		DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - TINHA - ppvSigBlob\n");
		*ppvSigBlob = new COR_SIGNATURE[signature_len];
		memcpy((void*)*ppvSigBlob, tempSignature, signature_len * sizeof(COR_SIGNATURE));
	}

	if (pcbSigBlob) {
		DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - TINHA - pcbSigBlob\n");
	 	*pcbSigBlob = signature_len;
	}

	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, method_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_RVA_FLAGS, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	int rva = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int flags = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	if (pdwAttr) 
		*pdwAttr = flags;
	if (pulCodeRVA)
	{
		*pulCodeRVA = rva;
	}
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - rva - %d\n", rva);
	return S_OK;
}

HRESULT CordbSymbol::findMethodByToken(mdMethodDef mb, int &func_id)
{
	CordbFunction* func = this->pCordbAssembly->pProcess->cordb->findFunctionByToken(mb);
	if (!func)
	{
		DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - NAO TINHA FUNC\n");
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, mb);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);

		ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE(received_reply_packet);
		Buffer* localbuf2 = received_reply_packet->buf;
		func_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	}
	else {
		DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - TINHA FUNC\n");
		func_id = func->id;
	}
	return S_OK;
}


HRESULT CordbSymbol::findTypeByToken(mdToken mb, int& type_id)
{
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodProps - IMPLEMENTED - NAO TINHA FUNC\n");
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	buffer_add_int(&localbuf, mb);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
	CHECK_ERROR_RETURN_FALSE(received_reply_packet);
	Buffer* localbuf2 = received_reply_packet->buf;
	type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	return S_OK;
}


HRESULT CordbSymbol::GetMemberRefProps(           // S_OK or error.
mdMemberRef mr,                     // [IN] given memberref
mdToken* ptk,                   // [OUT] Put classref or classdef here.
__out_ecount_part_opt(cchMember, *pchMember)
LPWSTR      szMember,               // [OUT] buffer to fill for member's name
ULONG       cchMember,              // [IN] the count of char of szMember
ULONG* pchMember,             // [OUT] actual count of char in member name
PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to meta data blob value
ULONG* pbSig)           // [OUT] actual size of signature blob
{
	DEBUG_PRINTF(1, "CordbSymbol - GetMemberRefProps - IMPLEMENTED - %d\n", mr);

	CordbFunction* func = this->pCordbAssembly->pProcess->cordb->findFunctionByToken(mr);
	int func_id = 0;
	if (!func)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, mr);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);

		Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

		func_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	}
	else
		func_id = func->id;

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, func_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_NAME, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char *method_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	DEBUG_PRINTF(1, "CordbSymbol - GetMemberRefProps - IMPLEMENTED - 2 - %s\n", method_name);

	if (cchMember > strlen(method_name)) {
		mbstowcs(szMember, method_name, strlen(method_name) + 1);
	}
	if (pchMember)
		*pchMember = strlen(method_name) + 1;

	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	buffer_add_int(&localbuf, mr);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_INFO_FROM_MEMBERREF_TOKEN, &localbuf);
	buffer_free(&localbuf);
	localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	int blob_len = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	COR_SIGNATURE *ppvSigLocal = new COR_SIGNATURE[blob_len];
	DEBUG_PRINTF(1, "CordbSymbol - GetMemberRefProps - IMPLEMENTED - blob_len - %d\n", blob_len);
	for (int i = 0 ; i < blob_len; i++) {
		ppvSigLocal[i] = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		DEBUG_PRINTF(1, "CordbSymbol - GetMemberRefProps - IMPLEMENTED - %d - %d\n", i, ppvSigLocal[i]);
	}
	if (ptk) {
		*ptk = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}

	if (pbSig) {
		*ppvSigBlob = new COR_SIGNATURE[blob_len];
		memcpy((void*)*ppvSigBlob, ppvSigLocal, blob_len * sizeof(COR_SIGNATURE));
		*pbSig = blob_len;
	}
	

	return S_OK;
}

HRESULT CordbSymbol::EnumProperties(              // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
mdProperty  rProperties[],          // [OUT] Put Properties here.
ULONG       cMax,                   // [IN] Max properties to put.
ULONG* pcProperties)    // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - %d - %d\n", td, this->pCordbAssembly->id);
	if (*phEnum == NULL)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, td);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);
		
		ReceivedReplyPacket *received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE (received_reply_packet);
		Buffer *localbuf2 = received_reply_packet->buf;
		
		int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - 1\n");
		
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, klass_id);

		DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - 1.1 - %d\n", klass_id);

		cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_PROPERTIES, &localbuf);
		buffer_free(&localbuf);
		localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);
		int num_properties = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		GPtrArray *properties = g_ptr_array_new();
		DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - 2 - NUMPROPERTIES - %d - cl - %d\n", num_properties, td);
		for (int i = 0; i < num_properties; i++) {
			int* property_id = new int;
			*property_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

			char* prop_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			int method_get = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			int method_set = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			int prop_attr = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

			*property_id = mono_metadata_make_token(MONO_TABLE_PROPERTY, *property_id);
			DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - 3 -  %d - %s\n", *property_id, prop_name);
			g_ptr_array_add(properties, property_id);
		}
		HENUMInternal* enumInternal = new HENUMInternal();
		enumInternal->items = properties;
		enumInternal->currentIdx = 0;
		*phEnum = enumInternal;
	}
	HENUMInternal* enumInternal = static_cast<HENUMInternal*>(*phEnum);
	for (int i = 0; i < cMax; i++) {
		DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - to fazendo o loop - %d - %d\n", enumInternal->currentIdx, enumInternal->items->len);
		if (enumInternal->currentIdx >= enumInternal->items->len) {
			*pcProperties = 0;
			return S_FALSE;
		}
		rProperties[i] = *(int*)(g_ptr_array_index(enumInternal->items, enumInternal->currentIdx));
		DEBUG_PRINTF(1, "CordbSymbol - EnumProperties - to fazendo o loop - %d\n", rProperties[i]);
		enumInternal->currentIdx++;
	}
	*pcProperties = cMax;
	return S_OK;
}

HRESULT CordbSymbol::EnumEvents(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
mdEvent     rEvents[],              // [OUT] Put events here.
ULONG       cMax,                   // [IN] Max events to put.
ULONG* pcEvents)        // [OUT] Put # put here.
{
	*pcEvents = 0;
	DEBUG_PRINTF(1, "CordbSymbol - EnumEvents - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetEventProps(               // S_OK, S_FALSE, or error.
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
	DEBUG_PRINTF(1, "CordbSymbol - GetEventProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumMethodSemantics(         // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
mdToken     rEventProp[],           // [OUT] Put Event/Property here.
ULONG       cMax,                   // [IN] Max properties to put.
ULONG* pcEventProp)     // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumMethodSemantics - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetMethodSemantics(          // S_OK, S_FALSE, or error.
mdMethodDef mb,                     // [IN] method token
mdToken     tkEventProp,            // [IN] event/property token.
DWORD* pdwSemanticsFlags) // [OUT] the role flags for the method/propevent pair
{
	DEBUG_PRINTF(1, "CordbSymbol - GetMethodSemantics - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetClassLayout(
mdTypeDef   td,                     // [IN] give typedef
DWORD* pdwPackSize,           // [OUT] 1, 2, 4, 8, or 16
COR_FIELD_OFFSET rFieldOffset[],    // [OUT] field offset array
ULONG       cMax,                   // [IN] size of the array
ULONG* pcFieldOffset,         // [OUT] needed array size
ULONG* pulClassSize)        // [OUT] the size of the class
{
	DEBUG_PRINTF(1, "CordbSymbol - GetClassLayout - NOT IMPLEMENTED\n");

	return 0x80131130;
}

HRESULT CordbSymbol::GetFieldMarshal(
mdToken     tk,                     // [IN] given a field's memberdef
PCCOR_SIGNATURE* ppvNativeType,     // [OUT] native type of this field
ULONG* pcbNativeType)   // [OUT] the count of bytes of *ppvNativeType
{
	DEBUG_PRINTF(1, "CordbSymbol - GetFieldMarshal - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetRVA(                      // S_OK or error.
mdToken     tk,                     // Member for which to set offset
ULONG* pulCodeRVA,            // The offset
DWORD* pdwImplFlags)    // the implementation flags
{
	DEBUG_PRINTF(1, "CordbSymbol - GetRVA - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetPermissionSetProps(
mdPermission pm,                    // [IN] the permission token.
DWORD* pdwAction,             // [OUT] CorDeclSecurity.
void const** ppvPermission,        // [OUT] permission blob.
ULONG* pcbPermission)   // [OUT] count of bytes of pvPermission.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetPermissionSetProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetSigFromToken(             // S_OK or error.
mdSignature mdSig,                  // [IN] Signature token.
PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to token.
ULONG* pcbSig)          // [OUT] return size of signature.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetSigFromToken - IMPLEMENTED - %d\n", mdSig);

	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	buffer_add_int(&localbuf, mdSig);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_SIGNATURE_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);
	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	//COR_SIGNATURE tempSignature[9] = { 7, 6, 10, 8, 3, 2, 14, ELEMENT_TYPE_CLASS, 8 }; // segundo parametro quantas locais, terceiro em diante os tipos
	if (ppvSig)
	{
		int blob_len = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		COR_SIGNATURE *ppvSigLocal = new COR_SIGNATURE[blob_len];
		int sig_type = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int num_locals = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		ppvSigLocal[0] = sig_type;
		ppvSigLocal[1] = num_locals;
		DEBUG_PRINTF(1, "CordbSymbol - GetSigFromToken - IMPLEMENTED - blob_len - %d\n", blob_len);
		DEBUG_PRINTF(1, "CordbSymbol - GetSigFromToken - IMPLEMENTED - num_locals - %d\n", num_locals);
		for (int i = 2 ; i < blob_len; i++) {
			ppvSigLocal[i] = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			DEBUG_PRINTF(1, "CordbSymbol - GetSigFromToken - IMPLEMENTED - %d - %d\n", i, ppvSigLocal[i]);
		}
		*ppvSig = new COR_SIGNATURE[blob_len];
		memcpy((void*)*ppvSig, ppvSigLocal, blob_len * sizeof(COR_SIGNATURE));
		*pcbSig = blob_len;
	}
	return S_OK;
}

HRESULT CordbSymbol::GetModuleRefProps(           // S_OK or error.
mdModuleRef mur,                    // [IN] moduleref token.
__out_ecount_part_opt(cchName, *pchName)
LPWSTR      szName,                 // [OUT] buffer to fill with the moduleref name.
ULONG       cchName,                // [IN] size of szName in wide characters.
ULONG* pchName)         // [OUT] actual count of characters in the name.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetModuleRefProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumModuleRefs(              // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdModuleRef rModuleRefs[],          // [OUT] put modulerefs here.
ULONG       cmax,                   // [IN] max memberrefs to put.
ULONG* pcModuleRefs)    // [OUT] put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumModuleRefs - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetTypeSpecFromToken(        // S_OK or error.
mdTypeSpec typespec,                // [IN] TypeSpec token.
PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to TypeSpec signature
ULONG* pcbSig)          // [OUT] return size of signature.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetTypeSpecFromToken - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetNameFromToken(            // Not Recommended! May be removed!
mdToken     tk,                     // [IN] Token to get name from.  Must have a name.
MDUTF8CSTR* pszUtf8NamePtr)  // [OUT] Return pointer to UTF8 name in heap.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetNameFromToken - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumUnresolvedMethods(       // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdToken     rMethods[],             // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumUnresolvedMethods - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetUserString(               // S_OK or error.
mdString    stk,                    // [IN] String token.
__out_ecount_part_opt(cchString, *pchString)
LPWSTR      szString,               // [OUT] Copy of string.
ULONG       cchString,              // [IN] Max chars of room in szString.
ULONG* pchString)       // [OUT] How many chars in actual string.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetUserString - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetPinvokeMap(               // S_OK or error.
mdToken     tk,                     // [IN] FieldDef or MethodDef.
DWORD* pdwMappingFlags,       // [OUT] Flags used for mapping.
__out_ecount_part_opt(cchImportName, *pchImportName)
LPWSTR      szImportName,           // [OUT] Import name.
ULONG       cchImportName,          // [IN] Size of the name buffer.
ULONG* pchImportName,         // [OUT] Actual number of characters stored.
mdModuleRef* pmrImportDLL)    // [OUT] ModuleRef token for the target DLL.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetPinvokeMap - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumSignatures(              // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdSignature rSignatures[],          // [OUT] put signatures here.
ULONG       cmax,                   // [IN] max signatures to put.
ULONG* pcSignatures)    // [OUT] put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumSignatures - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeSpecs(               // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdTypeSpec  rTypeSpecs[],           // [OUT] put TypeSpecs here.
ULONG       cmax,                   // [IN] max TypeSpecs to put.
ULONG* pcTypeSpecs)     // [OUT] put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumTypeSpecs - NOT IMPLEMENTED\n");
	return S_OK;
}


HRESULT CordbSymbol::EnumUserStrings(             // S_OK or error.
HCORENUM* phEnum,                // [IN/OUT] pointer to the enum.
mdString    rStrings[],             // [OUT] put Strings here.
ULONG       cmax,                   // [IN] max Strings to put.
ULONG* pcStrings)       // [OUT] put # put here.
{
	DEBUG_PRINTF(1, "CordbSymbol - EnumUserStrings - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetParamForMethodIndex(      // S_OK or error.
mdMethodDef md,                     // [IN] Method token.
ULONG       ulParamSeq,             // [IN] Parameter sequence.
mdParamDef* ppd)             // [IN] Put Param token here.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetParamForMethodIndex - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::EnumCustomAttributes(        // S_OK or error.
HCORENUM* phEnum,                // [IN, OUT] COR enumerator.
mdToken     tk,                     // [IN] Token to scope the enumeration, 0 for all.
mdToken     tkType,                 // [IN] Type of interest, 0 for all.
mdCustomAttribute rCustomAttributes[], // [OUT] Put custom attribute tokens here.
ULONG       cMax,                   // [IN] Size of rCustomAttributes.
ULONG* pcCustomAttributes)  // [OUT, OPTIONAL] Put count of token values here.
{
	*pcCustomAttributes = 0;
	DEBUG_PRINTF(1, "CordbSymbol - EnumCustomAttributes - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetCustomAttributeProps(     // S_OK or error.
mdCustomAttribute cv,               // [IN] CustomAttribute token.
mdToken* ptkObj,                // [OUT, OPTIONAL] Put object token here.
mdToken* ptkType,               // [OUT, OPTIONAL] Put AttrType token here.
void const** ppBlob,               // [OUT, OPTIONAL] Put pointer to data here.
ULONG* pcbSize)         // [OUT, OPTIONAL] Put size of date here.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetCustomAttributeProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::FindTypeRef(
mdToken     tkResolutionScope,      // [IN] ModuleRef, AssemblyRef or TypeRef.
LPCWSTR     szName,                 // [IN] TypeRef Name.
mdTypeRef* ptr)             // [OUT] matching TypeRef.
{
	DEBUG_PRINTF(1, "CordbSymbol - FindTypeRef - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetMemberProps(
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
	DEBUG_PRINTF(1, "CordbSymbol - GetMemberProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetFieldProps(
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
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, mono_metadata_token_index(mb));
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_FIELD, CMD_FIELD_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char *field_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int attrs = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int parent_token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int klass_token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	
	if (ppvSigBlob || pcbSigBlob) {
		if (type == ELEMENT_TYPE_CLASS) 
		{
			COR_SIGNATURE tempSignature[3];
			*pcbSigBlob = 3;
			tempSignature[0] = attrs;
			tempSignature[1] = type;
			tempSignature[2] = 8; //???
			*ppvSigBlob = new COR_SIGNATURE[3];
			memcpy((void*)*ppvSigBlob, tempSignature, 3 * sizeof(COR_SIGNATURE));
		}
		else
		{
			COR_SIGNATURE tempSignature[2];
			*pcbSigBlob = 2;
			tempSignature[0] = attrs;
			tempSignature[1] = type;
			*ppvSigBlob = new COR_SIGNATURE[2];
			memcpy((void*)*ppvSigBlob, tempSignature, 2 * sizeof(COR_SIGNATURE));
		}
		*pdwCPlusTypeFlag = 1;
		*pdwAttr = 1;
	}

	DEBUG_PRINTF(1, "CordbSymbol - GetFieldProps - IMPLEMENTED, %s, %d, %d, %d\n", field_name, attrs, type, klass_id);

	*pClass = parent_token;
	if (cchField > strlen(field_name)) {
		DEBUG_PRINTF(1, "CordbSymbol - GetFieldProps - FIZ COPIA DO NOME - IMPLEMENTED, %s, %d, %d\n", field_name, attrs, type);
		mbstowcs(szField, field_name, strlen(field_name) + 1);
		*pchField = strlen(field_name);
	}
	
	return S_OK;
}

HRESULT CordbSymbol::GetPropertyProps(            // S_OK, S_FALSE, or error.
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
	DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED, %d - %d\n", prop, this->pCordbAssembly->id);
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, mono_metadata_token_index(prop));
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_PROPERTY, CMD_PROPERTY_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = pCordbAssembly->pProcess->connection->get_answer(cmdId);

	char *property_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED, %s\n", property_name);
	*pmdSetter = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED, %d\n", *pmdSetter);

	*pmdGetter = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED, %d\n", *pmdGetter);		

	if (pchProperty)
		*pchProperty = strlen(property_name);

	DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED, %s\n", property_name);

	if (cchProperty > strlen(property_name)) {
		DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - FIZ COPIA DO NOME - IMPLEMENTED, %s\n", property_name);
		mbstowcs((LPWSTR)szProperty, property_name, strlen(property_name) + 1);
	}

	*pClass = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	if (ppvSig || pbSig) {
		int blob_len = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		COR_SIGNATURE *ppvSigLocal = new COR_SIGNATURE[blob_len];
		DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED - blob_len - %d\n", blob_len);
		for (int i = 0 ; i < blob_len; i++) {
			ppvSigLocal[i] = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
			DEBUG_PRINTF(1, "CordbSymbol - GetPropertyProps - IMPLEMENTED - %d - %d\n", i, ppvSigLocal[i]);
		}
		if (pbSig) {
			*ppvSig = new COR_SIGNATURE[blob_len];
			memcpy((void*)*ppvSig, ppvSigLocal, blob_len * sizeof(COR_SIGNATURE));
			*pbSig = blob_len;
		}
	}
	
	return S_OK;
}

HRESULT CordbSymbol::GetParamProps(               // S_OK or error.
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
	CordbParameter *ret = (CordbParameter*)g_hash_table_lookup(parameters, GINT_TO_POINTER(tk));
	DEBUG_PRINTF(1, "CordbSymbol - GetParamProps - IMPLEMENTED - %s\n", ret->name);
	*pmd = ret->method_token;
	if (cchName >= strlen(ret->name)) {
		mbstowcs(szName, ret->name, strlen(ret->name) + 1);
	}
	*pchName = strlen(ret->name)+1;
	*pdwCPlusTypeFlag = ret->type;
	return S_OK;
}

HRESULT CordbSymbol::GetAssemblyFromScope(mdAssembly* ptkAssembly)
{
	*ptkAssembly = mono_metadata_make_token(MONO_TABLE_ASSEMBLY, pCordbAssembly->id);
	DEBUG_PRINTF(1, "CordbSymbol - GetAssemblyFromScope - IMPLEMENTED - id\n");
	return S_OK;
}

HRESULT CordbSymbol::GetCustomAttributeByName(    // S_OK or error.
mdToken     tkObj,                  // [IN] Object with Custom Attribute.
LPCWSTR     szName,                 // [IN] Name of desired Custom Attribute.
const void** ppData,               // [OUT] Put pointer to data here.
ULONG* pcbData)         // [OUT] Put size of data here.
{
	int custom_attribute_id;
	int method_type_id;
	int type = mono_metadata_token_table(tkObj);
	Buffer* localbuf2 = NULL;
	DEBUG_PRINTF(1, "CordbSymbol - GetCustomAttributeByName - IMPLEMENTED\n");

	if (FindTypeDefByNameInternal(szName, custom_attribute_id) != S_OK)
		return S_FALSE;
	if (type == MONO_TABLE_ASSEMBLY) {
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, mono_metadata_token_index(tkObj));
		buffer_add_int(&localbuf, custom_attribute_id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_CATTRS, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE(received_reply_packet);
		localbuf2 = received_reply_packet->buf;
	}
	else if (type == MONO_TABLE_METHOD && findMethodByToken(tkObj, method_type_id) == S_OK){
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, method_type_id);
		buffer_add_int(&localbuf, custom_attribute_id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_CATTRS, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE(received_reply_packet);
		localbuf2 = received_reply_packet->buf;
	}
	else if (type == MONO_TABLE_TYPEDEF && findTypeByToken(tkObj, method_type_id) == S_OK) {
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_int(&localbuf, method_type_id);
		buffer_add_int(&localbuf, custom_attribute_id);
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_CATTRS, &localbuf);
		buffer_free(&localbuf);
		ReceivedReplyPacket* received_reply_packet = pCordbAssembly->pProcess->connection->get_answer_with_error(cmdId);
		CHECK_ERROR_RETURN_FALSE(received_reply_packet);
		localbuf2 = received_reply_packet->buf;
	}

	int cinfo = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	if (cinfo == 0)
		return S_FALSE;

	DEBUG_PRINTF(1, "CordbSymbol - GetCustomAttributeByName - IMPLEMENTED\n");

	if (!wcscmp(szName, L"System.Diagnostics.DebuggerNonUserCodeAttribute") || !wcscmp(szName, L"System.Diagnostics.DebuggerStepThroughAttribute"))
		return S_FALSE;
	if (!wcscmp(szName, L"System.Diagnostics.DebuggerHiddenAttribute"))
		return S_FALSE;
	if (!wcscmp(szName, L"System.Runtime.CompilerServices.AsyncStateMachineAttribute"))
		return S_FALSE;
	if (!wcscmp(szName, L"System.Diagnostics.DebuggerStepperBoundaryAttribute"))
		return S_FALSE;

	byte *ret = new byte[6];
	ret[0] = 1;
	ret[1] = 0;
	ret[2] = 7;
	ret[3] = 1;
	ret[4] = 0;
	ret[5] = 0;
	*ppData = ret;
	*pcbData = 6;
	return S_OK;
}

BOOL CordbSymbol::IsValidToken(         // True or False.
mdToken     tk)               // [IN] Given token.
{
	DEBUG_PRINTF(1, "CordbSymbol - IsValidToken - IMPLEMENTED - %d\n", tk);
	return 1;
}

HRESULT CordbSymbol::GetNestedClassProps(         // S_OK or error.
mdTypeDef   tdNestedClass,          // [IN] NestedClass token.
mdTypeDef* ptdEnclosingClass) // [OUT] EnclosingClass token.
{
	DEBUG_PRINTF(1, "CordbSymbol - GetNestedClassProps - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::GetNativeCallConvFromSig(    // S_OK or error.
void const* pvSig,                 // [IN] Pointer to signature.
ULONG       cbSig,                  // [IN] Count of signature bytes.
ULONG* pCallConv)       // [OUT] Put calling conv here (see CorPinvokemap).
{
	DEBUG_PRINTF(1, "CordbSymbol - GetNativeCallConvFromSig - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbSymbol::IsGlobal(                    // S_OK or error.
mdToken     pd,                     // [IN] Type, Field, or Method token.
int* pbGlobal)        // [OUT] Put 1 if global, 0 otherwise.

{
	DEBUG_PRINTF(1, "CordbSymbol - IsGlobal - NOT IMPLEMENTED\n");
	return S_OK;
}

