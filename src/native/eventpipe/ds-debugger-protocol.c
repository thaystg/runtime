#include "ds-rt-config.h"

#ifdef ENABLE_PERFTRACING
#if !defined(DS_INCLUDE_SOURCE_FILES) || defined(DS_FORCE_INCLUDE_SOURCE_FILES)

#define DS_IMPL_DEBUGGER_PROTOCOL_GETTER_SETTER
#include "ds-protocol.h"
#include "ds-debugger-protocol.h"
#include "ds-server.h"
#include "ds-rt.h"

/*
 * Forward declares of all static functions.
 */
static
uint8_t *
load_inproc_debugger_command_try_parse_payload (
	uint8_t *buffer,
	uint16_t buffer_len);

static
bool
debugger_protocol_helper_load_inproc_debugger (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream);

static
bool
debugger_protocol_helper_unknown_command (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream);

/*
* DiagnosticsLoadInprocDebuggerCommandPayload
*/

static
uint8_t *
load_inproc_debugger_command_try_parse_payload (
	uint8_t *buffer,
	uint16_t buffer_len)
{
	EP_ASSERT (buffer != NULL);

	uint8_t * buffer_cursor = buffer;
	uint32_t buffer_cursor_len = buffer_len;

	DiagnosticsLoadInprocDebuggerCommandPayload *instance = ds_load_inproc_debugger_command_payload_alloc ();
	ep_raise_error_if_nok (instance != NULL);

	instance->incoming_buffer = buffer;

	if (!ds_ipc_message_try_parse_string_utf16_t (&buffer_cursor, &buffer_cursor_len, &instance->library_name))
		ep_raise_error ();

	if (!ds_ipc_message_try_parse_string_utf16_t (&buffer_cursor, &buffer_cursor_len, &instance->entry_point_name))
		ep_raise_error ();

ep_on_exit:
	return (uint8_t *)instance;

ep_on_error:
	ds_load_inproc_debugger_command_payload_free (instance);
	instance = NULL;
	ep_exit_error_handler ();
}

DiagnosticsLoadInprocDebuggerCommandPayload *
ds_load_inproc_debugger_command_payload_alloc (void)
{
	return ep_rt_object_alloc (DiagnosticsLoadInprocDebuggerCommandPayload);
}

void
ds_load_inproc_debugger_command_payload_free (DiagnosticsLoadInprocDebuggerCommandPayload *payload)
{
	ep_return_void_if_nok (payload != NULL);
	ep_rt_byte_array_free (payload->incoming_buffer);
	ep_rt_object_free (payload);
}

/*
 * DiagnosticsDebuggerProtocolHelper.
 */

static
bool
debugger_protocol_helper_unknown_command (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream)
{
	DS_LOG_WARNING_1 ("Received unknown request type (%d)", ds_ipc_header_get_commandset (ds_ipc_message_get_header_ref (message)));
	ds_ipc_message_send_error (stream, DS_IPC_E_UNKNOWN_COMMAND);
	ds_ipc_stream_free (stream);
	return true;
}

static
bool
debugger_protocol_helper_load_inproc_debugger (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream)
{
	EP_ASSERT (message != NULL);
	EP_ASSERT (stream != NULL);

	if (!stream)
		return false;

	bool result = false;
	DiagnosticsLoadInprocDebuggerCommandPayload *payload = NULL;

	payload = (DiagnosticsLoadInprocDebuggerCommandPayload *)ds_ipc_message_try_parse_payload (message, load_inproc_debugger_command_try_parse_payload);

	if (!payload) {
		ds_ipc_message_send_error (stream, DS_IPC_E_BAD_ENCODING);
		ep_raise_error ();
	}

	// Enable the debugger infrastructure (RC thread + transport) before
	// loading the library.  On mobile, the RC thread is not started during
	// Debugger::Startup() and must be brought up now so the in-proc DBI
	// can connect to the transport.
	ds_rt_debugger_enable_debugger ();

	// Send the success response BEFORE loading the library, because the
	// library load will detach the socket fd for gRPC reuse.
	ds_ipc_message_send_success (stream, DS_IPC_S_OK);

	ds_ipc_result_t ipc_result;
	ipc_result = ds_rt_debugger_load_inproc_debugger (payload, stream);
	if (ipc_result != DS_IPC_S_OK) {
		// Can't send error back — socket may already be detached
		ep_raise_error ();
	} else {
		// Auto-resume the runtime after the debugger library is loaded.
		ds_server_resume_runtime_startup ();
	}

	result = true;

ep_on_exit:
	ds_load_inproc_debugger_command_payload_free (payload);
	ds_ipc_stream_free (stream);
	return result;

ep_on_error:
	EP_ASSERT (!result);
	ep_exit_error_handler ();
}

bool
ds_debugger_protocol_helper_handle_ipc_message (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream)
{
	EP_ASSERT (message != NULL);
	EP_ASSERT (stream != NULL);

	bool result = false;

	switch ((DiagnosticsDebuggerCommandId)ds_ipc_header_get_commandid (ds_ipc_message_get_header_ref (message))) {
#if defined(TARGET_ANDROID) || defined(TARGET_IOS) || defined(TARGET_MACCATALYST)
	case DS_DEBUGGER_COMMANDID_LOAD_INPROC_DEBUGGER:
		result = debugger_protocol_helper_load_inproc_debugger (message, stream);
		break;
#endif // TARGET_ANDROID || TARGET_IOS || TARGET_MACCATALYST
	default:
		result = debugger_protocol_helper_unknown_command (message, stream);
		break;
	}

	return result;
}

#endif /* !defined(DS_INCLUDE_SOURCE_FILES) || defined(DS_FORCE_INCLUDE_SOURCE_FILES) */
#endif /* ENABLE_PERFTRACING */

#if !defined(ENABLE_PERFTRACING) || (defined(DS_INCLUDE_SOURCE_FILES) && !defined(DS_FORCE_INCLUDE_SOURCE_FILES))
extern const char quiet_linker_empty_file_warning_diagnostics_debugger_protocol;
const char quiet_linker_empty_file_warning_diagnostics_debugger_protocol = 0;
#endif
