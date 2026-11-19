@echo off
[!if HM_NOTE]

REM -- 请注意 OEM 字符集与 ANSI 字符集的差异
REM -- 在大多数编辑器中，下面的许多文件名可能无法正常显示，因为
REM –– 要使批处理文件正常工作，它们的字符集应为 OEM，  
REM –– 而不是 ANSI。输出窗口和编辑器窗口使用 ANSI 字符集。  
REM -- 如果名称只出现在输出窗口中，它们将保留为
REM -- ANSI 字符集。

[!endif]
REM -- 首先，从 resource.h 创建映射文件。
echo // MAKEHELP.BAT 生成的帮助映射文件。由 [!output PROJECT_NAME].HPJ 使用。 > "hlp\[!output HM_FILE_OEM].hm"
echo. >>hlp\[!output HM_FILE_OEM].hm
echo // 命令 (ID_* 和 IDM_*) >> "hlp\[!output HM_FILE_OEM].hm"
makehm ID_,HID_,0x10000 IDM_,HIDM_,0x10000 resource.h >> "hlp\[!output HM_FILE_OEM].hm"
echo. >> "hlp\[!output HM_FILE_OEM].hm"
echo // 提示 (IDP_*) >> "hlp\[!output HM_FILE_OEM].hm"
makehm IDP_,HIDP_,0x30000 resource.h >> "hlp\[!output HM_FILE_OEM].hm"
echo. >> "hlp\[!output HM_FILE_OEM].hm"
echo // 资源 (IDR_*) >> "hlp\[!output HM_FILE_OEM].hm"
makehm IDR_,HIDR_,0x20000 resource.h >> "hlp\[!output HM_FILE_OEM].hm"
echo. >> "hlp\[!output HM_FILE_OEM].hm"
echo // 对话框 (IDD_*) >> "hlp\[!output HM_FILE_OEM].hm"
makehm IDD_,HIDD_,0x20000 resource.h >> "hlp\[!output HM_FILE_OEM].hm"
echo. >> "hlp\[!output HM_FILE_OEM].hm"
echo // 框架控件 (IDW_*) >> "hlp\[!output HM_FILE_OEM].hm"
makehm IDW_,HIDW_,0x50000 resource.h >> "hlp\[!output HM_FILE_OEM].hm"
REM -- 生成 [!output PROJECT_NAME] 项目的帮助
start /wait hcw /C /E /M "[!output PROJECT_NAME_OEM].hpj"
echo.