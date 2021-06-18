// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//

#include <config.h>

#include "mono/mini/mini-runtime.h"
#include "mono/mini/debugger-agent.h"
#include <mono/utils/mono-proclib.h>
#include <mono/metadata/mono-debug.h>

#include <mono/metadata/components.h>
#include <mono/component/debugger.h>

typedef struct {
	gboolean enabled;
	char *transport;
	char *address;
	int log_level;
	char *log_file;
	gboolean suspend;
	gboolean server;
	gboolean onuncaught;
	GSList *onthrow;
	int timeout;
	char *launch;
	gboolean embedding;
	gboolean defer;
	int keepalive;
	gboolean setpgid;
	gboolean using_icordbg;
} AgentConfig;

static bool
debugger_avaliable (void);

static void
debugger_parse_options (char *options);

static void
debugger_init (void);

static void
debugger_breakpoint_hit (void *sigctx);

static void
debugger_single_step_event (void *sigctx);

static void
debugger_free_mem_manager (gpointer mem_manager);

static void
debugger_handle_exception (MonoException *exc, MonoContext *throw_ctx, MonoContext *catch_ctx, StackFrameInfo *catch_frame);

static void
debugger_begin_exception_filter (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx);

static void
debugger_end_exception_filter (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx);

static void
debugger_user_break (void);

static void
debugger_debug_log (int level, MonoString *category, MonoString *message);

static gboolean
debugger_debug_log_is_enabled (void);

static void
debugger_unhandled_exception (MonoException *exc);

static void
debugger_single_step_from_context (MonoContext *ctx);

static void
debugger_breakpoint_from_context (MonoContext *ctx);

static void
debugger_send_crash (char *json_dump, MonoStackHash *hashes, int pause);


/*debugger-engine*/
static DebuggerEngineCallbacksTHAYS rt_callbacks;

void
mono_de_init (DebuggerEngineCallbacksTHAYS *cbs);

/*end-debugger-engine*/

static MonoComponentDebugger fn_table = {
	{ MONO_COMPONENT_ITF_VERSION, &debugger_avaliable },
	&debugger_parse_options,
	&debugger_init,
	&debugger_breakpoint_hit,
	&debugger_single_step_event,
	&debugger_single_step_from_context,
	&debugger_breakpoint_from_context,
	&debugger_free_mem_manager,
	&debugger_unhandled_exception,
	&debugger_handle_exception,
	&debugger_begin_exception_filter,
	&debugger_end_exception_filter,
	&debugger_user_break,
	&debugger_debug_log,
	&debugger_debug_log_is_enabled,
	&debugger_send_crash,
	&mono_de_init,
};

/*debugger-agent*/
/*
 * Globals
 */
static AgentConfig agent_config;


/* 
 * Whenever the agent is fully initialized.
 * When using the onuncaught or onthrow options, only some parts of the agent are
 * initialized on startup, and the full initialization which includes connection
 * establishment and the startup of the agent thread is only done in response to
 * an event.
 */
static gint32 agent_inited;

#ifndef DISABLE_SOCKET_TRANSPORT
static int conn_fd;
static int listen_fd;
#endif

static int objref_id = 0;

static int event_request_id = 0;

static int frame_id = 0;

static GPtrArray *event_requests;

static MonoNativeTlsKey debugger_tls_id;

static gboolean vm_start_event_sent, vm_death_event_sent, disconnected;

/* Maps MonoInternalThread -> DebuggerTlsDataTHAYS */
/* Protected by the loader lock */
static MonoGHashTable *thread_to_tls;

/* Maps tid -> MonoInternalThread */
/* Protected by the loader lock */
static MonoGHashTable *tid_to_thread;

/* Maps tid -> MonoThread (not MonoInternalThread) */
/* Protected by the loader lock */
static MonoGHashTable *tid_to_thread_obj;

static MonoNativeThreadId debugger_thread_id;

static MonoThreadHandle *debugger_thread_handle;

static int log_level;

static int file_check_valid_memory = -1;

static char* filename_check_valid_memory;

static gboolean embedding;

static FILE *log_file;

/* Assemblies whose assembly load event has no been sent yet */
/* Protected by the dbg lock */
static GPtrArray *pending_assembly_loads;

/* Whenever the debugger thread has exited */
static gboolean debugger_thread_exited;

/* Cond variable used to wait for debugger_thread_exited becoming true */
static MonoCoopCond debugger_thread_exited_cond;

/* Mutex for the cond var above */
static MonoCoopMutex debugger_thread_exited_mutex;

/* The protocol version of the client */
static int major_version, minor_version;

/* If the debugger is using icordbg interface */
static gboolean using_icordbg;

/* Whenever the variables above are set by the client */
static gboolean protocol_version_set;

/* The number of times the runtime is suspended */
static gint32 suspend_count;

/* Whenever to buffer reply messages and send them together */
static gboolean buffer_replies;

