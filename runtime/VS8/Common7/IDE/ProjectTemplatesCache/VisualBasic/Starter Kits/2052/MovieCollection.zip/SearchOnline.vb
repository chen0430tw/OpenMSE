Imports System.ComponentModel
''' <summary>
''' 负责搜索 Amazon.com Web 服务以查找电影标题及将标题添加到收藏的用户控件。 
''' </summary>
''' <remarks>
''' 单击“搜索”按钮或在键入搜索文本后回车，以执行联机搜索。  
''' 选择一个有效的电影标题并单击“添加到收藏”以将数据保存到本地。  
''' 简化的 Amazon.com 搜索类返回 DVD 对象的泛型 BindingList。用户控件 UI 元素是
''' 使用“数据源对象”绑定功能绑定到 DVD 列表的数据。将标题添加到收藏
''' 将 DVD 对象转换为内存中数据表的格式，应用程序的其余部分使用这种格式。  
''' </remarks>
Public Class SearchOnline

    ''' <summary>调用 Amazon.com Web 服务并将 DVD bindinglist 结果绑定到 UI。</summary>
    Private Sub PerformSearch()
        '负责包含 DVD 搜索结果的对象
        Dim searchResults As New BindingList(Of DVD)

        '简单包装对象负责处理来自 Amazon.com Web 服务的请求和响应
        Dim amazonService As New SimpleAmazonWS

        '在搜索过程中显示沙漏，以通知用户搜索正在进行
        Me.Cursor = Cursors.WaitCursor

        Try
            '请求 Web 服务根据用户的搜索条件返回搜索结果
            searchResults = amazonService.SearchDVDs(Me.SearchTextBox.Text)

            '将搜索结果数据绑定到窗体 UI
            Me.DVDBindingSource.DataSource = searchResults
        Catch ex As Exception
            MsgBox(String.Format("连接到 Web 服务时出现问题。请确认您已连接到 Internet。其他详细信息: {0}", ex.Message))
            My.Application.Log.WriteException(ex)
        Finally
            '工作完成后，将光标重新设置为默认值
            Me.Cursor = Cursors.Default
        End Try

        '告知用户查找到的结果数目。使用 String.Format 功能按照便于本地化的方式连接字符串。
        Me.Label2.Text = String.Format("找到了 {0} 个结果。  ", searchResults.Count.ToString)
    End Sub

    ''' <summary>将当前电影选择添加到本地电影收藏中，然后显示 ListDetails 用户控件。</summary>
    Private Sub AddCurrentToCollection()
        '尝试将数据添加到收藏中
        Try
            '检查边界情况: DVD 列表是否为空或者当前是否未进行选择
            If (Me.DVDBindingSource.Count > 0) AndAlso (Me.DVDBindingSource.Current IsNot Nothing) Then
                '存储当前 DVD 选定内容的对象
                Dim currentDVD As DVD = CType(Me.DVDBindingSource.Current, DVD)

                '尝试将图像缓存为 DVD 对象
                If Me.PictureBox1.Image IsNot Nothing Then
                    '首先尝试从图片框加载
                    currentDVD.ImageCache = Me.PictureBox1.Image
                ElseIf currentDVD.ImageUrl <> "" Then
                    '否则尝试从源 URL 加载
                    Me.PictureBox1.Load(currentDVD.ImageUrl)
                    currentDVD.ImageCache = Me.PictureBox1.Image
                End If

                '将自定义 DVD 对象转换为该应用程序所使用的数据表/数据行格式，并
                '将数据行添加到数据表
                My.Forms.MainForm.ListDetailsPart.DVDCollectionDataSet.DVDs.AddDVDsRow(currentDVD)

                '更新显示，以显示选中了新行的 ListDetailsPart
                My.Forms.MainForm.ListDetailsPart.DVDsBindingSource.MoveLast()
                My.Forms.MainForm.ShowListDetailsPart()
            Else
                MsgBox("未选择 DVD。请在添加到收藏前先选择一个 DVD。")
            End If

        Catch ex As Exception
            MsgBox("向收藏添加此 DVD 时出现问题。")
            My.Application.Log.WriteException(ex)
        End Try
    End Sub

    ''' <summary>单击按钮后执行联机搜索。</summary>
    Private Sub SearchButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles SearchButton1.Click
        Me.PerformSearch()
    End Sub

    ''' <summary>在搜索文本框中按下回车后执行联机搜索。  </summary>
    Private Sub SearchTextBox_KeyUp(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles SearchTextBox.KeyUp
        If e.KeyCode = Keys.Enter Then
            Me.PerformSearch()
        End If
    End Sub

    ''' <summary>按下按钮后将当前选定内容添加到本地收藏。</summary>
    Private Sub AddToCollectionButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AddToCollectionButton.Click
        Me.AddCurrentToCollection()
    End Sub

    ''' <summary>双击 DataGridView 将当前选定内容添加到本地收藏中。</summary>
    Private Sub DVDDataGridView_DoubleClick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles DVDDataGridView.DoubleClick
        If Me.DVDBindingSource.Current IsNot Nothing Then
            Me.AddCurrentToCollection()
        End If
    End Sub
End Class
