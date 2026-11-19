Imports System.Collections.Generic
Imports System.IO


''' <summary>
''' 负责呈现屏幕保护程序的主要视觉内容的屏幕。
''' </summary>
''' <remarks>
''' 此窗体完全使用 GDI+ 图形对象进行自定义绘制。若要更改显示，
''' 请修改图形对象的代码或在窗体中放置新的用户界面控件。
''' </remarks>
Public Class ScreenSaverForm

    '存储要显示的 RSS 源项数据
    Private rss As RssFeed

    '用于显示 RSS 内容的对象
    Private rssView As ItemListView(Of RssItem)
    Private WithEvents rssDescriptionView As ItemDescriptionView(Of RssItem)
    Private WithEvents descriptionFadeTimer As Timer

    '存储在背景中显示的图像
    Private backgroundImages As List(Of Image)

    '存储当前图像的索引
    Private currentImageIndex As Integer

    ' 跟踪屏幕保护程序是否已激活。
    Private isActive As Boolean = False

    ' 跟踪鼠标的位置
    Private mouseLocation As Point

    '包含所有可显示的图像的数组
    Private IMAGE_FILE_EXTENSIONS As String() = {"*.bmp", "*.gif", "*.png", "*.jpg", "*.jpeg"}

    Public Sub New()
        InitializeComponent()

        SetupScreenSaver()
        LoadBackgroundImage()

        LoadRssFeed()

        ' 初始化 ItemListView 以显示 RssItem 的说明信息。
        ' 该信息将出现在屏幕的右侧。            
        rssView = New ItemListView(Of RssItem)(Me.rss.MainChannel.Title, Me.rss.MainChannel.Items)
        InitializeRssView()

        ' 初始化 ItemDescriptionView 以显示 RssItem 的说明信息。
        ' 该信息将出现在屏幕的右侧。            
        rssDescriptionView = New ItemDescriptionView(Of RssItem)
        InitializeRssDescriptionView()

        descriptionFadeTimer = rssDescriptionView.FadeTimer

    End Sub

   

    '''<summary>
    '''将主窗体设置为全屏显示的屏幕保护程序。
    '''</summary>
    Private Sub SetupScreenSaver()
        ' 使用双缓冲来改进绘制性能
        Me.SetStyle(ControlStyles.OptimizedDoubleBuffer Or ControlStyles.UserPaint Or ControlStyles.AllPaintingInWmPaint, True)
        ' 捕获鼠标
        Me.Capture = True

        ' 将应用程序设置为全屏模式并隐藏鼠标
        Windows.Forms.Cursor.Hide()
        Bounds = Screen.PrimaryScreen.Bounds
        WindowState = FormWindowState.Maximized
        ShowInTaskbar = False
        DoubleBuffered = True
        BackgroundImageLayout = ImageLayout.Stretch

    End Sub


    ''' <summary>
    ''' 将图像从指定位置加载到内存中进行显示。
    ''' </summary>
    ''' <remarks>
    ''' 尝试从用户指定的路径加载图像，否则使用资源设计器
    ''' 中保存的默认图像。
    ''' </remarks>
    Private Sub LoadBackgroundImage()

        ' 初始化背景图像
        backgroundImages = New List(Of Image)
        currentImageIndex = 0

        If Directory.Exists(My.Settings.BackgroundImagePath)
            Try
                ' 尝试加载用户指定的图像
                LoadImagesFromFolder()
            Catch
                ' 如果失败，则加载默认图像
                LoadDefaultBackgroundImages()
            End Try
        End If

        If backgroundImages.Count = 0
            LoadDefaultBackgroundImages()
        End If

    End Sub

    Private Sub LoadImagesFromFolder()

        Dim currentImage As Image
        Dim backgroundImageDir As DirectoryInfo = new DirectoryInfo(My.Settings.BackgroundImagePath)

        ' 对于每种图像扩展名(.jpg、.bmp 等)
        For Each imageExtension As String In IMAGE_FILE_EXTENSIONS
            ' 对于用户提供的目录中的每个文件
            For Each file As FileInfo In backgroundImageDir.GetFiles(imageExtension)
                ' 尝试加载图像
                Try
                    currentImage = Image.FromFile(file.FullName)
                    backgroundImages.Add(currentImage)
                Catch ex As OutOfMemoryException
                    ' 如果图像无法加载，则继续。
                    Continue For
                End Try
            Next
        Next

    End Sub

    Private Sub LoadDefaultBackgroundImages()
        ' 如果出于某种原因无法加载背景图像，
        ' 则使用资源中存储的图像
        backgroundImages.Add(My.Resources.SSaverBackground)
        backgroundImages.Add(My.Resources.SSaverBackground2)
    End Sub

    ''' <summary>
    ''' 从用户指定的位置加载 RSS 源数据。
    ''' </summary>
    ''' <remarks>尝试从用户指定的 URI 加载，如果失败则从“设置”设计器加载静态的错误文本数据。</remarks>
    Private Sub LoadRssFeed()
        Try
            ' 尝试从用户设置获取该数据
            rss = RssFeed.FromUri(My.Settings.RssFeedUri)
        Catch ex As Exception
            My.Application.Log.WriteEntry(String.Format("从指定的 URI 加载 RSS 源时出现了问题: {0}", ex.Message))
            My.Application.Log.WriteException(ex)

            ' 如果加载 RSS 时出现了问题，则加载一个错误信息 RSS 源
            rss = RssFeed.FromText(My.Resources.DefaultRSSText)
        End Try

    End Sub

    ''' <summary>
    ''' 初始化 rssView 的显示属性。
    ''' </summary>
    Private Sub InitializeRssView()
        rssView.BackColor = Color.FromArgb(120, 240, 234, 232)
        rssView.BorderColor = Color.White
        rssView.ForeColor = Color.FromArgb(255, 40, 40, 40)
        rssView.SelectedBackColor = Color.FromArgb(200, 105, 61, 76)
        rssView.SelectedForeColor = Color.FromArgb(255, 204, 184, 163)
        rssView.TitleBackColor = Color.Empty
        rssView.TitleForeColor = Color.FromArgb(255, 240, 234, 232)
        rssView.MaxItemsToShow = 20
        rssView.MinItemsToShow = 15
        rssView.Location = New Point(Width / 10, Height / 10)
        rssView.Size = New Size(Width / 2, Height / 2)
    End Sub

    ''' <summary>
    ''' 初始化 rssDescriptionView 的显示属性。
    ''' </summary>
    Private Sub InitializeRssDescriptionView()
        rssDescriptionView.DisplayItem = rssView.SelectedItem
        rssDescriptionView.ForeColor = Color.FromArgb(255, 240, 234, 232)
        rssDescriptionView.TitleFont = rssView.TitleFont
        rssDescriptionView.LineColor = Color.FromArgb(120, 240, 234, 232)
        rssDescriptionView.LineWidth = 2.0F

        rssDescriptionView.FadeTimer.Interval = 40
        rssDescriptionView.Location = New Point(3 * Width / 4, Height / 3)
        rssDescriptionView.Size = New Size(Width / 4, Height / 2)

    End Sub

    Private Sub ScreenSaverForm_MouseMove(ByVal sender As Object, ByVal e As MouseEventArgs) Handles Me.MouseMove
        ' 仅在第一次调用此事件时设置 IsActive 和 MouseLocation。
        If Not isActive Then
            mouseLocation = MousePosition
            isActive = True
        Else
            ' 如果在第一次调用后鼠标发生明显的移动，则关闭。
            If Math.Abs(MousePosition.X - mouseLocation.X) > 10 OrElse Math.Abs(MousePosition.Y - mouseLocation.Y) > 10 Then
                Close()
            End If
        End If

    End Sub


    Private Sub ScreenSaverForm_KeyDown(ByVal sender As Object, ByVal e As KeyEventArgs) Handles Me.KeyDown
        Close()
    End Sub


    Private Sub ScreenSaverForm_MouseDown(ByVal sender As Object, ByVal e As MouseEventArgs) Handles Me.MouseDown
        Close()
    End Sub


    Protected Overrides Sub OnPaintBackground(ByVal e As PaintEventArgs)
        ' 绘制当前背景图像并拉伸至全屏
        e.Graphics.DrawImage(backgroundImages(currentImageIndex), 0, 0, Size.Width, Size.Height)

    End Sub


    Protected Overrides Sub OnPaint(ByVal e As PaintEventArgs)
        rssView.Paint(e)
        rssDescriptionView.Paint(e)

    End Sub


    Private Sub backgroundChangeTimerTick(ByVal sender As Object, ByVal e As EventArgs) Handles backgroundChangeTimer.Tick
        ' 将背景图像更改为下一幅图像。
        currentImageIndex = (currentImageIndex + 1) Mod backgroundImages.Count

    End Sub


    Private Sub rssDescriptionView_FadingComplete(ByVal sender As Object, ByVal e As System.EventArgs) Handles rssDescriptionView.FadingComplete
        rssView.NextArticle()
        rssDescriptionView.DisplayItem = rssView.SelectedItem

    End Sub


    Private Sub FadeTimer_Tick(ByVal sender As Object, ByVal e As System.EventArgs) Handles descriptionFadeTimer.Tick
        Me.Refresh()

    End Sub

End Class