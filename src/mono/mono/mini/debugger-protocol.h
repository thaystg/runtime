#ifndef __MONO_DEBUGGER_PROTOCOL_H__
#define __MONO_DEBUGGER_PROTOCOL_H__

#include "mini.h"
#include "debugger-engine.h"

#define HEADER_LEN 11
#define REPLY_PACKET 0x80

typedef enum {
	CMD_COMPOSITE = 100
} CmdComposite;

typedef enum {
	CMD_VM_VERSION = 1,
	CMD_VM_ALL_THREADS = 2,
	CMD_VM_SUSPEND = 3,
	CMD_VM_RESUME = 4,
	CMD_VM_EXIT = 5,
	CMD_VM_DISPOSE = 6,
	CMD_VM_INVOKE_METHOD = 7,
	CMD_VM_SET_PROTOCOL_VERSION = 8,
	CMD_VM_ABORT_INVOKE = 9,
	CMD_VM_SET_KEEPALIVE = 10,
	CMD_VM_GET_TYPES_FOR_SOURCE_FILE = 11,
	CMD_VM_GET_TYPES = 12,
	CMD_VM_INVOKE_METHODS = 13,
	CMD_VM_START_BUFFERING = 14,
	CMD_VM_STOP_BUFFERING = 15
} CmdVM;

typedef enum {
	CMD_SET_VM = 1,
	CMD_SET_OBJECT_REF = 9,
	CMD_SET_STRING_REF = 10,
	CMD_SET_THREAD = 11,
	CMD_SET_ARRAY_REF = 13,
	CMD_SET_EVENT_REQUEST = 15,
	CMD_SET_STACK_FRAME = 16,
	CMD_SET_APPDOMAIN = 20,
	CMD_SET_ASSEMBLY = 21,
	CMD_SET_METHOD = 22,
	CMD_SET_TYPE = 23,
	CMD_SET_MODULE = 24,
	CMD_SET_FIELD = 25,
	CMD_SET_EVENT = 64,
	CMD_SET_POINTER = 65
} CommandSet;



typedef enum {
	ERR_NONE = 0,
	ERR_INVALID_OBJECT = 20,
	ERR_INVALID_FIELDID = 25,
	ERR_INVALID_FRAMEID = 30,
	ERR_NOT_IMPLEMENTED = 100,
	ERR_NOT_SUSPENDED = 101,
	ERR_INVALID_ARGUMENT = 102,
	ERR_UNLOADED = 103,
	ERR_NO_INVOCATION = 104,
	ERR_ABSENT_INFORMATION = 105,
	ERR_NO_SEQ_POINT_AT_IL_OFFSET = 106,
	ERR_INVOKE_ABORTED = 107,
	ERR_LOADER_ERROR = 200, /*XXX extend the protocol to pass this information down the pipe */
} ErrorCode;

typedef enum {
	TOKEN_TYPE_STRING = 0,
	TOKEN_TYPE_TYPE = 1,
	TOKEN_TYPE_FIELD = 2,
	TOKEN_TYPE_METHOD = 3,
	TOKEN_TYPE_UNKNOWN = 4
} DebuggerTokenType;

typedef enum {
	VALUE_TYPE_ID_NULL = 0xf0,
	VALUE_TYPE_ID_TYPE = 0xf1,
	VALUE_TYPE_ID_PARENT_VTYPE = 0xf2,
	VALUE_TYPE_ID_FIXED_ARRAY = 0xf3
} ValueTypeId;

typedef enum {
	FRAME_FLAG_DEBUGGER_INVOKE = 1,
	FRAME_FLAG_NATIVE_TRANSITION = 2
} StackFrameFlags;

typedef enum {
	INVOKE_FLAG_DISABLE_BREAKPOINTS = 1,
	INVOKE_FLAG_SINGLE_THREADED = 2,
	INVOKE_FLAG_RETURN_OUT_THIS = 4,
	INVOKE_FLAG_RETURN_OUT_ARGS = 8,
	INVOKE_FLAG_VIRTUAL = 16
} InvokeFlags;

typedef enum {
	BINDING_FLAGS_IGNORE_CASE = 0x70000000,
} BindingFlagsExtensions;

