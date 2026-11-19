using System;
using System.Configuration;
using System.Drawing;
using System.Windows.Forms;
using $safeprojectname$.Rss;


namespace $safeprojectname$
{
    partial class OptionsForm : Form
    {
        public OptionsForm()
        {
            InitializeComponent();

            // 从当前设置加载文本框
            try
            {
                backgroundImageFolderTextBox.Text = Properties.Settings.Default.BackgroundImagePath;
                rssFeedTextBox.Text = Properties.Settings.Default.RssFeedUri;
            }
            catch
            {
                MessageBox.Show("读入屏幕保护程序的设置时出现了问题。");
            }
        }

        // 仅在上次按下“应用”按钮后
        // 发生了更改的情况下将该按钮更新为活动状态
        private void UpdateApply()
        {
            if(Properties.Settings.Default.BackgroundImagePath != backgroundImageFolderTextBox.Text
                  || Properties.Settings.Default.RssFeedUri != rssFeedTextBox.Text)
                applyButton.Enabled = true;
            else
                applyButton.Enabled = false;
        }

        // 应用自上次按下“应用”按钮后发生的所有更改
        private void ApplyChanges()
        {
            Properties.Settings.Default.BackgroundImagePath = backgroundImageFolderTextBox.Text;
            Properties.Settings.Default.RssFeedUri = rssFeedTextBox.Text;
            Properties.Settings.Default.Save();
        }

        private void btnOK_Click(object sender, EventArgs e)
        {
            try
            {
                ApplyChanges();
            }
            catch (ConfigurationException)
            {
                MessageBox.Show("无法保存您的设置。请确保屏幕保护程序所在的目录包含一个 .config 文件", "未能保存设置", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
            finally
            {
                Close();
            }
        }

        private void btnCancel_Click(object sender, EventArgs e)
        {
            Close();
        }

        private void btnApply_Click(object sender, EventArgs e)
        {
            ApplyChanges();
            applyButton.Enabled = false;
        }

        // 检查用户提供的 URI 是否指向一个有效的 RSS 源
        private void validateButton_Click(object sender, EventArgs e)
        {
            try
            {
                RssFeed.FromUri(rssFeedTextBox.Text);
            }
            catch
            {
                MessageBox.Show("不是一个有效的 RSS 源。","不是一个有效的 RSS 源。",MessageBoxButtons.OK,MessageBoxIcon.Error);
                return;
            }

            MessageBox.Show("有效的 RSS 源。", "有效的 RSS 源。",MessageBoxButtons.OK, MessageBoxIcon.Exclamation);
        }

        private void browseButton_Click(object sender, EventArgs e)
        {
            // 打开一个“打开文件”对话框，以选择一幅图像
            
            DialogResult result = backgroundImageFolderBrowser.ShowDialog();
            if (result == DialogResult.OK)
            {
                backgroundImageFolderTextBox.Text = backgroundImageFolderBrowser.SelectedPath;
                UpdateApply();
            }
        }

        private void rssFeedTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateApply();
        }

        private void backgroundImageFolderTextBox_TextChanged(object sender, EventArgs e)
        {
            UpdateApply();
        }
    }
}