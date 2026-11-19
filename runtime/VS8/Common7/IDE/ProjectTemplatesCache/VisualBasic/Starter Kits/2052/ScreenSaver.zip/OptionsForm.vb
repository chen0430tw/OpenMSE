

'''<summary>
''' 负责读写常见用户设置的屏幕。
''' </summary>
Public Class OptionsForm

    Private Sub OptionsForm_Load(ByVal sender As Object, ByVal e As System.EventArgs) Handles Me.Load
        ' 从当前设置加载文本框
        Try
            backgroundImageFolderTextBox.Text = My.Settings.BackgroundImagePath
            rssFeedTextBox.Text = My.Settings.RssFeedUri
        Catch
            MessageBox.Show("读入屏幕保护程序的设置时出现了问题。")
        End Try
    End Sub

    '''<summary>
    '''仅在自上次按下“应用”按钮后
    '''发生了更改的情况下将该按钮更新为活动状态
    '''</summary>
    Private Sub UpdateApply()
        If My.Settings.BackgroundImagePath <> backgroundImageFolderTextBox.Text OrElse My.Settings.RssFeedUri <> rssFeedTextBox.Text Then
            applyButton.Enabled = True
        Else
            applyButton.Enabled = False
        End If
    End Sub

    ''' <summary>
    ''' 应用自上次按下“应用”按钮后所做的所有更改
    ''' </summary>
    Private Sub ApplyChanges()
        My.Settings.BackgroundImagePath = backgroundImageFolderTextBox.Text
        My.Settings.RssFeedUri = rssFeedTextBox.Text
        My.Settings.Save()
    End Sub


    Private Sub btnOK_Click(ByVal sender As Object, ByVal e As EventArgs) Handles okButton.Click
        Try
            ApplyChanges()
        Catch ex As Exception
            MessageBox.Show("无法保存您的设置。请确保屏幕保护程序所在的目录包含一个 .config 文件", "未能保存设置", MessageBoxButtons.OK, MessageBoxIcon.Error)
        Finally
            Close()
        End Try
    End Sub


    Private Sub btnCancel_Click(ByVal sender As Object, ByVal e As EventArgs) Handles cancelButton1.Click
        Close()
    End Sub


    Private Sub btnApply_Click(ByVal sender As Object, ByVal e As EventArgs) Handles applyButton.Click
        ApplyChanges()
        applyButton.Enabled = False
    End Sub



    Private Sub validateButton_Click(ByVal sender As Object, ByVal e As EventArgs) Handles validateButton.Click
        ' 检查用户提供的 URI 是否指向一个有效的 RSS 源
        Try
            RssFeed.FromUri(rssFeedTextBox.Text)
        Catch ex As Exception
            MessageBox.Show("不是一个有效的 RSS 源。", "不是一个有效的 RSS 源。", MessageBoxButtons.OK, MessageBoxIcon.Error)
            Return
        End Try

        MessageBox.Show("有效的 RSS 源。", "有效的 RSS 源。", MessageBoxButtons.OK, MessageBoxIcon.Exclamation)

    End Sub

    Private Sub browseButton_Click(ByVal sender As Object, ByVal e As EventArgs) Handles browseButton.Click
        ' 打开一个“打开文件”对话框，以选择一幅图像
        Dim result As DialogResult = backgroundImageFolderBrowser.ShowDialog()
        If result = Windows.Forms.DialogResult.OK Then
            backgroundImageFolderTextBox.Text = backgroundImageFolderBrowser.SelectedPath
            UpdateApply()
        End If

    End Sub

    Private Sub rssFeedTextBox_TextChanged(ByVal sender As Object, ByVal e As EventArgs) Handles rssFeedTextBox.TextChanged
        UpdateApply()
    End Sub

    Private Sub backgroundImageFolderTextBox_TextChanged(ByVal sender As Object, ByVal e As EventArgs) Handles backgroundImageFolderTextBox.TextChanged
        UpdateApply()
    End Sub

End Class