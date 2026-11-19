// [!output DOC_HEADER] : Schnittstelle der Klasse [!output DOC_CLASS]
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // Nur aus Serialisierung erstellen
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// Attribute
public:

// Vorgänge
public:

// Überschreibungen
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// Implementierung
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generierte Funktionen für die Meldungstabellen
protected:
	DECLARE_MESSAGE_MAP()
};


