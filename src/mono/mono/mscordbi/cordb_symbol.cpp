#include <iostream>
#include <fstream>

#include <cordb.hpp>
#include <cordb_frame.hpp>
#include <cordb_thread.hpp>
#include <cordb_stepper.hpp>
#include <cordb_symbol.hpp>
#include <cordb_assembly.hpp>
#include <cordb_process.hpp>
#include <cordb_function.hpp>

using namespace std;


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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumGenericParams - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetGenericParamProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}       // [OUT] Put size of name (wide chars) here.

HRESULT CordbSymbol::GetMethodSpecProps(
	mdMethodSpec mi,                    // [IN] The method instantiation
	mdToken* tkParent,                  // [OUT] MethodDef or MemberRef
	PCCOR_SIGNATURE* ppvSigBlob,        // [OUT] point to the blob value of meta data
	ULONG* pcbSigBlob) {

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetMethodSpecProps - NOT IMPLEMENTED" << endl;
	file.close();

	return S_OK;
}     // [OUT] actual size of signature blob

HRESULT CordbSymbol::EnumGenericParamConstraints(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdGenericParam tk,                  // [IN] GenericParam whose constraints are requested
	mdGenericParamConstraint rGenericParamConstraints[],    // [OUT] Put GenericParamConstraints here.
	ULONG       cMax,                   // [IN] Max GenericParamConstraints to put.
	ULONG* pcGenericParamConstraints) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumGenericParamConstraints - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}// [OUT] Put # put here.

HRESULT CordbSymbol::GetGenericParamConstraintProps( // S_OK or error.
	mdGenericParamConstraint gpc,       // [IN] GenericParamConstraint
	mdGenericParam* ptGenericParam,     // [OUT] GenericParam that is constrained
	mdToken* ptkConstraintType) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetGenericParamConstraintProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}// [OUT] TypeDef/Ref/Spec constraint

HRESULT CordbSymbol::GetPEKind(                   // S_OK or error.
	DWORD* pdwPEKind,                   // [OUT] The kind of PE (0 - not a PE)
	DWORD* pdwMAchine) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetPEKind - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}           // [OUT] Machine as defined in NT header

HRESULT CordbSymbol::GetVersionString(            // S_OK or error.
	_Out_writes_to_opt_(ccBufSize, *pccBufSize)
	LPWSTR      pwzBuf,                 // [OUT] Put version string here.
	DWORD       ccBufSize,              // [IN] size of the buffer, in wide chars
	DWORD* pccBufSize) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetVersionString - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}     // [OUT] Size of the version string, wide chars, including terminating nul.

HRESULT CordbSymbol::EnumMethodSpecs(
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdToken      tk,                    // [IN] MethodDef or MemberRef whose MethodSpecs are requested
	mdMethodSpec rMethodSpecs[],        // [OUT] Put MethodSpecs here.
	ULONG       cMax,                   // [IN] Max tokens to put.
	ULONG* pcMethodSpecs) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMethodSpecs - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetAssemblyProps - NOT IMPLEMENTED" << std::endl;
	*pchName = 23;
	if (cchName >= 23)
	{
		mbstowcs(szName, "System.Private.CoreLib", strlen("System.Private.CoreLib") + 1);
	}
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetAssemblyRefProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetFileProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetExportedTypeProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetManifestResourceProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}// [OUT] Flags.

HRESULT CordbSymbol::EnumAssemblyRefs(            // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdAssemblyRef rAssemblyRefs[],      // [OUT] Put AssemblyRefs here.
	ULONG       cMax,                   // [IN] Max AssemblyRefs to put.
	ULONG* pcTokens) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumAssemblyRefs - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumFiles(                   // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdFile      rFiles[],               // [OUT] Put Files here.
	ULONG       cMax,                   // [IN] Max Files to put.
	ULONG* pcTokens) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumFiles - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumExportedTypes(           // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdExportedType   rExportedTypes[],  // [OUT] Put ExportedTypes here.
	ULONG       cMax,                   // [IN] Max ExportedTypes to put.
	ULONG* pcTokens) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumExportedTypes - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::EnumManifestResources(       // S_OK or error
	HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
	mdManifestResource  rManifestResources[],   // [OUT] Put ManifestResources here.
	ULONG       cMax,                   // [IN] Max Resources to put.
	ULONG* pcTokens) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumManifestResources - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}       // [OUT] Put # put here.

