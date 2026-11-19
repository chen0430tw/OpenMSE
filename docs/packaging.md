# 打包与安装检查（阶段 6）

OpenMSE 分为一个核心包和三个可选包。每个包只声明经过实测的场景，每个场景都有一条可以在“组装好的包目录”中运行的验证命令。包的定义见 `config/packages.json`，实测得到的依赖闭包见 `config/packages/<包名>.json`。

| 包 | 内容 | 场景 | 依赖 |
|---|---|---|---|
| `core` | 统一界面、内核、CLR 2.0 调试适配器（x86/x64）、工具箱、示例、文档、安装检查 | 编辑、设计、项目图、生成/运行、调试 | Windows x64、.NET 4.x、.NET 2.0/3.5（MSBuild 2.0、mscordbi） |
| `vs8-backend` | 启动器、重定向层、桥接、宿主清单、VS8 注册表模板，以及实测的 `runtime/` 子集 | 后端宿主生命周期；DTE 文档编辑保存、打开解决方案并生成 | core；EnvDTE 8.0 PIA（GAC）；闭包清单中列出的外部依赖 |
| `mse-window` | 启动器、重定向层、MSE 注册表模板、核验脚本，以及实测的 `runtime/` 子集 | 以 HTML 文档启动 MSE 兼容窗口，编辑器与设计模块正常加载 | core；闭包清单中列出的外部依赖 |
| `legacy-full` | 完整的 `runtime/` | 所有旧 IDE 场景，包括未测量的设计器、Crystal Reports、SmartDevices 等 | core |

核心包不包含任何旧运行组件。可选包只在装有对应原始依赖（GAC 程序集、原安装中仍被读取的文件）的机器上可用；安装检查会逐项报告这些依赖。

## 依赖闭包的测量方法

`OpenMSEHook.dll` 支持可选的文件访问追踪：设置环境变量 `OPENMSE_TRACE_FILES=<文件>`（可用 `{engine}` 占位）后，旧进程每次成功打开的文件和目录（`NtCreateFile` / `NtOpenFile`，包括作为数据文件加载的资源 DLL）都会去重后记录下来。未设置该变量时不追踪，也不改变任何重定向行为。

```powershell
python scripts\package.py trace      # 运行各场景并记录追踪，结果在 logs\package-trace
python scripts\package.py manifest   # 归类，写出 config\packages\vs8-backend.json、mse-window.json
python scripts\package.py assemble D:\某个新目录 core vs8-backend mse-window
```

归类规则：

- **打进包里**：项目根下 `runtime\` 中、并且受 Git 跟踪的文件；目录也会一并记录。
- **外部依赖**：原 VS2005/Office 安装目录中的文件、GAC 程序集、.NET Framework 文件。只记录，不打包。

闭包只代表被测场景用到的文件。场景之外的功能（例如其他设计器）需要 `legacy-full`。

## 验证

每个包组装到一个新目录后，在该目录中运行它的场景。目录变化还会同时验证路径重定位，因为新根目录会得到自己的注册表实例键。

```powershell
python scripts\package.py assemble D:\OpenMSE\dist\pkg core vs8-backend mse-window
D:\OpenMSE\dist\pkg\bin\OpenMSE.Shell.exe --smoke-test <新目录>             # core
D:\OpenMSE\build\host-tests.exe D:\OpenMSE\dist\pkg <新目录>                  # vs8-backend 生命周期 + MSE 验收
python scripts\package.py scenario-vs8-build D:\OpenMSE\dist\pkg <新目录>     # vs8-backend DTE 文档与生成
python scripts\package.py scenario-mse-window D:\OpenMSE\dist\pkg            # mse-window
powershell -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\dist\pkg\scripts\check-install.ps1 -Full
```

## 实测结果（2026-11-20，本机）

| 包 | 打包的 `runtime/` 文件 | 大小 | 外部依赖（实测） | 在组装目录中的验证 |
|---|---|---|---|---|
| `core` | 0（共 21 个文件） | 约 1 MiB | .NET 4.x、.NET 2.0/3.5 | GUI 冒烟测试全部通过（编辑、设计器闭环、生成/运行、失败定位、F9/F5/F10 调试） |
| `vs8-backend` | 46 | 约 23 MiB | 33 个 GAC 程序集、27 个 .NET Framework 文件、原安装中的 2 个文件 | `host-tests` 30/30；`scenario-vs8-build` 通过（DTE 编辑保存文档、打开解决方案、生成 0 个失败项目、产物退出码 0） |
| `mse-window` | 32 | 约 44 MiB | 原 Office 安装中的 1 个文件 | `scenario-mse-window` 通过（设计窗口加载、无错误对话框、无模块来自原安装） |
| `legacy-full` | 完整 `runtime/` | 约 2.2 GiB | — | 沿用此前对完整运行目录的验证 |

`core + vs8-backend + mse-window` 合计 107 个文件、约 71 MiB。完整运行目录约 2.2 GiB。

这些包仍依赖原安装中的文件：

- VS2005 以类型库方式读取原安装的 `Common7\IDE\dteproperties.tlb` 和 `msenv.dll`。
- MSE 读取原 Office 的 `Cultures\OFFICE.ODF`。

原因是重定向层只改写 DLL 加载，不改写普通文件访问。安装检查会逐项核对这些文件。在没有原安装的机器上，这两个可选包不可用；核心包不受影响。

测量过程中发现并修正了两个问题：

1. **追踪缺口**。重定向层作为第一个导入项注入，加载器会在任何 `DllMain` 运行之前先解析静态导入，因此启动早期加载的 DLL 不会出现在追踪里。第一次组装的包因此缺少 `devenv.exe` 的静态导入 `custsat.dll`，VS2005 启动后立即退出。现在 `manifest` 会解析闭包中每个 PE 文件的导入表和延迟导入表，补齐传递依赖。
2. **启动器不创建 `logs\`**。在新解压的目录中，PID 文件写不出来，内核因此无法绑定宿主。启动器现在会先创建该目录。

## 安装检查

`scripts\check-install.ps1` 检查以下内容，并写出 `logs\install-check.json`：

- 系统前提。
- 已安装包的完整性：默认比较文件大小，`-Full` 时比较 SHA-256。
- 每个可选包入口程序的版本和 SHA-256（与 `config/hosts.json` 一致）。
- 桥接需要的 EnvDTE 8.0。
- 实测闭包中的外部依赖是否存在。

核心包可用时退出码为 0。界面本身也会降级：缺少宿主清单或核对失败时后端命令不可用，缺少调试适配器时调试命令不可用。