/* Callbacks used by debugger-engine */
static MonoContext* tls_get_restore_state (void *the_tls);
static gboolean try_process_suspend (void *tls, MonoContext *ctx, gboolean from_breakpoint);
static gboolean begin_breakpoint_processing (void *tls, MonoContext *ctx, MonoJitInfo *ji, gboolean from_signal);
static void begin_single_step_processing (MonoContext *ctx, gboolean from_signal);
static void ss_discard_frame_context (void *the_tls);
static void ss_calculate_framecount (void *tls, MonoContext *ctx, gboolean force_use_ctx, DbgEngineStackFrameTHAYS ***frames, int *nframes);
static gboolean ensure_jit (DbgEngineStackFrameTHAYS* the_frame);
static int ensure_runtime_is_suspended (void);
static int get_this_async_id (DbgEngineStackFrameTHAYS *frame);
static void* create_breakpoint_events (GPtrArray *ss_reqs, GPtrArray *bp_reqs, MonoJitInfo *ji, MdbgProtEventKind kind);
static void process_breakpoint_events (void *_evts, MonoMethod *method, MonoContext *ctx, int il_offset);
static int ss_create_init_args (SingleStepReqTHAYS *ss_req, SingleStepArgsTHAYS *args);
static void ss_args_destroy (SingleStepArgsTHAYS *ss_args);
static int handle_multiple_ss_requests (void);


/*
 * Logging support
 */
static int log_level;
static FILE *log_file;


/*
 * Locking
 */
#define dbg_lock() mono_coop_mutex_lock (&debug_mutex)
#define dbg_unlock() mono_coop_mutex_unlock (&debug_mutex)
static MonoCoopMutex debug_mutex;


static bool
debugger_avaliable (void)
{
	return true;
}

static gboolean
parse_flag (const char *option, char *flag)
{
	if (!strcmp (flag, "y"))
		return TRUE;
	else if (!strcmp (flag, "n"))
		return FALSE;
	else {
		PRINT_ERROR_MSG ("debugger-agent: The valid values for the '%s' option are 'y' and 'n'.\n", option);
		exit (1);
		return FALSE;
	}
}

static int
parse_address (char *address, char **host, int *port)
{
	char *pos = strchr (address, ':');

	if (pos == NULL || pos == address)
		return 1;

	size_t len = pos - address;
	*host = (char *)g_malloc (len + 1);
	memcpy (*host, address, len);
	(*host) [len] = '\0';

	if (!strcmp(pos + 1, "pid_based"))
		*port = -1;
	else
		*port = atoi (pos + 1);

	return 0;
}

static void
print_usage (void)
{
	PRINT_ERROR_MSG ("Usage: mono --debugger-agent=[<option>=<value>,...] ...\n");
	PRINT_ERROR_MSG ("Available options:\n");
	PRINT_ERROR_MSG ("  transport=<transport>\t\tTransport to use for connecting to the debugger (mandatory, possible values: 'dt_socket')\n");
	PRINT_ERROR_MSG ("  address=<hostname>:<port>\tAddress to connect to (mandatory)\n");
	PRINT_ERROR_MSG ("  loglevel=<n>\t\t\tLog level (defaults to 0)\n");
	PRINT_ERROR_MSG ("  logfile=<file>\t\tFile to log to (defaults to stdout)\n");
	PRINT_ERROR_MSG ("  suspend=y/n\t\t\tWhether to suspend after startup.\n");
	PRINT_ERROR_MSG ("  timeout=<n>\t\t\tTimeout for connecting in milliseconds.\n");
	PRINT_ERROR_MSG ("  server=y/n\t\t\tWhether to listen for a client connection.\n");
	PRINT_ERROR_MSG ("  keepalive=<n>\t\t\tSend keepalive events every n milliseconds.\n");
	PRINT_ERROR_MSG ("  setpgid=y/n\t\t\tWhether to call setpid(0, 0) after startup.\n");
	PRINT_ERROR_MSG ("  help\t\t\t\tPrint this help.\n");
}

