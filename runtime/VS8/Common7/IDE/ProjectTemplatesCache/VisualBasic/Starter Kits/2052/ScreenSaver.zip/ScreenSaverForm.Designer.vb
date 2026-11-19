<Global.Microsoft.VisualBasic.CompilerServices.DesignerGenerated()> _
Partial Class ScreenSaverForm
    Inherits System.Windows.Forms.Form

    '窗体重写释放，以清理组件列表。
    <System.Diagnostics.DebuggerNonUserCode()> _
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

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.backgroundChangeTimer = New System.Windows.Forms.Timer(Me.components)
        Me.label1 = New System.Windows.Forms.Label
        Me.SuspendLayout()
        '
        'backgroundChangeTimer
        '
        Me.backgroundChangeTimer.Enabled = True
        Me.backgroundChangeTimer.Interval = 10000
        '
        'label1
        '
        Me.label1.AutoSize = True
        Me.label1.Location = New System.Drawing.Point(58, 212)
        Me.label1.Name = "label1"
        Me.label1.Size = New System.Drawing.Size(340, 13)
        Me.label1.TabIndex = 1
        Me.label1.Text = "此窗体在 OnPaint() 和 OnPaintBackground() 方法中绘制。"
        Me.label1.Visible = False
        '
        'ScreenSaverForm
        '
        Me.AutoScaleDimensions = New System.Drawing.SizeF(6.0!, 13.0!)
        Me.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font
        Me.ClientSize = New System.Drawing.Size(472, 459)
        Me.Controls.Add(Me.label1)
        Me.DoubleBuffered = True
        Me.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None
        Me.Name = "ScreenSaverForm"
        Me.ShowInTaskbar = False
        Me.TopMost = True
        Me.WindowState = System.Windows.Forms.FormWindowState.Maximized
        Me.ResumeLayout(False)
        Me.PerformLayout()

    End Sub

    Private WithEvents backgroundChangeTimer As System.Windows.Forms.Timer
    Friend WithEvents label1 As System.Windows.Forms.Label
End Class
