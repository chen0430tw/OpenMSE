// [!output DOC_IMPL] : [!output DOC_CLASS] 类的实现
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

// [!output DOC_CLASS] 构造/析构

[!output DOC_CLASS]::[!output DOC_CLASS]()
{
	// TODO: 在此添加一次性构造代码

}

[!output DOC_CLASS]::~[!output DOC_CLASS]()
{
}

BOOL [!output DOC_CLASS]::OnNewDocument()
{
	if (![!output DOC_BASE_CLASS]::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}

// [!output DOC_CLASS] 序列化

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void [!output DOC_CLASS]::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// [!output DOC_CLASS] 诊断

#ifdef _DEBUG
void [!output DOC_CLASS]::AssertValid() const
{
	[!output DOC_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG


// [!output DOC_CLASS] 命令

