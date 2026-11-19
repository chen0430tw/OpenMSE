<%@	Page Language="C#" MasterPageFile="~/Default.master" Title="此处是您的姓名 | 主页"
	CodeFile="Default.aspx.cs" Inherits="Default_aspx" %>

<asp:content id="Content1" contentplaceholderid="Main" runat="server">

	<div class="shim column"></div>
	
	<div class="page" id="home">
		<div id="sidebar">
			<asp:loginview id="LoginArea" runat="server">
				<AnonymousTemplate>
					<asp:login id="Login1" runat="server">
						<layouttemplate>
							<div class="login">
								<h4>登录站点</h4>
								<asp:label runat="server" id="UserNameLabel" CssClass="label" associatedcontrolid="UserName">用户名</asp:label>
								<asp:textbox runat="server"	id="UserName" cssclass="textbox" accesskey="u" />
								<asp:requiredfieldvalidator	runat="server" id="UserNameRequired" controltovalidate="UserName" validationgroup="Login1" errormessage="必须填写“用户名”。" tooltip="必须填写“用户名”。" >*</asp:requiredfieldvalidator>
								<asp:label runat="server" id="PasswordLabel" CssClass="label" associatedcontrolid="Password">密码</asp:label>
								<asp:textbox runat="server"	id="Password" textmode="Password" cssclass="textbox" accesskey="p" />
								<asp:requiredfieldvalidator	runat="server" id="PasswordRequired" controltovalidate="Password" validationgroup="Login1" tooltip="必须填写“密码”。" >*</asp:requiredfieldvalidator>
								<div><asp:checkbox runat="server" id="RememberMe" text="下次记住我"/></div>
								<asp:imagebutton runat="server"	id="LoginButton" CommandName="Login" AlternateText="登录" skinid="login" CssClass="button"/>
								或
								<a href="register.aspx"	class="button"><asp:image id="Image1" runat="server"  AlternateText="创建新帐户" skinid="create"/></a>
								<p><asp:literal	runat="server" id="FailureText"	enableviewstate="False"></asp:literal></p>
							</div>
						</layouttemplate>
					</asp:login>
				</anonymoustemplate>
				<LoggedInTemplate>
					<h4><asp:loginname id="LoginName1" runat="server" formatstring="欢迎	{0}!" /></h4>
				</LoggedInTemplate>
			</asp:loginview>
			<hr />
			<asp:formview id="FormView1" runat="server" datasourceid="ObjectDataSource1" ondatabound="Randomize" cellpadding="0" borderwidth="0" enableviewstate="false">
				<ItemTemplate>
					<h4>每日照片</h4>
					<table border="0" cellpadding="0" cellspacing="0" class="photo-frame">
						<tr>
							<td class="topx--"></td>
							<td class="top-x-"></td>
							<td class="top--x"></td>
						</tr>
						<tr>
							<td class="midx--"></td>
							<td><a href='Details.aspx?AlbumID=<%# Eval("AlbumID") %>&Page=<%# Container.DataItemIndex %>'>
								<img src="Handler.ashx?PhotoID=<%# Eval("PhotoID") %>&Size=M" class="photo_198" style="border:4px solid white" alt='照片编号 <%# Eval("PhotoID") %>' /></a></td>
							<td class="mid--x"></td>
						</tr>
						<tr>
							<td class="botx--"></td>
							<td class="bot-x-"></td>
							<td class="bot--x"></td>
						</tr>
					</table>
					<p>Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod </p>
					<p><a href='Download.aspx?AlbumID=<%# Eval("AlbumID") %>&Page=<%# Container.DataItemIndex %>'>
						<asp:image runat="Server" id="DownloadButton" AlternateText="下载照片" skinid="download"/></a></p>
					<p>请查看<a href="Albums.aspx">更多照片</a></p>
					<hr />
				</ItemTemplate>
			</asp:formview>
			<h4>我最近的作品</h4>
			<p>Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam nonummy nibh euismod tincidunt ut	laoreet	dolore magna aliquam erat volutpat.</p>
		</div>
		<div id="content">
			<h3>欢迎访问“我的站点”</h3>
			<p>这是我的个人站点。在这里您可以看到 lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam	nonummy	nibh euismod tincidunt ut laoreet dolore magna aliquam erat	volutpat. Ut wisi enim ad minim	veniam.	</p>
			<hr	/>
			<div id="whatsnew">
				<h4>新内容</h4>
				<p>Lorem <a href="#">ipsum</a> dolor sit amet, consectetuer	adipiscing elit, sed diam nonummy nibh euismod.</p>
				<p>Lorem <a href="#">ipsum</a> dolor sit amet, consectetuer	adipiscing elit, sed diam nonummy nibh euismod.</p>
			</div>
			<div id="coollinks">
				<h4>精彩链接</h4>
				<ul	class="link">
					<li><a href="#">Lorem ipsum dolositionr</a></li>
					<li><a href="#">Lorem ipsum dolositionr</a></li>
					<li><a href="#">Lorem ipsum dolositionr</a></li>
					<li><a href="#">Lorem ipsum dolositionr</a></li>
					<li><a href="#">Lorem ipsum dolositionr</a></li>
				</ul>
			</div>
			<hr	/>
			<h4>最新动态 </h4>
			<p>Lorem ipsum dolor sit amet, <a href="#">consectetuer</a>	adipiscing elit, sed diam nonummy nibh euismod tincidunt ut	laoreet	dolore magna aliquam erat volutpat.	Ut wisi	enim ad	minim veniam, quis nostrud exercitation	consequat. Duis	autem veleum iriure	dolor in hendrerit in vulputate	velit esse molestie	consequat, vel willum.</p>
			<p>Lorem ipsum dolor sit amet, consectetuer	adipiscing elit, sed diam nonummy nibh <a href="#">euismod tincidunt ut</a>	laoreet	dolore magna aliquam erat volutpat.	Ut wisi	enim ad	minim veniam, quis nostrud exercitation	consequat. Duis	autem veleum iriure	dolor in hendrerit in vulputate	velit esse molestie	consequat, vel willum.</p>
			<p>Lorem<a href="#"> ipsum dolor sit amet</a>, consectetuer	adipiscing elit, sed diam nonummy nibh euismod tincidunt ut	laoreet	dolore magna aliquam erat volutpat.	Ut wisi	enim ad	minim veniam, quis nostrud exercitation	consequat. Duis	autem veleum iriure	dolor in hendrerit in vulputate	velit esse molestie	consequat, vel willum.</p>
			<p>Lorem ipsum dolor sit amet, consectetuer	adipiscing elit, sed diam nonummy nibh euismod tincidunt ut	laoreet	dolore magna aliquam erat volutpat.	Ut wisi	enim ad	minim veniam, quis nostrud exercitation	consequat. <a href="#">Duis	autem veleum</a> iriure	dolor in hendrerit in vulputate	velit esse molestie	consequat, vel willum.</p>
			<p>Lorem ipsum dolor sit amet, consectetuer	adipiscing elit, sed diam nonummy nibh euismod tincidunt ut	laoreet	dolore magna aliquam erat volutpat.	Ut wisi	enim ad	minim veniam, quis nostrud exercitation	consequat. <a href="#">Duis	autem veleum</a> iriure	dolor in hendrerit in vulputate	velit esse molestie	consequat, vel willum.</p>
		</div>
	</div>

	<asp:ObjectDataSource ID="ObjectDataSource1" Runat="server" TypeName="PhotoManager" 
		SelectMethod="GetPhotos">
	</asp:ObjectDataSource>

</asp:content>
