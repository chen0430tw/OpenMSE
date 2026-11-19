Partial Class MainForm
    Inherits System.Windows.Forms.Form

    Public Sub New()
        MyBase.New()

        '此调用是 Windows 窗体设计器所必需的。
        InitializeComponent()
        '解决办法 -- 应启用项目设计器
        Application.EnableVisualStyles()
        '在 InitializeComponent() 调用之后添加任何初始化

    End Sub

    '窗体重写释放，以清理组件列表。
    Protected Overrides Sub Dispose(ByVal disposing As Boolean)
        Try
            If disposing AndAlso components IsNot Nothing Then
                components.Dispose()
            End If
        Finally
            MyBase.Dispose(disposing)
        End Try
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    Friend WithEvents NotifyIcon1 As System.Windows.Forms.NotifyIcon

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerNonUserCode()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Dim resources As System.ComponentModel.ComponentResourceManager = New System.ComponentModel.ComponentResourceManager(GetType(MainForm))
        Me.NotifyIcon1 = New System.Windows.Forms.NotifyIcon(Me.components)
        Me.TargetPanel = New System.Windows.Forms.Panel
        Me.SearchOnlineButton = New System.Windows.Forms.Button
        Me.ViewDetailsButton = New System.Windows.Forms.Button
        Me.Label1 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'NotifyIcon1
        '
        Me.NotifyIcon1.BalloonTipText = Nothing
        Me.NotifyIcon1.BalloonTipTitle = Nothing
        Me.NotifyIcon1.Icon = CType(resources.GetObject("NotifyIcon1.Icon"), System.Drawing.Icon)
        Me.NotifyIcon1.Text = "使用电影收藏"
        Me.NotifyIcon1.Visible = True
        '
        'TargetPanel
        '
        Me.TargetPanel.BackColor = System.Drawing.Color.DimGray
        Me.TargetPanel.Location = New System.Drawing.Point(0, 103)
        Me.TargetPanel.Name = "TargetPanel"
        Me.TargetPanel.Size = New System.Drawing.Size(920, 494)
        Me.TargetPanel.TabIndex = 2
        '
        'SearchOnlineButton
        '
        Me.SearchOnlineButton.BackColor = System.Drawing.Color.Transparent
        Me.SearchOnlineButton.BackgroundImage = Global.$safeprojectname$.My.Resources.Resources.button_search_active
        Me.SearchOnlineButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch
        Me.SearchOnlineButton.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.SearchOnlineButton.FlatAppearance.BorderSize = 0
        Me.SearchOnlineButton.FlatAppearance.MouseDownBackColor = Color.Transparent
        Me.SearchOnlineButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.SearchOnlineButton.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.SearchOnlineButton.Location = New System.Drawing.Point(767, 28)
        Me.SearchOnlineButton.Name = "SearchOnlineButton"
        Me.SearchOnlineButton.Size = New System.Drawing.Size(120, 110)
        Me.SearchOnlineButton.TabIndex = 1
        Me.SearchOnlineButton.Text = "联机搜索"
        Me.SearchOnlineButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.SearchOnlineButton.UseVisualStyleBackColor = False
        '
        'ViewDetailsButton
        '
        Me.ViewDetailsButton.BackColor = System.Drawing.Color.Transparent
        Me.ViewDetailsButton.BackgroundImage = Global.$safeprojectname$.My.Resources.Resources.button_viewDvd_active
        Me.ViewDetailsButton.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch
        Me.ViewDetailsButton.DialogResult = System.Windows.Forms.DialogResult.OK
        Me.ViewDetailsButton.FlatAppearance.BorderSize = 0
        Me.ViewDetailsButton.FlatAppearance.MouseDownBackColor = Color.Transparent
        Me.ViewDetailsButton.FlatStyle = System.Windows.Forms.FlatStyle.Flat
        Me.ViewDetailsButton.Font = New System.Drawing.Font("Microsoft Sans Serif", 9.75!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.ViewDetailsButton.Location = New System.Drawing.Point(627, 28)
        Me.ViewDetailsButton.Name = "ViewDetailsButton"
        Me.ViewDetailsButton.Size = New System.Drawing.Size(120, 110)
        Me.ViewDetailsButton.TabIndex = 0
        Me.ViewDetailsButton.Text = "观看 DVD"
        Me.ViewDetailsButton.TextAlign = System.Drawing.ContentAlignment.BottomCenter
        Me.ViewDetailsButton.UseVisualStyleBackColor = False
        '
        'Label1
        '
        Me.Label1.AutoSize = True
        Me.Label1.BackColor = System.Drawing.Color.Transparent
        Me.Label1.Font = New System.Drawing.Font("Microsoft Sans Serif", 14.25!, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, CType(0, Byte))
        Me.Label1.Location = New System.Drawing.Point(13, 32)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(194, 24)
        Me.Label1.TabIndex = 3
        Me.Label1.Text = "电影收藏"
        '
        'MainForm
        '
        Me.BackgroundImage = Global.$safeprojectname$.My.Resources.Resources.Blue_fullBg_noSideText
        Me.BackgroundImageLayout = System.Windows.Forms.ImageLayout.None
        Me.ClientSize = New System.Drawing.Size(914, 618)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.SearchOnlineButton)
        Me.Controls.Add(Me.ViewDetailsButton)
        Me.Controls.Add(Me.TargetPanel)
        Me.ForeColor = System.Drawing.Color.DimGray
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedSingle
        Me.Icon = CType(resources.GetObject("$this.Icon"), System.Drawing.Icon)
        Me.Name = "MainForm"
        Me.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen
        Me.Text = "电影收藏"
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub
    Friend WithEvents TargetPanel As System.Windows.Forms.Panel
    Friend WithEvents SearchOnlineButton As System.Windows.Forms.Button
    Friend WithEvents ViewDetailsButton As System.Windows.Forms.Button
    Friend WithEvents Label1 As System.Windows.Forms.Label

End Class
