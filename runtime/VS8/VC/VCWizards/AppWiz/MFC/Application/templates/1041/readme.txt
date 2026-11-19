================================================================================
MICROSOFT FOUNDATION CLASS ライブラリ: [!output PROJECT_NAME] プロジェクトの概要
===============================================================================

アプリケーション ウィザードが作成した [!output PROJECT_NAME] には Microsoft 
Foundation Class の基本的な使い方が示されています。アプリケーション作成のひな型
としてお使いください。

このファイルには [!output PROJECT_NAME] アプリケーションを構成している各ファイ
ルの概要説明が含まれます。

[!output PROJECT_NAME].vcproj
   これはアプリケーション ウィザードで生成された VC++ プロジェクトのメイン プ
   ロジェクト ファイルです。ファイルが生成された Visual C++ のバージョン情報が
   含まれています。また、アプリケーション ウィザードで選択したプラットフォー
   ム、構成およびプロジェクト機能に関する情報も含まれています。

[!output APP_HEADER]
   これはアプリケーションのメイン ヘッダー ファイルです。このファイルには、
   Resource.h を含む、その他のプロジェクト固有のヘッダーが含まれていて、
   [!output APP_CLASS] アプリケーション クラスを宣言します。

[!output APP_IMPL]
   これは、アプリケーション クラス [!output APP_CLASS] を含むメイン アプリケー
   ション ソース ファイルです。

[!output PROJECT_NAME].rc
   これは、プログラムが使用する Microsoft Windows のリソースの一覧ファイルで
   す。このファイルには RES サブディレクトリに保存されているアイコン、ビットマ
   ップ、カーソルが含まれています。このファイルは、Microsoft Visual C++ で直接
   編集することができます。プロジェクト リソースは [!output LCID] にあります。

res\[!output PROJECT_NAME].ico
   これは、アプリケーションのアイコンとして使用されるアイコンファイルです。この
   アイコンはメイン リソース ファイル [!output PROJECT_NAME].rc に含まれていま
   す。

res\[!output RC2_FILE_NAME].rc2
   このファイルは Microsoft Visual C++ を使用しないで編集されたリソースを含んで
   います。リソース エディタで編集できないリソースはすべてこのファイルに入れて
   ください。
[!if CONTAINER_SERVER || FULL_SERVER || MINI_SERVER || AUTOMATION || HAS_SUFFIX]
[!if !HTML_EDITVIEW]
[!output PROJECT_NAME].reg
   このレジストリ ファイルは、フレームワークの登録設定方法を紹介するためのサン
   プルファイルです。
[!if APP_TYPE_DLG]
   アプリケーションと一緒に .reg ファイルとして使用します。
[!else]
   アプリケーションと一緒に .reg ファイルとして使用するか、または削除して既定
   の登録 RegisterShellFileTypes を使用します。
[!endif]

[!if AUTOMATION]
[!output SAFE_IDL_NAME].idl
   このファイルは、アプリケーションのタイプ ライブラリ用のインターフェース記述
   言語ソース コードを含みます。
[!endif]
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

メイン フレーム ウィンドウ:
[!if PROJECT_STYLE_EXPLORER]
   Windows Explorer Style: これらのプロジェクトは、2 つフレームを持つ Windows 
   Explorer のようなインターフェイスを含みます。
[!else]
    プロジェクトは標準の MFC インターフェイスを含みます。
[!endif]

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
   これらのファイルは、フレーム クラス [!output MAIN_FRAME_CLASS] を含みます。
[!if APP_TYPE_MDI]
   フレーム クラスは CMDIFrameWnd から派生し、すべての MDI フレーム機能を制御
   します。
[!else]
   フレーム クラスは CFrameWnd から派生し、すべての SDI フレーム機能を制御しま
   す。
[!endif]
[!if PROJECT_STYLE_EXPLORER]

[!output TREE_VIEW_HEADER], [!output TREE_VIEW_IMPL]
   これらのファイルは、CTreeView から派生した左フレーム クラス 
   [!output TREE_VIEW_CLASS] を含みます。
[!endif]

