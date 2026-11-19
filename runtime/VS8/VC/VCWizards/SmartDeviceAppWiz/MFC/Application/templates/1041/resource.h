//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by [!output PROJECT_NAME].rc
//
[!if LIST_VIEW]
#define ID_VIEW_ARRANGE				127
[!endif] 
#define IDR_MAINFRAME			128
#define IDR_[!output SAFE_DOC_TYPE_NAME]TYPE			129
#define IDD_ABOUTBOX			100
[!if FORM_VIEW]
#define IDD_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_FORM			101
[!endif]
[!if SOCKETS]
#define IDP_SOCKETS_INIT_FAILED		104
[!endif]
[!if APP_TYPE_SDI || APP_TYPE_SDI_DOCLIST]
#define IDS_NEW				65000
#define IDS_FILE			65001
#define IDS_HELP			65002
#define IDS_SAVE			65003
#define IDS_CUT				65004
#define IDS_COPY			65005
#define IDS_PASTE			65006
[!endif]
[!if APP_TYPE_SDI]
#define IDS_ABOUT			65007
[!endif]

[!if POCKETPC2003_UI_MODEL]
#define IDS_EDIT			65010
#define IDS_TOOL			65011
#define IDM_MENU			65012
#define IDR_MAIN_EDIT			65013
#define IDR_MAIN_TOOL			65014
[!endif]

[!if SMARTPHONE2003_UI_MODEL]
#define IDS_OK	102
[!endif]

// 新しいオブジェクトの次の既定値
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE	130
#define _APS_NEXT_CONTROL_VALUE	1000
#define _APS_NEXT_SYMED_VALUE		101
#define _APS_NEXT_COMMAND_VALUE	32771
#endif
#endif
