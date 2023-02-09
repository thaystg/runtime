// Licensed to the .NET Foundation under one or more agreements.
// The .NET Foundation licenses this file to you under the MIT license.

using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Net.WebSockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using Microsoft.Extensions.Logging;
using Microsoft.WebAssembly.Diagnostics;
using Newtonsoft.Json.Linq;

namespace DebuggerTests
{
    internal class InspectorClient : DevToolsClient
    {
        protected Dictionary<MessageId, TaskCompletionSource<Result>> pending_cmds = new Dictionary<MessageId, TaskCompletionSource<Result>>();
        protected Func<string, string, JObject, CancellationToken, Task> onEvent;
        protected int next_cmd_id;
        public SessionId CurrentSessionId { get; set; } = SessionId.Null;

        public InspectorClient(ILogger logger) : base(logger) { }

        protected override async Task<WasmDebuggerConnection> SetupConnection(Uri webserverUri, CancellationToken token)
            => new DevToolsDebuggerConnection(
                        await ConnectToWebServer(webserverUri, token),
                        "client",
                         logger);

        protected virtual Task HandleMessage(string msg, CancellationToken token)
        {
            var res = JObject.Parse(msg);
            Console.WriteLine($"to recebendo a mensagem - {msg}");
            if (res["method"]?.Value<string>()?.Equals("Runtime.executionContextCreated") == true)
            {
                //pending_cmds.Clear();
                //next_cmd_id = 1;
                //SendCommand("Runtime.enable", null, token);
                Console.WriteLine("vou enviar de novo");
                //SendCommand("Debugger.enable", null, token);
                //SendCommand("Debugger.setAsyncCallStackDepth", JObject.FromObject(new { maxDepth = 32 }), token);
            }
            if (res["id"] == null)
                return onEvent(res["sessionId"]?.Value<string>(), res["method"].Value<string>(), res["params"] as JObject, token);

            var id = res.ToObject<MessageId>();
            if (!pending_cmds.Remove(id, out var item))
                logger.LogError($"Unable to find command {id}");
            else
                item.SetResult(Result.FromJson(res));
            return null;
        }

        public virtual async Task ProcessCommand(Result command, CancellationToken token)
        {
            await Task.FromResult(true);
        }

        public virtual async Task Connect(
            Uri uri,
            Func<string, string, JObject, CancellationToken, Task> onEvent,
            CancellationTokenSource cts)
        {
            this.onEvent = onEvent;

            RunLoopStopped += (_, args) =>
            {
                logger.LogDebug($"Failing {pending_cmds.Count} pending cmds");
                foreach (var pending in pending_cmds)
                {

                }
                if (args.reason == RunLoopStopReason.Exception)
                {
                    foreach (var cmd in pending_cmds.Values)
                        cmd.SetException(args.exception);
                }
                else
                {
                    foreach (var cmd in pending_cmds.Values)
                        cmd.SetCanceled();
                }
            };

            await ConnectAndStartRunLoopAsync(uri, HandleMessage, cts);
        }

        public Task<Result> SendCommand(string method, JObject args, CancellationToken token)
            => SendCommand(CurrentSessionId, method, args, token);

        public virtual Task<Result> SendCommand(SessionId sessionId, string method, JObject args, CancellationToken token)
        {
            int id = ++next_cmd_id;
            Console.WriteLine($"to enviando a mensagem - {id} - {method} - {args}");            
            if (args == null)
                args = new JObject();

            var o = JObject.FromObject(new
            {
                id = id,
                method = method,
                @params = args
            });

            if (sessionId != SessionId.Null)
                o.Add("sessionId", sessionId.sessionId);
            var tcs = new TaskCompletionSource<Result>();
            pending_cmds[new MessageId(sessionId.sessionId, id)] = tcs;

            var str = o.ToString();

            var bytes = Encoding.UTF8.GetBytes(str);
            Send(bytes, token);
            return tcs.Task;
        }
    }
}
