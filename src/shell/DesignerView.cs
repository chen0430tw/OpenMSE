using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    // Renders the kernel's design snapshot and turns gestures into kernel design operations.
    // It owns no model: drag previews stay local until mouse-up, then one operation is committed.
    internal sealed class DesignerView : UserControl
    {
        readonly IDesignSource source;
        readonly Panel scroll=new Panel { Dock=DockStyle.Fill,AutoScroll=true,BackColor=Color.FromArgb(235,239,245) };
        internal readonly Panel Canvas=new Panel { Location=new Point(20,20),Size=new Size(720,480),BackColor=Color.White,AllowDrop=true };
        readonly Label message=new Label { Dock=DockStyle.Top,Height=30,Padding=new Padding(8,6,0,0),BackColor=Color.FromArgb(246,248,252),ForeColor=Color.FromArgb(88,104,126) };
        readonly List<DesignSlot> slots=new List<DesignSlot>();
        internal readonly bool Html;
        internal DesignItem Selected;
        internal string ErrorMessage;
        KernelDesign design;
        FormProperties formProperties;
        internal object SelectionObject { get { return Selected!=null ? (object)Selected : Html ? null : formProperties; } }
        internal IList<DesignItem> Items { get { return design==null || ErrorMessage!=null ? (IList<DesignItem>)new DesignItem[0] : design.Nodes; } }
        internal ulong Revision { get { return design==null ? 0 : design.Revision; } }
        internal Action<object> SelectionChanged;
        internal Action<string> Failure;
        internal DesignerView(bool isHtml,IDesignSource designSource)
        {
            Html=isHtml; source=designSource; Dock=DockStyle.Fill; TabStop=true;
            scroll.Controls.Add(Canvas); Controls.Add(scroll); Controls.Add(message);
            Canvas.Paint+=delegate(object sender,PaintEventArgs e) { using(var pen=new Pen(Color.FromArgb(232,235,240))) for(int x=10;x<Canvas.Width;x+=10) for(int y=10;y<Canvas.Height;y+=10) e.Graphics.DrawRectangle(pen,x,y,1,1); };
            Canvas.MouseDown+=delegate { Select(null); Focus(); };
            Canvas.DragEnter+=delegate(object sender,DragEventArgs e) { var entry=e.Data.GetData(typeof(ToolboxEntry)) as ToolboxEntry; e.Effect=entry!=null && (entry.Family=="HTML")==Html ? DragDropEffects.Copy : DragDropEffects.None; };
            Canvas.DragDrop+=delegate(object sender,DragEventArgs e) { Try(delegate { Insert((ToolboxEntry)e.Data.GetData(typeof(ToolboxEntry)),Canvas.PointToClient(new Point(e.X,e.Y))); }); };
            KeyDown+=delegate(object sender,KeyEventArgs e) { if(e.KeyCode==Keys.Delete) { Try(RemoveSelected); e.Handled=true; } };
        }
        void Try(Action action) { try { action(); } catch(Exception ex) { if(Failure!=null) Failure(ex.Message); } }

        internal void Reload() { Show(null,null); }
        // Rebuilds the canvas from a snapshot (loading one when null). Selection follows `node`, else the previous Name.
        void Show(KernelDesign snapshot,string node)
        {
            string selectedName=Selected==null ? null : Selected.Name;
            foreach(Control child in Canvas.Controls.Cast<Control>().ToArray()) child.Dispose(); slots.Clear(); Selected=null; ErrorMessage=null; formProperties=null;
            try {
                design=snapshot ?? source.LoadDesign();
                if(design.Error!=null) throw new InvalidOperationException(design.Error);
                Canvas.Size=new Size(design.CanvasWidth,design.CanvasHeight);
                if(Html) message.Text="HTML 布局设计 · 拖放控件，拖动调整位置，右下角调整大小"+(design.Truncated ? " · 仅显示前 200 个元素" : "");
                else { formProperties=new FormProperties(design.Title,design.Width,design.Height); message.Text=design.Title+" · Windows 窗体布局 · 双击工具箱添加控件"; }
                foreach(var item in design.Nodes) {
                    var slot=new DesignSlot(this,item); slots.Add(slot); Canvas.Controls.Add(slot);
                    if(node!=null ? item.NodeId==node : item.Name==selectedName) Selected=item;
                }
                if(slots.Count==0) {
                    var hint=new Label { Text="将左侧工具箱中的控件拖放到这里\r\n\r\n也可以双击控件，随后在右侧修改属性。",AutoSize=false,Size=new Size(440,70),Location=new Point(80,150),ForeColor=Color.FromArgb(145,151,160),TextAlign=ContentAlignment.MiddleCenter };
                    hint.MouseDown+=delegate { Select(null); }; Canvas.Controls.Add(hint); hint.SendToBack();
                }
                Canvas.Enabled=true;
            } catch(Exception ex) { ErrorMessage=ex.Message; message.Text="无法加载设计视图："+ex.Message; Canvas.Enabled=false; }
            if(Selected==null && ErrorMessage==null && Items.Count>0) Selected=Items[0];
            Select(Selected);
        }
        internal void Select(DesignItem item) { Selected=item; foreach(var slot in slots) slot.Invalidate(); if(SelectionChanged!=null) SelectionChanged(SelectionObject); }

        // Sends one operation against the revision this view shows. A newer source revision is a conflict:
        // the view reloads and the operation is not applied.
        void Commit(Dictionary<string,object> operation)
        {
            if(ErrorMessage!=null) throw new InvalidOperationException(ErrorMessage);
            KernelDesignResult result;
            try { result=source.ApplyDesign(operation,design.Revision); }
            catch(KernelException ex) {
                Reload();
                if(ex.Code=="conflict") throw new InvalidOperationException("源码已变化，设计视图已重新加载，请重试。",ex);
                throw;
            }
            Show(result.Design,result.Node);
        }
        internal void Insert(ToolboxEntry entry,Point point)
        {
            point=new Point(Math.Max(0,point.X/10*10),Math.Max(0,point.Y/10*10));
            Commit(new Dictionary<string,object> { {"op","add"}, {"toolbox_id",entry.Id}, {"x",point.X}, {"y",point.Y} });
        }
        internal void CommitSelected()
        {
            try {
                Dictionary<string,object> changes;
                if(Selected==null) {
                    if(formProperties==null) return;
                    changes=formProperties.Changes(); if(changes.Count>0) Commit(new Dictionary<string,object> { {"op","set_form"}, {"properties",changes} });
                    return;
                }
                changes=Selected.Changes(); if(changes.Count>0) Commit(new Dictionary<string,object> { {"op","set"}, {"node",Selected.NodeId}, {"properties",changes} });
            } catch { Reload(); throw; }
        }
        internal void RemoveSelected()
        {
            if(Selected==null) return;
            string node=Selected.NodeId; Selected=null;
            Commit(new Dictionary<string,object> { {"op","delete"}, {"node",node} });
        }
        internal string Export(bool vb) { if(Html) throw new InvalidOperationException("HTML 页面直接保存为 .html；窗体源码导出适用于 .omform 布局。"); return source.ExportDesign(vb); }

        sealed class DesignSlot : Control
        {
            readonly DesignerView owner;
            readonly DesignItem item;
            Bitmap preview;
            Point mouseStart,origin;
            Size originalSize;
            bool dragging,resizing;
            internal DesignSlot(DesignerView parent,DesignItem model)
            {
                owner=parent; item=model; Bounds=new Rectangle(item.X,item.Y,item.Width,item.Height); TabStop=true;
                SetStyle(ControlStyles.UserPaint|ControlStyles.AllPaintingInWmPaint|ControlStyles.OptimizedDoubleBuffer,true);
                try {
                    var entry=new ToolboxEntry { ControlType=item.Kind };
                    using(Control sample=entry.CreateControl()) {
                        sample.Size=Size; sample.Text=item.Text; sample.BackColor=ColorTranslator.FromHtml(item.BackColor); sample.ForeColor=ColorTranslator.FromHtml(item.ForeColor);
                        if(sample is Label) ((Label)sample).TextAlign=ContentAlignment.MiddleLeft;
                        if(sample is TextBox && item.Password) ((TextBox)sample).UseSystemPasswordChar=true;
                        if(sample is Panel) ((Panel)sample).BorderStyle=BorderStyle.FixedSingle;
                        if(sample is PictureBox) ((PictureBox)sample).BorderStyle=BorderStyle.FixedSingle;
                        if(sample is TabControl) ((TabControl)sample).TabPages.Add("TabPage1");
                        if(sample is RichTextBox) { sample.Dispose(); preview=new Bitmap(Math.Max(1,Width),Math.Max(1,Height)); using(var g=Graphics.FromImage(preview)) { g.Clear(Color.White); g.DrawRectangle(Pens.Gray,0,0,Width-1,Height-1); g.DrawString(item.Text,SystemFonts.DefaultFont,Brushes.Black,new RectangleF(3,3,Width-6,Height-6)); } }
                        else { sample.CreateControl(); preview=new Bitmap(Math.Max(1,Width),Math.Max(1,Height)); sample.DrawToBitmap(preview,new Rectangle(Point.Empty,Size)); }
                    }
                } catch { if(preview!=null) preview.Dispose(); preview=null; }
                MouseDown+=delegate(object sender,MouseEventArgs e) { owner.Select(item); Focus(); if(e.Button!=MouseButtons.Left) return; mouseStart=MousePosition; origin=Location; originalSize=Size; resizing=e.X>=Width-12 && e.Y>=Height-12; dragging=true; Capture=true; };
                MouseMove+=delegate(object sender,MouseEventArgs e) { if(!dragging) { Cursor=e.X>=Width-12 && e.Y>=Height-12 ? Cursors.SizeNWSE : Cursors.SizeAll; return; } Point delta=new Point(MousePosition.X-mouseStart.X,MousePosition.Y-mouseStart.Y); if(resizing) Size=new Size(Math.Max(12,Math.Min(4096,(originalSize.Width+delta.X)/10*10)),Math.Max(12,Math.Min(4096,(originalSize.Height+delta.Y)/10*10))); else Location=new Point(Math.Max(0,Math.Min(10000,(origin.X+delta.X)/10*10)),Math.Max(0,Math.Min(10000,(origin.Y+delta.Y)/10*10))); Invalidate(); };
                // The drag preview is local; releasing the mouse commits one move/resize transaction.
                MouseUp+=delegate { if(!dragging) return; dragging=false; Capture=false; if(Location==origin && Size==originalSize) return; item.X=Left; item.Y=Top; item.Width=Width; item.Height=Height; owner.Try(owner.CommitSelected); };
                KeyDown+=delegate(object sender,KeyEventArgs e) { if(e.KeyCode==Keys.Delete) { owner.Try(owner.RemoveSelected); e.Handled=true; } };
            }
            protected override void OnPaint(PaintEventArgs e)
            {
                base.OnPaint(e); if(preview!=null) e.Graphics.DrawImage(preview,ClientRectangle); else { e.Graphics.Clear(Color.WhiteSmoke); e.Graphics.DrawString(item.Text,Font,Brushes.Black,ClientRectangle); }
                if(owner.Selected==item) { using(var pen=new Pen(Color.FromArgb(42,108,198),2)) e.Graphics.DrawRectangle(pen,1,1,Width-3,Height-3); e.Graphics.FillRectangle(Brushes.RoyalBlue,Width-8,Height-8,7,7); }
            }
            protected override void Dispose(bool disposing) { if(disposing && preview!=null) preview.Dispose(); base.Dispose(disposing); }
        }
    }
}
