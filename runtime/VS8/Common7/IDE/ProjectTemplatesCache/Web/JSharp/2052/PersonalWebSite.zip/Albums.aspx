<%@ Page Language="VJ#" MasterPageFile="~/Default.master" Title="此处是您的姓名 | 管理" %>

<asp:content id="Content1" contentplaceholderid="Main" runat="server">

	<div class="shim column"></div>

	<div class="page" id="admin-albums">

		<div id="sidebar">
			<h3>添加新相册</h3>
			<p>上载图片前，请先创建相册以组织您的图片。</p>
			<asp:FormView ID="FormView1" Runat="server"
				DataSourceID="ObjectDataSource1" DefaultMode="Insert"
				BorderWidth="0" CellPadding="0">
				<InsertItemTemplate>
					<asp:RequiredFieldValidator	ID="RequiredFieldValidator1" Runat="server" ErrorMessage="必须选择标题。" ControlToValidate="TextBox1" Display="Dynamic" Enabled="false" />
					<p>
						标题<br />
						<asp:TextBox ID="TextBox1" Runat="server" Width="200" Text='<%# Bind("Caption") %>' CssClass="textfield" />
						<asp:CheckBox ID="CheckBox2" Runat="server" checked='<%# Bind("IsPublic") %>' text="公开此相册" />
					</p>
					<p style="text-align:right;">
						<asp:ImageButton ID="ImageButton1" Runat="server" CommandName="Insert" skinid="add"/>
					</p>
				</InsertItemTemplate>
			</asp:FormView>
		</div>

		<div id="content">
			<h3>您的相册</h3>
			
			<p>当前，您的站点上有以下相册。单击“编辑”<b></b>可修改每个相册中的图片。单击“删除”<b></b>可永久移除相册和它的所有图片</p>
			
			<asp:gridview id="GridView1" runat="server"
				datasourceid="ObjectDataSource1" datakeynames="AlbumID" cellpadding="6"
				autogeneratecolumns="False" BorderStyle="None" BorderWidth="0px" width="420px" showheader="false">
				<EmptyDataTemplate>
				您当前没有相册。
				</EmptyDataTemplate>
				<EmptyDataRowStyle CssClass="emptydata"></EmptyDataRowStyle>
				<columns>
					<asp:TemplateField>
						<ItemStyle Width="116" />
						<ItemTemplate>
							<table border="0" cellpadding="0" cellspacing="0" class="photo-frame">
								<tr>
									<td class="topx--"></td>
									<td class="top-x-"></td>
									<td class="top--x"></td>
								</tr>
								<tr>
									<td class="midx--"></td>
									<td><a href='Photos.aspx?AlbumID=<%# Eval("AlbumID") %>'>
										<img src="../Handler.ashx?AlbumID=<%# Eval("AlbumID") %>&Size=S" class="photo_198" style="border:4px solid white" alt="示例照片，相册编号 <%# Eval("AlbumID") %>" /></a></td>
									<td class="mid--x"></td>
								</tr>
								<tr>
									<td class="botx--"></td>
									<td class="bot-x-"></td>
									<td class="bot--x"></td>
								</tr>
							</table>
						</ItemTemplate>
					</asp:TemplateField>
					<asp:TemplateField>
						<ItemStyle Width="280" />
						<ItemTemplate>
							<div style="padding:8px 0;">
								<b><%# get_Server().HtmlEncode(Eval("Caption").ToString()) %></b><br />
								<%# Eval("Count") %> 张照片<asp:Label ID="Label1" Runat="server" Text=" 公开" Visible='<%# Eval("IsPublic") %>'></asp:Label>
							</div>
							<div style="width:100%;text-align:right;">
								<asp:ImageButton ID="ImageButton2" Runat="server" CommandName="Edit" SkinID="rename" />
								<a href='<%# "Photos.aspx?AlbumID=" + Eval("AlbumID") %>'><asp:image ID="Image1" runat="Server"  skinid="edit" /></a>
								<asp:ImageButton ID="ImageButton3" Runat="server" CommandName="Delete" SkinID="delete" />
							</div>
						</ItemTemplate>
						<EditItemTemplate>
							<div style="padding:8px 0;">
								<asp:TextBox ID="TextBox2" Runat="server" Width="160" Text='<%# Bind("Caption") %>' CssClass="textfield" />
								<asp:CheckBox ID="CheckBox1" Runat="server" checked='<%# Bind("IsPublic") %>' text="公开" />
							</div>
							<div style="width:100%;text-align:right;">
								<asp:ImageButton ID="ImageButton4" Runat="server" CommandName="Update" SkinID="save" />
								<asp:ImageButton ID="ImageButton5" Runat="server" CommandName="Cancel" SkinID="cancel" />
							</div>
						</EditItemTemplate>
					</asp:TemplateField>
				</columns>
			</asp:gridview>

		</div>

	</div>
	
	<asp:ObjectDataSource ID="ObjectDataSource1" Runat="server" TypeName="PhotoManager" 
		SelectMethod="GetAlbums"
		InsertMethod="AddAlbum" 
		DeleteMethod="RemoveAlbum" 
		UpdateMethod="EditAlbum" >
	</asp:ObjectDataSource>

</asp:content>
