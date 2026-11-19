Public Class $safeitemname$
    Inherits System.ComponentModel.Component

#Region " 组件设计器生成的代码 "

    Public Sub New(Container As System.ComponentModel.IContainer)
        MyClass.New()

        'Windows.Forms 类撰写设计器支持所必需的
        Container.Add(CType(Me, System.ComponentModel.IComponent))
    End Sub

    Public Sub New()
        MyBase.New()

        '组件设计器需要此调用。
        InitializeComponent()

        '在 InitializeComponent() 调用之后添加任何初始化

    End Sub

    '组件设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是组件设计器所必需的
    '可使用组件设计器修改它。
    '不要使用代码编辑器修改它。
    <System.Diagnostics.DebuggerStepThrough()> _
    Private Sub InitializeComponent()
        components = New System.ComponentModel.Container()
    End Sub

#End Region

End Class
