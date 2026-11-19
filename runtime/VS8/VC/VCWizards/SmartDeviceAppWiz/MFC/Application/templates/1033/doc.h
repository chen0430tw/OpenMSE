// [!output DOC_HEADER] : interface of the [!output DOC_CLASS] class
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // create from serialization only
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// Implementation
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