static void
debugger_parse_options (char *options)
{
	char **args, **ptr;
	char *host;
	int port;
	char *extra;

#ifndef MONO_ARCH_SOFT_DEBUG_SUPPORTED
	PRINT_ERROR_MSG ("--debugger-agent is not supported on this platform.\n");
	exit (1);
#endif

	extra = g_getenv ("MONO_SDB_ENV_OPTIONS");
	if (extra) {
		options = g_strdup_printf ("%s,%s", options, extra);
		g_free (extra);
	}

	agent_config.enabled = TRUE;
	agent_config.suspend = TRUE;
	agent_config.server = FALSE;
	agent_config.defer = FALSE;
	agent_config.address = NULL;

	//agent_config.log_level = 10;

	args = g_strsplit (options, ",", -1);
	for (ptr = args; ptr && *ptr; ptr ++) {
		char *arg = *ptr;

		if (strncmp (arg, "transport=", 10) == 0) {
			agent_config.transport = g_strdup (arg + 10);
		} else if (strncmp (arg, "address=", 8) == 0) {
			agent_config.address = g_strdup (arg + 8);
		} else if (strncmp (arg, "loglevel=", 9) == 0) {
			agent_config.log_level = atoi (arg + 9);
		} else if (strncmp (arg, "logfile=", 8) == 0) {
			agent_config.log_file = g_strdup (arg + 8);
		} else if (strncmp (arg, "suspend=", 8) == 0) {
			agent_config.suspend = parse_flag ("suspend", arg + 8);
		} else if (strncmp (arg, "server=", 7) == 0) {
			agent_config.server = parse_flag ("server", arg + 7);
		} else if (strncmp (arg, "onuncaught=", 11) == 0) {
			agent_config.onuncaught = parse_flag ("onuncaught", arg + 11);
		} else if (strncmp (arg, "onthrow=", 8) == 0) {
			/* We support multiple onthrow= options */
			agent_config.onthrow = g_slist_append (agent_config.onthrow, g_strdup (arg + 8));
		} else if (strncmp (arg, "onthrow", 7) == 0) {
			agent_config.onthrow = g_slist_append (agent_config.onthrow, g_strdup (""));
		} else if (strncmp (arg, "help", 4) == 0) {
			print_usage ();
			exit (0);
		} else if (strncmp (arg, "timeout=", 8) == 0) {
			agent_config.timeout = atoi (arg + 8);
		} else if (strncmp (arg, "launch=", 7) == 0) {
			agent_config.launch = g_strdup (arg + 7);
		} else if (strncmp (arg, "embedding=", 10) == 0) {
			agent_config.embedding = atoi (arg + 10) == 1;
		} else if (strncmp (arg, "keepalive=", 10) == 0) {
			agent_config.keepalive = atoi (arg + 10);
		} else if (strncmp (arg, "setpgid=", 8) == 0) {
			agent_config.setpgid = parse_flag ("setpgid", arg + 8);
		} else {
			print_usage ();
			exit (1);
		}
	}

	if (agent_config.server && !agent_config.suspend) {
		/* Waiting for deferred attachment */
		agent_config.defer = TRUE;
		if (agent_config.address == NULL) {
			agent_config.address = g_strdup_printf ("0.0.0.0:%u", 56000 + (mono_process_current_pid () % 1000));
		}
	}

	//agent_config.log_level = 0;

	if (agent_config.transport == NULL) {
		PRINT_ERROR_MSG ("debugger-agent: The 'transport' option is mandatory.\n");
		exit (1);
	}

	if (agent_config.address == NULL && !agent_config.server) {
		PRINT_ERROR_MSG ("debugger-agent: The 'address' option is mandatory.\n");
		exit (1);
	}

	// FIXME:
	if (!strcmp (agent_config.transport, "dt_socket")) {
		if (agent_config.address && parse_address (agent_config.address, &host, &port)) {
			PRINT_ERROR_MSG ("debugger-agent: The format of the 'address' options is '<host>:<port>'\n");
			exit (1);
		}
	}
}

static void
runtime_initialized (MonoProfiler *prof)
{
	process_profiler_event (MDBGPROT_EVENT_KIND_VM_START, mono_thread_current ());
	if (CHECK_PROTOCOL_VERSION (2, 59))
		process_profiler_event (MDBGPROT_EVENT_KIND_ASSEMBLY_LOAD, (mono_defaults.corlib->assembly));
	if (agent_config.defer) {
		ERROR_DECL (error);
		start_debugger_thread (error);
		mono_error_assert_ok (error);
	}
}


static void
appdomain_load (MonoProfiler *prof, MonoDomain *domain)
{
	mono_de_domain_add (domain);

	process_profiler_event (MDBGPROT_EVENT_KIND_APPDOMAIN_CREATE, domain);
}

static void
appdomain_start_unload (MonoProfiler *prof, MonoDomain *domain)
{
	DebuggerTlsDataTHAYS *tls;

	/* This might be called during shutdown on the debugger thread from the CMD_VM_EXIT code */
	if (is_debugger_thread ())
		return;

	/*
	 * Remember the currently unloading appdomain as it is needed to generate
	 * proper ids for unloading assemblies.
	 */
	tls = (DebuggerTlsDataTHAYS *)mono_native_tls_get_value (debugger_tls_id);
	g_assert (tls);
	tls->domain_unloading = domain;
}

/*
 * invalidate_each_thread:
 *
 *   A GHFunc to invalidate frames.
 *   value must be a DebuggerTlsDataTHAYS*
 */
static void
invalidate_each_thread (gpointer key, gpointer value, gpointer user_data)
{
	invalidate_frames ((DebuggerTlsDataTHAYS *)value);
}

static void
appdomain_unload (MonoProfiler *prof, MonoDomain *domain)
{
	DebuggerTlsDataTHAYS *tls;

	if (is_debugger_thread ())
		return;

	tls = (DebuggerTlsDataTHAYS *)mono_native_tls_get_value (debugger_tls_id);
	g_assert (tls);
	tls->domain_unloading = NULL;

	mono_de_clear_breakpoints_for_domain (domain);
	
	mono_loader_lock ();
	/* Invalidate each thread's frame stack */
	mono_g_hash_table_foreach (thread_to_tls, invalidate_each_thread, NULL);
	mono_loader_unlock ();
	
	process_profiler_event (MDBGPROT_EVENT_KIND_APPDOMAIN_UNLOAD, domain);
}


