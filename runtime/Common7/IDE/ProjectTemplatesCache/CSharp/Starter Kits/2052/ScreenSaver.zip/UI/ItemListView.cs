using System;
using System.Drawing;
using System.Windows.Forms;
using System.Collections.Generic;

namespace $safeprojectname$.UI
{
    /// <summary>
    /// 封装项列表的呈现方式。每个项的说明都显示在列表中，并且已经选中了其中的一个项。
    /// </summary>
    /// <typeparam name="T">此 ItemListView 将要绘制的项的类型。</typeparam>
    public class ItemListView<T> : IDisposable where T : IItem
    {
        private const float percentOfArticleDisplayBoxToFillWithText = 0.5f;
        private const float percentOfFontHeightForSelectionBox = 1.5f;
        private const int padding = 20;

        // 绘制的位置
        private Point location;
        private Size size;

        private string title;
        private Font itemFont;
        private Font titleFont;
        private Color backColor;
        private Color borderColor;
        private Color foreColor;
        private Color titleBackColor;
        private Color titleForeColor;
        private Color selectedForeColor;
        private Color selectedBackColor;
        private float itemFontHeight;
        // 当前选中的项的索引
        private int selectedIndex = 0;
        // 要绘制的项的列表
        private IList<T> items;
        // 显示的最大文章数
        private int maxItemsToShow;
        // 显示的最小文章数
        // 如果 RSS 频道中的项的数目小于此值，
        // 则会显示空白
        private int minItemsToShow;

        private int NumArticles { get { return Math.Min(items.Count, maxItemsToShow); } }
        private int NumArticleRows { get { return Math.Max(NumArticles, minItemsToShow); } }

        public Point Location { get { return location; } set { location = value; } }
        public Size Size { get { return size; } set { size = value; } }

        public Color ForeColor { get { return foreColor; } set { foreColor = value; } }
        public Color BackColor { get { return backColor; } set { backColor = value; } }
        public Color BorderColor { get { return borderColor; } set { borderColor = value; } }
        public Color TitleForeColor { get { return titleForeColor; } set { titleForeColor = value; } }
        public Color TitleBackColor { get { return titleBackColor; } set { titleBackColor = value; } }
        public Color SelectedForeColor { get { return selectedForeColor; } set { selectedForeColor = value; } }
        public Color SelectedBackColor { get { return selectedBackColor; } set { selectedBackColor = value; } }
        public int MaxItemsToShow { get { return maxItemsToShow; } set { maxItemsToShow = value; } }
        public int MinItemsToShow { get { return minItemsToShow; } set { minItemsToShow = value; } }
        public int SelectedIndex { get { return selectedIndex; } }
        public T SelectedItem { get { return items[selectedIndex]; } }

        public int RowHeight
        {
            get
            {
                // 每项占用一行的空间，标题则需再占用两行空间。
                return size.Height / (NumArticleRows + 2);
            }
        }

        public Font ItemFont
        {
            get
            {
                // 为所有的项标题选择一种字体，使项标题的大小与控件中所有的 numItem 的大小相适应 
                // (在标题文字间添加一些空隙)
                itemFontHeight = (float)(percentOfArticleDisplayBoxToFillWithText * RowHeight);
                if (itemFont == null || itemFont.Size != itemFontHeight)
                {
                    itemFont = new Font("Microsoft Sans Serif", itemFontHeight, GraphicsUnit.Pixel);
                }
                return itemFont;
            }
        }

        public Font TitleFont
        {
            get
            {
                // 选择标题文本的字体。
                // 该字体的大小为 ItemFont 的两倍
                float titleFontHeight = (float)(percentOfArticleDisplayBoxToFillWithText * 2 * RowHeight);
                if (titleFont == null || titleFont.Size != titleFontHeight)
                {
                    titleFont = new Font("Microsoft Sans Serif", titleFontHeight, GraphicsUnit.Pixel);
                }
                return titleFont;
            }
        }

        public void NextArticle()
        {
            if (selectedIndex < NumArticles - 1)
                selectedIndex++;
            else
                selectedIndex = 0;
        }

