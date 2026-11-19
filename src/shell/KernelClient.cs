using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Web.Script.Serialization;

namespace OpenMSE.Shell
{
    // Managed side of the OpenMSE.Core C ABI (include/openmse/kernel/abi.h). Only this file knows the native boundary.
    internal sealed class KernelException : Exception
    {
        internal readonly string Code;
        internal readonly int Status;
        internal KernelException(int status,string code,string message) : base(message) { Status=status; Code=code; }
    }

    internal enum ClosePolicy : uint { RefuseDirty=0, Save=1, Discard=2 }

    internal sealed class KernelEdit
    {
        internal int BeginLine, BeginColumn, EndLine, EndColumn;
        internal string Text;
    }

    internal sealed class KernelState
    {
        internal ulong Id, Revision;
        internal bool Dirty, CanUndo, CanRedo;
        internal KernelEdit Edit;
        internal static KernelState From(Dictionary<string,object> r)
        {
            var s=new KernelState { Id=KernelClient.U64(r["id"]), Revision=KernelClient.U64(r["revision"]), Dirty=(bool)r["dirty"], CanUndo=(bool)r["can_undo"], CanRedo=(bool)r["can_redo"] };
            object edit;
            if(r.TryGetValue("edit",out edit) && edit!=null) {
                var e=(Dictionary<string,object>)edit; var b=(Dictionary<string,object>)e["begin"]; var x=(Dictionary<string,object>)e["end"];
                s.Edit=new KernelEdit { BeginLine=(int)KernelClient.U64(b["line"]), BeginColumn=(int)KernelClient.U64(b["column"]), EndLine=(int)KernelClient.U64(x["line"]), EndColumn=(int)KernelClient.U64(x["column"]), Text=(string)e["text"] };
            }
            return s;
        }
    }

    internal sealed class KernelSnapshot
    {
        internal KernelState State;
        internal string Name, Path, Text, Encoding, Newline;
        internal bool ReadOnly, AlreadyOpen;
        internal static KernelSnapshot From(Dictionary<string,object> r)
        {
            return new KernelSnapshot { State=KernelState.From(r), Name=(string)r["name"], Path=r["path"] as string, Text=(string)r["text"], Encoding=(string)r["encoding"], Newline=(string)r["newline"], ReadOnly=(bool)r["read_only"], AlreadyOpen=(bool)r["already_open"] };
        }
    }

    internal sealed class KernelEvent
    {
        internal ulong Sequence;
        internal string Topic;
        internal Dictionary<string,object> Payload;
        internal ulong DocumentId { get { object id; return Payload!=null && Payload.TryGetValue("id",out id) ? KernelClient.U64(id) : 0; } }
    }

    internal sealed class KernelCommandState
    {
        internal string Id, UnavailableReason;
        internal bool Enabled;
    }

