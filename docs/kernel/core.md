# OpenMSE.Core：统一内核

状态：整合计划阶段 1–6 已实现（2026-11-20）。阶段 6 的打包与安装检查见 [docs/packaging.md](../packaging.md)。

| 阶段 | 内容 | 本文章节 |
|---|---|---|
| 1 | 文档服务、最小命令路由、事件 | 文档 |
| 2 | ToolboxService、DesignService、设计事务 | 工具箱与设计 |
| 3 | 项目图、工具发现、异步构建/运行任务 | 工作区与任务 |
| 4 | 宿主清单、旧宿主生命周期、桥协议 v2 | 旧宿主 |
| 5 | CLR 2.0 调试适配器、能力注册表、MSE 独立验收 | 调试与能力 |

## 组成

| 部分 | 位置 | 说明 |
|---|---|---|
| 稳定 C ABI | `include/openmse/kernel/abi.h` | 不透明句柄；参数为带长度 UTF-8；结果为内核分配的 JSON 缓冲区；ABI 版本握手（当前为 5） |
| 内核 | `src/core/` | `text` 编码；`documents` 文档；`json`/`xml` 读取器；`design` 设计与工具箱；`workspace` 项目图与工具发现；`process` 子进程与作业对象；`tasks` 构建/运行任务；`hosts` 旧宿主；`debug` 调试服务；`abi.cpp` 边界、事件、命令、能力 |
| 调试适配器 | `src/debug/DebugHost.cpp` | `OpenMSE.DebugHost.x86.exe` / `.x64.exe`，基于 ICorDebug v2 和 PDB 符号读取 |
| 桥接 | `src/bridge/HostBridge.cs` | x86，EnvDTE 8.0，协议 v2（见 [protocol.md](protocol.md)） |
| 宿主清单 | `config/hosts.json` | VS2005 / MSE 的入口、版本、SHA-256、启动器、桥接与能力 |
| 托管客户端 | `src/shell/KernelClient.cs`、`KernelWorkspace.cs` | P/Invoke 封装与结果模型；事件在 UI 线程上分发 |
| 界面 | `src/shell/MainForm*.cs`、`DesignerView.cs` | 只呈现内核状态、转发命令 |
| 测试 | `tests/kernel/core_tests.cpp`（125 项）、`tests/kernel/host_tests.cpp`（30 项）、`tests/gui/*.cs` | 内核测试只经过公开 ABI |

`OpenMSE.Core.dll` 为 x64，静态链接 C/C++ 运行库，不加载 MSENV、Office、COM 组件或 CLR。旧引擎、MSBuild 和被调试程序都在独立进程中运行。

## 文档（阶段 1）

- 内核为每个文档持有唯一的权威缓冲区：UTF-8、LF。磁盘编码（`utf-8`、`utf-8-bom`、`utf-16le`、`utf-16be`、`cp<代码页>`）和换行策略单独保存。
- 每次修改携带 expectedRevision，过期返回 `conflict`。修改、撤销、重做都会递增版本；保存不改变文本和版本。
- 位置为零基行号和 UTF-16 列号。列号落在代理对中间、越过行尾或范围倒置时返回 `invalid_argument`。
- 撤销历史保存编辑差量，而不是整份文本。带 `OMK_EDIT_COALESCE` 的相邻短输入、连续退格或删除合并为一步；换行、保存、撤销和重做会结束当前合并。脏状态按历史状态号比较：撤销回已保存状态即为干净。
- 打开时按规范化路径（不区分大小写）和卷序列号+文件索引去重。另存为的目标若已在其他文档中打开则拒绝。
- 保存前比较磁盘字节的长度和 FNV-1a 哈希；文件被外部修改或删除时返回 `conflict`，不覆盖。写入同目录临时文件，刷盘后用 `ReplaceFileW` / `MoveFileExW` 替换。代码页中无法表示的字符导致保存失败（`io_error`），不做替换。
- 关闭策略：`refuse_dirty` 对脏文档返回 `unsaved_document`；`save` 先保存；`discard` 丢弃。
- 命令：`file.save`、`file.close`、`edit.undo`、`edit.redo`（`omk_command_query` / `omk_command_execute`），菜单启用状态来自命令查询。

