using System;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal static class GuiSmoke
    {
        static void Check(bool condition,string name) { if(!condition) throw new Exception("GUI smoke failed: "+name); }
        internal static int Run(string root,string directory)
        {
            Directory.CreateDirectory(directory);
            string report="";
            using(var form=new MainForm(root)) {
                form.Show(); Application.DoEvents();
                using(var dialog=new NewItemDialog(directory)) { dialog.Show(form); Application.DoEvents(); Check(dialog.Visible,"new item dialog visible"); dialog.Close(); }
                var html=form.NewDocument(Template.All[1]); html.Editor.AppendText("\r\n<!-- 中文保存验证 -->\r\n");
                string htmlPath=Path.Combine(directory,"中文.html"); html.Document.Save(htmlPath,html.Editor.Text); html.UpdateTitle();
                Check(!html.Dirty,"saved editor clean"); form.OpenFile(htmlPath); Check(form.Active==html,"open document deduplication");
                Check(File.ReadAllText(htmlPath,Encoding.UTF8).Contains("中文保存验证"),"unicode disk roundtrip");
                // Exercise disk conflict protection, then restore the externally changed text in the test-owned file.
                string saved=File.ReadAllText(htmlPath,Encoding.UTF8); File.AppendAllText(htmlPath,"external",Encoding.UTF8);
                bool conflict=false; try { html.Document.Save(htmlPath,"must not overwrite"); } catch(IOException) { conflict=true; }
                Check(conflict && File.ReadAllText(htmlPath,Encoding.UTF8).EndsWith("external"),"external edits preserved");
                File.WriteAllText(htmlPath,saved,new UTF8Encoding(true));
                report+="PASS shared new-item dialog, document tabs, Unicode save, disk conflict guard\r\n";
                string lf=Path.Combine(directory,"lf.txt"); File.WriteAllText(lf,"a\n中文\n",new UTF8Encoding(false));
                var doc=new EditorDocument(); string value=doc.Read(lf); doc.Save(lf,value+"b\n"); byte[] bytes=File.ReadAllBytes(lf);
                Check(bytes[0]!=239 && !File.ReadAllText(lf).Contains("\r"),"UTF-8 no BOM and LF preserved");
                report+="PASS UTF-8 without BOM and LF preservation\r\n";
                foreach(var template in Template.All) if(template.Project) {
                    string name=template.Extension==".cs" ? "CSharpSmoke" : "VisualBasicSmoke";
                    string solution=template.CreateProject(directory,name); form.OpenProject(solution); form.OpenFile(Path.Combine(Path.GetDirectoryName(solution),"Program"+template.Extension));
                    Check(form.Active.Editor.Text.Contains("Console.WriteLine"),name+" editor contents visible");
                    form.StartBuild(); var timer=Stopwatch.StartNew();
                    while(form.BuildRunning && timer.Elapsed.TotalSeconds<45) { Application.DoEvents(); Thread.Sleep(30); }
                    Check(!form.BuildRunning && form.LastBuildExitCode==0,name+" GUI build");
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