typedef enum {
	CMD_THREAD_GET_FRAME_INFO = 1,
	CMD_THREAD_GET_NAME = 2,
	CMD_THREAD_GET_STATE = 3,
	CMD_THREAD_GET_INFO = 4,
	CMD_THREAD_GET_ID = 5,
	CMD_THREAD_GET_TID = 6,
	CMD_THREAD_SET_IP = 7,
	CMD_THREAD_ELAPSED_TIME = 8
} CmdThread;

typedef enum {
	CMD_APPDOMAIN_GET_ROOT_DOMAIN = 1,
	CMD_APPDOMAIN_GET_FRIENDLY_NAME = 2,
	CMD_APPDOMAIN_GET_ASSEMBLIES = 3,
	CMD_APPDOMAIN_GET_ENTRY_ASSEMBLY = 4,
	CMD_APPDOMAIN_CREATE_STRING = 5,
	CMD_APPDOMAIN_GET_CORLIB = 6,
	CMD_APPDOMAIN_CREATE_BOXED_VALUE = 7,
	CMD_APPDOMAIN_CREATE_BYTE_ARRAY = 8,
} CmdAppDomain;

typedef enum {
	CMD_ASSEMBLY_GET_LOCATION = 1,
	CMD_ASSEMBLY_GET_ENTRY_POINT = 2,
	CMD_ASSEMBLY_GET_MANIFEST_MODULE = 3,
	CMD_ASSEMBLY_GET_OBJECT = 4,
	CMD_ASSEMBLY_GET_TYPE = 5,
	CMD_ASSEMBLY_GET_NAME = 6,
	CMD_ASSEMBLY_GET_DOMAIN = 7,
	CMD_ASSEMBLY_GET_METADATA_BLOB = 8,
	CMD_ASSEMBLY_GET_IS_DYNAMIC = 9,
	CMD_ASSEMBLY_GET_PDB_BLOB = 10,
	CMD_ASSEMBLY_GET_TYPE_FROM_TOKEN = 11,
	CMD_ASSEMBLY_GET_METHOD_FROM_TOKEN = 12,
	CMD_ASSEMBLY_HAS_DEBUG_INFO = 13,
	CMD_ASSEMBLY_GET_CUSTOM_ATTRIBUTES = 14,
	CMD_ASSEMBLY_GET_SIGNATURE_FROM_TOKEN = 15,
	CMD_ASSEMBLY_GET_INFO_FROM_MEMBERREF_TOKEN = 16
} CmdAssembly;

typedef enum {
	CMD_MODULE_GET_INFO = 1,
} CmdModule;

typedef enum {
	CMD_FIELD_GET_INFO = 1,
} CmdField;

typedef enum {
	CMD_METHOD_GET_NAME = 1,
	CMD_METHOD_GET_DECLARING_TYPE = 2,
	CMD_METHOD_GET_DEBUG_INFO = 3,
	CMD_METHOD_GET_PARAM_INFO = 4,
	CMD_METHOD_GET_LOCALS_INFO = 5,
	CMD_METHOD_GET_INFO = 6,
	CMD_METHOD_GET_BODY = 7,
	CMD_METHOD_RESOLVE_TOKEN = 8,
	CMD_METHOD_GET_CATTRS = 9,
	CMD_METHOD_MAKE_GENERIC_METHOD = 10,
	CMD_METHOD_RVA = 11,
	CMD_METHOD_TOKEN = 12,
	CMD_METHOD_ASSEMBLY = 13
} CmdMethod;

typedef enum {
	CMD_TYPE_GET_INFO = 1,
	CMD_TYPE_GET_METHODS = 2,
	CMD_TYPE_GET_FIELDS = 3,
	CMD_TYPE_GET_VALUES = 4,
	CMD_TYPE_GET_OBJECT = 5,
	CMD_TYPE_GET_SOURCE_FILES = 6,
	CMD_TYPE_SET_VALUES = 7,
	CMD_TYPE_IS_ASSIGNABLE_FROM = 8,
	CMD_TYPE_GET_PROPERTIES = 9,
	CMD_TYPE_GET_CATTRS = 10,
	CMD_TYPE_GET_FIELD_CATTRS = 11,
	CMD_TYPE_GET_PROPERTY_CATTRS = 12,
	CMD_TYPE_GET_SOURCE_FILES_2 = 13,
	CMD_TYPE_GET_VALUES_2 = 14,
	CMD_TYPE_GET_METHODS_BY_NAME_FLAGS = 15,
	CMD_TYPE_GET_INTERFACES = 16,
	CMD_TYPE_GET_INTERFACE_MAP = 17,
	CMD_TYPE_IS_INITIALIZED = 18,
	CMD_TYPE_CREATE_INSTANCE = 19,
	CMD_TYPE_GET_VALUE_SIZE = 20
} CmdType;

