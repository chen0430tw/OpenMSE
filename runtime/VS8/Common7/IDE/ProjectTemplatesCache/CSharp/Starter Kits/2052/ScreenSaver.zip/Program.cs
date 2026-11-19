using System;
using System.Windows.Forms;
using System.Globalization;

namespace $safeprojectname$
{
	static class Program
	{
		/// <summary>
		/// 应用程序的主入口点。
		/// </summary>
		[STAThread]
		static void Main(string[] args)
		{
			if (args.Length > 0)
			{
				// 获取双字符命令行参数
				string arg = args[0].ToLower(CultureInfo.InvariantCulture).Trim().Substring(0, 2);
				switch (arg)
				{
					case "/c":
						// 显示选项对话框
						ShowOptions();
						break;
					case "/p":
						// 不要进行任何预览操作
						break;
					case "/s":
						// 显示屏幕保护程序窗体
						ShowScreenSaver();
						break;
					default:
						MessageBox.Show("无效的命令行参数 :" + arg,"无效的命令行参数",MessageBoxButtons.OK,MessageBoxIcon.Error);
						break;
				}
			}
			else
			{
				// 如果未传入参数，则显示屏幕保护程序
				ShowScreenSaver();
			}
		}


		static void ShowOptions()
		{
			OptionsForm optionsForm = new OptionsForm();
			Application.Run(optionsForm);
		}

		static void ShowScreenSaver()
		{
			ScreenSaverForm screenSaver = new ScreenSaverForm();
			Application.Run(screenSaver);
		}
	}
}