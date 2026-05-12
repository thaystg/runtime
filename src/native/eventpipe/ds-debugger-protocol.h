#ifndef __DIAGNOSTICS_DEBUGGER_PROTOCOL_H__
#define __DIAGNOSTICS_DEBUGGER_PROTOCOL_H__

#include "ds-rt-config.h"

#ifdef ENABLE_PERFTRACING
#include "ds-types.h"
#include "ds-ipc.h"

#undef DS_IMPL_GETTER_SETTER
#ifdef DS_IMPL_DEBUGGER_PROTOCOL_GETTER_SETTER
#define DS_IMPL_GETTER_SETTER
#endif
#include "ds-getter-setter.h"

/*
* DiagnosticsLoadInprocDebuggerCommandPayload
*/

#if defined(DS_INLINE_GETTER_SETTER) || defined(DS_IMPL_DEBUGGER_PROTOCOL_GETTER_SETTER)
struct _DiagnosticsLoadInprocDebuggerCommandPayload {
#else
struct _DiagnosticsLoadInprocDebuggerCommandPayload_Internal {
#endif
	uint8_t * incoming_buffer;

	// The protocol buffer is defined as:
	//   string - debugger path
	//   string - entry point function name
	// returns
	//   ulong - status

	const ep_char16_t *library_name;
	const ep_char16_t *entry_point_name;
};

#if !defined(DS_INLINE_GETTER_SETTER) && !defined(DS_IMPL_DEBUGGER_PROTOCOL_GETTER_SETTER)
struct _DiagnosticsLoadInprocDebuggerCommandPayload {
	uint8_t _internal [sizeof (struct _DiagnosticsLoadInprocDebuggerCommandPayload_Internal)];
};
#endif

DS_DEFINE_GETTER(DiagnosticsLoadInprocDebuggerCommandPayload *, load_inproc_debugger_command_payload, const ep_char16_t *, library_name)
DS_DEFINE_GETTER(DiagnosticsLoadInprocDebuggerCommandPayload *, load_inproc_debugger_command_payload, const ep_char16_t *, entry_point_name)

DiagnosticsLoadInprocDebuggerCommandPayload *
ds_load_inproc_debugger_command_payload_alloc (void);

void
ds_load_inproc_debugger_command_payload_free (DiagnosticsLoadInprocDebuggerCommandPayload *payload);

/*
 * DiagnosticsDebuggerProtocolHelper.
 */

bool
ds_debugger_protocol_helper_handle_ipc_message (
	DiagnosticsIpcMessage *message,
	DiagnosticsIpcStream *stream);

#endif /* ENABLE_PERFTRACING */
#endif /* __DIAGNOSTICS_DEBUGGER_PROTOCOL_H__ */
