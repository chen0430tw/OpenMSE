using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Windows.Forms;

namespace OpenMSE.Shell
{
    // Own clones of framework images: the ToolboxBitmapAttribute images are shared.
    internal sealed class ToolboxIcons : IDisposable
    {
        readonly Dictionary<string,Bitmap> images=new Dictionary<string,Bitmap>();
        internal Bitmap Get(ToolboxEntry entry)
        {
            string key=entry.Family+"/"+entry.Name;
            Bitmap result;
            if(images.TryGetValue(key,out result)) return result;
            result=new Bitmap(16,16);
            Type type=typeof(Button).Assembly.GetType("System.Windows.Forms."+entry.ControlType);
            Image original=null;
            if(type!=null) {
                var attribute=TypeDescriptor.GetAttributes(type)[typeof(ToolboxBitmapAttribute)] as ToolboxBitmapAttribute;
                if(attribute!=null) original=attribute.GetImage(type,false);
            }
            using(var graphics=Graphics.FromImage(result)) {
                graphics.Clear(Color.Transparent);
                if(original!=null) graphics.DrawImageUnscaled(original,0,0);
                else using(var pen=new Pen(Color.FromArgb(51,90,140))) {
                    graphics.DrawRectangle(pen,1,2,13,11);
                    using(var font=new Font("Segoe UI",7F,FontStyle.Bold,GraphicsUnit.Pixel))
                        graphics.DrawString(entry.Name.Substring(0,1),font,Brushes.Navy,3,3);
                }
                if(entry.Family=="HTML") DecorateHtml(graphics,entry.Name);
            }
            images.Add(key,result);
            return result;
        }
        static void DecorateHtml(Graphics graphics,string name)
        {
            // Preserve the familiar native control glyph and distinguish HTML variants.
            if(name=="Reset Button" || name=="Submit Button" || name=="File Field") {
                graphics.FillRectangle(Brushes.White,7,6,9,10);
                using(var pen=new Pen(name=="Reset Button" ? Color.FromArgb(207,130,35) : Color.FromArgb(29,124,79),1.5F)) {
                    if(name=="Reset Button") { graphics.DrawArc(pen,8,8,6,6,0,275); graphics.DrawLines(pen,new[]{new Point(8,7),new Point(8,11),new Point(12,11)}); }
                    else if(name=="Submit Button") { graphics.DrawLine(pen,8,11,14,11); graphics.DrawLines(pen,new[]{new Point(11,8),new Point(14,11),new Point(11,14)}); }
                    else { graphics.FillRectangle(Brushes.Khaki,8,9,7,5); graphics.DrawRectangle(pen,8,9,7,5); graphics.DrawLine(pen,8,8,11,8); }
                }
            } else if(name=="Password Field") {
                graphics.FillRectangle(Brushes.White,2,5,12,7);
                using(var font=new Font("Consolas",9F,FontStyle.Bold,GraphicsUnit.Pixel)) graphics.DrawString("***",font,Brushes.Black,1,4);
            } else if(name=="Horizontal Rule") {
                graphics.Clear(Color.Transparent);
                using(var pen=new Pen(Color.FromArgb(57,81,113),2)) graphics.DrawLine(pen,1,8,14,8);
            } else if(name=="Grid Layout Panel") {
                using(var pen=new Pen(Color.FromArgb(48,104,164))) { graphics.DrawLine(pen,6,3,6,13); graphics.DrawLine(pen,10,3,10,13); graphics.DrawLine(pen,2,6,14,6); graphics.DrawLine(pen,2,10,14,10); }
            } else if(name=="Linear Layout Panel") {
                using(var brush=new SolidBrush(Color.FromArgb(48,104,164))) { graphics.FillRectangle(brush,3,5,3,6); graphics.FillRectangle(brush,8,5,3,6); }
            }
        }
        internal void Draw(Graphics graphics,ToolboxEntry entry,Rectangle bounds,bool selected)
        {
            Bitmap image=Get(entry);
            if(selected) {
                using(var brush=new SolidBrush(Color.FromArgb(248,250,253))) graphics.FillRectangle(brush,bounds.X-1,bounds.Y-1,bounds.Width+2,bounds.Height+2);
            }
            var interpolation=graphics.InterpolationMode; var pixels=graphics.PixelOffsetMode;
            try {
                graphics.InterpolationMode=InterpolationMode.NearestNeighbor;
                graphics.PixelOffsetMode=PixelOffsetMode.Half;
                graphics.DrawImage(image,bounds,0,0,image.Width,image.Height,GraphicsUnit.Pixel);
            } finally { graphics.InterpolationMode=interpolation; graphics.PixelOffsetMode=pixels; }
        }
        public void Dispose() { foreach(var image in images.Values) image.Dispose(); images.Clear(); }
    }
}