## 工具箱与设计（阶段 2）

**ToolboxService** 读取 `config/toolbox.json`，校验分类、控件类型和 HTML 片段，分配稳定 ID（例如 `html.button`、`winforms.datagridview`）并标明后端。界面只负责图标和拖放。

**DesignService** 按扩展名选择适配器：`.html`/`.htm` 使用源码片段扫描器，`.omform` 使用 JSON 布局。节点 ID（`n0`、`n1`…）只在快照报告的版本内有效。设计操作有四种：`add`、`set`、`delete`、`set_form`，每个都携带期望版本。内核核对版本，生成并完整校验新源码，再通过 DocumentService 以一次最小区间编辑原子提交，即一个版本、一步撤销。结果携带已应用的编辑、应选中的节点和新快照。

- **HTML**：只替换所选元素的开始标签或文本，或在 `</body>` 前插入片段；注释、脚本、未知标签按字节保留。
- **`.omform`**：在原位置替换 JSON 值，缺少的成员按所在对象的缩进补入，未知成员和原有格式保留；修改后整个布局必须仍然有效，否则不提交。
- **导出**：`omk_design_export` 生成独立的 C# / VB 窗体源码。

## 工作区与任务（阶段 3）

**WorkspaceService** 解析 `.sln`（项目、解决方案配置、项目配置映射与 Build.0 标记）以及 VS2005 格式的 `.csproj` / `.vbproj`。项目文件用内置的最小 XML 读取器解析，不支持 DTD。

- 对每个项目求值 `'$(Configuration)|$(Platform)'` 条件属性组，得到 OutputPath、AssemblyName、OutputType、PlatformTarget 和预期产物路径。
- 读取 Compile/Content/None/EmbeddedResource 文件、ProjectReference（解析为工作区内的项目 ID）和程序集引用。
- 也可以直接打开单个项目文件。界面的项目树显示这个图，不再列目录。

**ToolService** 发现本机 .NET Framework 2.0 / 3.5 / 4.x 的 MSBuild、csc、vbc，带文件版本。没有 ToolsVersion 的 VS2005 项目使用 MSBuild 2.0。

**TaskManager**：构建和运行都是有身份的任务，调用立即返回。

- 子进程不创建窗口，只继承指定的管道句柄，并放入作业对象，因此取消会结束整棵进程树（测试中 MSBuild 启动的 `ping` 也会结束）。
- 事件：`task.started`、`task.output`（逐行）、`task.finished`。结束状态为 `succeeded`、`failed` 或 `cancelled`，并记录真实退出码。
- 诊断从编译器的标准格式中解析出文件、行、列、代码和消息，相对路径按项目目录解析，重复行去重。错误数和警告数按诊断计数，不依赖本地化的摘要文本。
- 只有在本次任务开始后写入的预期产物才计为“产物”。运行任务执行启动项目的产物，并捕获输出。
- 界面提供配置选择、生成、生成并运行、取消、错误列表（双击跳转到源码位置）。UI 用 100 ms 的计时器在 UI 线程上取事件。

## 旧宿主（阶段 4）

`config/hosts.json` 是宿主清单：引擎、位数、入口、文件版本、SHA-256、启动器及参数、PID 文件、依赖根、配置命名空间、桥接程序和能力。`omk_hosts_load` 逐项核对版本和哈希，不一致的条目不会被启动（返回 `dependency_missing`）。

**HostManager** 的规则：

