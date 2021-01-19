// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.
//
// File: CORDB.CPP
//

#include <fstream>
#include <iostream>

#include <cordb-appdomain.h>
#include <cordb-assembly.h>
#include <cordb-breakpoint.h>
#include <cordb-code.h>
#include <cordb-eval.h>
#include <cordb-frame.h>
#include <cordb-function.h>
#include <cordb-process.h>
#include <cordb-stepper.h>
#include <cordb-symbol.h>
#include <cordb-thread.h>
#include <cordb.h>

int convert_mono_type_2_icordbg_size(int type) {
  switch (type) {
  case MONO_TYPE_VOID:
    return 0;
  case MONO_TYPE_BOOLEAN:
  case MONO_TYPE_I1:
  case MONO_TYPE_U1:
    return 1;
    break;
  case MONO_TYPE_CHAR:
  case MONO_TYPE_I2:
  case MONO_TYPE_U2:
    return 2;
  case MONO_TYPE_I4:
  case MONO_TYPE_U4:
  case MONO_TYPE_R4:
    return 4;
  case MONO_TYPE_I8:
  case MONO_TYPE_U8:
  case MONO_TYPE_R8:
    return 8;
  }
  return 0;
}

MONO_API HRESULT CoreCLRCreateCordbObjectEx(int iDebuggerVersion, DWORD pid,
                                            LPCWSTR lpApplicationGroupId,
                                            HMODULE hmodTargetCLR,
                                            void **ppCordb) {
  DEBUG_PRINTF(1, "CoreCLRCreateCordbObjectEx \n");
  *ppCordb = new Cordb();
  return S_OK;
}

static void receive_thread(Connection *c) { c->receive(); }

static void debugger_thread(void *ppProcess) {
  Connection *connection = new Connection((CordbProcess *)ppProcess,
                                          ((CordbProcess *)ppProcess)->cordb);
  ((CordbProcess *)ppProcess)->SetConnection(connection);
  connection->start_connection();
  connection->transport_handshake();
  connection->loop_send_receive();
  connection->close_connection();
}

