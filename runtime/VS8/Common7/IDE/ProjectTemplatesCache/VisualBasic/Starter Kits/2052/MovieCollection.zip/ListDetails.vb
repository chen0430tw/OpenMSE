''' <summary>
''' 用户控件负责查看、管理和实时编辑收藏的电影的详细信息。  
''' </summary>
''' <remarks>
''' 通过 SearchOnline 控件以编程的方式添加电影，或在按下“添加”按钮后手动添加。 
''' 可通过选择网格中的一个值实时编辑电影。可通过选择并按下“删除”按钮删除电影。
''' 按下“联机观看”按钮以访问 Amazon.com 网站。按下“刷新”按钮 
''' 用来自 Amazon.com Web 服务的实时信息刷新数据字段。  
''' 电影从 MovieDatabase.mdf 中读取并向其写入。建议定期刷新  
''' 电影信息。  
''' </remarks>
Public Class ListDetails

    '表示电影收藏在内存中的表示形式。类型在 DVDCollectionDataSet.xsd 文件中定义。
    '此变量再次在局部定义为 WithEvents，以使得在代码编辑器中处理事件变得更容易。  
    Dim WithEvents dvdsDataTable As $safeprojectname$.DVDCollectionDataSet.DVDsDataTable

    Dim WithEvents hostingForm As Form

    '负责对电影标题进行分级的控件对象。  
    '解决方法: 在使用“新建项目”向导后，项目无法动态生成此自定义控件类型；
    '                      控件实例将在运行时创建和布置，而不会
    '                      添加到设计图面。  
    Friend WithEvents RatingPickerControl1 As RatingPickerControl

    '在对数据库进行更新时打开。
    Dim IsUpdating As Boolean = False

    ''' <summary>
    ''' 初始化此控件并加载数据。     
    ''' </summary>
    ''' <remarks>
    ''' 将收藏中的所有电影标题加载到内存中的数据表；   
    ''' 此控件监视对数据表的更改，并自动保存对每个字段进行的更改。
    ''' </remarks>
    Private Sub ListDetails_Load(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.Load
        '动态创建分级选择器控件并布置在窗体上
        InitRatingPickerControl()

        '引用设计图面上的 DVD 数据表
        Me.dvdsDataTable = Me.DVDCollectionDataSet.DVDs

        '从数据库加载数据
        If Me.LoadData() = 0 Then
            ' 如果无可用行，则创建新行，以便用户可以开始输入新的 dvd
            Me.DVDsBindingSource.AddNew()
        End If

        Me.TitleTextBox.Select()
    End Sub

    Private Sub dt_rowChanged(ByVal sender As Object, ByVal e As Data.DataRowChangeEventArgs) Handles dvdsDataTable.RowChanged
        If Me.IsUpdating Then
            Return
        End If
        Dim dvdDataRow As DVDCollectionDataSet.DVDsRow = CType(e.Row, DVDCollectionDataSet.DVDsRow)
        If dvdDataRow IsNot Nothing Then
            Me.SaveData()
        End If
    End Sub

    ''' <summary>
    ''' 单击“添加”按钮时在收藏中创建新电影。  
    ''' </summary>
    ''' <remarks>从 DataConnector 调用 AddNew，以保持控件与数据表之间的数据绑定同步这一做法非常可取。  </remarks>
    Private Sub AddTitleButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles AddTitleButton.Click
        Me.DVDsBindingSource.AddNew()
    End Sub

    ''' <summary> 删除当前选中的电影。  </summary>
    Private Sub RemoveButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RemoveButton.Click
        Me.DeleteCurrent()
    End Sub

    ''' <summary>单击时应用用户指定的筛选器。  </summary>
    Private Sub FilterButton_Clicked(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles FilterButton.Click
        Me.ApplyTextboxFilter()
    End Sub

    ''' <summary>输入筛选器字符串时应用用户指定的筛选器。</summary>
    Private Sub FilterTextBox_KeyUp(ByVal sender As Object, ByVal e As System.Windows.Forms.KeyEventArgs) Handles FilterTextBox.KeyUp
        If e.KeyCode = Keys.Enter Then
            Me.ApplyTextboxFilter()
        End If
    End Sub

    ''' <summary>单击此按钮时将 DataConnector 重置为未筛选状态。</summary>
    Private Sub ShowAllButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ShowAllImage.Click, ShowAllButton.Click
        Me.DVDsBindingSource.Filter = ""
    End Sub

    ''' <summary>
    ''' 在提示用户后用来自 Amazon.com Web 服务的数据刷新当前的电影选择。  
    ''' </summary>
    ''' <remarks>
    ''' 使用 UPC 值从 Web 服务获取电影标题，并合并/重写  
    ''' 现有的数据。“Comments”和“MyRating”字段是作为特例保留的。  
    ''' </remarks>
    Private Sub RefreshOnlineButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles RefreshOnlineButton.Click
        '提示用户刷新并存储结果
        Dim yesNoResult As MsgBoxResult = MsgBox("此功能可能会改写您的更改。要继续吗?", MsgBoxStyle.YesNo, "是否从联机 Web 服务刷新数据?")

        '如果是，则开始刷新过程
        If yesNoResult = MsgBoxResult.Yes Then
            '用于调用 Amazon.com Web 服务的对象
            Dim amazonService As New SimpleAmazonWS

            '用于存储搜索结果的对象
            Dim refreshedDVD As DVD
            Dim refreshedDVDRow As $safeprojectname$.DVDCollectionDataSet.DVDsRow

            Try
                '边界检查: 确保电影数据表不为空
                If Me.DVDsBindingSource.Count > 0 Then
                    '获取当前电影选择并转换为类型化的电影行对象
                    Dim rowView As DataRowView = CType(Me.DVDsBindingSource.Current, DataRowView)
                    Dim dvdRow As DVDCollectionDataSet.DVDsRow = CType(rowView.Row, $safeprojectname$.DVDCollectionDataSet.DVDsRow)

                    ' 检查 UPC 值是否为空
                    If (Not dvdRow.IsUPCNull) AndAlso dvdRow.UPC <> "" Then

                        '用于存储和保留自定义评价与分级值的变量
                        Dim currentComments As String = dvdRow.Comments
                        Dim currentRating As Integer = dvdRow.MyRating

                        '从 Web 服务中获取更新的 DVD 对象
                        refreshedDVD = amazonService.RefreshDVDByUPC(dvdRow.UPC)

                        '更新缓存的图像
                        Me.BoxArtPictureBox.Load(refreshedDVD.ImageUrl)
                        refreshedDVD.ImageCache = Me.BoxArtPictureBox.Image

                        '将 DVD 转换为 dvdRow
                        refreshedDVDRow = Me.DVDCollectionDataSet.DVDs.DVDsRowFromDVDObject(refreshedDVD)

                        '设置为原始行的 ID，从而可以成功执行对内存中的数据表的行搜索
                        refreshedDVDRow.ID = dvdRow.ID

                        '恢复/合并回自定义评价和分级值
                        refreshedDVDRow.Comments = currentComments
                        refreshedDVDRow.MyRating = currentRating

                        '将 dvdRow 与匹配行合并
                        Me.DVDCollectionDataSet.DVDs.LoadDataRow(refreshedDVDRow.ItemArray, LoadOption.Upsert)
                    End If
                End If

            Catch ex As Exception
                MsgBox(String.Format("从联机 Web 服务刷新数据时出现问题: {0}", ex.Message))
                My.Application.Log.WriteException(ex)
            End Try
        End If

    End Sub

    ''' <summary>
    ''' 提示用户后，在外部浏览器中导航到 Amazon.com 网页，以进行当前电影选择。 
    ''' </summary>
    ''' <remarks>在启动浏览器进程和导航到 URL 之前，用户应查看此超链接。  </remarks>
    Private Sub ViewOnlineButton_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles ViewOnlineButton.Click

        '边界检查: 确保电影数据表不为空
        If Me.DVDsBindingSource.Count > 0 Then
            '获取当前电影选择并转换为类型化的电影行对象
            Dim rowView As DataRowView = CType(Me.DVDsBindingSource.Current, DataRowView)
            Dim dvdRow As DVDCollectionDataSet.DVDsRow = CType(rowView.Row, $safeprojectname$.DVDCollectionDataSet.DVDsRow)

            '用于存储网页 URL 值的变量
            Dim webLink As String = ""

            '访问 WebPageLink 属性之前检查其是否为空
            If Not dvdRow.IsWebPageLinkNull Then webLink = dvdRow.WebPageLink

            '提示查看最终用户是否信任 URL 并存储提示结果值
            Dim msgText As String = String.Format("将要在外部浏览器中查看此 URL: '{0}'。继续操作是否安全?", webLink)
            Dim yesNoResult As MsgBoxResult = MsgBox(msgText, MsgBoxStyle.YesNo, "确定要浏览?")

            '如果用户接受提示信息，则启动 URL 路径
            If (yesNoResult = MsgBoxResult.Yes) AndAlso (webLink <> "") Then
                Process.Start(webLink)
            End If
        End If
    End Sub

    ''' <summary>
    ''' 单击时提示用户要导入的文件。  
    ''' </summary>
    Private Sub ImportButton_Click(ByVal sender As Object, ByVal e As System.EventArgs) Handles ImportButton.Click
        Me.OpenFileDialog1.ShowDialog()
    End Sub

    ''' <summary>
    ''' 用户单击“确定”打开文件提示时，导入用户提供的图像。   
    ''' </summary>
    Private Sub OpenFileDialog1_FileOk(ByVal sender As System.Object, ByVal e As System.ComponentModel.CancelEventArgs) Handles OpenFileDialog1.FileOk
        Me.ImportImage(Me.OpenFileDialog1.FileName)
    End Sub

    ''' <summary>
    ''' 加载电影收藏数据。  
    ''' </summary>
    ''' <returns>正整数表示加载的行数；若没有行被加载则为 0。  </returns>
    ''' <remarks>将电影收藏数据加载到内存中的数据表</remarks>
    Friend Function LoadData() As Integer
        Dim rowsLoaded As Integer = 0

        '试图使用对所有电影的查询来填充数据表。
        '注意: 如果数据库中有大量的电影，您可能要添加对
        'TableAdapter 的查询，以便只将数据库中的一部分电影填入内存中。
        Try
            Me.IsUpdating = True
            rowsLoaded = Me.DVDsTableAdapter.Fill(Me.DVDCollectionDataSet.DVDs)
        Catch ex As Exception
            MsgBox(String.Format("加载数据时出现问题: {0}", ex.Message))
            My.Application.Log.WriteException(ex)
        Finally
            Me.IsUpdating = False
        End Try

        Return rowsLoaded
    End Function

    ''' <summary>
    ''' 保存对电影收藏数据进行的更改。   
    ''' </summary>
    ''' <returns>正整数表示数据库中更新的行数；若没有行被更新则为 0。  </returns>
    ''' <remarks>保存来自内存中的数据表和绑定 UI 的更改。  </remarks>
    Friend Function SaveData() As Integer
        If IsUpdating Then
            Return 0
        End If
        Dim rowsSaved As Integer = 0

        '尝试获取更改并将更改保存到数据库
        Try
            '只在检测到更改时更新数据库
            If Me.DVDCollectionDataSet.HasChanges Then
                IsUpdating = True
                rowsSaved = Me.DVDsTableAdapter.Update(Me.DVDCollectionDataSet.DVDs)
            End If
        Catch ex As Exception
            MsgBox(String.Format("保存数据时出现问题: {0}", ex.Message))
            My.Application.Log.WriteException(ex)
        Finally
            IsUpdating = False
        End Try
        Return rowsSaved
    End Function

    ''' <summary>
    ''' 使用文本框中用户指定的输入对内存中电影记录应用筛选器。  
    ''' </summary>
    ''' <remarks>DataConnector 对象的“Filter”属性适用于绑定到 DataConnector 的所有控件。</remarks>
    Private Sub ApplyTextboxFilter()
        Try
            '将筛选字符串 WHERE 子句设置为“Where COLUMNNAME like '@FilterText'”的格式
            Me.DVDsBindingSource.Filter = _
                  String.Format("{0} like '%{1}%'", _
                                Me.DVDCollectionDataSet.DVDs.TitleColumn.ColumnName, _
                                Me.FilterTextBox.Text)
        Catch
        End Try
    End Sub

    ''' <summary>提示用户后删除当前选中的电影。  </summary>
    Friend Sub DeleteCurrent()
        '执行删除之前执行边界检查: 数据表是否为空，或者是否未进行选择
        If (Me.DVDsBindingSource.Count > 0) AndAlso (Me.DVDsBindingSource.Current IsNot Nothing) Then
            '将 DataConnector 返回的泛型 Current 对象转换为类型化的电影行对象
            Dim rowView As DataRowView = CType(Me.DVDsBindingSource.Current, DataRowView)
            Dim dvdRow As DVDCollectionDataSet.DVDsRow = CType(rowView.Row, $safeprojectname$.DVDCollectionDataSet.DVDsRow)

            '表示要删除的当前电影的友好名称的字符串
            Dim title As String = "[无标题]"

            '使用类型化行对象中的电影标题属性(如果该属性包含有效数据)
            If (Not dvdRow.IsTitleNull) AndAlso (dvdRow.Title IsNot Nothing) Then title = dvdRow.Title

            '删除前在用户提示中显示的文字
            Dim msgText As String = String.Format("确实要从电影收藏中永久删除 {0}? ", title)

            '向用户显示“是/否”提示并存储结果
            Dim result As MsgBoxResult = MsgBox(msgText, MsgBoxStyle.YesNo, "确定要删除?")

            '如果用户接受提示信息，则移除当前电影
            If result = MsgBoxResult.Yes Then
                Me.DVDsBindingSource.RemoveCurrent()
            End If

            ' 确保始终有可用行
            If DVDsBindingSource.Count = 0 Then
                Me.DVDsBindingSource.AddNew()
            End If
        End If
    End Sub

    ''' <summary>
    ''' 初始化此用户控件使用的分级选择器控件。 
    ''' </summary>
    ''' <remarks>
    ''' 由 ListDetails.Load 事件调用。若在单独的控件库中生成和引用 RatingPickerControl，
    ''' 则此特殊的初始化代码不是必需的。  
    ''' </remarks>
    Private Sub InitRatingPickerControl()
        '创建分级选择器控件对象并将其布置在父用户控件上
        Me.RatingPickerControl1 = New RatingPickerControl
        Me.DVDDetailsPanel.Controls.Add(Me.RatingPickerControl1)

        '设置默认属性和数据绑定
        CType(Me.RatingPickerControl1, System.ComponentModel.ISupportInitialize).BeginInit()
        Me.RatingPickerControl1.Anchor = CType((System.Windows.Forms.AnchorStyles.Top Or System.Windows.Forms.AnchorStyles.Right), System.Windows.Forms.AnchorStyles)
        Me.RatingPickerControl1.BackColor = System.Drawing.Color.Transparent
        Me.RatingPickerControl1.CurrentRating = -1
        Me.RatingPickerControl1.DataBindings.Add("CurrentRating", Me.DVDsBindingSource, "MyRating", True, DataSourceUpdateMode.OnPropertyChanged)
        Me.RatingPickerControl1.Location = New System.Drawing.Point(128, 388)
        Me.RatingPickerControl1.Name = "RatingPickerControl1"
        Me.RatingPickerControl1.Size = New System.Drawing.Size(80, 16)
        Me.RatingPickerControl1.TabIndex = 7
        CType(Me.RatingPickerControl1, System.ComponentModel.ISupportInitialize).EndInit()
    End Sub

    ''' <summary>
    ''' 从用户提供的文件中将图像值导入到数据表。  
    ''' </summary>
    ''' <remarks></remarks>
    Private Sub ImportImage(ByVal filename As String)
        Try
            '执行删除之前执行边界检查: 数据表是否为空，或者是否未进行选择
            If Me.DVDsBindingSource.Current IsNot Nothing Then
                '将 DataConnector 返回的泛型 Current 对象转换为类型化的电影行对象
                Dim rowView As DataRowView = CType(Me.DVDsBindingSource.Current, DataRowView)
                Dim dvdRow As DVDCollectionDataSet.DVDsRow = CType(rowView.Row, $safeprojectname$.DVDCollectionDataSet.DVDsRow)

                '从文件系统以只读方式打开文件，复制字节，然后赋给当前行的图像属性
                dvdRow.ImageBinary = My.Computer.FileSystem.ReadAllBytes(filename)

                Me.DVDsBindingSource.ResetCurrentItem()
            End If

        Catch ex As Exception
            MsgBox(String.Format("加载图像时出现问题: {0}", ex.Message))
            My.Application.Log.WriteException(ex)
        End Try
    End Sub

    Private Sub hostingForm_FormClosed(ByVal sender As Object, ByVal e As System.Windows.Forms.FormClosedEventArgs)
        Try
            '调用 EndEdit 来捕获 UI 中所做的所有更改
            Me.DVDsBindingSource.EndEdit()
        Catch ex As Exception
            MsgBox(String.Format("保存数据时出现问题: {0}", ex.Message))
            My.Application.Log.WriteException(ex)
        End Try

        ' 当父窗体关闭时，请确保对此控件的数据进行保存
        Me.SaveData()
    End Sub

    Private Sub ListDetails_ParentChanged(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles MyBase.ParentChanged
        ' 如果此用户控件的父窗体进行过更改，则需要重置新父窗体的 form.Closed 事件处理程序
        If hostingForm IsNot Nothing Then
            RemoveHandler hostingForm.FormClosed, AddressOf hostingForm_FormClosed
        End If
        Me.hostingForm = Me.ParentForm
        AddHandler hostingForm.FormClosed, AddressOf hostingForm_FormClosed

    End Sub

    Private Sub TitleTextBox_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles TitleTextBox.Click
        If Me.TitleTextBox.Text = "[新行 - 选择以输入详细信息]" Then
            Me.TitleTextBox.SelectAll()
        End If
    End Sub
End Class
