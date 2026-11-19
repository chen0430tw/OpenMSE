// [!output DOC_IMPL] : [!output DOC_CLASS] 類別的實作
//

#include "stdafx.h"
#include "[!output APP_HEADER]"

#include "[!output DOC_HEADER]"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// [!output DOC_CLASS]

IMPLEMENT_DYNCREATE([!output DOC_CLASS], [!output DOC_BASE_CLASS])

BEGIN_MESSAGE_MAP([!output DOC_CLASS], [!output DOC_BASE_CLASS])
END_MESSAGE_MAP()

// [!output DOC_CLASS] 建構/解構

[!output DOC_CLASS]::[!output DOC_CLASS]()
{
	// TODO: 在此加入一次建構程式碼

}

[!output DOC_CLASS]::~[!output DOC_CLASS]()
{
}

BOOL [!output DOC_CLASS]::OnNewDocument()
{
	if (![!output DOC_BASE_CLASS]::OnNewDocument())
		return FALSE;

	// TODO: 在此加入重新初始化程式碼
	// (SDI 文件會重用此文件)

	return TRUE;
}

// [!output DOC_CLASS] 序列化

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void [!output DOC_CLASS]::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// [!output DOC_CLASS] 診斷

#ifdef _DEBUG
void [!output DOC_CLASS]::AssertValid() const
{
	[!output DOC_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG


// [!output DOC_CLASS] 命令

