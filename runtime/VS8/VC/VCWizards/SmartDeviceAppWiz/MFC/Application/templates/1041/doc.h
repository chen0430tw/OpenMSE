// [!output DOC_HEADER] : [!output DOC_CLASS] クラスのインターフェイス
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // シリアル化からのみ作成します。
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// 属性
public:

// 操作
public:

// オーバーライド
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// 実装
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
};


