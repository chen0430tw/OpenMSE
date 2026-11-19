// [!output DOC_IMPL] : [!output DOC_CLASS] 클래스의 구현
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

// [!output DOC_CLASS] 생성/소멸

[!output DOC_CLASS]::[!output DOC_CLASS]()
{
	// TODO: 여기에 일회성 생성 코드를 추가합니다.

}

[!output DOC_CLASS]::~[!output DOC_CLASS]()
{
}

BOOL [!output DOC_CLASS]::OnNewDocument()
{
	if (![!output DOC_BASE_CLASS]::OnNewDocument())
		return FALSE;

	// TODO: 여기에 재초기화 코드를 추가합니다.
	// SDI 문서는 이 문서를 다시 사용합니다.

	return TRUE;
}

// [!output DOC_CLASS] serialization

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void [!output DOC_CLASS]::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// [!output DOC_CLASS] 진단

#ifdef _DEBUG
void [!output DOC_CLASS]::AssertValid() const
{
	[!output DOC_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG


// [!output DOC_CLASS] 명령

