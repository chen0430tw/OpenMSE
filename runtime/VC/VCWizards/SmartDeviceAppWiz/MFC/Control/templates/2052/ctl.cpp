// [!output CONTROL_IMPL] : [!output CONTROL_CLASS] ActiveX 控件类的实现。

#include "stdafx.h"
#include "[!output PROJECT_NAME].h"
#include "[!output CONTROL_HEADER]"
#include "[!output PROPERTY_PAGE_HEADER]"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNCREATE([!output CONTROL_CLASS], COleControl)


// 消息映射

BEGIN_MESSAGE_MAP([!output CONTROL_CLASS], COleControl)
[!if SUBCLASS_WINDOW]
	ON_MESSAGE(OCM_COMMAND, &[!output CONTROL_CLASS]::OnOcmCommand)
[!endif]
[!if INSERTABLE]
	ON_OLEVERB(AFX_IDS_VERB_EDIT, OnEdit)
[!endif]
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP([!output CONTROL_CLASS], COleControl)
[!if ASYNC_PROPERTY_LOAD]
	DISP_STOCKPROP_READYSTATE()
[!endif]
[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
[!if ABOUT_BOX]
	DISP_FUNCTION_ID([!output CONTROL_CLASS], "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
[!endif]
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
[!if ABOUT_BOX]
	DISP_FUNCTION_ID([!output CONTROL_CLASS], "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP([!output CONTROL_CLASS], COleControl)
[!if ASYNC_PROPERTY_LOAD]
	EVENT_STOCK_READYSTATECHANGE()
[!endif]
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS([!output CONTROL_CLASS], 1)
	PROPPAGEID([!output PROPERTY_PAGE_CLASS]::guid)
END_PROPPAGEIDS([!output CONTROL_CLASS])



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX([!output CONTROL_CLASS], "[!output CONTROL_TYPE_ID]",
	[!output CONTROL_CLSID_IMPLEMENT_OLECREATE_FORMAT])



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB([!output CONTROL_CLASS], _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID BASED_CODE IID_D[!output SAFE_PROJECT_IDENTIFIER_NAME] =
		[!output PRIMARY_IID_STATIC_CONST_GUID_FORMAT];
const IID BASED_CODE IID_D[!output SAFE_PROJECT_IDENTIFIER_NAME]Events =
		[!output EVENT_IID_STATIC_CONST_GUID_FORMAT];



// 控件类型信息

static const DWORD BASED_CODE _dw[!output SAFE_PROJECT_IDENTIFIER_NAME]OleMisc =
[!if SIMPLE_FRAME]
	OLEMISC_SIMPLEFRAME |
[!endif]
[!if INVISIBLE_AT_RUNTIME]
	OLEMISC_INVISIBLEATRUNTIME |
[!endif]
[!if ACTIVATE_WHEN_VISIBLE]
	OLEMISC_ACTIVATEWHENVISIBLE |
[!if MOUSE_NOTIFICATIONS]
	OLEMISC_IGNOREACTIVATEWHENVISIBLE |
[!endif]
[!endif]
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE([!output CONTROL_CLASS], IDS_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME], _dw[!output SAFE_PROJECT_IDENTIFIER_NAME]OleMisc)

BEGIN_INTERFACE_MAP([!output CONTROL_CLASS], COleControl)
	INTERFACE_PART([!output CONTROL_CLASS], IID_IObjectSafety, ObjectSafety)
END_INTERFACE_MAP()

// [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::UpdateRegistry -
// 添加或移除 [!output CONTROL_CLASS] 的系统注册表项

BOOL [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::UpdateRegistry(BOOL bRegister)
{
#ifndef _CE_DCOM
	int nRegFlags = afxRegFreeThreading;
#else
	int nRegFlags = afxRegApartmentThreading;
#endif // !_CE_DCOM

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME],
			IDB_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME],
[!if INSERTABLE]
			afxRegInsertable | nRegFlags,
[!else]
			nRegFlags,
[!endif]
			_dw[!output SAFE_PROJECT_IDENTIFIER_NAME]OleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}

[!if RUNTIME_LICENSE]


// 授权字符串

static const TCHAR BASED_CODE _szLicFileName[] = _T("[!output PROJECT_NAME].lic");

static const WCHAR BASED_CODE _szLicString[] =
	L"版权所有 (c) [!output YEAR] [!output COMPANY_NAME]";



// [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::VerifyUserLicense -
// 检查是否存在用户许可证

BOOL [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::VerifyUserLicense()
{
	return AfxVerifyLicFile(AfxGetInstanceHandle(), _szLicFileName,
		_szLicString);
}



// [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::GetLicenseKey -
// 返回运行时授权密钥

BOOL [!output CONTROL_CLASS]::[!output CONTROL_CLASS]Factory::GetLicenseKey(DWORD dwReserved,
	BSTR FAR* pbstrKey)
{
	if (pbstrKey == NULL)
		return FALSE;

	*pbstrKey = SysAllocString(_szLicString);
	return (*pbstrKey != NULL);
}

[!endif]


// [!output CONTROL_CLASS]::[!output CONTROL_CLASS] - 构造函数

[!output CONTROL_CLASS]::[!output CONTROL_CLASS]()
{
	// TODO: 需要时为 m_dwCurrentSafety 和 m_dwSupportedSafety 属性设置适当的值。
	// 可以是 INTERFACESAFE_FOR_UNTRUSTED_DATA 或 INTERFACESAFE_FOR_UNTRUSTED_CALLER 或这两者，
	// 具体取决于实际方案。
	m_dwCurrentSafety = 0;
	m_dwSupportedSafety = 0;
	InitializeIIDs(&IID_D[!output SAFE_PROJECT_IDENTIFIER_NAME], &IID_D[!output SAFE_PROJECT_IDENTIFIER_NAME]Events);
[!if SIMPLE_FRAME]

	EnableSimpleFrame();
[!endif]
[!if ASYNC_PROPERTY_LOAD]

	m_lReadyState = READYSTATE_LOADING;
	// TODO: 当 readystate 更改时调用 InternalSetReadyState。
[!endif]
	// TODO: 在此初始化控件的实例数据。
}



// [!output CONTROL_CLASS]::~[!output CONTROL_CLASS] - 析构函数

[!output CONTROL_CLASS]::~[!output CONTROL_CLASS]()
{
	// TODO: 在此清理控件的实例数据。
}



// [!output CONTROL_CLASS]::OnDraw - 绘图函数

void [!output CONTROL_CLASS]::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

[!if SUBCLASS_WINDOW]
	DoSuperclassPaint(pdc, rcBounds);
[!else]
	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
[!endif]
[!if OPTIMIZED_DRAW]

	if (!IsOptimizedDraw())
	{
		// 容器不支持优化绘图。

		// TODO: 如果将任何 GDI 对象选入到设备上下文 *pdc 中，
		//		请在此处恢复先前选定的对象。
	}
[!endif]
}



// [!output CONTROL_CLASS]::DoPropExchange - 持久性支持

void [!output CONTROL_CLASS]::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}


[!if WINDOWLESS || UNCLIPPED_DEVICE_CONTEXT || FLICKER_FREE || MOUSE_NOTIFICATIONS || OPTIMIZED_DRAW]

// [!output CONTROL_CLASS]::GetControlFlags -
// 自定义 MFC 的 ActiveX 控件实现的标志。
//
DWORD [!output CONTROL_CLASS]::GetControlFlags()
{
	DWORD dwFlags = COleControl::GetControlFlags();

[!if UNCLIPPED_DEVICE_CONTEXT]
	// 当前未剪辑控件的输出。
	// 控件保证它不会绘制到它的
	// 矩形工作区之外。
	dwFlags &= ~clipPaintDC;
[!endif]
[!if WINDOWLESS]

	// 不用创建窗口即可激活控件。
	// TODO: 编写控件的消息处理程序时，在使用
	//		m_hWnd 成员变量之前应首先检查它的值是否
	//		非空。
	dwFlags |= windowlessActivate;
[!endif]
[!if FLICKER_FREE]

	// 在活动和不活动状态之间进行转换时，
	// 不会重新绘制控件。
	dwFlags |= noFlickerActivate;
[!endif]
[!if MOUSE_NOTIFICATIONS]

	// 控件在不活动时可以接收鼠标通知。
	// TODO: 如果编写 WM_SETCURSOR 和 WM_MOUSEMOVE 的处理程序，
	//		在使用 m_hWnd 成员变量之前应首先检查它的值
	//		是否非空。
	dwFlags |= pointerInactive;
[!endif]
[!if OPTIMIZED_DRAW]

	// 控件通过不还原设备上下文中的
	// 原始 GDI 对象，可以优化它的 OnDraw 方法。
	dwFlags |= canOptimizeDraw;
[!endif]
	return dwFlags;
}


[!endif]

// [!output CONTROL_CLASS]::OnResetState - 将控件重置为默认状态

void [!output CONTROL_CLASS]::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}

