<%@	Page Language="C#" MasterPageFile="~/Default.master" Title="此处是您的姓名 | 管理"
	CodeFile="Photos.aspx.cs" Inherits="Admin_Photos_aspx" %>

<asp:content id="Content1" contentplaceholderid="Main" runat="server">

	<div class="shim column"></div>
	
	<div class="page" id="admin-photos">
		<div id="sidebar">
			<h4>批量上载照片</h4>
			<p>在“上载”<b></b>文件夹中找到下列文件。单击“导入”<b></b>可将这些图片导入到相册中。此操作可能需要一段时间。</p>
			<asp:ImageButton ID="ImageButton1" Runat="server" onclick="Button1_Click" SkinID="import" />
			<br />
			<br />
			<asp:datalist runat="server" id="UploadList" repeatcolumns="1" repeatlayout="table" repeatdirection="horizontal" DataSourceID="ObjectDataSource2">
				<itemtemplate>
					<%#	Container.DataItem %>
				</itemtemplate>
			</asp:datalist>
		</div>

		<div id="content">
			<h4>添加照片</h4>
			<p>若要通过 HTTP 添加单张照片，请选择文件和标题，然后单击“添加”<b></b>。</p>
			<asp:FormView ID="FormView1" Runat="server" 
				DataSourceID="ObjectDataSource1" DefaultMode="insert"
				BorderWidth="0px" CellPadding="0" OnItemInserting="FormView1_ItemInserting">
				<InsertItemTemplate>
					<asp:RequiredFieldValidator	ID="RequiredFieldValidator1" Runat="server" ErrorMessage="必须选择标题。" ControlToValidate="PhotoFile" Display="Dynamic" Enabled="false" />
					<p>
						照片<br />
						<asp:FileUpload ID="PhotoFile" Runat="server" Width="416" FileBytes='<%# Bind("BytesOriginal") %>' CssClass="textfield" /><br />
						标题<br />
						<asp:TextBox ID="PhotoCaption" Runat="server" Width="326" Text='<%# Bind("Caption") %>' CssClass="textfield" />
					</p>
					<p style="text-align:right;">
						<asp:ImageButton ID="AddNewPhotoButton" Runat="server" CommandName="Insert" skinid="add"/>
					</p>
				</InsertItemTemplate>
			</asp:FormView>
			<hr />
			<h4>此相册中的照片</h4>
			<p>此相册中当前有以下照片。</p>
			<asp:gridview id="GridView1" runat="server" datasourceid="ObjectDataSource1" 
				datakeynames="PhotoID" cellpadding="6" EnableViewState="false"
				autogeneratecolumns="False" BorderStyle="None" BorderWidth="0px" width="420px" showheader="false" >
				<EmptyDataRowStyle CssClass="emptydata"></EmptyDataRowStyle>
				<EmptyDataTemplate>
					当前没有照片。
				</EmptyDataTemplate>
				<columns>
					<asp:TemplateField>
						<ItemStyle Width="50" />
						<ItemTemplate>
							<table border="0" cellpadding="0" cellspacing="0" class="photo-frame">
								<tr>
									<td class="topx--"></td>
									<td class="top-x-"></td>
									<td class="top--x"></td>
								</tr>
								<tr>
									<td class="midx--"></td>
									<td><a href='Details.aspx?AlbumID=<%# Eval("AlbumID") %>&Page=<%# ((GridViewRow)Container).RowIndex %>'>
										<img src='../Handler.ashx?Size=S&PhotoID=<%# Eval("PhotoID") %>' class="photo_198" style="border:2px solid white;width:50px;" alt='缩略图，照片编号 <%# Eval("PhotoID") %>' /></a></td>
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
					<asp:boundfield headertext="Caption" datafield="Caption" />
					<asp:TemplateField>
						<ItemStyle Width="150" />
						<ItemTemplate>
							<div style="width:100%;text-align:right;">
								<asp:ImageButton ID="ImageButton2" Runat="server" CommandName="Edit" SkinID="rename" />
								<asp:ImageButton ID="ImageButton3" Runat="server" CommandName="Delete"  SkinID="delete" />
							</div>
						</ItemTemplate>
						<EditItemTemplate>
							<div style="width:100%;text-align:right;">
								<asp:ImageButton ID="ImageButton4" Runat="server" CommandName="Update" SkinID="save" />
								<asp:ImageButton ID="ImageButton5" Runat="server" CommandName="Cancel"  SkinID="cancel" />
							</div>
						</EditItemTemplate>
					</asp:TemplateField>
				</columns>
			</asp:gridview>
		</div>

	 </div>
	
	<asp:ObjectDataSource ID="ObjectDataSource1" Runat="server" TypeName="PhotoManager" 
		SelectMethod="GetPhotos"
		InsertMethod="AddPhoto" 
		DeleteMethod="RemovePhoto" 
		UpdateMethod="EditPhoto" >
		<SelectParameters>
			<asp:QueryStringParameter Name="AlbumID" Type="Int32" QueryStringField="AlbumID" />
		</SelectParameters>
		<InsertParameters>
			<asp:QueryStringParameter Name="AlbumID" Type="Int32" QueryStringField="AlbumID" />
		</InsertParameters>
	</asp:ObjectDataSource>
	
	<asp:ObjectDataSource ID="ObjectDataSource2" Runat="server" TypeName="PhotoManager" 
		SelectMethod="ListUploadDirectory" >
	</asp:ObjectDataSource>
	 
</asp:content>