HRESULT Cordb::Initialize(void) {
  DEBUG_PRINTF(1, "Cordb - Initialize - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::Terminate(void) {
  DEBUG_PRINTF(1, "Cordb - Terminate - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::SetManagedHandler(
    /* [in] */ ICorDebugManagedCallback *pCallback) {
  DEBUG_PRINTF(1, "Cordb - SetManagedHandler - IMPLEMENTED\n");
  this->pCallback = pCallback;
  this->pCallback->AddRef();

  return S_OK;
}

HRESULT Cordb::SetUnmanagedHandler(
    /* [in] */ ICorDebugUnmanagedCallback *pCallback) {
  DEBUG_PRINTF(1, "Cordb - SetUnmanagedHandler - NOT IMPLEMENTED\n");
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
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - CreateProcess - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::DebugActiveProcess(
    /* [in] */ DWORD id,
    /* [in] */ BOOL win32Attach,
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - DebugActiveProcess - IMPLEMENTED\n");
  *ppProcess = new CordbProcess();
  ((CordbProcess *)*ppProcess)->cordb = this;

  DWORD thread_id;
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)debugger_thread,
               ((CordbProcess *)*ppProcess), 0, &thread_id);
  return S_OK;
}

HRESULT Cordb::EnumerateProcesses(
    /* [out] */ ICorDebugProcessEnum **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - EnumerateProcesses - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::GetProcess(
    /* [in] */ DWORD dwProcessId,
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - GetProcess - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::CanLaunchOrAttach(
    /* [in] */ DWORD dwProcessId,
    /* [in] */ BOOL win32DebuggingEnabled) {
  DEBUG_PRINTF(1, "Cordb - CanLaunchOrAttach - NOT IMPLEMENTED\n");
  return S_OK;
}

Cordb::Cordb() {
  pCallback = NULL;
  breakpoints = g_ptr_array_new();
  threads = g_ptr_array_new();
  functions = g_ptr_array_new();
  modules = g_hash_table_new(NULL, NULL);
}

CordbFunction *Cordb::findFunction(int id) {
  int i = 0;
  while (i < functions->len) {
    CordbFunction *function = (CordbFunction *)g_ptr_array_index(functions, i);
    if (function->id == id) {
      return function;
    }
    i++;
  }
  return NULL;
}

CordbFunction *Cordb::findFunctionByToken(int token) {
  int i = 0;
  while (i < functions->len) {
    CordbFunction *function = (CordbFunction *)g_ptr_array_index(functions, i);
    if (function->token == token) {
      return function;
    }
    i++;
  }
  return NULL;
}

HRESULT Cordb::QueryInterface(
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ _COM_Outptr_ void __RPC_FAR *__RPC_FAR *ppvObject) {
  DEBUG_PRINTF(1, "Cordb - QueryInterface - NOT IMPLEMENTED\n");
  return S_OK;
}

ULONG Cordb::AddRef(void) { return S_OK; }

ULONG Cordb::Release(void) { return S_OK; }

HRESULT Cordb::CreateProcessEx(
    /* [in] */ ICorDebugRemoteTarget *pRemoteTarget,
    /* [in] */ LPCWSTR lpApplicationName,
    /* [annotation][in] */
    _In_ LPWSTR lpCommandLine,
    /* [in] */ LPSECURITY_ATTRIBUTES lpProcessAttributes,
    /* [in] */ LPSECURITY_ATTRIBUTES lpThreadAttributes,
    /* [in] */ BOOL bInheritHandles,
    /* [in] */ DWORD dwCreationFlags,
    /* [in] */ PVOID lpEnvironment,
    /* [in] */ LPCWSTR lpCurrentDirectory,
    /* [in] */ LPSTARTUPINFOW lpStartupInfo,
    /* [in] */ LPPROCESS_INFORMATION lpProcessInformation,
    /* [in] */ CorDebugCreateProcessFlags debuggingFlags,
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - CreateProcessEx - NOT IMPLEMENTED\n");
  return S_OK;
}

HRESULT Cordb::DebugActiveProcessEx(
    /* [in] */ ICorDebugRemoteTarget *pRemoteTarget,
    /* [in] */ DWORD dwProcessId,
    /* [in] */ BOOL fWin32Attach,
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "Cordb - DebugActiveProcessEx - NOT IMPLEMENTED\n");
  return S_OK;
}

Connection::Connection(CordbProcess *proc, Cordb *cordb) {
  ppProcess = proc;
  ppCordb = cordb;
  pCorDebugAppDomain = NULL;
  received_replies = g_hash_table_new(NULL, NULL);
  received_packets_to_process = g_ptr_array_new();
  is_answer_pending = false;
  pending_eval = g_ptr_array_new();
}

CordbThread *Connection::findThread(GPtrArray *threads, long thread_id) {
  int i = 0;
  while (i < threads->len) {
    CordbThread *thread = (CordbThread *)g_ptr_array_index(threads, i);
    if (thread->thread_id == thread_id) {
      return thread;
    }
    i++;
  }
  return NULL;
}

void Connection::receive() {
  while (true) {
    Buffer recvbuf_header;
    buffer_init(&recvbuf_header, HEADER_LEN);

    int iResult =
        recv(connect_socket, (char *)recvbuf_header.buf, HEADER_LEN, 0);

    if (iResult == -1)
      break;
    while (iResult == 0) {
      DEBUG_PRINTF(1,
                   "[dbg] transport_recv () sleep returned %d, expected %d.\n",
                   iResult, HEADER_LEN);
      iResult = recv(connect_socket, (char *)recvbuf_header.buf, HEADER_LEN, 0);
      Sleep(1000);
    }

    Header header;
    Buffer *recvbuf = new Buffer();
    decode_command_header(&recvbuf_header, &header);

    buffer_init(recvbuf, header.len - HEADER_LEN);

    if (header.len < HEADER_LEN) {
      return;
    }

    if (header.len - HEADER_LEN != 0) {
      iResult = recv(connect_socket, (char *)recvbuf->buf,
                     header.len - HEADER_LEN, 0);
      int totalRead = iResult;
      while (totalRead < header.len - HEADER_LEN) {
        iResult = recv(connect_socket, (char *)recvbuf->buf + totalRead,
                       (header.len - HEADER_LEN) - totalRead, 0);
        totalRead += iResult;
      }
    }

    dbg_lock();
    if (header.flags == REPLY_PACKET) {
      DEBUG_PRINTF(
          1, "header->id - %d - header->error - %d - header->error_2 - %d\n",
          header.id, header.error, header.error_2);
      ReceivedReplyPacket *rp =
          (ReceivedReplyPacket *)g_malloc0(sizeof(ReceivedReplyPacket));
      rp->error = header.error;
      rp->error_2 = header.error_2;
      rp->buf = recvbuf;
      g_hash_table_insert(received_replies, (gpointer)(gssize)(header.id), rp);
    } else {
      g_ptr_array_add(received_packets_to_process, recvbuf);
    }
    dbg_unlock();
  }
}

Buffer *Connection::get_answer(int cmdId) {
  ReceivedReplyPacket *ret = NULL;
  while (ret == NULL) {
    dbg_lock();
    ret = (ReceivedReplyPacket *)g_hash_table_lookup(received_replies,
                                                     (gpointer)(gssize)(cmdId));
    dbg_unlock();
  }
  return ret->buf;
}

ReceivedReplyPacket *Connection::get_answer_with_error(int cmdId) {
  ReceivedReplyPacket *ret = NULL;
  while (ret == NULL) {
    dbg_lock();
    ret = (ReceivedReplyPacket *)g_hash_table_lookup(received_replies,
                                                     (gpointer)(gssize)(cmdId));
    dbg_unlock();
  }
  return ret;
}

void Connection::process_packet_internal(Buffer *recvbuf) {
  int spolicy = decode_byte(recvbuf->buf, &recvbuf->buf, recvbuf->end);
  int nevents = decode_int(recvbuf->buf, &recvbuf->buf, recvbuf->end);

  for (int i = 0; i < nevents; ++i) {

    int kind = decode_byte(recvbuf->buf, &recvbuf->buf, recvbuf->end);
    int req_id = decode_int(recvbuf->buf, &recvbuf->buf, recvbuf->end);

    EventKind etype = (EventKind)kind;

    long thread_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);

    DEBUG_PRINTF(1, "Received %d %d events %s, suspend=%d\n", i, nevents,
                 event_to_string(etype), etype);

    switch (etype) {
    case EVENT_KIND_VM_START: {
      ppProcess->suspended = true;
      ppCordb->pCallback->CreateProcess(
          static_cast<ICorDebugProcess *>(ppProcess));
    } break;
    case EVENT_KIND_THREAD_START: {
      DEBUG_PRINTF(1, "criei a thread certinha pelo EVENT_KIND_THREAD_START\n");
      CordbThread *thread = new CordbThread(this, ppProcess, thread_id);
      g_ptr_array_add(ppCordb->threads, thread);
      ppCordb->pCallback->CreateThread(pCorDebugAppDomain, thread);
    } break;
    case EVENT_KIND_APPDOMAIN_CREATE: {

    } break;
    case EVENT_KIND_ASSEMBLY_LOAD: {
      // all the callbacks call a resume, in this case that we are faking 2
      // callbacks without receive command, we should not send the continue
      int assembly_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
      if (pCorDebugAppDomain == NULL) {
        pCorDebugAppDomain = new CordbAppDomain(
            this, static_cast<ICorDebugProcess *>(ppProcess));
        ppCordb->pCallback->CreateAppDomain(
            static_cast<ICorDebugProcess *>(ppProcess), pCorDebugAppDomain);
      }
      DEBUG_PRINTF(1, "Recebi assembly load - %d\n", assembly_id);
      ICorDebugAssembly *pAssembly =
          new CordbAssembly(this, ppProcess, pCorDebugAppDomain, assembly_id);
      ppCordb->pCallback->LoadAssembly(pCorDebugAppDomain, pAssembly);

      ppProcess->suspended = true;
      ICorDebugModule *pModule = new CordbModule(
          this, ppProcess, (CordbAssembly *)pAssembly, assembly_id);
      g_hash_table_insert(ppCordb->modules, GINT_TO_POINTER(assembly_id),
                          pModule);
      ppCordb->pCallback->LoadModule(pCorDebugAppDomain, pModule);
    } break;
    case EVENT_KIND_BREAKPOINT: {
      int method_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
      long offset = decode_long(recvbuf->buf, &recvbuf->buf, recvbuf->end);
      CordbThread *thread = findThread(ppCordb->threads, thread_id);
      if (thread == NULL) {
        DEBUG_PRINTF(1, "criei a thread errada pelo EVENT_KIND_BREAKPOINT\n");
        thread = new CordbThread(this, ppProcess, thread_id);
        g_ptr_array_add(ppCordb->threads, thread);
        ppCordb->pCallback->CreateThread(pCorDebugAppDomain, thread);
      }
      int i = 0;
      CordbFunctionBreakpoint *breakpoint;
      while (i < ppCordb->breakpoints->len) {
        ppProcess->suspended = true;
        breakpoint = (CordbFunctionBreakpoint *)g_ptr_array_index(
            ppCordb->breakpoints, i);
        if (breakpoint->offset == offset &&
            breakpoint->code->func->id == method_id) {
          ppCordb->pCallback->Breakpoint(
              pCorDebugAppDomain, thread,
              static_cast<ICorDebugFunctionBreakpoint *>(breakpoint));
          break;
        }
        i++;
      }
    } break;
    case EVENT_KIND_STEP: {
      int method_id = decode_id(recvbuf->buf, &recvbuf->buf, recvbuf->end);
      long offset = decode_long(recvbuf->buf, &recvbuf->buf, recvbuf->end);

      DEBUG_PRINTF(1, "EVENT_KIND_STEP - %d - %d - %d\n", thread_id, method_id,
                   offset);

      CordbThread *thread = findThread(ppCordb->threads, thread_id);
      if (thread == NULL) {
        DEBUG_PRINTF(1, "criei a thread errada pelo EVENT_KIND_STEP\n");
        thread = new CordbThread(this, ppProcess, thread_id);
        g_ptr_array_add(ppCordb->threads, thread);
        ppCordb->pCallback->CreateThread(pCorDebugAppDomain, thread);
      }
      ppProcess->suspended = true;
      if (!thread->stepper->isComplete)
        ppCordb->pCallback->StepComplete(pCorDebugAppDomain, thread,
                                         thread->stepper, STEP_NORMAL);
      thread->stepper->isComplete = true;
    } break;
    }
  }
  // buffer_free(&recvbuf);
}

int Connection::process_packet(bool is_answer) {
  if (!is_answer)
    process_packet_from_queue();
  return 1;
}

void Connection::process_packet_from_queue() {
  int i = 0;
  while (i < received_packets_to_process->len) {
    Buffer *req = (Buffer *)g_ptr_array_index(received_packets_to_process, i);
    process_packet_internal(req);
    dbg_lock();
    g_ptr_array_remove_index_fast(received_packets_to_process, i);
    dbg_unlock();
    delete req;
    i--;
  }
  while (i < pending_eval->len) {
    CordbEval *eval = (CordbEval *)g_ptr_array_index(pending_eval, i);
    Buffer *recvbuf = get_answer(eval->cmdId);
    eval->EvalComplete(recvbuf);
    dbg_lock();
    g_ptr_array_remove_index_fast(pending_eval, i);
    dbg_unlock();
    i--;
  }
}

void Connection::loop_send_receive() {
  DWORD thread_id;
  CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)receive_thread, this, 0,
               &thread_id);

  // machine.EnableEvents(EventType.AssemblyLoad, EventType.ThreadStart,
  // EventType.ThreadDeath, EventType.AppDomainUnload, EventType.UserBreak,
  // EventType.UserLog);
  enable_event(EVENT_KIND_ASSEMBLY_LOAD);
  enable_event(EVENT_KIND_APPDOMAIN_CREATE);
  enable_event(EVENT_KIND_THREAD_START);
  enable_event(EVENT_KIND_THREAD_DEATH);
  enable_event(EVENT_KIND_APPDOMAIN_UNLOAD);
  enable_event(EVENT_KIND_USER_BREAK);
  enable_event(EVENT_KIND_USER_LOG);

  Buffer localbuf;
  buffer_init(&localbuf, 128);
  buffer_add_int(&localbuf, MAJOR_VERSION);
  buffer_add_int(&localbuf, MINOR_VERSION);
  int cmdId = send_event(CMD_SET_VM, CMD_VM_SET_PROTOCOL_VERSION, &localbuf);
  buffer_free(&localbuf);

  buffer_init(&localbuf, 128);
  cmdId = send_event(CMD_SET_VM, CMD_VM_VERSION, &localbuf);
  buffer_free(&localbuf);

  Buffer *localbuf2 = get_answer(cmdId);
  char *vm_version =
      decode_string(localbuf2->buf, &localbuf2->buf, localbuf2->end);
  int major_version =
      decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);
  int minor_version =
      decode_int(localbuf2->buf, &localbuf2->buf, localbuf2->end);

  DEBUG_PRINTF(1,
               "[dbg] Protocol version %d.%d, server protocol version %d.%d.\n",
               MAJOR_VERSION, MINOR_VERSION, major_version, minor_version);

  int iResult = 0;
  // Receive until the peer closes the connection
  do {
    if (!ppProcess->suspended)
      iResult = process_packet();
    Sleep(100);
  } while (iResult >= 0);
}

