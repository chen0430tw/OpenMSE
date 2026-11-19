using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal static class DesignerSmoke
    {
        static void Check(bool value,string message) { if(!value) throw new Exception("Designer smoke failed: "+message); }
        internal static string Run(MainForm form,string root,string directory)
        {
            var catalog=form.ToolboxEntries;
            Check(catalog.Count(c=>c.Family=="HTML")==17 && catalog.Count(c=>c.Family!="HTML")==22 && catalog.All(c=>!String.IsNullOrEmpty(c.Id)) && catalog.Select(c=>c.Id).Distinct().Count()==39,"kernel toolbox: both catalogs with stable IDs");
            Func<EditorPage,KernelDesign> kernelDesign=p=>form.Kernel.Design(p.Document.Id);
            Func<EditorPage,bool> inSync=p=>p.Editor.Text==form.Kernel.Snapshot(p.Document.Id).Text && p.Designer.Revision==p.Document.Revision;

            // ---- HTML: drop -> property -> source sync -> undo/redo -> save -> reopen.
            var page=form.NewDocument(Template.All[1]);
            string untouched="<!-- preserve -->\n<script>var data = '<button id=\"notAnElement\">';</script>\n<section data-note='a > b'>未知内容</section>\n";
            page.Editor.Text=page.Editor.Text.Replace("</body>",untouched+"</body>"); page.Designer.Reload();
            Check(page.Designer.ErrorMessage==null && page.Designer.Html,"HTML designer initialized from kernel snapshot");
            var entry=catalog.First(c=>c.Family=="HTML" && c.Name=="Button");
            ulong revision=page.Document.Revision;
            page.Designer.Insert(entry,new Point(60,100));
            Check(page.Dirty && page.Document.Revision==revision+1 && inSync(page),"design insertion is one kernel revision; source view in sync");
            var selected=page.Designer.Selected; Check(selected!=null && selected.Kind=="Button" && selected.Name=="control1","inserted element selected");
            selected.Text="按钮 \"中文\" & 保存"; selected.X=180; selected.Y=120; selected.Width=180; page.Designer.CommitSelected();
            Check(page.Editor.Text.Contains(untouched),"unrelated scripts and unknown markup preserved exactly");
            Check(page.Editor.Text.Contains("value=\"按钮 &quot;中文&quot; &amp; 保存\"") && inSync(page),"property change patched into the source view");
            var button=kernelDesign(page).Nodes.Last();
            Check(button.Text=="按钮 \"中文\" & 保存" && button.X==180 && button.Y==120 && button.Width==180,"properties persisted to HTML");
            Check(Object.ReferenceEquals(form.SelectedDesignerObject,page.Designer.Selected),"property panel follows design selection");
            string before=page.Editor.Text; page.Views.SelectedIndex=1; page.Views.SelectedIndex=0; Check(page.Editor.Text==before,"switching views does not rewrite source");
            page.Undo(); Check(kernelDesign(page).Nodes.Last().Text=="Button" && page.Designer.Items.Last().Text=="Button" && inSync(page),"property transaction undone; designer reloaded");
            page.Redo(); Check(kernelDesign(page).Nodes.Last().X==180 && inSync(page),"property transaction redone");
            int count=kernelDesign(page).Nodes.Count;
            page.Designer.Insert(catalog.First(c=>c.Family=="HTML" && c.Name=="Text Field"),new Point(60,180));
            page.Designer.RemoveSelected(); Check(kernelDesign(page).Nodes.Count==count,"delete selected element");
            page.Undo(); Check(kernelDesign(page).Nodes.Count==count+1 && page.Designer.ErrorMessage==null,"designer delete undone through kernel history");
            page.Redo(); Check(kernelDesign(page).Nodes.Count==count,"designer delete redone");
            page.Editor.AppendText("\n<!-- newer source -->"); bool stale=false;
            try { page.Designer.Insert(entry,new Point(20,20)); } catch(InvalidOperationException) { stale=true; }
            Check(stale && page.Editor.Text.Contains("newer source") && page.Designer.Revision==page.Document.Revision,"stale designer revision refused by the kernel and reloaded");
            bool wrong=false; try { page.Designer.Insert(catalog.First(c=>c.Family!="HTML"),Point.Empty); } catch(KernelException ex) { wrong=ex.Code=="invalid_state"; } Check(wrong,"cross-family drops rejected by the kernel");
            selected=page.Designer.Items.Last(); page.Designer.Select(selected); selected.Name="1 bad"; bool invalid=false;
            try { page.Designer.CommitSelected(); } catch(KernelException ex) { invalid=ex.Code=="invalid_argument"; }
            Check(invalid && page.Designer.Items.Last().Name=="control1","invalid property refused; view restored from the kernel");
            string htmlPath=Path.Combine(directory,"designer-roundtrip.html"); form.SaveTo(page,htmlPath);
            form.ClosePage(page,ClosePolicy.RefuseDirty); form.OpenFile(htmlPath); page=form.Active;
            Check(page.Designer!=null && page.Designer.Items.Last().Text==button.Text && page.Designer.Items.Last().X==180 && page.Editor.Text.Contains(untouched),"HTML save, close and reopen keep design and source");
            string report="PASS kernel toolbox: 17 MSE HTML + 22 Windows Forms entries with stable IDs; designer/property selection\r\n"
                +"PASS HTML loop: drop, property writeback, source sync, undo/redo, delete, save, reopen; source preservation\r\n"
                +"PASS stale-revision, cross-family and invalid-property guards enforced by the kernel\r\n";

            // ---- Windows Forms layout: the same loop, plus export.
            var native=form.NewDocument(Template.All.Last());
            Check(native.Designer!=null && !native.Designer.Html && native.Designer.ErrorMessage==null,"Windows Forms designer initialized");
            native.Designer.Select(null); ((FormProperties)form.SelectedDesignerObject).Title="测试窗体"; native.Designer.CommitSelected(); Check(kernelDesign(native).Title=="测试窗体" && inSync(native),"form root properties saved");
            int index=0; foreach(var control in catalog.Where(c=>c.Family!="HTML")) { native.Designer.Insert(control,new Point(20+(index%4)*165,20+(index/4)*70)); index++; }
            Check(kernelDesign(native).Nodes.Count==22 && inSync(native),"all 22 controls dropped");
            native.Designer.Select(native.Designer.Items.First());
            native.Designer.Selected.Name="this"; native.Designer.Selected.Text="测试 \"quote\" \\ path\r\n第二行"; native.Designer.CommitSelected();
            Check(kernelDesign(native).Nodes.First().Name=="this" && kernelDesign(native).Nodes.First().Text=="测试 \"quote\" \\ path\r\n第二行","control properties written");
            native.Undo(); Check(kernelDesign(native).Nodes.First().Name=="button1" && inSync(native),"form property transaction undone");
            native.Redo(); Check(kernelDesign(native).Nodes.First().Name=="this","form property transaction redone");
            string nativePath=Path.Combine(directory,"form.omform"); form.SaveTo(native,nativePath);
            form.ClosePage(native,ClosePolicy.RefuseDirty); form.OpenFile(nativePath); native=form.Active;
            Check(native.Designer.Items.Count==22 && native.Designer.Items.First().Text=="测试 \"quote\" \\ path\r\n第二行" && kernelDesign(native).Title=="测试窗体","form save, close and reopen keep all controls");
            foreach(bool vb in new[]{false,true}) {
                string source=Path.Combine(directory,vb ? "DesignedForm.vb" : "DesignedForm.cs"); File.WriteAllText(source,native.Designer.Export(vb),new UTF8Encoding(true));
                string tools=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Windows),@"Microsoft.NET\Framework\v2.0.50727");
                var start=new ProcessStartInfo(Path.Combine(tools,vb ? "vbc.exe" : "csc.exe"),"/nologo /target:winexe /reference:System.Windows.Forms.dll /reference:System.Drawing.dll /out:"+MainForm.Quote(source+".exe")+" "+MainForm.Quote(source)) { UseShellExecute=false,CreateNoWindow=true,RedirectStandardOutput=true,RedirectStandardError=true };
                using(var process=Process.Start(start)) { string stdout=process.StandardOutput.ReadToEnd(),stderr=process.StandardError.ReadToEnd(); Check(process.WaitForExit(15000) && process.ExitCode==0,"exported "+(vb ? "VB" : "C#")+" compiles against .NET 2.0: "+stdout+stderr); }
            }
            report+="PASS form loop: root and control properties, 22 drops, undo/redo, save, reopen; kernel C# and VB export compiled against .NET 2.0\r\n";
            string valid=native.Editor.Text; native.Editor.Text="{ invalid layout"; native.Designer.Reload(); Check(native.Designer.ErrorMessage!=null && native.Editor.Text=="{ invalid layout","invalid JSON preserves source"); native.Editor.Text=valid; native.Designer.Reload();
            form.SaveTo(native,null);

            // ---- An existing hand-formatted layout keeps its formatting: moving one control changes one value.
            string sample=Path.Combine(directory,"sample.omform"); File.Copy(Path.Combine(root,@"samples\WindowsForm.omform"),sample);
            byte[] original=File.ReadAllBytes(sample);
            form.OpenFile(sample); var formatted=form.Active; var first=formatted.Designer.Items.First(); formatted.Designer.Select(first);
            int oldX=first.X; first.X=oldX+10; formatted.Designer.CommitSelected(); form.SaveTo(formatted,null);
            string expected=Encoding.UTF8.GetString(original); int at=expected.IndexOf("\"X\": "+oldX,StringComparison.Ordinal);
            expected=expected.Substring(0,at)+"\"X\": "+(oldX+10)+expected.Substring(at+("\"X\": "+oldX).Length);
            Check(at>0 && Encoding.UTF8.GetString(File.ReadAllBytes(sample))==expected,"pretty-printed layout: only the moved value changes on disk");
            form.ClosePage(formatted,ClosePolicy.RefuseDirty);
            report+="PASS hand-formatted .omform: one design move changes exactly one value on disk\r\n";

            // Leave a clean HTML document active for the visual inspection capture.
            form.Tabs.SelectedTab=page; page.Designer.Reload(); page.Designer.Select(page.Designer.Items.Last());
            Check(!page.Dirty && !native.Dirty,"test documents clean before close");
            return report;
        }
    }
}
