using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Drawing;
using System.Windows.Forms;
using $safeprojectname$.Controls;

namespace $safeprojectname$
{
    /// <summary>
    /// 用户控件，负责搜索 Amazon.com Web 服务以查找电影标题及将标题添加到收藏。 
    /// </summary>
    /// <remarks>
    /// 单击“搜索”按钮或在键入搜索文本后回车，以执行联机搜索。  
    /// 选择一个有效的电影标题并单击“添加到收藏”以将数据保存到本地。
    /// 简化的 Amazon.com 搜索类返回 DVD 对象的泛型 BindingList。用户控件 UI 元素是
    /// 使用“数据源对象”绑定功能绑定到 DVD 列表的数据。将标题添加到收藏
    /// 的操作会将 DVD 对象转换为内存中的数据表格式，应用程序的其余部分使用这种格式。  
    /// </remarks>
    public partial class SearchOnline : UserControl
    {
        private MainForm mainForm;
        public MainForm MainForm { get { return mainForm; } set { mainForm = value; } }

        public SearchOnline()
        {
            //此调用是 Windows 窗体设计器所必需的。
            InitializeComponent();
        }

        /// <summary>
        /// 调用 Amazon.com Web 服务并将 DVD bindinglist 结果绑定到 UI。  
        /// </summary>
        private void PerformSearch()
        {
            //负责包含 DVD 搜索结果的对象
            BindingList<DVD> searchResults = new BindingList<DVD>();

            //简单包装对象负责处理来自 Amazon.com Web 服务的请求和响应
            SimpleAmazonWS amazonService = new SimpleAmazonWS();

            //在搜索过程中显示沙漏，以通知用户搜索正在进行
            this.Cursor = Cursors.WaitCursor;

            try
            {
                //请求 Web 服务根据用户的搜索条件返回搜索结果
                searchResults = amazonService.SearchDVDs(this.searchTextBox.Text);

                //将搜索结果数据绑定到窗体 UI
                this.dvdDataConnector.DataSource = searchResults;
            }
            catch (Exception ex)
            {
                MessageBox.Show(String.Format("连接到 Web 服务时出现问题。请确认您已连接到 Internet。其他详细信息: {0}", ex.Message));
                Debug.WriteLine(ex);
            }
            finally
            {
                //工作完成后，将光标重新设置为默认值
                this.Cursor = Cursors.Default;
            }

            //告知用户查找到的结果数目。使用 String.Format 功能按照便于本地化的方式连接字符串。
            this.label2.Text = String.Format("找到了 {0} 个结果。  ", searchResults.Count.ToString());
        }

        /// <summary>
        /// 将当前电影选择添加到本地电影收藏中，然后显示 ListDetails 用户控件。
        /// </summary>
        private void AddCurrentToCollection()
        {
            //尝试将数据添加到收藏中
            try
            {
                //检查边界情况: DVD 列表是否为空或者当前是否未进行选择
                if ((this.dvdDataConnector.Count > 0) && (this.dvdDataConnector.Current != null))
                {
                    //存储当前 DVD 选定内容的对象
                    DVD currentDVD = (DVD)this.dvdDataConnector.Current;

                    //尝试将图像缓存为 DVD 对象
                    if (this.pictureBox1.Image != null)
                    {
                        //首先尝试从图片框加载
                        currentDVD.ImageCache = (Bitmap)this.pictureBox1.Image;
                    }
                    else if (!String.IsNullOrEmpty(currentDVD.ImageUrl))
                    {
                        //否则尝试从源 URL 加载
                        this.pictureBox1.Load(currentDVD.ImageUrl);
                        currentDVD.ImageCache = (Bitmap)this.pictureBox1.Image;
                    }

                    //将自定义 DVD 对象转换为该应用程序所使用的数据表/数据行格式，并
                    //将数据行添加到数据表
                    mainForm.ListDetailsPart.dvdCollectionDataSet.DVDs.AddDVDsRow(currentDVD);

                    //更新显示，以显示选中了新行的 ListDetailsPart
                    mainForm.ListDetailsPart.dvdsDataConnector.Position = mainForm.ListDetailsPart.dvdsDataConnector.Count - 1;
                    mainForm.ShowListDetailsPart();
                }
                else
                {
                    MessageBox.Show("未选择 DVD。请在添加到收藏前先选择一个 DVD。");
                }

            }
            catch (Exception ex)
            {
                MessageBox.Show("向收藏添加此 DVD 时出现问题。");
                Debug.WriteLine(ex);
            }
        }

        /// <summary>
        /// 单击按钮后执行联机搜索。
        /// </summary>
        private void SearchButton_Click(object sender, System.EventArgs e)
        {
            PerformSearch();
        }

        /// <summary>
        /// 在搜索文本框中按下 Enter 后执行联机搜索。
        /// </summary>
        private void SearchTextBox_KeyUp(object sender, System.Windows.Forms.KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                PerformSearch();
            }
        }

        /// <summary>
        /// 按下按钮后将当前选定内容添加到本地收藏。
        /// </summary>
        private void AddToCollectionButton_Click(System.Object sender, System.EventArgs e)
        {
            AddCurrentToCollection();
        }

        /// <summary>
        /// 双击 datagridview 将当前选定内容添加到本地收藏中。
        /// </summary>
        private void dvdDataGridView_DoubleClick(System.Object sender, System.EventArgs e)
        {
            if (this.dvdDataConnector.Current != null)
            {
                AddCurrentToCollection();
            }
        }
    }
}