void Connection::enable_event(EventKind eventKind) {
  Buffer sendbuf;
  int buflen = 128;
  buffer_init(&sendbuf, buflen);
  buffer_add_byte(&sendbuf, eventKind);
  buffer_add_byte(&sendbuf, SUSPEND_POLICY_ALL);
  buffer_add_byte(&sendbuf, 0); // modifiers
  send_event(CMD_SET_EVENT_REQUEST, CMD_EVENT_REQUEST_SET, &sendbuf);
  buffer_free(&sendbuf);
}

void Connection::close_connection() {
  closesocket(connect_socket);
  WSACleanup();
}

void Connection::start_connection() {
  DEBUG_PRINTF(1, "Start Connection\n");

  WSADATA wsaData;
  connect_socket = INVALID_SOCKET;
  struct addrinfo *result = NULL, *ptr = NULL, hints;
  int iResult;

  // Initialize Winsock
  iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
  if (iResult != 0) {
    return;
  }

  ZeroMemory(&hints, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_protocol = IPPROTO_TCP;

  DEBUG_PRINTF(1, "Listening to 127.0.0.1:1003\n");

  // Resolve the server address and port
  iResult = getaddrinfo("127.0.0.1", "1003", &hints, &result);
  if (iResult != 0) {
    WSACleanup();
    return;
  }

  // Attempt to connect to an address until one succeeds
  for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

    // Create a SOCKET for connecting to server
    connect_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

    if (connect_socket == INVALID_SOCKET) {
      WSACleanup();
      return;
    }

    int flag = 1;
    if (setsockopt(connect_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&flag,
                   sizeof(int)))
      continue;

    iResult = bind(connect_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == -1)
      continue;

    iResult = listen(connect_socket, 16);
    if (iResult == -1)
      continue;

    break;
  }

  connect_socket = accept(connect_socket, NULL, NULL);
  if (connect_socket == -1)
    exit(1);

  DEBUG_PRINTF(1, "Accepted connection from client.\n");

  freeaddrinfo(result);

  if (connect_socket == INVALID_SOCKET) {
    WSACleanup();
    return;
  }
}

