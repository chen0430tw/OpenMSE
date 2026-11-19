<%@ WebHandler Language="VB" Class="Handler" %>

Public Class Handler
	Implements IHttpHandler

	ReadOnly Property IsReusable() As Boolean Implements IHttpHandler.IsReusable
		Get
			Return True
		End Get
	End Property
    
	Sub ProcessRequest(ByVal context As HttpContext) Implements IHttpHandler.ProcessRequest
		' 设置响应设置
		context.Response.ContentType = "image/jpeg"
		context.Response.Cache.SetCacheability(HttpCacheability.Public)
		context.Response.BufferOutput = False
		' 设置 Size 参数
		Dim size As PhotoSize = PhotoSize.Original
		Select Case context.Request.QueryString("Size")
			Case "S"
				size = PhotoSize.Small
			Case "M"
				size = PhotoSize.Medium
			Case "L"
				size = PhotoSize.Large
			Case Else
				size = PhotoSize.Original
		End Select
		' 设置 PhotoID 参数
		Dim id As Int32 = 1
		Dim stream As IO.Stream = Nothing
		If ((Not (context.Request.QueryString("PhotoID")) Is Nothing) _
		   AndAlso (context.Request.QueryString("PhotoID") <> "")) Then
			id = [Convert].ToInt32(context.Request.QueryString("PhotoID"))
			stream = PhotoManager.GetPhoto(id, size)
		Else
			id = [Convert].ToInt32(context.Request.QueryString("AlbumID"))
			stream = PhotoManager.GetFirstPhoto(id, size)
		End If
		' 从数据库获取照片，如果未返回照片，将获取默认的“placeholder”照片
		If (stream Is Nothing) Then
			stream = PhotoManager.GetPhoto(size)
		End If
		' 将图像流写入响应流中
		Dim buffersize As Integer = (1024 * 16)
		Dim buffer() As Byte = New Byte((buffersize) - 1) {}
		Dim count As Integer = stream.Read(buffer, 0, buffersize)
        
		Do While (count > 0)
			context.Response.OutputStream.Write(buffer, 0, count)
			count = stream.Read(buffer, 0, buffersize)
            
		Loop
	End Sub
	
End Class