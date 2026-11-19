using System;
using System.Collections.Generic;
using System.Drawing;
using System.IO;
using System.Linq;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    internal sealed class DesignerView : UserControl
    {
        readonly Func<string> read;
        readonly Action<string> write;
        readonly Panel scroll=new Panel { Dock=DockStyle.Fill,AutoScroll=true,BackColor=Color.FromArgb(235,239,245) };
        internal readonly Panel Canvas=new Panel { Location=new Point(20,20),Size=new Size(720,480),BackColor=Color.White,AllowDrop=true };
        readonly Label message=new Label { Dock=DockStyle.Top,Height=30,Padding=new Padding(8,6,0,0),BackColor=Color.FromArgb(246,248,252),ForeColor=Color.FromArgb(88,104,126) };
        readonly List<DesignSlot> slots=new List<DesignSlot>();
        internal readonly bool Html;
        internal DesignItem Selected;
        internal string ErrorMessage;
        internal object SelectionObject { get { return Selected!=null ? (object)Selected : Html ? null : form; } }
        internal IList<DesignItem> Items { get { return Html ? (IList<DesignItem>)html.Items : form.Controls; } }
        internal Action<object> SelectionChanged;
        internal Action<string> Failure;
        string loaded;
        HtmlLayout html;
        FormLayout form;
        internal DesignerView(bool isHtml,Func<string> get,Action<string> set)
        {
            Html=isHtml; read=get; write=set; Dock=DockStyle.Fill; TabStop=true;
            scroll.Controls.Add(Canvas); Controls.Add(scroll); Controls.Add(message);
            Canvas.Paint+=delegate(object sender,PaintEventArgs e) { using(var pen=new Pen(Color.FromArgb(232,235,240))) for(int x=10;x<Canvas.Width;x+=10) for(int y=10;y<Canvas.Height;y+=10) e.Graphics.DrawRectangle(pen,x,y,1,1); };
            Canvas.MouseDown+=delegate { Select(null); Focus(); };
            Canvas.DragEnter+=delegate(object sender,DragEventArgs e) { var entry=e.Data.GetData(typeof(ToolboxEntry)) as ToolboxEntry; e.Effect=entry!=null && (entry.Family=="HTML")==Html ? DragDropEffects.Copy : DragDropEffects.None; };
            Canvas.DragDrop+=delegate(object sender,DragEventArgs e) { Try(delegate { Insert((ToolboxEntry)e.Data.GetData(typeof(ToolboxEntry)),Canvas.PointToClient(new Point(e.X,e.Y))); }); };
            KeyDown+=delegate(object sender,KeyEventArgs e) { if(e.KeyCode==Keys.Delete) { Try(RemoveSelected); e.Handled=true; } };
        }
        void Try(Action action) { try { action(); } catch(Exception ex) { if(Failure!=null) Failure(ex.Message); } }
        internal void Reload()
        {
            string selected=Selected==null ? null : Selected.Name;
            foreach(Control child in Canvas.Controls.Cast<Control>().ToArray()) child.Dispose(); slots.Clear(); Selected=null; ErrorMessage=null;
            try {
                loaded=read(); IEnumerable<DesignItem> items;
                if(Html) { html=new HtmlLayout(loaded); items=html.Items.Take(200); Canvas.Size=new Size(900,650); message.Text="HTML 布局设计 · 拖放控件，拖动调整位置，右下角调整大小"; }
                else { form=FormLayout.Parse(loaded); items=form.Controls; Canvas.Size=new Size(form.Width,form.Height); message.Text=form.Title+" · Windows 窗体布局 · 双击工具箱添加控件"; }
                foreach(var item in items) {
                    var slot=new DesignSlot(this,item); slots.Add(slot); Canvas.Controls.Add(slot);
                    if(item.Name==selected) Selected=item;
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
        void CheckSource() { if(ErrorMessage!=null) throw new InvalidOperationException(ErrorMessage); if(read()!=loaded) { Reload(); throw new InvalidOperationException("源码已变化，设计视图已重新加载，请重试。"); } }
        void Replace(string value,string selected)
        {
            write(value); if(Selected!=null) Selected.Name=selected; Reload();
        }
        internal void Insert(ToolboxEntry entry,Point point)
        {
            CheckSource(); if((entry.Family=="HTML")!=Html) throw new InvalidOperationException("请在对应的 HTML 页面或 Windows 窗体布局中使用此控件。");
            point=new Point(Math.Max(0,point.X/10*10),Math.Max(0,point.Y/10*10));
            if(Html) Replace(html.Insert(entry,point),null);
            else {
                if(form.Controls.Count>=200) throw new InvalidOperationException("当前设计器最多支持 200 个控件。");
                string name; int index=1; do { name=entry.ControlType.ToLowerInvariant()+index++; } while(form.Controls.Any(c=>c.Name==name));
                var item=new DesignItem { Kind=entry.ControlType,Family="Windows Forms",Name=name,Text=entry.Name,X=point.X,Y=point.Y,Width=entry.ControlType=="Panel" || entry.ControlType=="GroupBox" ? 240 : 140,Height=entry.ControlType=="Panel" || entry.ControlType=="GroupBox" || entry.ControlType=="DataGridView" ? 120 : 32 };
                form.Controls.Add(item); Selected=item; Replace(form.Serialize(),name);
            }
            if(Html && html.Items.Count>0) Select(html.Items.Last());
        }
        internal void CommitSelected()
        {
            if(Selected==null) { if(!Html && form!=null) { CheckSource(); Replace(form.Serialize(),null); } return; }
            try { CheckSource(); Selected.Validate(); if(Html) Replace(html.Update(Selected),Selected.Name); else Replace(form.Serialize(),Selected.Name); }
            catch { Reload(); throw; }
        }
        internal void RemoveSelected()
        {
            if(Selected==null) return; CheckSource(); string value;
            if(Html) value=html.Delete(Selected); else { form.Controls.Remove(Selected); value=form.Serialize(); }
            Selected=null; Replace(value,null);
        }
        internal string Export(bool vb) { if(Html) throw new InvalidOperationException("HTML 页面直接保存为 .html；窗体源码导出适用于 .omform 布局。"); CheckSource(); return form.Generate(vb); }

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
                        if(sample is TextBox && owner.Html && HtmlLayout.Attribute(item.Token.Open,"type")=="password") ((TextBox)sample).UseSystemPasswordChar=true;
                        if(sample is Panel) ((Panel)sample).BorderStyle=BorderStyle.FixedSingle;
                        if(sample is PictureBox) ((PictureBox)sample).BorderStyle=BorderStyle.FixedSingle;
                        if(sample is TabControl) ((TabControl)sample).TabPages.Add("TabPage1");
                        if(sample is RichTextBox) { sample.Dispose(); preview=new Bitmap(Math.Max(1,Width),Math.Max(1,Height)); using(var g=Graphics.FromImage(preview)) { g.Clear(Color.White); g.DrawRectangle(Pens.Gray,0,0,Width-1,Height-1); g.DrawString(item.Text,SystemFonts.DefaultFont,Brushes.Black,new RectangleF(3,3,Width-6,Height-6)); } }
                        else { sample.CreateControl(); preview=new Bitmap(Math.Max(1,Width),Math.Max(1,Height)); sample.DrawToBitmap(preview,new Rectangle(Point.Empty,Size)); }
                    }
                } catch { if(preview!=null) preview.Dispose(); preview=null; }
                MouseDown+=delegate(object sender,MouseEventArgs e) { owner.Select(item); Focus(); if(e.Button!=MouseButtons.Left) return; mouseStart=MousePosition; origin=Location; originalSize=Size; resizing=e.X>=Width-12 && e.Y>=Height-12; dragging=true; Capture=true; };
                MouseMove+=delegate(object sender,MouseEventArgs e) { if(!dragging) { Cursor=e.X>=Width-12 && e.Y>=Height-12 ? Cursors.SizeNWSE : Cursors.SizeAll; return; } Point delta=new Point(MousePosition.X-mouseStart.X,MousePosition.Y-mouseStart.Y); if(resizing) Size=new Size(Math.Max(12,Math.Min(4096,(originalSize.Width+delta.X)/10*10)),Math.Max(12,Math.Min(4096,(originalSize.Height+delta.Y)/10*10))); else Location=new Point(Math.Max(0,Math.Min(10000,(origin.X+delta.X)/10*10)),Math.Max(0,Math.Min(10000,(origin.Y+delta.Y)/10*10))); Invalidate(); };
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