static void
thread_startup (MonoProfiler *prof, uintptr_t tid)
{
	MonoInternalThread *thread = mono_thread_internal_current ();
	MonoInternalThread *old_thread;
	DebuggerTlsDataTHAYS *tls;

	if (is_debugger_thread ())
		return;

	g_assert (mono_native_thread_id_equals (MONO_UINT_TO_NATIVE_THREAD_ID (tid), MONO_UINT_TO_NATIVE_THREAD_ID (thread->tid)));

	mono_loader_lock ();
	old_thread = (MonoInternalThread *)mono_g_hash_table_lookup (tid_to_thread, GUINT_TO_POINTER (tid));
	mono_loader_unlock ();
	if (old_thread) {
		if (thread == old_thread) {
			/* 
			 * For some reason, thread_startup () might be called for the same thread
			 * multiple times (attach ?).
			 */
			PRINT_DEBUG_MSG (1, "[%p] thread_start () called multiple times for %p, ignored.\n", GUINT_TO_POINTER (tid), GUINT_TO_POINTER (tid));
			return;
		} else {
			/*
			 * thread_end () might not be called for some threads, and the tid could
			 * get reused.
			 */
			PRINT_DEBUG_MSG (1, "[%p] Removing stale data for tid %p.\n", GUINT_TO_POINTER (tid), GUINT_TO_POINTER (tid));
			mono_loader_lock ();
			mono_g_hash_table_remove (thread_to_tls, old_thread);
			mono_g_hash_table_remove (tid_to_thread, GUINT_TO_POINTER (tid));
			mono_g_hash_table_remove (tid_to_thread_obj, GUINT_TO_POINTER (tid));
			mono_loader_unlock ();
		}
	}

	tls = (DebuggerTlsDataTHAYS *)mono_native_tls_get_value (debugger_tls_id);
	g_assert (!tls);
	// FIXME: Free this somewhere
	tls = g_new0 (DebuggerTlsDataTHAYS, 1);
	MONO_GC_REGISTER_ROOT_SINGLE (tls->thread, MONO_ROOT_SOURCE_DEBUGGER, NULL, "Debugger Thread Reference");
	tls->thread = thread;
	// Do so we have thread id even after termination
	tls->thread_id = (intptr_t) thread->tid;
	mono_native_tls_set_value (debugger_tls_id, tls);

	PRINT_DEBUG_MSG (1, "[%p] Thread started, obj=%p, tls=%p.\n", (gpointer)tid, thread, tls);

	mono_loader_lock ();
	mono_g_hash_table_insert_internal (thread_to_tls, thread, tls);
	mono_g_hash_table_insert_internal (tid_to_thread, (gpointer)tid, thread);
	mono_g_hash_table_insert_internal (tid_to_thread_obj, GUINT_TO_POINTER (tid), mono_thread_current ());
	mono_loader_unlock ();

	process_profiler_event (MDBGPROT_EVENT_KIND_THREAD_START, thread);

	/* 
	 * suspend_vm () could have missed this thread, so wait for a resume.
	 */
	
	suspend_current ();
}

static void
thread_end (MonoProfiler *prof, uintptr_t tid)
{
	MonoInternalThread *thread;
	DebuggerTlsDataTHAYS *tls = NULL;

	mono_loader_lock ();
	thread = (MonoInternalThread *)mono_g_hash_table_lookup (tid_to_thread, GUINT_TO_POINTER (tid));
	if (thread) {
		mono_g_hash_table_remove (tid_to_thread_obj, GUINT_TO_POINTER (tid));
		tls = (DebuggerTlsDataTHAYS *)mono_g_hash_table_lookup (thread_to_tls, thread);
		if (tls) {
			/* FIXME: Maybe we need to free this instead, but some code can't handle that */
			tls->terminated = TRUE;
			/* Can't remove from tid_to_thread, as that would defeat the check in thread_start () */
			MONO_GC_UNREGISTER_ROOT (tls->thread);
			tls->thread = NULL;
		}
	}
	mono_loader_unlock ();

	/* We might be called for threads started before we registered the start callback */
	if (thread) {
		PRINT_DEBUG_MSG (1, "[%p] Thread terminated, obj=%p, tls=%p (domain=%p).\n", (gpointer)tid, thread, tls, (gpointer)mono_domain_get ());

		if (mono_thread_internal_is_current (thread) &&
		    (!mono_native_tls_get_value (debugger_tls_id) ||
		     !mono_domain_get ())
		) {
			/*
			 * This can happen on darwin and android since we
			 * deregister threads using pthread dtors.
			 * process_profiler_event () and the code it calls
			 * cannot handle a null TLS value.
			 */
			return;
		}

		process_profiler_event (MDBGPROT_EVENT_KIND_THREAD_DEATH, thread);
	}
}

static void
assembly_load (MonoProfiler *prof, MonoAssembly *assembly)
{
	/* Sent later in jit_end () */
	dbg_lock ();
	g_ptr_array_add (pending_assembly_loads, assembly);
	dbg_unlock ();
}

static void
assembly_unload (MonoProfiler *prof, MonoAssembly *assembly)
{
	if (is_debugger_thread ())
		return;

	process_profiler_event (MDBGPROT_EVENT_KIND_ASSEMBLY_UNLOAD, assembly);

	clear_event_requests_for_assembly (assembly);
	clear_types_for_assembly (assembly);
}