[!if !MULTIPLE_PLATFORM]
[!if !SMARTPHONE2003_UI_MODEL]
[!if ABOUT_BOX]

// [!output CONTROL_CLASS]::AboutBox - 向用户显示“关于”框

void [!output CONTROL_CLASS]::AboutBox()
{
#ifdef _DEVICE_RESOLUTION_AWARE
	CDialog dlgAbout(IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME], IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_WIDE);
#else
	CDialog dlgAbout(IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]);
#endif
	dlgAbout.DoModal();
}

[!endif]
[!endif]
[!else]
[!if SMARTPHONE2003_UI_MODEL]
#ifndef WIN32_PLATFORM_WFSP
[!endif]
[!if ABOUT_BOX]

// [!output CONTROL_CLASS]::AboutBox - 向用户显示“关于”框

void [!output CONTROL_CLASS]::AboutBox()
{
#ifdef _DEVICE_RESOLUTION_AWARE
	CDialog dlgAbout(IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME], IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]_WIDE);
#else
	CDialog dlgAbout(IDD_ABOUTBOX_[!output UPPER_CASE_SAFE_PROJECT_IDENTIFIER_NAME]);
#endif
	dlgAbout.DoModal();
}

[!endif]
[!if SMARTPHONE2003_UI_MODEL]
#endif // !WIN32_PLATFORM_WFSP
[!endif]
[!endif]

