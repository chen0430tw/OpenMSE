<%@	Page Language="VJ#"	MasterPageFile="~/Default.master" Title="此处是您的姓名	| 注册"	%>

<asp:Content ID="Content1" ContentPlaceHolderID="Main" Runat="server">

	<div class="shim column"></div>

	<div class="page" id="register">
		<div id="content">
			<h3>请求一个帐户</h3>
			<p>经管理员批准后帐户将被激活。</p>
			<asp:CreateUserWizard ID="CreateUserWizard1" Runat="server" 
				ContinueDestinationPageUrl="default.aspx"
				DisableCreatedUser="True"
				EmailRegularExpression="\S+@\S+\.\S+"
				EmailRegularExpressionErrorMessage="The email format is invalid.">
			</asp:CreateUserWizard>
		</div>
	</div>

</asp:Content>