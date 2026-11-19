#pragma once

using namespace System;
using namespace System::Collections;
using namespace System::ServiceProcess;
using namespace System::ComponentModel;


[!output SAFE_NAMESPACE_BEGIN]
	/// <summary>
	/// [!output SAFE_NAMESPACE_NAME]WinService 摘要
	/// </summary>
	///
	/// 警告: 如果更改此类的名称，则需要更改
	///          与此类所依赖的所有 .resx 文件关联的托管资源编译器工具的
	///          “资源文件名”属性。否则，
	///          设计器将不能与此窗体的关联
	///          本地化资源正确交互。
	public ref class [!output SAFE_NAMESPACE_NAME]WinService : public System::ServiceProcess::ServiceBase
	{
	public:
		[!output SAFE_NAMESPACE_NAME]WinService()
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
		~[!output SAFE_NAMESPACE_NAME]WinService()
		{
			if (components)
			{
				delete components;
			}
		}

		/// <summary>
		/// 设置具体的操作，以便服务可以执行它的工作。
		/// </summary>
		virtual void OnStart(array<String^>^ args) override
		{
			// TODO: 在此处添加代码以启动服务。
		}

		/// <summary>
		/// 停止此服务。
		/// </summary>
		virtual void OnStop() override
		{
			// TODO: 在此处添加代码以执行停止服务所需的关闭操作。
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
		void InitializeComponent(void)
		{
			this->components = gcnew System::ComponentModel::Container();
			this->CanStop = true;
			this->CanPauseAndContinue = true;
			this->AutoLog = true;
			this->ServiceName = L"[!output SAFE_NAMESPACE_NAME]WinService";
		}
#pragma endregion
	};
[!output SAFE_NAMESPACE_END]