static void gc_finalizing (MonoProfiler *prof)
{
	DebuggerTlsDataTHAYS *tls;

	if (is_debugger_thread ())
		return;

	tls = (DebuggerTlsDataTHAYS *)mono_native_tls_get_value (debugger_tls_id);
	g_assert (tls);
	tls->gc_finalizing = TRUE;
}

static void gc_finalized (MonoProfiler *prof)
{
	DebuggerTlsDataTHAYS *tls;

	if (is_debugger_thread ())
		return;

	tls = (DebuggerTlsDataTHAYS *)mono_native_tls_get_value (debugger_tls_id);
	g_assert (tls);
	tls->gc_finalizing = FALSE;
}

static void
jit_done (MonoProfiler *prof, MonoMethod *method, MonoJitInfo *jinfo)
{
	jit_end (prof, method, jinfo);
}

static void
jit_failed (MonoProfiler *prof, MonoMethod *method)
{
	jit_end (prof, method, NULL);
}

static MonoContext*
tls_get_restore_state (void *the_tls)
{
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS*)the_tls;

	return &tls->restore_state.ctx;
}

/* Conditionally call process_suspend depending oh the current state */
static gboolean
try_process_suspend (void *the_tls, MonoContext *ctx, gboolean from_breakpoint)
{
	MONO_REQ_GC_UNSAFE_MODE;
	
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS*)the_tls;
	/* if there is a suspend pending that is not executed yes */
	if (suspend_count > 0) { 
		/* Fastpath during invokes, see in process_suspend () */
		/* if there is a suspend pending but this thread is already resumed, we shouldn't suspend it again and the breakpoint/ss can run */
		if (suspend_count - tls->resume_count == 0) 
			return FALSE;
		/* if there is in a invoke the breakpoint/step should be executed even with the suspend pending */
		if (tls->invoke) 
			return FALSE;
		/* with the multithreaded single step check if there is a suspend_count pending in the current thread and not in the vm */
		if (from_breakpoint && tls->suspend_count <= tls->resume_count_internal)
			return FALSE;
		process_suspend (tls, ctx);
		return TRUE;
	} /* if there isn't any suspend pending, the breakpoint/ss will be executed and will suspend then vm when the event is sent */
	return FALSE;
}


static gboolean
begin_breakpoint_processing (void *the_tls, MonoContext *ctx, MonoJitInfo *ji, gboolean from_signal)
{
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS*)the_tls;

	/*
	 * Skip the instruction causing the breakpoint signal.
	 */
	if (from_signal)
		mono_arch_skip_breakpoint (ctx, ji);

	if (tls->disable_breakpoints)
		return FALSE;
	return TRUE;
}


static void
begin_single_step_processing (MonoContext *ctx, gboolean from_signal)
{
	if (from_signal)
		mono_arch_skip_single_step (ctx);
}

/*
 * ss_discard_frame_data:
 *
 * Discard frame data and invalidate any context
 */
static void
ss_discard_frame_context (void *the_tls)
{
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS*)the_tls;
	tls->context.valid = FALSE;
	tls->async_state.valid = FALSE;
	invalidate_frames (tls);
}


/*
 * ss_calculate_framecount:
 *
 * Ensure DebuggerTlsDataTHAYS fields are filled out.
 */
static void
ss_calculate_framecount (void *the_tls, MonoContext *ctx, gboolean force_use_ctx, DbgEngineStackFrameTHAYS ***frames, int *nframes)
{
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS*)the_tls;

	if (force_use_ctx || !tls->context.valid)
		mono_thread_state_init_from_monoctx (&tls->context, ctx);
	compute_frame_info (tls->thread, tls, FALSE);
	if (frames)
		*frames = (DbgEngineStackFrameTHAYS**)tls->frames;
	if (nframes)
		*nframes = tls->frame_count;
}


static gboolean
ensure_jit (DbgEngineStackFrameTHAYS* the_frame)
{
	StackFrame *frame = (StackFrame*)the_frame;
	if (!frame->jit) {
		frame->jit = mono_debug_find_method (frame->api_method, frame->de.domain);
		if (!frame->jit && frame->api_method->is_inflated)
			frame->jit = mono_debug_find_method(mono_method_get_declaring_generic_method (frame->api_method), frame->de.domain);
		if (!frame->jit) {
			char *s;

			/* This could happen for aot images with no jit debug info */
			s = mono_method_full_name (frame->api_method, TRUE);
			PRINT_DEBUG_MSG(1, "[dbg] No debug information found for '%s'.\n", s);
			g_free (s);
			return FALSE;
		}
	}
	return TRUE;
}

gboolean
set_set_notification_for_wait_completion_flag (DbgEngineStackFrameTHAYS *frame)
{
	MonoClassField *builder_field = mono_class_get_field_from_name_full (get_class_to_get_builder_field(frame), "<>t__builder", NULL);
	if (!builder_field)
		return FALSE;
	gpointer builder = get_async_method_builder (frame);
	if (!builder)
		return FALSE;

	MonoMethod* method = get_set_notification_method (mono_class_from_mono_type_internal (builder_field->type));
	if (method == NULL)
		return FALSE;
	gboolean arg = TRUE;
	ERROR_DECL (error);
	void *args [ ] = { &arg };
	mono_runtime_invoke_checked (method, builder, args, error);
	mono_error_assert_ok (error);
	return TRUE;
}

