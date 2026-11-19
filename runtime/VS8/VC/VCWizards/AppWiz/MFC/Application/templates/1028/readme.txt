================================================================================
    MFC 程式庫 : [!output PROJECT_NAME] 專案概觀
===============================================================================

應用程式精靈已經為您建立了這個 [!output PROJECT_NAME] 應用程式。這個應用程式
不僅示範了使用 MFC 的基本概念，也是您撰寫應用程式的起點。

這個檔案的內容摘要，包含各個構成 [!output PROJECT_NAME] 應用程式的檔案。

[!output PROJECT_NAME].vcproj
    這是使用應用程式精靈所產生之 VC++ 專案的主要專案檔。 
    它包含產生檔案時的 Visual C++ 版本資訊，以及使用應用程式精靈產生檔案的
    過程中，選取的平台、組態和專案功能的相關資訊。

[!output APP_HEADER]
    這是應用程式的主要標頭檔。它包含其他專案特定的標頭 (包括 Resource.h)，
    並宣告 [!output APP_CLASS] 應用程式類別。

[!output APP_IMPL]
    這是主要的應用程式原始程式檔，它包含應用程式類別 [!output APP_CLASS]。

[!output PROJECT_NAME].rc
    這份清單包含了所有程式要使用的 Microsoft Windows 資源。其中包括了儲存
    在 RES 子目錄下的圖示、點陣圖和游標。您可以直接在 Microsoft Visual C++ 
    內編輯這個檔案。您的專案資源在 [!output LCID]。

res\[!output PROJECT_NAME].ico
    這個圖示檔是用來做為應用程式的圖示。這個圖示包含在主要資源檔
    [!output PROJECT_NAME].rc 內。

res\[!output RC2_FILE_NAME].rc2
    這個檔案包含 Microsoft Visual C++ 編輯無法編輯的資源。您應該將所有資源
    編輯器無法編輯的資源放到這個檔案。
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]

[!output PROJECT_NAME].reg
    這是範例 .reg 檔，說明 Framework 為您設定的登錄設定類型。
    您可以使用這個檔案，
[!if APP_TYPE_DLG]
    做為與應用程式一起執行的 .reg。
[!else]
    做為與應用程式一起執行的 .reg，或者直接將它刪除，使用預設的
    RegisterShellFileTypes 登錄檔。
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
    這個檔案包含介面描述語言 (Interface Description Language) 原始程式碼，
    適用於您的應用程式的型別程式庫。
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

主框架視窗:
[!if PROJECT_STYLE_EXPLORER]
    Windows 檔案總管樣式: 專案含有類似 Windows 檔案總管的介面，包括兩個框架。
[!else]
    專案含有標準 MFC 介面。
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
    這些檔案包含框架類別 [!output MAIN_FRAME_CLASS]，該框架類別衍生自 
[!if APP_TYPE_MDI]
    CMDIFrameWnd，並控制所有 MDI 框架功能。
[!else]
    CFrameWnd，並控制所有 SDI 框架功能。
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
    這些檔案包含左框架類別 [!output TREE_VIEW_CLASS]，該框架類別衍生自
    CTreeView。
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
    這個點陣圖檔是用來建立並排顯示在工具列的影像。
    初始工具列和狀態列是在 [!output MAIN_FRAME_CLASS] 類別中建構的。
    您可以使用資源編輯器，編輯這個工具列點陣圖，而且，您也可以更新
    [!output PROJECT_NAME].rc 的 IDR_MAINFRAME TOOLBAR 陣列，即可
    新增工具列的按鈕。
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
    當您的伺服器應用程式在其他容器內就地啟動時，您可以使用這個點陣圖檔，
    建立並列在工具列的影像。這個工具列是在 [!output INPLACE_FRAME_CLASS] 
    類別中建構的。這個點陣圖類似 res\Toolbar.bmp 內的點陣圖，差別在於，
    它移除許多非伺服器命令的點陣圖。

