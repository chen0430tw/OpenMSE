using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Drawing;
using System.IO;
using System.Windows.Forms;
using $safeprojectname$.UI;
using $safeprojectname$.Rss;

namespace $safeprojectname$
{
    partial class ScreenSaverForm : Form
    {
        // 用于显示文章来源的 RssFeed
        private RssFeed rssFeed;

        // 用于显示 RSS 内容的对象
        private ItemListView<RssItem> rssView;
        private ItemDescriptionView<RssItem> rssDescriptionView;

        // 在背景中显示的图像
        private List<Image> backgroundImages;
        private int currentImageIndex;

        // 跟踪屏幕保护程序是否已激活。
        private bool isActive = false;

        // 跟踪鼠标的位置
        private Point mouseLocation;

        private List<string> imageExtensions = new List<string>(new string[]{ "*.bmp", "*.gif", "*.png", "*.jpg", "*.jpeg" });

        public ScreenSaverForm()
        {
            InitializeComponent();

            SetupScreenSaver();
            LoadBackgroundImage();
            LoadRssFeed();

            // 初始化 ItemListView 以显示 RssItem 中的项的列表。
            // 该信息将出现在屏幕的左侧。            
            rssView = new ItemListView<RssItem>(rssFeed.MainChannel.Title, rssFeed.MainChannel.Items);
            InitializeRssView();

            // 初始化 ItemDescriptionView 以显示 RssItem 的说明信息。
            // 该信息将出现在屏幕的右侧。            
            rssDescriptionView = new ItemDescriptionView<RssItem>();
            InitializeRssDescriptionView();
        }
        

        /// <summary>
        /// 将主窗体设置为全屏显示的屏幕保护程序。
        /// </summary>
        private void SetupScreenSaver()
        {
            // 使用双缓冲来改进绘制性能
            this.SetStyle(ControlStyles.OptimizedDoubleBuffer | ControlStyles.UserPaint | ControlStyles.AllPaintingInWmPaint, true);
            // 捕获鼠标
            this.Capture = true;

            // 将应用程序设置为全屏模式并隐藏鼠标
            Cursor.Hide();
            Bounds = Screen.PrimaryScreen.Bounds;
            WindowState = FormWindowState.Maximized;
            ShowInTaskbar = false;
            DoubleBuffered = true;
            BackgroundImageLayout = ImageLayout.Stretch;
        }

        private void LoadBackgroundImage()
        {
            // 初始化背景图像。
            backgroundImages = new List<Image>();
            currentImageIndex = 0;

            if (Directory.Exists(Properties.Settings.Default.BackgroundImagePath))
            {
                try
                {
                    // 尝试加载用户指定的图像。
                    LoadImagesFromFolder();
                }
                catch
                {
                    // 如果失败，则加载默认图像。
                    LoadDefaultBackgroundImages();
                }
            }

            // 如果未加载任何图像，则加载默认图像
            if (backgroundImages.Count == 0)
            {
                LoadDefaultBackgroundImages();
            }
        }

        private void LoadImagesFromFolder()
        {
            DirectoryInfo backgroundImageDir = new DirectoryInfo(Properties.Settings.Default.BackgroundImagePath);
            // 对于每种图像扩展名(.jpg、.bmp 等)
            foreach (string imageExtension in imageExtensions)
            {
                // 对于用户提供的目录中的每个文件
                foreach (FileInfo file in backgroundImageDir.GetFiles(imageExtension))
                {
                    // 尝试加载图像
                    try
                    {
                        Image image = Image.FromFile(file.FullName);
                        backgroundImages.Add(image);
                    }
                    catch (OutOfMemoryException)
                    {
                        // 如果图像无法加载，则继续。
                        continue;
                    }
                }
            }
        }


        private void LoadDefaultBackgroundImages()
        {
            // 如果出于某种原因无法加载背景图像，
            // 则使用资源中存储的图像
            backgroundImages.Add(Properties.Resources.SSaverBackground);
            backgroundImages.Add(Properties.Resources.SSaverBackground2);
        }

