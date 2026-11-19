using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    // A toolbox entry as described by the kernel's ToolboxService. Icons and preview controls stay in the UI.
    public sealed class ToolboxEntry
    {
        public string Id { get; set; }
        public string Name { get; set; }
        public string Family { get; set; }      // display label: "HTML" or "Windows Forms"
        public string Markup { get; set; }
        public string ControlType { get; set; }
        public string Backend { get; set; }
        public override string ToString() { return Name; }
        internal Control CreateControl()
        {
            if(!Allowed.Contains(ControlType)) throw new InvalidOperationException("不支持的控件类型："+ControlType);
            return (Control)Activator.CreateInstance(typeof(Button).Assembly.GetType("System.Windows.Forms."+ControlType,true));
        }
        internal static readonly HashSet<string> Allowed=new HashSet<string>(new[]{"Button","Label","TextBox","RichTextBox","CheckBox","RadioButton","ComboBox","ListBox","CheckedListBox","GroupBox","Panel","PictureBox","ProgressBar","TrackBar","NumericUpDown","DateTimePicker","MonthCalendar","TreeView","ListView","TabControl","LinkLabel","DataGridView"});
    }

    // Property-grid view of one kernel design node. Edits are sent back as a "set" operation containing
    // only the properties that differ from the snapshot the node was read from.
    public sealed class DesignItem
    {
        [Category("标识"), ReadOnly(true)] public string Kind { get; set; }
        [Category("标识")] public string Name { get; set; }
        [Category("外观")] public string Text { get; set; }
        [Category("布局")] public int X { get; set; }
        [Category("布局")] public int Y { get; set; }
        [Category("布局")] public int Width { get; set; }
        [Category("布局")] public int Height { get; set; }
        [Category("外观")] public string BackColor { get; set; }
        [Category("外观")] public string ForeColor { get; set; }
        [Browsable(false)] public bool Enabled { get; set; }
        internal string NodeId;
        internal bool TextEditable, Password;
        DesignItem original;

        internal static DesignItem From(Dictionary<string,object> n)
        {
            var item=new DesignItem { NodeId=(string)n["id"], Kind=(string)n["kind"], Name=(string)n["name"], Text=(string)n["text"], X=Convert.ToInt32(n["x"]), Y=Convert.ToInt32(n["y"]), Width=Convert.ToInt32(n["width"]), Height=Convert.ToInt32(n["height"]), BackColor=(string)n["back_color"], ForeColor=(string)n["fore_color"], Enabled=(bool)n["enabled"], TextEditable=(bool)n["text_editable"], Password=(bool)n["password"] };
            item.original=(DesignItem)item.MemberwiseClone();
            return item;
        }
        // Colour names are resolved by System.Drawing at preview and in exported code, so they are checked here.
        internal static void CheckColor(string value,string property)
        {
            try { ColorTranslator.FromHtml(value ?? ""); }
            catch(Exception) { throw new ArgumentException(property+" 不是有效的颜色。"); }
        }
        internal Dictionary<string,object> Changes()
        {
            CheckColor(BackColor,"BackColor"); CheckColor(ForeColor,"ForeColor");
            var p=new Dictionary<string,object>();
            if(Name!=original.Name) p["name"]=Name;
            if(Text!=original.Text) p["text"]=Text ?? "";
            if(X!=original.X) p["x"]=X;
            if(Y!=original.Y) p["y"]=Y;
            if(Width!=original.Width) p["width"]=Width;
            if(Height!=original.Height) p["height"]=Height;
            if(BackColor!=original.BackColor) p["back_color"]=BackColor ?? "";
            if(ForeColor!=original.ForeColor) p["fore_color"]=ForeColor ?? "";
            return p;
        }
    }

    // Property-grid view of the .omform root.
    public sealed class FormProperties
    {
        [Category("外观")] public string Title { get; set; }
        public int Width { get; set; }
        public int Height { get; set; }
        string title; int width, height;
        internal FormProperties(string t,int w,int h) { Title=title=t; Width=width=w; Height=height=h; }
        internal Dictionary<string,object> Changes()
        {
            var p=new Dictionary<string,object>();
            if(Title!=title) p["title"]=Title ?? "";
            if(Width!=width) p["width"]=Width;
            if(Height!=height) p["height"]=Height;
            return p;
        }
    }

    internal sealed class KernelDesign
    {
        internal ulong Document, Revision;
        internal string Kind, Error, Title;
        internal int Width, Height, CanvasWidth, CanvasHeight;
        internal bool Truncated;
        internal List<DesignItem> Nodes=new List<DesignItem>();
        internal static KernelDesign From(Dictionary<string,object> r)
        {
            var d=new KernelDesign { Document=KernelClient.U64(r["document"]), Revision=KernelClient.U64(r["revision"]), Kind=(string)r["kind"], Error=r["error"] as string, Title=(string)r["title"], Width=Convert.ToInt32(r["width"]), Height=Convert.ToInt32(r["height"]), CanvasWidth=Convert.ToInt32(r["canvas_width"]), CanvasHeight=Convert.ToInt32(r["canvas_height"]), Truncated=(bool)r["truncated"] };
            foreach(object node in (System.Collections.IEnumerable)r["nodes"]) d.Nodes.Add(DesignItem.From((Dictionary<string,object>)node));
            return d;
        }
    }

    internal sealed class KernelDesignResult
    {
        internal KernelState State;
        internal string Node;
        internal KernelDesign Design;
    }

    // What a designer view needs from its document; implemented by EditorPage over the kernel.
    internal interface IDesignSource
    {
        KernelDesign LoadDesign();
        KernelDesignResult ApplyDesign(Dictionary<string,object> operation,ulong expectedRevision);
        string ExportDesign(bool vb);
    }
}
