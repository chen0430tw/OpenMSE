================================================================================MICROSOFT FOUNDATION CLASS ライブラリ: [!output PROJECT_NAME] プロジェクトの概要
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
   これはアプリケーションのメイン ヘッダー ファイルです。このファイルには、その
   他のプロジェクト固有のヘッダーが含まれていて、[!output APP_CLASS] アプリケー
   ション クラスを宣言します。

[!output APP_IMPL]
   これは、アプリケーション クラス [!output APP_CLASS] を含むメイン アプリケー
   ション ソース ファイルです。

[!if STANDARDSHELL_UI_MODEL]
[!output PROJECT_NAME].rc
   これは、プロジェクトが使用する Microsoft Windows のリソースを一覧表示する、
   プロジェクトのメイン リソース ファイルです。このファイルには RES サブディレ
   クトリに保存されているアイコン、ビットマップ、カーソルが含まれています。この
   ファイルは、Microsoft Visual C++ で直接編集することができます。プロジェクト 
   リソースは [!output LCID] にあります。.rc ファイル が持続されるとき、データ 
   セクション内の定義は定義の表示名ではなく、定義された 16 進数の数値として持
   続されます。

res\[!output RC2_FILE_NAME].rc2
   このファイルは Microsoft Visual C++ を使用しないで編集されたリソースを含んで
   います。リソース エディタで編集できないリソースはすべてこのファイルに入れて
   ください。
[!endif]

[!if POCKETPC2003_UI_MODEL]
[!output PROJECT_NAME]ppc.rc
   これはプロジェクトのメイン リソース ファイルで、プロジェクトが Pocket PC プ
   ラットフォーム用、または同じユーザー インターフェイス モデルをサポートするプ
   ラットフォーム用にコンパイルするときに使用する Microsoft Windows のリソース
   を一覧表示します。このファイルには RES サブディレクトリに保存されているアイ
   コン、ビットマップ、カーソルが含まれています。このファイルは Microsoft 
   Visual C++ で直接編集することができます。プロジェクト リソースは 
   [!output LCID] にあります。.rc ファイルが持続されるとき、データ セクション内
   の定義は定義の表示名ではなく、定義された 16 進数の数値として持続されます。
res\[!output RC2_FILE_NAME]ppc.rc2
   このファイルは Microsoft Visual C++ を使用しないで編集されたリソースを含んで
   います。リソース エディタで編集できないリソースはすべてこのファイルに入れて
   ください。
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
[!output PROJECT_NAME]sp.rc
   これはプロジェクトのメイン リソース ファイルで、プロジェクトが Smartphone プ
   ラットフォーム用、または同じユーザー インターフェイス モデルをサポートするプ
   ラットフォーム用にコンパイルするときに使用する Microsoft Windows のリソース
   を一覧表示します。このファイルには RES サブディレクトリに保存されているアイ
   コン、ビットマップ、カーソルが含まれています。このファイルは Microsoft 
   Visual C++ で直接編集することができます。プロジェクト リソースは 
   [!output LCID] にあります。.rc ファイルが持続されるとき、データ セクション内
   の定義は定義の表示名ではなく、定義された 16 進数の数値として持続されます。
res\[!output RC2_FILE_NAME]sp.rc2
   このファイルは Microsoft Visual C++ を使用しないで編集されたリソースを含んで
   います。リソース エディタで編集できないリソースはすべてこのファイルに入れて
   ください。
[!endif]
[!if AYGSHELL_UI_MODEL]
[!output PROJECT_NAME]ayg.rc
   これはプロジェクトのメイン リソース ファイルで、プロジェクトが AYGShell ユー
   ザー インターフェイス モデルをサポートするプラットフォーム用にコンパイルする
   ときに使用する Microsoft Windows のリソースを一覧表示します。このファイルに
   は RES サブディレクトリに保存されているアイコン、ビットマップ、カーソルが含
   まれています。このファイルは Microsoft Visual C++ で直接編集することができま
   す。プロジェクト リソースは [!output LCID] にあります。.rc ファイル が持続さ
   れるとき、データ セクション内の定義は定義の表示名ではなく、定義された 16 進
   数の数値として持続されます。
