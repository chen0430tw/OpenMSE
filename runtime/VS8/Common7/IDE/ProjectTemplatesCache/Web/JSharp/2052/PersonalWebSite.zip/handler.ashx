<%@ WebHandler Language="VJ#" Class="Handler" %>

import System.*;
import System.IO.*;
import System.Web.*;

public class Handler implements IHttpHandler {

	/**@property*/
	public boolean get_IsReusable() { return true; }

	public void ProcessRequest(HttpContext context) {
		// 设置响应设置
		context.get_Response().set_ContentType("image/jpeg");
		context.get_Response().get_Cache().SetCacheability(HttpCacheability.Public);
		context.get_Response().set_BufferOutput(false);
		// 设置 Size 参数
		PhotoSize size = new PhotoSize(PhotoSize.Original);
		if (context.get_Request().get_QueryString().get_Item("Size") != null) {
			if (context.get_Request().get_QueryString().get_Item("Size").equals("S")) size.set_Size(PhotoSize.Small);
			if (context.get_Request().get_QueryString().get_Item("Size").equals("M")) size.set_Size(PhotoSize.Medium);
			if (context.get_Request().get_QueryString().get_Item("Size").equals("L")) size.set_Size(PhotoSize.Large);
		}
		// 设置 PhotoID 参数
		int id = -1;
		Stream stream = null;
		if (context.get_Request().get_QueryString().get_Item("PhotoID") != null
		&& context.get_Request().get_QueryString().get_Item("PhotoID") != "") {
			id = (int)Convert.ToInt32(context.get_Request().get_QueryString().get_Item("PhotoID"));
			stream = PhotoManager.GetPhoto(id, size);
		} else {
			id = (int)Convert.ToInt32(context.get_Request().get_QueryString().get_Item("AlbumID"));
			stream = PhotoManager.GetFirstPhoto(id, size);
		}
		// 从数据库获取照片，如果未返回照片，将获取默认的“placeholder”照片
		if (stream == null) stream = PhotoManager.GetPhoto(size);
		// 将图像流写入响应流中
		final int buffersize = 1024 * 16;
		ubyte[] buffer = new ubyte[buffersize];
		int count = stream.Read(buffer, 0, buffersize);
		while (count > 0) {
			context.get_Response().get_OutputStream().Write(buffer, 0, count);
			count = stream.Read(buffer, 0, buffersize);
		}
	}

}