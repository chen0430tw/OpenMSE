using System;
using System.Windows.Forms;
using System.Drawing;

namespace $safeprojectname$.UI
{
    /// <summary>
    /// 对呈现项的说明的过程进行封装。
    /// </summary>
    /// <typeparam name="T">此 ItemDescriptionView 将绘制的项的类型。</typeparam>
    public class ItemDescriptionView<T> : IDisposable where T : IItem
    {
        private Point location;
        private Size size;

        private static Brush textDrawingBrush = Brushes.Black;
        private Color lineColor;
        private float lineWidth;
        private Rectangle textRect;
        private Timer fadeTimer;
        private Color foreColor;
        private Font titleFont;
        private T displayItem;

        // 初始的 alpha 值及该值每次的更改量
        private int textAlpha = 0;
        private int textAlphaDelta = 4;
        private int textAlphaMax = 200;

        public T DisplayItem { get { return displayItem; } set { displayItem = value; } }
        public Point Location { get { return location; } set { location = value; } }
        public Size Size { get { return size; } set { size = value; } }
        public Color ForeColor { get { return foreColor; } set { foreColor = value; } }
        public Color LineColor { get { return lineColor; } set { lineColor = value; } }
        public Font TitleFont { get { return titleFont; } set { titleFont = value; } }
        public float LineWidth { get { return lineWidth; } set { lineWidth = value; } }
        public Timer FadeTimer { get { return fadeTimer; } }

        public event EventHandler FadingComplete;

        /// <summary>
        /// 创建一个连接到 <paramref name="listView"/> 的新 ItemDescriptionView。
        /// </summary>
        /// <param name="listView"></param>
        public ItemDescriptionView()
        {
            fadeTimer = new Timer();
            fadeTimer.Tick += new EventHandler(scrollTimer_Tick);
            fadeTimer.Enabled = true;
            fadeTimer.Start();
        }

        public void Paint(PaintEventArgs e)
        {
            // 更改图形设置以绘制清晰的文本
            e.Graphics.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            // 确定位于文本上方和下方的
            // 线条的位置
            float lineLeftX = Size.Width / 4;
            float lineRightX = 3 * Size.Width / 4;
            int lineVerticalBuffer = Size.Height / 50;
            float lineTopY = Location.Y + lineVerticalBuffer;
            float lineBottomY = Location.Y + Size.Height - lineVerticalBuffer;

            // 绘制两条线
            using (Pen linePen = new Pen(lineColor, lineWidth))
            {
                e.Graphics.DrawLine(linePen, Location.X + lineLeftX, lineTopY, Location.X + lineRightX, lineTopY);
                e.Graphics.DrawLine(linePen, Location.X + lineLeftX, lineBottomY, Location.X + lineRightX, lineBottomY);
            }

            // 绘制文章的文本
            using(StringFormat textFormat = new StringFormat(StringFormatFlags.LineLimit))
            {
                textFormat.Alignment = StringAlignment.Near;
                textFormat.LineAlignment = StringAlignment.Near;
                textFormat.Trimming = StringTrimming.EllipsisWord;
                int textVerticalBuffer = 4 * lineVerticalBuffer;
                textRect = new Rectangle(Location.X, Location.Y + textVerticalBuffer, Size.Width, Size.Height - (2 * textVerticalBuffer));
                using (Brush textBrush = new SolidBrush(Color.FromArgb(textAlpha, ForeColor)))
                {
                    e.Graphics.DrawString(displayItem.Description, titleFont, textBrush, textRect, textFormat);
                }
            }
        }

        private void scrollTimer_Tick(object sender, EventArgs e)
        {
            // 更改要绘制的文本的 alpha 值
            // 逐步增加值，直至达到 textAlphaMax，然后再逐步减小值
            // 当值重新为零时移动到下一文章
            textAlpha += textAlphaDelta;
            if (textAlpha >= textAlphaMax)
            {
                textAlphaDelta *= -1;
            }
            else if (textAlpha <= 0)
            {
                FadingComplete(this, new EventArgs());
                textAlpha = 0;
                textAlphaDelta *= -1;
            }
        }


        /// <summary>
        /// 释放所有不再需要的字段
        /// </summary>
        public void Dispose()
        {
            fadeTimer.Dispose();
        }
    }
}
