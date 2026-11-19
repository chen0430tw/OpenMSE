Public Class $safeitemname$
    Inherits System.Windows.Forms.Form
    Friend WithEvents MainMenu1 As System.Windows.Forms.MainMenu

#Region " Windows 窗体设计器生成的代码 "

    Public Sub New()
        MyBase.New()

        '此调用是 Windows 窗体设计器所必需的。
        InitializeComponent()

        '在 InitializeComponent() 调用之后添加任何初始化

    End Sub

    '窗体重写释放，以清理组件列表。
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        MyBase.Dispose(disposing)
    End Sub

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改它。
    '不要使用代码编辑器修改它。
    Private Sub InitializeComponent()
        Me.MainMenu1 = New System.Windows.Forms.MainMenu()
        Me.Menu = Me.MainMenu1
        Me.Text = "$safeitemname$"

    End Sub

#End Region

End Class
