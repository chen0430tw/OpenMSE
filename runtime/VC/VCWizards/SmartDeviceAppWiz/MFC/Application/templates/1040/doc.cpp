// [!output DOC_IMPL] : implementazione della classe [!output DOC_CLASS]
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

// costruzione/distruzione di [!output DOC_CLASS]

[!output DOC_CLASS]::[!output DOC_CLASS]()
{
	// TODO: aggiungere qui il codice di costruzione unico

}

[!output DOC_CLASS]::~[!output DOC_CLASS]()
{
}

BOOL [!output DOC_CLASS]::OnNewDocument()
{
	if (![!output DOC_BASE_CLASS]::OnNewDocument())
		return FALSE;

	// TODO: aggiungere qui il codice di reinizializzazione
	// (nei documenti SDI verrà riutilizzato questo documento).

	return TRUE;
}

// serializzazione di [!output DOC_CLASS]

#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
void [!output DOC_CLASS]::Serialize(CArchive& ar)
{
	(ar);
}
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT


// diagnostica di [!output DOC_CLASS]

#ifdef _DEBUG
void [!output DOC_CLASS]::AssertValid() const
{
	[!output DOC_BASE_CLASS]::AssertValid();
}
#endif //_DEBUG


// comandi di [!output DOC_CLASS]

