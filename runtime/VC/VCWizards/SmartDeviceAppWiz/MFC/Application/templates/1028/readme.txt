================================================================================
    MFC 程式庫 : [!output PROJECT_NAME] 專案概觀
===============================================================================

應用程式精靈已經為您建立了這個 [!output PROJECT_NAME] 應用程式。這個應用程式
不僅示範了使用 MFC 的基本概念，也是您撰寫應用程式的起點。

這個檔案的內容摘要，包含各個構成 [!output PROJECT_NAME] 應用程式的檔案。

[!output PROJECT_NAME].vcproj
    這是使用應用程式精靈所產生之 VC++ 專案的主要專案檔。它包含產生檔案時
    的 Visual C++ 版本資訊，以及使用應用程式精靈產生檔案的過程中，選取的
    平台、組態和專案功能的相關資訊。

[!output APP_HEADER]
    這是應用程式的主要標頭檔。它包含其他專案特定的標頭，並宣告
    [!output APP_CLASS] 應用程式類別。

[!output APP_IMPL]
    這是主要的應用程式原始程式檔，它包含應用程式類別 [!output APP_CLASS]。

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
    這是專案的主要資源檔案清單，包含專案要使用的所有 Microsoft Windows 資源。
    其中包括了儲存在 RES 子目錄下的圖示、點陣圖和游標。您可以直接在
    Microsoft Visual C++ 內編輯這個檔案。您的專案資源在 [!output LCID]。保存
    .rc 檔時，資料區段中的定義也會以所定義的十六進位數值保存下來，而不是保存
    定義的易記名稱。

res\[!output RC2_FILE_NAME].rc2
    這個檔案包含 Microsoft Visual C++ 編輯無法編輯的資源。您應該將所有資源
    編輯器無法編輯的資源放到這個檔案。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
    這是專案的主要資源檔案清單，包含專案在 Pocket PC 平台或支援相同使用者
    介面模型的平台進行編譯時，要使用的所有 Microsoft Windows 資源。其中
    包括了儲存在 RES 子目錄下的圖示、點陣圖和游標。您可以直接在 
    Microsoft Visual C++ 內編輯這個檔案。您的專案資源在 [!output LCID]。
    保存 .rc 檔時，資料區段中的定義也會以所定義的十六進位數值保存下來，
    而不是保存定義的易記名稱。

res\[!output RC2_FILE_NAME]ppc.rc2
    這個檔案包含 Microsoft Visual C++ 編輯無法編輯的資源。您應該將所有資源
    編輯器無法編輯的資源放到這個檔案。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
    這是專案的主要資源檔案清單，包含專案在 Smartphone platform 平台或支援
    相同使用者介面模型的平台進行編譯時，要使用的所有 Microsoft Windows 資源。
    其中包括了儲存在 RES 子目錄下的圖示、點陣圖和游標。您可以直接在
    Microsoft Visual C++ 內編輯這個檔案。您的專案資源在 [!output LCID]。保存
    .rc 檔時，資料區段中的定義也會以所定義的十六進位數值保存下來，而不是保存
    定義的易記名稱。

res\[!output RC2_FILE_NAME]sp.rc2
    這個檔案包含 Microsoft Visual C++ 編輯無法編輯的資源。
    您應該將所有資源編輯器無法編輯的資源放到這個檔案。
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
    這是專案的主要資源檔案清單，包含專案在支援 AYGShell 使用者介面模型的平
    台進行編譯時，要使用的所有 Microsoft Windows 資源。其中包括了儲存在 RES
    子目錄下的圖示、點陣圖和游標。您可以直接在 Microsoft Visual C++ 內編輯
    這個檔案。您的專案資源在 [!output LCID]。保存 .rc 檔時，資料區段中的定
    義也會以所定義的十六進位數值保存下來，而不是保存定義的易記名稱。

res\[!output RC2_FILE_NAME]ayg.rc2
    這個檔案包含 Microsoft Visual C++ 編輯無法編輯的資源。您應該將所有資源
    編輯器無法編輯的資源放到這個檔案。
[!endif]

res\[!output PROJECT_NAME].ico
    這個圖示檔是用來做為應用程式的圖示。這個圖示包含在主要資源檔內。

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
    這是範例 .reg 檔，說明 Framework 為您設定的登錄設定類型。您可以
    使用這個檔案，
[!if APP_TYPE_DLG]
    做為與應用程式一起執行的 .reg。
[!else]
    做為與應用程式一起執行的 .reg，或者直接將它刪除，使用預設的
    RegisterShellFileTypes 登錄檔。
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

主框架視窗:
    專案含有標準 MFC 介面。

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    這些檔案包含框架類別 [!output MAIN_FRAME_CLASS]，該框架類別衍生自
    CFrameWnd，並控制所有的 SDI 框架功能。