        private void LoadRssFeed()
        {
            try
            {
                // 尝试从用户设置获取该数据
                rssFeed = RssFeed.FromUri(Properties.Settings.Default.RssFeedUri);
            }
            catch
            {
                // 如果加载 RSS 时出现了问题，则加载一个错误信息 RSS 源
                rssFeed = RssFeed.FromText(Properties.Resources.DefaultRSSText);
            }
        }

        /// <summary>
        /// 初始化 rssView 的显示属性。
        /// </summary>
        private void InitializeRssView()
        {
            rssView.BackColor = Color.FromArgb(120, 240, 234, 232);
            rssView.BorderColor = Color.White;
            rssView.ForeColor = Color.FromArgb(255, 40, 40, 40);
            rssView.SelectedBackColor = Color.FromArgb(200, 105, 61, 76);
            rssView.SelectedForeColor = Color.FromArgb(255, 204, 184, 163);
            rssView.TitleBackColor = Color.Empty;
            rssView.TitleForeColor = Color.FromArgb(255, 240, 234, 232);
            rssView.MaxItemsToShow = 20;
            rssView.MinItemsToShow = 15;
            rssView.Location = new Point(Width / 10, Height / 10);
            rssView.Size = new Size(Width / 2, Height / 2);
        }

        /// <summary>
        /// 初始化 rssDescriptionView 的显示属性。
        /// </summary>
        private void InitializeRssDescriptionView()
        {
            rssDescriptionView.DisplayItem = rssView.SelectedItem;
            rssDescriptionView.ForeColor = Color.FromArgb(255, 240, 234, 232);
            rssDescriptionView.TitleFont = rssView.TitleFont;
            rssDescriptionView.LineColor = Color.FromArgb(120, 240, 234, 232);
            rssDescriptionView.LineWidth = 2f;
            rssDescriptionView.FadeTimer.Tick += new EventHandler(FadeTimer_Tick);
            rssDescriptionView.FadeTimer.Interval = 40;
            rssDescriptionView.Location = new Point(3 * Width / 4, Height / 3);
            rssDescriptionView.Size = new Size(Width / 4, Height / 2);
            rssDescriptionView.FadingComplete += new EventHandler(rssItemView_FadingComplete);
        }

        private void ScreenSaverForm_MouseMove(object sender, MouseEventArgs e)
        {
            // 仅在第一次调用此事件时设置 IsActive 和 MouseLocation。
            if (!isActive)
            {
                mouseLocation = MousePosition;
                isActive = true;
            }
            else
            {
                // 如果在第一次调用后鼠标发生明显的移动，则关闭。
                if ((Math.Abs(MousePosition.X - mouseLocation.X) > 10) ||
                    (Math.Abs(MousePosition.Y - mouseLocation.Y) > 10))
                {
                    Close();
                }
            }
        }

        private void ScreenSaverForm_KeyDown(object sender, KeyEventArgs e)
        {
            Close();

        }

        private void ScreenSaverForm_MouseDown(object sender, MouseEventArgs e)
        {
            Close();
        }

        protected override void OnPaintBackground(PaintEventArgs e)
        {
            // 绘制当前背景图像并拉伸至全屏
            e.Graphics.DrawImage(backgroundImages[currentImageIndex], 0, 0, Size.Width, Size.Height);
        }

        protected override void OnPaint(PaintEventArgs e)
        {
            rssView.Paint(e);
            rssDescriptionView.Paint(e);
        }

        private void backgroundChangeTimerTick(object sender, EventArgs e)
        {
            // 将背景图像更改为下一幅图像。
            currentImageIndex = (currentImageIndex + 1) % backgroundImages.Count;
        }

        void FadeTimer_Tick(object sender, EventArgs e)
        {
            this.Refresh();
        }

        void rssItemView_FadingComplete(object sender, EventArgs e)
        {
            rssView.NextArticle();
            rssDescriptionView.DisplayItem = rssView.SelectedItem;
        }
    }
}
