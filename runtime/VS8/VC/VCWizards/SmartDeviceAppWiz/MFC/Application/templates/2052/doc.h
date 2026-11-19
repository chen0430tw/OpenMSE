// [!output DOC_HEADER] : [!output DOC_CLASS] 类的接口
//


#pragma once

class [!output DOC_CLASS] : public [!output DOC_BASE_CLASS]
{
protected: // 仅从序列化创建
	[!output DOC_CLASS]();
	DECLARE_DYNCREATE([!output DOC_CLASS])

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL OnNewDocument();
#ifndef _WIN32_WCE_NO_ARCHIVE_SUPPORT
	virtual void Serialize(CArchive& ar);
#endif // !_WIN32_WCE_NO_ARCHIVE_SUPPORT

// 实现
public:
	virtual ~[!output DOC_CLASS]();
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
};