static MonoMethod* notify_debugger_of_wait_completion_method_cache;

MonoMethod*
get_notify_debugger_of_wait_completion_method (void)
{
	if (notify_debugger_of_wait_completion_method_cache != NULL)
		return notify_debugger_of_wait_completion_method_cache;
	ERROR_DECL (error);
	MonoClass* task_class = mono_class_load_from_name (mono_defaults.corlib, "System.Threading.Tasks", "Task");
	GPtrArray* array = mono_class_get_methods_by_name (task_class, "NotifyDebuggerOfWaitCompletion", 0x24, 1, FALSE, error);
	mono_error_assert_ok (error);
	g_assert (array->len == 1);
	notify_debugger_of_wait_completion_method_cache = (MonoMethod *)g_ptr_array_index (array, 0);
	g_ptr_array_free (array, TRUE);
	return notify_debugger_of_wait_completion_method_cache;
}

typedef struct {
	GSList *bp_events, *ss_events, *enter_leave_events;
	MdbgProtMdbgProtEventKind kind;
	int suspend_policy;
} BreakPointEvents;

static void*
create_breakpoint_events (GPtrArray *ss_reqs, GPtrArray *bp_reqs, MonoJitInfo *ji, MdbgProtMdbgProtEventKind kind)
{
	int suspend_policy = 0;
	BreakPointEvents *evts = g_new0 (BreakPointEvents, 1);
	if (ss_reqs && ss_reqs->len > 0)
		evts->ss_events = create_event_list (MDBGPROT_EVENT_KIND_STEP, ss_reqs, ji, NULL, &suspend_policy);
	else if (bp_reqs && bp_reqs->len > 0)
		evts->bp_events = create_event_list (MDBGPROT_EVENT_KIND_BREAKPOINT, bp_reqs, ji, NULL, &suspend_policy);
	else if (kind != MDBGPROT_EVENT_KIND_BREAKPOINT)
		evts->enter_leave_events = create_event_list (kind, NULL, ji, NULL, &suspend_policy);

	evts->kind = kind;
	evts->suspend_policy = suspend_policy;
	return evts;
}

static void
process_breakpoint_events (void *_evts, MonoMethod *method, MonoContext *ctx, int il_offset)
{
	BreakPointEvents *evts = (BreakPointEvents*)_evts;
	/*
	 * FIXME: The first event will suspend, so the second will only be sent after the
	 * resume.
	 */
	if (evts->ss_events)
		process_event (MDBGPROT_EVENT_KIND_STEP, method, il_offset, ctx, evts->ss_events, evts->suspend_policy);
	if (evts->bp_events)
		process_event (evts->kind, method, il_offset, ctx, evts->bp_events, evts->suspend_policy);
	if (evts->enter_leave_events)
		process_event (evts->kind, method, il_offset, ctx, evts->enter_leave_events, evts->suspend_policy);

	g_free (evts);
}


static int
ss_create_init_args (SingleStepReqTHAYS *ss_req, SingleStepArgsTHAYS *args)
{
	MonoSeqPointInfo *info = NULL;
	gboolean found_sp;
	MonoMethod *method = NULL;
	MonoDebugMethodInfo *minfo;
	gboolean step_to_catch = FALSE;
	gboolean set_ip = FALSE;
	StackFrame **frames = NULL;
	int nframes = 0;

	mono_loader_lock ();
	DebuggerTlsDataTHAYS *tls = (DebuggerTlsDataTHAYS *)mono_g_hash_table_lookup (thread_to_tls, ss_req->thread);
	mono_loader_unlock ();
	g_assert (tls);
	if (!tls->context.valid) {
		PRINT_DEBUG_MSG (1, "Received a single step request on a thread with no managed frames.\n");
		return ERR_INVALID_ARGUMENT;
	}

	if (tls->restore_state.valid && MONO_CONTEXT_GET_IP (&tls->context.ctx) != MONO_CONTEXT_GET_IP (&tls->restore_state.ctx)) {
		/*
		 * Need to start single stepping from restore_state and not from the current state
		 */
		set_ip = TRUE;
		frames = compute_frame_info_from (ss_req->thread, tls, &tls->restore_state, &nframes);
	}

	ss_req->start_sp = ss_req->last_sp = MONO_CONTEXT_GET_SP (&tls->context.ctx);

	if (tls->has_catch_frame) {
		StackFrameInfo frame;

		/*
		 * We are stopped at a throw site. Stepping should go to the catch site.
		 */
		frame = tls->catch_frame;
		if (frame.type != FRAME_TYPE_MANAGED && frame.type != FRAME_TYPE_INTERP) {
			PRINT_DEBUG_MSG (1, "Current frame is not managed nor interpreter.\n");
			return ERR_INVALID_ARGUMENT;
		}

		/*
		 * Find the seq point corresponding to the landing site ip, which is the first seq
		 * point after ip.
		 */
		found_sp = mono_find_next_seq_point_for_native_offset (frame.method, frame.native_offset, &info, &args->sp);
		if (!found_sp)
			no_seq_points_found (frame.method, frame.native_offset);
		if (!found_sp) {
			PRINT_DEBUG_MSG (1, "Could not find next sequence point.\n");
			return ERR_INVALID_ARGUMENT;
		}

		method = frame.method;

		step_to_catch = TRUE;
		/* This make sure the seq point is not skipped by process_single_step () */
		ss_req->last_sp = NULL;
	}

	if (!step_to_catch) {
		StackFrame *frame = NULL;

		if (set_ip) {
			if (frames && nframes)
				frame = frames [0];
		} else {
			compute_frame_info (ss_req->thread, tls, FALSE);

			if (tls->frame_count)
				frame = tls->frames [0];
		}

		if (ss_req->size == STEP_SIZE_LINE) {
			if (frame) {
				ss_req->last_method = frame->de.method;
				ss_req->last_line = -1;

				minfo = mono_debug_lookup_method (frame->de.method);
				if (minfo && frame->il_offset != -1) {
					MonoDebugSourceLocation *loc = mono_debug_method_lookup_location (minfo, frame->il_offset);

					if (loc) {
						ss_req->last_line = loc->row;
						g_free (loc);
					}
				}
			}
		}

		if (frame) {
			if (!method && frame->il_offset != -1) {
				/* FIXME: Sort the table and use a binary search */
				found_sp = mono_find_prev_seq_point_for_native_offset (frame->de.method, frame->de.native_offset, &info, &args->sp);
				if (!found_sp)
					no_seq_points_found (frame->de.method, frame->de.native_offset);
				if (!found_sp) {
					PRINT_DEBUG_MSG (1, "Could not find next sequence point.\n");
					return ERR_INVALID_ARGUMENT;
				}
				method = frame->de.method;
			}
		}
	}

	ss_req->start_method = method;

	args->method = method;
	args->ctx = set_ip ? &tls->restore_state.ctx : &tls->context.ctx;
	args->tls = tls;
	args->step_to_catch = step_to_catch;
	args->info = info;
	args->frames = (DbgEngineStackFrameTHAYS**)frames;
	args->nframes = nframes;

	return ERR_NONE;
}