typedef enum {
	CMD_STACK_FRAME_GET_VALUES = 1,
	CMD_STACK_FRAME_GET_THIS = 2,
	CMD_STACK_FRAME_SET_VALUES = 3,
	CMD_STACK_FRAME_GET_DOMAIN = 4,
	CMD_STACK_FRAME_SET_THIS = 5,
	CMD_STACK_FRAME_GET_ARGUMENT = 6,
} CmdStackFrame;

typedef enum {
	CMD_ARRAY_REF_GET_LENGTH = 1,
	CMD_ARRAY_REF_GET_VALUES = 2,
	CMD_ARRAY_REF_SET_VALUES = 3,
	CMD_ARRAY_REF_GET_TYPE = 4
} CmdArray;

typedef enum {
	CMD_STRING_REF_GET_VALUE = 1,
	CMD_STRING_REF_GET_LENGTH = 2,
	CMD_STRING_REF_GET_CHARS = 3
} CmdString;

typedef enum {
	CMD_POINTER_GET_VALUE = 1
} CmdPointer;

typedef enum {
	CMD_OBJECT_REF_GET_TYPE = 1,
	CMD_OBJECT_REF_GET_VALUES = 2,
	CMD_OBJECT_REF_IS_COLLECTED = 3,
	CMD_OBJECT_REF_GET_ADDRESS = 4,
	CMD_OBJECT_REF_GET_DOMAIN = 5,
	CMD_OBJECT_REF_SET_VALUES = 6,
	CMD_OBJECT_REF_GET_INFO = 7,
} CmdObject;

typedef enum {
	SUSPEND_POLICY_NONE = 0,
	SUSPEND_POLICY_EVENT_THREAD = 1,
	SUSPEND_POLICY_ALL = 2
} SuspendPolicy;

typedef enum {
	CMD_EVENT_REQUEST_SET = 1,
	CMD_EVENT_REQUEST_CLEAR = 2,
	CMD_EVENT_REQUEST_CLEAR_ALL_BREAKPOINTS = 3
} CmdEvent;

typedef struct {
	guint8 *buf, *p, *end;
} Buffer;

typedef struct {
	int len;
	int id;
	int flags;
	int command_set;
	int command;
	int error;
	int error_2;
} Header;

typedef struct ReplyPacket {
	int id;
	int error;
	Buffer *data;
} ReplyPacket;


int buffer_add_command_header(Buffer* recvbuf, int cmd_set, int cmd, Buffer* out);
void decode_command_header(Buffer* recvbuf, Header* header);

/*
 * Functions to decode protocol data
 */

int decode_byte (guint8 *buf, guint8 **endbuf, guint8 *limit);
int decode_int (guint8 *buf, guint8 **endbuf, guint8 *limit);
gint64 decode_long (guint8 *buf, guint8 **endbuf, guint8 *limit);
int decode_id (guint8 *buf, guint8 **endbuf, guint8 *limit);
char* decode_string (guint8 *buf, guint8 **endbuf, guint8 *limit);
guint8* decode_byte_array(guint8* buf, guint8** endbuf, guint8* limit, guint32* len);

/*
 * Functions to encode protocol data
 */

void buffer_init (Buffer *buf, int size);
int buffer_len (Buffer *buf);
void buffer_make_room (Buffer *buf, int size);
void buffer_add_byte (Buffer *buf, guint8 val);
void buffer_add_short (Buffer *buf, guint32 val);
void buffer_add_int (Buffer *buf, guint32 val);
void buffer_add_long (Buffer *buf, guint64 l);
void buffer_add_id (Buffer *buf, int id);
void buffer_add_data (Buffer *buf, guint8 *data, int len);
void buffer_add_utf16 (Buffer *buf, guint8 *data, int len);
void buffer_add_string (Buffer *buf, const char *str);
void buffer_add_byte_array (Buffer *buf, guint8 *bytes, guint32 arr_len);
void buffer_add_buffer (Buffer *buf, Buffer *data);
void buffer_free (Buffer *buf);

const char* event_to_string (EventKind event);

#endif