void Connection::transport_handshake() {
  int buflen = 128;

  Buffer sendbuf;
  buffer_init(&sendbuf, buflen);

  Buffer recvbuf;
  buffer_init(&recvbuf, buflen);

  int iResult;
  iResult = recv(connect_socket, (char *)recvbuf.buf, buflen, 0);

  // Send an initial buffer
  buffer_add_data(&sendbuf, (guint8 *)"DWP-Handshake", 13);
  send_packet(sendbuf);
}

void Connection::send_packet(Buffer &sendbuf) {
  int iResult =
      send(connect_socket, (const char *)sendbuf.buf, buffer_len(&sendbuf), 0);
  if (iResult == SOCKET_ERROR) {
    WSACleanup();
    return;
  }
}

void Connection::receive_packet(Buffer &recvbuf, int len) {
  buffer_init(&recvbuf, len);
  int iResult;
  iResult = recv(connect_socket, (char *)recvbuf.buf, len, 0);
}

void Connection::receive_header(Header *header) {
  Buffer recvbuf;
  buffer_init(&recvbuf, 11);
  int iResult;
  iResult = recv(connect_socket, (char *)recvbuf.buf, 11, 0);
  decode_command_header(&recvbuf, header);
}

int Connection::send_event(int cmd_set, int cmd, Buffer *sendbuf) {
  Buffer outbuf;
  int ret = buffer_add_command_header(sendbuf, cmd_set, cmd, &outbuf);
  send_packet(outbuf);
  return ret;
}

