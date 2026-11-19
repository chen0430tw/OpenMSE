#region Using 指令

using System;
using System.Drawing;
using System.Collections;
using System.Windows.Forms;
using System.Data;

#endregion

namespace $rootnamespace$
{
	/// <summary>
	/// $safeitemrootname$ 的摘要说明。
	/// </summary>
	public class $safeitemrootname$ : System.Windows.Forms.Form
	{
		/// <summary>
		/// 窗体的主菜单。
		/// </summary>
		private System.Windows.Forms.MainMenu mainMenu1;

		public $safeitemrootname$()
		{
			InitializeComponent();
		}

		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		protected override void Dispose( bool disposing )
		{
			base.Dispose( disposing );
		}

		#region Windows 窗体设计器生成的代码

		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			this.mainMenu1 = new System.Windows.Forms.MainMenu();
			this.Menu = this.mainMenu1;

			this.Text = "$safeitemrootname$";
		}

		#endregion
	}
}
