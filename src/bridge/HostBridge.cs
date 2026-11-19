using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Runtime.InteropServices.ComTypes;
using System.Text;
using System.Web.Script.Serialization;
using EnvDTE;
using Process = System.Diagnostics.Process;
using StackFrame = EnvDTE.StackFrame;

namespace OpenMSE.Bridge
{
    internal sealed class BridgeFailure : Exception
    {
        internal readonly string Code;
        internal BridgeFailure(string code, string message) : base(message) { Code = code; }
    }

    [ComImport, Guid("00000016-0000-0000-C000-000000000046"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    internal interface IOleMessageFilter
    {
        [PreserveSig] int HandleInComingCall(int type, IntPtr caller, int tick, IntPtr info);
        [PreserveSig] int RetryRejectedCall(IntPtr callee, int tick, int reject);
        [PreserveSig] int MessagePending(IntPtr callee, int tick, int pending);
    }

    internal sealed class MessageFilter : IOleMessageFilter
    {
        [DllImport("ole32.dll")] private static extern int CoRegisterMessageFilter(IOleMessageFilter next, out IOleMessageFilter previous);
        internal static void Register() { IOleMessageFilter previous; Marshal.ThrowExceptionForHR(CoRegisterMessageFilter(new MessageFilter(), out previous)); }
        internal static void Revoke() { IOleMessageFilter previous; CoRegisterMessageFilter(null, out previous); }
        public int HandleInComingCall(int type, IntPtr caller, int tick, IntPtr info) { return 0; }
        public int RetryRejectedCall(IntPtr callee, int tick, int reject) { return reject == 2 && tick < 10000 ? 100 : -1; }
        public int MessagePending(IntPtr callee, int tick, int pending) { return 2; }
    }

    internal sealed class Session : IDisposable
    {
        [DllImport("ole32.dll")] private static extern int GetRunningObjectTable(int reserved, out IRunningObjectTable table);
        [DllImport("ole32.dll")] private static extern int CreateBindCtx(int reserved, out IBindCtx context);
        [DllImport("user32.dll")] private static extern uint GetWindowThreadProcessId(IntPtr window, out uint pid);
        private readonly Process process;
        private readonly DTE dte;
        private readonly string workspace;
        private readonly string identity;
        private readonly bool owned;
        private readonly List<Breakpoint> createdBreakpoints = new List<Breakpoint>();
        internal readonly int Pid;
        internal readonly string Engine;
        internal readonly string Moniker;

        internal static List<Dictionary<string, object>> Enumerate()
        {
            IRunningObjectTable table; IBindCtx context; IEnumMoniker enumeration = null;
            Marshal.ThrowExceptionForHR(GetRunningObjectTable(0, out table));
            Marshal.ThrowExceptionForHR(CreateBindCtx(0, out context));
            var result = new List<Dictionary<string, object>>();
            try
            {
                table.EnumRunning(out enumeration); var entries = new IMoniker[1];
                while (enumeration.Next(1, entries, IntPtr.Zero) == 0)
                {
                    try
                    {
                        string name; entries[0].GetDisplayName(context, null, out name);
                        if (name.IndexOf("DTE", StringComparison.OrdinalIgnoreCase) >= 0 || name.IndexOf("MSE", StringComparison.OrdinalIgnoreCase) >= 0)
                            result.Add(new Dictionary<string, object> { { "moniker", name } });
                    }
                    catch (COMException) { }
                    finally { if (entries[0] != null) Marshal.ReleaseComObject(entries[0]); }
                }
            }
            finally
            {
                if (enumeration != null) Marshal.ReleaseComObject(enumeration);
                Marshal.ReleaseComObject(context); Marshal.ReleaseComObject(table);
            }
            return result;
        }

        internal Session(int pid, string engine, string root, string work, string ownedManifest)
        {
            if (engine != "mse" && engine != "vs8") throw new BridgeFailure("invalid_arguments", "Engine must be mse or vs8.");
            Pid = pid; Engine = engine; workspace = Path.GetFullPath(work).TrimEnd(Path.DirectorySeparatorChar) + Path.DirectorySeparatorChar;
            process = Process.GetProcessById(pid);
            IntPtr heldHandle = process.Handle; // Bind lifetime to this process, even if the PID is later reused.
            identity = process.StartTime.ToUniversalTime().ToFileTimeUtc().ToString(CultureInfo.InvariantCulture);
            string expected = Path.Combine(root, engine == "vs8" ? @"runtime\Common7\IDE\devenv.exe" : @"runtime\MSE7.EXE");
            if (!String.Equals(Path.GetFullPath(process.MainModule.FileName), Path.GetFullPath(expected), StringComparison.OrdinalIgnoreCase))
                throw new BridgeFailure("wrong_host", "PID does not identify the selected OpenMSE engine.");
            if (!String.IsNullOrEmpty(ownedManifest))
            {
                var marker = new JavaScriptSerializer().Deserialize<Dictionary<string, object>>(File.ReadAllText(ownedManifest));
                owned = Convert.ToInt32(marker["pid"]) == pid && Convert.ToString(marker["createdFileTime"]) == identity && Convert.ToString(marker["engine"]) == engine;
                if (!owned) throw new BridgeFailure("ownership_mismatch", "Owned session marker does not match the live process.");
            }
            IRunningObjectTable table; IBindCtx context; IEnumMoniker enumeration = null;
            Marshal.ThrowExceptionForHR(GetRunningObjectTable(0, out table)); Marshal.ThrowExceptionForHR(CreateBindCtx(0, out context));
            try
            {
                table.EnumRunning(out enumeration); var entries = new IMoniker[1];
                while (enumeration.Next(1, entries, IntPtr.Zero) == 0)
                {
                    try
                    {
                        string name; entries[0].GetDisplayName(context, null, out name);
                        if (!name.EndsWith(":" + pid, StringComparison.OrdinalIgnoreCase)) continue;
                        object candidate; table.GetObject(entries[0], out candidate);
                        DTE automation = candidate as DTE;
                        if (automation == null) { Marshal.ReleaseComObject(candidate); continue; }
                        uint actual; GetWindowThreadProcessId(new IntPtr(automation.MainWindow.HWnd), out actual);
                        if (actual != pid) { Marshal.ReleaseComObject(candidate); continue; }
                        dte = automation; Moniker = name; break;
                    }
                    finally { if (entries[0] != null) Marshal.ReleaseComObject(entries[0]); }
                }
            }
            finally
            {
                if (enumeration != null) Marshal.ReleaseComObject(enumeration);
                Marshal.ReleaseComObject(context); Marshal.ReleaseComObject(table);
            }
            if (dte == null) throw new BridgeFailure("automation_not_registered", "No PID-matched DTE object is registered in the running object table.");
        }

        private string GuardPath(string path)
        {
            string value = Path.GetFullPath(path);
            if (!value.StartsWith(workspace, StringComparison.OrdinalIgnoreCase))
                throw new BridgeFailure("path_outside_workspace", "Prototype operations are restricted to the explicit test workspace.");
            return value;
        }
        private void RequireOwned() { if (!owned) throw new BridgeFailure("owned_session_required", "This operation requires a matching owned session marker."); }
        private Document FindDocument(string path)
        {
            path = GuardPath(path);
            foreach (Document doc in dte.Documents)
                if (String.Equals(doc.FullName, path, StringComparison.OrdinalIgnoreCase)) return doc;
            throw new BridgeFailure("document_not_open", path);
        }
        private static string ReadDocument(Document document)
        {
            var text = document.Object("TextDocument") as TextDocument;
            if (text == null) throw new BridgeFailure("unsupported_document", "Document does not expose a TextDocument interface.");
            return text.StartPoint.CreateEditPoint().GetText(text.EndPoint);
        }
        private object DebugState()
        {
            var result = new Dictionary<string, object> { { "mode", dte.Debugger.CurrentMode.ToString() }, { "modeValue", (int)dte.Debugger.CurrentMode } };
            if (dte.Debugger.CurrentMode == dbgDebugMode.dbgBreakMode)
            {
                var frame = dte.Debugger.CurrentStackFrame;
                if (frame != null) result["function"] = frame.FunctionName;
                var frames = new List<string>();
                foreach (StackFrame item in dte.Debugger.CurrentThread.StackFrames) frames.Add(item.FunctionName);
                result["stack"] = frames;
            }
            return result;
        }
        internal object Dispatch(string method, Dictionary<string, object> args)
        {
            if (process.HasExited) throw new BridgeFailure("host_exited", "The bound host process has exited.");
            Func<string, string> value = key => args.ContainsKey(key) ? Convert.ToString(args[key], CultureInfo.InvariantCulture) : "";
            switch (method)
            {
                case "host.identity":
                    return new { pid = Pid, engine = Engine, createdFileTime = identity, moniker = Moniker, name = dte.Name, version = dte.Version, owned = owned };
                case "host.inspect":
                    var commands = new List<string>(); foreach (Command command in dte.Commands) if (!String.IsNullOrEmpty(command.Name)) commands.Add(command.Name);
                    return new { documents = dte.Documents.Count, solutionOpen = dte.Solution.IsOpen, commandCount = commands.Count,
                                 commands = commands, debugger = DebugState() };
                case "documents.open":
                    RequireOwned(); string path = GuardPath(value("path"));
                    Window window = dte.ItemOperations.OpenFile(path, Constants.vsViewKindTextView);
                    return new { path = window.Document.FullName, saved = window.Document.Saved };
                case "documents.read":
                    Document read = FindDocument(value("path")); return new { path = read.FullName, text = ReadDocument(read), saved = read.Saved };
                case "documents.write":
                    RequireOwned(); Document write = FindDocument(value("path")); string before = ReadDocument(write);
                    if (!args.ContainsKey("expectedText") || before != value("expectedText")) throw new BridgeFailure("conflict", "Buffer content changed; write rejected.");
                    var textDoc = (TextDocument)write.Object("TextDocument"); var point = textDoc.StartPoint.CreateEditPoint();
                    point.Delete(textDoc.EndPoint); point.Insert(value("text")); return new { saved = write.Saved };
                case "documents.save":
                    RequireOwned(); Document save = FindDocument(value("path")); save.Save(""); return new { saved = save.Saved };
                case "documents.close":
                    RequireOwned(); Document close = FindDocument(value("path"));
                    if (!close.Saved) throw new BridgeFailure("unsaved_document", "Save the audit document before closing it.");
                    close.Close(vsSaveChanges.vsSaveChangesNo); return new { closed = true };
                case "projects.open":
                    RequireOwned(); dte.Solution.Open(GuardPath(value("path"))); return new { path = dte.Solution.FullName, projects = dte.Solution.Projects.Count };
                case "build.run":
                    RequireOwned(); GuardPath(dte.Solution.FullName); dte.Solution.SolutionBuild.Build(true);
                    return new { failedProjects = dte.Solution.SolutionBuild.LastBuildInfo, state = dte.Solution.SolutionBuild.BuildState.ToString() };
                case "debug.inspect": return DebugState();
                case "debug.breakpoints.add":
                    RequireOwned(); string source = GuardPath(value("path")); int line = Convert.ToInt32(args["line"]);
                    Breakpoints points = dte.Debugger.Breakpoints.Add("", source, line, 1, "", dbgBreakpointConditionType.dbgBreakpointConditionTypeWhenTrue,
                                                                    "", "", 0, "", 0, dbgHitCountType.dbgHitCountTypeNone);
                    foreach (Breakpoint breakpoint in points) createdBreakpoints.Add(breakpoint);
                    return new { count = points.Count };
                case "debug.start":
                    RequireOwned(); GuardPath(dte.Solution.FullName); dte.Debugger.Go(false); return DebugState();
                case "debug.step_over":
                    RequireOwned(); if (dte.Debugger.CurrentMode != dbgDebugMode.dbgBreakMode) throw new BridgeFailure("invalid_state", "Debugger is not stopped at a breakpoint.");
                    dte.Debugger.StepOver(false); return DebugState();
                case "debug.evaluate":
                    RequireOwned(); if (dte.Debugger.CurrentMode != dbgDebugMode.dbgBreakMode) throw new BridgeFailure("invalid_state", "Debugger is not in break mode.");
                    Expression expression = dte.Debugger.GetExpression(value("expression"), false, 2000);
                    return new { valid = expression.IsValidValue, type = expression.Type, value = expression.Value };
                case "debug.stop":
                    RequireOwned(); dte.Debugger.Stop(false); return DebugState();
                case "host.shutdown":
                    RequireOwned();
                    if (dte.Debugger.CurrentMode != dbgDebugMode.dbgDesignMode) throw new BridgeFailure("debug_session_active", "Stop the owned debug session before shutdown.");
                    foreach (Document doc in dte.Documents)
                        if (!doc.Saved) throw new BridgeFailure("unsaved_document", "Host contains unsaved documents.");
                    foreach (Breakpoint pointToDelete in createdBreakpoints) pointToDelete.Delete();
                    if (dte.Solution.IsOpen) { GuardPath(dte.Solution.FullName); dte.Solution.Close(false); }
                    dte.Quit(); return new { shutdownRequested = true };
                default: throw new BridgeFailure("method_not_supported", method);
            }
        }
        public void Dispose() { if (dte != null) Marshal.ReleaseComObject(dte); process.Dispose(); }
    }

    internal static class Program
    {
        private static readonly JavaScriptSerializer Json = new JavaScriptSerializer { MaxJsonLength = 4 * 1024 * 1024 };
        [STAThread]
        private static int Main(string[] args)
        {
            Console.InputEncoding = new UTF8Encoding(false); Console.OutputEncoding = new UTF8Encoding(false);
            MessageFilter.Register();
            try
            {
                if (args.Length == 1 && args[0] == "--enumerate") { Console.WriteLine(Json.Serialize(Session.Enumerate())); return 0; }
                if (args.Length != 6 || args[5] != "--stdio") throw new BridgeFailure("invalid_arguments", "Expected PID ENGINE PROJECT_ROOT WORKSPACE OWNED_MARKER_OR_DASH --stdio");
                using (var session = new Session(Int32.Parse(args[0]), args[1], args[2], args[3], args[4] == "-" ? "" : args[4]))
                {
                    Console.WriteLine(Json.Serialize(new { type = "ready", protocolVersion = 1, pid = session.Pid, engine = session.Engine })); Console.Out.Flush();
                    string line;
                    while ((line = Console.ReadLine()) != null)
                    {
                        object id = null;
                        try
                        {
                            if (line.Length > 1024 * 1024) throw new BridgeFailure("request_too_large", "Maximum request size is 1 MiB.");
                            var request = Json.Deserialize<Dictionary<string, object>>(line);
                            if (request == null || !request.ContainsKey("id") || !request.ContainsKey("method")) throw new BridgeFailure("invalid_request", "Request requires id and method.");
                            id = request["id"];
                            var parameters = request.ContainsKey("params") ? request["params"] as Dictionary<string, object> : null;
                            object result = session.Dispatch(Convert.ToString(request["method"]), parameters ?? new Dictionary<string, object>());
                            Console.WriteLine(Json.Serialize(new { id = id, ok = true, result = result }));
                        }
                        catch (Exception error) { Console.WriteLine(Json.Serialize(Error(id, error))); }
                        Console.Out.Flush();
                    }
                }
                return 0;
            }
            catch (Exception error) { Console.WriteLine(Json.Serialize(Error(null, error))); return 1; }
            finally { MessageFilter.Revoke(); }
        }
        private static object Error(object id, Exception error)
        {
            while (error is TargetInvocationException && error.InnerException != null) error = error.InnerException;
            var failure = error as BridgeFailure; var com = error as COMException;
            string code = failure != null ? failure.Code : (com != null && (uint)com.ErrorCode == 0x80010001 ? "host_busy" : "host_operation_failed");
            return new { id = id, ok = false, error = new { code = code, message = error.Message, hresult = "0x" + error.HResult.ToString("X8") } };
        }
    }
}