res\[!output RC2_FILE_NAME]ayg.rc2
   このファイルは Microsoft Visual C++ を使用しないで編集されたリソースを含んで
   います。リソース エディタで編集できないリソースはすべてこのファイルに入れて
   ください。
[!endif]

res\[!output PROJECT_NAME].ico
   これは、アプリケーションのアイコンとして使用されるアイコン ファイルです。
   このアイコンはメイン リソース ファイルに含まれています。

[!if HAS_SUFFIX]
[!output PROJECT_NAME].reg
   このレジストリ ファイルは、フレームワークの登録設定方法を紹介するためのサン
   プルファイルです。
[!if APP_TYPE_DLG]
   アプリケーションと一緒に .reg ファイルとして使用します。
[!else]
   アプリケーションと一緒に .reg ファイルとして使用するか、または削除してデフ
   ォルトの登録 RegisterShellFileTypes を使用します。
[!endif]
[!endif]

[!if !APP_TYPE_DLG]
/////////////////////////////////////////////////////////////////////////////

メイン フレーム ウィンドウ:
   プロジェクトは標準の MFC インターフェイスを含みます。

[!output MAIN_FRAME_HEADER], [!output MAIN_FRAME_IMPL]
   これらのファイルは、CFrameWnd から派生しすべての SDI フレーム機能を制御する
   フレーム クラス [!output MAIN_FRAME_CLASS] を含みます。
    
[!if CMD_BAR_MENUNBUTTON || CMD_BAR_BROWSERSTYLE]
res\Cmdbar.bmp
   このビットマップ ファイルはコマンド バーのタイル イメージとして使用されま
   す。初期コマンド バーは [!output MAIN_FRAME_CLASS] クラスで作成されます。
   コマンド バーのボタンを追加するには、この cmdbar ビットマップと同様に 
   [!output MAIN_FRAME_IMPL] ファイル内の配列を編集してください。
[!endif]

/////////////////////////////////////////////////////////////////////////////

[!if DOCVIEW]
アプリケーション ウィザードは 1 つのドキュメントの種類と 1 つのビューを作成し
ます:

[!output DOC_HEADER], [!output DOC_IMPL] - ドキュメント
   これらのファイルは [!output DOC_CLASS] クラスを含みます。特別なドキュメント 
   データを付け加えたり、ファイルを保存したり、
   (via [!output DOC_CLASS]::Serialize) を読み込んだりするには、これらのファイ
   ルを編集してください。
[!if HAS_SUFFIX]
   ドキュメントには次の文字列が含まれます:
        ファイルの拡張子:               [!output FILE_EXTENSION]
        ファイルの種類のID:             [!output FILE_TYPE_ID]
        メイン フレーム キャプション:   [!output MAIN_FRAME_CAPTION]
        ドキュメントの種類の名前:       [!output DOC_TYPE_NAME]
        フィルタ名:                     [!output FILTER_NAME]
        ファイルの新しい短い名前:       [!output FILE_NEW_NAME_SHORT]
        ファイルの種類の長い名前:       [!output FILE_NEW_NAME_LONG]
[!endif]

[!output VIEW_HEADER], [!output VIEW_IMPL] - ドキュメントのビュー
   これらのファイルは [!output VIEW_CLASS] クラスを含みます。
   [!output VIEW_CLASS] オブジェクトは [!output DOC_CLASS] オブジェクトを表示
   するのに使用されます。

[!endif]

[!else]
/////////////////////////////////////////////////////////////////////////////

アプリケーション ウィザードは 1 つのダイアログ クラスを作成します:

