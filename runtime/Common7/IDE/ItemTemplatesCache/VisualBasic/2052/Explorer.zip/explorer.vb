Imports System.Diagnostics
Imports System.Windows.Forms

Public Class $safeitemrootname$

    Private Sub $safeitemrootname$_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        '设置 UI
        SetUpListViewColumns()
        LoadTree()
    End Sub

    Private Sub LoadTree()
        ' TODO: 添加代码以向树视图添加项

        Dim tvRoot As TreeNode
        Dim tvNode As TreeNode

        tvRoot = Me.TreeView.Nodes.Add("Root")
        tvNode = tvRoot.Nodes.Add("TreeItem1")
        tvNode = tvRoot.Nodes.Add("TreeItem2")
        tvNode = tvRoot.Nodes.Add("TreeItem3")
    End Sub

    Private Sub LoadListView()
        ' TODO: 添加代码以根据在树视图中选择的项向列表视图中添加项

        Dim lvItem As ListViewItem
        ListView.Items.Clear()

        lvItem = ListView.Items.Add("ListViewItem1")
        lvItem.SubItems.AddRange(New String() {"Column2", "Column3"})

        lvItem = ListView.Items.Add("ListViewItem2")
        lvItem.SubItems.AddRange(New String() {"Column2", "Column3"})

        lvItem = ListView.Items.Add("ListViewItem3")
        lvItem.SubItems.AddRange(New String() {"Column2", "Column3"})
    End Sub

    Private Sub SetUpListViewColumns()
        ' TODO: 添加代码以设置列表视图中的列
        ListView.Columns.Add("Column1")
        ListView.Columns.Add("Column2")
        ListView.Columns.Add("Column3")
        SetView(View.Details)
    End Sub

    Private Sub ExitToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ExitToolStripMenuItem.Click
        '关闭此窗体
        Me.Close()
    End Sub

    Private Sub ToolBarToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ToolBarToolStripMenuItem.Click
        '切换工具条的可见性及相关菜单项的选中状态
        ToolBarToolStripMenuItem.Checked = Not ToolBarToolStripMenuItem.Checked
        ToolStrip.Visible = ToolBarToolStripMenuItem.Checked
    End Sub

    Private Sub StatusBarToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles StatusBarToolStripMenuItem.Click
        '切换状态条的可见性及相关菜单项的选中状态
        StatusBarToolStripMenuItem.Checked = Not StatusBarToolStripMenuItem.Checked
        StatusStrip.Visible = StatusBarToolStripMenuItem.Checked
    End Sub

    '更改文件夹窗格的可见性
    Private Sub ToggleFoldersVisible()
        '首先切换相关菜单项的选中状态
        FoldersToolStripMenuItem.Checked = Not FoldersToolStripMenuItem.Checked

        '同步文件夹工具栏按钮
        FoldersToolStripButton.Checked = FoldersToolStripMenuItem.Checked

        ' 折叠包含 TreeView 的面板。
        Me.SplitContainer.Panel1Collapsed = Not FoldersToolStripMenuItem.Checked
    End Sub

    Private Sub FoldersToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FoldersToolStripMenuItem.Click
        ToggleFoldersVisible()
    End Sub

    Private Sub FoldersToolStripButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FoldersToolStripButton.Click
        ToggleFoldersVisible()
    End Sub

    Private Sub SetView(ByVal View As System.Windows.Forms.View)
        '判断哪个菜单项应处于选中状态
        Dim MenuItemToCheck As ToolStripMenuItem = Nothing
        Select Case View
            Case View.Details
                MenuItemToCheck = DetailsToolStripMenuItem
            Case View.LargeIcon
                MenuItemToCheck = LargeIconsToolStripMenuItem
            Case View.List
                MenuItemToCheck = ListToolStripMenuItem
            Case View.SmallIcon
                MenuItemToCheck = SmallIconsToolStripMenuItem
            Case View.Tile
                MenuItemToCheck = TileToolStripMenuItem
            Case Else
                Debug.Fail("意外视图")
                View = View.Details
                MenuItemToCheck = DetailsToolStripMenuItem
        End Select

        '选中“视图”菜单下相应的菜单项并取消选中所有其他菜单项
        For Each MenuItem As ToolStripMenuItem In ListViewToolStripButton.DropDownItems
            If MenuItem Is MenuItemToCheck Then
                MenuItem.Checked = True
            Else
                MenuItem.Checked = False
            End If
        Next

        '最后，设置所请求的视图
        ListView.View = View
    End Sub

    Private Sub ListToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ListToolStripMenuItem.Click
        SetView(View.List)
    End Sub

    Private Sub DetailsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DetailsToolStripMenuItem.Click
        SetView(View.Details)
    End Sub

    Private Sub LargeIconsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles LargeIconsToolStripMenuItem.Click
        SetView(View.LargeIcon)
    End Sub

    Private Sub SmallIconsToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles SmallIconsToolStripMenuItem.Click
        SetView(View.SmallIcon)
    End Sub

    Private Sub TileToolStripMenuItem_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TileToolStripMenuItem.Click
        SetView(View.Tile)
    End Sub

    Private Sub OpenToolStripMenuItem_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles OpenToolStripMenuItem.Click
        Dim OpenFileDialog As New OpenFileDialog
        OpenFileDialog.InitialDirectory = My.Computer.FileSystem.SpecialDirectories.MyDocuments
        OpenFileDialog.Filter = "Text Files (*.txt)|*.txt"
        OpenFileDialog.ShowDialog(Me)

        Dim FileName As String = OpenFileDialog.FileName
        ' TODO: 添加代码以打开该文件
    End Sub

    Private Sub SaveAsToolStripMenuItem_Click(ByVal sender As Object, ByVal e As EventArgs) Handles SaveAsToolStripMenuItem.Click
        Dim SaveFileDialog As New SaveFileDialog
        SaveFileDialog.InitialDirectory = My.Computer.FileSystem.SpecialDirectories.MyDocuments
        SaveFileDialog.Filter = "Text Files (*.txt)|*.txt"
        SaveFileDialog.ShowDialog(Me)

        Dim FileName As String = SaveFileDialog.FileName
        ' TODO: 在此处添加代码，将窗体的当前内容保存到一个文件中。
    End Sub

    Private Sub TreeView_AfterSelect(ByVal sender As Object, ByVal e As System.Windows.Forms.TreeViewEventArgs) Handles TreeView.AfterSelect
        ' TODO: 根据树视图当前选定的节点，添加更改 Listview 内容的代码
        LoadListView()
    End Sub

End Class
