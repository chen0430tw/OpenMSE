using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;

namespace OpenMSE.Shell
{
    internal sealed class NewItemDialog : Form
    {
        internal Template Selected;
        internal string ItemName, ParentFolder;
        readonly ListBox items = new ListBox();
        readonly Label description = new Label();
        readonly TextBox name = new TextBox { Text="OpenMSEProject" };
        readonly TextBox folder = new TextBox();
        internal NewItemDialog(string workspace)
        {
            Text="新建 — OpenMSE"; Size=new Size(760, 510); MinimumSize=Size;
            StartPosition=FormStartPosition.CenterParent; Font=new Font("Microsoft YaHei UI", 9F);
            ShowInTaskbar=false; MinimizeBox=false; MaximizeBox=false; BackColor=Color.White;
            var layout=new TableLayoutPanel { Dock=DockStyle.Fill, Padding=new Padding(20), ColumnCount=3, RowCount=6 };
            layout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 78)); layout.ColumnStyles.Add(new ColumnStyle(SizeType.Percent, 100)); layout.ColumnStyles.Add(new ColumnStyle(SizeType.Absolute, 90));
            layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 34)); layout.RowStyles.Add(new RowStyle(SizeType.Percent, 100));
            layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 52)); layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 36)); layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 36)); layout.RowStyles.Add(new RowStyle(SizeType.Absolute, 46));
            var heading=new Label { Text="选择文件或项目模板", Dock=DockStyle.Fill, Font=new Font(Font, FontStyle.Bold) }; layout.Controls.Add(heading,0,0); layout.SetColumnSpan(heading,3);
            items.Dock=DockStyle.Fill; items.BorderStyle=BorderStyle.FixedSingle; items.ItemHeight=32; items.IntegralHeight=false;
            items.Items.AddRange(Template.All); layout.Controls.Add(items,0,1); layout.SetColumnSpan(items,3);
            description.Dock=DockStyle.Fill; description.Padding=new Padding(0,10,0,0); layout.Controls.Add(description,0,2); layout.SetColumnSpan(description,3);
            layout.Controls.Add(new Label { Text="名称", AutoSize=true, Padding=new Padding(0,6,0,0) },0,3); name.Dock=DockStyle.Fill; layout.Controls.Add(name,1,3); layout.SetColumnSpan(name,2);
            layout.Controls.Add(new Label { Text="位置", AutoSize=true, Padding=new Padding(0,6,0,0) },0,4); folder.Text=workspace; folder.Dock=DockStyle.Fill; layout.Controls.Add(folder,1,4);
            var browse=new Button { Text="浏览…", Dock=DockStyle.Fill }; browse.Click+=delegate { using(var dialog=new FolderBrowserDialog()) { dialog.SelectedPath=folder.Text; if(dialog.ShowDialog(this)==DialogResult.OK) folder.Text=dialog.SelectedPath; } }; layout.Controls.Add(browse,2,4);
            var buttons=new FlowLayoutPanel { Dock=DockStyle.Fill, FlowDirection=FlowDirection.RightToLeft, Padding=new Padding(0,8,0,0) };
            var cancel=new Button { Text="取消", DialogResult=DialogResult.Cancel, Width=90 }; var create=new Button { Text="创建", Width=90 };
            create.Click+=delegate { Selected=(Template)items.SelectedItem; ItemName=name.Text.Trim(); ParentFolder=folder.Text; if(Selected!=null && (!Selected.Project || !String.IsNullOrWhiteSpace(ItemName))) { DialogResult=DialogResult.OK; Close(); } };
            buttons.Controls.Add(cancel); buttons.Controls.Add(create); layout.Controls.Add(buttons,0,5); layout.SetColumnSpan(buttons,3); AcceptButton=create; CancelButton=cancel;
            items.SelectedIndexChanged+=delegate { Template t=(Template)items.SelectedItem; description.Text=t.Description; name.Enabled=folder.Enabled=browse.Enabled=t.Project; };
            items.SelectedIndex=0; Controls.Add(layout);
        }
    }

    internal sealed class EditorPage : TabPage, IDesignSource
    {
        // The kernel owns the text, revisions and undo history; the RichTextBox is only a view of it.
        internal readonly KernelDocument Document;
        // Native shortcuts are off so Ctrl+Z/Y never use the control's private undo stack.
        internal readonly RichTextBox Editor = new RichTextBox { Dock=DockStyle.Fill, BorderStyle=BorderStyle.None, Font=new Font("Consolas", 11F), AcceptsTab=true, WordWrap=false, DetectUrls=false, HideSelection=false, BackColor=Color.White, ForeColor=Color.FromArgb(32,40,53), ScrollBars=RichTextBoxScrollBars.Both, ShortcutsEnabled=false };
        internal readonly string UntitledName;
        internal Action<Exception> SyncFailed;
        bool applying;
        internal DesignerView Designer;
        internal MainForm.CodeBarState CodeBar;   // object/event lists when this is a form's code file
        internal TabControl Views;
        internal void SetupDesigner()
        {
            string extension=Path.GetExtension(Document.PathName ?? UntitledName).ToLowerInvariant();
            bool html=extension==".html" || extension==".htm", native=extension==".omform";
            if(!html && !native) return;
            if(Designer==null) {
                Padding=new Padding(0); Controls.Remove(Editor);
                Views=new TabControl { Dock=DockStyle.Fill,Alignment=TabAlignment.Bottom,Padding=new Point(15,4) };
                var designTab=new TabPage("设计"); var sourceTab=new TabPage(html ? "HTML" : "布局 JSON");
                Designer=new DesignerView(html,this);
                designTab.Controls.Add(Designer); sourceTab.Controls.Add(Editor); Views.TabPages.Add(designTab); Views.TabPages.Add(sourceTab); Controls.Add(Views);
                Views.SelectedIndexChanged+=delegate { if(Views.SelectedIndex==0) Designer.Reload(); };
            }
            Designer.Reload();
        }
        internal bool Dirty { get { return Document.Dirty; } }
        internal EditorPage(KernelDocument document)
        {
            Document=document; UntitledName=document.Name; Padding=new Padding(12,10,0,0); BackColor=Color.White;
            SetText(document.Shadow); Editor.TextChanged+=OnTextChanged; Controls.Add(Editor); UpdateTitle();
        }
        void SetText(string text) { applying=true; try { Editor.Text=text; } finally { applying=false; } }
        // Every user edit becomes one ranged kernel edit; typing is coalesced into undo groups by the kernel.
        void OnTextChanged(object sender,EventArgs e)
        {
            if(applying) return;
            try { Document.Sync(Editor.Text,true); }
            catch(Exception ex) { Resync(); if(SyncFailed!=null) SyncFailed(ex); }
        }
        internal void Resync() { Document.Refresh(); SetText(Document.Shadow); }
        // Design operations go to the kernel's DesignService, which commits one source patch (one revision,
        // one undo step); the source view is then patched with exactly that edit.
        KernelDesign IDesignSource.LoadDesign() { return Document.Design(); }
        KernelDesignResult IDesignSource.ApplyDesign(Dictionary<string,object> operation,ulong expectedRevision)
        {
            bool changed; int index,length; string text;
            var result=Document.ApplyDesign(operation,expectedRevision,out changed,out index,out length,out text);
            if(changed) PatchView(index,length,text);
            UpdateTitle();
            return result;
        }
        string IDesignSource.ExportDesign(bool vb) { return Document.ExportDesign(vb); }
        internal void Undo() { History(true); }
        internal void Redo() { History(false); }
        void History(bool undo)
        {
            int index,length; string text;
            if(!(undo ? Document.Undo(out index,out length,out text) : Document.Redo(out index,out length,out text))) return;
            PatchView(index,length,text);
            if(Designer!=null) Designer.Reload();
            UpdateTitle();
        }
        // Patch the view in place so the caret lands on the change; fall back to the kernel text if the control disagrees.
        void PatchView(int index,int length,string text)
        {
            applying=true; try { Editor.Select(index,length); Editor.SelectedText=text; } finally { applying=false; }
            if(KernelDocument.Normalize(Editor.Text)!=Document.Shadow) SetText(Document.Shadow);
        }
        internal void UpdateTitle() { Text=(Document.PathName==null ? UntitledName : Path.GetFileName(Document.PathName))+(Dirty ? " *" : ""); ToolTipText=Document.PathName ?? UntitledName; }
    }

    internal sealed partial class MainForm : Form
    {
        internal readonly TabControl Tabs = new TabControl { Dock=DockStyle.Fill, Padding=new Point(16,7), ShowToolTips=true };
        readonly TreeView tree = new TreeView { Dock=DockStyle.Fill, BorderStyle=BorderStyle.None, HideSelection=false, FullRowSelect=true, ItemHeight=26, BackColor=Color.FromArgb(247,249,252) };
        readonly TextBox output = new TextBox { Dock=DockStyle.Fill, Multiline=true, ReadOnly=true, ScrollBars=ScrollBars.Both, WordWrap=false, BorderStyle=BorderStyle.None, Font=new Font("Consolas", 10F), BackColor=Color.FromArgb(248,250,253) };
        readonly ToolStripStatusLabel status = new ToolStripStatusLabel("就绪");
        readonly ToolStripStatusLabel position = new ToolStripStatusLabel { Spring=true, TextAlign=ContentAlignment.MiddleRight };
        readonly SplitContainer horizontal = new SplitContainer { Dock=DockStyle.Fill, Orientation=Orientation.Horizontal, FixedPanel=FixedPanel.Panel2 };
        readonly SplitContainer vertical = new SplitContainer { Dock=DockStyle.Fill, FixedPanel=FixedPanel.Panel1 };
        readonly SplitContainer right=new SplitContainer { Dock=DockStyle.Fill,FixedPanel=FixedPanel.Panel2 };
        readonly SplitContainer inspector=new SplitContainer { Dock=DockStyle.Fill,Orientation=Orientation.Horizontal };
        readonly PropertyGrid properties=new PropertyGrid { Dock=DockStyle.Fill,HelpVisible=true,ToolbarVisible=true };
        readonly ToolboxPanel toolbox;
        readonly ListView errors=new ListView { Dock=DockStyle.Fill, View=View.Details, FullRowSelect=true, BorderStyle=BorderStyle.None, HideSelection=false };
        readonly TabControl outputTabs=new TabControl { Dock=DockStyle.Fill, Alignment=TabAlignment.Bottom };
        readonly ToolStripComboBox configurationBox=new ToolStripComboBox { DropDownStyle=ComboBoxStyle.DropDownList, AutoSize=false, Width=160, ToolTipText="解决方案配置" };
        readonly Timer eventPump=new Timer { Interval=100 };
        readonly string root;
        string workspace;
        int untitled;
        // The workspace (project graph) and build/run tasks belong to the kernel; this form only shows them.
        internal KernelWorkspace Workspace;
        KernelTask currentTask;
        bool runAfterBuild;
        internal KernelTask LastTask;
        internal bool BuildRunning { get { return currentTask!=null && currentTask.Running; } }
        internal int? LastBuildExitCode, LastRunExitCode;
        internal string Configuration { get { return configurationBox.SelectedItem as string; } set { configurationBox.SelectedItem=value; } }
        internal IList<KernelDiagnostic> Diagnostics { get { return errors.Items.Cast<ListViewItem>().Select(i=>(KernelDiagnostic)i.Tag).ToList(); } }
        internal string OutputText { get { return output.Text; } }
        ToolStripMenuItem buildItem, runItem, cancelItem, undoItem, redoItem;
        ToolStripButton buildButton, runButton, cancelButton, undoButton, redoButton;
        internal readonly KernelClient Kernel;
        internal readonly IList<ToolboxEntry> ToolboxEntries;
        internal MainForm(string projectRoot)
        {
            try { Kernel=new KernelClient(); }
            catch(DllNotFoundException ex) { throw new InvalidOperationException("找不到 OpenMSE 内核 bin\\OpenMSE.Core.dll，请运行 build-core.cmd。",ex); }
            catch(BadImageFormatException ex) { throw new InvalidOperationException("OpenMSE.Core.dll 与界面位数不一致（界面为 x64）。",ex); }
            catch(EntryPointNotFoundException ex) { throw new InvalidOperationException("OpenMSE.Core.dll 版本过旧，缺少所需的接口。",ex); }
            Kernel.EventRaised+=OnKernelEvent;
            root=projectRoot; workspace=Path.Combine(root,"workspace");
            ToolboxEntries=Kernel.LoadToolbox(Path.Combine(root,"config","toolbox.json")).AsReadOnly(); toolbox=new ToolboxPanel(ToolboxEntries);
            Text="OpenMSE"; Size=new Size(1360,860); MinimumSize=new Size(960,640); StartPosition=FormStartPosition.CenterScreen;
            Font=new Font("Microsoft YaHei UI",9F); BackColor=Color.White;
            Icon=Icon.ExtractAssociatedIcon(Path.Combine(root,"OpenMSE.exe"));
            var menu=new MenuStrip { BackColor=Color.White, Padding=new Padding(10,4,0,4) };
            var file=AddMenu(menu,"文件(&F)"); Item(file,"新建文件或项目…",Keys.Control|Keys.N,NewItem); Item(file,"打开文件…",Keys.Control|Keys.O,OpenDialog); Item(file,"打开项目或解决方案…",Keys.Control|Keys.Shift|Keys.O,OpenProjectDialog);
            file.DropDownItems.Add(new ToolStripSeparator()); Item(file,"保存",Keys.Control|Keys.S,delegate { Save(Active,false); }); Item(file,"另存为…",Keys.Control|Keys.Shift|Keys.S,delegate { Save(Active,true); }); Item(file,"全部保存",Keys.Control|Keys.Alt|Keys.S,delegate { SaveAll(); });
            Item(file,"关闭当前文档",Keys.Control|Keys.W,CloseActive); file.DropDownItems.Add(new ToolStripSeparator()); Item(file,"退出",Keys.Alt|Keys.F4,Close);
            var edit=AddMenu(menu,"编辑(&E)"); undoItem=Item(edit,"撤销",Keys.Control|Keys.Z,Undo); redoItem=Item(edit,"重做",Keys.Control|Keys.Y,Redo); redoItem.ShortcutKeyDisplayString="Ctrl+Y / Ctrl+Shift+Z"; edit.DropDownItems.Add(new ToolStripSeparator());
            Item(edit,"剪切",Keys.Control|Keys.X,delegate { if(Active!=null) Active.Editor.Cut(); }); Item(edit,"复制",Keys.Control|Keys.C,delegate { if(Active!=null) Active.Editor.Copy(); }); Item(edit,"粘贴",Keys.Control|Keys.V,delegate { if(Active!=null) Active.Editor.Paste(DataFormats.GetFormat(DataFormats.UnicodeText)); }); Item(edit,"全选",Keys.Control|Keys.A,delegate { if(Active!=null) Active.Editor.SelectAll(); });
            var view=AddMenu(menu,"视图(&V)"); Item(view,"项目资源管理器",Keys.None,delegate { right.Panel2Collapsed=!right.Panel2Collapsed; }); Item(view,"输出",Keys.Control|Keys.Alt|Keys.O,delegate { horizontal.Panel2Collapsed=!horizontal.Panel2Collapsed; }); Item(view,"自动换行",Keys.None,delegate { if(Active!=null) Active.Editor.WordWrap=!Active.Editor.WordWrap; });
            Item(view,"工具箱",Keys.Control|Keys.Alt|Keys.X,delegate { vertical.Panel1Collapsed=!vertical.Panel1Collapsed; });
            var project=AddMenu(menu,"项目(&P)"); Item(project,"新建项目…",Keys.None,NewItem); Item(project,"刷新项目树",Keys.F5,RefreshTree); Item(project,"导出窗体 C# / VB 源码…",Keys.None,ExportForm);
            var builds=AddMenu(menu,"生成(&B)"); buildItem=Item(builds,"生成解决方案",Keys.Control|Keys.Shift|Keys.B,StartBuild); runItem=Item(builds,"生成并运行启动项目",Keys.Control|Keys.F5,BuildAndRun); cancelItem=Item(builds,"取消生成或运行",Keys.Control|Keys.Pause,CancelTask);
            var tools=AddMenu(menu,"工具(&T)"); Item(tools,"在 MSE 兼容窗口中打开",Keys.None,delegate { LaunchLegacy(false); }); Item(tools,"在 VS2005 兼容窗口中打开",Keys.None,delegate { LaunchLegacy(true); });
            var help=AddMenu(menu,"帮助(&H)"); Item(help,"关于 OpenMSE",Keys.None,delegate { MessageBox.Show(this,"OpenMSE\r\n版本 " + Application.ProductVersion + "\r\n\r\n移植自微软套件库的 Microsoft Script Editor。","关于 OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Information); });
            MainMenuStrip=menu;
            var toolbar=new ToolStrip { GripStyle=ToolStripGripStyle.Hidden, BackColor=Color.FromArgb(243,246,251), Padding=new Padding(12,5,6,5), ImageScalingSize=new Size(18,18) };
            Button(toolbar,"新建",SystemIcons.Application,NewItem); Button(toolbar,"打开",SystemIcons.WinLogo,OpenDialog); Button(toolbar,"保存",SystemIcons.Information,delegate { Save(Active,false); }); toolbar.Items.Add(new ToolStripSeparator());
            undoButton=Button(toolbar,"撤销",SystemIcons.Information,Undo); undoButton.ToolTipText="撤销 (Ctrl+Z)"; redoButton=Button(toolbar,"重做",SystemIcons.Information,Redo); redoButton.ToolTipText="重做 (Ctrl+Y 或 Ctrl+Shift+Z)"; toolbar.Items.Add(new ToolStripSeparator());
            toolbar.Items.Add(configurationBox); buildButton=Button(toolbar,"生成",SystemIcons.Shield,StartBuild); runButton=Button(toolbar,"运行",SystemIcons.Shield,BuildAndRun); cancelButton=Button(toolbar,"取消",SystemIcons.Shield,CancelTask);
            toolbar.Items.Add(new ToolStripSeparator()); toolbar.Items.Add(new ToolStripLabel("统一工作区") { ForeColor=Color.FromArgb(85,100,120) });
            var brand=new Panel { Dock=DockStyle.Top, Height=48, BackColor=Color.FromArgb(27,45,74), Padding=new Padding(16,0,0,0) };
            brand.Controls.Add(new Label { Text="OpenMSE", Font=new Font("Segoe UI",17F,FontStyle.Bold), ForeColor=Color.White, Dock=DockStyle.Left, Width=170, TextAlign=ContentAlignment.MiddleLeft });
            brand.Controls.Add(new Label { Text="脚本与项目工作区", ForeColor=Color.FromArgb(185,204,231), Dock=DockStyle.Right, Width=190, TextAlign=ContentAlignment.MiddleLeft });
            var bar=new StatusStrip { BackColor=Color.FromArgb(27,79,141), ForeColor=Color.White }; bar.Items.Add(status); bar.Items.Add(position);
            vertical.Panel1.Controls.Add(toolbox); vertical.Panel1.Controls.Add(PanelTitle("工具箱")); vertical.Panel2.Controls.Add(right);
            right.Panel1.Controls.Add(horizontal); right.Panel2.Controls.Add(inspector);
            inspector.Panel1.Controls.Add(tree); inspector.Panel1.Controls.Add(PanelTitle("项目资源管理器"));
            inspector.Panel2.Controls.Add(properties); inspector.Panel2.Controls.Add(PanelTitle("属性"));
            toolbox.CurrentDesigner=delegate { return Active==null ? null : Active.Designer; };
            toolbox.Failure=delegate(string error) { MessageBox.Show(this,error,"OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Information); };
            toolbox.NewHtml=delegate { Guard(delegate { NewDocument(Template.All[1]); }); };
            toolbox.NewForm=delegate { Guard(delegate { NewDocument(Template.All.Last()); }); };
            properties.PropertyValueChanged+=delegate { if(Active!=null && Active.Designer!=null) Guard(Active.Designer.CommitSelected); };
            horizontal.Panel2Collapsed=true;
            errors.Columns.Add("类型",60); errors.Columns.Add("代码",80); errors.Columns.Add("说明",460); errors.Columns.Add("文件",220); errors.Columns.Add("行",50);
            errors.DoubleClick+=delegate { if(errors.SelectedIndices.Count>0) Guard(delegate { GoToDiagnostic(errors.SelectedIndices[0]); }); };
            var outputTab=new TabPage("输出"); outputTab.Controls.Add(output); var errorTab=new TabPage("错误列表"); errorTab.Controls.Add(errors); outputTabs.TabPages.Add(outputTab); outputTabs.TabPages.Add(errorTab);
            horizontal.Panel1.Controls.Add(Tabs); horizontal.Panel2.Controls.Add(outputTabs); horizontal.Panel2.Controls.Add(PanelTitle("输出 · 生成 · 调试"));
            BuildDebugUi(menu,tools,bar); SetupTabs();
            // Task output and state arrive as kernel events from worker threads; they are drained here on the UI thread.
            eventPump.Tick+=delegate { try { Kernel.DrainEvents(); } catch(Exception ex) { eventPump.Stop(); MessageBox.Show(this,ex.Message,"OpenMSE"); } }; eventPump.Start();
            Controls.Add(vertical); Controls.Add(toolbar); Controls.Add(menu); Controls.Add(brand); Controls.Add(bar);
            Shown+=delegate { vertical.SplitterDistance=210; right.SplitterDistance=Math.Max(250,right.Width-260); inspector.SplitterDistance=Math.Max(150,inspector.Height/2-80); horizontal.SplitterDistance=Math.Max(200,horizontal.Height-160); };
            tree.NodeMouseDoubleClick+=delegate(object sender,TreeNodeMouseClickEventArgs e) { var editorPage=e.Node.Tag as EditorPage; if(editorPage!=null) { Tabs.SelectedTab=editorPage; return; } string path=e.Node.Tag as string; if(path!=null && File.Exists(path)) Guard(delegate { OpenFile(path); }); };
            Tabs.SelectedIndexChanged+=delegate { UpdateState(); };
            FormClosing+=OnClosing; Welcome(); RefreshTree(); UpdateState();
        }
        static Label PanelTitle(string text) { return new Label { Text=text, Dock=DockStyle.Top, Height=32, Padding=new Padding(10,7,0,0), BackColor=Color.FromArgb(233,238,246), ForeColor=Color.FromArgb(40,57,78) }; }
        static ToolStripMenuItem AddMenu(MenuStrip parent,string text) { var item=new ToolStripMenuItem(text); parent.Items.Add(item); return item; }
        ToolStripMenuItem Item(ToolStripMenuItem parent,string text,Keys keys,Action action) { var item=new ToolStripMenuItem(text) { ShortcutKeys=keys }; item.Click+=delegate { Guard(action); }; parent.DropDownItems.Add(item); return item; }
        ToolStripButton Button(ToolStrip parent,string text,Icon icon,Action action) { var item=new ToolStripButton(text,ToolIcon(text)) { DisplayStyle=ToolStripItemDisplayStyle.ImageAndText }; item.Click+=delegate { Guard(action); }; parent.Items.Add(item); return item; }
        // Undo and redo go through the kernel's history for the active document (text and design edits alike).
        internal void Undo() { if(Active!=null) { Active.Undo(); UpdateState(); } }
        internal void Redo() { if(Active!=null) { Active.Redo(); UpdateState(); } }
        // Ctrl+Shift+Z is the second redo shortcut; a menu item holds only one.
        internal bool HandleShortcut(Keys keys)
        {
            if(keys==(Keys.Control|Keys.Shift|Keys.Z) && redoItem.Enabled) { Guard(Redo); return true; }
            return false;
        }
        protected override bool ProcessCmdKey(ref Message msg,Keys keyData) { return HandleShortcut(keyData) || base.ProcessCmdKey(ref msg,keyData); }
        static Image ToolIcon(string text)
        {
            var image=new Bitmap(20,20);
            using(var graphics=Graphics.FromImage(image)) using(var pen=new Pen(Color.FromArgb(45,86,139),1.6F)) {
                graphics.SmoothingMode=System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
                if(text=="新建") { graphics.DrawRectangle(pen,4,2,11,15); graphics.DrawLine(pen,8,7,17,7); graphics.DrawLine(pen,13,3,13,12); }
                else if(text=="打开") { graphics.DrawLines(pen,new[]{new Point(2,16),new Point(2,5),new Point(8,5),new Point(10,7),new Point(17,7)}); graphics.DrawPolygon(pen,new[]{new Point(2,16),new Point(5,9),new Point(18,9),new Point(15,16)}); }
                else if(text=="撤销" || text=="重做") {
                    // A curved arrow: left-pointing for undo, mirrored for redo.
                    bool redo=text=="重做";
                    if(redo) { graphics.TranslateTransform(20,0); graphics.ScaleTransform(-1,1); }
                    graphics.DrawArc(pen,5,5,12,10,180,-200);
                    graphics.DrawLines(pen,new[]{ new Point(3,6), new Point(5,11), new Point(10,9) });
                }
                else if(text=="保存") { graphics.DrawRectangle(pen,3,2,14,15); graphics.DrawRectangle(pen,6,2,8,5); graphics.DrawRectangle(pen,6,11,8,6); }
                else { using(var brush=new SolidBrush(Color.FromArgb(38,135,92))) graphics.FillPolygon(brush,new[]{new Point(5,3),new Point(17,10),new Point(5,17)}); }
            }
            return image;
        }
        protected override void Dispose(bool disposing) { if(disposing) eventPump.Dispose(); base.Dispose(disposing); if(disposing && Kernel!=null) Kernel.Dispose(); }
        IEnumerable<EditorPage> Pages { get { return Tabs.TabPages.OfType<EditorPage>(); } }
        // Kernel events drive title, status and task output; they are delivered on the UI thread.
        void OnKernelEvent(KernelEvent e)
        {
            if(e.Topic=="task.output") { if(currentTask!=null && KernelClient.U64(e.Payload["task"])==currentTask.Id) output.AppendText((string)e.Payload["line"]+"\r\n"); return; }
            if(e.Topic=="task.finished") { TaskFinished(KernelTask.From(e.Payload)); return; }
            if(e.Topic.StartsWith("debug.") || e.Topic.StartsWith("host.")) { OnDebugOrHostEvent(e); return; }
            if(!e.Topic.StartsWith("document.")) return;
            var page=Pages.FirstOrDefault(p=>p.Document.Id==e.DocumentId);
            if(page==null) return;
            if(e.Topic=="document.changed" || e.Topic=="document.saved") page.UpdateTitle();
        }
        void Guard(Action action) { try { action(); } catch(Exception ex) { MessageBox.Show(this,ex.Message,"OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Error); } }
        internal object SelectedDesignerObject { get { return properties.SelectedObject; } }
        internal EditorPage Active { get { return Tabs.SelectedTab as EditorPage; } }
        void Welcome()
        {
            var page=new TabPage("开始") { BackColor=Color.White, Padding=new Padding(38) };
            var body=new FlowLayoutPanel { Dock=DockStyle.Fill, FlowDirection=FlowDirection.TopDown, WrapContents=false };
            body.Controls.Add(new Label { Text="欢迎使用 OpenMSE", Font=new Font("Microsoft YaHei UI",23F), AutoSize=true, Margin=new Padding(0,28,0,12) });
            body.Controls.Add(new Label { Text="在同一个工作区编辑脚本、管理项目并查看生成输出。", AutoSize=true, ForeColor=Color.FromArgb(95,108,124), Margin=new Padding(0,0,0,32) });
            foreach(var entry in new[] {new {Text="新建文件或项目",Action=(Action)NewItem},new {Text="打开文件",Action=(Action)OpenDialog},new {Text="打开项目或解决方案",Action=(Action)OpenProjectDialog}}) {
                var button=new Button { Text=entry.Text+"  →", Width=300, Height=48, TextAlign=ContentAlignment.MiddleLeft, FlatStyle=FlatStyle.Flat, BackColor=Color.FromArgb(243,247,253), Margin=new Padding(0,0,0,12) }; Action action=entry.Action; button.FlatAppearance.BorderColor=Color.FromArgb(212,223,239); button.Click+=delegate { Guard(action); }; body.Controls.Add(button);
            }
            body.Controls.Add(new Label { Text="文件模板：文本 / HTML / JavaScript / VBScript\r\n项目模板：C# / Visual Basic · .NET 2.0\r\n\r\n工具箱支持 HTML / Windows 窗体布局；专有设计器保留在兼容窗口。", AutoSize=true, ForeColor=Color.FromArgb(95,108,124), Margin=new Padding(0,24,0,0) });
            page.Controls.Add(body); Tabs.TabPages.Add(page);
        }
        void NewItem()
        {
            using(var dialog=new NewItemDialog(workspace)) {
                if(dialog.ShowDialog(this)!=DialogResult.OK) return;
                if(dialog.Selected.Project) { string path=dialog.Selected.CreateProject(dialog.ParentFolder,dialog.ItemName); OpenProject(path); OpenFile(Path.Combine(Path.GetDirectoryName(path),"Program"+dialog.Selected.Extension)); }
                else NewDocument(dialog.Selected);
            }
        }
        internal EditorPage NewDocument(Template template)
        {
            var page=new EditorPage(KernelDocument.Create(Kernel,"未命名"+(++untitled)+template.Extension,template.Content)); AddEditor(page); page.SetupDesigner(); page.UpdateTitle(); RefreshTree(); UpdateState(); return page;
        }
        void AddEditor(EditorPage page) { Tabs.TabPages.Add(page); Tabs.SelectedTab=page; page.SyncFailed=delegate(Exception ex) { MessageBox.Show(this,"编辑未能提交到内核，已恢复为内核中的文本：\r\n"+ex.Message,"OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Warning); }; page.Editor.TextChanged+=delegate { UpdateState(); }; page.Editor.SelectionChanged+=delegate { UpdatePosition(); }; page.SetupDesigner(); if(page.Designer!=null) { page.Designer.SelectionChanged=delegate(object selected) { if(Active==page) properties.SelectedObject=selected; }; page.Designer.Failure=toolbox.Failure; page.Designer.Activated=delegate(DesignItem item) { ActivateDesign(page,item); }; } AttachCodeBar(page); page.Editor.Focus(); RefreshTree(); }
        internal void OpenFile(string path)
        {
            if(String.Equals(Path.GetExtension(path),".sln",StringComparison.OrdinalIgnoreCase) || String.Equals(Path.GetExtension(path),".csproj",StringComparison.OrdinalIgnoreCase) || String.Equals(Path.GetExtension(path),".vbproj",StringComparison.OrdinalIgnoreCase)) { OpenProject(path); return; }
            path=Path.GetFullPath(path);
            if(File.Exists(path) && new FileInfo(path).Length>8*1024*1024) throw new IOException("文本编辑器目前支持最大 8 MB 的文件。");
            // The kernel deduplicates by normalised path and file identity, so a second open returns the same document.
            bool alreadyOpen; var document=KernelDocument.Open(Kernel,path,out alreadyOpen);
            var existing=Pages.FirstOrDefault(p=>p.Document.Id==document.Id);
            if(existing!=null) { Tabs.SelectedTab=existing; return; }
            var page=new EditorPage(document); AddEditor(page); page.UpdateTitle(); UpdateState();
        }
        void OpenDialog() { using(var d=new OpenFileDialog { Filter="文本与源文件|*.txt;*.htm;*.html;*.js;*.vbs;*.cs;*.vb;*.cpp;*.h;*.xml;*.json;*.md|所有文件|*.*", Multiselect=true }) if(d.ShowDialog(this)==DialogResult.OK) foreach(string path in d.FileNames) OpenFile(path); }
        void OpenProjectDialog() { using(var d=new OpenFileDialog { Filter="项目与解决方案|*.sln;*.csproj;*.vbproj|所有文件|*.*" }) if(d.ShowDialog(this)==DialogResult.OK) OpenProject(d.FileName); }
        internal void OpenProject(string path)
        {
            if(BuildRunning) throw new InvalidOperationException("生成或运行进行中，请等待完成或先取消。");
            var opened=Kernel.OpenWorkspace(Path.GetFullPath(path));
            if(Workspace!=null) Kernel.CloseWorkspace(Workspace.Id);
            Workspace=opened; workspace=Workspace.Root;
            configurationBox.Items.Clear(); foreach(string c in Workspace.Configurations) configurationBox.Items.Add(c);
            if(configurationBox.Items.Count>0) configurationBox.SelectedIndex=0;
            status.Text="已打开 "+Path.GetFileName(Workspace.Path)+"（"+Workspace.Projects.Count+" 个项目，"+(Workspace.MSBuild ?? "未找到 MSBuild")+"）";
            RefreshTree(); UpdateState();
        }
        // The tree shows the kernel's project graph: projects, their files and references; not a directory listing.
        void RefreshTree()
        {
            tree.BeginUpdate();
            try {
                tree.Nodes.Clear();
                if(Workspace==null) { var node=tree.Nodes.Add("其他文件"); foreach(var page in Pages) { var child=node.Nodes.Add(page.Text); child.Tag=page; } node.Expand(); return; }
                var top=tree.Nodes.Add(Path.GetFileName(Workspace.Path)+(Workspace.Solution ? "（"+Workspace.Projects.Count+" 个项目）" : "")); top.Tag=Workspace.Path;
                foreach(var project in Workspace.Projects) {
                    string label=project.Name+"  ·  "+(project.Kind=="csharp" ? "C#" : project.Kind=="vb" ? "VB" : project.Kind)+(project.OutputType.Length>0 ? " · "+project.OutputType : "")+(project.Id==Workspace.StartupProject ? " · 启动" : "");
                    var node=top.Nodes.Add(project.LoadError.Length>0 ? project.Name+"（无法加载："+project.LoadError+"）" : label); node.Tag=project.Path;
                    foreach(string file in project.Files) { string relative=file.StartsWith(Path.GetDirectoryName(project.Path)+"\\",StringComparison.OrdinalIgnoreCase) ? file.Substring(Path.GetDirectoryName(project.Path).Length+1) : file; var child=node.Nodes.Add(relative); child.Tag=file; }
                    if(project.ProjectReferences.Count+project.AssemblyReferences.Count+project.UnresolvedReferences.Count>0) {
                        var references=node.Nodes.Add("引用");
                        foreach(string id in project.ProjectReferences) { var target=Workspace.Project(id); references.Nodes.Add("项目："+(target==null ? id : target.Name)).Tag=target==null ? null : target.Path; }
                        foreach(string missing in project.UnresolvedReferences) references.Nodes.Add("缺失："+missing);
                        foreach(string assembly in project.AssemblyReferences) references.Nodes.Add(assembly);
                    }
                    node.Expand();
                }
                top.Expand();
            } finally { tree.EndUpdate(); }
        }
        internal bool Save(EditorPage page,bool saveAs)
        {
            if(page==null) return true; string path=page.Document.PathName;
            // Code written for an unsaved form is saved with that form, next to it.
            if(path==null && page.CodeBar!=null && page.CodeBar.LayoutPage!=null && !page.CodeBar.LayoutPage.IsDisposed && page.CodeBar.LayoutPage.Document.PathName==null) return Save(page.CodeBar.LayoutPage,false);
            if(saveAs || path==null) using(var dialog=new SaveFileDialog { FileName=path==null ? page.UntitledName : Path.GetFileName(path), InitialDirectory=path==null ? workspace : Path.GetDirectoryName(path), Filter="所有文件|*.*", OverwritePrompt=true }) { if(dialog.ShowDialog(this)!=DialogResult.OK) return false; return SaveTo(page,dialog.FileName); }
            return SaveTo(page,null);
        }
        // path == null saves in place through the kernel's file.save command; otherwise "save as".
        // The kernel refuses targets open in another tab and files changed on disk since they were read.
        internal bool SaveTo(EditorPage page,string path)
        {
            page.Document.Save(path); page.SetupDesigner(); RegenerateAfterSave(page); page.UpdateTitle(); status.Text="已保存 "+Path.GetFileName(page.Document.PathName); RefreshTree(); UpdateState(); return true;
        }
        bool SaveAll() { foreach(var page in Pages.ToList()) if((page.Dirty || page.Document.PathName==null) && !Save(page,false)) return false; return true; }
        // Returns the close policy the user chose, or null to cancel.
        ClosePolicy? ConfirmClose(EditorPage page)
        {
            if(!page.Dirty) return ClosePolicy.RefuseDirty;
            var result=MessageBox.Show(this,"保存对 "+page.Text.TrimEnd(' ','*')+" 的更改？","OpenMSE",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question);
            if(result==DialogResult.No) return ClosePolicy.Discard;
            if(result==DialogResult.Yes && Save(page,false)) return ClosePolicy.RefuseDirty;
            return null;
        }
        internal void ClosePage(EditorPage page,ClosePolicy policy) { page.Document.Close(policy); Tabs.TabPages.Remove(page); page.Dispose(); RefreshTree(); UpdateState(); }
        void CloseActive() { if(Active!=null) CloseDocument(Active); }
        void OnClosing(object sender,FormClosingEventArgs e) { if(BuildRunning) { MessageBox.Show(this,"生成正在进行，请等待完成后关闭。","OpenMSE"); e.Cancel=true; return; } try { foreach(var page in Pages.ToList()) if(ConfirmClose(page)==null) { e.Cancel=true; return; } } catch(Exception ex) { e.Cancel=true; MessageBox.Show(this,ex.Message,"OpenMSE"); } }
        void UpdatePosition() { if(Active==null) { position.Text=""; return; } int index=Active.Editor.SelectionStart, line=Active.Editor.GetLineFromCharIndex(index); position.Text="行 "+(line+1)+", 列 "+(index-Active.Editor.GetFirstCharIndexFromLine(line)+1)+"    |    "+(Active.Document.EncodingName ?? "").ToUpperInvariant()+"    |    版本 "+Active.Document.Revision; }
        void UpdateState() { Text=(Active==null ? "" : Active.Text+" — ")+"OpenMSE"+(Workspace==null ? "" : " · "+Path.GetFileNameWithoutExtension(Workspace.Path));
            if(buildItem!=null) { buildItem.Enabled=buildButton.Enabled=runItem.Enabled=runButton.Enabled=Workspace!=null && !BuildRunning; cancelItem.Enabled=cancelButton.Enabled=BuildRunning; configurationBox.Enabled=Workspace!=null && !BuildRunning; }
            UpdateDebugState();
            if(undoItem!=null) { undoItem.Enabled=undoButton.Enabled=Active!=null && Kernel.Query("edit.undo",Active.Document.Id).Enabled; redoItem.Enabled=redoButton.Enabled=Active!=null && Kernel.Query("edit.redo",Active.Document.Id).Enabled; } toolbox.SetContext(Active==null ? null : Active.Designer); properties.SelectedObject=Active==null || Active.Designer==null ? null : Active.Designer.SelectionObject; UpdatePosition(); }
        internal static string Quote(string value) { var result=new StringBuilder("\""); int slashes=0; foreach(char c in value) { if(c=='\\') { slashes++; continue; } result.Append('\\',slashes*(c=='"' ? 2 : 1)); slashes=0; if(c=='"') result.Append('\\'); result.Append(c); } result.Append('\\',slashes*2); return result.Append('"').ToString(); }
        // Builds run as kernel tasks: the call returns at once, output and the final state arrive as events.
        internal void StartBuild()
        {
            if(Workspace==null || BuildRunning || !SaveAll()) return;
            LastBuildExitCode=null; horizontal.Panel2Collapsed=false; outputTabs.SelectedIndex=0; output.Clear(); errors.Items.Clear();
            currentTask=Kernel.StartBuild(Workspace.Id,Configuration,null);
            output.AppendText("生成："+Workspace.Path+"  ["+Configuration+"]\r\n"+currentTask.Command+"\r\n\r\n"); status.Text="正在生成…"; UpdateState();
        }
        internal void BuildAndRun() { runAfterBuild=true; try { StartBuild(); } finally { if(!BuildRunning) runAfterBuild=false; } }
        internal void StartRun()
        {
            if(Workspace==null || BuildRunning) return;
            LastRunExitCode=null; horizontal.Panel2Collapsed=false; outputTabs.SelectedIndex=0;
            currentTask=Kernel.StartRun(Workspace.Id,Configuration,null);
            output.AppendText("\r\n运行："+currentTask.Command+"\r\n"); status.Text="正在运行…"; UpdateState();
        }
        internal void CancelTask() { if(BuildRunning) { currentTask=Kernel.CancelTask(currentTask.Id); status.Text="正在取消…"; } }
        void TaskFinished(KernelTask task)
        {
            if(currentTask==null || task.Id!=currentTask.Id) return;
            currentTask=task; LastTask=task;
            string state=task.State=="succeeded" ? "成功" : task.State=="cancelled" ? "已取消" : "失败";
            if(task.Kind=="build") {
                LastBuildExitCode=(int)(task.ExitCode ?? -1);
                output.AppendText("\r\n生成"+state+"：退出码 "+task.ExitCode+"，错误 "+task.Errors+"，警告 "+task.Warnings+"，用时 "+(task.DurationMs/1000.0).ToString("0.0")+" 秒\r\n");
                foreach(string artifact in task.Artifacts) output.AppendText("  产物："+artifact+"\r\n");
                errors.BeginUpdate(); errors.Items.Clear();
                foreach(var d in task.Diagnostics) { var item=new ListViewItem(new[]{ d.IsError ? "错误" : "警告", d.Code, d.Message, Path.GetFileName(d.File ?? ""), d.Line>0 ? d.Line.ToString() : "" }) { Tag=d, ForeColor=d.IsError ? Color.Firebrick : Color.DarkGoldenrod }; errors.Items.Add(item); }
                errors.EndUpdate();
                if(task.Errors>0) outputTabs.SelectedIndex=1;
                status.Text="生成"+state+(task.Errors>0 ? "，"+task.Errors+" 个错误" : "");
                bool run=runAfterBuild && task.State=="succeeded", debug=debugAfterBuild && task.State=="succeeded"; runAfterBuild=debugAfterBuild=false;
                UpdateState();
                if(run) Guard(StartRun);
                if(debug) Guard(BeginDebugSession);
            } else {
                LastRunExitCode=(int)(task.ExitCode ?? -1);
                output.AppendText("程序"+(task.State=="cancelled" ? "已终止" : "已退出")+"，退出码 "+task.ExitCode+"\r\n");
                status.Text="运行"+(task.State=="cancelled" ? "已终止" : "结束，退出码 "+task.ExitCode); UpdateState();
            }
        }
        // Opens the diagnostic's file at its line and column (one-based in compiler output).
        internal void GoToDiagnostic(int index)
        {
            var d=Diagnostics[index];
            if(String.IsNullOrEmpty(d.File) || !File.Exists(d.File)) throw new FileNotFoundException("该诊断没有可打开的源文件。",d.File);
            OpenFile(d.File); var page=Active; if(page==null) return;
            if(page.Views!=null) page.Views.SelectedIndex=1;
            string text=page.Document.Shadow; int line=Math.Max(0,d.Line-1), column=Math.Max(0,d.Column-1);
            int start=0; for(int l=0; l<line; l++) { int n=text.IndexOf('\n',start); if(n<0) break; start=n+1; }
            int end=text.IndexOf('\n',start); if(end<0) end=text.Length;
            page.Editor.Select(Math.Min(start+column,end),0); page.Editor.ScrollToCaret(); page.Editor.Focus(); UpdatePosition();
        }
        void ExportForm()
        {
            if(Active==null || Active.Designer==null || Active.Designer.Html) throw new InvalidOperationException("请先打开 Windows 窗体布局（.omform）。");
            Active.Designer.Reload();
            using(var dialog=new SaveFileDialog { Filter="C# 窗体源码|*.cs|Visual Basic 窗体源码|*.vb",FileName="DesignedForm.cs",OverwritePrompt=true }) {
                if(dialog.ShowDialog(this)!=DialogResult.OK) return;
                string value=Active.Designer.Export(Path.GetExtension(dialog.FileName).Equals(".vb",StringComparison.OrdinalIgnoreCase));
                if(Tabs.TabPages.OfType<EditorPage>().Any(p=>String.Equals(p.Document.PathName,dialog.FileName,StringComparison.OrdinalIgnoreCase))) throw new IOException("导出目标已在编辑器中打开，请选择其他文件。");
                File.WriteAllText(dialog.FileName,value,new UTF8Encoding(true)); status.Text="已导出窗体源码";
            }
        }
        void LaunchLegacy(bool vs8)
        {
            if(!SaveAll()) return;
            string path=vs8 && Workspace!=null ? Workspace.Path : Active==null ? null : Active.Document.PathName;
            Process.Start(new ProcessStartInfo(Path.Combine(root,"OpenMSE.exe"),(vs8 ? "--vs8" : "--mse")+(path==null ? "" : " "+Quote(path))) { WorkingDirectory=root, UseShellExecute=false, CreateNoWindow=true });
        }
    }
}
