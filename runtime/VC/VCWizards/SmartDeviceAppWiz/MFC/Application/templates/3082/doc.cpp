// [!output DOC_IMPL]: implementación de la clase [!output DOC_CLASS]
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

// Construcción o destrucción de [!output DOC_CLASS]

[!output DOC_CLASS]::[!output DOC_CLASS]()
{
	// TODO: agregar aquí el código de construcción único

}

[!output DOC_CLASS]::~[!output DOC_CLASS]()
{
}

BOOL [!output DOC_CLASS]::OnNewDocument()
{
	if (![!output DOC_BASE_CLASS]::OnNewDocument())
		return FALSE;

	// TODO: agregar aquí código de reinicio
	// (los documentos SDI volverán a utilizar este documento)

	return TRUE;
}

// Serialización de [!output DOC_CLASS]

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void [!output DOC_CLASS]::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// Diagnósticos de [!output DOC_CLASS]

#ifdef _DEBUG
void [!output DOC_CLASS]::AssertValid() const
{
	[!output DOC_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG


// Comandos de [!output DOC_CLASS]