[!if DOCKING_TOOLBAR]
res\Toolbar.bmp
   このビットマップ ファイルはツール バーのタイル イメージとして使用されます。
   初期ツール バーおよびステータス バーは、[!output MAIN_FRAME_CLASS] クラスで
   作成されます。ツール バーのボタンを追加するには、このツール バーのビットマッ
   プをリソース エディタを使用して編集し、[!output PROJECT_NAME].rc ファイル内
   の IDR_MAINFRAME TOOLBAR 配列を更新してください。
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

res\IToolbar.bmp
   サーバー アプリケーションが他のコンテナ内で埋め込み先編集されたときに、この
   ビットマップ ファイルがツール バーのタイル イメージとして使用されます。この
   ツール バーは [!output INPLACE_FRAME_CLASS] クラスで作成されます。このビット
   マップは、多くの非サーバー コマンドが削除されている点を除いては、
   res\Toolbar.bmp 内のビットマップに類似しています。
[!endif]
[!endif]
[!if APP_TYPE_MDI]
/////////////////////////////////////////////////////////////////////////////

子フレーム ウィンドウ:

ChildFrm.h, ChildFrm.cpp
   これらのファイルは、MDI アプリケーションの子ウィンドウをサポートする 
   [!output CHILD_FRAME_CLASS] クラスの定義と実装を行います。

[!endif]
/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
[!if !DB_VIEW_NO_FILE]
アプリケーション ウィザードは 1 つのドキュメントの種類と 1 つのビューを作成し
ます:

[!output DOC_HEADER], [!output DOC_IMPL] - ドキュメント
   これらのファイルは [!output DOC_CLASS] クラスを含みます。特別なドキュメント 
   データを追加したり、ファイルを保存したり、
   (via [!output DOC_CLASS]::Serialize) を読み込んだりするには、これらのファイ
   ルを編集してください。
[!if HAS_SUFFIX && !HTML_EDITVIEW]
    ドキュメントには次の文字列が含まれます:
        ファイルの拡張子:             [!output FILE_EXTENSION]
        ファイルの種類のID:           [!output FILE_TYPE_ID]
        メイン フレーム キャプション: [!output MAIN_FRAME_CAPTION]
        ドキュメントの種類の名前:     [!output DOC_TYPE_NAME]
        フィルタ名:                   [!output FILTER_NAME]
        ファイルの新しい短い名前:     [!output FILE_NEW_NAME_SHORT]
        ファイルの種類の長い名前:     [!output FILE_NEW_NAME_LONG]
[!endif]
[!else]
アプリケーション ウィザードは 1 つのビューを作成します:
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - ドキュメントのビュー
   これらのファイルは [!output VIEW_CLASS] クラスを含みます。
[!if !DB_VIEW_NO_FILE]
   [!output VIEW_CLASS] オブジェクトは [!output DOC_CLASS] オブジェクトを表示す
   るのに使用されます。
[!endif]

[!if APP_TYPE_MDI]
res\[!output PROJECT_NAME]Doc.ico
   このファイルは、[!output DOC_CLASS] クラスの MDI 子ウィンドウ用アイコンとし
   て使用されるアイコン ファイルです。このアイコンは、メイン リソースファイル 
   [!output PROJECT_NAME].rc に含まれています。
[!endif]
[!endif]
[!if DB_VIEW_NO_FILE || DB_VIEW_WITH_FILE]
/////////////////////////////////////////////////////////////////////////////

データベース サポート:

[!output ROWSET_HEADER], [!output ROWSET_IMPL]
   これらのファイルには [!output ROWSET_CLASS] クラスが含まれます。このクラスは
   ウィザードで選択したデータ ソースにアクセスするのに使用されます。
[!if DB_VIEW_NO_FILE]
   シリアル化サポートは追加されません。
[!else]
   シリアル化サポートが追加されました。
[!endif]
[!endif]
[!if CONTAINER || FULL_SERVER || MINI_SERVER || CONTAINER_SERVER]
/////////////////////////////////////////////////////////////////////////////

アプリケーション ウィザードは OLE 固有のクラスを作成しました。