HRESULT CordbSymbol::FindExportedTypeByName(      // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ExportedType.
	mdToken     mdtExportedType,        // [IN] ExportedType for the enclosing class.
	mdExportedType* ptkExportedType) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindExportedTypeByName - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}// [OUT] Put the ExportedType token here.

HRESULT CordbSymbol::FindManifestResourceByName(  // S_OK or error
	LPCWSTR     szName,                 // [IN] Name of the ManifestResource.
	mdManifestResource* ptkManifestResource) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindManifestResourceByName - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
} // [OUT] Put the ManifestResource token here.

HRESULT CordbSymbol::FindAssembliesByName(        // S_OK or error
	LPCWSTR  szAppBase,                 // [IN] optional - can be NULL
	LPCWSTR  szPrivateBin,              // [IN] optional - can be NULL
	LPCWSTR  szAssemblyName,            // [IN] required - this is the assembly you are requesting
	IUnknown* ppIUnk[],                 // [OUT] put IMetaDataAssemblyImport pointers here
	ULONG    cMax,                      // [IN] The max number to put
	ULONG* pcAssemblies) {
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindAssembliesByName - NOT IMPLEMENTED" << std::endl;
	file.close();
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
					fstream file;
					file.open("c:\\thays\\example.txt", ios::out | ios::in | ios::app);
					file << "CordbSymbol - QueryInterface - E_NOTIMPL" << std::endl;
					file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - CloseEnum - NOT IMPLEMENTED" << std::endl;
	file.close();
	return;
}

