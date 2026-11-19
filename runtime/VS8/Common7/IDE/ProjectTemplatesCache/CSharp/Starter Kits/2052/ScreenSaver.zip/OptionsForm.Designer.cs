namespace $safeprojectname$
{
    partial class OptionsForm : System.Windows.Forms.Form
    {
        /// <summary>
        /// 必需的设计器变量。
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows 窗体设计器生成的代码

        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            this.okButton = new System.Windows.Forms.Button();
            this.cancelButton = new System.Windows.Forms.Button();
            this.applyButton = new System.Windows.Forms.Button();
            this.rssGroupBox = new System.Windows.Forms.GroupBox();
            this.validateButton = new System.Windows.Forms.Button();
            this.rssFeedLabel = new System.Windows.Forms.Label();
            this.rssFeedTextBox = new System.Windows.Forms.TextBox();
            this.imageGroupBox = new System.Windows.Forms.GroupBox();
            this.browseButton = new System.Windows.Forms.Button();
            this.backgroundImageFolderTextBox = new System.Windows.Forms.TextBox();
            this.backgroundImageLabel = new System.Windows.Forms.Label();
            this.backgroundImageOpenFileDialog = new System.Windows.Forms.OpenFileDialog();
            this.backgroundImageFolderBrowser = new System.Windows.Forms.FolderBrowserDialog();
            this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
            this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
            this.rssGroupBox.SuspendLayout();
            this.imageGroupBox.SuspendLayout();
            this.tableLayoutPanel1.SuspendLayout();
            this.tableLayoutPanel2.SuspendLayout();
            this.SuspendLayout();
            // 
            // okButton
            // 
            this.okButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.okButton.Location = new System.Drawing.Point(156, 3);
            this.okButton.Margin = new System.Windows.Forms.Padding(3, 3, 2, 3);
            this.okButton.Name = "okButton";
            this.okButton.Size = new System.Drawing.Size(75, 23);
            this.okButton.TabIndex = 1;
            this.okButton.Text = "确定";
            this.okButton.Click += new System.EventHandler(this.btnOK_Click);
            // 
            // cancelButton
            // 
            this.cancelButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.cancelButton.Location = new System.Drawing.Point(74, 3);
            this.cancelButton.Margin = new System.Windows.Forms.Padding(1, 3, 3, 3);
            this.cancelButton.Name = "cancelButton";
            this.cancelButton.Size = new System.Drawing.Size(75, 23);
            this.cancelButton.TabIndex = 2;
            this.cancelButton.Text = "取消";
            this.cancelButton.Click += new System.EventHandler(this.btnCancel_Click);
            // 
            // applyButton
            // 
            this.applyButton.Anchor = System.Windows.Forms.AnchorStyles.None;
            this.applyButton.Location = new System.Drawing.Point(237, 3);
            this.applyButton.Name = "applyButton";
            this.applyButton.Size = new System.Drawing.Size(75, 23);
            this.applyButton.TabIndex = 3;
            this.applyButton.Text = "应用";
            this.applyButton.Click += new System.EventHandler(this.btnApply_Click);
            // 
            // rssGroupBox
            // 
            this.rssGroupBox.Controls.Add(this.validateButton);
            this.rssGroupBox.Controls.Add(this.rssFeedLabel);
            this.rssGroupBox.Controls.Add(this.rssFeedTextBox);
            this.rssGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.rssGroupBox.Location = new System.Drawing.Point(3, 3);
            this.rssGroupBox.Name = "rssGroupBox";
            this.rssGroupBox.Size = new System.Drawing.Size(315, 115);
            this.rssGroupBox.TabIndex = 4;
            this.rssGroupBox.TabStop = false;
            this.rssGroupBox.Text = "RSS 源";
            // 
            // validateButton
            // 
            this.validateButton.Location = new System.Drawing.Point(7, 68);
            this.validateButton.Name = "validateButton";
            this.validateButton.Size = new System.Drawing.Size(75, 23);
            this.validateButton.TabIndex = 2;
            this.validateButton.Text = "验证";
            this.validateButton.Click += new System.EventHandler(this.validateButton_Click);
            // 
            // rssFeedLabel
            // 
            this.rssFeedLabel.AutoSize = true;
            this.rssFeedLabel.Location = new System.Drawing.Point(7, 20);
            this.rssFeedLabel.Name = "rssFeedLabel";
            this.rssFeedLabel.Size = new System.Drawing.Size(68, 13);
            this.rssFeedLabel.TabIndex = 1;
            this.rssFeedLabel.Text = "RSS 2.0 URI:";
            // 
            // rssFeedTextBox
            // 
            this.rssFeedTextBox.Location = new System.Drawing.Point(7, 41);
            this.rssFeedTextBox.Name = "rssFeedTextBox";
            this.rssFeedTextBox.Size = new System.Drawing.Size(301, 20);
            this.rssFeedTextBox.TabIndex = 0;
            this.rssFeedTextBox.TextChanged += new System.EventHandler(this.rssFeedTextBox_TextChanged);
            // 
            // imageGroupBox
            // 
            this.imageGroupBox.Controls.Add(this.browseButton);
            this.imageGroupBox.Controls.Add(this.backgroundImageFolderTextBox);
            this.imageGroupBox.Controls.Add(this.backgroundImageLabel);
            this.imageGroupBox.Dock = System.Windows.Forms.DockStyle.Fill;
            this.imageGroupBox.Location = new System.Drawing.Point(3, 124);
            this.imageGroupBox.Name = "imageGroupBox";
            this.imageGroupBox.Size = new System.Drawing.Size(315, 115);
            this.imageGroupBox.TabIndex = 5;
            this.imageGroupBox.TabStop = false;
            this.imageGroupBox.Text = "背景图像";
            // 
            // browseButton
            // 
            this.browseButton.Location = new System.Drawing.Point(7, 68);
            this.browseButton.Name = "browseButton";
            this.browseButton.Size = new System.Drawing.Size(75, 23);
            this.browseButton.TabIndex = 2;
            this.browseButton.Text = "浏览...";
            this.browseButton.Click += new System.EventHandler(this.browseButton_Click);
            // 
            // backgroundImageFolderTextBox
            // 
            this.backgroundImageFolderTextBox.Location = new System.Drawing.Point(7, 41);
            this.backgroundImageFolderTextBox.Name = "backgroundImageFolderTextBox";
            this.backgroundImageFolderTextBox.Size = new System.Drawing.Size(301, 20);
            this.backgroundImageFolderTextBox.TabIndex = 1;
            this.backgroundImageFolderTextBox.TextChanged += new System.EventHandler(this.backgroundImageFolderTextBox_TextChanged);
            // 
            // backgroundImageLabel
            // 
            this.backgroundImageLabel.AutoSize = true;
            this.backgroundImageLabel.Location = new System.Drawing.Point(7, 20);
            this.backgroundImageLabel.Name = "backgroundImageLabel";
            this.backgroundImageLabel.Size = new System.Drawing.Size(162, 13);
            this.backgroundImageLabel.TabIndex = 0;
            this.backgroundImageLabel.Text = "背景图像目录的路径:";
            // 
            // backgroundImageFolderBrowser
            // 
            this.backgroundImageFolderBrowser.RootFolder = System.Environment.SpecialFolder.MyPictures;
            // 
            // tableLayoutPanel1
            // 
            this.tableLayoutPanel1.ColumnCount = 1;
            this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.Controls.Add(this.imageGroupBox, 0, 1);
            this.tableLayoutPanel1.Controls.Add(this.rssGroupBox, 0, 0);
            this.tableLayoutPanel1.Controls.Add(this.tableLayoutPanel2, 0, 2);
            this.tableLayoutPanel1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel1.Location = new System.Drawing.Point(9, 9);
            this.tableLayoutPanel1.Name = "tableLayoutPanel1";
            this.tableLayoutPanel1.RowCount = 3;
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Absolute, 35F));
            this.tableLayoutPanel1.Size = new System.Drawing.Size(321, 278);
            this.tableLayoutPanel1.TabIndex = 6;
            // 
            // tableLayoutPanel2
            // 
            this.tableLayoutPanel2.ColumnCount = 4;
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 81F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 81F));
            this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Absolute, 81F));
            this.tableLayoutPanel2.Controls.Add(this.applyButton, 3, 0);
            this.tableLayoutPanel2.Controls.Add(this.cancelButton, 1, 0);
            this.tableLayoutPanel2.Controls.Add(this.okButton, 2, 0);
            this.tableLayoutPanel2.Dock = System.Windows.Forms.DockStyle.Fill;
            this.tableLayoutPanel2.Location = new System.Drawing.Point(3, 245);
            this.tableLayoutPanel2.Name = "tableLayoutPanel2";
            this.tableLayoutPanel2.RowCount = 1;
            this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 50F));
            this.tableLayoutPanel2.Size = new System.Drawing.Size(315, 30);
            this.tableLayoutPanel2.TabIndex = 6;
            // 
            // OptionsForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(339, 296);
            this.Controls.Add(this.tableLayoutPanel1);
            this.Name = "OptionsForm";
            this.Padding = new System.Windows.Forms.Padding(9);
            this.ShowIcon = false;
            this.Text = "屏幕保护程序设置";
            this.rssGroupBox.ResumeLayout(false);
            this.rssGroupBox.PerformLayout();
            this.imageGroupBox.ResumeLayout(false);
            this.imageGroupBox.PerformLayout();
            this.tableLayoutPanel1.ResumeLayout(false);
            this.tableLayoutPanel2.ResumeLayout(false);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button applyButton;
        private System.Windows.Forms.Button cancelButton;
        private System.Windows.Forms.Button okButton;
        private System.Windows.Forms.OpenFileDialog backgroundImageOpenFileDialog;
        private System.Windows.Forms.GroupBox rssGroupBox;
        private System.Windows.Forms.TextBox rssFeedTextBox;
        private System.Windows.Forms.GroupBox imageGroupBox;
        private System.Windows.Forms.Button validateButton;
        private System.Windows.Forms.Label rssFeedLabel;
        private System.Windows.Forms.Button browseButton;
        private System.Windows.Forms.TextBox backgroundImageFolderTextBox;
        private System.Windows.Forms.Label backgroundImageLabel;
        private System.Windows.Forms.FolderBrowserDialog backgroundImageFolderBrowser;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
        private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
    }
}