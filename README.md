# OpenMSE

OpenMSE 提供统一图形工作区及 MSE / Visual Studio 2005 的本机兼容运行环境。双击根目录 `OpenMSE.exe` 或 `OpenMSE.lnk` 进入统一主界面，不创建命令行窗口。参见[统一 GUI 使用说明](docs/gui.md)。

```powershell
D:\OpenMSE\OpenMSE.exe
D:\OpenMSE\OpenMSE.exe D:\OpenMSE\samples\hello.htm
```

默认入口打开 `bin/OpenMSE.Shell.exe`。使用 `--mse` / `--vs8` 时，启动器先导入项目配置，再运行对应旧引擎并启用路径重定向。

## 工具箱与设计视图

已加入左侧统一工具箱、中央设计/源码切换、右侧项目树和属性面板。包含 17 个原 MSE HTML 条目及 22 个 Windows 窗体标准控件，支持插入、移动、调整尺寸、属性修改和保存。参见[工具箱操作与范围](docs/designer.md)。

## VS2005 / VS8 集成

现在同一启动器提供两个独立模式；VS7 与 VS8 核心保持在不同目录、不同进程和不同用户配置中。

- 双击 `OpenMSE.lnk` 或直接运行 `OpenMSE.exe`：统一图形工作区。
- 双击 `MSE.lnk` 或运行 `OpenMSE.exe --mse`：原版 MSE 兼容窗口。
- 双击 `Visual Studio 2005.lnk`：Visual Studio 2005。
- 命令行：`OpenMSE.exe --vs8`，也可以在后面传入解决方案或文档路径。

VS8 的私有文件在 `runtime/VS8`。已经验证 VS8 起始页、C# 项目加载和 Debug 构建；`samples/VS8Smoke` 的构建结果为 0 错误、0 警告，输出程序执行成功。VS8 的 `MSENV.DLL` 为 8.0.50727.867，MSE 的 VS7 核心保持 7.00.9064.9606。

VS8 配置使用同一项目实例键下的 `MachineVS8` / `UserVS8`，与 MSE 的配置分开。原安装的授权信息仍由本机提供，不导出授权键；最近项目列表不复制。用户项目默认目录在 `workspace/VS8`。

VS8 使用系统 .NET 2.0/3.5、GAC 和 COM 环境，尚未验证干净机器。复制范围已补齐为整个原 VS8 安装目录，包括 Crystal Reports、ReportViewer、SmartDevices 等注册扩展；已审计 585 个已注册组件路径，无漏搬路径。Crystal Reports 报表设计器已验证打开空白 .rpt 文件，相关 Business Objects 共享运行库仍由本机原安装提供。C++ 构建、设计器和断点调试尚未逐项验证。

重新生成入口：`powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/create-shortcuts.ps1`。

VS8 核验：`powershell.exe -NoProfile -ExecutionPolicy Bypass -File scripts/verify.ps1 -Engine vs8`；结果在 `logs/validation-vs8.json`。`config/vs8-files.json` 保存复制文件的来源与 SHA-256。共用兼容层位于项目根目录的 `OpenMSEHook.dll`。

## 已验证

- 空白设计界面正常启动。
- 打开 `samples\hello.htm` 后，HTML 编辑器及设计模块正常加载，没有启动错误对话框。
- MSE、MSO、MSENV、MSOMSE、HTMED、TRIDSN 和共享调试 DLL 均从本项目目录加载。
- 原始安装目录中的二进制文件未移动、删除或修改。

这是在当前机器上验证的二进制移植环境。MSE 本体仍是复制的微软二进制；项目源码实现启动器及兼容层，并非 MSE 源码重写。尚未验证未安装 Office 的干净机器，也尚未验证脚本断点调试和 Office 文档集成。Windows COM 注册、Windows Installer、VC80 并行运行库和系统 MSHTML 等仍可能依赖本机环境。

## 目录

| 路径 | 用途 |
|---|---|
| `OpenMSE.exe` | x86 启动器 |
| `src/` | 启动器和重定向 DLL 源码 |
| `runtime/OFFICE12/` | 私有 MSE、Office 组件、VS7 核心及语言资源 |
| `runtime/VS7Debug/` | 私有共享调试组件 |
| `config/registry-template.json` | 从本机 MSE 配置导出的模板 |
| `config/files.json` | 复制文件来源、大小和 SHA-256 |
| `scripts/setup.ps1` | 导入当前项目路径对应的用户配置 |
| `scripts/verify.ps1` | 核验运行中的模块来源与窗口状态 |
| `logs/` | 启动日志、重定向日志与运行时证据 |
| `samples/hello.htm` | 不含脚本的编辑器测试文档 |