HRESULT CordbSymbol::CountEnum(HCORENUM hEnum, ULONG* pulCount)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - CountEnum - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::ResetEnum(HCORENUM hEnum, ULONG ulPos)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - ResetEnum - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeDefs(HCORENUM* phEnum, mdTypeDef rTypeDefs[],
ULONG cMax, ULONG* pcTypeDefs)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumTypeDefs - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumInterfaceImpls(HCORENUM* phEnum, mdTypeDef td,
mdInterfaceImpl rImpls[], ULONG cMax,
ULONG* pcImpls)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumInterfaceImpls - NOT IMPLEMENTED" << std::endl;
	file.close();
	*pcImpls = 0;
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeRefs(HCORENUM* phEnum, mdTypeRef rTypeRefs[],
ULONG cMax, ULONG* pcTypeRefs)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumTypeRefs - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_VM, CMD_VM_GET_TYPES, &sendbuf);
	buffer_free(&sendbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int count_class = decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
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

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindTypeDefByName - IMPLEMENTED" << std::endl;
	file.close();
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

		Buffer* localbuf2 = NULL;
		while (!localbuf2) {
			this->pCordbAssembly->pProcess->connection->process_packet(true);
			localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
		}

		module_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	}

	Buffer localbuf;
	buffer_init(&localbuf, 128);
	buffer_add_id(&localbuf, module_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_MODULE, CMD_MODULE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 1" << std::endl;
	file.close();
	char* basename = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 2" << std::endl;
	file.close();
	char* module_name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 3" << std::endl;
	file.close();
	char* name = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 4" << std::endl;
	file.close();
	char* guid = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 5" << std::endl;
	file.close();
	int assembly_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 6" << std::endl;
	file.close();
	/*char* sourcelink = decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);*/
	guint32 mvid_len;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 7" << std::endl;
	file.close();
	guint8 *mvid = decode_byte_array(localbuf2->buf, &localbuf2->buf, localbuf2->end, &mvid_len);
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED - 8" << std::endl;
	file.close();
	memcpy(pmvid, mvid, mvid_len);
	if (cchName > strlen(name))
	{
		mbstowcs(szName, name, strlen(name) + 1);
		*pchName = strlen(name) + 1;
	}
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetScopeProps - IMPLEMENTED" << std::endl;

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
	

	
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetModuleFromScope(          // S_OK.
mdModule* pmd)            // [OUT] Put mdModule token here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetModuleFromScope - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}
	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
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


	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetTypeDefProps - IMPLEMENTED" << std::endl;
	file.close();
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, type_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);

	localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetInterfaceImplProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetTypeRefProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::ResolveTypeRef(mdTypeRef tr, REFIID riid, IUnknown** ppIScope, mdTypeDef* ptd)
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - ResolveTypeRef - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumMembers(                 // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdToken     rMembers[],             // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMembers - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMembersWithName - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumMethods(                 // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdMethodDef rMethods[],             // [OUT] Put MethodDefs here.
ULONG       cMax,                   // [IN] Max MethodDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMethods - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMethodsWithName - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumFields(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   cl,                     // [IN] TypeDef to scope the enumeration.
mdFieldDef  rFields[],              // [OUT] Put FieldDefs here.
ULONG       cMax,                   // [IN] Max FieldDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumFields - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumFieldsWithName - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbSymbol::EnumParams(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
mdParamDef  rParams[],              // [OUT] Put ParamDefs here.
ULONG       cMax,                   // [IN] Max ParamDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumParams - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumMemberRefs(              // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdToken     tkParent,               // [IN] Parent token to scope the enumeration.
mdMemberRef rMemberRefs[],          // [OUT] Put MemberRefs here.
ULONG       cMax,                   // [IN] Max MemberRefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMemberRefs - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMethodImpls - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumPermissionSets - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::FindMember(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdToken* pmb)             // [OUT] matching memberdef
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindMember - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::FindMethod(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdMethodDef* pmb)             // [OUT] matching memberdef
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindMethod - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::FindField(
mdTypeDef   td,                     // [IN] given typedef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdFieldDef* pmb)             // [OUT] matching memberdef
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindField - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::FindMemberRef(
mdTypeRef   td,                     // [IN] given typeRef
LPCWSTR     szName,                 // [IN] member name
PCCOR_SIGNATURE pvSigBlob,          // [IN] point to a blob value of CLR signature
ULONG       cbSigBlob,              // [IN] count of bytes in the signature blob
mdMemberRef* pmr)             // [OUT] matching memberref
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindMemberRef - NOT IMPLEMENTED" << std::endl;
	file.close();
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
		int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_ASSEMBLY, CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN, &localbuf);
		buffer_free(&localbuf);

		Buffer* localbuf2 = NULL;
		while (!localbuf2) {
			this->pCordbAssembly->pProcess->connection->process_packet(true);
			localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
		}

		func_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	}
	else
		func_id = func->id;

	Buffer localbuf;
	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, func_id);
	int cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_METHOD, CMD_METHOD_GET_DECLARING_TYPE, &localbuf);
	buffer_free(&localbuf);

	Buffer* localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
	}

	int klass_id = decode_id(localbuf2->buf, &localbuf2->buf, localbuf2->end);

	buffer_init(&localbuf, 128);

	buffer_add_id(&localbuf, klass_id);
	cmdId = this->pCordbAssembly->pProcess->connection->send_event(CMD_SET_TYPE, CMD_TYPE_GET_INFO, &localbuf);
	buffer_free(&localbuf);
	
	localbuf2 = NULL;
	while (!localbuf2) {
		this->pCordbAssembly->pProcess->connection->process_packet(true);
		localbuf2 = (Buffer*)g_hash_table_lookup(this->pCordbAssembly->pProcess->connection->received_replies, (gpointer)(gssize)(cmdId));
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetMethodProps - IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetMemberRefProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumProperties(              // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
mdProperty  rProperties[],          // [OUT] Put Properties here.
ULONG       cMax,                   // [IN] Max properties to put.
ULONG* pcProperties)    // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumProperties - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumEvents(                  // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdTypeDef   td,                     // [IN] TypeDef to scope the enumeration.
mdEvent     rEvents[],              // [OUT] Put events here.
ULONG       cMax,                   // [IN] Max events to put.
ULONG* pcEvents)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumEvents - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetEventProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumMethodSemantics(         // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdMethodDef mb,                     // [IN] MethodDef to scope the enumeration.
mdToken     rEventProp[],           // [OUT] Put Event/Property here.
ULONG       cMax,                   // [IN] Max properties to put.
ULONG* pcEventProp)     // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumMethodSemantics - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetMethodSemantics(          // S_OK, S_FALSE, or error.
mdMethodDef mb,                     // [IN] method token
mdToken     tkEventProp,            // [IN] event/property token.
DWORD* pdwSemanticsFlags) // [OUT] the role flags for the method/propevent pair
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetMethodSemantics - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetClassLayout - NOT IMPLEMENTED" << std::endl;
	file.close();
	return 0x80131130;
}