[!output DIALOG_HEADER], [!output DIALOG_IMPL] - ダイアログ
   これらのファイルは [!output DIALOG_CLASS] クラスを含みます。このクラスはアプ
   リケーションのメイン ダイアログの動作を定義します。ダイアログ テンプレートは
   Microsoft Visual C++ で編集可能なメイン リソース ファイルに含まれます。
[!endif]

[!if HELPSTYLE_WINHELP]
/////////////////////////////////////////////////////////////////////////////

ヘルプ サポート:

hlp\[!output PROJECT_NAME].hpj
   このファイルはアプリケーションのヘルプ ファイルを作成するためにヘルプ コンパ
   イラが使用するヘルプ プロジェクト ファイルです。

hlp\*.bmp
   これらは、Microsoft Foundation Class ライブラリの標準コマンドに関する標準ヘ
   ルプ ファイルのトピックに必要なビットマップ ファイルです。

hlp\*.rtf
   これらのファイルは標準 MFC コマンドと画面オブジェクトに関する標準ヘルプ トピ
   ックを含んでいます。
[!endif]

[!if ACTIVEX_CONTROLS || PRINTING || SOCKETS]
/////////////////////////////////////////////////////////////////////////////

その他の機能:
[!if APP_TYPE_DLG]
[!else]
[!if CMD_BAR_MENUONLY]
   メニューおよび表示要素を含むメイン フレームのコマンド バーです
[!else]
[!if CMD_BAR_MENUNBUTTON]
   メニュー、ボタン、および表示要素を含むメイン フレームのメニュー バーです
[!endif]
[!endif]
[!if STATUS_BAR]
   メイン フレームの初期ステータス バーです
[!endif]
[!endif]
[!if HELPSTYLE_WINHELP]
   Windows CE のヘルプ サポートです
[!endif]
[!if DYNAMIC_MFC]
   共有 DLL の実装を使用します
[!endif]
[!if ACTIVEX_CONTROLS]
ActiveX コントロール
   アプリケーションは ActiveX コントロールを使用に関するサポートを含みます。
[!endif]
[!if PRINTING]
印刷と印刷プレビューのサポート
    アプリケーション ウィザードは、MFC ライブラリから CView クラスのメンバ関数
   を呼び出すことによって、印刷、印刷の設定、および印刷プレビュー コマンドを処
   理するコードを生成しました。
[!endif]
[!if SOCKETS]
Windows ソケット 
   アプリケーションは TCP/IP ネットワーク経由の通信を確立するためのサポートを含
   みます。
[!endif]

[!endif]
/////////////////////////////////////////////////////////////////////////////

その他の標準ファイル:

StdAfx.h, StdAfx.cpp
   これらのファイルは既にコンパイルされたヘッダー ファイル (PCH) 
   [!output PROJECT_NAME].pch や既にコンパイルされた型のファイル StdAfx.obj を
   ビルドするために使われるファイルです。

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
   このファイルは新規リソース ID を定義する標準ヘッダー ファイルです。Microsoft
   Visual C++ はこのファイルの読み取りと更新を行います。

/////////////////////////////////////////////////////////////////////////////

その他の注意:

アプリケーション ウィザードは "TODO:" で始まるコメントを使用して、追加またはカ
スタマイズする必要があるソース コードの部分を示します。
[!if APP_TYPE_SDI || APP_TYPE_DLG]

アプリケーションが共有 DLL 内で MFC を使用する場合や、アプリケーションがオペ
レーティング システムで現在使用している言語以外の言語を使用している場合は、対応
するローカライズされたリソース MFC80XXX.DLL をアプリケーション ディレクトリにコ
ピーする必要があります。("XXX" は言語の省略形を示します。例えば、MFC80JPN.DLL 
は日本語に翻訳されたリソースを含みます。) この処理を行わない場合、アプリケーシ
ョン内のいくつかの UI 要素はオペレーティング システムの言語のまま残ります。
[!endif]

/////////////////////////////////////////////////////////////////////////////
