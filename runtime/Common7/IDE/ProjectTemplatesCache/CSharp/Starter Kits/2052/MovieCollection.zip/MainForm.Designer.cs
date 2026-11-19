using $safeprojectname$;
using System.Windows.Forms;

namespace $safeprojectname$
{
    partial class MainForm
    {
        //窗体重写释放，以清理组件列表。
        protected override void Dispose(bool disposing)
        {
            if (disposing && components != null)
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        //Windows 窗体设计器所必需的
        private System.ComponentModel.IContainer components;

        //注意: 以下过程是 Windows 窗体设计器所必需的
        //可以使用 Windows 窗体设计器修改它。
        //不要使用代码编辑器修改它。
        [System.Diagnostics.DebuggerNonUserCode]
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.notifyIcon1 = new System.Windows.Forms.NotifyIcon(this.components);
            this.targetPanel = new System.Windows.Forms.Panel();
            this.searchOnlineButton = new System.Windows.Forms.Button();
            this.viewDetailsButton = new System.Windows.Forms.Button();
            this.label1 = new System.Windows.Forms.Label();
            this.SuspendLayout();
            // 
            // notifyIcon1
            // 
            this.notifyIcon1.BalloonTipText = null;
            this.notifyIcon1.BalloonTipTitle = null;
            this.notifyIcon1.Text = "使用电影收藏";
            this.notifyIcon1.Visible = true;
            this.notifyIcon1.Click += new System.EventHandler(this.NotifyIcon1_Click);
            this.notifyIcon1.DoubleClick += new System.EventHandler(this.NotifyIcon1_DoubleClick);
            // 
            // targetPanel
            // 
            this.targetPanel.BackColor = System.Drawing.Color.DimGray;
            this.targetPanel.Location = new System.Drawing.Point(0, 103);
            this.targetPanel.Name = "targetPanel";
            this.targetPanel.Size = new System.Drawing.Size(920, 494);
            this.targetPanel.TabIndex = 2;
            // 
            // searchOnlineButton
            // 
            this.searchOnlineButton.BackColor = System.Drawing.Color.Transparent;
            this.searchOnlineButton.BackgroundImage = global::$safeprojectname$.Properties.Resources.button_search_active;
            this.searchOnlineButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.searchOnlineButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.searchOnlineButton.FlatAppearance.BorderSize = 0;
            this.searchOnlineButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.searchOnlineButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.searchOnlineButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.searchOnlineButton.Location = new System.Drawing.Point(767, 28);
            this.searchOnlineButton.Name = "searchOnlineButton";
            this.searchOnlineButton.Size = new System.Drawing.Size(120, 110);
            this.searchOnlineButton.TabIndex = 1;
            this.searchOnlineButton.Text = "联机搜索";
            this.searchOnlineButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.searchOnlineButton.UseVisualStyleBackColor = false;
            this.searchOnlineButton.Click += new System.EventHandler(this.SearchOnlineButton_Click);
            // 
            // viewDetailsButton
            // 
            this.viewDetailsButton.BackColor = System.Drawing.Color.Transparent;
            this.viewDetailsButton.BackgroundImage = global::$safeprojectname$.Properties.Resources.button_viewDvd_active;
            this.viewDetailsButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.viewDetailsButton.DialogResult = System.Windows.Forms.DialogResult.OK;
            this.viewDetailsButton.FlatAppearance.BorderSize = 0;
            this.viewDetailsButton.FlatAppearance.MouseDownBackColor = System.Drawing.Color.Transparent;
            this.viewDetailsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.viewDetailsButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.viewDetailsButton.Location = new System.Drawing.Point(627, 28);
            this.viewDetailsButton.Name = "viewDetailsButton";
            this.viewDetailsButton.Size = new System.Drawing.Size(120, 110);
            this.viewDetailsButton.TabIndex = 0;
            this.viewDetailsButton.Text = "观看 DVD";
            this.viewDetailsButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter;
            this.viewDetailsButton.UseVisualStyleBackColor = false;
            this.viewDetailsButton.Click += new System.EventHandler(this.ViewDetailsButton_Click);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.BackColor = System.Drawing.Color.Transparent;
            this.label1.Font = new System.Drawing.Font("Microsoft Sans Serif", 14.25F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.label1.Location = new System.Drawing.Point(13, 32);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(194, 24);
            this.label1.TabIndex = 3;
            this.label1.Text = "电影收藏";
            // 
            // MainForm
            // 
            this.BackgroundImage = global::$safeprojectname$.Properties.Resources.Blue_fullBg_noSideText;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None;
            this.ClientSize = new System.Drawing.Size(914, 618);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.searchOnlineButton);
            this.Controls.Add(this.viewDetailsButton);
            this.Controls.Add(this.targetPanel);
            this.ForeColor = System.Drawing.Color.DimGray;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle;
            this.Name = "MainForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "电影收藏";
            this.Load += new System.EventHandler(this.MainForm_Load);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        private NotifyIcon notifyIcon1;
        private Panel targetPanel;
        private Button searchOnlineButton;
        private Button viewDetailsButton;
        private Label label1;

    }
}