HRESULT CordbSymbol::GetFieldMarshal(
mdToken     tk,                     // [IN] given a field's memberdef
PCCOR_SIGNATURE* ppvNativeType,     // [OUT] native type of this field
ULONG* pcbNativeType)   // [OUT] the count of bytes of *ppvNativeType
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetFieldMarshal - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetRVA(                      // S_OK or error.
mdToken     tk,                     // Member for which to set offset
ULONG* pulCodeRVA,            // The offset
DWORD* pdwImplFlags)    // the implementation flags
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetRVA - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetPermissionSetProps(
mdPermission pm,                    // [IN] the permission token.
DWORD* pdwAction,             // [OUT] CorDeclSecurity.
void const** ppvPermission,        // [OUT] permission blob.
ULONG* pcbPermission)   // [OUT] count of bytes of pvPermission.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetPermissionSetProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetSigFromToken(             // S_OK or error.
mdSignature mdSig,                  // [IN] Signature token.
PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to token.
ULONG* pcbSig)          // [OUT] return size of signature.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetSigFromToken - NOT IMPLEMENTED" << std::endl;
	file.close();

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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetModuleRefProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumModuleRefs(              // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdModuleRef rModuleRefs[],          // [OUT] put modulerefs here.
ULONG       cmax,                   // [IN] max memberrefs to put.
ULONG* pcModuleRefs)    // [OUT] put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumModuleRefs - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetTypeSpecFromToken(        // S_OK or error.
mdTypeSpec typespec,                // [IN] TypeSpec token.
PCCOR_SIGNATURE* ppvSig,            // [OUT] return pointer to TypeSpec signature
ULONG* pcbSig)          // [OUT] return size of signature.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetTypeSpecFromToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetNameFromToken(            // Not Recommended! May be removed!
mdToken     tk,                     // [IN] Token to get name from.  Must have a name.
MDUTF8CSTR* pszUtf8NamePtr)  // [OUT] Return pointer to UTF8 name in heap.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetNameFromToken - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumUnresolvedMethods(       // S_OK, S_FALSE, or error.
HCORENUM* phEnum,                // [IN|OUT] Pointer to the enum.
mdToken     rMethods[],             // [OUT] Put MemberDefs here.
ULONG       cMax,                   // [IN] Max MemberDefs to put.
ULONG* pcTokens)        // [OUT] Put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumUnresolvedMethods - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetUserString(               // S_OK or error.
mdString    stk,                    // [IN] String token.
__out_ecount_part_opt(cchString, *pchString)
LPWSTR      szString,               // [OUT] Copy of string.
ULONG       cchString,              // [IN] Max chars of room in szString.
ULONG* pchString)       // [OUT] How many chars in actual string.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetUserString - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetPinvokeMap - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumSignatures(              // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdSignature rSignatures[],          // [OUT] put signatures here.
ULONG       cmax,                   // [IN] max signatures to put.
ULONG* pcSignatures)    // [OUT] put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumSignatures - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::EnumTypeSpecs(               // S_OK or error.
HCORENUM* phEnum,                // [IN|OUT] pointer to the enum.
mdTypeSpec  rTypeSpecs[],           // [OUT] put TypeSpecs here.
ULONG       cmax,                   // [IN] max TypeSpecs to put.
ULONG* pcTypeSpecs)     // [OUT] put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumTypeSpecs - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}


