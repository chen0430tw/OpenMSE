using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    // Debugging (kernel DebugService) and the legacy back-end host (kernel HostManager) in the unified window.
    // The kernel owns session state; this part only presents it and forwards commands.
    internal sealed partial class MainForm
    {
        readonly ListView stack=new ListView { Dock=DockStyle.Fill, View=View.Details, FullRowSelect=true, BorderStyle=BorderStyle.None, HideSelection=false };
        readonly ListView locals=new ListView { Dock=DockStyle.Fill, View=View.Details, FullRowSelect=true, BorderStyle=BorderStyle.None };
        readonly Dictionary<string,SortedSet<int>> breakpoints=new Dictionary<string,SortedSet<int>>(StringComparer.OrdinalIgnoreCase);
        readonly ToolStripStatusLabel hostStatus=new ToolStripStatusLabel("VS2005 后端：未启动") { ForeColor=Color.White };
        ToolStripMenuItem startDebugItem, stepOverItem, stepIntoItem, stepOutItem, pauseItem, stopDebugItem, hostStartItem, hostSyncItem, hostVerifyItem, hostStopItem;
        internal KernelDebugSession DebugSession;
        internal int SelectedFrame;
        bool debugAfterBuild, debugAvailable;
        ulong? host;
        string hostState="", hostUnavailable;
        EditorPage currentLinePage;
        int currentLine=-1;

        internal IList<KernelFrame> DebugFrames { get { return DebugSession==null ? new List<KernelFrame>() : DebugSession.Frames; } }
        internal IList<string[]> Locals { get { return locals.Items.Cast<ListViewItem>().Select(i=>new[]{ i.Text, i.SubItems[1].Text, i.SubItems[2].Text }).ToList(); } }

        void BuildDebugUi(MenuStrip menu,ToolStripMenuItem tools,StatusStrip bar)
        {
            var debug=new ToolStripMenuItem("调试(&D)"); menu.Items.Insert(menu.Items.IndexOf(tools),debug);
            startDebugItem=Item(debug,"开始调试 / 继续",Keys.F5,StartOrContinue);
            pauseItem=Item(debug,"全部中断",Keys.Control|Keys.Alt|Keys.Pause,delegate { DebugCommand("pause"); });
            stopDebugItem=Item(debug,"停止调试",Keys.Shift|Keys.F5,delegate { DebugCommand("stop"); });
            debug.DropDownItems.Add(new ToolStripSeparator());
            stepOverItem=Item(debug,"逐过程",Keys.F10,delegate { DebugCommand("step_over"); });
            stepIntoItem=Item(debug,"逐语句",Keys.F11,delegate { DebugCommand("step_into"); });
            stepOutItem=Item(debug,"跳出",Keys.Shift|Keys.F11,delegate { DebugCommand("step_out"); });
            debug.DropDownItems.Add(new ToolStripSeparator());
            Item(debug,"切换断点",Keys.F9,ToggleBreakpoint);
            tools.DropDownItems.Add(new ToolStripSeparator());
            hostStartItem=Item(tools,"启动 VS2005 后端",Keys.None,StartHost);
            hostSyncItem=Item(tools,"把当前文档同步到 VS2005 后端",Keys.None,SyncToHost);
            hostVerifyItem=Item(tools,"核对 VS2005 后端",Keys.None,delegate { if(host!=null) Kernel.VerifyHost(host.Value); });
            hostStopItem=Item(tools,"关闭 VS2005 后端",Keys.None,delegate { if(host!=null) Kernel.StopHost(host.Value,"shutdown"); });
            bar.Items.Insert(1,hostStatus);
            stack.Columns.Add("函数",320); stack.Columns.Add("文件",260); stack.Columns.Add("行",60);
            stack.DoubleClick+=delegate { if(stack.SelectedIndices.Count>0) Guard(delegate { SelectFrame(stack.SelectedIndices[0]); }); };
            locals.Columns.Add("名称",160); locals.Columns.Add("值",360); locals.Columns.Add("类型",120);
            var stackTab=new TabPage("调用堆栈"); stackTab.Controls.Add(stack); var localsTab=new TabPage("局部变量"); localsTab.Controls.Add(locals);
            outputTabs.TabPages.Add(stackTab); outputTabs.TabPages.Add(localsTab);
            // DebugSession commands are offered only where the kernel reports verified behaviour.
            try { string evidence; debugAvailable=Kernel.Capabilities().TryGetValue("debug.clr2",out evidence) && evidence=="behavior_verified"; } catch(Exception) { debugAvailable=false; }
            try {
                var vs8=Kernel.LoadHosts(Path.Combine(root,"config","hosts.json"),root).FirstOrDefault(h=>(string)h["engine"]=="vs8");
                hostUnavailable=vs8==null ? "宿主清单中没有 VS2005。" : (bool)vs8["verified"] ? null : (string)vs8["verify_error"];
            } catch(Exception ex) { hostUnavailable=ex.Message; }
            if(hostUnavailable!=null) { hostStatus.Text="VS2005 后端：不可用"; hostStatus.ToolTipText=hostUnavailable; }
        }

        void OnDebugOrHostEvent(KernelEvent e)
        {
            if(e.Topic.StartsWith("host.")) { OnHostEvent(e); return; }
            object id; ulong session=e.Payload.TryGetValue("session",out id) ? KernelClient.U64(id) : e.Payload.TryGetValue("id",out id) ? KernelClient.U64(id) : 0;
            if(DebugSession==null || session!=DebugSession.Id) return;
            var body=e.Payload.ContainsKey("body") ? e.Payload["body"] as Dictionary<string,object> : null;
            if(e.Topic=="debug.output" && body!=null) { output.AppendText((string)body["text"]+"\r\n"); return; }
            if(e.Topic=="debug.state" || e.Topic=="debug.stopped" || e.Topic=="debug.exited") {
                DebugSession=Kernel.DebugSnapshot(DebugSession.Id);
                if(DebugSession.Stopped && e.Topic=="debug.stopped") ShowStop();
                if(DebugSession.State=="exited" || DebugSession.State=="failed") {
                    ClearCurrentLine(); stack.Items.Clear(); locals.Items.Clear();
                    output.AppendText(DebugSession.State=="exited" ? "调试结束："+(DebugSession.Reason=="terminated" ? "已终止" : "程序退出码 "+DebugSession.ExitCode)+"\r\n" : "调试失败："+DebugSession.LastError+"\r\n");
                    status.Text=DebugSession.State=="exited" ? "调试已结束" : "调试失败";
                }
                if(DebugSession.State=="running") { ClearCurrentLine(); status.Text="正在运行（调试）…"; }
                UpdateState();
            }
        }

        void OnHostEvent(KernelEvent e)
        {
            if(e.Topic=="host.state") {
                var id=KernelClient.U64(e.Payload["id"]); if(host==null || id!=host.Value) return;
                hostState=(string)e.Payload["state"]; string error=(string)e.Payload["last_error"];
                hostStatus.Text="VS2005 后端："+hostState+(hostState=="ready" ? "（PID "+e.Payload["pid"]+"）" : ""); hostStatus.ToolTipText=error;
                if(hostState=="exited" || hostState=="failed" || hostState=="stopped") { if(hostState=="failed") status.Text="VS2005 后端启动失败："+error; host=null; }
                UpdateState();
            } else if(e.Topic=="host.response") {
                var request=(Dictionary<string,object>)e.Payload["request"];
                string method=(string)request["method"], outcome=(string)request["outcome"];
                var error=request["error"] as Dictionary<string,object>;
                if(method=="documents.sync") status.Text=outcome=="ok" ? "已同步到 VS2005 后端" : "同步未完成（"+outcome+(error!=null ? "："+error["message"] : "")+"）";
                else if(method=="host.shutdown" && outcome!="ok") status.Text="VS2005 后端拒绝关闭："+(error!=null ? error["message"] : outcome);
            }
        }

        static int LineOfIndex(string text,int index) { int line=0; for(int i=text.IndexOf('\n'); i>=0 && i<index; i=text.IndexOf('\n',i+1)) line++; return line; }
        static int LineStart(string text,int line) { int start=0; for(int l=0; l<line; l++) { int n=text.IndexOf('\n',start); if(n<0) return text.Length; start=n+1; } return start; }

        // Breakpoints live in the window (per file, one-based lines) and are handed to the kernel at launch,
        // or added to a running session immediately.
        internal void ToggleBreakpoint()
        {
            var page=Active; if(page==null) return;
            if(page.Document.PathName==null) throw new InvalidOperationException("请先保存文档，再设置断点。");
            int line=LineOfIndex(page.Document.Shadow,page.Editor.SelectionStart)+1;
            SetBreakpoint(page.Document.PathName,line,!HasBreakpoint(page.Document.PathName,line));
        }
        internal bool HasBreakpoint(string file,int line) { SortedSet<int> lines; return breakpoints.TryGetValue(file,out lines) && lines.Contains(line); }
        internal void SetBreakpoint(string file,int line,bool on)
        {
            SortedSet<int> lines; if(!breakpoints.TryGetValue(file,out lines)) breakpoints[file]=lines=new SortedSet<int>();
            if(on) {
                lines.Add(line);
                if(DebugSession!=null && DebugSession.Active) { var r=Kernel.AddBreakpoint(DebugSession.Id,file,line); status.Text=(bool)r["verified"] ? "断点已绑定到第 "+r["bound_line"]+" 行" : "断点暂未绑定（模块尚未加载）"; }
            } else {
                lines.Remove(line);
                if(DebugSession!=null && DebugSession.Active) status.Text="断点已移除；当前调试会话中将在下次启动时生效。";
            }
            var page=Pages.FirstOrDefault(p=>String.Equals(p.Document.PathName,file,StringComparison.OrdinalIgnoreCase)); if(page!=null) Repaint(page);
        }

        internal void StartOrContinue()
        {
            if(DebugSession!=null && DebugSession.Stopped) { DebugCommand("continue"); return; }
            if(DebugSession!=null && DebugSession.Active) return;
            if(Workspace==null) throw new InvalidOperationException("请先打开项目或解决方案。");
            if(!debugAvailable) throw new InvalidOperationException("托管调试器不可用（缺少 OpenMSE.DebugHost）。");
            debugAfterBuild=true; try { StartBuild(); } finally { if(!BuildRunning) debugAfterBuild=false; }
        }
        void BeginDebugSession()
        {
            var list=breakpoints.SelectMany(b=>b.Value.Select(l=>new Dictionary<string,object>{ {"file",b.Key}, {"line",l} })).ToList();
            output.AppendText("\r\n开始调试 ["+Configuration+"]\r\n");
            DebugSession=Kernel.StartDebugging(Workspace.Id,Configuration,new JavaScriptSerializer().Serialize(list));
            output.AppendText("调试适配器："+DebugSession.Adapter+"，程序："+DebugSession.Program+"\r\n"); status.Text="正在调试…"; UpdateState();
        }
        // Each command is checked by the kernel's state machine (for example, stepping requires a stopped session).
        internal void DebugCommand(string action)
        {
            if(DebugSession==null) return;
            DebugSession=Kernel.DebugControl(DebugSession.Id,action);
            if(action=="stop") status.Text="正在停止调试…";
            UpdateState();
        }

        void ShowStop()
        {
            stack.BeginUpdate(); stack.Items.Clear();
            foreach(var f in DebugSession.Frames) stack.Items.Add(new ListViewItem(new[]{ f.Function, f.File==null ? "" : Path.GetFileName(f.File), f.Line>0 ? f.Line.ToString() : "" }) { Tag=f, ForeColor=f.File==null ? Color.Gray : stack.ForeColor });
            stack.EndUpdate();
            string reason=DebugSession.Reason=="breakpoint" ? "命中断点" : DebugSession.Reason=="step" ? "单步完成" : DebugSession.Reason=="pause" ? "已中断" : DebugSession.Reason=="exception" ? "未处理的异常" : DebugSession.Reason;
            status.Text="调试已暂停："+reason;
            horizontal.Panel2Collapsed=false; outputTabs.SelectedIndex=2;
            int first=DebugSession.Frames.FindIndex(f=>f.File!=null);
            SelectFrame(first<0 ? 0 : first);
        }
        internal void SelectFrame(int index)
        {
            if(DebugSession==null || !DebugSession.Stopped || index>=DebugSession.Frames.Count) return;
            SelectedFrame=index; var f=DebugSession.Frames[index];
            if(f.File!=null && File.Exists(f.File)) {
                OpenFile(f.File); var page=Active;
                if(page.Views!=null) page.Views.SelectedIndex=1;
                ClearCurrentLine(); currentLinePage=page; currentLine=f.Line; Repaint(page);
                page.Editor.Select(LineStart(page.Document.Shadow,f.Line-1),0); page.Editor.ScrollToCaret();
            }
            locals.BeginUpdate(); locals.Items.Clear();
            try { foreach(var v in Kernel.Variables(DebugSession.Id,index)) locals.Items.Add(new ListViewItem(new[]{ (string)v["name"], (string)v["value"], (string)v["type"] })); }
            catch(KernelException ex) { locals.Items.Add(new ListViewItem(new[]{ "（不可用）", ex.Message, "" })); }
            locals.EndUpdate();
        }
        internal string Evaluate(string expression) { var r=Kernel.Evaluate(DebugSession.Id,SelectedFrame,expression); return (string)r["value"]; }

        void ClearCurrentLine() { var page=currentLinePage; currentLinePage=null; currentLine=-1; if(page!=null && !page.IsDisposed) Repaint(page); }
        // Breakpoint lines are tinted red and the current execution line yellow; formatting never changes the text.
        void Repaint(EditorPage page)
        {
            var editor=page.Editor; string text=page.Document.Shadow; if(text.Length>2*1024*1024) return;
            int start=editor.SelectionStart, length=editor.SelectionLength;
            editor.SuspendLayout();
            try {
                editor.SelectAll(); editor.SelectionBackColor=editor.BackColor;
                SortedSet<int> lines;
                if(page.Document.PathName!=null && breakpoints.TryGetValue(page.Document.PathName,out lines)) foreach(int l in lines) Mark(editor,text,l,Color.FromArgb(250,214,214));
                if(page==currentLinePage && currentLine>0) Mark(editor,text,currentLine,Color.FromArgb(255,238,140));
                editor.Select(start,length);
            } finally { editor.ResumeLayout(); }
        }
        static void Mark(RichTextBox editor,string text,int line,Color color)
        {
            int s=LineStart(text,line-1); if(s>=text.Length && line>1) return;
            int e=text.IndexOf('\n',s); if(e<0) e=text.Length;
            editor.Select(s,Math.Max(1,e-s)); editor.SelectionBackColor=color;
        }

        void StartHost()
        {
            if(hostUnavailable!=null) throw new InvalidOperationException("VS2005 后端不可用："+hostUnavailable);
            if(host!=null) throw new InvalidOperationException("VS2005 后端已在运行或正在启动。");
            string dir=Workspace!=null ? Workspace.Root : workspace; Directory.CreateDirectory(dir);
            var h=Kernel.StartHost("vs8",dir); host=KernelClient.U64(h["id"]); hostState=(string)h["state"];
            hostStatus.Text="VS2005 后端：正在启动…"; UpdateState();
        }
        void SyncToHost()
        {
            if(host==null || Active==null) return;
            Kernel.SyncToHost(host.Value,Active.Document.Id); status.Text="正在同步到 VS2005 后端…";
        }

        void UpdateDebugState()
        {
            if(startDebugItem==null) return;
            bool active=DebugSession!=null && DebugSession.Active, stopped=DebugSession!=null && DebugSession.Stopped;
            startDebugItem.Enabled=debugAvailable && (stopped || (!active && Workspace!=null && !BuildRunning));
            startDebugItem.Text=stopped ? "继续" : "开始调试";
            stepOverItem.Enabled=stepIntoItem.Enabled=stepOutItem.Enabled=stopped;
            pauseItem.Enabled=DebugSession!=null && DebugSession.State=="running";
            stopDebugItem.Enabled=active;
            hostStartItem.Enabled=hostUnavailable==null && host==null;
            hostSyncItem.Enabled=host!=null && hostState=="ready" && Active!=null && Active.Document.PathName!=null;
            hostVerifyItem.Enabled=host!=null && hostState=="needs_verification";
            hostStopItem.Enabled=host!=null && hostState=="ready";
        }
    }
}