static void
ss_args_destroy (SingleStepArgsTHAYS *ss_args)
{
	if (ss_args->frames)
		free_frames ((StackFrame**)ss_args->frames, ss_args->nframes);
}

static int
handle_multiple_ss_requests (void)
{
	if (!CHECK_PROTOCOL_VERSION (2, 57))
		return DE_ERR_NOT_IMPLEMENTED;
	return 1;
}

static void
debugger_init (void)
{
	if (!agent_config.enabled)
		return;

	DebuggerEngineCallbacksTHAYS cbs;
	memset (&cbs, 0, sizeof (cbs));
	cbs.tls_get_restore_state = tls_get_restore_state;
	cbs.try_process_suspend = try_process_suspend;
	cbs.begin_breakpoint_processing = begin_breakpoint_processing;
	cbs.begin_single_step_processing = begin_single_step_processing;
	cbs.ss_discard_frame_context = ss_discard_frame_context;
	cbs.ss_calculate_framecount = ss_calculate_framecount;
	cbs.ensure_jit = ensure_jit;
	cbs.ensure_runtime_is_suspended = ensure_runtime_is_suspended;
	cbs.get_this_async_id = get_this_async_id;
	cbs.set_set_notification_for_wait_completion_flag = set_set_notification_for_wait_completion_flag;
	cbs.get_notify_debugger_of_wait_completion_method = get_notify_debugger_of_wait_completion_method;
	cbs.create_breakpoint_events = create_breakpoint_events;
	cbs.process_breakpoint_events = process_breakpoint_events;
	cbs.ss_create_init_args = ss_create_init_args;
	cbs.ss_args_destroy = ss_args_destroy;
	cbs.handle_multiple_ss_requests = handle_multiple_ss_requests;

	mono_component_debugger ()->mono_de_init (&cbs);

	transport_init ();

	/* Need to know whenever a thread has acquired the loader mutex */
	mono_loader_lock_track_ownership (TRUE);

	event_requests = g_ptr_array_new ();

	mono_coop_mutex_init (&debugger_thread_exited_mutex);
	mono_coop_cond_init (&debugger_thread_exited_cond);

	MonoProfilerHandle prof = mono_profiler_create (NULL);
	mono_profiler_set_runtime_initialized_callback (prof, runtime_initialized);
	mono_profiler_set_domain_loaded_callback (prof, appdomain_load);
	mono_profiler_set_domain_unloading_callback (prof, appdomain_start_unload);
	mono_profiler_set_domain_unloaded_callback (prof, appdomain_unload);
	mono_profiler_set_thread_started_callback (prof, thread_startup);
	mono_profiler_set_thread_stopped_callback (prof, thread_end);
	mono_profiler_set_assembly_loaded_callback (prof, assembly_load);
	mono_profiler_set_assembly_unloading_callback (prof, assembly_unload);
	mono_profiler_set_jit_done_callback (prof, jit_done);
	mono_profiler_set_jit_failed_callback (prof, jit_failed);
	mono_profiler_set_gc_finalizing_callback (prof, gc_finalizing);
	mono_profiler_set_gc_finalized_callback (prof, gc_finalized);
	
	mono_native_tls_alloc (&debugger_tls_id, NULL);

	/* Needed by the hash_table_new_type () call below */
	mono_gc_base_init ();

	thread_to_tls = mono_g_hash_table_new_type_internal ((GHashFunc)mono_object_hash_internal, NULL, MONO_HASH_KEY_GC, MONO_ROOT_SOURCE_DEBUGGER, NULL, "Debugger TLS Table");

	tid_to_thread = mono_g_hash_table_new_type_internal (NULL, NULL, MONO_HASH_VALUE_GC, MONO_ROOT_SOURCE_DEBUGGER, NULL, "Debugger Thread Table");

	tid_to_thread_obj = mono_g_hash_table_new_type_internal (NULL, NULL, MONO_HASH_VALUE_GC, MONO_ROOT_SOURCE_DEBUGGER, NULL, "Debugger Thread Object Table");

	pending_assembly_loads = g_ptr_array_new ();

	log_level = agent_config.log_level;

	embedding = agent_config.embedding;
	disconnected = TRUE;

	if (agent_config.log_file) {
		log_file = fopen (agent_config.log_file, "w+");
		if (!log_file) {
			PRINT_ERROR_MSG ("Unable to create log file '%s': %s.\n", agent_config.log_file, strerror (errno));
			exit (1);
		}
	} else {
		log_file = stdout;
	}
	mono_de_set_log_level (log_level, log_file);

	ids_init ();
	objrefs_init ();
	suspend_init ();

	mini_get_debug_options ()->gen_sdb_seq_points = TRUE;
	/* 
	 * This is needed because currently we don't handle liveness info.
	 */
	mini_get_debug_options ()->mdb_optimizations = TRUE;

#ifndef MONO_ARCH_HAVE_CONTEXT_SET_INT_REG
	/* This is needed because we can't set local variables in registers yet */
	mono_disable_optimizations (MONO_OPT_LINEARS);
#endif

	/*
	 * The stack walk done from thread_interrupt () needs to be signal safe, but it
	 * isn't, since it can call into mono_aot_find_jit_info () which is not signal
	 * safe (#3411). So load AOT info eagerly when the debugger is running as a
	 * workaround.
	 */
	mini_get_debug_options ()->load_aot_jit_info_eagerly = TRUE;

#ifdef HAVE_SETPGID
	if (agent_config.setpgid)
		setpgid (0, 0);
#endif

	if (!agent_config.onuncaught && !agent_config.onthrow)
		finish_agent_init (TRUE);
}

