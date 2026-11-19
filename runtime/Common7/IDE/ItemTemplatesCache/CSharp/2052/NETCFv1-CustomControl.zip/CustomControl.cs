#region Using 指令

using System;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

#endregion

namespace $rootnamespace$
{
	/// <summary>
	/// $safeitemrootname$ 的摘要说明。
	/// </summary>
	public class $safeitemrootname$ : System.Windows.Forms.Control
	{
		/// <summary>
		/// 必需的设计器变量。
		/// </summary>
		private System.ComponentModel.IContainer components;


		public $safeitemrootname$()
		{
			// 此调用是 Windows.Forms 窗体设计器所必需的。
			InitializeComponent();

			// TODO: 在 InitComponent 调用之后添加任何初始化代码
		}

		/// <summary>
		/// 清理所有正在使用的资源。
		/// </summary>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region 组件设计器生成的代码
		/// <summary>
		/// 设计器支持所需的方法 - 不要
		/// 使用代码编辑器修改此方法的内容。
		/// </summary>
		private void InitializeComponent()
		{
			components = new System.ComponentModel.Container();
		}
		#endregion

		protected override void OnPaint(PaintEventArgs pe)
		{
			// TODO: 在此处添加自定义绘制代码

			// 调用基类 OnPaint
			base.OnPaint(pe);
		}
	}
}
