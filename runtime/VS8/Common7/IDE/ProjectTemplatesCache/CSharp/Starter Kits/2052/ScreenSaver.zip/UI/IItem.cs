using System;
using System.Collections.Generic;
using System.Text;

namespace $safeprojectname$.UI
{
    /// <summary>
    /// 带有 <c>说明</c> 和 <c>标题</c>的项的泛化。
    /// IItem 的任何实现都可以使用 ItemListView 和 ItemDescriptionView 类型来呈现。
    /// </summary>
    public interface IItem
    {
        string Description { get; }
        string Title { get; }
    }
}
