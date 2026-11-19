// [!output DOC_HEADER] : interfaccia della classe [!output DOC_CLASS]
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // creare solo da serializzazione.
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// Attributi
public:

// Operazioni
public:

// Override
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// Implementazione
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Funzioni generate per la mappa dei messaggi
protected:
	DECLARE_MESSAGE_MAP()
};


