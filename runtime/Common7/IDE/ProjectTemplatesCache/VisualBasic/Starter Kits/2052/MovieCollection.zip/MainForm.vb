''' <summary>
''' 宿主窗体负责在应用程序的观看和联机搜索模式之间导航。  
''' </summary>
''' <remarks>在设计器中查看 MainForm.vb 文件，修改其他属性。</remarks>
Public Class MainForm

    '用户控件对象负责查看收藏和联机搜索
    Dim m_ListDetailsPart As ListDetails
    Dim m_SearchOnlinePart As SearchOnline

    ''' <summary>
    ''' 获取负责查看电影收藏的用户控件实例。  
    ''' </summary>
    ''' <value>ListDetails 用户控件的活实例。  </value>
    ''' <remarks>单一实例模式；在第一次访问时创建该对象，此后的所有访问将重用该对象。</remarks>
    Friend ReadOnly Property ListDetailsPart() As ListDetails
        Get
            '如果变量为空，使用一个新的对象实例初始化该变量
            If Me.m_ListDetailsPart Is Nothing Then
                '正在创建对象
                Me.m_ListDetailsPart = New ListDetails

                '将此控件布置在宿主用户控件上，并停靠填充它。
                Me.TargetPanel.Controls.Add(Me.m_ListDetailsPart)
                Me.m_ListDetailsPart.Dock = DockStyle.Fill
            End If

            Return Me.m_ListDetailsPart
        End Get
    End Property

    ''' <summary>
    ''' 获取负责查看电影收藏的用户控件属性。  
    ''' </summary>
    ''' <value>ListDetails 用户控件的活实例。  </value>
    ''' <remarks>单一实例模式；在第一次访问时创建该对象，此后的所有访问将重用该对象。</remarks>
    Friend ReadOnly Property SearchOnlinePart() As SearchOnline
        Get
            '如果变量为空，使用一个新的对象实例初始化该变量
            If Me.m_SearchOnlinePart Is Nothing Then
                '正在创建对象
                Me.m_SearchOnlinePart = New SearchOnline

                '将此控件布置在宿主用户控件上，并停靠填充它。
                Me.TargetPanel.Controls.Add(Me.m_SearchOnlinePart)
                Me.m_SearchOnlinePart.Dock = DockStyle.Fill
            End If

            Return Me.m_SearchOnlinePart
        End Get
    End Property

    ''' <summary>在第一次加载应用程序时显示 ListDetails 用户控件。</summary>
    Private Sub MainForm_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        Me.ShowListDetailsPart()
    End Sub

    ''' <summary>单击任务栏图标时恢复窗口。</summary>
    Private Sub NotifyIcon1_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles NotifyIcon1.Click
        Me.RestoreWindow()
    End Sub

    ''' <summary>双击任务栏图标时恢复窗口。</summary>
    Private Sub NotifyIcon1_DoubleClick(ByVal sender As Object, ByVal e As System.EventArgs) Handles NotifyIcon1.DoubleClick
        Me.RestoreWindow()
    End Sub

    ''' <summary>使 ListDetails 用户控件可见并隐藏 SearchOnline 用户控件。</summary>
    Private Sub ViewDetailsButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ViewDetailsButton.Click
        Me.ShowListDetailsPart()
    End Sub

    ''' <summary>使 SearchOnline 用户控件可见并隐藏 ListDetails 用户控件。 </summary>
    Private Sub SearchOnlineButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles SearchOnlineButton.Click
        Me.ShowSearchOnlinePart()
    End Sub

    ''' <summary>将窗口恢复为正常大小并确保其可见。</summary>
    Private Sub RestoreWindow()
        Me.WindowState = FormWindowState.Normal
        Me.Visible = True
    End Sub

    ''' <summary>
    ''' 显示 ListDetails 用户控件并隐藏 SearchOnline 用户控件。  
    ''' </summary>
    ''' <remarks>启用列表模式和搜索模式的交换行为。</remarks>
    Friend Sub ShowListDetailsPart()
        '性能优化 - 如果此控件尚未创建，则跳过控件隐藏操作
        If Me.SearchOnlinePart IsNot Nothing Then
            Me.SearchOnlinePart.Visible = False
        End If

        Me.ListDetailsPart.Visible = True
    End Sub

    ''' <summary>
    ''' 显示 SearchOnline 用户控件并隐藏 ListDetails 用户控件。  
    ''' </summary>
    ''' <remarks>启用列表模式和搜索模式的交换行为。</remarks>
    Friend Sub ShowSearchOnlinePart()
        '性能优化 - 如果此控件尚未创建，则跳过控件隐藏操作
        If Me.ListDetailsPart IsNot Nothing Then
            Me.ListDetailsPart.Visible = False
        End If

        '显示此控件并隐藏所有其他控件
        Me.SearchOnlinePart.Visible = True
    End Sub

End Class