// [!output WND_VIEW_HEADER] : [!output WND_VIEW_CLASS] 클래스의 인터페이스
//


#pragma once


// [!output WND_VIEW_CLASS] 창

class [!output WND_VIEW_CLASS] : public [!output WND_VIEW_BASE_CLASS]
{
// 생성입니다.
public:
	[!output WND_VIEW_CLASS]();

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~[!output WND_VIEW_CLASS]();

	// 생성된 메시지 맵 함수
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
};