        public void PreviousArticle()
        {
            if (selectedIndex > 0)
                selectedIndex--;
            else
                selectedIndex = NumArticles - 1;
        }

        public ItemListView(string title, IList<T> items)
        {
            if (items == null)
                throw new ArgumentException("项不能为空","items");

            this.items = items;
            this.title = title;
        }

        public void Paint(PaintEventArgs args)
        {
            Graphics g = args.Graphics;

            // 用于改进文本绘制效果的设置
            g.SmoothingMode = System.Drawing.Drawing2D.SmoothingMode.AntiAlias;
            g.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            DrawBackground(g);

            // 绘制每篇文章的说明
            for (int index = 0; index < items.Count && index < maxItemsToShow; index++)
            {
                DrawItemTitle(g, index);
            }

            // 绘制标题文本
            DrawTitle(g);
        }

        /// <summary>
        /// 绘制一个方框和边界，将在它上面绘制项的文本。
        /// </summary>
        /// <param name="g">将要在它上面执行绘制操作的图形对象</param>
        private void DrawBackground(Graphics g)
        {
            using (Brush backBrush = new SolidBrush(BackColor))
            using (Pen borderPen = new Pen(BorderColor, 4))
            {
                g.FillRectangle(backBrush, new Rectangle(Location.X + 4, Location.Y + 4, Size.Width - 8, Size.Height - 8));
                g.DrawRectangle(borderPen, new Rectangle(Location, Size));
            }
        }

        /// <summary>
        /// 绘制具有给定索引的项的标题。
        /// </summary>
        /// <param name="g">将要在它上面执行绘制操作的图形对象</param>
        /// <param name="index">列表中的项的索引</param>
        private void DrawItemTitle(Graphics g, int index)
        {
            // 设置格式和布局
            StringFormat stringFormat = new StringFormat(StringFormatFlags.LineLimit);
            stringFormat.Trimming = StringTrimming.EllipsisCharacter;
            Rectangle articleRect = new Rectangle(Location.X + padding, Location.Y + (int)(index * RowHeight) + padding, Size.Width - (2 * padding), (int)(percentOfFontHeightForSelectionBox * itemFontHeight));

            // 如果选中当前索引，则选择颜色和绘制边框
            Color textBrushColor = ForeColor;
            if (index == SelectedIndex)
            {
                textBrushColor = SelectedForeColor;
                using (Brush backBrush = new SolidBrush(SelectedBackColor))
                {
                    g.FillRectangle(backBrush, articleRect);
                }
            }

            // 绘制项的标题
            string textToDraw = items[index].Title;
            using (Brush textBrush = new SolidBrush(textBrushColor))
            {
                g.DrawString(textToDraw, ItemFont, textBrush, articleRect, stringFormat);
            }
        }

        /// <summary>
        /// 绘制标题栏。
        /// </summary>
        /// <param name="g">将要在它上面执行绘制操作的图形对象</param>
        private void DrawTitle(Graphics g)
        {
            Point titleLocation = new Point(Location.X + padding, Location.Y + Size.Height - (RowHeight) - padding);
            Size titleSize = new Size(Size.Width - (2 * padding), 2 * RowHeight);
            Rectangle titleRectangle = new Rectangle(titleLocation, titleSize);

            // 为标题和所选项绘制边框
            using (Brush titleBackBrush = new SolidBrush(TitleBackColor))
            {
                g.FillRectangle(titleBackBrush, titleRectangle);
            }

            // 绘制标题文本
            StringFormat titleFormat = new StringFormat(StringFormatFlags.LineLimit);
            titleFormat.Alignment = StringAlignment.Far;
            titleFormat.Trimming = StringTrimming.EllipsisCharacter;
            using (Brush titleBrush = new SolidBrush(TitleForeColor))
            {
                g.DrawString(title, titleFont, titleBrush, titleRectangle, titleFormat);
            }
        }

        /// <summary>
        /// 释放所有不再需要的字段
        /// </summary>
        public void Dispose()
        {
            if(itemFont != null)
                itemFont.Dispose();
            if(titleFont != null)
                titleFont.Dispose();
        }
    }
}