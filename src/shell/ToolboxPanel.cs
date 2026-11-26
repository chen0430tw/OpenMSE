using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Web.Script.Serialization;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal sealed class ToolboxPanel : UserControl
    {
        readonly ComboBox category=new ComboBox { Dock=DockStyle.Top,DropDownStyle=ComboBoxStyle.DropDownList,Height=28 };
        readonly TextBox search=new TextBox { Dock=DockStyle.Top };
        readonly ListBox list=new ListBox { Dock=DockStyle.Fill,BorderStyle=BorderStyle.None,IntegralHeight=false,DrawMode=DrawMode.OwnerDrawFixed,ItemHeight=27,BackColor=Color.FromArgb(247,249,252) };
        readonly Label hint=new Label { Dock=DockStyle.Bottom,Height=52,Padding=new Padding(8),ForeColor=Color.FromArgb(100,112,130) };
        readonly IList<ToolboxEntry> entries;
        readonly ToolboxIcons icons=new ToolboxIcons();
        internal Func<DesignerView> CurrentDesigner;
        internal Action<string> Failure;
        internal Action NewHtml,NewForm;
        Point down;
        // Entries come from the kernel's ToolboxService; this panel only presents them.
        internal ToolboxPanel(IList<ToolboxEntry> catalog)
        {
            Dock=DockStyle.Fill; BackColor=Color.FromArgb(247,249,252);
            entries=catalog;
            category.Items.AddRange(new object[]{"HTML · MSE","Windows 窗体 · VS2005"});
            var shortcuts=new FlowLayoutPanel { Dock=DockStyle.Bottom,Height=42,Padding=new Padding(4),WrapContents=false };
            var html=new Button { Text="新建 HTML",AutoSize=true,Height=29 }; var form=new Button { Text="新建窗体",AutoSize=true,Height=29 };
            html.Click+=delegate { if(NewHtml!=null) NewHtml(); }; form.Click+=delegate { if(NewForm!=null) NewForm(); }; shortcuts.Controls.Add(html); shortcuts.Controls.Add(form);
            Controls.Add(list); Controls.Add(hint); Controls.Add(shortcuts); Controls.Add(search); Controls.Add(category);
            category.SelectedIndexChanged+=delegate { Filter(); }; search.TextChanged+=delegate { Filter(); };
            list.DrawItem+=delegate(object sender,DrawItemEventArgs e) {
                e.DrawBackground(); if(e.Index<0) return; var entry=(ToolboxEntry)list.Items[e.Index];
                bool selected=(e.State & DrawItemState.Selected)!=0;
                int size=Math.Max(16,Math.Min(e.Bounds.Height-6,(int)Math.Round(16*e.Graphics.DpiX/96F)));
                var bounds=new Rectangle(e.Bounds.X+8,e.Bounds.Y+(e.Bounds.Height-size)/2,size,size);
                icons.Draw(e.Graphics,entry,bounds,selected);
                int textLeft=bounds.Right+8;
                TextRenderer.DrawText(e.Graphics,entry.Name,Font,new Rectangle(textLeft,e.Bounds.Y,Math.Max(0,e.Bounds.Right-textLeft),e.Bounds.Height),selected ? SystemColors.HighlightText : list.ForeColor,TextFormatFlags.VerticalCenter|TextFormatFlags.EndEllipsis); e.DrawFocusRectangle();
            };
            list.MouseDown+=delegate(object sender,MouseEventArgs e) { down=e.Location; };
            list.MouseMove+=delegate(object sender,MouseEventArgs e) { if(e.Button!=MouseButtons.Left || list.SelectedItem==null || Math.Abs(e.X-down.X)+Math.Abs(e.Y-down.Y)<8) return; list.DoDragDrop(list.SelectedItem,DragDropEffects.Copy); };
            list.DoubleClick+=delegate { var entry=list.SelectedItem as ToolboxEntry; var designer=CurrentDesigner==null ? null : CurrentDesigner(); if(entry==null || designer==null) return; try { designer.Insert(entry,new Point(30+(designer.Canvas.Controls.Count%5)*20,30+(designer.Canvas.Controls.Count%8)*45)); } catch(Exception ex) { if(Failure!=null) Failure(ex.Message); } };
            category.SelectedIndex=0;
        }
        internal void SetContext(DesignerView view)
        {
            if(view!=null) { int target=view.Html ? 0 : 1; if(category.SelectedIndex!=target) category.SelectedIndex=target; }
            hint.Text=view==null ? "新建 HTML 或窗体布局即可使用工具箱。" : "双击添加，或拖放到设计画布。\r\n选中控件后在右侧编辑属性。";
        }
        protected override void Dispose(bool disposing) { if(disposing) icons.Dispose(); base.Dispose(disposing); }
        void Filter() { list.BeginUpdate(); list.Items.Clear(); foreach(var item in entries) if((item.Family=="HTML")== (category.SelectedIndex==0) && item.Name.IndexOf(search.Text,StringComparison.OrdinalIgnoreCase)>=0) list.Items.Add(item); list.EndUpdate(); }
    }
}