- 只操作自有实例。通过启动器启动宿主后，核对进程映像路径与清单一致、创建时间晚于本次启动；随后一直持有进程句柄，防止 PID 被复用后误绑定。不提供附加到任意 PID 的接口。启动器本身不放入作业对象，以便它启动的 IDE 可以在启动器退出后继续运行。
- 握手：桥接回报协议版本（需要 2）以及 PID 和创建时间，必须与本次启动的实例一致。
- 请求异步进行，调用方从不阻塞。结果为 `ok`、`error`、`timeout`、`not_executed`、`disconnected` 或 `host_exited`。
- 超时表示结果未知，不表示没有执行。宿主随之进入 `needs_verification`，在 `omk_host_verify` 重新确认身份前拒绝所有写操作；迟到的应答只记录为 `late`，不会应用，内核也不会自动重放任何请求。
- 在桥接队列中已过截止时间的请求不会执行，并回报 `not_executed`。
- 桥接进程退出后宿主变为 `disconnected`，IDE 保持运行；`omk_host_reconnect` 重新核对身份后启动新的桥接。宿主进程退出后变为 `exited`，未完成的请求结束为 `host_exited`。
- `omk_host_sync_document` 把内核文档写入宿主缓冲区，按内容幂等：内容已相同时不写；宿主缓冲区与上次同步的文本不同（即有人在宿主中修改过）时返回冲突。内核文档始终是权威，宿主退出、断开或重启都不影响它。
- 停止方式：`detach`（保留 IDE）、`shutdown`（宿主可能因未保存缓冲区而拒绝）、`terminate`（只针对自有实例）。内核销毁时会结束仍在运行的隐藏自有实例。
- 宿主若在超时前没有完成绑定，内核会读取该进程显示的对话框文本并写入错误信息。本机的实际案例是 VS2005 SP1 启动时弹出的模态提示“建议使用管理员权限”，隐藏实例无法关闭它。`scripts/setup.ps1` 现在只在本项目私有的 VS8 用户配置里写入 `ShowWindowsAdminDlg=0`，不修改全局 VS 设置。

界面在“工具”菜单提供启动、同步当前文档、核对和关闭 VS2005 后端的命令，状态栏显示宿主状态。清单缺失或核对失败时这些命令不可用。

## 调试与能力（阶段 5）

VS2005 的 `Debugger.Go` 在本机隐藏和可见实例中都返回 `0x80004005`；打开解决方案、构建和创建断点都成功，也没有任何对话框（实测脚本见下文）。该能力因此保持 `blocked`，调试改由新实现提供：

**OpenMSE.DebugHost**（x86 与 x64 各一份）用 `CreateDebuggingInterfaceFromVersion("v2.0.50727")` 获取 ICorDebug，用 `CorSymBinder_SxS` 读取 PDB。

- 断点按文件和行绑定到第一个有序列点的行，模块加载时绑定；在暂停或运行中添加时也会绑定（运行中会短暂暂停目标）。
- 单步覆盖整行源码；跳过隐藏序列点；只在有符号的模块中停下（JMC）。断点、中断或异常会结束未完成的单步。
- 调用堆栈逐帧给出函数名、源文件、行和列。局部变量来自符号作用域，参数名来自元数据。
- 求值只支持“变量名”和“变量名.字段”，不执行被调试程序的任何代码。
- 程序输出逐行转发；退出时回报真实退出码，并释放符号读取器，避免 `.pdb` 被占用而导致下一次生成失败。

**DebugService**（内核）按目标 PE 头选择适配器：PE32+，或者只含 IL 且未要求 32 位的 AnyCPU 程序，在 64 位系统上使用 x64 适配器，其余使用 x86。会话状态为 `starting`、`running`、`stopped`、`terminating`、`exited`、`failed`。只有 `stopped` 允许单步、读取变量和求值，只有 `running` 允许暂停。事件：`debug.state`、`debug.stopped`（附带调用堆栈）、`debug.output`、`debug.breakpoint`、`debug.exited`。

界面提供“调试”菜单：F5 生成并调试或继续、F10、F11、Shift+F11、Shift+F5、F9 切换断点、全部中断。另有“调用堆栈”和“局部变量”页；暂停时打开对应源文件，当前行显示黄色、断点行显示红色。

