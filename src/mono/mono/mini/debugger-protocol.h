#ifndef __MONO_DEBUGGER_PROTOCOL_H__
#define __MONO_DEBUGGER_PROTOCOL_H__

#include "mini.h"
#include "debugger-engine.h"

#define HEADER_LEN 11

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
} Header;

typedef struct ReplyPacket {
	int id;
	int error;
	Buffer *data;
} ReplyPacket;


void buffer_add_command_header(Buffer* recvbuf, int cmd_set, int cmd, Buffer* out);
void decode_command_header(Buffer* recvbuf, Header* header);

/*
 * Functions to decode protocol data
 */

int decode_byte (guint8 *buf, guint8 **endbuf, guint8 *limit);
int decode_int (guint8 *buf, guint8 **endbuf, guint8 *limit);
gint64 decode_long (guint8 *buf, guint8 **endbuf, guint8 *limit);
int decode_id (guint8 *buf, guint8 **endbuf, guint8 *limit);
char* decode_string (guint8 *buf, guint8 **endbuf, guint8 *limit);

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
