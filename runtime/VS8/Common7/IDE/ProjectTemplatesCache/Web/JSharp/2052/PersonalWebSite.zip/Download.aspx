<%@	Page Language="VJ#"	MasterPageFile="" Title="此处是您的姓名	| 下载"	 %>

<script	runat="server">

	void Page_Load(Object sender, EventArgs e) {
		if (!get_IsPostBack()) {
			int i = Convert.ToInt32(get_Request().get_QueryString().get_Item("Page"));
			if (i >= 0) FormView1.set_PageIndex(i);
		}
	}

</script>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml" >
<head runat="server">
	<title>无标题页</title>
	<style type="text/css">
	body {
		margin:0;
		padding:0;
		}
	p {
		font:13px 宋体, verdana;
		padding:15px 0 0 6px;
		}
	</style>
</head>
<body>
	<form id="form1" runat="server">
	<div>

		<p>右击然后选择“图片另存为..”可以下载图片。</p>
		<asp:formview id="FormView1" runat="server" datasourceid="ObjectDataSource1" borderstyle="none" borderwidth="0" CellPadding="0" cellspacing="0">
			<itemtemplate>
				<img src="Handler.ashx?PhotoID=<%# Eval("PhotoID") %>" alt='照片编号 <%# Eval("PhotoID") %>' /></itemtemplate>
		</asp:formview>

		<asp:ObjectDataSource ID="ObjectDataSource1" Runat="server" TypeName="PhotoManager" SelectMethod="GetPhotos">
			<SelectParameters>
				<asp:QueryStringParameter Name="AlbumID" Type="Int32" QueryStringField="AlbumID" DefaultValue="0"/>
			</SelectParameters>
		</asp:ObjectDataSource>

	</div>
	</form>
</body>
</html>