static void
debugger_breakpoint_hit (void *sigctx)
{
}

static void
debugger_single_step_event (void *sigctx)
{
}

static void
debugger_free_mem_manager (gpointer mem_manager)
{
}

static void
debugger_handle_exception (MonoException *exc, MonoContext *throw_ctx,
									  MonoContext *catch_ctx, StackFrameInfo *catch_frame)
{
}

static void
debugger_begin_exception_filter (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx)
{
}

static void
debugger_end_exception_filter (MonoException *exc, MonoContext *ctx, MonoContext *orig_ctx)
{
}

static void
debugger_user_break (void)
{
	G_BREAKPOINT ();
}

static void
debugger_debug_log (int level, MonoString *category, MonoString *message)
{
}

static gboolean
debugger_debug_log_is_enabled (void)
{
	return FALSE;
}

static void
debugger_unhandled_exception (MonoException *exc)
{
	g_assert_not_reached ();
}

static void
debugger_single_step_from_context (MonoContext *ctx)
{
	g_assert_not_reached ();
}

static void
debugger_breakpoint_from_context (MonoContext *ctx)
{
	g_assert_not_reached ();
}

static void
debugger_send_crash (char *json_dump, MonoStackHash *hashes, int pause)
{
}

MonoComponentDebugger *
mono_component_debugger_init (void)
{
	return &fn_table;
}

/*DEBUGGER-ENGINE*/
void
mono_de_init (DebuggerEngineCallbacksTHAYS *cbs)
{
	rt_callbacks = *cbs;
	mono_coop_mutex_init_recursive (&debug_mutex);

	domains_init ();
	breakpoints_init ();
	ss_req_init ();
	mono_debugger_log_init ();
}


/*
 * Domain support
 */


/* A hash table containing all active domains */
/* Protected by the loader lock */
static GHashTable *domains;


static void
domains_init (void)
{
	domains = g_hash_table_new (mono_aligned_addr_hash, NULL);
}

static void
domains_cleanup (void)
{
	//FIXME can we safely destroy `domains`?
}

/*
 * BREAKPOINTS
 */

/* List of breakpoints */
/* Protected by the loader lock */
static GPtrArray *breakpoints;
/* Maps breakpoint locations to the number of breakpoints at that location */
static GHashTable *bp_locs;

static void
breakpoints_init (void)
{
	breakpoints = g_ptr_array_new ();
	bp_locs = g_hash_table_new (NULL, NULL);
}

/* Single stepping engine */
/* Number of single stepping operations in progress */
static int ss_count;

/* The single step request instances */
static GPtrArray *the_ss_reqs;

static void
ss_req_init (void)
{
	the_ss_reqs = g_ptr_array_new ();
}