[!if CONTAINER || CONTAINER_SERVER]
[!output CONTAINER_ITEM_HEADER], [!output CONTAINER_ITEM_IMPL]
   これらのファイルは [!output CONTAINER_ITEM_CLASS] クラスを含みます。このクラ
   スは OLE オブジェクトの操作に使用されます。OLE オブジェクトは 
   [!output VIEW_CLASS] クラスによって表示され、[!output DOC_CLASS] クラスの一
   部としてシリアル化されます。
[!if ACTIVE_DOC_CONTAINER]
   このプログラムは、フレーム内でアクティブ ドキュメントを操作するためのサポー
   トを含みます。
[!endif]
[!endif]
[!if MINI_SERVER || FULL_SERVER || CONTAINER_SERVER]

[!output SERVER_ITEM_HEADER], [!output SERVER_ITEM_IMPL]
   これらのファイルは [!output SERVER_ITEM_CLASS] を含みます。このクラスは 
   [!output DOC_CLASS] クラスを OLE システムに接続し、必要に応じてドキュメント
   にリンクを設定するのに使用されます。

[!if ACTIVE_DOC_SERVER]
   プロジェクトは、アクティブ ドキュメントを作成し管理することをサポートしま
   す。
[!endif]

[!output INPLACE_FRAME_HEADER], [!output INPLACE_FRAME_IMPL]
   これらのファイルは [!output INPLACE_FRAME_CLASS] を含みます。このクラスは 
   COleIPFrameWnd から派生し、埋め込み先で編集されている間すべてのフレームの機
   能を制御します。
[!endif]

[!if SUPPORT_COMPOUND_FILES]
   プロジェクトは複合ファイルをサポートします。複合ファイル フォーマットは、
   1 つのファイルの中に 1 つ以上の自動オブジェクトを持つドキュメントを格納しま
   す。またファイルの中のオブジェクトには個別にアクセスすることができます。
[!endif]
[!endif]
[!else]

/////////////////////////////////////////////////////////////////////////////

[!if AUTOMATION]
アプリケーション ウィザードは 1 つのダイアログ クラスおよびオートメーション 
プロキシ クラスを作成します:
[!else]
アプリケーション ウィザードは 1 つのダイアログ クラスを作成します:
[!endif]

[!output DIALOG_HEADER], [!output DIALOG_IMPL] - ダイアログ
   これらのファイルは [!output DIALOG_CLASS] クラスを含みます。このクラスはアプ
   リケーションのメイン ダイアログの動作を定義します。ダイアログ テンプレートは
   Microsoft Visual C++ で編集可能な [!output PROJECT_NAME].rc に含まれます。
[!if AUTOMATION]

[!output DIALOG_AUTO_PROXY_HEADER], [!output DIALOG_AUTO_PROXY_IMPL] - オート
メーション オブジェクト
   これらのファイルは [!output DIALOG_AUTO_PROXY_CLASS] クラスを含みます。オー
   トメーション コントローラがダイアログにアクセスするために使用可能なオートメ
   ーション メソッドおよびプロパティの公開を行うため、このクラスはダイアログの
   オートメーション プロキシ クラスと呼ばれます。モーダル ダイアログ ベースの
   MFC アプリケーションの場合、オートメーション オブジェクトをユーザー インター
   フェイスから切り離しておく方が簡単なため、これらのメソッドおよびプロパティは
   ダイアログ クラスから直接公開されません。
[!endif]
[!endif]

[!if CONTEXT_HELP]
/////////////////////////////////////////////////////////////////////////////

ヘルプ サポート:

[!if HELPSTYLE_HTML]
hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhp
   このファイルはヘルプ プロジェクト ファイルです。ヘルプ ファイルを .chm ファ
   イルにコンパイルするのに必要なデータを含んでいます。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhc
   このファイルはヘルプ プロジェクトのコンテンツ一覧を表示します。

hlp\[!output SAFE_PROJECT_HELP_FILE_NAME].hhk
   このファイルはヘルプ トピックのインデックスを含みます。

hlp\afxcore.htm
   このファイルは標準の MFC コマンドと画面オブジェクト用の標準ヘルプ トピックを
   含みます。任意のヘルプ トピックをこのファイルに追加してください。

[!if PRINTING]
hlp\afxprint.htm
   このファイルは印刷コマンド用のヘルプ トピックを含みます。

[!endif]
makehtmlhelp.bat
   このファイルはヘルプ ファイルをコンパイルするためにビルド システムで使用しま
   す。

