// [!output SAFE_NAMESPACE_NAME]Class.h

#pragma once

using namespace System;
using namespace System::Web;
using namespace System::Web::Services;

[!output SAFE_NAMESPACE_BEGIN]
	[WebServiceBinding(ConformsTo=WsiProfiles::BasicProfile1_1,EmitConformanceClaims = true)]
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
    public ref class [! output SAFE_NAMESPACE_NAME]Class : public System::Web::Services::WebService
    {

    public:
		[! output SAFE_NAMESPACE_NAME]Class()
		{
			InitializeComponent();
			//
			//TODO: 在此处添加构造函数代码
			//
		}
	protected:
		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		~[! output SAFE_NAMESPACE_NAME]Class()
		{
			if (components)
			{
				delete components;
			}
		}

	private:
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		void InitializeComponent()
		{
		}
#pragma endregion

		// WEB 服务示例
		// HelloWorld() 示例服务返回字符串“Hello, World!”。
		// 若要测试此 Web 服务，请确保部署路径中的 .asmx 文件
		// 在项目属性中设置为“调试 HTTP URL”。
		// 然后按 F5。

	public:
        [System::Web::Services::WebMethod]
        String ^HelloWorld();

        // TODO: 在此添加 Web 服务的方法
	};
[!output SAFE_NAMESPACE_END]