    internal sealed class KernelClient : IDisposable
    {
        const string Dll="OpenMSE.Core.dll";
        internal const uint AbiVersion=5, EditCoalesce=1;
        [StructLayout(LayoutKind.Sequential)] struct Buffer { internal IntPtr Data; internal UIntPtr Size; }
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern uint omk_abi_version();
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_create(uint version,out IntPtr kernel);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern void omk_destroy(IntPtr kernel);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern void omk_buffer_free(ref Buffer buffer);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_create(IntPtr k,byte[] name,UIntPtr nameSize,byte[] text,UIntPtr textSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_open(IntPtr k,byte[] path,UIntPtr pathSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_snapshot(IntPtr k,ulong document,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_replace(IntPtr k,ulong document,ulong expected,uint beginLine,uint beginColumn,uint endLine,uint endColumn,byte[] text,UIntPtr textSize,uint flags,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_save(IntPtr k,ulong document,ulong expected,byte[] path,UIntPtr pathSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_document_close(IntPtr k,ulong document,uint policy,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_command_query(IntPtr k,byte[] id,UIntPtr idSize,ulong document,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_command_execute(IntPtr k,byte[] id,UIntPtr idSize,ulong document,ulong expected,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_event_poll(IntPtr k,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_workspace_open(IntPtr k,byte[] path,UIntPtr pathSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_workspace_close(IntPtr k,ulong workspace,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_build_start(IntPtr k,ulong workspace,byte[] configuration,UIntPtr configurationSize,byte[] project,UIntPtr projectSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_run_start(IntPtr k,ulong workspace,byte[] configuration,UIntPtr configurationSize,byte[] project,UIntPtr projectSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_task_cancel(IntPtr k,ulong task,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_task_snapshot(IntPtr k,ulong task,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_start(IntPtr k,ulong workspace,byte[] configuration,UIntPtr configurationSize,byte[] project,UIntPtr projectSize,byte[] breakpoints,UIntPtr breakpointsSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_snapshot(IntPtr k,ulong session,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_control(IntPtr k,ulong session,byte[] action,UIntPtr actionSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_breakpoint_add(IntPtr k,ulong session,byte[] file,UIntPtr fileSize,uint line,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_variables(IntPtr k,ulong session,uint frame,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_debug_evaluate(IntPtr k,ulong session,uint frame,byte[] expression,UIntPtr expressionSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_capabilities(IntPtr k,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_hosts_load(IntPtr k,byte[] manifest,UIntPtr manifestSize,byte[] root,UIntPtr rootSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_host_start(IntPtr k,byte[] engine,UIntPtr engineSize,byte[] workspace,UIntPtr workspaceSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_host_sync_document(IntPtr k,ulong host,ulong document,uint timeoutMs,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_host_verify(IntPtr k,ulong host,uint timeoutMs,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_host_stop(IntPtr k,ulong host,byte[] mode,UIntPtr modeSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_toolbox_load(IntPtr k,byte[] path,UIntPtr pathSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_design_snapshot(IntPtr k,ulong document,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_design_apply(IntPtr k,ulong document,ulong expected,byte[] op,UIntPtr opSize,out Buffer result);
        [DllImport(Dll,CallingConvention=CallingConvention.Cdecl)] static extern int omk_design_export(IntPtr k,ulong document,byte[] language,UIntPtr languageSize,out Buffer result);

        // Strict: a lone surrogate must fail instead of silently becoming U+FFFD and diverging from the editor.
        static readonly UTF8Encoding Utf8=new UTF8Encoding(false,true);
        readonly JavaScriptSerializer json=new JavaScriptSerializer { MaxJsonLength=Int32.MaxValue, RecursionLimit=32 };
        IntPtr handle;
        bool draining;
        internal event Action<KernelEvent> EventRaised;

        internal KernelClient()
        {
            uint version=omk_abi_version();
            if(version!=AbiVersion) throw new InvalidOperationException("OpenMSE.Core.dll 的 ABI 版本为 "+version+"，界面需要 "+AbiVersion+"。");
            int status=omk_create(AbiVersion,out handle);
            if(status!=0 || handle==IntPtr.Zero) throw new InvalidOperationException("无法创建 OpenMSE 内核（状态 "+status+"）。");
        }
        public void Dispose() { if(handle!=IntPtr.Zero) { omk_destroy(handle); handle=IntPtr.Zero; } }

        internal static ulong U64(object value) { return Convert.ToUInt64(value); }
        static byte[] Bytes(string value)
        {
            try { return Utf8.GetBytes(value ?? ""); }
            catch(EncoderFallbackException) { throw new KernelException(2,"invalid_argument","文本包含无法配对的 UTF-16 代理项。"); }
        }
        static UIntPtr Size(byte[] data) { return new UIntPtr((uint)data.Length); }
        IntPtr Handle { get { if(handle==IntPtr.Zero) throw new ObjectDisposedException("KernelClient"); return handle; } }

        delegate int NativeCall(out Buffer buffer);
        Dictionary<string,object> Invoke(NativeCall call,bool drain=true)
        {
            Buffer buffer; int status=call(out buffer); string text;
            try {
                if(buffer.Data==IntPtr.Zero) throw new KernelException(status,"backend_error","内核未返回结果（状态 "+status+"）。");
                int size=checked((int)buffer.Size.ToUInt64()); byte[] data=new byte[size]; Marshal.Copy(buffer.Data,data,0,size); text=Utf8.GetString(data);
            } finally { omk_buffer_free(ref buffer); }
            var message=(Dictionary<string,object>)json.DeserializeObject(text);
            if(drain) DrainEvents();
            if(!(bool)message["ok"]) { var error=(Dictionary<string,object>)message["error"]; throw new KernelException(status,(string)error["code"],(string)error["message"]); }
            return (Dictionary<string,object>)message["result"];
        }

        // Events are delivered on the caller's (UI) thread after each call, in kernel sequence order.
        internal void DrainEvents()
        {
            if(draining) return; draining=true;
            try {
                while(true) {
                    var result=Invoke(delegate(out Buffer b) { return omk_event_poll(Handle,out b); },false);
                    var e=result["event"] as Dictionary<string,object>; if(e==null) return;
                    var handler=EventRaised; if(handler!=null) handler(new KernelEvent { Sequence=U64(e["sequence"]), Topic=(string)e["topic"], Payload=e["payload"] as Dictionary<string,object> });
                }
            } finally { draining=false; }
        }

        internal KernelSnapshot Create(string name,string text) { byte[] n=Bytes(name),t=Bytes(text); return KernelSnapshot.From(Invoke(delegate(out Buffer b) { return omk_document_create(Handle,n,Size(n),t,Size(t),out b); })); }
        internal KernelSnapshot Open(string path) { byte[] p=Bytes(path); return KernelSnapshot.From(Invoke(delegate(out Buffer b) { return omk_document_open(Handle,p,Size(p),out b); })); }
        internal KernelSnapshot Snapshot(ulong id) { return KernelSnapshot.From(Invoke(delegate(out Buffer b) { return omk_document_snapshot(Handle,id,out b); })); }
        internal KernelState Replace(ulong id,ulong expected,int beginLine,int beginColumn,int endLine,int endColumn,string text,bool coalesce)
        {
            byte[] t=Bytes(text);
            return KernelState.From(Invoke(delegate(out Buffer b) { return omk_document_replace(Handle,id,expected,(uint)beginLine,(uint)beginColumn,(uint)endLine,(uint)endColumn,t,Size(t),coalesce ? EditCoalesce : 0,out b); }));
        }
        internal Dictionary<string,object> Save(ulong id,ulong expected,string path) { byte[] p=path==null ? new byte[0] : Bytes(path); return Invoke(delegate(out Buffer b) { return omk_document_save(Handle,id,expected,p,Size(p),out b); }); }
        internal void Close(ulong id,ClosePolicy policy) { Invoke(delegate(out Buffer b) { return omk_document_close(Handle,id,(uint)policy,out b); }); }
        internal KernelCommandState Query(string command,ulong document)
        {
            byte[] c=Bytes(command); var r=Invoke(delegate(out Buffer b) { return omk_command_query(Handle,c,Size(c),document,out b); });
            return new KernelCommandState { Id=(string)r["id"], Enabled=(bool)r["enabled"], UnavailableReason=(string)r["unavailable_reason"] };
        }
        internal Dictionary<string,object> Execute(string command,ulong document,ulong expected)
        {
            byte[] c=Bytes(command); return Invoke(delegate(out Buffer b) { return omk_command_execute(Handle,c,Size(c),document,expected,out b); });
        }

        internal KernelWorkspace OpenWorkspace(string path) { byte[] p=Bytes(path); return KernelWorkspace.From(Invoke(delegate(out Buffer b) { return omk_workspace_open(Handle,p,Size(p),out b); })); }
        internal void CloseWorkspace(ulong id) { Invoke(delegate(out Buffer b) { return omk_workspace_close(Handle,id,out b); }); }
        internal KernelTask StartBuild(ulong workspace,string configuration,string project)
        {
            byte[] c=Bytes(configuration),p=Bytes(project ?? "");
            return KernelTask.From(Invoke(delegate(out Buffer b) { return omk_build_start(Handle,workspace,c,Size(c),p,Size(p),out b); }));
        }
        internal KernelTask StartRun(ulong workspace,string configuration,string project)
        {
            byte[] c=Bytes(configuration),p=Bytes(project ?? "");
            return KernelTask.From(Invoke(delegate(out Buffer b) { return omk_run_start(Handle,workspace,c,Size(c),p,Size(p),out b); }));
        }
        internal KernelTask CancelTask(ulong task) { return KernelTask.From(Invoke(delegate(out Buffer b) { return omk_task_cancel(Handle,task,out b); })); }
        internal KernelTask Task(ulong task) { return KernelTask.From(Invoke(delegate(out Buffer b) { return omk_task_snapshot(Handle,task,out b); })); }

        // ---- debugging (DebugService) and capabilities
        internal KernelDebugSession StartDebugging(ulong workspace,string configuration,string breakpointsJson)
        {
            byte[] c=Bytes(configuration),p=new byte[0],b=Bytes(breakpointsJson);
            return KernelDebugSession.From(Invoke(delegate(out Buffer r) { return omk_debug_start(Handle,workspace,c,Size(c),p,Size(p),b,Size(b),out r); }));
        }
        internal KernelDebugSession DebugSnapshot(ulong session) { return KernelDebugSession.From(Invoke(delegate(out Buffer r) { return omk_debug_snapshot(Handle,session,out r); })); }
        internal KernelDebugSession DebugControl(ulong session,string action) { byte[] a=Bytes(action); return KernelDebugSession.From(Invoke(delegate(out Buffer r) { return omk_debug_control(Handle,session,a,Size(a),out r); })); }
        internal Dictionary<string,object> AddBreakpoint(ulong session,string file,int line) { byte[] f=Bytes(file); return Invoke(delegate(out Buffer r) { return omk_debug_breakpoint_add(Handle,session,f,Size(f),(uint)line,out r); }); }
        internal List<Dictionary<string,object>> Variables(ulong session,int frame)
        {
            var r=Invoke(delegate(out Buffer b) { return omk_debug_variables(Handle,session,(uint)frame,out b); });
            return ((System.Collections.IEnumerable)r["variables"]).Cast<object>().Select(v=>(Dictionary<string,object>)v).ToList();
        }
        internal Dictionary<string,object> Evaluate(ulong session,int frame,string expression) { byte[] e=Bytes(expression); return Invoke(delegate(out Buffer b) { return omk_debug_evaluate(Handle,session,(uint)frame,e,Size(e),out b); }); }
        internal Dictionary<string,string> Capabilities()
        {
            var r=Invoke(delegate(out Buffer b) { return omk_capabilities(Handle,out b); });
            return ((System.Collections.IEnumerable)r["capabilities"]).Cast<object>().Select(c=>(Dictionary<string,object>)c).ToDictionary(c=>(string)c["name"],c=>(string)c["evidence"]);
        }
        // ---- legacy hosts (HostManager)
        internal List<Dictionary<string,object>> LoadHosts(string manifest,string root)
        {
            byte[] m=Bytes(manifest),r=Bytes(root);
            var result=Invoke(delegate(out Buffer b) { return omk_hosts_load(Handle,m,Size(m),r,Size(r),out b); });
            return ((System.Collections.IEnumerable)result["hosts"]).Cast<object>().Select(h=>(Dictionary<string,object>)h).ToList();
        }
        internal Dictionary<string,object> StartHost(string engine,string workspace) { byte[] e=Bytes(engine),w=Bytes(workspace); return Invoke(delegate(out Buffer b) { return omk_host_start(Handle,e,Size(e),w,Size(w),out b); }); }
        internal Dictionary<string,object> SyncToHost(ulong host,ulong document) { return Invoke(delegate(out Buffer b) { return omk_host_sync_document(Handle,host,document,30000,out b); }); }
        internal Dictionary<string,object> VerifyHost(ulong host) { return Invoke(delegate(out Buffer b) { return omk_host_verify(Handle,host,15000,out b); }); }
        internal Dictionary<string,object> StopHost(ulong host,string mode) { byte[] m=Bytes(mode); return Invoke(delegate(out Buffer b) { return omk_host_stop(Handle,host,m,Size(m),out b); }); }

        internal List<ToolboxEntry> LoadToolbox(string path)
        {
            byte[] p=Bytes(path); var r=Invoke(delegate(out Buffer b) { return omk_toolbox_load(Handle,p,Size(p),out b); });
            var entries=new List<ToolboxEntry>();
            foreach(object o in (System.Collections.IEnumerable)r["entries"]) {
                var e=(Dictionary<string,object>)o;
                entries.Add(new ToolboxEntry { Id=(string)e["id"], Name=(string)e["name"], Family=(string)e["family_label"], ControlType=(string)e["control_type"], Markup=e["markup"] as string, Backend=(string)e["backend"] });
            }
            return entries;
        }
        internal KernelDesign Design(ulong document) { return KernelDesign.From(Invoke(delegate(out Buffer b) { return omk_design_snapshot(Handle,document,out b); })); }
        internal KernelDesignResult ApplyDesign(ulong document,ulong expected,Dictionary<string,object> operation)
        {
            byte[] op=Bytes(json.Serialize(operation));
            var r=Invoke(delegate(out Buffer b) { return omk_design_apply(Handle,document,expected,op,Size(op),out b); });
            return new KernelDesignResult { State=KernelState.From(r), Node=r["node"] as string, Design=KernelDesign.From((Dictionary<string,object>)r["design"]) };
        }
        internal string ExportDesign(ulong document,bool vb)
        {
            byte[] l=Bytes(vb ? "vb" : "csharp"); return (string)Invoke(delegate(out Buffer b) { return omk_design_export(Handle,document,l,Size(l),out b); })["source"];
        }
    }

    // One kernel document as seen by one editor view. `Shadow` is the kernel's text in UTF-16/LF form,
    // so view changes can be sent as minimal ranged edits and kernel undo can be patched into the view.
    internal sealed class KernelDocument
    {
        readonly KernelClient kernel;
        internal readonly ulong Id;
        internal ulong Revision { get; private set; }
        internal bool Dirty { get; private set; }
        internal bool CanUndo { get; private set; }
        internal bool CanRedo { get; private set; }
        internal string Name { get; private set; }
        internal string PathName { get; private set; }
        internal string EncodingName { get; private set; }
        internal string Shadow { get; private set; }

        KernelDocument(KernelClient client,KernelSnapshot s) { kernel=client; Id=s.State.Id; Apply(s); }
        internal static KernelDocument Create(KernelClient client,string name,string text) { return new KernelDocument(client,client.Create(name,text)); }
        internal static KernelDocument Open(KernelClient client,string path,out bool alreadyOpen) { var s=client.Open(path); alreadyOpen=s.AlreadyOpen; return new KernelDocument(client,s); }

        internal static string Normalize(string text) { return text.Replace("\r\n","\n").Replace("\r","\n"); }
        void Apply(KernelSnapshot s) { Apply(s.State); Name=s.Name; PathName=s.Path; EncodingName=s.Encoding; Shadow=s.Text; }
        void Apply(KernelState s) { Revision=s.Revision; Dirty=s.Dirty; CanUndo=s.CanUndo; CanRedo=s.CanRedo; }

        internal void Refresh() { Apply(kernel.Snapshot(Id)); }

        static void LineColumn(string text,int index,out int line,out int column)
        {
            line=0; int start=0;
            for(int i=text.IndexOf('\n'); i>=0 && i<index; i=text.IndexOf('\n',i+1)) { line++; start=i+1; }
            column=index-start;
        }
        internal static int IndexOf(string text,int line,int column)
        {
            int start=0;
            for(int l=0; l<line; l++) { int n=text.IndexOf('\n',start); if(n<0) throw new ArgumentOutOfRangeException("line"); start=n+1; }
            return start+column;
        }

        // Sends the difference between the kernel text and the view text as one ranged replace.
        internal void Sync(string viewText,bool coalesce)
        {
            string current=Normalize(viewText), old=Shadow;
            if(current==old) return;
            int prefix=0, max=Math.Min(old.Length,current.Length);
            while(prefix<max && old[prefix]==current[prefix]) prefix++;
            if(prefix>0 && Char.IsHighSurrogate(old[prefix-1])) prefix--;
            int suffix=0;
            while(suffix<max-prefix && old[old.Length-1-suffix]==current[current.Length-1-suffix]) suffix++;
            if(suffix>0 && Char.IsLowSurrogate(old[old.Length-suffix])) suffix--;
            int endOld=old.Length-suffix;
            int bl,bc,el,ec; LineColumn(old,prefix,out bl,out bc); LineColumn(old,endOld,out el,out ec);
            var state=kernel.Replace(Id,Revision,bl,bc,el,ec,current.Substring(prefix,current.Length-suffix-prefix),coalesce);
            Shadow=current; Apply(state);
        }

        // Runs kernel undo/redo; returns the change as (index, length, text) against the pre-call view text.
        internal bool Undo(out int index,out int length,out string text) { return History(true,out index,out length,out text); }
        internal bool Redo(out int index,out int length,out string text) { return History(false,out index,out length,out text); }
        bool History(bool undo,out int index,out int length,out string text)
        {
            index=length=0; text=null;
            if(undo ? !CanUndo : !CanRedo) return false;
            var state=KernelState.From(kernel.Execute(undo ? "edit.undo" : "edit.redo",Id,Revision));
            Patch(state,out index,out length,out text);
            return true;
        }
        // Applies a kernel edit (pre-change coordinates) to the shadow; reports it in UTF-16 indexes for the view.
        void Patch(KernelState state,out int index,out int length,out string text)
        {
            var e=state.Edit; index=IndexOf(Shadow,e.BeginLine,e.BeginColumn); length=IndexOf(Shadow,e.EndLine,e.EndColumn)-index; text=e.Text;
            Shadow=Shadow.Substring(0,index)+text+Shadow.Substring(index+length); Apply(state);
        }

        internal KernelDesign Design() { return kernel.Design(Id); }
        // Design operations are committed by the kernel as one source patch; returns false when nothing changed.
        internal KernelDesignResult ApplyDesign(Dictionary<string,object> operation,ulong expectedRevision,out bool changed,out int index,out int length,out string text)
        {
            var result=kernel.ApplyDesign(Id,expectedRevision,operation);
            changed=result.State.Edit!=null; index=length=0; text=null;
            if(changed) Patch(result.State,out index,out length,out text); else Apply(result.State);
            return result;
        }
        internal string ExportDesign(bool vb) { return kernel.ExportDesign(Id,vb); }

        internal void Save(string path)
        {
            var r=path==null ? kernel.Execute("file.save",Id,Revision) : kernel.Save(Id,Revision,path);
            Revision=KernelClient.U64(r["revision"]); Dirty=(bool)r["dirty"]; CanUndo=(bool)r["can_undo"]; CanRedo=(bool)r["can_redo"];
            PathName=(string)r["path"]; Name=(string)r["name"]; EncodingName=(string)r["encoding"];
        }
        internal void Close(ClosePolicy policy) { kernel.Close(Id,policy); }
    }
}
