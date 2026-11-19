using System;
using System.Collections;
using System.ComponentModel;
using System.Drawing;
using System.Windows.Forms;

namespace [!output NAMESPACE]
{
    [!output CLASS_MODIFIER] class [!output SAFE_ITEM_NAME] : [!output BASE_FULL_NAME]
    {
        private System.ComponentModel.IContainer components = null;

        public [!output SAFE_ITEM_NAME]()
        {
            // 此调用是 Windows 窗体设计器所必需的。
            InitializeComponent();

            // TODO: 在 InitializeComponent 调用之后添加任何初始化代码
        }

        /// <summary>
        /// 清理所有正在使用的资源。
        /// </summary>
        protected override void Dispose( bool disposing )
        {
            if( disposing )
            {
                if (components != null) 
                {
                    components.Dispose();
                }
            }
            base.Dispose( disposing );
        }

        #region 设计器生成的代码
        /// <summary>
        /// 设计器支持所需的方法 - 不要
        /// 使用代码编辑器修改此方法的内容。
        /// </summary>
        private void InitializeComponent()
        {
            components = new System.ComponentModel.Container();
        }
        #endregion
    }
}
