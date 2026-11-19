Imports System
Imports System.Data
Imports System.Data.SqlClient
Imports System.Data.SqlTypes
Imports Microsoft.SqlServer.Server

Partial Public Class UserDefinedFunctions
    <Microsoft.SqlServer.Server.SqlFunction()> _
    Public Shared Function $safeitemname$() As SqlString
        ' 在此处添加您的代码
        Return New SqlString("Hello")
    End Function
End Class
