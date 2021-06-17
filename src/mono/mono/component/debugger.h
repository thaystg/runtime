// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//

#ifndef _MONO_COMPONENT_DEBUGGER_H
#define _MONO_COMPONENT_DEBUGGER_H

#include "mono/mini/debugger-agent.h"
#include "mono/mini/debugger-engine.h"
#include "mono/component/component.h"

typedef struct MonoComponentDebugger {
	MonoComponent component;
	void (*parse_options) (char *options);
	void (*init) (void);
	void (*breakpoint_hit) (void *sigctx);
	void (*single_step_event) (void *sigctx);
	void (*single_step_from_context) (MonoContext *ctx);
	void (*breakpoint_from_context) (MonoContext *ctx);
	void (*free_mem_manager) (gpointer mem_manager);
	void (*unhandled_exception) (MonoException *exc);
	void (*handle_exception) (MonoException *exc, MonoContext *throw_ctx,
							  MonoContext *catch_ctx, StackFrameInfo *catch_frame);
	void (*begin_exception_filter) (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx);
	void (*end_exception_filter) (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx);
	void (*user_break) (void);
	void (*debug_log) (int level, MonoString *category, MonoString *message);
	gboolean (*debug_log_is_enabled) (void);
	void (*send_crash) (char *json_dump, MonoStackHash *hashes, int pause);
} MonoComponentDebugger;

MONO_COMPONENT_EXPORT_ENTRYPOINT
MonoComponentDebugger *
mono_component_debugger_init (void);

#endif/*_MONO_COMPONENT_DEBUGGER_H*/
