using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal static class DesignerSmoke
    {
        static void Check(bool value,string message) { if(!value) throw new Exception("Designer smoke failed: "+message); }
        internal static string Run(MainForm form,string root,string directory)
        {
            var catalog=new JavaScriptSerializer().Deserialize<List<ToolboxEntry>>(File.ReadAllText(Path.Combine(root,"config/toolbox.json")));
            Check(catalog.Count(c=>c.Family=="HTML")==17 && catalog.Count(c=>c.Family!="HTML")==22,"both toolbox catalogs");
            var page=form.NewDocument(Template.All[1]);
            string untouched="<!-- preserve -->\n<script>var data = '<button id=\"notAnElement\">';</script>\n<section data-note='a > b'>未知内容</section>\n";
            page.Editor.Text=page.Editor.Text.Replace("</body>",untouched+"</body>"); page.Designer.Reload();
            Check(page.Designer.ErrorMessage==null && page.Designer.Html,"HTML designer initialized");
            var entry=catalog.First(c=>c.Family=="HTML" && c.Name=="Button");
            page.Designer.Insert(entry,new Point(60,100)); Check(page.Dirty,"design insertion changes document");
            var selected=page.Designer.Selected; Check(selected!=null && selected.Kind=="Button","inserted element selected");
            selected.Text="按钮 \"中文\" & 保存"; selected.X=180; selected.Y=120; selected.Width=180; page.Designer.CommitSelected();
            Check(page.Editor.Text.Contains(untouched),"unrelated scripts and unknown markup preserved exactly");
            var html=new HtmlLayout(page.Editor.Text); var button=html.Items.Last();
            Check(button.Text=="按钮 \"中文\" & 保存" && button.X==180 && button.Y==120 && button.Width==180,"properties persisted to HTML");
            Check(Object.ReferenceEquals(form.SelectedDesignerObject,page.Designer.Selected),"property panel follows design selection");
            string before=page.Editor.Text; page.Views.SelectedIndex=1; page.Views.SelectedIndex=0; Check(page.Editor.Text==before,"switching views does not rewrite source");
            page.Designer.Insert(catalog.First(c=>c.Family=="HTML" && c.Name=="Text Field"),new Point(60,180));
            page.Designer.RemoveSelected(); Check(new HtmlLayout(page.Editor.Text).Items.Count==html.Items.Count,"delete selected element");
            page.Editor.AppendText("\n<!-- newer source -->"); bool stale=false;
            try { page.Designer.Insert(entry,new Point(20,20)); } catch(InvalidOperationException) { stale=true; }
            Check(stale && page.Editor.Text.Contains("newer source"),"stale designer cannot overwrite source edits");
            bool wrong=false; try { page.Designer.Insert(catalog.First(c=>c.Family!="HTML"),Point.Empty); } catch(InvalidOperationException) { wrong=true; } Check(wrong,"cross-family drops rejected");
            string htmlPath=Path.Combine(directory,"designer-roundtrip.html"); page.Document.Save(htmlPath,page.Editor.Text); form.Save(page,false);
            var saved=new EditorDocument(); var reopened=new HtmlLayout(saved.Read(htmlPath)); Check(reopened.Items.Last().Text==button.Text && reopened.Items.Last().X==180,"HTML save and reopen");
            string report="PASS 17 MSE HTML entries + 22 Windows Forms entries; designer/property selection\r\nPASS HTML insertion, property writeback, deletion, save/reopen, view switching, source preservation\r\nPASS stale-source and cross-family guards\r\n";
            var native=form.NewDocument(Template.All.Last());
            Check(native.Designer!=null && !native.Designer.Html,"Windows Forms designer initialized");
            native.Designer.Select(null); ((FormLayout)form.SelectedDesignerObject).Title="测试窗体"; native.Designer.CommitSelected(); Check(FormLayout.Parse(native.Editor.Text).Title=="测试窗体","form root properties saved");
            int index=0; foreach(var control in catalog.Where(c=>c.Family!="HTML")) { native.Designer.Insert(control,new Point(20+(index%4)*165,20+(index/4)*70)); index++; }
            native.Designer.Select(FormLayout.Parse(native.Editor.Text).Controls.First());
            // Select the live model item rather than a separately parsed copy before changing properties.
            native.Designer.Reload(); native.Designer.Selected.Name="this"; native.Designer.Selected.Text="测试 \"quote\" \\ path\r\n第二行"; native.Designer.CommitSelected();
            string nativePath=Path.Combine(directory,"form.omform"); native.Document.Save(nativePath,native.Editor.Text); form.Save(native,false);
            Check(FormLayout.Parse(File.ReadAllText(nativePath)).Controls.Count==22,"all native controls saved");
            foreach(bool vb in new[]{false,true}) {
                string source=Path.Combine(directory,vb ? "DesignedForm.vb" : "DesignedForm.cs"); File.WriteAllText(source,native.Designer.Export(vb),new UTF8Encoding(true));
                string tools=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Windows),@"Microsoft.NET\Framework\v2.0.50727");
                var start=new ProcessStartInfo(Path.Combine(tools,vb ? "vbc.exe" : "csc.exe"),"/nologo /target:winexe /reference:System.Windows.Forms.dll /reference:System.Drawing.dll /out:"+MainForm.Quote(source+".exe")+" "+MainForm.Quote(source)) { UseShellExecute=false,CreateNoWindow=true,RedirectStandardOutput=true,RedirectStandardError=true };
                using(var process=Process.Start(start)) { string stdout=process.StandardOutput.ReadToEnd(),stderr=process.StandardError.ReadToEnd(); Check(process.WaitForExit(15000) && process.ExitCode==0,"exported "+(vb ? "VB" : "C#")+" compiles against .NET 2.0: "+stdout+stderr); }
            }
            report+="PASS native layout property changes/save/reopen; all 22 controls exported and compiled in C# and VB against .NET 2.0\r\n";
            string valid=native.Editor.Text; native.Editor.Text="{ invalid layout"; native.Designer.Reload(); Check(native.Designer.ErrorMessage!=null && native.Editor.Text=="{ invalid layout","invalid JSON preserves source"); native.Editor.Text=valid; native.Designer.Reload();
            // Leave a clean HTML document active for the visual inspection capture.
            native.Document.Save(nativePath,native.Editor.Text);
            form.Tabs.SelectedTab=page; page.Designer.Reload(); page.Designer.Select(page.Designer.Items.Last());
            Check(!page.Dirty && !native.Dirty,"test documents clean before close");
            return report;
        }
    }
}