HRESULT CordbSymbol::EnumUserStrings(             // S_OK or error.
HCORENUM* phEnum,                // [IN/OUT] pointer to the enum.
mdString    rStrings[],             // [OUT] put Strings here.
ULONG       cmax,                   // [IN] max Strings to put.
ULONG* pcStrings)       // [OUT] put # put here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumUserStrings - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetParamForMethodIndex(      // S_OK or error.
mdMethodDef md,                     // [IN] Method token.
ULONG       ulParamSeq,             // [IN] Parameter sequence.
mdParamDef* ppd)             // [IN] Put Param token here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetParamForMethodIndex - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - EnumCustomAttributes - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetCustomAttributeProps(     // S_OK or error.
mdCustomAttribute cv,               // [IN] CustomAttribute token.
mdToken* ptkObj,                // [OUT, OPTIONAL] Put object token here.
mdToken* ptkType,               // [OUT, OPTIONAL] Put AttrType token here.
void const** ppBlob,               // [OUT, OPTIONAL] Put pointer to data here.
ULONG* pcbSize)         // [OUT, OPTIONAL] Put size of date here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetCustomAttributeProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::FindTypeRef(
mdToken     tkResolutionScope,      // [IN] ModuleRef, AssemblyRef or TypeRef.
LPCWSTR     szName,                 // [IN] TypeRef Name.
mdTypeRef* ptr)             // [OUT] matching TypeRef.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - FindTypeRef - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetMemberProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetFieldProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetPropertyProps - NOT IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetParamProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetCustomAttributeByName(    // S_OK or error.
mdToken     tkObj,                  // [IN] Object with Custom Attribute.
LPCWSTR     szName,                 // [IN] Name of desired Custom Attribute.
const void** ppData,               // [OUT] Put pointer to data here.
ULONG* pcbData)         // [OUT] Put size of data here.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetCustomAttributeByName - IMPLEMENTED" << std::endl;
	file.close();
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
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - IsValidToken - IMPLEMENTED" << std::endl;
	file.close();
	return 1;
}

HRESULT CordbSymbol::GetNestedClassProps(         // S_OK or error.
mdTypeDef   tdNestedClass,          // [IN] NestedClass token.
mdTypeDef* ptdEnclosingClass) // [OUT] EnclosingClass token.
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetNestedClassProps - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::GetNativeCallConvFromSig(    // S_OK or error.
void const* pvSig,                 // [IN] Pointer to signature.
ULONG       cbSig,                  // [IN] Count of signature bytes.
ULONG* pCallConv)       // [OUT] Put calling conv here (see CorPinvokemap).
{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - GetNativeCallConvFromSig - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

HRESULT CordbSymbol::IsGlobal(                    // S_OK or error.
mdToken     pd,                     // [IN] Type, Field, or Method token.
int* pbGlobal)        // [OUT] Put 1 if global, 0 otherwise.

{
	fstream file;
	file.open ("c:\\thays\\example.txt", ios::out | ios::in | ios::app );
	file << "CordbSymbol - IsGlobal - NOT IMPLEMENTED" << std::endl;
	file.close();
	return S_OK;
}