[!if SUBCLASS_WINDOW]


// [!output CONTROL_CLASS]::PreCreateWindow - 修改 CreateWindowEx 的参数

BOOL [!output CONTROL_CLASS]::PreCreateWindow(CREATESTRUCT& cs)
{
[!if SUBCLASS_WINDOW]
	cs.lpszClass = _T("[!output WINDOW_CLASS]");
[!else]
	// TODO: 填充要为其创建子类的窗口类的名称。
	cs.lpszClass = _T("");
[!endif]
	return COleControl::PreCreateWindow(cs);
}



// [!output CONTROL_CLASS]::IsSubclassedControl - 这是一个子类控件

BOOL [!output CONTROL_CLASS]::IsSubclassedControl()
{
	return TRUE;
}



// [!output CONTROL_CLASS]::OnOcmCommand - 处理命令消息

LRESULT [!output CONTROL_CLASS]::OnOcmCommand(WPARAM wParam, LPARAM lParam)
{
#ifdef _WIN32
	WORD wNotifyCode = HIWORD(wParam);
#else
	WORD wNotifyCode = HIWORD(lParam);
#endif

	// TODO: 在此接通 wNotifyCode。

	return 0;
}

[!endif]


// [!output CONTROL_CLASS] 消息处理程序

// [!output CONTROL_CLASS]::XObjectSafety
STDMETHODIMP_(ULONG) [!output CONTROL_CLASS]::XObjectSafety::AddRef()
{
	METHOD_PROLOGUE_EX_([!output CONTROL_CLASS], ObjectSafety)
	return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) [!output CONTROL_CLASS]::XObjectSafety::Release()
{
	METHOD_PROLOGUE_EX_([!output CONTROL_CLASS], ObjectSafety)
	return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP [!output CONTROL_CLASS]::XObjectSafety::QueryInterface(
	REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE_EX_([!output CONTROL_CLASS], ObjectSafety)
	return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

STDMETHODIMP [!output CONTROL_CLASS]::XObjectSafety::GetInterfaceSafetyOptions(REFIID riid, DWORD *pdwSupportedOptions, DWORD *pdwEnabledOptions)
{
	METHOD_PROLOGUE_EX_([!output CONTROL_CLASS], ObjectSafety)

	if (pdwSupportedOptions == NULL || pdwEnabledOptions == NULL)
		return E_POINTER;

	// 检查我们是否支持此接口
	IUnknown* pUnk;
	HRESULT hr = this->QueryInterface(riid, (void**)&pUnk);
	if (SUCCEEDED(hr))
	{
		// 我们支持此接口，因此可相应地设置安全选项
		*pdwSupportedOptions = pThis->m_dwSupportedSafety;
		*pdwEnabledOptions   = pThis->m_dwCurrentSafety;
	}
	else
	{
		// 我们不支持此接口
		*pdwSupportedOptions = 0;
		*pdwEnabledOptions   = 0;
	}
	return hr;
}

STDMETHODIMP [!output CONTROL_CLASS]::XObjectSafety::SetInterfaceSafetyOptions(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions)
{
	// 如果已修改，则返回 S_OK，否则返回 S_FALSE。
	METHOD_PROLOGUE_EX_([!output CONTROL_CLASS], ObjectSafety)

	// 检查我们是否支持该接口，如果不支持则返回 E_NOINTEFACE
	IUnknown* pUnk;
	if (FAILED(this->QueryInterface(riid, (void**)&pUnk)))
		return E_NOINTERFACE;

	// 如果要求我们设置不支持的选项，则会失败
	if (dwOptionSetMask & ~pThis->m_dwSupportedSafety)
		return E_FAIL;

	// 设置所要求的安全选项
	pThis->m_dwCurrentSafety = (pThis->m_dwCurrentSafety  & ~dwOptionSetMask)  |  (dwOptionSetMask & dwEnabledOptions);		
	return S_OK;
}
