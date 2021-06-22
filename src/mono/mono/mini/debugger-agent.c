/**
 * \file
 * Soft Debugger back-end module
 *
 * Author:
 *   Zoltan Varga (vargaz@gmail.com)
 *
 * Copyright 2009-2010 Novell, Inc.
 * Copyright 2011 Xamarin Inc.
 * Licensed under the MIT license. See LICENSE file in the project root for full license information.
 */

#include <glib.h>
#include <mono/metadata/components.h>

#ifndef DISABLE_SDB

MONO_API void
mono_debugger_agent_register_transport (DebuggerTransport *trans);

void
mono_debugger_agent_register_transport (DebuggerTransport *trans)
{
	mono_component_debugger ()->register_transport (trans);
}

gboolean
mono_debugger_agent_transport_handshake (void)
{
	return mono_component_debugger ()->mono_debugger_agent_transport_handshake ();
}

void
mono_debugger_agent_init (void)
{
	//not need to do anything anymore
}

void
mono_debugger_agent_parse_options (char *options)
{
	mono_component_debugger ()->mono_debugger_agent_parse_options (options);
}

#endif /* DISABLE_SDB */