**能力注册表** `omk_capabilities` 列出每项能力的提供者和证据等级，界面只在 `debug.clr2` 为 `behavior_verified` 时启用调试命令。补全和跳转到定义目前没有提供者，标为 `unsupported`；诊断来自构建。

**MSE 独立验收**：通过 HostManager 启动自有的隐藏 MSE 实例后，桥接持续报告 `automation_not_registered`（运行对象表中没有 PID 匹配的 DTE），启动在 30 秒后以 `failed` 结束，并由内核结束该实例。MSE 的自动化、文档、构建和调试能力因此仍为 `blocked`，详见[能力矩阵](capabilities.md)。

## 构建与验证

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\scripts\build-shell.ps1   # 内核、调试适配器、界面
powershell.exe -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\scripts\build-bridge.ps1  # HostBridge（需要 EnvDTE 8.0）
D:\OpenMSE\build\core-tests.exe <独立测试目录>
D:\OpenMSE\build\host-tests.exe D:\OpenMSE <独立测试目录>     # 会启动并结束自有的隐藏 VS2005 / MSE 实例
D:\OpenMSE\bin\OpenMSE.Shell.exe --smoke-test <独立测试目录>
```

已验证（2026-11-20，本机）：

- 内核测试 125/125，覆盖以下内容：
  - 文档与设计。
  - 由 VB 库和 C# 程序组成的双项目解决方案：Debug/Release 构建、运行与中文输出，失败构建定位到 `Program.cs` 第 6 行第 17 列 CS1525，取消时整棵进程树被结束。
  - 调试：Program.cs 第 8 行命中断点，单步后变量从 41 变为 42，单步进入 VB 库并得到两帧调用堆栈，单步跳出，运行中添加断点，退出码 42，运行或退出状态下拒绝单步与求值。
  - AnyCPU 程序使用 x64 适配器，可以暂停、读取循环变量并终止。
  - 能力注册表。
- 宿主测试 30/30，在真实 VS2005 上覆盖：
  - 篡改清单被拒绝；启动调用是异步的。
  - 同步幂等；超时后进入核对状态并锁定写操作；迟到的应答只记录。
  - 队列中过期的请求不会执行。
  - 桥接断开后重新连接到同一实例；宿主被结束后可重启新实例并从内核恢复文档。
  - 优雅关闭，以及 MSE 的独立验收。
  - 测试结束后不残留任何 IDE 进程。
- GUI 冒烟测试全部通过，覆盖：
  - 生成并运行；失败构建进入错误列表并可跳转。
  - F9/F5/F10/继续 的调试流程，以及调用堆栈、局部变量和求值。
  - 设计器闭环。
  - 在没有 `runtime/` 和 `hosts.json` 的独立目录中也能运行，此时后端命令不可用。

## 尚未实现与限制

- 内核 ABI 是 JSON 调用，异步只体现在任务、宿主和调试会话上。`services.hpp` 中带 future 的 C++ 接口仍是契约草案，内核没有实现它们。
- 调试器只支持 CLR 2.0（.NET 2.0–3.5）托管程序。不支持函数求值、属性求值、条件断点、附加到进程、多线程视图、异常设置和本机代码调试。运行中移除断点要到下次启动才生效。
- VS2005 的调试能力仍然阻塞，具体原因（调试引擎注册还是路径重定向）未定位。MSE 的自动化接口仍然阻塞。
- 补全、跳转到定义没有实现；语言诊断只来自构建。
- 项目求值只覆盖 VS2005 的常见条件形式，不执行 MSBuild 导入，也不处理通配符和自定义属性；构建本身仍由真正的 MSBuild 完成。
- 拖放、拖动和调整大小的鼠标手势尚未经过自动或手动验证；冒烟测试直接调用这些手势最终提交的操作。
- 内核缓冲区上限 64 MB，编辑器限制 8 MB；每次编辑会重建行索引。
