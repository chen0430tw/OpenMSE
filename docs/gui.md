# 统一图形界面

直接运行根目录 `OpenMSE.exe` 或 `OpenMSE.lnk`，进入新的统一工作区。主窗口使用 x64 .NET Framework 4 Windows Forms，与独立的 x86 旧引擎分离，不创建控制台窗口。

## 已实现

- 共用的文件、编辑、视图、项目、生成、工具菜单与工具栏。
- 一个新建对话框提供文本、HTML、JavaScript、VBScript 文件，以及 C# 和 Visual Basic 的 .NET 2.0 控制台项目。
- 多标签文本编辑、撤销/重做、剪贴板操作、保存/另存为/全部保存、未保存文档关闭确认。
- 打开 `.sln`、`.csproj`、`.vbproj`，显示所在目录的项目文件树；双击源文件打开编辑。
- 项目树来自内核解析的项目图，显示项目、文件和引用。工具栏可选择解决方案配置。
- 构建、运行都是内核任务：输出实时显示；可以取消；结束后显示退出码、错误数、警告数和产物；错误列表双击可跳到源码位置。
- 调试（F5/F9/F10/F11/Shift+F11/Shift+F5）由 OpenMSE.DebugHost 提供，有调用堆栈和局部变量页；“工具”菜单可启动、同步和关闭 VS2005 后端。
- 文档由 x64 [OpenMSE.Core](kernel/core.md) 持有：编辑以区间修改提交，撤销/重做、脏状态、版本号、编码和换行由内核管理。保存前检查外部变更，写同目录临时文件后替换目标。文本编辑限制 8 MB。

`Ctrl+N` 新建，`Ctrl+O` 打开文件，`Ctrl+Shift+O` 打开项目，`Ctrl+S` 保存，`Ctrl+W` 关闭当前文档，`Ctrl+Shift+B` 生成。

## 兼容入口

```powershell
D:\OpenMSE\OpenMSE.exe                           # 统一工作区
D:\OpenMSE\OpenMSE.exe D:\OpenMSE\samples\hello.htm
D:\OpenMSE\OpenMSE.exe --mse                     # 原 MSE 窗口
D:\OpenMSE\OpenMSE.exe --vs8                     # 原 VS2005 窗口
```

统一窗口的“工具”菜单可以把保存后的当前文档或项目交给对应旧引擎。`MSE.lnk` 和 `Visual Studio 2005.lnk` 仍是明确的兼容入口；`OpenMSE.lnk` 是统一入口。现有 `--hidden`、`--wait` 和审计脚本继续使用旧引擎启动路径。

这是共用编辑、模板与构建流程的 GUI 第一版。已加入[HTML 与 Windows 窗体布局工具箱](designer.md)，中央提供设计/源码切换，右侧提供属性面板。没有移植旧引擎的语法服务或窗口停靠插件；调试器是新的 CLR 2.0 适配器，不是 VS2005 的调试器。旧窗口未被跨进程嵌入。文档、设计、项目图、构建/运行任务、调试会话和后端宿主都由 C++ 内核管理，见[统一内核](kernel/core.md)。项目树显示 .sln/.csproj/.vbproj 的项目、文件和引用，不处理解决方案文件夹和通配符项。只提供已验证的 C#/VB 项目模板，不声称支持完整 VS2005 项目系统。

## 构建与验证

`build.cmd --launcher-only` 同时编译 GUI 和原生入口。单独编译 GUI：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\scripts\build-shell.ps1
```

`tests/gui/GuiSmoke.cs` 通过真实控件创建文档、读写中文、验证外部修改保护，并从新模板经主窗口构建 C# / VB 项目，再运行生成程序核对输出。运行 `bin\OpenMSE.Shell.exe --smoke-test <独立测试目录>`，结果写入测试目录的 `results.txt` / `failure.txt`。不要对用户工作目录运行该内部测试。最近一次测试目录记录在 `logs/gui-smoke-path.txt`。
