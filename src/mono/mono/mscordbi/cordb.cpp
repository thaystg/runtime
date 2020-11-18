#define MONO_DLL_EXPORT

#include <cordb.hpp>


CordbSymbol::CordbSymbol(CordbAssembly* cordbAssembly)
{
	module_id = -1;
	pCordbAssembly = cordbAssembly;
}

HRESULT CordbSymbol::EnumGenericParams(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken      tk,                    // [IN] TypeDef or MethodDef whose generic parameters are requested
	mdGenericParam rGenericParams[],    // [OUT] Put GenericParams here.
	ULONG       cMax,                   // [IN] Max GenericParams to put.
	ULONG* pcGenericParams)
{
	printf("CordbSymbol - EnumGenericParams - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetGenericParamProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}       // [OUT] Put size of name (wide chars) here.

HRESULT CordbSymbol::GetMethodSpecProps(
	mdMethodSpec mi,                    // [IN] The method instantiation
	mdToken* tkParent,                  // [OUT] MethodDef or MemberRef
	PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
	ULONG* pcbSigBlob) {
	printf("CordbSymbol - GetMethodSpecProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}     // [OUT] actual size of signature blob

HRESULT CordbSymbol::EnumGenericParamConstraints(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdGenericParam tk,                  // [IN] GenericParam whose constraints are requested
	mdGenericParamConstraint rGenericParamConstraints[],    // [OUT] Put GenericParamConstraints here.
	ULONG       cMax,                   // [IN] Max GenericParamConstraints to put.
	ULONG* pcGenericParamConstraints) {
	printf("CordbSymbol - EnumGenericParamConstraints - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}// [OUT] Put # put here.

HRESULT CordbSymbol::GetGenericParamConstraintProps( // S_OK or error.
	mdGenericParamConstraint gpc,       // [IN] GenericParamConstraint
	mdGenericParam* ptGenericParam,     // [OUT] GenericParam that is constrained
	mdToken* ptkConstraintType) {
	printf("CordbSymbol - GetGenericParamConstraintProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}// [OUT] TypeDef/Ref/Spec constraint

HRESULT CordbSymbol::GetPEKind(                   // S_OK or error.
	DWORD* pdwPEKind,                   // [OUT] The kind of PE (0 - not a PE)
	DWORD* pdwMAchine) {
	printf("CordbSymbol - GetPEKind - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}           // [OUT] Machine as defined in NT header

HRESULT CordbSymbol::GetVersionString(            // S_OK or error.
	_Out_writes_to_opt_(ccBufSize, *pccBufSize)
	LPWSTR      pwzBuf,                 // [OUT] Put version string here.
	DWORD       ccBufSize,              // [IN] size of the buffer, in wide chars
	DWORD* pccBufSize) {
	printf("CordbSymbol - GetVersionString - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}     // [OUT] Size of the version string, wide chars, including terminating nul.

HRESULT CordbSymbol::EnumMethodSpecs(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken      tk,                    // [IN] MethodDef or MemberRef whose MethodSpecs are requested
	mdMethodSpec rMethodSpecs[],        // [OUT] Put MethodSpecs here.
	ULONG       cMax,                   // [IN] Max tokens to put.
	ULONG* pcMethodSpecs) {
	printf("CordbSymbol - EnumMethodSpecs - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetAssemblyProps - NOT IMPLEMENTED - %d - %d\n", cchName, mda);
	*pchName = 23;
	if (cchName >= 23)
	{
		mbstowcs(szName, "System.Private.CoreLib", strlen("System.Private.CoreLib") + 1);
	}
	fflush(stdout);
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
	printf("CordbSymbol - GetAssemblyRefProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetFileProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetExportedTypeProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetManifestResourceProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}// [OUT] Flags.

HRESULT CordbSymbol::EnumAssemblyRefs(            // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdAssemblyRef rAssemblyRefs[],      // [OUT] Put AssemblyRefs here.
	ULONG       cMax,                   // [IN] Max AssemblyRefs to put.
	ULONG* pcTokens) {
	printf("CordbSymbol - EnumAssemblyRefs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumFiles(                   // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdFile      rFiles[],               // [OUT] Put Files here.
	ULONG       cMax,                   // [IN] Max Files to put.
	ULONG* pcTokens) {
	printf("CordbSymbol - EnumFiles - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumExportedTypes(           // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdExportedType   rExportedTypes[],  // [OUT] Put ExportedTypes here.
	ULONG       cMax,                   // [IN] Max ExportedTypes to put.
	ULONG* pcTokens) {
	printf("CordbSymbol - EnumExportedTypes - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumManifestResources(       // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdManifestResource  rManifestResources[],   // [OUT] Put ManifestResources here.
	ULONG       cMax,                   // [IN] Max Resources to put.
	ULONG* pcTokens) {
	printf("CordbSymbol - EnumManifestResources - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::FindExportedTypeByName(      // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ExportedType.
	mdToken     mdtExportedType,        // [IN] ExportedType for the enclosing class.
	mdExportedType* ptkExportedType) {
	printf("CordbSymbol - FindExportedTypeByName - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}// [OUT] Put the ExportedType token here.

HRESULT CordbSymbol::FindManifestResourceByName(  // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ManifestResource.
	mdManifestResource* ptkManifestResource) {
	printf("CordbSymbol - FindManifestResourceByName - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
} // [OUT] Put the ManifestResource token here.

HRESULT CordbSymbol::FindAssembliesByName(        // S_OK or error
	LPCWSTR  szAppBase,                 // [IN] optional - can be NULL
	LPCWSTR  szPrivateBin,              // [IN] optional - can be NULL
	LPCWSTR  szAssemblyName,            // [IN] required - this is the assembly you are requesting
	IUnknown* ppIUnk[],                 // [OUT] put IMetaDataAssemblyImport pointers here
	ULONG    cMax,                      // [IN] The max number to put
	ULONG* pcAssemblies) {
	printf("CordbSymbol - FindAssembliesByName - NOT IMPLEMENTED\n");
	fflush(stdout);
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
					//((unsigned long*)&rguid1)[0]
					printf("CordbSymbol - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&riid)[0], ((unsigned long*)&riid)[1], ((unsigned long*)&riid)[2], ((unsigned long*)&riid)[3]);
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
	printf("CordbSymbol - CloseEnum - NOT IMPLEMENTED\n");
	fflush(stdout);
	return;
}

HRESULT CordbSymbol::CountEnum(HCORENUM hEnum, ULONG* pulCount)
{
	printf("CordbSymbol - CountEnum - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT CordbSymbol::ResetEnum(HCORENUM hEnum, ULONG ulPos)
{
	printf("CordbSymbol - ResetEnum - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbSymbol::EnumTypeDefs(HCORENUM* phEnum, mdTypeDef rTypeDefs[],
	ULONG cMax, ULONG* pcTypeDefs)
{
	printf("CordbSymbol - EnumTypeDefs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbSymbol::EnumInterfaceImpls(HCORENUM* phEnum, mdTypeDef td,
	mdInterfaceImpl rImpls[], ULONG cMax,
	ULONG* pcImpls)
{
	printf("CordbSymbol - EnumInterfaceImpls - NOT IMPLEMENTED\n");
	fflush(stdout);
	*pcImpls = 0;
	return S_OK;
}
	HRESULT CordbSymbol::EnumTypeRefs(HCORENUM* phEnum, mdTypeRef rTypeRefs[],
	ULONG cMax, ULONG* pcTypeRefs)
{
	printf("CordbSymbol - EnumTypeRefs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT CordbSymbol::FindTypeDefByName(           // S_OK or error.
LPCWSTR     szTypeDef,              // [IN] Name of the Type.
mdToken     tkEnclosingClass,       // [IN] TypeDef/TypeRef for Enclosing class.
mdTypeDef* ptd)             // [OUT] Put the TypeDef token here.
{

	Buffer sendbuf;
	buffer_init(&sendbuf, 128);

	char output[256];
	sprintf(output, "%ws", szTypeDef);

	buffer_add_string(&sendbuf, output);
	buffer_add_byte(&sendbuf, true);
	int cmdId = connection->send_event(CMD_SET_VM, CMD_VM_GET_TYPES, &sendbuf);

	buffer_free(&sendbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int count_class = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_fullname_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*ptd = token;

	printf("CordbSymbol - FindTypeDefByName - IMPLEMENTED - %d - %d - %s\n", count_class, token, class_name_str);
	fflush(stdout);
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
		int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_MANIFEST_MODULE, &localbuf);
		buffer_free(&localbuf);

		Buffer* localbuf2 = NULL;
		while (!localbuf2) {
			connection->process_packet(true);
			localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
		}

		module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}

	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, module_id);
	int cmdId = connection->send_event(CMD_SET_MODULE, CMD_MODULE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 1\n");
	fflush(stdout);
	char* basename = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 2\n");
	fflush(stdout);
	char* module_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 3\n");
	fflush(stdout);
	char* name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 4\n");
	fflush(stdout);
	char* guid = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 5\n");
	fflush(stdout);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 6\n");
	fflush(stdout);
	/*char* sourcelink = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);*/
	guint32 mvid_len;
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 7\n");
	fflush(stdout);
	guint8 *mvid = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &mvid_len);
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - 8\n");
	fflush(stdout);
	memcpy(pmvid, mvid, mvid_len);
	if (cchName > strlen(name))
	{
		mbstowcs(szName, name, strlen(name) + 1);
		*pchName = strlen(name) + 1;
	}
	printf("CordbSymbol - GetScopeProps - IMPLEMENTED - %s - %s - %d\n", name, guid, mvid_len);

	/*
	* 
	buffer_add_string (buf, basename); // name
	buffer_add_string (buf, image->module_name); // scopename
	buffer_add_string (buf, image->name); // fqname
	buffer_add_string (buf, mono_image_get_guid (image)); // guid
	buffer_add_assemblyid (buf, domain, image->assembly); // assembly
	if (CHECK_PROTOCOL_VERSION (2, 48))
		buffer_add_string (buf, sourcelink);
	buffer_add_byte_array(buf, mono_metadata_module_mvid(image));
	*/
	

	
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetModuleFromScope(          // S_OK.
	mdModule* pmd)            // [OUT] Put mdModule token here.
{
	printf("CordbSymbol - GetModuleFromScope - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	if (td == 33554532)
		return E_NOTIMPL;
	if (td == 0)
		return E_NOTIMPL;
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, this->pCordbAssembly->id);
	buffer_add_int(&localbuf, td);
	int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

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
	if (cchTypeDef > strlen(class_fullname_str)) {
		mbstowcs(szTypeDef, class_fullname_str, strlen(class_fullname_str) + 1);
	}
	if (pchTypeDef)
		*pchTypeDef = strlen(class_fullname_str) + 1;


	printf("CordbSymbol - GetTypeDefProps - IMPLEMENTED\n");
	fflush(stdout);
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, type_id);
	cmdId = connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	{
		char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		char* class_fullname_str2 = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int type_id3 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int rank = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int flags2 = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		if (pdwTypeDefFlags)
			*pdwTypeDefFlags = flags;
		if (ptkExtends)
			*ptkExtends = token;
	}
	return S_OK;
}

	HRESULT CordbSymbol::GetInterfaceImplProps(       // S_OK or error.
	mdInterfaceImpl iiImpl,             // [IN] InterfaceImpl token.
	mdTypeDef* pClass,                // [OUT] Put implementing class token here.
	mdToken* ptkIface)        // [OUT] Put implemented interface token here.

{
	printf("CordbSymbol - GetInterfaceImplProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetTypeRefProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown** ppIScope, mdTypeDef* ptd)
{
	printf("CordbSymbol - ResolveTypeRef - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumMembers(                 // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
	mdToken     rMembers[],             // [OUT] Put MemberDefs here.
	ULONG       cMax,                   // [IN] Max MemberDefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumMembers - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumMembersWithName - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumMethods(                 // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
	mdMethodDef rMethods[],             // [OUT] Put MethodDefs here.
	ULONG       cMax,                   // [IN] Max MethodDefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumMethods - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumMethodsWithName - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumFields(                  // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
	mdFieldDef  rFields[],              // [OUT] Put FieldDefs here.
	ULONG       cMax,                   // [IN] Max FieldDefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumFields - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumFieldsWithName - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbSymbol::EnumParams(                  // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
	mdParamDef  rParams[],              // [OUT] Put ParamDefs here.
	ULONG       cMax,                   // [IN] Max ParamDefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumParams - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumMemberRefs(              // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken     tkParent,               // [IN] Parent token to scope the enumeration.
	mdMemberRef rMemberRefs[],          // [OUT] Put MemberRefs here.
	ULONG       cMax,                   // [IN] Max MemberRefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumMemberRefs - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumMethodImpls - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumPermissionSets - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::FindMember(
	mdTypeDef   td,                     // [IN] given typedef
	LPCWSTR     szName,                 // [IN] member name
	PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
	ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
	mdToken* pmb)             // [OUT] matching memberdef

{
	printf("CordbSymbol - FindMember - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::FindMethod(
	mdTypeDef   td,                     // [IN] given typedef
	LPCWSTR     szName,                 // [IN] member name
	PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
	ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
	mdMethodDef* pmb)             // [OUT] matching memberdef

{
	printf("CordbSymbol - FindMethod - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::FindField(
	mdTypeDef   td,                     // [IN] given typedef
	LPCWSTR     szName,                 // [IN] member name
	PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
	ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
	mdFieldDef* pmb)             // [OUT] matching memberdef

{
	printf("CordbSymbol - FindField - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::FindMemberRef(
	mdTypeRef   td,                     // [IN] given typeRef
	LPCWSTR     szName,                 // [IN] member name
	PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
	ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
	mdMemberRef* pmr)             // [OUT] matching memberref

{
	printf("CordbSymbol - FindMemberRef - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	CordbFunction* func = this->pCordbAssembly->pProcess->cordb->findFunctionByToken(mb);
	int func_id = 0;
	if (!func)
	{
		Buffer localbuf;
		buffer_init(&localbuf, 128);
		buffer_add_id(&localbuf, this->pCordbAssembly->id);
		buffer_add_int(&localbuf, mb);
		int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);

		Buffer* localbuf2 = NULL;
		while (!localbuf2) {
			connection->process_packet(true);
			localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
		}

		func_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	}
	else
		func_id = func->id;

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, func_id);
	int cmdId = connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_DECLARING_TYPE, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);
	
	localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	char* namespace_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_name_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	char* class_fullname_str = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int type_id2 = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int token = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*pClass = token;
	if (cchMethod > strlen(class_name_str)) {
		mbstowcs(szMethod, class_name_str, strlen(class_name_str) + 1);
	}
	if (pchMethod)
		*pchMethod = strlen(class_name_str) + 1;
	COR_SIGNATURE tempSignature[4] = { 0, 1, 1, 8 };
	if (ppvSigBlob)
	{
		*ppvSigBlob = new COR_SIGNATURE[4];
		memcpy((void*)*ppvSigBlob, tempSignature, 4 * sizeof(COR_SIGNATURE));
		*pcbSigBlob = 4;
	}
	if (pulCodeRVA)
	{
		*pulCodeRVA = 8460;
	}
	printf("CordbSymbol - GetMethodProps - IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetMemberRefProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumProperties(              // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
	mdProperty  rProperties[],          // [OUT] Put Properties here.
	ULONG       cMax,                   // [IN] Max properties to put.
	ULONG* pcProperties)    // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumProperties - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumEvents(                  // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
	mdEvent     rEvents[],              // [OUT] Put events here.
	ULONG       cMax,                   // [IN] Max events to put.
	ULONG* pcEvents)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumEvents - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetEventProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumMethodSemantics(         // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
	mdToken     rEventProp[],           // [OUT] Put Event/Property here.
	ULONG       cMax,                   // [IN] Max properties to put.
	ULONG* pcEventProp)     // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumMethodSemantics - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetMethodSemantics(          // S_OK, S_FALSE, or error.
	mdMethodDef mb,                     // [IN] method token
	mdToken     tkEventProp,            // [IN] event/property token.
	DWORD* pdwSemanticsFlags) // [OUT] the role flags for the method/propevent pair

{
	printf("CordbSymbol - GetMethodSemantics - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetClassLayout - NOT IMPLEMENTED\n");
	fflush(stdout);
	return 0x80131130;
}

	HRESULT CordbSymbol::GetFieldMarshal(
	mdToken     tk,                     // [IN] given a field's memberdef
	PCCOR_SIGNATURE* ppvNativeType,     // [OUT] native type of this field
	ULONG* pcbNativeType)   // [OUT] the count of bytes of *ppvNativeType

{
	printf("CordbSymbol - GetFieldMarshal - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetRVA(                      // S_OK or error.
	mdToken     tk,                     // Member for which to set offset
	ULONG* pulCodeRVA,            // The offset
	DWORD* pdwImplFlags)    // the implementation flags

{
	printf("CordbSymbol - GetRVA - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetPermissionSetProps(
	mdPermission pm,                    // [IN] the permission token.
	DWORD* pdwAction,             // [OUT] CorDeclSecurity.
	void const** ppvPermission,        // [OUT] permission blob.
	ULONG* pcbPermission)   // [OUT] count of bytes of pvPermission.

{
	printf("CordbSymbol - GetPermissionSetProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetSigFromToken(             // S_OK or error.
	mdSignature mdSig,                  // [IN] Signature token.
	PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to token.
	ULONG* pcbSig)          // [OUT] return size of signature.

{
	printf("CordbSymbol - GetSigFromToken - NOT IMPLEMENTED - %d\n", mdSig);
	fflush(stdout);

	COR_SIGNATURE tempSignature[3] = { 7, 1, 8 };
	if (ppvSig)
	{
		*ppvSig = new COR_SIGNATURE[4];
		memcpy((void*)*ppvSig, tempSignature, 3 * sizeof(COR_SIGNATURE));
	}

	*pcbSig = 3;
	return S_OK;
}

	HRESULT CordbSymbol::GetModuleRefProps(           // S_OK or error.
	mdModuleRef mur,                    // [IN] moduleref token.
	__out_ecount_part_opt(cchName, *pchName)
	LPWSTR      szName,                 // [OUT] buffer to fill with the moduleref name.
	ULONG       cchName,                // [IN] size of szName in wide characters.
	ULONG* pchName)         // [OUT] actual count of characters in the name.

{
	printf("CordbSymbol - GetModuleRefProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumModuleRefs(              // S_OK or error.
	HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
	mdModuleRef rModuleRefs[],          // [OUT] put modulerefs here.
	ULONG       cmax,                   // [IN] max memberrefs to put.
	ULONG* pcModuleRefs)    // [OUT] put # put here.

{
	printf("CordbSymbol - EnumModuleRefs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetTypeSpecFromToken(        // S_OK or error.
	mdTypeSpec typespec,                // [IN] TypeSpec token.
	PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to TypeSpec signature
	ULONG* pcbSig)          // [OUT] return size of signature.

{
	printf("CordbSymbol - GetTypeSpecFromToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetNameFromToken(            // Not Recommended! May be removed!
	mdToken     tk,                     // [IN] Token to get name from.  Must have a name.
	MDUTF8CSTR* pszUtf8NamePtr)  // [OUT] Return pointer to UTF8 name in heap.

{
	printf("CordbSymbol - GetNameFromToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumUnresolvedMethods(       // S_OK, S_FALSE, or error.
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken     rMethods[],             // [OUT] Put MemberDefs here.
	ULONG       cMax,                   // [IN] Max MemberDefs to put.
	ULONG* pcTokens)        // [OUT] Put # put here.

{
	printf("CordbSymbol - EnumUnresolvedMethods - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetUserString(               // S_OK or error.
	mdString    stk,                    // [IN] String token.
	__out_ecount_part_opt(cchString, *pchString)
	LPWSTR      szString,               // [OUT] Copy of string.
	ULONG       cchString,              // [IN] Max chars of room in szString.
	ULONG* pchString)       // [OUT] How many chars in actual string.

{
	printf("CordbSymbol - GetUserString - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetPinvokeMap - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumSignatures(              // S_OK or error.
	HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
	mdSignature rSignatures[],          // [OUT] put signatures here.
	ULONG       cmax,                   // [IN] max signatures to put.
	ULONG* pcSignatures)    // [OUT] put # put here.
{
	printf("CordbSymbol - EnumSignatures - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::EnumTypeSpecs(               // S_OK or error.
	HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
	mdTypeSpec  rTypeSpecs[],           // [OUT] put TypeSpecs here.
	ULONG       cmax,                   // [IN] max TypeSpecs to put.
	ULONG* pcTypeSpecs)     // [OUT] put # put here.
{
	printf("CordbSymbol - EnumTypeSpecs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbSymbol::EnumUserStrings(             // S_OK or error.
	HCORENUM* phEnum,                // [IN/OUT] pointer to the enum.
	mdString    rStrings[],             // [OUT] put Strings here.
	ULONG       cmax,                   // [IN] max Strings to put.
	ULONG* pcStrings)       // [OUT] put # put here.
{
	printf("CordbSymbol - EnumUserStrings - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetParamForMethodIndex(      // S_OK or error.
	mdMethodDef md,                     // [IN] Method token.
	ULONG       ulParamSeq,             // [IN] Parameter sequence.
	mdParamDef* ppd)             // [IN] Put Param token here.
{
	printf("CordbSymbol - GetParamForMethodIndex - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - EnumCustomAttributes - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetCustomAttributeProps(     // S_OK or error.
	mdCustomAttribute cv,               // [IN] CustomAttribute token.
	mdToken* ptkObj,                // [OUT, OPTIONAL] Put object token here.
	mdToken* ptkType,               // [OUT, OPTIONAL] Put AttrType token here.
	void const** ppBlob,               // [OUT, OPTIONAL] Put pointer to data here.
	ULONG* pcbSize)         // [OUT, OPTIONAL] Put size of date here.
{
	printf("CordbSymbol - GetCustomAttributeProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::FindTypeRef(
	mdToken     tkResolutionScope,      // [IN] ModuleRef, AssemblyRef or TypeRef.
	LPCWSTR     szName,                 // [IN] TypeRef Name.
	mdTypeRef* ptr)             // [OUT] matching TypeRef.
{
	printf("CordbSymbol - FindTypeRef - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetMemberProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetFieldProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetPropertyProps - NOT IMPLEMENTED\n");
	fflush(stdout);
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
	printf("CordbSymbol - GetParamProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetCustomAttributeByName(    // S_OK or error.
	mdToken     tkObj,                  // [IN] Object with Custom Attribute.
	LPCWSTR     szName,                 // [IN] Name of desired Custom Attribute.
	const void** ppData,               // [OUT] Put pointer to data here.
	ULONG* pcbData)         // [OUT] Put size of data here.
{
	printf("CordbSymbol - GetCustomAttributeByName - IMPLEMENTED\n");
	fflush(stdout);
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
	ret[2] = 0;
	ret[3] = 0;
	ret[4] = 0;
	ret[5] = 0;
	*ppData = ret;
	*pcbData = 6;
	return S_OK;
}

BOOL CordbSymbol::IsValidToken(         // True or False.
mdToken     tk)               // [IN] Given token.
{
	printf("CordbSymbol - IsValidToken - IMPLEMENTED\n");
	fflush(stdout);
	return 1;
}

	HRESULT CordbSymbol::GetNestedClassProps(         // S_OK or error.
	mdTypeDef   tdNestedClass,          // [IN] NestedClass token.
	mdTypeDef* ptdEnclosingClass) // [OUT] EnclosingClass token.
{
	printf("CordbSymbol - GetNestedClassProps - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbSymbol::GetNativeCallConvFromSig(    // S_OK or error.
	void const* pvSig,                 // [IN] Pointer to signature.
	ULONG       cbSig,                  // [IN] Count of signature bytes.
	ULONG* pCallConv)       // [OUT] Put calling conv here (see CorPinvokemap).
{
	printf("CordbSymbol - GetNativeCallConvFromSig - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT CordbSymbol::IsGlobal(                    // S_OK or error.
mdToken     pd,                     // [IN] Type, Field, or Method token.
int* pbGlobal)        // [OUT] Put 1 if global, 0 otherwise.

{
	printf("CordbSymbol - IsGlobal - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


CordbAppDomain::CordbAppDomain(ICorDebugProcess* ppProcess)
{
	pProcess = ppProcess;
}

	HRESULT CordbAppDomain::Stop(
	/* [in] */ DWORD dwTimeoutIgnored)
{
	printf("CordbAppDomain - Stop - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::Continue(
	/* [in] */ BOOL fIsOutOfBand)
{
	printf("CordbAppDomain - Continue - NOT IMPLEMENTED\n");
	fflush(stdout);
	pProcess->Continue(fIsOutOfBand);
	return S_OK;
}

	HRESULT CordbAppDomain::IsRunning(
	/* [out] */ BOOL* pbRunning)
{
	printf("CordbAppDomain - IsRunning - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::HasQueuedCallbacks(
	/* [in] */ ICorDebugThread* pThread,
	/* [out] */ BOOL* pbQueued)
{
	printf("CordbAppDomain - HasQueuedCallbacks - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::EnumerateThreads(
	/* [out] */ ICorDebugThreadEnum** ppThreads)
{
	printf("CordbAppDomain - EnumerateThreads - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::SetAllThreadsDebugState(
	/* [in] */ CorDebugThreadState state,
	/* [in] */ ICorDebugThread* pExceptThisThread)
{
	printf("CordbAppDomain - SetAllThreadsDebugState - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::Detach(void)
{
	printf("CordbAppDomain - Detach - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::Terminate(
	/* [in] */ UINT exitCode)
{
	printf("CordbAppDomain - Terminate - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::CanCommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError)
{
	printf("CordbAppDomain - CanCommitChanges - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::CommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError)
{
	printf("CordbAppDomain - CommitChanges - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::QueryInterface(
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
		printf("CordbAppDomain - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		fflush(stdout);
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG CordbAppDomain::AddRef(void)
{
	return S_OK;
}

ULONG CordbAppDomain::Release(void)
{
	return S_OK;
}

HRESULT CordbAppDomain::GetProcess(
/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("CordbAppDomain - GetProcess - NOT IMPLEMENTED\n");
	fflush(stdout);
	*ppProcess = pProcess;
	return S_OK;
}

	HRESULT CordbAppDomain::EnumerateAssemblies(
	/* [out] */ ICorDebugAssemblyEnum** ppAssemblies)
{
	printf("CordbAppDomain - EnumerateAssemblies - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetModuleFromMetaDataInterface(
	/* [in] */ IUnknown* pIMetaData,
	/* [out] */ ICorDebugModule** ppModule)
{
	printf("CordbAppDomain - GetModuleFromMetaDataInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::EnumerateBreakpoints(
	/* [out] */ ICorDebugBreakpointEnum** ppBreakpoints)
{
	printf("CordbAppDomain - EnumerateBreakpoints - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::EnumerateSteppers(
	/* [out] */ ICorDebugStepperEnum** ppSteppers)
{
	printf("CordbAppDomain - EnumerateSteppers - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::IsAttached(
	/* [out] */ BOOL* pbAttached)
{
	printf("CordbAppDomain - IsAttached - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	printf("CordbAppDomain - GetName - IMPLEMENTED\n");
	if (cchName < strlen("DefaultDomain")) {
		*pcchName = strlen("DefaultDomain") + 1;
		return S_OK;
	}
	wcscpy(szName, L"DefaultDomain");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetObject(
	/* [out] */ ICorDebugValue** ppObject)
{
	printf("CordbAppDomain - GetObject - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::Attach(void)
{
	printf("CordbAppDomain - Attach - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetID(
	/* [out] */ ULONG32* pId)
{
	printf("CordbAppDomain - GetID - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbAppDomain::GetArrayOrPointerType(
	/* [in] */ CorElementType elementType,
	/* [in] */ ULONG32 nRank,
	/* [in] */ ICorDebugType* pTypeArg,
	/* [out] */ ICorDebugType** ppType)
{
	printf("CordbAppDomain - GetArrayOrPointerType - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetFunctionPointerType(
	/* [in] */ ULONG32 nTypeArgs,
	/* [size_is][in] */ ICorDebugType* ppTypeArgs[],
	/* [out] */ ICorDebugType** ppType)
{
	printf("CordbAppDomain - GetFunctionPointerType - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbAppDomain::GetCachedWinRTTypesForIIDs(
	/* [in] */ ULONG32 cReqTypes,
	/* [size_is][in] */ GUID* iidsToResolve,
	/* [out] */ ICorDebugTypeEnum** ppTypesEnum)
{
	printf("CordbAppDomain - GetCachedWinRTTypesForIIDs - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetCachedWinRTTypes(
	/* [out] */ ICorDebugGuidToTypeEnum** ppGuidToTypeEnum)
{
	printf("CordbAppDomain - GetCachedWinRTTypes - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


	HRESULT CordbAppDomain::GetObjectForCCW(
	/* [in] */ CORDB_ADDRESS ccwPointer,
	/* [out] */ ICorDebugValue** ppManagedObject)
{
	printf("CordbAppDomain - GetObjectForCCW - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}


CordbModule::CordbModule(CordbProcess* process, CordbAssembly* assembly, int id_assembly)
{
	pProcess = process;
	pCordbSymbol = NULL;
	pAssembly = assembly;
	id = id_assembly;
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
		printf("CordbModule - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		fflush(stdout);

		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG CordbModule::AddRef(void)
{
	return S_OK;
}

ULONG CordbModule::Release(void)
{
	return S_OK;
}
HRESULT CordbModule::IsMappedLayout(
	/* [out] */ BOOL* pIsMapped)
{
	*pIsMapped = FALSE;
	printf("CordbModule - IsMappedLayout - IMPLEMENTED\n");
	return S_OK;
}
HRESULT CordbModule::CreateReaderForInMemorySymbols(
	/* [in] */ REFIID riid,
	/* [iid_is][out] */ void** ppObj)
{
	printf("CordbModule - CreateReaderForInMemorySymbols - NOT IMPLEMENTED\n");
	return S_OK;
}
HRESULT CordbModule::SetJMCStatus(
	/* [in] */ BOOL bIsJustMyCode,
	/* [in] */ ULONG32 cTokens,
	/* [size_is][in] */ mdToken pTokens[])
{
	printf("CordbModule - SetJMCStatus - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::ApplyChanges(
	/* [in] */ ULONG cbMetadata,
	/* [size_is][in] */ BYTE pbMetadata[],
	/* [in] */ ULONG cbIL,
	/* [size_is][in] */ BYTE pbIL[])
{
	printf("CordbModule - ApplyChanges - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::SetJITCompilerFlags(
	/* [in] */ DWORD dwFlags)
{
	printf("CordbModule - SetJITCompilerFlags - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetJITCompilerFlags(
	/* [out] */ DWORD* pdwFlags)
{
	printf("CordbModule - GetJITCompilerFlags - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::ResolveAssembly(
	/* [in] */ mdToken tkAssemblyRef,
	/* [out] */ ICorDebugAssembly** ppAssembly)
{
	printf("CordbModule - ResolveAssembly - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("CordbModule - GetProcess - NOT IMPLEMENTED\n");
	// *ppProcess = pProcess;
	return S_OK;
}

HRESULT CordbModule::GetBaseAddress(
	/* [out] */ CORDB_ADDRESS* pAddress)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METADATA_BLOB, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	assembly_metadata_blob = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &assembly_metadata_len);

	printf("CordbModule - GetBaseAddress - %x - %d - IMPLEMENTED\n", assembly_metadata_blob, assembly_metadata_len);
	fflush(stdout);
	*pAddress = (CORDB_ADDRESS)assembly_metadata_blob;
	return S_OK;
}

HRESULT CordbModule::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_LOCATION, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	char* assembly_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	fflush(stdout);
	if (cchName < strlen(assembly_name) + 1) {
		printf("CordbModule - GetName - %s - IMPLEMENTED - devolvi soh o tamanho\n", assembly_name);
		*pcchName = strlen(assembly_name) + 1;
		g_free(assembly_name);
		return S_OK;
	}
	printf("CordbModule - GetName - %s - IMPLEMENTED - devolvi o tamanho e o valor\n", assembly_name);
	mbstowcs(szName, assembly_name, strlen(assembly_name)+1);
	*pcchName = strlen(assembly_name) + 1;
	g_free(assembly_name);
	return S_OK;
}

HRESULT CordbModule::EnableJITDebugging(
	/* [in] */ BOOL bTrackJITInfo,
	/* [in] */ BOOL bAllowJitOpts)
{
	printf("CordbModule - EnableJITDebugging - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::EnableClassLoadCallbacks(
	/* [in] */ BOOL bClassLoadCallbacks)
{
	printf("CordbModule - EnableClassLoadCallbacks - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromToken(
	/* [in] */ mdMethodDef methodDef,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	//check in a cache before talk to mono runtime to get info
	printf("CordbModule - GetFunctionFromToken - %d - IMPLEMENTED\n", methodDef);
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, id);
	buffer_add_int(&localbuf, methodDef);
	int cmdId = connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	CordbFunction* func = NULL;
	func = pProcess->cordb->findFunction(id);
	if (func == NULL)
	{
		printf("CRIANDO FUNCAO - %d\n", id);
		func = new CordbFunction(methodDef, id, this);
		g_ptr_array_add(pProcess->cordb->functions, func);
	}
	*ppFunction = func;
	return S_OK;
}

HRESULT CordbModule::GetFunctionFromRVA(
	/* [in] */ CORDB_ADDRESS rva,
	/* [out] */ ICorDebugFunction** ppFunction)
{
	printf("CordbModule - GetFunctionFromRVA - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetClassFromToken(
	/* [in] */ mdTypeDef typeDef,
	/* [out] */ ICorDebugClass** ppClass)
{
	CordbClass* pClass = new CordbClass(typeDef);
	*ppClass = static_cast<ICorDebugClass*>(pClass);
	return S_OK;
}

HRESULT CordbModule::CreateBreakpoint(
	/* [out] */ ICorDebugModuleBreakpoint** ppBreakpoint)
{
	printf("CordbModule - CreateBreakpoint - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetEditAndContinueSnapshot(
	/* [out] */ ICorDebugEditAndContinueSnapshot** ppEditAndContinueSnapshot)
{
	printf("CordbModule - GetEditAndContinueSnapshot - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetMetaDataInterface(
	/* [in] */ REFIID riid,
	/* [out] */ IUnknown** ppObj)
{
	if (pCordbSymbol == NULL)
		pCordbSymbol = new CordbSymbol(pAssembly);
	pCordbSymbol->QueryInterface(riid, (void**)ppObj);
	printf("CordbModule - GetMetaDataInterface - IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetToken(
	/* [out] */ mdModule* pToken)
{
	printf("CordbModule - GetToken - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::IsDynamic(
	/* [out] */ BOOL* pDynamic)
{
	printf("CordbModule - IsDynamic - IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetGlobalVariableValue(
	/* [in] */ mdFieldDef fieldDef,
	/* [out] */ ICorDebugValue** ppValue)
{
	printf("CordbModule - GetGlobalVariableValue - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbModule::GetSize(
	/* [out] */ ULONG32* pcBytes)
{
	printf("CordbModule - GetSize - %d - IMPLEMENTED\n", assembly_metadata_len);
	*pcBytes = assembly_metadata_len;
	return S_OK;
}

HRESULT CordbModule::IsInMemory(
	/* [out] */ BOOL* pInMemory)
{
	printf("CordbModule - IsInMemory - IMPLEMENTED\n");
	return S_OK;
}

CordbAssembly::CordbAssembly(CordbProcess* process, CordbAppDomain* appDomain, int id_assembly)
{
	pProcess = process;
	pAppDomain = appDomain;
	id = id_assembly;
}
HRESULT CordbAssembly::IsFullyTrusted(
/* [out] */ BOOL* pbFullyTrusted)
{
	*pbFullyTrusted = true;
	printf("CorDebugAssembly - IsFullyTrusted - NOT IMPLEMENTED\n");
	return S_OK;
}

HRESULT CordbAssembly::GetAppDomain(
/* [out] */ ICorDebugAppDomain** ppAppDomain)
{
	printf("CorDebugAssembly - GetAppDomain - IMPLEMENTED\n");
	*ppAppDomain = static_cast<ICorDebugAppDomain*>(pAppDomain);
	return S_OK;
}

	HRESULT CordbAssembly::EnumerateModules(
	/* [out] */ ICorDebugModuleEnum** ppModules)
{
	printf("CorDebugAssembly - EnumerateModules - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::GetCodeBase(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	printf("CorDebugAssembly - GetCodeBase - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::GetName(
	/* [in] */ ULONG32 cchName,
	/* [out] */ ULONG32* pcchName,
	/* [length_is][size_is][out] */ WCHAR szName[])
{
	printf("CorDebugAssembly - GetName - NOT IMPLEMENTED\n");
	return S_OK;
}

	HRESULT CordbAssembly::QueryInterface(
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
		printf("CordbAssembly - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		fflush(stdout);

		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG CordbAssembly::AddRef(void)
{
	return S_OK;
}

ULONG CordbAssembly::Release(void)
{
	return S_OK;
}

	HRESULT CordbProcess::EnumerateLoaderHeapMemoryRegions(
	/* [out] */ ICorDebugMemoryRangeEnum** ppRanges) {
	printf("CordbProcess - EnumerateLoaderHeapMemoryRegions - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::EnableGCNotificationEvents(
	BOOL fEnable) {
	printf("CordbProcess - EnableGCNotificationEvents - NOT IMPLEMENTED\n"); 
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::EnableExceptionCallbacksOutsideOfMyCode(
	/* [in] */ BOOL enableExceptionsOutsideOfJMC) {
	printf("CordbProcess - EnableExceptionCallbacksOutsideOfMyCode - NOT IMPLEMENTED\n"); 
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::SetWriteableMetadataUpdateMode(
	WriteableMetadataUpdateMode flags) {
	printf("CordbProcess - SetWriteableMetadataUpdateMode - NOT IMPLEMENTED\n"); 
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::GetGCHeapInformation(
	/* [out] */ COR_HEAPINFO* pHeapInfo) {
	printf("CordbProcess - GetGCHeapInformation - NOT IMPLEMENTED\n"); 
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateHeap(
	/* [out] */ ICorDebugHeapEnum** ppObjects) {
	printf("CordbProcess - EnumerateHeap - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateHeapRegions(
	/* [out] */ ICorDebugHeapSegmentEnum** ppRegions) {
	printf("CordbProcess - EnumerateHeapRegions - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetObject(
	/* [in] */ CORDB_ADDRESS addr,
	/* [out] */ ICorDebugObjectValue** pObject) {
	printf("CordbProcess - GetObject - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateGCReferences(
	/* [in] */ BOOL enumerateWeakReferences,
	/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
	printf("CordbProcess - EnumerateGCReferences - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateHandles(
	/* [in] */ CorGCReferenceType types,
	/* [out] */ ICorDebugGCReferenceEnum** ppEnum) {
	printf("CordbProcess - EnumerateHandles - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetTypeID(
	/* [in] */ CORDB_ADDRESS obj,
	/* [out] */ COR_TYPEID* pId) {
	printf("CordbProcess - GetTypeID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetTypeForTypeID(
	/* [in] */ COR_TYPEID id,
	/* [out] */ ICorDebugType** ppType) {
	printf("CordbProcess - GetTypeForTypeID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetArrayLayout(
	/* [in] */ COR_TYPEID id,
	/* [out] */ COR_ARRAY_LAYOUT* pLayout) {
	printf("CordbProcess - GetArrayLayout - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetTypeLayout(
	/* [in] */ COR_TYPEID id,
	/* [out] */ COR_TYPE_LAYOUT* pLayout) {
	printf("CordbProcess - GetTypeLayout - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetTypeFields(
	/* [in] */ COR_TYPEID id,
	ULONG32 celt,
	COR_FIELD fields[],
	ULONG32* pceltNeeded) {
	printf("CordbProcess - GetTypeFields - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnableNGENPolicy(
	/* [in] */ CorDebugNGENPolicy ePolicy) {
	printf("CordbProcess - EnableNGENPolicy - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::Filter(
	/* [size_is][length_is][in] */ const BYTE pRecord[],
	/* [in] */ DWORD countBytes,
	/* [in] */ CorDebugRecordFormat format,
	/* [in] */ DWORD dwFlags,
	/* [in] */ DWORD dwThreadId,
	/* [in] */ ICorDebugManagedCallback* pCallback,
	/* [out][in] */ CORDB_CONTINUE_STATUS* pContinueStatus) {
	printf("CordbProcess - Filter - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::ProcessStateChanged(
	/* [in] */ CorDebugStateChange eChange) {
	printf("CordbProcess - ProcessStateChanged - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::SetEnableCustomNotification(
	ICorDebugClass* pClass,
	BOOL fEnable) {
	printf("CordbProcess - SetEnableCustomNotification - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::GetID(
	/* [out] */ DWORD* pdwProcessId) {
	printf("CordbProcess - GetID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetHandle(
	/* [out] */ HPROCESS* phProcessHandle) {
	printf("CordbProcess - GetHandle - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetThread(
	/* [in] */ DWORD dwThreadId,
	/* [out] */ ICorDebugThread** ppThread) {
	printf("CordbProcess - GetThread - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateObjects(
	/* [out] */ ICorDebugObjectEnum** ppObjects) {
	printf("CordbProcess - EnumerateObjects - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::IsTransitionStub(
	/* [in] */ CORDB_ADDRESS address,
	/* [out] */ BOOL* pbTransitionStub) {
	printf("CordbProcess - IsTransitionStub - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::IsOSSuspended(
	/* [in] */ DWORD threadID,
	/* [out] */ BOOL* pbSuspended) {
	printf("CordbProcess - IsOSSuspended - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetThreadContext(
	/* [in] */ DWORD threadID,
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][out][in] */ BYTE context[]) {
	printf("CordbProcess - GetThreadContext - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::SetThreadContext(
	/* [in] */ DWORD threadID,
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][in] */ BYTE context[]) {
	printf("CordbProcess - SetThreadContext - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT CordbProcess::ReadMemory(
/* [in] */ CORDB_ADDRESS address,
/* [in] */ DWORD size,
/* [length_is][size_is][out] */ BYTE buffer[],
/* [out] */ SIZE_T* read) {
	memcpy(buffer, (void*)address, size);
	if (read != NULL)
		*read = size;
	printf("CordbProcess - ReadMemory - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::WriteMemory(
	/* [in] */ CORDB_ADDRESS address,
	/* [in] */ DWORD size,
	/* [size_is][in] */ BYTE buffer[],
	/* [out] */ SIZE_T* written) {
	printf("CordbProcess - WriteMemory - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::ClearCurrentException(
	/* [in] */ DWORD threadID) {
	printf("CordbProcess - ClearCurrentException - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnableLogMessages(
	/* [in] */ BOOL fOnOff) {
	printf("CordbProcess - EnableLogMessages - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::ModifyLogSwitch(
	/* [annotation][in] */
	_In_  WCHAR* pLogSwitchName,
	/* [in] */ LONG lLevel) {
	printf("CordbProcess - ModifyLogSwitch - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateAppDomains(
	/* [out] */ ICorDebugAppDomainEnum** ppAppDomains) {
	printf("CordbProcess - EnumerateAppDomains - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetObject(
	/* [out] */ ICorDebugValue** ppObject) {
	printf("CordbProcess - GetObject - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::ThreadForFiberCookie(
	/* [in] */ DWORD fiberCookie,
	/* [out] */ ICorDebugThread** ppThread) {
	printf("CordbProcess - ThreadForFiberCookie - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetHelperThreadID(
	/* [out] */ DWORD* pThreadID) {
	printf("GetHelperThreadID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::GetThreadForTaskID(
	/* [in] */ TASKID taskid,
	/* [out] */ ICorDebugThread2** ppThread) {
	printf("CordbProcess - GetHelperThreadID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetVersion(
	/* [out] */ COR_VERSION* version) {
	printf("CordbProcess - GetVersion - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::SetUnmanagedBreakpoint(
	/* [in] */ CORDB_ADDRESS address,
	/* [in] */ ULONG32 bufsize,
	/* [length_is][size_is][out] */ BYTE buffer[],
	/* [out] */ ULONG32* bufLen) {
	printf("CordbProcess - SetUnmanagedBreakpoint - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::ClearUnmanagedBreakpoint(
	/* [in] */ CORDB_ADDRESS address) {
	printf("CordbProcess - ClearUnmanagedBreakpoint - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::SetDesiredNGENCompilerFlags(
	/* [in] */ DWORD pdwFlags) {
	printf("CordbProcess - SetDesiredNGENCompilerFlags - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetDesiredNGENCompilerFlags(
	/* [out] */ DWORD* pdwFlags)
{
	printf("CordbProcess - GetDesiredNGENCompilerFlags - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::GetReferenceValueFromGCHandle(
	/* [in] */ UINT_PTR handle,
	/* [out] */ ICorDebugReferenceValue** pOutValue)
{
	printf("CordbProcess - GetReferenceValueFromGCHandle - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
	HRESULT CordbProcess::QueryInterface(
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
		printf("CordbProcess - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		fflush(stdout);

		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG CordbProcess::AddRef(void)
{
	return S_OK;
}

ULONG CordbProcess::Release(void)
{
	return S_OK;
}

HRESULT CordbProcess::Stop(
/* [in] */ DWORD dwTimeoutIgnored) {
	if (suspended)
	{
		printf("CordbProcess - Stop - ALREADY SUSPENDED\n");
		fflush(stdout);
		return S_OK;
	}
	printf("CordbProcess - Stop - IMPLEMENTED\n");
	fflush(stdout);
	Buffer sendbuf;
	buffer_init(&sendbuf, 128);
	connection->send_event(CMD_SET_VM, CMD_VM_SUSPEND, &sendbuf);
	suspended = true;
	return S_OK;
}

	HRESULT CordbProcess::Continue(
	/* [in] */ BOOL fIsOutOfBand)
{
	if (suspended == false) {
		printf("RESUME SEM ESTAR SUSPENSO - fIsOutOfBand - %d\n", fIsOutOfBand);
		return S_OK;
	}
	suspended = false;
	printf("CordbProcess - Continue - IMPLEMENTED\n");
	fflush(stdout);
	Buffer sendbuf;
	buffer_init(&sendbuf, 128);
	connection->send_event(CMD_SET_VM, CMD_VM_RESUME, &sendbuf);
	return S_OK;
}

	HRESULT CordbProcess::IsRunning(
	/* [out] */ BOOL* pbRunning) {
	printf("CordbProcess - IsRunning - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::HasQueuedCallbacks(
	/* [in] */ ICorDebugThread* pThread,
	/* [out] */ BOOL* pbQueued) {
	//connection->process_packet_from_queue();
	printf("CordbProcess - HasQueuedCallbacks - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::EnumerateThreads(
	/* [out] */ ICorDebugThreadEnum** ppThreads) {
	printf("CordbProcess - EnumerateThreads - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::SetAllThreadsDebugState(
	/* [in] */ CorDebugThreadState state,
	/* [in] */ ICorDebugThread* pExceptThisThread) {
	printf("CordbProcess - SetAllThreadsDebugState - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::Detach(void) {
	printf("CordbProcess - Detach - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::Terminate(
	/* [in] */ UINT exitCode) {
	printf("CordbProcess - Terminate - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::CanCommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError) {
	printf("CordbProcess - CanCommitChanges - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

	HRESULT CordbProcess::CommitChanges(
	/* [in] */ ULONG cSnapshots,
	/* [size_is][in] */ ICorDebugEditAndContinueSnapshot* pSnapshots[],
	/* [out] */ ICorDebugErrorInfoEnum** pError) {
	printf("CordbProcess - CommitChanges - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

MONO_API HRESULT CoreCLRCreateCordbObjectEx(int iDebuggerVersion, DWORD pid, LPCWSTR lpApplicationGroupId, HMODULE hmodTargetCLR, void** ppCordb)
{
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


HRESULT Cordb::Initialize(void)
{
	printf("Cordb - Initialize - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::Terminate(void)
{
	printf("Cordb - Terminate - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::SetManagedHandler(
/* [in] */ ICorDebugManagedCallback* pCallback)
{
	printf("Cordb - SetManagedHandler - IMPLEMENTED\n");
	this->pCallback = pCallback;
	this->pCallback->AddRef();
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::SetUnmanagedHandler(
/* [in] */ ICorDebugUnmanagedCallback* pCallback)
{
	printf("Cordb - SetUnmanagedHandler - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::CreateProcess(
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
	printf("Cordb - CreateProcess - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::DebugActiveProcess(
/* [in] */ DWORD id,
/* [in] */ BOOL win32Attach,
/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("Cordb - DebugActiveProcess - IMPLEMENTED\n");
	*ppProcess = new CordbProcess;
	((CordbProcess*)*ppProcess)->cordb = this;
		
	DWORD thread_id;
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)debugger_thread, ((CordbProcess*)*ppProcess), 0, &thread_id);

	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::EnumerateProcesses(
/* [out] */ ICorDebugProcessEnum** ppProcess)
{
	printf("Cordb - EnumerateProcesses - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::GetProcess(
/* [in] */ DWORD dwProcessId,
/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("Cordb - GetProcess - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::CanLaunchOrAttach(
/* [in] */ DWORD dwProcessId,
/* [in] */ BOOL win32DebuggingEnabled)
{
	printf("Cordb - CanLaunchOrAttach - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

Cordb::Cordb()
{
	pCallback = NULL;
	breakpoints = g_ptr_array_new();
	threads = g_ptr_array_new();
	functions = g_ptr_array_new();
}

CordbFunction* Cordb::findFunction(int id)
{
	int i = 0;
	while (i < functions->len) {
		CordbFunction* function = (CordbFunction*)g_ptr_array_index(functions, i);
		if (function->id == id) {
			return function;
		}
		i++;
	}
	return NULL;
}

CordbFunction* Cordb::findFunctionByToken(int token)
{
	int i = 0;
	while (i < functions->len) {
		CordbFunction* function = (CordbFunction*)g_ptr_array_index(functions, i);
		if (function->token == token) {
			return function;
		}
		i++;
	}
	return NULL;
}

HRESULT Cordb::QueryInterface(
/* [in] */ REFIID riid,
/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppvObject)
{
	printf("Cordb - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

ULONG Cordb::AddRef(void)
{
	return S_OK;
}

ULONG Cordb::Release(void)
{
	return S_OK;
}

HRESULT Cordb::CreateProcessEx(
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
	printf("Cordb - CreateProcessEx - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT Cordb::DebugActiveProcessEx(
/* [in] */ ICorDebugRemoteTarget* pRemoteTarget,
/* [in] */ DWORD dwProcessId,
/* [in] */ BOOL fWin32Attach,
/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("Cordb - DebugActiveProcessEx - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}



Connection::Connection(CordbProcess* proc, Cordb* cordb)
{
	ppProcess = proc;
	ppCordb = cordb;
	pCorDebugAppDomain = NULL;
	received_replies = g_hash_table_new(NULL, NULL);
	received_replies_to_process = g_ptr_array_new();
	is_answer_pending = false;
}

CordbThread* Connection::findThread(GPtrArray* threads, long thread_id)
{
	int i = 0;
	while (i < threads->len) {
		CordbThread *thread = (CordbThread*)g_ptr_array_index(threads, i);
		if (thread->thread_id == thread_id) {
			return thread;
		}
		i++;
	}
	return NULL;
}

void Connection::process_packet_internal(Buffer *recvbuf)
{
	int spolicy = decode_byte(recvbuf->buf, &recvbuf->buf, recvbuf->end);
	int nevents = decode_int(recvbuf->buf, &recvbuf->buf, recvbuf->end);

	for (int i = 0; i < nevents; ++i) {

		int kind = decode_byte(recvbuf->buf, &recvbuf->buf, recvbuf->end);
		int req_id = decode_int(recvbuf->buf, &recvbuf->buf, recvbuf->end);

		EventKind etype = (EventKind)kind;

		long thread_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);

		printf("Received %d events %s(%d), suspend=%d.\n", nevents, event_to_string(etype), etype, spolicy);
		fflush(stdout);
		switch (etype)
		{
		case EVENT_KIND_VM_START:
		{
			ppProcess->suspended = true;
			Buffer sendbuf;
			buffer_init(&sendbuf, 128);
			send_event(0, 0, &sendbuf);
			ppCordb->pCallback->CreateProcess(static_cast<ICorDebugProcess*>(ppProcess));
		}
		break;
		case EVENT_KIND_ASSEMBLY_LOAD:
		{
			// all the callbacks call a resume, in this case that we are faking 2 callbacks without receive command, we should not send the continue
			int assembly_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
			if (pCorDebugAppDomain == NULL) {
				pCorDebugAppDomain = new CordbAppDomain(static_cast<ICorDebugProcess*>(ppProcess));
				ppCordb->pCallback->CreateAppDomain(static_cast<ICorDebugProcess*>(ppProcess), pCorDebugAppDomain);
			}

			ICorDebugAssembly* pAssembly = new CordbAssembly(ppProcess, pCorDebugAppDomain, assembly_id);
			ppCordb->pCallback->LoadAssembly(pCorDebugAppDomain, pAssembly);

			ppProcess->suspended = true;
			ICorDebugModule* pModule = new CordbModule(ppProcess, (CordbAssembly*)pAssembly, assembly_id);
			ppCordb->pCallback->LoadModule(pCorDebugAppDomain, pModule);
		}
		break;
		case EVENT_KIND_BREAKPOINT:
		{
			int method_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
			long offset = decode_long(recvbuf->buf, &recvbuf->buf, recvbuf->end);
			CordbThread* thread = findThread(ppCordb->threads, thread_id);
			if (thread == NULL)
			{ 
				thread = new CordbThread(ppProcess, thread_id);
				g_ptr_array_add(ppCordb->threads, thread);
				ppCordb->pCallback->CreateThread(pCorDebugAppDomain, thread);
			}
			

			Buffer sendbuf;
			buffer_init(&sendbuf, 128);
			send_event(0, 0, &sendbuf);
			int i = 0;
			CordbFunctionBreakpoint* breakpoint;
			while (i < ppCordb->breakpoints->len) {
				ppProcess->suspended = true;
				breakpoint = (CordbFunctionBreakpoint*)g_ptr_array_index(ppCordb->breakpoints, i);
				if (breakpoint->offset == offset && breakpoint->code->func->id == method_id) {
					printf("CHEGOU O BREAKPOINT = %d - %d - %d - %d - %x", method_id, offset, breakpoint->offset, breakpoint->code->func->id, static_cast<ICorDebugFunctionBreakpoint*>(breakpoint));
					ppCordb->pCallback->Breakpoint(pCorDebugAppDomain, thread, static_cast<ICorDebugFunctionBreakpoint*>(breakpoint));
					break;
				}
				i++;
			}
		}
		break;
		case EVENT_KIND_STEP:
		{
			int method_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
			long offset = decode_long(recvbuf->buf, &recvbuf->buf, recvbuf->end);
			CordbThread* thread = findThread(ppCordb->threads, thread_id);
			if (thread == NULL)
			{
				thread = new CordbThread(ppProcess, thread_id);
				g_ptr_array_add(ppCordb->threads, thread);
				ppCordb->pCallback->CreateThread(pCorDebugAppDomain, thread);
			}


			Buffer sendbuf;
			buffer_init(&sendbuf, 128);
			send_event(0, 0, &sendbuf);
			ppProcess->suspended = true;
			ppCordb->pCallback->StepComplete(pCorDebugAppDomain, thread, thread->stepper, STEP_NORMAL);
		}
		break;
		}
	}
	//buffer_free(&recvbuf);
}

int Connection::process_packet(bool is_answer)
{
	if (!is_answer)
		process_packet_from_queue();

	int iResult;
	Buffer sendbuf, recvbufheader;
	Buffer* recvbuf;
	recvbuf = new Buffer();

	buffer_init(&sendbuf, 128);
	buffer_init(&recvbufheader, HEADER_LEN);
	
	iResult = recv(connect_socket, (char*)recvbufheader.buf, HEADER_LEN, 0);
	if (iResult > 0)
		printf("");
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	Header header;
	decode_command_header(&recvbufheader, &header);
	
	if (header.len < HEADER_LEN) {
		fflush(stdout);
		return 0;
	}

	if (header.flags == REPLY_PACKET) //add to the dictionary of messages
	{
		Buffer *recvbuf2;
		recvbuf2 = new Buffer();
		buffer_init(recvbuf2, header.len - HEADER_LEN);
		if (header.len - HEADER_LEN != 0)
		{
			iResult = recv(connect_socket, (char*)recvbuf2->buf, header.len - HEADER_LEN, 0);
			int totalRead = iResult;
			while (totalRead < header.len - HEADER_LEN)
			{
				iResult = recv(connect_socket, (char*)recvbuf2->buf+ totalRead, (header.len - HEADER_LEN) - totalRead, 0);
				fflush(stdout);
				totalRead += iResult;
			}
		}
		g_hash_table_insert(received_replies, (gpointer)(gssize)(header.id), recvbuf2);
		return 0;
	}

	buffer_init(recvbuf, header.len - HEADER_LEN);
	iResult = recv(connect_socket, (char*)recvbuf->buf, header.len - HEADER_LEN, 0);

	if (iResult > 0)
		printf("");
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	if (header.command_set == CMD_SET_EVENT && header.command == CMD_COMPOSITE) {
		if (is_answer)
		{
			g_ptr_array_add(received_replies_to_process, recvbuf);
			return iResult;
		}
		process_packet_internal(recvbuf);
	}
	return iResult;
}

void Connection::process_packet_from_queue()
{
	int i = 0;
	while (i < received_replies_to_process->len) {
		Buffer* req = (Buffer*)g_ptr_array_index(received_replies_to_process, i);
		process_packet_internal(req);
		g_ptr_array_remove_index_fast(received_replies_to_process, i);
		g_free(req);
	}
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
		if (!ppProcess->suspended)
			iResult = process_packet();
		Sleep(100);
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

		int flag = 1;
		int result = setsockopt(connect_socket,
			IPPROTO_TCP,
			TCP_NODELAY,
			(char*)&flag,
			sizeof(int));
		break;
	}

	freeaddrinfo(result);

	if (connect_socket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
	
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
	int iResult = send(connect_socket, (const char*)sendbuf.buf, buffer_len(&sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(connect_socket);
		WSACleanup();
		return;
	}
}
void Connection::receive_packet(Buffer& recvbuf, int len)
{
	buffer_init(&recvbuf, len);
	int iResult;
	iResult = recv(connect_socket, (char*)recvbuf.buf, len, 0);
	if (iResult > 0)
		printf("Bytes received receive_packet: %d\n", iResult);
}

void Connection::receive_header(Header *header)
{
	Buffer recvbuf;
	buffer_init(&recvbuf, 11);
	int iResult;
	iResult = recv(connect_socket, (char*)recvbuf.buf, 11, 0);
	if (iResult > 0)
		printf("Bytes received receive_header: %d\n", iResult);
	decode_command_header(&recvbuf, header);
}

int Connection::send_event(int cmd_set, int cmd, Buffer *sendbuf)
{
	Buffer outbuf;
	int ret = buffer_add_command_header(sendbuf, cmd_set, cmd, &outbuf);
	send_packet(outbuf);
	return ret;
}




MONO_API HRESULT CoreCLRCreateCordbObject(int iDebuggerVersion, DWORD pid, HMODULE hmodTargetCLR, void** ppCordb)
{
	*ppCordb = new Cordb();
	return S_OK;
}

MONO_API HRESULT CreateCordbObject(int iDebuggerVersion, void** ppCordb)
{
	return S_OK;
}


HRESULT CordbModule::GetAssembly(
	/* [out] */ ICorDebugAssembly** ppAssembly)
{
	printf("CordbModule - GetAssembly\n");
	*ppAssembly = static_cast<ICorDebugAssembly*> (pAssembly);
	return S_OK;
}

HRESULT CordbAssembly::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("CorDebugAssembly - GetProcess\n");
	*ppProcess = static_cast<ICorDebugProcess*>(pProcess);
	return S_OK;
}

HRESULT CordbSymbol::GetAssemblyFromScope(mdAssembly* ptkAssembly)
{
	*ptkAssembly = pCordbAssembly->id;
	printf("CordbSymbol - GetAssemblyFromScope - IMPLEMENTED - id = %d\n", *ptkAssembly);
	fflush(stdout);
	return S_OK;
}    // [OUT] Put token here.

CordbFunction::CordbFunction(mdToken token, int id, CordbModule* module)
{
	this->token = token;
	this->id = id;
	code = NULL;
	this->module = module;
}

HRESULT __stdcall CordbFunction::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugFunction)
	{
		*pInterface = static_cast<ICorDebugFunction*>(this);
	}
	else if (id == IID_ICorDebugFunction2)
	{
		*pInterface = static_cast<ICorDebugFunction2*>(this);
	}
	else if (id == IID_ICorDebugFunction3)
	{
		*pInterface = static_cast<ICorDebugFunction3*>(this);
	}
	else if (id == IID_ICorDebugFunction4)
	{
		*pInterface = static_cast<ICorDebugFunction4*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugFunction*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG __stdcall CordbFunction::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbFunction::Release(void)
{
	return 0;
}

HRESULT __stdcall CordbFunction::GetModule(ICorDebugModule** ppModule)
{
	*ppModule = static_cast<ICorDebugModule*>(this->module);
	printf("CordbFunction - GetModule - IMPLEMENTED\n");
	fflush(stdout);
	if (!*ppModule)
		return S_FALSE;
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetClass(ICorDebugClass** ppClass)
{
	printf("CordbFunction - GetClass - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetToken(mdMethodDef* pMethodDef)
{
	*pMethodDef = this->token;
	printf("CordbFunction - GetToken - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetILCode(ICorDebugCode** ppCode)
{
	if (code == NULL)
		code = new CordbCode(this); 
	*ppCode = static_cast<ICorDebugCode*>(code);
	printf("CordbFunction - GetILCode - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetNativeCode(ICorDebugCode** ppCode)
{
	*ppCode = static_cast<ICorDebugCode*>(code);
	printf("CordbFunction - GetNativeCode - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunction::CreateBreakpoint(ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	printf("CordbFunction - CreateBreakpoint - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetLocalVarSigToken(mdSignature* pmdSig)
{
	printf("CordbFunction - GetLocalVarSigToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetCurrentVersionNumber(ULONG32* pnCurrentVersion)
{
	*pnCurrentVersion = 1;
	printf("CordbFunction - GetCurrentVersionNumber - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunction::SetJMCStatus(BOOL bIsJustMyCode)
{
	printf("CordbFunction - SetJMCStatus - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetJMCStatus(BOOL* pbIsJustMyCode)
{
	printf("CordbFunction - GetJMCStatus - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::EnumerateNativeCode(ICorDebugCodeEnum** ppCodeEnum)
{
	printf("CordbFunction - EnumerateNativeCode - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::GetVersionNumber(ULONG32* pnVersion)
{
	*pnVersion = 1;
	printf("CordbFunction - GetVersionNumber - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunction::GetActiveReJitRequestILCode(ICorDebugILCode** ppReJitedILCode)
{
	printf("CordbFunction - GetActiveReJitRequestILCode - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunction::CreateNativeBreakpoint(ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	printf("CordbFunction - CreateNativeBreakpoint - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

CordbCode::CordbCode(CordbFunction* func)
{
	this->func = func;
}

HRESULT __stdcall CordbCode::IsIL(BOOL* pbIL)
{
	printf("CordbCode - IsIL - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetFunction(ICorDebugFunction** ppFunction)
{
	printf("CordbCode - GetFunction - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetAddress(CORDB_ADDRESS* pStart)
{
	printf("CordbCode - GetAddress - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetSize(ULONG32* pcBytes)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, this->func->id);
	int cmdId = connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_BODY, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int code_size = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	*pcBytes = code_size;
	printf("CordbCode - GetSize - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbCode::CreateBreakpoint(ULONG32 offset, ICorDebugFunctionBreakpoint** ppBreakpoint)
{
	//add it in a list to not recreate a already created breakpoint
	CordbFunctionBreakpoint* bp = new CordbFunctionBreakpoint(this, offset);
	*ppBreakpoint = static_cast<ICorDebugFunctionBreakpoint*>(bp);
	g_ptr_array_add(this->func->module->pProcess->cordb->breakpoints, bp);
	printf("CordbCode - CreateBreakpoint - %ld - IMPLEMENTED\n", offset);
	printf("CreateBreakpoint - static_cast<ICorDebugFunctionBreakpoint*>(breakpoint) - %x", *ppBreakpoint);
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbCode::GetCode(ULONG32 startOffset, ULONG32 endOffset, ULONG32 cBufferAlloc, BYTE buffer[], ULONG32* pcBufferSize)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, this->func->id);
	int cmdId = connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_BODY, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	guint8*  code = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, pcBufferSize);
	
	memcpy(buffer, code, *pcBufferSize);
	printf("CordbCode - GetCode - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbCode::GetVersionNumber(ULONG32* nVersion)
{
	*nVersion = 1;
	printf("CordbCode - GetVersionNumber - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbCode::GetILToNativeMapping(ULONG32 cMap, ULONG32* pcMap, COR_DEBUG_IL_TO_NATIVE_MAP map[])
{
	printf("CordbCode - GetILToNativeMapping - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::GetEnCRemapSequencePoints(ULONG32 cMap, ULONG32* pcMap, ULONG32 offsets[])
{
	printf("CordbCode - GetEnCRemapSequencePoints - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbCode::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugCode)
	{
		*pInterface = static_cast<ICorDebugCode*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugCode*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG __stdcall CordbCode::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbCode::Release(void)
{
	return 0;
}

CordbFunctionBreakpoint::CordbFunctionBreakpoint(CordbCode* code, ULONG32 offset)
{
	this->code = code;
	this->offset = offset;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetFunction(ICorDebugFunction** ppFunction)
{
	*ppFunction = static_cast<ICorDebugFunction*>(code->func) ;
	printf("CordbFunctionBreakpoint - GetFunction - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::GetOffset(ULONG32* pnOffset)
{
	printf("CordbFunctionBreakpoint - GetOffset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunctionBreakpoint::Activate(BOOL bActive)
{
	if (bActive)
	{
		Buffer sendbuf;
		int buflen = 128;
		buffer_init(&sendbuf, buflen);
		buffer_add_byte(&sendbuf, EVENT_KIND_BREAKPOINT);
		buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
		buffer_add_byte(&sendbuf, 1); //modifiers
		buffer_add_byte(&sendbuf, MOD_KIND_LOCATION_ONLY);
		buffer_add_id(&sendbuf, this->code->func->id);
		buffer_add_long(&sendbuf, offset);
		connection->send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
		buffer_free(&sendbuf);

		printf("CordbFunctionBreakpoint - Activate - %d - %ld - IMPLEMENTED\n", this->code->func->id, offset);
		fflush(stdout);
	}
	return S_OK;
}

HRESULT __stdcall CordbFunctionBreakpoint::IsActive(BOOL* pbActive)
{
	printf("CordbFunctionBreakpoint - IsActive - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbFunctionBreakpoint::QueryInterface(REFIID id, void** pInterface)
{
	printf("CordbFunctionBreakpoint - QueryInterface - IMPLEMENTED\n");
	fflush(stdout);
	if (id == IID_ICorDebugFunctionBreakpoint)
	{
		*pInterface = static_cast<ICorDebugFunctionBreakpoint*>(this);
	}
	else
	{
		// Not looking for a function breakpoint? See if the base class handles
		// this interface. (issue 143976)
		printf("return CordbBreakpoint::QueryInterface(id, pInterface);\n");
		fflush(stdout);
		//return CordbBreakpoint::QueryInterface(id, pInterface);
	}
	return S_OK;
}

ULONG __stdcall CordbFunctionBreakpoint::AddRef(void)
{
	return 0;
}

ULONG __stdcall CordbFunctionBreakpoint::Release(void)
{
	return 0;
}

CordbThread::CordbThread(CordbProcess* ppProcess, long thread_id)
{
	this->ppProcess = ppProcess;
	this->thread_id = thread_id;
	stepper = NULL;
}

HRESULT STDMETHODCALLTYPE CordbThread::HasUnhandledException(void)
{
	printf("CordbThread - HasUnhandledException - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetBlockingObjects(
	/* [out] */ ICorDebugBlockingObjectEnum** ppBlockingObjectEnum)
{
	printf("CordbThread - GetBlockingObjects - IMPLEMENTED\n");
	CordbBlockingObjectEnum* blockingObject = new CordbBlockingObjectEnum();
	*ppBlockingObjectEnum = static_cast<ICorDebugBlockingObjectEnum*>(blockingObject);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentCustomDebuggerNotification(
	/* [out] */ ICorDebugValue** ppNotificationObject)
{
	printf("CordbThread - GetCurrentCustomDebuggerNotification - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStackWalk(
	/* [out] */ ICorDebugStackWalk** ppStackWalk)
{
	printf("CordbThread - CreateStackWalk - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveInternalFrames(
	/* [in] */ ULONG32 cInternalFrames,
	/* [out] */ ULONG32* pcInternalFrames,
	/* [length_is][size_is][out][in] */ ICorDebugInternalFrame2* ppInternalFrames[])
{
	printf("CordbThread - GetActiveInternalFrames - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFunctions(
	/* [in] */ ULONG32 cFunctions,
	/* [out] */ ULONG32* pcFunctions,
	/* [length_is][size_is][out][in] */ COR_ACTIVE_FUNCTION pFunctions[])
{
	printf("CordbThread - GetActiveFunctions - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetConnectionID(
	/* [out] */ CONNID* pdwConnectionId)
{
	printf("CordbThread - GetConnectionID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetTaskID(
	/* [out] */ TASKID* pTaskId)
{
	printf("CordbThread - GetTaskID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetVolatileOSThreadID(
	/* [out] */ DWORD* pdwTid)
{
	printf("CordbThread - GetVolatileOSThreadID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::InterceptCurrentException(

	/* [in] */ ICorDebugFrame* pFrame)
{
	printf("CordbThread - InterceptCurrentException - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThread::GetProcess(
	/* [out] */ ICorDebugProcess** ppProcess)
{
	printf("CordbThread - GetProcess - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetID(
	/* [out] */ DWORD* pdwThreadId)
{
	*pdwThreadId = thread_id;
	printf("CordbThread - GetID - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetHandle(
	/* [out] */ HTHREAD* phThreadHandle)
{
	printf("CordbThread - GetHandle - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetAppDomain(
	/* [out] */ ICorDebugAppDomain** ppAppDomain)
{
	*ppAppDomain = static_cast<ICorDebugAppDomain*>(connection->pCorDebugAppDomain);
	printf("CordbThread - GetAppDomain - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;

}

HRESULT STDMETHODCALLTYPE CordbThread::SetDebugState(
	/* [in] */ CorDebugThreadState state)
{
	printf("CordbThread - SetDebugState - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetDebugState(
	/* [out] */ CorDebugThreadState* pState)
{
	printf("CordbThread - GetDebugState - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetUserState(
	/* [out] */ CorDebugUserState* pState)
{
	printf("CordbThread - GetUserState - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetCurrentException(
	/* [out] */ ICorDebugValue** ppExceptionObject)
{
	printf("CordbThread - GetCurrentException - IMPLEMENTED\n");
	fflush(stdout);
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbThread::ClearCurrentException(void)
{
	printf("CordbThread - ClearCurrentException - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateStepper(
	/* [out] */ ICorDebugStepper** ppStepper)
{
	if (!this->stepper)
	{
		CordbStepper* stepper = new CordbStepper(this);
		this->stepper = stepper;
	}
	*ppStepper = static_cast<ICorDebugStepper*>(stepper);

	printf("CordbThread - CreateStepper - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::EnumerateChains(
	/* [out] */ ICorDebugChainEnum** ppChains)
{
	CordbChainEnum* pChains = new CordbChainEnum(this);
	*ppChains = static_cast<ICorDebugChainEnum*>(pChains);
	printf("CordbThread - EnumerateChains - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveChain(
	/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbThread - GetActiveChain - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetActiveFrame(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread_id);
	buffer_add_int(&localbuf, 0);
	buffer_add_int(&localbuf, -1);

	int cmdId = connection->send_event(CMD_SET_THREAD, CMD_THREAD_GET_FRAME_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int nframes = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	for (int i = 0; i < nframes; i++)
	{
		int frameid = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int methoid = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int il_offset = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int flags = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		if (i == 0)
		{
			CordbNativeFrame* frame = new CordbNativeFrame(frameid, methoid, il_offset, flags, this);
			*ppFrame = static_cast<ICorDebugFrame*>(frame);
		}
	}
	guint32 ctx_len;
	guint8 *ctx = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &ctx_len);
	registerset = new CordbRegisteSet(ctx, ctx_len);
	printf("CordbThread - GetActiveFrame - IMPLEMENTED - %d\n", nframes);

	
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetRegisterSet(
	/* [out] */ ICorDebugRegisterSet** ppRegisters)
{
	
	*ppRegisters = static_cast<ICorDebugRegisterSet*>(registerset);
	printf("CordbThread - GetRegisterSet - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbThread::CreateEval(
	/* [out] */ ICorDebugEval** ppEval)
{
	printf("CordbThread - CreateEval - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbThread::GetObject(
	/* [out] */ ICorDebugValue** ppObject)
{
	printf("CordbThread - GetObject - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbThread::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* ppInterface)
{
	if (id == IID_ICorDebugThread)
	{
		*ppInterface = static_cast<ICorDebugThread*>(this);
	}
	else if (id == IID_ICorDebugThread2)
	{
		*ppInterface = static_cast<ICorDebugThread2*>(this);
	}
	else if (id == IID_ICorDebugThread3)
	{
		*ppInterface = static_cast<ICorDebugThread3*>(this);
	}
	else if (id == IID_ICorDebugThread4)
	{
		*ppInterface = static_cast<ICorDebugThread4*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*ppInterface = static_cast<IUnknown*>(static_cast<ICorDebugThread*>(this));
	}
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}
	printf("CordbThread - QueryInterface - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
ULONG STDMETHODCALLTYPE CordbThread::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbThread::Release(void)
{
	return 0;
}


CordbJITILFrame::CordbJITILFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread)
{
	this->frameid = frameid;
	this->methoid = methoid;
	this->il_offset = il_offset;
	this->flags = flags;
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetChain(
	/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbFrame - GetChain - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCode(
	/* [out] */ ICorDebugCode** ppCode)
{
	printf("CordbFrame - GetCode - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetFunction(
	/* [out] */ ICorDebugFunction** ppFunction)
{
	CordbFunction *func = thread->ppProcess->cordb->findFunction(methoid);
	if (!func)
	{
		func = new CordbFunction(0, methoid, NULL);
		g_ptr_array_add(thread->ppProcess->cordb->functions, func);
	}

	*ppFunction = static_cast<ICorDebugFunction*>(func);
	printf("CordbFrame - GetFunction - %x - %d - IMPLEMENTED\n", func, methoid);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetFunctionToken(
	/* [out] */ mdMethodDef* pToken)
{
	printf("CordbFrame - GetFunctionToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackRange(
	/* [out] */ CORDB_ADDRESS* pStart,
	/* [out] */ CORDB_ADDRESS* pEnd)
{
	*pStart = 4096;
	*pEnd = 8192;
	printf("CordbFrame - GetStackRange - NOT IMPLEMENTED - we need id?\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCaller(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	printf("CordbFrame - GetCaller - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCallee(
	/* [out] */ ICorDebugFrame** ppFrame)
{
	printf("CordbFrame - GetCallee - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CreateStepper(
	/* [out] */ ICorDebugStepper** ppStepper)
{
	printf("CordbFrame - CreateStepper - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}
HRESULT STDMETHODCALLTYPE CordbJITILFrame::QueryInterface(
	/* [in] */ REFIID id,
	/* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
	if (id == IID_ICorDebugILFrame)
	{
		*pInterface = static_cast<ICorDebugILFrame*>(this);
	}
	else if (id == IID_ICorDebugILFrame2)
	{
		*pInterface = static_cast<ICorDebugILFrame2*>(this);
	}
	else if (id == IID_ICorDebugILFrame3)
	{
		*pInterface = static_cast<ICorDebugILFrame3*>(this);
	}
	else if (id == IID_ICorDebugILFrame4)
	{
		*pInterface = static_cast<ICorDebugILFrame4*>(this);
	}
	else
	{
		*pInterface = NULL;
		printf("CordbFrame - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		return E_NOINTERFACE;
	}
	printf("CordbFrame - QueryInterface - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::RemapFunction(ULONG32 newILOffset)
{
	printf("CordbFrame - RemapFunction - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
	printf("CordbFrame - EnumerateTypeParameters - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetReturnValueForILOffset(ULONG32 ILoffset, ICorDebugValue** ppReturnValue)
{
	printf("CordbFrame - GetReturnValueForILOffset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariablesEx(ILCodeKind flags, ICorDebugValueEnum** ppValueEnum)
{
	printf("CordbFrame - EnumerateLocalVariablesEx - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetLocalVariableEx(ILCodeKind flags, DWORD dwIndex, ICorDebugValue** ppValue)
{
	printf("CordbFrame - GetLocalVariableEx - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetCodeEx(ILCodeKind flags, ICorDebugCode** ppCode)
{
	if (flags == ILCODE_REJIT_IL) {
		*ppCode = NULL;
	}
	else {
		CordbCode* code = new CordbCode(NULL);
		*ppCode = static_cast<ICorDebugCode*>(code);
	}
	printf("CordbJITILFrame - GetCodeEx - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbJITILFrame::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbJITILFrame::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetIP(
	/* [out] */ ULONG32* pnOffset,
	/* [out] */ CorDebugMappingResult* pMappingResult)
{
	*pnOffset = il_offset;
	*pMappingResult = MAPPING_EXACT;
	printf("CordbFrame - GetIP - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}
HRESULT STDMETHODCALLTYPE CordbJITILFrame::SetIP(
	/* [in] */ ULONG32 nOffset)
{
	printf("CordbFrame - SetIP - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateLocalVariables(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	printf("CordbFrame - EnumerateLocalVariables - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetLocalVariable(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_id(&localbuf, frameid);
	buffer_add_int(&localbuf, 1);
	buffer_add_int(&localbuf, dwIndex);

	int cmdId = connection->send_event(CMD_SET_STACK_FRAME, CMD_STACK_FRAME_GET_VALUES, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int type = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int value = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	*ppValue = new CordbValue(ELEMENT_TYPE_I4, value, 4);


	printf("CordbFrame - GetArgument - IMPLEMENTED - dwIndex - %d\n", dwIndex);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::EnumerateArguments(
	/* [out] */ ICorDebugValueEnum** ppValueEnum)
{
	printf("CordbFrame - EnumerateArguments - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetArgument(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_id(&localbuf, frameid);
	buffer_add_int(&localbuf, 1);
	buffer_add_int(&localbuf, -1);

	int cmdId = connection->send_event(CMD_SET_STACK_FRAME, CMD_STACK_FRAME_GET_VALUES, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	
	int type = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int value = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	*ppValue = new CordbValue(ELEMENT_TYPE_I4, value, 4);

	
	printf("CordbFrame - GetArgument - IMPLEMENTED - dwIndex - %d\n", dwIndex);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackDepth(
	/* [out] */ ULONG32* pDepth)
{
	printf("CordbFrame - GetStackDepth - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::GetStackValue(
	/* [in] */ DWORD dwIndex,
	/* [out] */ ICorDebugValue** ppValue)
{
	printf("CordbFrame - GetStackValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbJITILFrame::CanSetIP(
	/* [in] */ ULONG32 nOffset)
{
	printf("CordbFrame - CanSetIP - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbRegisteSet::GetRegistersAvailable(ULONG64* pAvailable)
{
	printf("CordbRegisteSet - GetRegistersAvailable - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

CordbRegisteSet::CordbRegisteSet(guint8* ctx, guint32 ctx_len)
{
	this->ctx = ctx;
	this->ctx_len = ctx_len;
}

HRESULT __stdcall CordbRegisteSet::QueryInterface(REFIID id, void** pInterface)
{
	printf("CordbRegisteSet - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbRegisteSet::GetRegisters(ULONG64 mask, ULONG32 regCount, CORDB_REGISTER regBuffer[])
{
	printf("CordbRegisteSet - GetRegisters - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG __stdcall CordbRegisteSet::Release(void)
{
	return 0;
}

ULONG __stdcall CordbRegisteSet::AddRef(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetRegisters(
	/* [in] */ ULONG64 mask,
	/* [in] */ ULONG32 regCount,
	/* [size_is][in] */ CORDB_REGISTER regBuffer[])
{
	printf("CordbRegisteSet - SetRegisters - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::GetThreadContext(
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][out][in] */ BYTE context[])
{
	AMD64_CONTEXT ctx_amd64;
	memcpy(&ctx_amd64.Rax, ctx, ctx_len);
	memcpy(context, &ctx_amd64, contextSize);
	printf("CordbRegisteSet - GetThreadContext - IMPLEMENTED - %d - %d\n", contextSize, ctx_len);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbRegisteSet::SetThreadContext(
	/* [in] */ ULONG32 contextSize,
	/* [size_is][length_is][in] */ BYTE context[])
{
	printf("CordbRegisteSet - SetThreadContext - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbChainEnum::Next(ULONG celt, ICorDebugChain* chains[], ULONG* pceltFetched)
{
	printf("CordbChainEnum - Next - IMPLEMENTED\n");
	fflush(stdout);
	chains[0] = new CordbChain(thread, CHAIN_PROCESS_START, false);
	chains[1] = new CordbChain(thread, CHAIN_ENTER_MANAGED, true);
	*pceltFetched = celt;
	return S_OK;
}

CordbChainEnum::CordbChainEnum(CordbThread* thread)
{
	this->thread = thread;
}

HRESULT __stdcall CordbChainEnum::QueryInterface(REFIID id, void** pInterface)
{
	printf("CordbChainEnum - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT __stdcall CordbChainEnum::Skip(ULONG celt)
{
	printf("CordbChainEnum - Skip - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbChainEnum::AddRef(void)
{
	return 0;
}
ULONG STDMETHODCALLTYPE CordbChainEnum::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::Reset(void)
{
	printf("CordbChainEnum - Reset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::Clone(
	/* [out] */ ICorDebugEnum** ppEnum)
{
	printf("CordbChainEnum - Clone - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChainEnum::GetCount(
	/* [out] */ ULONG* pcelt)
{
	printf("CordbChainEnum - GetCount - IMPLEMENTED\n");
	fflush(stdout);
	*pcelt = 2;
	return S_OK;
}

CordbChain::CordbChain(CordbThread* thread, CorDebugChainReason chain_reason, bool is_managed)
{
	this->thread = thread;
	this->chain_reason = chain_reason;
	this->is_managed = is_managed;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetThread(/* [out] */ ICorDebugThread** ppThread)
{
	*ppThread = static_cast<ICorDebugThread*>(thread);
	printf("CordbChain - GetThread - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetStackRange(/* [out] */ CORDB_ADDRESS* pStart, /* [out] */ CORDB_ADDRESS* pEnd)
{
	printf("CordbChain - GetStackRange - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetContext(/* [out] */ ICorDebugContext** ppContext)
{
	printf("CordbChain - GetContext - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetCaller(/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbChain - GetCaller - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetCallee(/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbChain - GetCallee - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetPrevious(/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbChain - GetPrevious - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetNext(/* [out] */ ICorDebugChain** ppChain)
{
	printf("CordbChain - GetNext - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::IsManaged(/* [out] */ BOOL* pManaged)
{
	printf("CordbChain - IsManaged - IMPLEMENTED\n");
	*pManaged = is_managed;
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::EnumerateFrames(/* [out] */ ICorDebugFrameEnum** ppFrames)
{
	*ppFrames = new CordbFrameEnum(thread);
	printf("CordbChain - EnumerateFrames - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetActiveFrame(/* [out] */ ICorDebugFrame** ppFrame)
{
	printf("CordbChain - GetActiveFrame - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetRegisterSet(/* [out] */ ICorDebugRegisterSet** ppRegisters)
{
	printf("CordbChain - GetRegisterSet - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbChain::GetReason(/* [out] */ CorDebugChainReason* pReason)
{
	*pReason = chain_reason;
	printf("CordbChain - GetReason - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbChain::QueryInterface(/* [in] */ REFIID id, /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR* __RPC_FAR* pInterface)
{
	printf("CordbChain - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbChain::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbChain::Release(void)
{
	return 0;
}

CordbFrameEnum::CordbFrameEnum(CordbThread* thread)
{
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Next(ULONG celt, ICorDebugFrame* frames[], ULONG* pceltFetched)
{
	for (int i = 0; i < nframes; i++)
	{
		frames[i] = this->frames[i];
	}
	printf("CordbFrameEnum - Next - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Skip(ULONG celt)
{
	printf("CordbFrameEnum - Skip - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Reset(void)
{
	printf("CordbFrameEnum - Reset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::Clone(ICorDebugEnum** ppEnum)
{
	printf("CordbFrameEnum - Clone - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::GetCount(ULONG* pcelt)
{
	printf("CordbFrameEnum::GetCount\n");
	fflush(stdout);
	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, thread->thread_id);
	buffer_add_int(&localbuf, 0);
	buffer_add_int(&localbuf, -1);

	int cmdId = connection->send_event(CMD_SET_THREAD, CMD_THREAD_GET_FRAME_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		printf("CordbFrameEnum::GetCount - 1\n");
		fflush(stdout);
		connection->process_packet(true);
		printf("CordbFrameEnum::GetCount - 2\n");
		fflush(stdout);
		localbuf2 = (Buffer*)g_hash_table_lookup(connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	printf("CordbFrameEnum::GetCount - 3\n");
	fflush(stdout);
	nframes = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	
	printf("CordbFrameEnum::GetCount - nframes - %d\n", nframes);
	fflush(stdout);

	frames = (CordbNativeFrame**) malloc(sizeof(CordbNativeFrame*) * nframes);
	printf("CordbFrameEnum::GetCount - 4\n");
	fflush(stdout);
	for (int i = 0; i < nframes; i++)
	{
		printf("CordbFrameEnum::GetCount - 5\n");
		fflush(stdout);
		int frameid = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int methoid = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int il_offset = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
		int flags = decode_byte(localbuf2->buf, &localbuf2->buf, localbuf2->end);

		CordbNativeFrame* frame = new CordbNativeFrame(frameid, methoid, il_offset, flags, thread);
		frames[i] = frame;
	}
	printf("CordbFrameEnum::GetCount - 6\n");
	fflush(stdout);

	*pcelt = nframes;
	printf("CordbFrameEnum - GetCount - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbFrameEnum::QueryInterface(REFIID riid, void** ppvObject)
{
	printf("CordbFrameEnum - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbFrameEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbFrameEnum::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Next(ULONG celt, CorDebugBlockingObject values[], ULONG* pceltFetched)
{
	printf("CordbBlockingObjectEnum - Next - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Skip(ULONG celt)
{
	printf("CordbBlockingObjectEnum - Skip - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Reset(void)
{
	printf("CordbBlockingObjectEnum - Reset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::Clone(ICorDebugEnum** ppEnum)
{
	printf("CordbBlockingObjectEnum - Clone - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::GetCount(ULONG* pcelt)
{
	pcelt = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbBlockingObjectEnum::QueryInterface(REFIID riid, void** ppvObject)
{
	printf("CordbBlockingObjectEnum - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbBlockingObjectEnum::Release(void)
{
	return 0;
}

CordbStepper::CordbStepper(CordbThread* thread)
{
	this->thread = thread;
	hasStepped = false;
}

HRESULT STDMETHODCALLTYPE CordbStepper::IsActive(BOOL* pbActive)
{
	printf("CordbStepper - IsActive - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Deactivate(void)
{
	printf("CordbStepper - Deactivate - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetInterceptMask(CorDebugIntercept mask)
{
	printf("CordbStepper - SetInterceptMask - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetUnmappedStopMask(CorDebugUnmappedStop mask)
{
	printf("CordbStepper - SetUnmappedStopMask - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::Step(BOOL bStepIn)
{
	printf("CordbStepper - Step - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepRange(BOOL bStepIn, COR_DEBUG_STEP_RANGE ranges[], ULONG32 cRangeCount)
{
	hasStepped = true;
	//thread->ppProcess->suspended = false;
	/*
	step_thread_id = decode_id (p, &p, end);
				size = (StepSize)decode_int (p, &p, end);
				depth = (StepDepth)decode_int (p, &p, end);
				if (CHECK_PROTOCOL_VERSION (2, 16))
					filter = (StepFilter)decode_int (p, &p, end);
				req->modifiers [i].data.filter = filter;
				if (!CHECK_PROTOCOL_VERSION (2, 26) && (req->modifiers [i].data.filter & STEP_FILTER_DEBUGGER_HIDDEN))
					/* Treat STEP_THOUGH the same as HIDDEN
	req->modifiers[i].data.filter = (StepFilter)(req->modifiers[i].data.filter | STEP_FILTER_DEBUGGER_STEP_THROUGH); */

	Buffer sendbuf;
	int buflen = 128;
	buffer_init(&sendbuf, buflen);
	buffer_add_byte(&sendbuf, EVENT_KIND_STEP);
	buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
	buffer_add_byte(&sendbuf, 1); //modifiers
	buffer_add_byte(&sendbuf, MOD_KIND_STEP);

	buffer_add_id(&sendbuf, thread->thread_id);
	buffer_add_int(&sendbuf, STEP_SIZE_MIN);
	buffer_add_int(&sendbuf, bStepIn ? STEP_DEPTH_INTO : STEP_DEPTH_OVER);
	buffer_add_int(&sendbuf, STEP_FILTER_NONE);

	connection->send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
	buffer_free(&sendbuf);

	printf("CordbStepper - StepRange - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbStepper::StepOut(void)
{
	printf("CordbStepper - StepOut - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetRangeIL(BOOL bIL)
{
	printf("CordbStepper - SetRangeIL - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::QueryInterface(REFIID riid, void** ppvObject)
{
	printf("CordbStepper - QueryInterface - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::AddRef(void)
{
	printf("CordbStepper - AddRef - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

ULONG STDMETHODCALLTYPE CordbStepper::Release(void)
{
	printf("CordbStepper - Release - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbStepper::SetJMC(BOOL fIsJMCStepper)
{
	printf("CordbStepper - SetJMC - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

CordbNativeFrame::CordbNativeFrame(int frameid, int methoid, int il_offset, int flags, CordbThread* thread)
{
	m_JITILFrame = new CordbJITILFrame(frameid, methoid, il_offset, flags, thread);
	this->thread = thread;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetIP(ULONG32* pnOffset)
{
	printf("CordbNativeFrame - GetIP - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::SetIP(ULONG32 nOffset)
{
	printf("CordbNativeFrame - SetIP - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetRegisterSet(ICorDebugRegisterSet** ppRegisters)
{
	return thread->GetRegisterSet(ppRegisters);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterValue(CorDebugRegister reg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	printf("CordbNativeFrame - GetLocalRegisterValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalDoubleRegisterValue(CorDebugRegister highWordReg, CorDebugRegister lowWordReg, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	printf("CordbNativeFrame - GetLocalDoubleRegisterValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryValue(CORDB_ADDRESS address, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	printf("CordbNativeFrame - GetLocalMemoryValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalRegisterMemoryValue(CorDebugRegister highWordReg, CORDB_ADDRESS lowWordAddress, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	printf("CordbNativeFrame - GetLocalRegisterMemoryValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetLocalMemoryRegisterValue(CORDB_ADDRESS highWordAddress, CorDebugRegister lowWordRegister, ULONG cbSigBlob, PCCOR_SIGNATURE pvSigBlob, ICorDebugValue** ppValue)
{
	printf("CordbNativeFrame - GetLocalMemoryRegisterValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CanSetIP(ULONG32 nOffset)
{
	printf("CordbNativeFrame - CanSetIP - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetChain(ICorDebugChain** ppChain)
{
	printf("CordbNativeFrame - GetChain - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCode(ICorDebugCode** ppCode)
{
	printf("CordbNativeFrame - GetCode - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL; 
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunction(ICorDebugFunction** ppFunction)
{
	return m_JITILFrame->GetFunction(ppFunction);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetFunctionToken(mdMethodDef* pToken)
{
	printf("CordbNativeFrame - GetFunctionToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackRange(CORDB_ADDRESS* pStart, CORDB_ADDRESS* pEnd)
{
	return m_JITILFrame->GetStackRange(pStart, pEnd);
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCaller(ICorDebugFrame** ppFrame)
{
	printf("CordbNativeFrame - GetCaller - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetCallee(ICorDebugFrame** ppFrame)
{
	printf("CordbNativeFrame - GetCallee - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::CreateStepper(ICorDebugStepper** ppStepper)
{
	printf("CordbNativeFrame - CreateStepper - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugFrame)
	{
		*pInterface = static_cast<ICorDebugFrame*>(static_cast<ICorDebugNativeFrame*>(this));
	}
	else if (id == IID_ICorDebugNativeFrame)
	{
		*pInterface = static_cast<ICorDebugNativeFrame*>(this);
	}
	else if (id == IID_ICorDebugNativeFrame2)
	{
		*pInterface = static_cast<ICorDebugNativeFrame2*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugNativeFrame*>(this));
	}
	else
	{
		// might be searching for an IL Frame. delegate that search to the
		// JITILFrame
		if (m_JITILFrame != NULL)
		{
			return m_JITILFrame->QueryInterface(id, pInterface);
		}
		else
		{
			*pInterface = NULL;
			return E_NOINTERFACE;
		}
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbNativeFrame::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbNativeFrame::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::IsChild(BOOL* pIsChild)
{
	printf("CordbNativeFrame - IsChild - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::IsMatchingParentFrame(ICorDebugNativeFrame2* pPotentialParentFrame, BOOL* pIsParent)
{
	printf("CordbNativeFrame - IsMatchingParentFrame - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbNativeFrame::GetStackParameterSize(ULONG32* pSize)
{
	printf("CordbNativeFrame - GetStackParameterSize - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

CordbClass::CordbClass(mdToken token)
{
	this->token = token;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetModule(ICorDebugModule** pModule)
{
	printf("CordbNativeFrame - GetModule - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetToken(mdTypeDef* pTypeDef)
{
	printf("CordbNativeFrame - GetToken - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue)
{
	CordbValue* value = new CordbValue(ELEMENT_TYPE_BOOLEAN, 0, 1);
	*ppValue = static_cast<ICorDebugValue*>(value);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbClass::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugClass)
	{
		*pInterface = static_cast<ICorDebugClass*>(this);
	}
	else if (id == IID_ICorDebugClass2)
	{
		*pInterface = static_cast<ICorDebugClass2*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugClass*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbClass::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbClass::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbClass::GetParameterizedType(CorElementType elementType, ULONG32 nTypeArgs, ICorDebugType* ppTypeArgs[], ICorDebugType** ppType)
{
	printf("CordbClass - GetParameterizedType - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbClass::SetJMCStatus(BOOL bIsJustMyCode)
{
	printf("CordbClass - SetJMCStatus - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

CordbValue::CordbValue(CorElementType type, int value, int size)
{
	this->type = type;
	this->value = value;
	this->size = size;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetType(CorElementType* pType)
{
	*pType = type;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize(ULONG32* pSize)
{
	*pSize = size;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetAddress(CORDB_ADDRESS* pAddress)
{
	*pAddress = (CORDB_ADDRESS)&value;
	printf("CordbValue - GetAddress - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::CreateBreakpoint(ICorDebugValueBreakpoint** ppBreakpoint)
{
	printf("CordbValue - CreateBreakpoint - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugValue)
	{
		*pInterface = static_cast<ICorDebugValue*>(static_cast<ICorDebugGenericValue*>(this));
	}
	else if (id == IID_ICorDebugValue2)
	{
		*pInterface = static_cast<ICorDebugValue2*>(this);
	}
	else if (id == IID_ICorDebugValue3)
	{
		*pInterface = static_cast<ICorDebugValue3*>(this);
	}
	else if (id == IID_ICorDebugGenericValue)
	{
		*pInterface = static_cast<ICorDebugGenericValue*>(this);
	}
	else if (id == IID_IUnknown)
	{
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugGenericValue*>(this));
	}
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbValue::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbValue::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetExactType(ICorDebugType** ppType)
{
	printf("CordbValue - GetExactType - IMPLEMENTED\n");
	CordbType *tp = new CordbType(type);
	*ppType = static_cast<ICorDebugType*>(tp);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetSize64(ULONG64* pSize)
{
	printf("CordbValue - GetSize64 - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbValue::GetValue(void* pTo)
{
	printf("CordbValue - GetValue - NOT IMPLEMENTED\n"); \
	memcpy(pTo, &value, size);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbValue::SetValue(void* pFrom)
{
	printf("CordbValue - SetValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

CordbType::CordbType(CorElementType type)
{
	this->type = type;
}

HRESULT STDMETHODCALLTYPE CordbType::GetType(CorElementType* ty)
{
	*ty = type;
	printf("CordbType - GetType - IMPLEMENTED - %d\n", type);
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetClass(ICorDebugClass** ppClass)
{
	printf("CordbType - GetClass - NOT IMPLEMENTED\n");
	fflush(stdout);
	return S_FALSE;
}

HRESULT STDMETHODCALLTYPE CordbType::EnumerateTypeParameters(ICorDebugTypeEnum** ppTyParEnum)
{
	CordbTypeEnum* tp = new CordbTypeEnum();
	*ppTyParEnum = static_cast<ICorDebugTypeEnum*>(tp);

	printf("CordbType - EnumerateTypeParameters - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbType::GetFirstTypeParameter(ICorDebugType** value)
{
	printf("CordbType - GetFirstTypeParameter - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetBase(ICorDebugType** pBase)
{
	printf("CordbType - GetBase - IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetStaticFieldValue(mdFieldDef fieldDef, ICorDebugFrame* pFrame, ICorDebugValue** ppValue)
{
	printf("CordbType - GetStaticFieldValue - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::GetRank(ULONG32* pnRank)
{
	printf("CordbType - GetRank - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbType::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugType)
		*pInterface = static_cast<ICorDebugType*>(this);
	else if (id == IID_ICorDebugType2)
		*pInterface = static_cast<ICorDebugType2*>(this);
	else if (id == IID_IUnknown)
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugType*>(this));
	else
	{
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbType::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbType::Release(void)
{
	return 0;
}

HRESULT STDMETHODCALLTYPE CordbType::GetTypeID(COR_TYPEID* id)
{
	printf("CordbType - GetTypeID - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Next(ULONG celt, ICorDebugType* values[], ULONG* pceltFetched)
{
	*pceltFetched = celt;
	printf("CordbTypeEnum - Next - IMPLEMENTED\n");
	fflush(stdout);
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Skip(ULONG celt)
{
	printf("CordbTypeEnum - Skip - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Reset(void)
{
	printf("CordbTypeEnum - Reset - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::Clone(ICorDebugEnum** ppEnum)
{
	printf("CordbTypeEnum - Clone - NOT IMPLEMENTED\n");
	fflush(stdout);
	return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::GetCount(ULONG* pcelt)
{
	*pcelt = 0;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE CordbTypeEnum::QueryInterface(REFIID id, void** pInterface)
{
	if (id == IID_ICorDebugEnum)
		*pInterface = static_cast<ICorDebugEnum*>(this);
	else if (id == IID_ICorDebugTypeEnum)
		*pInterface = static_cast<ICorDebugTypeEnum*>(this);
	else if (id == IID_IUnknown)
		*pInterface = static_cast<IUnknown*>(static_cast<ICorDebugTypeEnum*>(this));
	else
	{
		printf("CordbTypeEnum - QueryInterface - E_NOTIMPL - %x - %x - %x - %x\n", ((unsigned long*)&id)[0], ((unsigned long*)&id)[1], ((unsigned long*)&id)[2], ((unsigned long*)&id)[3]);
		fflush(stdout);
		*pInterface = NULL;
		return E_NOINTERFACE;
	}

	return S_OK;
}

ULONG STDMETHODCALLTYPE CordbTypeEnum::AddRef(void)
{
	return 0;
}

ULONG STDMETHODCALLTYPE CordbTypeEnum::Release(void)
{
	return 0;
}
