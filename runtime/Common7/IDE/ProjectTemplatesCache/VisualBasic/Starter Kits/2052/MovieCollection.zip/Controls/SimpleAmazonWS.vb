Imports System.ComponentModel

''' <summary>
''' 一个被简化的类，负责向 Amazon.com Web 服务提出请求并返回结果。
''' </summary>
''' <remarks>
''' ***本版本的初学者工具包中未实现联机 Web 服务功能。请查看“开始”文档，以了解如何下载更新版本。  
''' </remarks>
Public Class SimpleAmazonWS

    ''' <summary>
    ''' 为搜索请求配置默认值
    ''' </summary>
    ''' <remarks>可以在不更改代码的情况下，在“设置”设计器中更改某些默认值。</remarks>
    Public Sub New()

    End Sub

    ''' <summary>
    ''' 这是一个简单的搜索方法，它根据搜索字符串参数返回一个 DVD 对象的列表。 
    ''' </summary>
    ''' <param name="searchString">要搜索的 DVD 关键字，如: 姓名、演员或 UPC</param>
    ''' <returns>DVD 搜索结果的 BindingList</returns>
    ''' <remarks></remarks>
    Public Function SearchDVDs(ByVal searchString As String) As BindingList(Of DVD)
        Throw New NotImplementedException("此版本的初学者工具包未实现联机 Web 服务功能。请参见<入门>文档，以了解如何下载初学者工具包的更新版本。 ")
    End Function


    ''' <summary>
    ''' 根据 UPC 字符串参数查找有关特定 DVD 的刷新数据。  
    ''' </summary>
    ''' <param name="upc">要查找的 DVD 的 UPC 代码</param>
    ''' <returns>用此匹配 UPC 找到的第一个 DVD 对象，如果一个也未找到则返回空</returns>
    ''' <remarks></remarks>
    Public Function RefreshDVDByUPC(ByVal upc As String) As DVD
        Throw New NotImplementedException("此版本的初学者工具包未实现联机 Web 服务功能。请参见<入门>文档，以了解如何下载初学者工具包的更新版本。 ")
    End Function

    ''' <summary>
    ''' 这是一个实用工具函数，将 Amazon 自定义的日期格式返回为 .NET 日期类型
    ''' </summary>
    ''' <param name="amazonDate">由 Amazon.com 的 ProductInfo 类型返回的日期字符串</param>
    ''' <returns>由自定义字符串转换的日期类型，如果参数为空则为 Date.MinValue</returns>
    ''' <remarks></remarks>
    Private Function AmazonDateFormatToDateString(ByVal amazonDate As String) As String
        ' 用于处理 Amazon 日期字符串约定的格式；0 表示未设置
        Dim expectedFormats As String() = {"d", "yyyymmdd", "yyyy0000", "yyyymm00", "yyyy00dd"}
        Dim myDate As Date
        If amazonDate IsNot Nothing Then
            myDate = DateTime.ParseExact(amazonDate, _
                                expectedFormats, _
                                My.Application.Culture, Globalization.DateTimeStyles.AllowWhiteSpaces)
        Else
            Return Nothing
        End If

        Return myDate.ToString("yyyy")
    End Function

End Class
