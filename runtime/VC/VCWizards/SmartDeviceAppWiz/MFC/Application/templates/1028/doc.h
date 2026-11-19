// [!output DOC_HEADER] : [!output DOC_CLASS] 類別的介面
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // 僅從序列化建立
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// 屬性
public:

// 作業
public:

// 覆寫
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// 程式碼實作
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// 產生的訊息對應函式
protected:
	DECLARE_MESSAGE_MAP()
};


