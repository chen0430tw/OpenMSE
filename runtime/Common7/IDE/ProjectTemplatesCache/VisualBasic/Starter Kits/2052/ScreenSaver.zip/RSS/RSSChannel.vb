Imports System.Collections.Generic
Imports System.Xml

'''<summary>
'''表示 RSS 2.0 XML 文档中的 Channel 元素。
''' </summary>
''' <remarks>
'''一个 RssFeed 中包含零个或更多个 RssChannel。使用 MainChannel 属性访问
'''主频道。
''' </remarks>
Public Class RssChannel
    Private m_title As String
    Private m_link As String
    Private m_items As List(Of RssItem)

    Public ReadOnly Property Title() As String 
        Get
            Return Me.m_title
        End Get
    End Property
    
    Public ReadOnly Property Link() As String 
        Get
            Return Me.m_link
        End Get
    End Property

    Public ReadOnly Property Items() As IList(Of RssItem)
        Get
            Return Me.m_items.AsReadOnly()
        End Get
    End Property

    '''<summary>
    '''从一个表示 RSS 2.0 XML 文档内 Channel 元素的 XmlNode 生成一个 RSSChannel。
    '''</summary>
    '''<param name="channelNode"></param>
    Friend Sub New(ByVal channelNode As XmlNode) 
        m_items = New List(Of RssItem)
        m_title = channelNode.SelectSingleNode("title").InnerText
        m_link = channelNode.SelectSingleNode("link").InnerText

        Dim itemNodes As XmlNodeList = channelNode.SelectNodes("item")
        For Each itemNode As XmlNode In itemNodes
            m_items.Add(New RssItem(itemNode))
        Next
    
    End Sub

End Class