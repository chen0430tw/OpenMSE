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
                form.TopMost=true; form.Show(); form.Activate(); form.Refresh(); Application.DoEvents(); Thread.Sleep(250); Application.DoEvents();
                using(var bitmap=new Bitmap(form.Width,form.Height)) { using(var graphics=Graphics.FromImage(bitmap)) graphics.CopyFromScreen(form.Location,Point.Empty,form.Size); bitmap.Save(Path.Combine(directory,"gui.png"),System.Drawing.Imaging.ImageFormat.Png); }
                File.WriteAllText(Path.Combine(directory,"results.txt"),report,new UTF8Encoding(true));
                form.Close();
            }
            return 0;
        }
    }
}
