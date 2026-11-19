#pragma once

// [!output CONTROL_HEADER] : [!output CONTROL_CLASS] ActiveX 控件类的声明。
#include <objsafe.h>

// [!output CONTROL_CLASS] : 有关实现的信息，请参阅 [!output CONTROL_IMPL]。

class [!output CONTROL_CLASS] : public COleControl
{
	DECLARE_DYNCREATE([!output CONTROL_CLASS])

// 构造函数
public:
	[!output CONTROL_CLASS]();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
[!if SUBCLASS_WINDOW]
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
[!endif]
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
[!if WINDOWLESS || UNCLIPPED_DEVICE_CONTEXT || FLICKER_FREE || MOUSE_NOTIFICATIONS || OPTIMIZED_DRAW]
	virtual DWORD GetControlFlags();
[!endif]

// 数据成员
protected:
	DWORD m_dwCurrentSafety;
	DWORD m_dwSupportedSafety;

// 实现
protected:
	~[!output CONTROL_CLASS]();

[!if RUNTIME_LICENSE]
	BEGIN_OLEFACTORY([!output CONTROL_CLASS])        // 类工厂和 guid
		virtual BOOL VerifyUserLicense();
		virtual BOOL GetLicenseKey(DWORD, BSTR FAR*);
	END_OLEFACTORY([!output CONTROL_CLASS])

[!else]
	DECLARE_OLECREATE_EX([!output CONTROL_CLASS])    // 类工厂和 guid
[!endif]
	DECLARE_OLETYPELIB([!output CONTROL_CLASS])      // GetTypeInfo
	DECLARE_PROPPAGEIDS([!output CONTROL_CLASS])     // 属性页 ID
	DECLARE_OLECTLTYPE([!output CONTROL_CLASS])		// 类型名称和杂项状态

[!if SUBCLASS_WINDOW]
	// 子类控件支持
	BOOL IsSubclassedControl();
	LRESULT OnOcmCommand(WPARAM wParam, LPARAM lParam);

[!endif]
// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
[!if ABOUT_BOX]

	afx_msg void AboutBox();
[!endif]
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP

[!endif]
[!if ABOUT_BOX]

	afx_msg void AboutBox();
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

// 事件映射
	DECLARE_EVENT_MAP()

// 接口
public:

	// IObjectSafety
	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
		INIT_INTERFACE_PART([!output CONTROL_CLASS], ObjectSafety)
		STDMETHOD(GetInterfaceSafetyOptions)(REFIID, DWORD *, DWORD *);
		STDMETHOD(SetInterfaceSafetyOptions)(REFIID, DWORD, DWORD);
	END_INTERFACE_PART_STATIC(ObjectSafety)

	DECLARE_INTERFACE_MAP()

// 调度和事件 ID
public:

	enum {
	};
};