## 工作方式

使用 [Microsoft Detours 4.0.1](https://github.com/microsoft/Detours/tree/v4.0.1) 在新建的 MSE 进程内重定向 DLL 加载路径；不修改磁盘上的 MSE EXE/DLL。

只有 `Software\Microsoft\Office\12.0\MSE` 这一配置树的打开/创建操作会转向项目专用用户键：

```text
HKCU\Software\OpenMSE\Instances\<项目路径哈希>\MachineMSE
HKCU\Software\OpenMSE\Instances\<项目路径哈希>\UserMSE
```

模板中的组件路径按当前项目目录重定位。不改写全局注册表查询返回值，也不改写全局文件访问；原安装配置的键值不会被启动器重写。

诊断参数：`--hidden` 隐藏设计主窗口，`--wait` 等待 MSE 退出并将退出码写入 `logs/launcher.log`。`--loads-only` 仅用于对比测试，跳过配置隔离，不作为正常启动方式。

## 构建与核验

运行 `build.cmd`。需要 MSVC x86 工具链与 Windows SDK。构建脚本通过 `vswhere` 查找工具链，将启动器和兼容层编译为 x86，并静态链接其 C/C++ 运行库。

Detours 源码位于 `vendor/Detours`，固定到 v4.0.1 / `e4bfd6b03e50de46b47abfbd1e46b384f0c5f833`，保留上游许可文件。

在 MSE 初始化完成后核验：

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\scripts\verify.ps1
```

核验脚本只读取该项目启动的进程，不关闭程序。它会检查必要模块、来自原安装目录的私有模块、设计窗口和模态窗口，并将结果写入 `logs/validation.json`。

`scripts/prepare.py` 是首次复制时的来源记录脚本，要求目标目录不存在，不用于覆盖现有项目。复制的微软运行组件不适用 Detours 的开源许可；不要将它们误认为本项目自行实现的模块。

## 新内核重写：第一阶段

已加入进程隔离架构、C++17 服务接口草案与可运行的 x86 EnvDTE 桥原型。原有启动器继续用于运行旧引擎；C++ 新内核尚未实现。后续已加入[统一 GUI 第一版](docs/gui.md)，提供共用编辑、模板与构建流程。

- [架构与里程碑](docs/kernel/architecture.md)
- [本机实测能力矩阵](docs/kernel/capabilities.md)
- [桥接协议及验证命令](docs/kernel/protocol.md)
- 契约：`include/openmse/kernel/services.hpp`；x64 编译检查：`build-contracts.cmd`
- 桥接源码：`src/bridge/HostBridge.cs`；构建：`scripts/build-bridge.ps1`
- 实测入口：`scripts/audit-kernel.py`；机器可读结论：`config/kernel-capabilities.json`

VS2005 的中文文档读写、项目打开与构建通过。MSE 自动化接入尚未成功；VS2005 调试启动返回 `0x80004005`，断点命中、单步和变量读取尚未验证。这些限制需要在后续实现前解决。

## 从源码仓库构建

仓库包含源码、示例、配置模板及文件来源清单，不包含微软运行组件、编译结果或本机测试数据。旧引擎兼容模式需要本机已有的对应安装与运行依赖，详见上述说明。

只编译统一 GUI：运行 `scripts/build-shell.ps1`。编译启动器和兼容层前，需准备 MSVC x86 工具链、Windows SDK 和固定版本的 Detours：

```powershell
git clone --branch v4.0.1 https://github.com/microsoft/Detours.git vendor/Detours
git -C vendor/Detours checkout e4bfd6b03e50de46b47abfbd1e46b384f0c5f833
cmd.exe /c build.cmd
```

`scripts/prepare.py` 是最初本机迁移时使用的记录脚本，依赖原始暂存目录，不是干净机器的一键安装器。配置模板已排除帮助组件的本机注册姓名和产品标识，重新导出也会过滤这些值。