MONO_API HRESULT CoreCLRCreateCordbObject(int iDebuggerVersion, DWORD pid,
                                          HMODULE hmodTargetCLR,
                                          void **ppCordb) {
  DEBUG_PRINTF(1, "CoreCLRCreateCordbObject \n");
  *ppCordb = new Cordb();
  return S_OK;
}

MONO_API HRESULT CreateCordbObject(int iDebuggerVersion, void **ppCordb) {
  DEBUG_PRINTF(1, "CreateCordbObject \n");
  *ppCordb = new Cordb();
  return S_OK;
}

HRESULT CordbModule::GetAssembly(
    /* [out] */ ICorDebugAssembly **ppAssembly) {
  DEBUG_PRINTF(1, "CordbModule - GetAssembly\n");
  *ppAssembly = static_cast<ICorDebugAssembly *>(pAssembly);
  return S_OK;
}

HRESULT CordbAssembly::GetProcess(
    /* [out] */ ICorDebugProcess **ppProcess) {
  DEBUG_PRINTF(1, "CorDebugAssembly - GetProcess\n");
  *ppProcess = static_cast<ICorDebugProcess *>(pProcess);
  return S_OK;
}

CordbBaseMono::CordbBaseMono(Connection *conn) { this->conn = conn; }

void CordbBaseMono::SetConnection(Connection *conn) { this->conn = conn; }