[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
    這個點陣圖檔是用來建立並排顯示在命令列的影像。
    初始命令列是在 [!output MAIN_FRAME_CLASS] 類別中建構的。 
    編輯這個 cmdbar 點陣圖以及 [!output MAIN_FRAME_IMPL] 的
    陣列，即可新增更多的命令列按鈕。
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
應用程式精靈建立文件類型和檢視:

[!output DOC_HEADER]、[!output DOC_IMPL] - 文件
    這些檔案包含您的 [!output DOC_CLASS] 類別。編輯這些檔案，即可新增您的特
    殊文件資料，並實作檔案儲存和載入 (透過 [!output DOC_CLASS]::Serialize)。
[!if HAS_SUFFIX]
    文件將具有下列字串:
        副檔名:    	     [!output FILE_EXTENSION]
        檔案類型 ID:         [!output FILE_TYPE_ID]
        主框架標題: 	     [!output MAIN_FRAME_CAPTION]
        文件類型名稱:        [!output DOC_TYPE_NAME]
        篩選條件名稱:          [!output FILTER_NAME]
        檔案新簡短名稱:    [!output FILE_NEW_NAME_SHORT]
        檔案類型完整名稱:    [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER]、[!output VIEW_IMPL] - 文件的檢視
    這些檔案包含您的 [!output VIEW_CLASS] 類別。
    [!output VIEW_CLASS] 物件是用來檢視 [!output DOC_CLASS] 物件。

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

應用程式精靈建立一個對話方塊類別:

[!output DIALOG_HEADER]、[!output DIALOG_IMPL] - 對話方塊
    這些檔案包含您的 [!output DIALOG_CLASS] 類別。這個類別定義應用程式
    主對話方塊的行為。對話方塊的範本在主要資源檔內，您可以在
    Microsoft Visual C++ 編輯它。
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

說明支援:

hlp\[!output PROJECT_NAME].hpj
    這個檔案是說明專案檔，說明編譯器用它來建立應用程式說明檔。

hlp\*.bmp
    這些是 MFC 程式庫標準命令的標準說明檔主題所需要的點陣圖檔。

hlp\*.rtf
    這些檔案包含標準 MFC 命令和螢幕物件的標準說明主題。
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

其他功能:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]

    具有功能表和裝飾之主框架的命令列
[!else]
[!if CMD_BAR_MENUNBUTTON]
    具有功能表、按鈕和裝飾之主框架的功能表列
[!endif]
[!endif]
[!if STATUS_BAR]
    主框架的初始狀態列
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]

    Windows CE 說明支援
[!endif]
[!if DYNAMIC_MFC]

    使用共用的 DLL 實作
[!endif]
[!if ACTIVEX_CONTROLS]

ActiveX 控制項
    應用程式支援 ActiveX 控制項的使用。
[!endif]
[!if PRINTING]

列印和預覽列印支援
    應用程式精靈從 MFC 程式庫呼叫 CView 類別中的成員函式，產生了可以處理
    列印、列印設定和預覽列印命令的程式碼。
[!endif]
[!if SOCKETS]

Windows Sockets
    應用程式具有透過 TCP/IP 網路建立通訊的支援。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

其他標準檔案:

StdAfx.h, StdAfx.cpp
    這些檔案是用來建置名為 [!output PROJECT_NAME].pch 的先行編譯標頭 (PCH) 檔，
    以及名為 StdAfx.obj 的先行編譯型別檔。

[!if STANDARDSHELL_UI_MODEL]
Resource.h
[!endif]
[!if POCKETPC2003_UI_MODEL]
Resourceppc.h
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
Resourcesp.h
[!endif]
[!if AYGSHELL_UI_MODEL]
Resourceayg.h
[!endif]
    這是標準標頭檔，它定義新的資源 ID。
    Microsoft Visual C++ 會讀取和更新這個檔案。

/////////////////////////////////////////////////////////////////////////////

其他注意事項:

應用程式精靈使用 "TODO:" 來指示您應該加入或自訂的原始程式碼部分。
[!if APP_TYPE_SDI || APP_TYPE_DLG]

如果您的應用程式使用 MFC 的共用 DLL，而且應用程式的語言與作業系統目前的
語言不同，您要將對應的當地語系化資源 MFC80XXX.DLL，複製到您的應用程式目
錄 ("XXX" 為語言縮寫。例如，MFC80DEU.DLL 包含翻譯為德文的資源)。如果您
不這樣執行，您的應用程式有某些 UI 項目，將會保留與作業系統一樣的語言。
[!endif]

/////////////////////////////////////////////////////////////////////////////
