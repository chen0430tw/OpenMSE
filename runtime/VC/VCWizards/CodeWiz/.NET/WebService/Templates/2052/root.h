// [!output FILE_NAME].h
[!output COMMENTS_MANAGED_SWITCHES]
#pragma once

using namespace System;
using namespace System::Web;
using namespace System::Web::Services;

[!output SAFE_NAMESPACE_BEGIN]
    public ref class C[!output SAFE_ITEM_NAME] : public System::Web::Services::WebService
    {
    public:
		C[!output SAFE_ITEM_NAME]()
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
		~C[!output SAFE_ITEM_NAME]()
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
		// 为此类添加您的方法及方法的实现。
		// 若要测试此 Web 服务，请确保部署路径中的 .asmx 文件
		// 在项目属性中设置为“调试 HTTP URL”，然后按 F5。
		// 下面给出了 HelloWorld 示例模板。

    public:
        [System::Web::Services::WebMethod]
        String ^HelloWorld();

        // TODO: 在此添加 Web 服务的方法

    };
[!output SAFE_NAMESPACE_END]