hlp\Images\*.gif
   これらは、Microsoft Foundation Class ライブラリの標準コマンドに関する、標準
   ヘルプ ファイルのトピックに必要なビットマップ ファイルです。
[!else]
hlp\[!output PROJECT_NAME].hpj
   このファイルは、ヘルプ コンパイラがアプリケーションのヘルプ ファイルを作成す
   るためのヘルプ プロジェクト ファイルです。

hlp\*.bmp
   これらは、Microsoft Foundation Class ライブラリの標準コマンドに関する、標準
   ヘルプ ファイルのトピックに必要なビットマップ ファイルです。

hlp\*.rtf
   これらのファイルは標準 MFC コマンドと画面オブジェクトに関する標準ヘルプ トピ
   ックを含んでいます。
[!endif]
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SPLITTER || MAPI || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

その他の機能:
[!if ACTIVEX_CONTROLS]

ActiveX コントロール
   アプリケーションは ActiveX コントロールの使用に関するサポートを含みます。
[!endif]
[!if PRINTING]

印刷と印刷プレビューのサポート
   アプリケーション ウィザードは、 MFC ライブラリから CView クラスのメンバ関数
   を呼び出すことによって、印刷、印刷の設定、および印刷プレビュー コマンドを処
   理するコードを生成しました。
[!endif]
[!if DB_SUPPORT_HEADER_ONLY && !APP_TYPE_DLG]

データベース サポート
   アプリケーション ウィザードは、プログラムに基本レベルのデータベース サポート
   を追加しました。必要なファイルのみ含まれています。
[!endif]
[!if SPLITTER && !APP_TYPE_DLG]

分割ウィンドウ
   アプリケーション ウィザードは、アプリケーション ドキュメントに分割ウィンドウ
   のためのサポートを追加しました。

[!endif]
[!if MAPI]

MAPI サポート
   生成されたプロジェクトにはメール メッセージを作成、操作、送信、および保存す
   るために必要なコードが含まれます。
[!endif]
[!if SOCKETS]

Windows ソケット
   アプリケーションは TCP/IP ネットワーク経由の通信を確立するためのサポートを
   含みます。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

その他の標準ファイル:

StdAfx.h, StdAfx.cpp
   これらのファイルは、既にコンパイルされたヘッダー ファイル (PCH) 
   [!output PROJECT_NAME].pch や既にコンパイルされた型のファイル StdAfx.obj を
   ビルドするために使用されます。

Resource.h
   これは新規リソース ID を定義する標準ヘッダー ファイルです。Microsoft 
   Visual C++ はこのファイルの読み取りと更新を行います。

[!if MANIFEST]
[!output PROJECT_NAME].manifest
   アプリケーション マニフェスト ファイルは Windows XP で、Side-by-Side アセン
   ブリの特定のバージョンに関するアプリケーションの依存関係を説明するために使用
   されます。ローダーはこの情報を使用して、アセンブリ キャッシュから適切なアセ
   ンブリを、またはアプリケーションからプライベート アセンブリを読み込みます。
   アプリケーション マニフェストは再頒布用に、実行可能アプリケーションと同じフ
   ォルダにインストールされる外部 .manifest ファイルとして含まれているか、また
   はリソースのフォーム内の実行可能ファイルに含まれています。
[!endif]
/////////////////////////////////////////////////////////////////////////////

その他の注意:

アプリケーション ウィザードは "TODO:" で始まるコメントを使用して、追加したりカ
スタマイズする必要があるソース コードの部分を示します。
[!if APP_TYPE_MDI || APP_TYPE_SDI || APP_TYPE_DLG || APP_TYPE_MTLD]

アプリケーションが共有 DLL 内で MFC を使用する場合は、MFC DLL を再頒布する必要
があります。また、アプリケーションがオペレーティング システムのロケール以外の言
語を使用している場合も、対応するローカライズされたリソース MFC80XXX.DLL を再頒
布する必要があります。これらのトピックの詳細については、MSDN ドキュメントの 
Visual C++ アプリケーションの再頒布に関するセクションを参照してください。
[!endif]

/////////////////////////////////////////////////////////////////////////////
