# 实验桥协议 v1

此协议只描述当前 `HostBridge.exe`，不等于未来 `IHostTransport` 的完整实现。

## 编译与运行

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File D:\OpenMSE\scripts\build-bridge.ps1
cmd.exe /c D:\OpenMSE\build-contracts.cmd
python D:\OpenMSE\scripts\audit-kernel.py
```

桥依赖 .NET Framework 4 编译器、System.Web.Extensions 和本机 EnvDTE 8.0 PIA，构建为 x86。契约编译依赖 MSVC 的 x64 工具链，生成编译检查对象文件，不生成新 IDE。

独立使用形式：`build\HostBridge.exe PID mse|vs8 PROJECT_ROOT WORKSPACE OWNED_MARKER_OR_DASH --stdio`。marker 是由测试启动器写出的 JSON，包含 pid、engine、createdFileTime。`-` 仅允许只读方法；marker 不是认证令牌，协议只用于本地可信调用方。

## 消息

UTF-8，一行一个 JSON。启动成功后发出 ready：

```json
{"type":"ready","protocolVersion":1,"pid":1234,"engine":"vs8"}
{"id":"1","method":"host.identity","params":{}}
{"id":"1","ok":true,"result":{"pid":1234,"engine":"vs8"}}
```

最后一行仅为结果结构示意，真实 identity 还包含创建时间、moniker、name、version 和 owned。失败返回 `{id,ok:false,error:{code,message,hresult}}`。连接失败返回 id=null，进程退出码 1。成功的 EOF 仅断开桥，不关闭 IDE。

## 已实现的方法

| 方法 | 参数 / 结果 |
|---|---|
| host.identity | 返回绑定宿主身份 |
| host.inspect | 文档数、工程状态、具名命令清单、调试状态 |
| documents.open / read / save / close | path；open 强制文本视图；close 拒绝未保存文档 |
| documents.write | path、expectedText、text；完整旧文本不匹配时拒绝 |
| projects.open | path；打开 solution |
| build.run | 同步构建；返回 failedProjects 和 state。failedProjects 是失败项目数 |
| debug.inspect | 当前模式；只有暂停态才尝试读取调用栈 |
| debug.breakpoints.add | path、line；line 为旧 DTE 从 1 开始的行号 |
| debug.start / step_over / stop | 启动、单步、停止本次宿主调试 |
| debug.evaluate | expression；仅允许暂停态 |
| host.shutdown | 显式关闭自有测试实例，要求无调试活动且文档已保存 |

除 identity、inspect、read、debug.inspect 外的方法要求自有实例 marker。路径参数限制在显式工作区内；绑定校验要求 exe 路径、PID、进程创建时间以及 DTE 主窗口 PID 一致。

## 限制

仅顺序请求/响应，无事件通知、协商、批处理、持久任务、取消和 COM 调用截止时间。测试客户端有超时，必要时只终止自己创建的桥进程，不终止 IDE；这不保证已提交的 COM 操作未执行。

路径检查是规范化字符串边界检查，不提供对 reparse point、并发修改或恶意工程的安全隔离。expectedText 检查与后续 DTE 写入不是原子事务，不替代未来的版本控制。大文档受 JSON 长度限制；当前一行长度检查也不能防止读取超长行时的内存增长。

默认未知异常为 host_operation_failed，部分参数错误尚未精细分类。非法方法、脏文档关闭、内容冲突、越界路径、设计态求值已经行为测试。调试方法虽已实现，正常调试流程尚未通过实测，调用方必须参考能力矩阵。