[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

子框架視窗:

ChildFrm.h、ChildFrm.cpp
    這些檔案定義並實作 [!output CHILD_FRAME_CLASS] 類別，這個類別支援 MDI 
    應用程式的子視窗。

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
應用程式精靈建立文件類型和檢視:

[!output DOC_HEADER]、[!output DOC_IMPL] - 文件
    這些檔案包含您的 [!output DOC_CLASS] 類別。編輯這些檔案，即可新增您的特
    殊文件資料，並實作檔案儲存和載入 (透過 [!output DOC_CLASS]::Serialize)。
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    文件將具有下列字串:
        副檔名:      	     [!output FILE_EXTENSION]
        檔案類型 ID:         [!output FILE_TYPE_ID]
        主框架標題: 	     [!output MAIN_FRAME_CAPTION]
        文件類型名稱:        [!output DOC_TYPE_NAME]
        篩選條件名稱:          [!output FILTER_NAME]
        檔案新簡短名稱:    [!output FILE_NEW_NAME_SHORT]
        檔案類型完整名稱:    [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
應用程式精靈建立檢視:
[!endif]

[!output VIEW_HEADER]、[!output VIEW_IMPL] - 文件的檢視
    這些檔案包含您的 [!output VIEW_CLASS] 類別。
[!if !DB_VIEW_NO_FILE]
    [!output VIEW_CLASS] 物件是用來檢視 [!output DOC_CLASS] 物件。
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
    這個圖示檔是用來做為 [!output DOC_CLASS] 類別之 MDI 子視窗的圖示。
    這個圖示是包含在主要資源檔 [!output PROJECT_NAME].rc 內。
[!endif]

[!endif]

[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

資料庫支援:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
    這些檔案包含您的 [!output ROWSET_CLASS] 類別。這個類別是用來存取您
    在精靈中選取的資料來源。
[!if DB_VIEW_NO_FILE]
    將不會新增序列化支援。
[!else]
    已新增序列化支援。
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

應用程式精靈也建立了 OLE 專用的類別

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
    這些檔案包含您的 [!output CONTAINER_ITEM_CLASS] 類別。這個類別是用來
    管理 OLE 物件。OLE 物件通常會依您的 [!output VIEW_CLASS] 類別來顯示，
    並且序列化為 [!output DOC_CLASS] 類別的一部分。
[!if ACTIVE_DOC_CONTAINER]
    程式支援在框架內包含主動式文件 (Active Document)。
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
    這些檔案包含您的 [!output SERVER_ITEM_CLASS]。這個類別是用來連結您的
    [!output DOC_CLASS] 類別和 OLE 系統，並且選擇性提供文件的連結。
[!if ACTIVE_DOC_SERVER]
    專案支援建立及管理主動式文件。
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
    這些檔案包含您的 [!output INPLACE_FRAME_CLASS]。這個類別衍生自
    COleIPFrameWnd，並在就地啟動期間，控制所有的框架功能。
[!endif]

[!if SUPPORT_COMPOUND_FILES]
    專案支援複合檔案 (Compound File)。複合檔案格式將包含一或多個 Automation
    物件的文件儲存為一個檔案，並且仍然允許個別物件存取該檔案。
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
應用程式精靈建立一個對話方塊類別和 Automation Proxy 類別:
[!else]
應用程式精靈建立一個對話方塊類別:
[!endif]

[!output DIALOG_HEADER]、[!output DIALOG_IMPL] - 對話方塊
    這些檔案包含您的 [!output DIALOG_CLASS] 類別。這個類別定義應用程式主
    對話方塊的行為。對話方塊的範本在 [!output PROJECT_NAME].rc 內，您可以 
    在 Microsoft Visual C++ 編輯它。
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] - Automation 物件
    這些檔案包含您的 [!output DIALOG_AUTO_PROXY_CLASS] 類別。這個類別稱為
    對話方塊的 Automation Proxy 類別，因為它負責公開 Automation 方法和屬性，
    Automation 控制程式使用這些方法和屬性來存取對話方塊。這些方法和屬性不是
    直接由對話方塊類別公開，因為，對於強制回應對話方塊架構的 MFC 應用程式
    而言，將 Automation 物件從使用者介面區隔開來會比較完整且容易。
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

說明支援:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
    這個檔案是說明專案檔。它包含說明檔要編譯為 .chm 檔所需要的資料。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
    這個檔案列出說明專案的內容。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
    這個檔案包含說明主題的索引。

hlp\afxcore.htm
    這個檔案包含標準 MFC 命令和螢幕物件的標準說明主題。將您的說明主題
    加入至這個檔案。

[!if PRINTING]
hlp\afxprint.htm
    這個檔案包含列印命令的說明主題。

[!endif]
makehtmlhelp.bat
    建置系統使用這個檔案來編譯說明檔。

hlp\Images\*.gif
    這些是 MFC 程式庫標準命令的標準說明檔主題所需要的點陣圖檔。

[!else]
hlp\[!output PROJECT_NAME].hpj
    這個檔案是說明專案檔，說明編譯器用它來建立應用程式說明檔。

hlp\*.bmp
    這些是 MFC 程式庫標準命令的標準說明檔主題所需要的點陣圖檔。

hlp\*.rtf
    這些檔案包含標準 MFC 命令和螢幕物件的標準說明主題。
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

其他功能:
[!if ACTIVEX_CONTROLS]

ActiveX 控制項
    應用程式支援 ActiveX 控制項的使用。
[!endif]
[!if PRINTING]

列印和預覽列印支援
    應用程式精靈從 MFC 程式庫呼叫 CView 類別中的成員函式，產生了可以處理
    列印、列印設定和預覽列印命令的程式碼。
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

資料庫支援
    應用程式精靈已經為您的程式加入基礎層級的資料庫支援。 
    只包含了需要的檔案。
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

分隔視窗
    應用程式精靈已經為您的應用程式文件加入分隔視窗支援。
[!endif]
[!if MAPI]

MAPI 支援
    產生的專案包含建立、管理、傳輸和儲存郵件所需要的程式碼。
[!endif]
[!if SOCKETS]

Windows Sockets
    應用程式具有透過 TCP/IP 網路建立通訊的支援。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

其他標準檔案:

StdAfx.h、StdAfx.cpp
    這些檔案是用來建置名為 [!output PROJECT_NAME].pch 的先行編譯標頭 (PCH) 檔，
    以及名為 StdAfx.obj 的先行編譯型別檔。

Resource.h
    這是標準標頭檔，它定義新的資源 ID。
    Microsoft Visual C++ 會讀取和更新這個檔案。

[!if MANIFEST]
[!output PROJECT_NAME].manifest
	應用程式資訊清單檔案是 Windows XP 用來在並存組件的特定版本上，描述
	應用程式的相依性。載入器會利用這項資訊，從組件快取載入適當的組件，
	或者從應用程式載入私密金鑰。應用程式資訊清單可能用來做為外部  
	.manifest 檔的轉散發，這個 .manifest 檔的安裝位置，和應用程式可執行檔
	的資料夾相同，或者，它也可能以資源的形式包含在可執行檔內。 
[!endif]
/////////////////////////////////////////////////////////////////////////////

其他注意事項:

應用程式精靈使用 "TODO:" 來指示您應該加入或自訂的原始程式碼部分。
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

如果您的應用程式使用 MFC 的共用 DLL，您將需要轉散發 MFC DLL。
如果您的應用程式與作業系統的地區設定不同，您也必需轉散發對應的
當地語系化資源 MFC80XXX.DLL。如果需要這些主題的詳細資訊，請查
閱 MSDN 文件有關轉散發 Visual C++ 應用程式的章節。
[!endif]

/////////////////////////////////////////////////////////////////////////////
