using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal static class GuiSmoke
    {
        static void Check(bool condition,string name) { if(!condition) throw new Exception("GUI smoke failed: "+name); }
        static void Pump(Func<bool> done,int seconds) { var t=Stopwatch.StartNew(); while(!done() && t.Elapsed.TotalSeconds<seconds) { Application.DoEvents(); Thread.Sleep(30); } }
        [System.Runtime.InteropServices.DllImport("user32.dll")] static extern IntPtr SendMessage(IntPtr window,int message,IntPtr w,IntPtr l);
        [System.Runtime.InteropServices.DllImport("user32.dll")] static extern IntPtr WindowFromPoint(Point p);
        [System.Runtime.InteropServices.DllImport("user32.dll",CharSet=System.Runtime.InteropServices.CharSet.Unicode)] static extern int GetClassName(IntPtr h,StringBuilder s,int n);
        [System.Runtime.InteropServices.DllImport("user32.dll")] static extern uint GetWindowThreadProcessId(IntPtr h,out uint pid);
        // What is really on screen at a control's click point (WinForms only raises DoubleClick if it is the control).
        static string Probe(Control c) { var at=c.PointToScreen(new Point(8,8)); var h=WindowFromPoint(at); var cls=new StringBuilder(128); GetClassName(h,cls,128); uint pid; GetWindowThreadProcessId(h,out pid); return " [point "+at+" -> "+(h==c.Handle ? "the control" : cls+" pid "+pid+(pid==(uint)System.Diagnostics.Process.GetCurrentProcess().Id ? " (this process)" : ""))+"]"; }
        static IntPtr At(Point p) { return new IntPtr((p.Y<<16)|(p.X&0xFFFF)); }
        // Real window messages, as a user's double-click produces them, including the zero-distance mouse move.
        static void DoubleClick(Control c)
        {
            // WinForms raises DoubleClick only when the point really is this control on screen.
            var scroll=c.Parent as Panel; bool canvas=scroll!=null && scroll.AutoScroll; if(!canvas) scroll=(Panel)c.Parent.Parent; if(canvas) scroll.AutoScrollPosition=Point.Empty; else scroll.ScrollControlIntoView(c);
            Application.DoEvents();
            IntPtr at=At(new Point(8,8)), down=new IntPtr(1);
            SendMessage(c.Handle,0x201,down,at); SendMessage(c.Handle,0x200,down,at); SendMessage(c.Handle,0x202,IntPtr.Zero,at);
            if(c.IsDisposed) return;
            SendMessage(c.Handle,0x203,down,at); if(!c.IsDisposed) SendMessage(c.Handle,0x202,IntPtr.Zero,at);
            Application.DoEvents();
        }
        static Control Slot(EditorPage page,DesignItem item) { return page.Designer.Canvas.Controls.Cast<Control>().First(c=>c.Left==item.X && c.Top==item.Y && c.Width==item.Width); }
        static string CodeSmoke(MainForm form,string directory)
        {
            form.TopMost=true; var visual=form.Tabs.SelectedTab;
            foreach(bool vb in new[]{true,false}) {
                string name=vb ? "VisualBasicSmoke" : "CSharpSmoke", folder=Path.Combine(directory,name), ext=vb ? ".vb" : ".cs", code=Path.Combine(folder,"Form1"+ext);
                form.OpenProject(Path.Combine(folder,name+".sln"));
                var page=form.NewDocument(Template.All.Last()); form.SaveTo(page,Path.Combine(folder,"Form1.omform"));
                page.Designer.Select(null); var size=(FormProperties)form.SelectedDesignerObject; size.Width=800; size.Height=1200; page.Designer.CommitSelected();
                foreach(var entry in form.ToolboxEntries.Where(t=>t.Family!="HTML")) page.Designer.Insert(entry,new Point(20+(page.Designer.Items.Count%3)*250,20+(page.Designer.Items.Count/3)*130));
                int count=page.Designer.Items.Count;
                for(int i=0;i<count;i++) {
                    form.Tabs.SelectedTab=page; Application.DoEvents(); var item=page.Designer.Items[i]; int x=item.X,y=item.Y;
                    var probeTarget=Slot(page,item); DoubleClick(probeTarget); string probe=probeTarget.IsDisposed ? "" : Probe(probeTarget);
                    Check(form.Active.Document.PathName!=null && form.Active.Document.PathName.Equals(code,StringComparison.OrdinalIgnoreCase),name+" double-click opens code for "+item.Kind+probe);
                    item=page.Designer.Items[i]; Check(item.X==x && item.Y==y && item.Handler==item.Name+"_"+item.Event,name+" handler bound without moving "+item.Kind);
                    var codePage=form.Active; string inside=codePage.Document.Shadow.Substring(0,codePage.Editor.SelectionStart);
                    var header=System.Text.RegularExpressions.Regex.Matches(inside,vb ? @"Sub (\w+)\(" : @"void (\w+)\(");
                    Check(header.Count>0 && header[header.Count-1].Groups[1].Value==item.Handler && inside.TrimEnd().EndsWith(vb ? ")" : "{"),name+" caret inside "+item.Handler+" for "+item.Kind);
                }
                form.Tabs.SelectedTab=page; DoubleClick(page.Designer.Canvas);
                Check(form.Active.Document.Shadow.Contains("Form1_Load"),name+" double-click on the form opens Load");
                // Object and event lists above the code window, as in the VB editor.
                var codeWindow=form.Active; Check(codeWindow.CodeBar!=null && form.EventNames(codeWindow,"button1",false).Contains("MouseMove") && form.EventNames(codeWindow,"button1",true).SequenceEqual(new[]{"Click"}),name+" event list with bound events");
                form.ChooseEvent(codeWindow,"button1","MouseMove");
                Check(form.Active==codeWindow && codeWindow.Document.Shadow.Contains(vb ? "Sub button1_MouseMove(ByVal sender As Object, ByVal e As System.Windows.Forms.MouseEventArgs)" : "void button1_MouseMove(object sender, System.Windows.Forms.MouseEventArgs e)")
                    && form.EventNames(codeWindow,"button1",true).SequenceEqual(new[]{"Click","MouseMove"}) && codeWindow.CodeBar.Events.Text=="MouseMove" && ((Control)codeWindow.CodeBar.Objects).Text.StartsWith("button1"),name+" choosing an event creates and shows its procedure");
                form.ChooseEvent(codeWindow,"","Resize"); Check(codeWindow.Document.Shadow.Contains("Form1_Resize"),name+" form events from the list");
                codeWindow.Editor.Select(codeWindow.Document.Shadow.IndexOf("button1_Click")+20,0); Check(codeWindow.CodeBar.Events.Text=="Click" && codeWindow.CodeBar.Objects.Text.StartsWith("button1"),name+" lists follow the caret");
                string once=form.Active.Document.Shadow; form.Tabs.SelectedTab=page; DoubleClick(Slot(page,page.Designer.Items[0]));
                Check(form.Active.Document.Shadow==once,name+" second double-click only navigates");
                // Drag and resize gestures, as window messages.
                form.Tabs.SelectedTab=page; var first=page.Designer.Items[0]; int fx=first.X,fy=first.Y,fw=first.Width; var slot=Slot(page,first); ((ScrollableControl)page.Designer.Canvas.Parent).ScrollControlIntoView(slot);
                SendMessage(slot.Handle,0x201,new IntPtr(1),At(new Point(8,8))); SendMessage(slot.Handle,0x200,new IntPtr(1),At(new Point(48,28))); SendMessage(slot.Handle,0x202,IntPtr.Zero,At(new Point(8,8))); Application.DoEvents();
                first=page.Designer.Items[0]; Check(first.X==fx+40 && first.Y==fy+20,name+" drag moves the control: "+fx+","+fy+" -> "+first.X+","+first.Y+" size "+first.Width+"x"+first.Height+" drag "+SystemInformation.DragSize);
                slot=Slot(page,first); var corner=new Point(slot.Width-4,slot.Height-4);
                SendMessage(slot.Handle,0x201,new IntPtr(1),At(corner)); SendMessage(slot.Handle,0x200,new IntPtr(1),At(new Point(corner.X+30,corner.Y))); SendMessage(slot.Handle,0x202,IntPtr.Zero,At(corner)); Application.DoEvents();
                Check(page.Designer.Items[0].Width==fw+30,name+" corner drag resizes the control");
                page.Undo(); page.Undo(); Check(page.Designer.Items[0].X==fx && page.Designer.Items[0].Width==fw,name+" gestures undo one step each");
                if(vb) { form.Refresh(); Application.DoEvents(); Thread.Sleep(300); using(var shot=new Bitmap(form.Width,form.Height)) { using(var g=Graphics.FromImage(shot)) g.CopyFromScreen(form.Location,Point.Empty,form.Size); shot.Save(Path.Combine(directory,"designer.png"),System.Drawing.Imaging.ImageFormat.Png); }
                    form.Tabs.SelectedTab=form.Tabs.TabPages.OfType<EditorPage>().First(p=>code.Equals(p.Document.PathName,StringComparison.OrdinalIgnoreCase)); form.Refresh(); Application.DoEvents(); Thread.Sleep(300); using(var shot=new Bitmap(form.Width,form.Height)) { using(var g=Graphics.FromImage(shot)) g.CopyFromScreen(form.Location,Point.Empty,form.Size); shot.Save(Path.Combine(directory,"code.png"),System.Drawing.Imaging.ImageFormat.Png); } }
                form.StartBuild(); Pump(()=>!form.BuildRunning,60);
                Check(form.LastBuildExitCode==0,name+" form with 25 handlers builds in its project: "+form.OutputText.Substring(Math.Max(0,form.OutputText.Length-1500)));
                Check(form.Workspace.Projects[0].Files.Count(f=>Path.GetFileName(f).StartsWith("Form1"))==3,name+" form files in project graph");
                var assembly=System.Reflection.Assembly.Load(File.ReadAllBytes(form.LastTask.Artifacts[0])); var type=assembly.GetTypes().First(t=>t.Name=="Form1");
                using(var built=(Form)Activator.CreateInstance(type)) Check(built.Controls.Count==22,name+" built form constructs");
            }
            // A layout outside any project asks for the language.
            form.OpenProject(Path.Combine(directory,"CSharpSmoke","CSharpSmoke.sln")); string asked=null; form.LanguageChooser=delegate(string f) { asked=f; return true; };
            var loose=form.NewDocument(Template.All.Last()); Directory.CreateDirectory(Path.Combine(directory,"loose")); form.SaveTo(loose,Path.Combine(directory,"loose","窗体一.omform"));
            loose.Designer.Insert(form.ToolboxEntries.First(t=>t.Family!="HTML"),new Point(40,40)); form.Tabs.SelectedTab=loose; DoubleClick(Slot(loose,loose.Designer.Items[0]));
            Check(asked=="窗体一.omform（不在已打开的项目中）" && form.Active.Document.PathName.EndsWith("窗体一.vb") && form.Active.Document.Shadow.Contains("Partial Public Class 窗体一"),"language is asked for a layout outside projects; Unicode class name");
            // Unsaved form: double-click works at once (no save dialog); the code is saved and renamed with the form.
            // This one uses the real language dialog: it is captured, then answered with its own Visual Basic button.
            form.OpenProject(Path.Combine(directory,"VisualBasicSmoke","VisualBasicSmoke.sln")); form.LanguageChooser=null; string dialogText=null;
            var fresh=form.NewDocument(Template.All.Last()); fresh.Designer.Insert(form.ToolboxEntries.First(t=>t.Family!="HTML"),new Point(40,40));
            var watcher=new System.Windows.Forms.Timer { Interval=100 };
            watcher.Tick+=delegate {
                var dialog=Application.OpenForms.Cast<Form>().FirstOrDefault(f=>f.Text.StartsWith("窗体代码语言")); if(dialog==null) return;
                watcher.Stop(); dialogText=String.Join(" | ",dialog.Controls.Cast<Control>().Select(c=>c.Text).ToArray());
                using(var shot=new Bitmap(dialog.Width,dialog.Height)) { dialog.DrawToBitmap(shot,new Rectangle(Point.Empty,dialog.Size)); shot.Save(Path.Combine(directory,"language-dialog.png"),System.Drawing.Imaging.ImageFormat.Png); }
                dialog.Controls.OfType<Button>().First(b=>b.Text=="Visual Basic").PerformClick();
            };
            watcher.Start();
            form.Tabs.SelectedTab=fresh; DoubleClick(Slot(fresh,fresh.Designer.Items[0])); watcher.Dispose();
            var unsaved=form.Active;
            Check(dialogText!=null && dialogText.Contains(fresh.UntitledName) && fresh.Document.PathName==null && unsaved.Document.PathName==null && unsaved.UntitledName.EndsWith(".vb") && unsaved.Document.Shadow.Contains("Sub button1_Click") && unsaved.CodeBar!=null,"unsaved form: language asked, code opened without saving");
            form.ChooseEvent(unsaved,"button1","MouseDown"); Check(unsaved.Document.Shadow.Contains("button1_MouseDown"),"event lists work before the form is saved");
            string oldClass=System.IO.Path.GetFileNameWithoutExtension(fresh.UntitledName);
            form.SaveTo(fresh,Path.Combine(directory,"VisualBasicSmoke","Form2.omform"));
            Check(unsaved.Document.PathName!=null && unsaved.Document.PathName.EndsWith("Form2.vb") && unsaved.Document.Shadow.Contains("Partial Public Class Form2") && !unsaved.Document.Shadow.Contains(oldClass)
                && File.Exists(Path.Combine(directory,"VisualBasicSmoke","Form2.Designer.vb")) && form.Workspace.Projects[0].Files.Any(f=>f.EndsWith("Form2.vb")),"saving the form saves, renames and adds its code");
            form.StartBuild(); Pump(()=>!form.BuildRunning,60); Check(form.LastBuildExitCode==0,"project with the second form builds");
            var html=form.NewDocument(Template.All[1]); html.Designer.Insert(form.ToolboxEntries.First(t=>t.Family=="HTML" && t.Name=="Button"),new Point(100,100));
            form.Tabs.SelectedTab=html; DoubleClick(Slot(html,html.Designer.Items.Last()));
            Check(html.Views.SelectedIndex==1 && html.Document.Shadow.Contains("onclick=\"control1_onclick()\"") && html.Document.Shadow.Substring(0,html.Editor.SelectionStart).TrimEnd().EndsWith("function control1_onclick() {"),"HTML double-click opens its function");
            form.SaveTo(html,Path.Combine(directory,"code.html"));
            form.Tabs.SelectedTab=html; Application.DoEvents(); var close=form.CloseRect(form.Tabs.TabPages.IndexOf(html));
            SendMessage(form.Tabs.Handle,0x201,new IntPtr(1),At(new Point(close.X+8,close.Y+8))); SendMessage(form.Tabs.Handle,0x202,IntPtr.Zero,At(new Point(close.X+8,close.Y+8))); Application.DoEvents();
            Check(!form.Tabs.TabPages.Contains(html),"tab close button");
            form.Tabs.SelectedTab=visual;
            return "PASS double-click to code: 22 controls + form in VB and C# (bound, not moved, built, constructed), drag/resize, object/event lists, language choice, unsaved form code, HTML handler, tab close button\r\n";
        }
        internal static int Run(string root,string directory)
        {
            Directory.CreateDirectory(directory);
            string report="";
            using(var form=new MainForm(root)) {
                form.Show(); Application.DoEvents();
                using(var dialog=new NewItemDialog(directory)) { dialog.Show(form); Application.DoEvents(); Check(dialog.Visible,"new item dialog visible"); dialog.Close(); }
                var html=form.NewDocument(Template.All[1]); html.Editor.AppendText("\r\n<!-- 中文保存验证 -->\r\n");
                string htmlPath=Path.Combine(directory,"中文.html"); form.SaveTo(html,htmlPath);
                Check(!html.Dirty,"saved editor clean"); form.OpenFile(Path.Combine(directory,"中文.HTML")); Check(form.Active==html,"open document deduplication (kernel path identity)");
                Check(File.ReadAllText(htmlPath,Encoding.UTF8).Contains("中文保存验证"),"unicode disk roundtrip");
                // Exercise disk conflict protection, then restore the externally changed text in the test-owned file.
                string saved=File.ReadAllText(htmlPath,Encoding.UTF8); File.AppendAllText(htmlPath,"external",Encoding.UTF8);
                html.Editor.AppendText("must not overwrite");
                bool conflict=false; try { form.SaveTo(html,null); } catch(KernelException ex) { conflict=ex.Code=="conflict"; }
                Check(conflict && File.ReadAllText(htmlPath,Encoding.UTF8).EndsWith("external"),"external edits preserved");
                File.WriteAllText(htmlPath,saved,new UTF8Encoding(true));
                html.Undo(); Check(!html.Dirty && !html.Editor.Text.Contains("must not overwrite"),"undo back to the saved state is clean");
                report+="PASS shared new-item dialog, document tabs, Unicode save, disk conflict guard\r\n";
                string lf=Path.Combine(directory,"lf.txt"); File.WriteAllText(lf,"a\n中文\n",new UTF8Encoding(false));
                form.OpenFile(lf); var lfPage=form.Active; lfPage.Editor.AppendText("b\n"); form.SaveTo(lfPage,null); byte[] bytes=File.ReadAllBytes(lf);
                Check(bytes[0]!=239 && File.ReadAllText(lf,Encoding.UTF8)=="a\n中文\nb\n","UTF-8 no BOM and LF preserved");
                report+="PASS UTF-8 without BOM and LF preservation\r\n";
                // Kernel-backed editing: the view sends ranged edits; the kernel owns revisions, dirty state and undo.
                var typed=form.NewDocument(Template.All[0]); Check(!typed.Dirty,"empty untitled document clean");
                ulong first=typed.Document.Revision;
                foreach(string piece in new[] { "中","文","\U0001F600","a","b" }) { typed.Editor.Select(typed.Editor.TextLength,0); typed.Editor.SelectedText=piece; }
                Check(typed.Editor.Text=="中文\U0001F600ab" && typed.Document.Revision==first+5,"each view edit is one kernel revision");
                var snap=form.Kernel.Snapshot(typed.Document.Id);
                Check(snap.Text==typed.Editor.Text && snap.State.Dirty==typed.Dirty && snap.State.Revision==typed.Document.Revision,"view and kernel agree on text, revision and dirty state");
                typed.Undo(); Check(typed.Editor.Text=="" && !typed.Dirty,"typing undone as one step and clean again");
                typed.Redo(); Check(typed.Editor.Text=="中文\U0001F600ab" && typed.Dirty,"redo typing");
                typed.Editor.Select(2,2); typed.Editor.SelectedText="\n";
                Check(form.Kernel.Snapshot(typed.Document.Id).Text=="中文\nab","range edit replacing a surrogate pair");
                typed.Undo(); Check(typed.Editor.Text=="中文\U0001F600ab" && form.Kernel.Snapshot(typed.Document.Id).Text==typed.Editor.Text,"undo patches the view in place");
                bool refused=false; try { typed.Document.Close(ClosePolicy.RefuseDirty); } catch(KernelException ex) { refused=ex.Code=="unsaved_document"; }
                Check(refused,"kernel refuses to close a dirty document without a policy");
                form.ClosePage(typed,ClosePolicy.Discard); Check(!form.Tabs.TabPages.Contains(typed),"discard close");
                // Toolbar buttons and shortcuts reach the same kernel history.
                var buttons=form.Controls.OfType<ToolStrip>().SelectMany(t=>t.Items.OfType<ToolStripButton>()).ToList();
                var undoButton=buttons.First(b=>b.Text=="撤销"); var redoButton=buttons.First(b=>b.Text=="重做");
                var tools=form.NewDocument(Template.All[0]); tools.Editor.Select(0,0); tools.Editor.SelectedText="abc"; form.Tabs.SelectedTab=tools; Application.DoEvents();
                Check(undoButton.Enabled && !redoButton.Enabled,"toolbar undo enabled after an edit, redo disabled");
                undoButton.PerformClick(); Check(tools.Editor.Text=="" && redoButton.Enabled && !undoButton.Enabled,"toolbar undo");
                redoButton.PerformClick(); Check(tools.Editor.Text=="abc","toolbar redo");
                form.Undo(); Check(form.HandleShortcut(Keys.Control|Keys.Shift|Keys.Z) && tools.Editor.Text=="abc","Ctrl+Shift+Z redoes");
                Check(!form.HandleShortcut(Keys.Control|Keys.Shift|Keys.Z),"Ctrl+Shift+Z does nothing when there is nothing to redo");
                form.ClosePage(tools,ClosePolicy.Discard);
                report+="PASS toolbar undo/redo buttons, Ctrl+Shift+Z\r\n";
                report+="PASS kernel-backed typing, coalesced undo/redo, surrogate-safe ranges, revision agreement, close policy\r\n";
                foreach(var template in Template.All) if(template.Project) {
                    string name=template.Extension==".cs" ? "CSharpSmoke" : "VisualBasicSmoke";
                    string solution=template.CreateProject(directory,name); form.OpenProject(solution); form.OpenFile(Path.Combine(Path.GetDirectoryName(solution),"Program"+template.Extension));
                    Check(form.Active.Editor.Text.Contains("Console.WriteLine"),name+" editor contents visible");
                    Check(form.Workspace!=null && form.Workspace.Projects.Count==1 && form.Workspace.Projects[0].Executable && form.Configuration=="Debug|Any CPU",name+" kernel project graph and configuration");
                    // Build and run as kernel tasks; the UI keeps pumping messages while they run.
                    form.BuildAndRun(); var timer=Stopwatch.StartNew();
                    while((form.BuildRunning || form.LastRunExitCode==null) && timer.Elapsed.TotalSeconds<90) { Application.DoEvents(); Thread.Sleep(30); }
                    Check(!form.BuildRunning && form.LastBuildExitCode==0 && form.LastRunExitCode==0 && form.OutputText.Contains("Hello, OpenMSE"),name+" GUI build and run: "+form.OutputText);
                    if(template.Extension==".cs") {
                        var source=form.Active; string good=source.Editor.Text;
                        source.Editor.Text=good.Replace("Console.WriteLine(\"Hello, OpenMSE\");","Console.WriteLine(\"Hello, OpenMSE\")");
                        form.StartBuild(); timer=Stopwatch.StartNew();
                        while(form.BuildRunning && timer.Elapsed.TotalSeconds<60) { Application.DoEvents(); Thread.Sleep(30); }
                        Check(form.LastBuildExitCode!=0 && form.LastTask.State=="failed" && form.Diagnostics.Count>0 && form.Diagnostics[0].Code=="CS1002",name+" failed build listed in the error list");
                        form.Tabs.SelectedIndex=0; form.GoToDiagnostic(0);
                        var at=form.Active; int caret=at.Editor.SelectionStart; string before=at.Document.Shadow.Substring(0,caret);
                        Check(at==source && before.Count(c=>c=='\n')==form.Diagnostics[0].Line-1,name+" diagnostic navigates to its source line");
                        source.Undo(); Check(source.Editor.Text==good,name+" broken edit undone");
                        report+="PASS "+name+" failed build: error list and source navigation\r\n";
                        // Debugging from the window: F9 breakpoint, F5 build+debug, locals, F10, continue to exit.
                        source.Editor.Text=good.Replace("Console.WriteLine(\"Hello, OpenMSE\");","int answer = 42;\n        Console.WriteLine(\"Hello, OpenMSE\" + answer);");
                        form.SaveTo(source,null);
                        int bpLine=Array.FindIndex(source.Document.Shadow.Split('\n'),l=>l.Contains("Console.WriteLine"))+1;
                        source.Editor.Select(KernelDocument.IndexOf(source.Document.Shadow,bpLine-1,4),0); form.ToggleBreakpoint();
                        Check(form.HasBreakpoint(source.Document.PathName,bpLine),name+" F9 toggles a breakpoint on the caret line");
                        form.StartOrContinue();
                        Pump(()=>form.DebugSession!=null && !form.DebugSession.Active && form.DebugSession.State!="starting" || form.DebugSession!=null && form.DebugSession.Stopped,90);
                        Check(form.DebugSession!=null && form.DebugSession.Stopped && form.DebugFrames[0].Line==bpLine && form.Active==source,name+" F5 builds, debugs and stops at the breakpoint: "+(form.DebugSession==null ? "no session" : form.DebugSession.State+" "+form.DebugSession.LastError));
                        Check(form.Locals.Any(l=>l[0]=="answer" && l[1]=="42" && l[2]=="int") && form.Evaluate("answer")=="42",name+" locals window and evaluation");
                        form.DebugCommand("step_over"); Pump(()=>form.DebugSession.Stopped || !form.DebugSession.Active,30);
                        Check(form.DebugSession.Stopped && form.DebugFrames[0].Line>bpLine,name+" F10 steps to the next line");
                        form.StartOrContinue(); Pump(()=>!form.DebugSession.Active,30);
                        Check(form.DebugSession.State=="exited" && form.DebugSession.ExitCode==0 && form.OutputText.Contains("Hello, OpenMSE42"),name+" continue runs to exit with output: "+form.DebugSession.State);
                        form.SetBreakpoint(source.Document.PathName,bpLine,false); source.Editor.Text=good; form.SaveTo(source,null);
                        report+="PASS "+name+" debugging: F9 breakpoint, F5 build+debug, stop location, locals, evaluation, F10, continue to exit\r\n";
                    }
                    form.StartBuild(); timer=Stopwatch.StartNew();
                    while(form.BuildRunning && timer.Elapsed.TotalSeconds<60) { Application.DoEvents(); Thread.Sleep(30); }
                    Check(!form.BuildRunning && form.LastBuildExitCode==0 && form.LastTask.Artifacts.Count==1,name+" GUI build: exit "+form.LastBuildExitCode+", artifacts "+(form.LastTask==null ? -1 : form.LastTask.Artifacts.Count)+"\r\n"+form.OutputText.Substring(Math.Max(0,form.OutputText.Length-1500)));
                    string exe=Path.Combine(Path.GetDirectoryName(solution),@"bin\Debug\"+name+".exe");
                    var start=new ProcessStartInfo(exe) { UseShellExecute=false,CreateNoWindow=true,RedirectStandardOutput=true };
                    using(var process=Process.Start(start)) { string text=process.StandardOutput.ReadToEnd(); Check(process.WaitForExit(5000) && process.ExitCode==0 && text.Contains("Hello, OpenMSE"),name+" executable"); }
                    report+="PASS "+name+" template, unified GUI build, executable output\r\n";
                }
                report+=DesignerSmoke.Run(form,root,directory);
                report+=CodeSmoke(form,directory);
                form.TopMost=true; form.Show(); form.Activate(); form.Refresh(); Application.DoEvents(); Thread.Sleep(250); Application.DoEvents();
                using(var bitmap=new Bitmap(form.Width,form.Height)) { using(var graphics=Graphics.FromImage(bitmap)) graphics.CopyFromScreen(form.Location,Point.Empty,form.Size); bitmap.Save(Path.Combine(directory,"gui.png"),System.Drawing.Imaging.ImageFormat.Png); }
                File.WriteAllText(Path.Combine(directory,"results.txt"),report,new UTF8Encoding(true));
                form.Close();
            }
            return 0;
        }
    }
}
