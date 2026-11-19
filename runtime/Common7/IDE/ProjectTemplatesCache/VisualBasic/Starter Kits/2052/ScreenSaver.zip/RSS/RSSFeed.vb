Imports System.Collections.Generic
Imports System.IO
Imports System.Net
Imports System.Xml


'''<summary>
'''表示 RSS 2.0 XML 文档中的 RSS 元素。
'''</summary>
Public Class RssFeed

    Private m_channels As List(Of RssChannel)
    
    Public ReadOnly Property Channels() As IList(Of RssChannel)
        Get
            Return Me.m_channels
        End Get
    End Property

    Public ReadOnly Property MainChannel() As RssChannel
        Get
            Return Channels(0)
        End Get
    End Property

    ''' <summary>
    ''' 以工厂模式使用的私有构造函数。
    ''' </summary>
    ''' <param name="xmlNode">一个包含 RSSFeed 内容的 XML 块。</param>
    Private Sub New(ByVal xmlNode As XmlNode)
        Me.m_channels = New List(Of RssChannel)

        ' 读取 <rss> 标记
        Dim rssNode As XmlNode = xmlNode.SelectSingleNode("rss")

        ' 为 <rss> 节点中的每个 <channel> 节点
        ' 添加一个频道。
        Dim channelNodes As XmlNodeList = rssNode.ChildNodes
        For Each channelNode As XmlNode In channelNodes
            Dim newChannel As New RssChannel(channelNode)
            Me.m_channels.Add(newChannel)
        Next channelNode

    End Sub

    '''<summary>
    '''从一个指向有效的 RSS 2.0 XML 文件的 URI 构造一个 RSSFeed 对象。
    '''</summary>
    '''<exception cref="System.Net.WebException">当无法在 Web 上找到该 URI 时引发。</exception>
    '''<param name="uri">从该处读取 RSS 源的 URI。</param>
    Public Shared Function FromUri(ByVal uri As String) As RssFeed
        Dim wClient As New WebClient()
        Dim textReader As StreamReader
        Dim reader As XmlTextReader
        Dim xmlDoc As New XmlDocument()

        Using rssStream As Stream = wClient.OpenRead(uri)
            textReader = New StreamReader(rssStream)
            reader = New XmlTextReader(textReader)
            xmlDoc.Load(reader)
        End Using

        Return New RssFeed(xmlDoc)
    End Function


    '''<summary>
    '''从一个 RSS 2.0 XML 文件的文本构造一个 RssFeed 对象。
    '''</summary>
    '''<param name="rssText">一个包含 RSS 源的 XML 数据的字符串。</param>
    Public Shared Function FromText(ByVal rssText As String) As RssFeed
        Dim xmlDoc As New XmlDocument()

        xmlDoc.LoadXml(rssText)

        Return New RssFeed(xmlDoc)
    End Function

End Class