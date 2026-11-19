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

    internal sealed class EditorPage : TabPage
    {
        internal readonly EditorDocument Document = new EditorDocument();
        internal readonly RichTextBox Editor = new RichTextBox { Dock=DockStyle.Fill, BorderStyle=BorderStyle.None, Font=new Font("Consolas", 11F), AcceptsTab=true, WordWrap=false, DetectUrls=false, HideSelection=false, BackColor=Color.White, ForeColor=Color.FromArgb(32,40,53), ScrollBars=RichTextBoxScrollBars.Both };
        internal readonly string UntitledName;
        internal DesignerView Designer;
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
                Designer=new DesignerView(html,delegate { return Editor.Text; },delegate(string text) { Editor.SelectAll(); Editor.SelectedText=text; UpdateTitle(); });
                designTab.Controls.Add(Designer); sourceTab.Controls.Add(Editor); Views.TabPages.Add(designTab); Views.TabPages.Add(sourceTab); Controls.Add(Views);
                Views.SelectedIndexChanged+=delegate { if(Views.SelectedIndex==0) Designer.Reload(); };
            }
            Designer.Reload();
        }
        internal bool Dirty { get { return EditorDocument.Normalize(Editor.Text)!=Document.Original; } }
        internal EditorPage(string name) { UntitledName=name; Padding=new Padding(12,10,0,0); BackColor=Color.White; Controls.Add(Editor); UpdateTitle(); }
        internal void UpdateTitle() { Text=(Document.PathName==null ? UntitledName : Path.GetFileName(Document.PathName))+(Dirty ? " *" : ""); ToolTipText=Document.PathName ?? UntitledName; }
    }

    internal sealed class MainForm : Form
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
        readonly string root;
        string solution, workspace;
        int untitled;
        Process build;
        internal bool BuildRunning { get { return build!=null; } }
        internal int? LastBuildExitCode;
        ToolStripMenuItem buildItem;
        ToolStripButton buildButton;
        internal MainForm(string projectRoot)
        {
            root=projectRoot; workspace=Path.Combine(root,"workspace"); toolbox=new ToolboxPanel(root);
            Text="OpenMSE"; Size=new Size(1360,860); MinimumSize=new Size(960,640); StartPosition=FormStartPosition.CenterScreen;
            Font=new Font("Microsoft YaHei UI",9F); BackColor=Color.White;
            Icon=Icon.ExtractAssociatedIcon(Path.Combine(root,"OpenMSE.exe"));
            var menu=new MenuStrip { BackColor=Color.White, Padding=new Padding(10,4,0,4) };
            var file=AddMenu(menu,"文件(&F)"); Item(file,"新建文件或项目…",Keys.Control|Keys.N,NewItem); Item(file,"打开文件…",Keys.Control|Keys.O,OpenDialog); Item(file,"打开项目或解决方案…",Keys.Control|Keys.Shift|Keys.O,OpenProjectDialog);
            file.DropDownItems.Add(new ToolStripSeparator()); Item(file,"保存",Keys.Control|Keys.S,delegate { Save(Active,false); }); Item(file,"另存为…",Keys.Control|Keys.Shift|Keys.S,delegate { Save(Active,true); }); Item(file,"全部保存",Keys.Control|Keys.Alt|Keys.S,delegate { SaveAll(); });
            Item(file,"关闭当前文档",Keys.Control|Keys.W,CloseActive); file.DropDownItems.Add(new ToolStripSeparator()); Item(file,"退出",Keys.Alt|Keys.F4,Close);
            var edit=AddMenu(menu,"编辑(&E)"); Item(edit,"撤销",Keys.Control|Keys.Z,delegate { if(Active!=null) { Active.Editor.Undo(); if(Active.Designer!=null) Active.Designer.Reload(); } }); Item(edit,"重做",Keys.Control|Keys.Y,delegate { if(Active!=null) { Active.Editor.Redo(); if(Active.Designer!=null) Active.Designer.Reload(); } }); edit.DropDownItems.Add(new ToolStripSeparator());
            Item(edit,"剪切",Keys.Control|Keys.X,delegate { if(Active!=null) Active.Editor.Cut(); }); Item(edit,"复制",Keys.Control|Keys.C,delegate { if(Active!=null) Active.Editor.Copy(); }); Item(edit,"粘贴",Keys.Control|Keys.V,delegate { if(Active!=null) Active.Editor.Paste(DataFormats.GetFormat(DataFormats.UnicodeText)); }); Item(edit,"全选",Keys.Control|Keys.A,delegate { if(Active!=null) Active.Editor.SelectAll(); });
            var view=AddMenu(menu,"视图(&V)"); Item(view,"项目资源管理器",Keys.None,delegate { right.Panel2Collapsed=!right.Panel2Collapsed; }); Item(view,"输出",Keys.Control|Keys.Alt|Keys.O,delegate { horizontal.Panel2Collapsed=!horizontal.Panel2Collapsed; }); Item(view,"自动换行",Keys.None,delegate { if(Active!=null) Active.Editor.WordWrap=!Active.Editor.WordWrap; });
            Item(view,"工具箱",Keys.Control|Keys.Alt|Keys.X,delegate { vertical.Panel1Collapsed=!vertical.Panel1Collapsed; });
            var project=AddMenu(menu,"项目(&P)"); Item(project,"新建项目…",Keys.None,NewItem); Item(project,"刷新项目树",Keys.F5,RefreshTree); Item(project,"导出窗体 C# / VB 源码…",Keys.None,ExportForm);
            var builds=AddMenu(menu,"生成(&B)"); buildItem=Item(builds,"生成解决方案",Keys.Control|Keys.Shift|Keys.B,StartBuild);
            var tools=AddMenu(menu,"工具(&T)"); Item(tools,"在 MSE 兼容窗口中打开",Keys.None,delegate { LaunchLegacy(false); }); Item(tools,"在 VS2005 兼容窗口中打开",Keys.None,delegate { LaunchLegacy(true); });
            var help=AddMenu(menu,"帮助(&H)"); Item(help,"关于 OpenMSE",Keys.None,delegate { MessageBox.Show(this,"OpenMSE\r\n版本 " + Application.ProductVersion + "\r\n\r\n移植自微软套件库的 Microsoft Script Editor。","关于 OpenMSE",MessageBoxButtons.OK,MessageBoxIcon.Information); });
            MainMenuStrip=menu;
            var toolbar=new ToolStrip { GripStyle=ToolStripGripStyle.Hidden, BackColor=Color.FromArgb(243,246,251), Padding=new Padding(12,5,6,5), ImageScalingSize=new Size(18,18) };
            Button(toolbar,"新建",SystemIcons.Application,NewItem); Button(toolbar,"打开",SystemIcons.WinLogo,OpenDialog); Button(toolbar,"保存",SystemIcons.Information,delegate { Save(Active,false); }); toolbar.Items.Add(new ToolStripSeparator());
            buildButton=Button(toolbar,"生成",SystemIcons.Shield,StartBuild); toolbar.Items.Add(new ToolStripSeparator()); toolbar.Items.Add(new ToolStripLabel("统一工作区") { ForeColor=Color.FromArgb(85,100,120) });
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
            horizontal.Panel1.Controls.Add(Tabs); horizontal.Panel2.Controls.Add(output); horizontal.Panel2.Controls.Add(PanelTitle("输出 · 生成"));
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
        static Image ToolIcon(string text)
        {
            var image=new Bitmap(20,20);
            using(var graphics=Graphics.FromImage(image)) using(var pen=new Pen(Color.FromArgb(45,86,139),1.6F)) {
                graphics.SmoothingMode=System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
                if(text=="新建") { graphics.DrawRectangle(pen,4,2,11,15); graphics.DrawLine(pen,8,7,17,7); graphics.DrawLine(pen,13,3,13,12); }
                else if(text=="打开") { graphics.DrawLines(pen,new[]{new Point(2,16),new Point(2,5),new Point(8,5),new Point(10,7),new Point(17,7)}); graphics.DrawPolygon(pen,new[]{new Point(2,16),new Point(5,9),new Point(18,9),new Point(15,16)}); }
                else if(text=="保存") { graphics.DrawRectangle(pen,3,2,14,15); graphics.DrawRectangle(pen,6,2,8,5); graphics.DrawRectangle(pen,6,11,8,6); }
                else { using(var brush=new SolidBrush(Color.FromArgb(38,135,92))) graphics.FillPolygon(brush,new[]{new Point(5,3),new Point(17,10),new Point(5,17)}); }
            }
            return image;
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
            var page=new EditorPage("未命名"+(++untitled)+template.Extension); AddEditor(page); page.Editor.Text=template.Content; page.SetupDesigner(); page.UpdateTitle(); RefreshTree(); UpdateState(); return page;
        }
        void AddEditor(EditorPage page) { Tabs.TabPages.Add(page); Tabs.SelectedTab=page; page.Editor.TextChanged+=delegate { page.UpdateTitle(); UpdateState(); }; page.Editor.SelectionChanged+=delegate { UpdatePosition(); }; page.SetupDesigner(); if(page.Designer!=null) { page.Designer.SelectionChanged=delegate(object selected) { if(Active==page) properties.SelectedObject=selected; }; page.Designer.Failure=toolbox.Failure; } page.Editor.Focus(); RefreshTree(); }
        internal void OpenFile(string path)
        {
            if(String.Equals(Path.GetExtension(path),".sln",StringComparison.OrdinalIgnoreCase) || String.Equals(Path.GetExtension(path),".csproj",StringComparison.OrdinalIgnoreCase) || String.Equals(Path.GetExtension(path),".vbproj",StringComparison.OrdinalIgnoreCase)) { OpenProject(path); return; }
            path=Path.GetFullPath(path);
            foreach(EditorPage existing in Tabs.TabPages.OfType<EditorPage>()) if(String.Equals(existing.Document.PathName,path,StringComparison.OrdinalIgnoreCase)) { Tabs.SelectedTab=existing; return; }
            if(new FileInfo(path).Length>8*1024*1024) throw new IOException("文本编辑器目前支持最大 8 MB 的文件。");
            var page=new EditorPage(Path.GetFileName(path)); page.Editor.Text=page.Document.Read(path); AddEditor(page); page.UpdateTitle(); UpdateState();
        }
        void OpenDialog() { using(var d=new OpenFileDialog { Filter="文本与源文件|*.txt;*.htm;*.html;*.js;*.vbs;*.cs;*.vb;*.cpp;*.h;*.xml;*.json;*.md|所有文件|*.*", Multiselect=true }) if(d.ShowDialog(this)==DialogResult.OK) foreach(string path in d.FileNames) OpenFile(path); }
        void OpenProjectDialog() { using(var d=new OpenFileDialog { Filter="项目与解决方案|*.sln;*.csproj;*.vbproj|所有文件|*.*" }) if(d.ShowDialog(this)==DialogResult.OK) OpenProject(d.FileName); }
        internal void OpenProject(string path) { if(build!=null) throw new InvalidOperationException("生成进行中，请等待生成完成。"); solution=Path.GetFullPath(path); workspace=Path.GetDirectoryName(solution); RefreshTree(); UpdateState(); }
        void RefreshTree()
        {
            tree.BeginUpdate(); try { tree.Nodes.Clear(); var node=tree.Nodes.Add(solution==null ? "其他文件" : Path.GetFileName(solution)); node.Tag=solution; if(solution!=null) AddFiles(node,workspace,0); else foreach(var page in Tabs.TabPages.OfType<EditorPage>()) { var child=node.Nodes.Add(page.Text); child.Tag=page; } node.Expand(); } finally { tree.EndUpdate(); }
        }
        void AddFiles(TreeNode parent,string directory,int depth)
        {
            if(depth>6) return;
            try {
                foreach(string dir in Directory.GetDirectories(directory).OrderBy(x=>x)) { string name=Path.GetFileName(dir); if(name=="bin" || name=="obj" || name.StartsWith(".")) continue; var node=parent.Nodes.Add(name); AddFiles(node,dir,depth+1); }
                foreach(string path in Directory.GetFiles(directory).OrderBy(x=>x).Take(250)) if(!path.EndsWith(".suo") && !path.EndsWith(".user")) { var node=parent.Nodes.Add(Path.GetFileName(path)); node.Tag=path; }
            } catch(UnauthorizedAccessException) { parent.Nodes.Add("无法访问此目录"); }
        }
        internal bool Save(EditorPage page,bool saveAs)
        {
            if(page==null) return true; string path=page.Document.PathName;
            if(saveAs || path==null) using(var dialog=new SaveFileDialog { FileName=path==null ? page.UntitledName : Path.GetFileName(path), InitialDirectory=path==null ? workspace : Path.GetDirectoryName(path), Filter="所有文件|*.*", OverwritePrompt=true }) { if(dialog.ShowDialog(this)!=DialogResult.OK) return false; path=dialog.FileName; }
            foreach(EditorPage other in Tabs.TabPages.OfType<EditorPage>()) if(other!=page && String.Equals(other.Document.PathName,path,StringComparison.OrdinalIgnoreCase)) throw new IOException("目标文件已在另一个标签中打开。");
            page.Document.Save(path,page.Editor.Text); page.SetupDesigner(); page.UpdateTitle(); status.Text="已保存 "+Path.GetFileName(path); RefreshTree(); UpdateState(); return true;
        }
        bool SaveAll() { foreach(var page in Tabs.TabPages.OfType<EditorPage>()) if((page.Dirty || page.Document.PathName==null) && !Save(page,false)) return false; return true; }
        bool ConfirmClose(EditorPage page) { if(!page.Dirty) return true; var result=MessageBox.Show(this,"保存对 "+page.Text.TrimEnd(' ','*')+" 的更改？","OpenMSE",MessageBoxButtons.YesNoCancel,MessageBoxIcon.Question); return result==DialogResult.No || (result==DialogResult.Yes && Save(page,false)); }
        void CloseActive() { if(Active!=null && ConfirmClose(Active)) { var page=Active; Tabs.TabPages.Remove(page); page.Dispose(); UpdateState(); } }
        void OnClosing(object sender,FormClosingEventArgs e) { if(build!=null) { MessageBox.Show(this,"生成正在进行，请等待完成后关闭。","OpenMSE"); e.Cancel=true; return; } try { foreach(var page in Tabs.TabPages.OfType<EditorPage>()) if(!ConfirmClose(page)) { e.Cancel=true; return; } } catch(Exception ex) { e.Cancel=true; MessageBox.Show(this,ex.Message,"OpenMSE"); } }
        void UpdatePosition() { if(Active==null) { position.Text=""; return; } int index=Active.Editor.SelectionStart, line=Active.Editor.GetLineFromCharIndex(index); position.Text="行 "+(line+1)+", 列 "+(index-Active.Editor.GetFirstCharIndexFromLine(line)+1)+"    |    "+Active.Document.Encoding.WebName.ToUpperInvariant(); }
        void UpdateState() { Text=(Active==null ? "" : Active.Text+" — ")+"OpenMSE"+(solution==null ? "" : " · "+Path.GetFileNameWithoutExtension(solution)); if(buildItem!=null) buildItem.Enabled=buildButton.Enabled=solution!=null && build==null; toolbox.SetContext(Active==null ? null : Active.Designer); properties.SelectedObject=Active==null || Active.Designer==null ? null : Active.Designer.SelectionObject; UpdatePosition(); }
        internal static string Quote(string value) { var result=new StringBuilder("\""); int slashes=0; foreach(char c in value) { if(c=='\\') { slashes++; continue; } result.Append('\\',slashes*(c=='"' ? 2 : 1)); slashes=0; if(c=='"') result.Append('\\'); result.Append(c); } result.Append('\\',slashes*2); return result.Append('"').ToString(); }
        internal void StartBuild()
        {
            if(solution==null || build!=null || !SaveAll()) return;
            string compiler=Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.Windows),@"Microsoft.NET\Framework\v2.0.50727\MSBuild.exe");
            if(!File.Exists(compiler)) throw new FileNotFoundException("未找到 .NET 2.0 构建工具。",compiler);
            LastBuildExitCode=null; horizontal.Panel2Collapsed=false; output.Clear(); output.AppendText("生成："+solution+"\r\n\r\n"); status.Text="正在生成…";
            var start=new ProcessStartInfo(compiler,Quote(solution)+" /t:Build /p:Configuration=Debug /nologo") { WorkingDirectory=workspace, UseShellExecute=false, CreateNoWindow=true, RedirectStandardOutput=true, RedirectStandardError=true, StandardOutputEncoding=Encoding.Default, StandardErrorEncoding=Encoding.Default };
            build=new Process { StartInfo=start, EnableRaisingEvents=true }; Process activeBuild=build;
            var complete=new System.Windows.Forms.Timer { Interval=150 };
            Action<string> append=delegate(string line) { if(line!=null && !IsDisposed && IsHandleCreated) BeginInvoke((Action)delegate { output.AppendText(line+"\r\n"); }); };
            activeBuild.OutputDataReceived+=delegate(object s,DataReceivedEventArgs e) { append(e.Data); }; activeBuild.ErrorDataReceived+=delegate(object s,DataReceivedEventArgs e) { append(e.Data); };
            complete.Tick+=delegate { if(!activeBuild.HasExited) return; complete.Stop(); complete.Dispose(); activeBuild.WaitForExit(); int code=activeBuild.ExitCode; LastBuildExitCode=code; output.AppendText("\r\n生成进程退出码："+code+"\r\n"); status.Text=code==0 ? "生成成功" : "生成失败，请查看输出"; activeBuild.Dispose(); build=null; UpdateState(); };
            try { activeBuild.Start(); activeBuild.BeginOutputReadLine(); activeBuild.BeginErrorReadLine(); complete.Start(); UpdateState(); }
            catch { complete.Dispose(); activeBuild.Dispose(); build=null; status.Text="生成未能启动"; UpdateState(); throw; }
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
            string path=vs8 && solution!=null ? solution : Active==null ? null : Active.Document.PathName;
            Process.Start(new ProcessStartInfo(Path.Combine(root,"OpenMSE.exe"),(vs8 ? "--vs8" : "--mse")+(path==null ? "" : " "+Quote(path))) { WorkingDirectory=root, UseShellExecute=false, CreateNoWindow=true });
        }
    }
}
