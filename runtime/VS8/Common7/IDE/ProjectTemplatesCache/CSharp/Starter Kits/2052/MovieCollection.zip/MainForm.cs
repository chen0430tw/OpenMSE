using System.Windows.Forms;

namespace $safeprojectname$
{
    /// <summary>
    /// 宿主窗体负责应用程序的观看和联机搜索模式之间的导航。  
    /// </summary>
    /// <remarks>
    /// 在设计器中查看 MainForm.vb 文件，修改其他属性。  
    /// </remarks>
    public partial class MainForm : Form
    {
        //用户控件对象负责查看收藏和联机搜索
        ListDetails listDetailsPart;
        SearchOnline searchOnlinePart;

        public MainForm()
        {
            InitializeComponent();
        }

        /// <summary>
        /// 获取负责查看电影收藏的用户控件实例。  
        /// </summary>
        /// <value>ListDetails 用户控件的活实例。  </value>
        /// <remarks>单一实例模式；在第一次访问时创建该对象，此后的所有访问都重用该对象。</remarks>
        internal ListDetails ListDetailsPart
        {
            get
            {
                //如果变量为空，使用一个新的对象实例初始化该变量
                if (this.listDetailsPart == null)
                //正在创建对象
                {
                    this.listDetailsPart = new ListDetails();

                    //将此控件布置在宿主用户控件上，并停靠填充它。
                    this.targetPanel.Controls.Add(this.listDetailsPart);
                    this.listDetailsPart.Dock = DockStyle.Fill;
                }

                return this.listDetailsPart;
            }
        }

        /// <summary>
        /// 获取负责查看电影收藏的用户控件属性。  
        /// </summary>
        /// <value>ListDetails 用户控件的活实例。  </value>
        /// <remarks>单一实例模式；在第一次访问时创建该对象，此后的所有访问都重用该对象。</remarks>
        internal SearchOnline SearchOnlinePart
        {
            get
            {
                //如果变量为空，使用一个新的对象实例初始化该变量
                if (this.searchOnlinePart == null)
                //正在创建对象
                {
                    this.searchOnlinePart = new SearchOnline();
                    this.searchOnlinePart.MainForm = this;

                    //将此控件布置在宿主用户控件上，并停靠填充它。
                    this.targetPanel.Controls.Add(this.searchOnlinePart);
                    this.searchOnlinePart.Dock = DockStyle.Fill;
                }

                return this.searchOnlinePart;
            }
        }

        /// <summary>
        /// 在第一次加载应用程序时显示 ListDetails 用户控件。 
        /// </summary>
        private void MainForm_Load(System.Object sender, System.EventArgs e)
        {
            this.ShowListDetailsPart();
        }

        /// <summary>
        /// 单击任务栏图标时恢复窗口。
        /// </summary>
        private void NotifyIcon1_Click(object sender, System.EventArgs e)
        {
            this.RestoreWindow();
        }

        /// <summary>
        /// 双击任务栏图标时恢复窗口。 
        /// </summary>
        private void NotifyIcon1_DoubleClick(object sender, System.EventArgs e)
        {
            this.RestoreWindow();
        }

        /// <summary>
        /// 使 ListDetails 用户控件可见并隐藏 SearchOnline 用户控件。
        /// </summary>
        private void ViewDetailsButton_Click(object sender, System.EventArgs e)
        {
            this.ShowListDetailsPart();
        }

        /// <summary>
        /// 使 SearchOnline 用户控件可见并隐藏 ListDetails 用户控件。
        /// </summary>
        private void SearchOnlineButton_Click(object sender, System.EventArgs e)
        {
            this.ShowSearchOnlinePart();
        }

        /// <summary>
        /// 将窗口恢复为正常大小并确保其可见。
        /// </summary>
        private void RestoreWindow()
        {
            this.WindowState = FormWindowState.Normal;
            this.Visible = true;
        }

        /// <summary>
        /// 显示 ListDetails 用户控件并隐藏 SearchOnline 用户控件。  
        /// </summary>
        /// <remarks>启用列表模式和搜索模式的交换行为。</remarks>
        internal void ShowListDetailsPart()
        {
            //性能优化 - 如果此控件尚未创建，则跳过控件隐藏操作
            if (this.SearchOnlinePart != null)
            {
                this.SearchOnlinePart.Visible = false;
            }

            this.ListDetailsPart.Visible = true;
        }

        /// <summary>
        /// 显示 SearchOnline 用户控件并隐藏 ListDetails 用户控件。  
        /// </summary>
        /// <remarks>启用列表模式和搜索模式的交换行为。</remarks>
        internal void ShowSearchOnlinePart()
        {
            //性能优化 - 如果此控件尚未创建，则跳过控件隐藏操作
            if (this.ListDetailsPart != null)
            {
                this.ListDetailsPart.Visible = false;
            }

            //显示此控件并隐藏所有其他控件
            this.SearchOnlinePart.Visible = true;
        }
    }
}
