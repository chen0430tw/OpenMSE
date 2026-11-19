// [!output HEADER_FILE] : [!output CLASS_NAME] 的声明

#pragma once
[!if !DEVICE]
#include "resource.h"       // 主符号
[!else]
#ifdef STANDARDSHELL_UI_MODEL
#include "resource.h"
#endif
#ifdef POCKETPC2003_UI_MODEL
#include "resourceppc.h"
#endif
#ifdef SMARTPHONE2003_UI_MODEL
#include "resourcesp.h"
#endif
#ifdef AYGSHELL_UI_MODEL
#include "resourceayg.h"
#endif
[!endif]

[!if !ATTRIBUTED]
#include "[!output SAFE_IDL_NAME].h"
[!if CONNECTION_POINTS]
#include "_[!output INTERFACE_NAME]Events_CP.h"
[!endif]
[!endif]

[!if THREADING_SINGLE || THREADING_APARTMENT || THREADING_BOTH]

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif
[!endif]
[!if THREADING_NEUTRAL]
#ifdef _WIN32_WCE
#error "Windows CE 上不支持非特定语言线程的 COM 对象。"
#endif
[!endif]

[!if ATTRIBUTED]

// [!output INTERFACE_NAME]
[
	object,
	uuid("[!output INTERFACE_IID]"),
[!if INTERFACE_DUAL]	dual,[!endif]
[!if AUTOMATION]	oleautomation,[!endif]
[!if INTERFACE_DUAL][!if AUTOMATION]	nonextensible,[!endif][!endif]
	helpstring("[!output INTERFACE_NAME] 接口"),
	pointer_default(unique)
]
__interface [!output INTERFACE_NAME] : [!if INTERFACE_DUAL]IDispatch[!else]IUnknown[!endif]

{
};

[!if CONNECTION_POINTS]

// _[!output INTERFACE_NAME]Events
[
	dispinterface,
	uuid("[!output CONNECTION_POINT_IID]"),
	helpstring("_[!output INTERFACE_NAME]Events 接口")
]
__interface _[!output INTERFACE_NAME]Events
{
};
[!endif]
[!endif]


// [!output CLASS_NAME]

[!if ATTRIBUTED]
[
	coclass,
	default([!output INTERFACE_NAME][!if CONNECTION_POINTS], _[!output INTERFACE_NAME]Events[!endif]),
[!if THREADING_SINGLE]
	threading(single),
[!endif]
[!if THREADING_APARTMENT]
	threading(apartment),
[!endif]
[!if THREADING_BOTH]
	threading(both),
[!endif]
[!if THREADING_FREE]
	threading(free),
[!endif]
[!if THREADING_NEUTRAL]
	threading(neutral),
[!endif]
[!if SUPPORT_ERROR_INFO]
	support_error_info("[!output INTERFACE_NAME]"),
[!endif]
[!if CONNECTION_POINTS]
	event_source(com),
[!endif]
[!if AGGREGATION_NO]
	aggregatable(never),
[!endif]
[!if AGGREGATION_ONLY]
	aggregatable(always),
[!endif]
	vi_progid("[!output VERSION_INDEPENDENT_PROGID]"),
	progid("[!output PROGID]"),
	version(1.0),
	uuid("[!output CLSID_REGISTRY_FORMAT]"),
	helpstring("[!output TYPE_NAME]")
]
[!endif]
class ATL_NO_VTABLE [!output CLASS_NAME] :
[!if ATTRIBUTED]
[!if OBJECT_WITH_SITE]
	public IObjectWithSiteImpl<[!output CLASS_NAME]>,
[!endif]
	public [!output INTERFACE_NAME]
[!else]
[!if THREADING_SINGLE]
	public CComObjectRootEx<CComSingleThreadModel>,
[!endif]
[!if THREADING_APARTMENT]
	public CComObjectRootEx<CComSingleThreadModel>,
[!endif]
[!if THREADING_FREE]
	public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
[!if THREADING_BOTH]
	public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
[!if THREADING_NEUTRAL]
	public CComObjectRootEx<CComMultiThreadModel>,
[!endif]
	public CComCoClass<[!output CLASS_NAME], &CLSID_[!output COCLASS]>,
[!if SUPPORT_ERROR_INFO]
	public ISupportErrorInfo,
[!endif]
[!if CONNECTION_POINTS]
	public IConnectionPointContainerImpl<[!output CLASS_NAME]>,
	public CProxy_[!output INTERFACE_NAME]Events<[!output CLASS_NAME]>,
[!endif]
[!if OBJECT_WITH_SITE]
	public IObjectWithSiteImpl<[!output CLASS_NAME]>,
[!endif]
[!if INTERFACE_DUAL]
	public IDispatchImpl<[!output INTERFACE_NAME], &IID_[!output INTERFACE_NAME], &LIBID_[!output LIB_NAME], /*wMajor =*/ [!output TYPELIB_VERSION_MAJOR], /*wMinor =*/ [!output TYPELIB_VERSION_MINOR]>
[!else]
	public [!output INTERFACE_NAME]
[!endif]
[!endif]
{
public:
	[!output CLASS_NAME]()
	{
[!if FREE_THREADED_MARSHALER]
		m_pUnkMarshaler = NULL;
[!endif]
	}

[!if !ATTRIBUTED]
[!if !DEVICE]
DECLARE_REGISTRY_RESOURCEID([!output RGS_ID])
[!else]
[!if SUPPORT_NON_DCOM]
#ifndef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID([!output RGS_ID])
#endif
[!endif]
[!if SUPPORT_DCOM]
#ifdef _CE_DCOM
DECLARE_REGISTRY_RESOURCEID([!output RGSDCOM_ID])
#endif
[!endif]
[!endif]

[!if AGGREGATION_NO]
DECLARE_NOT_AGGREGATABLE([!output CLASS_NAME])
[!endif]
[!if AGGREGATION_ONLY]
DECLARE_ONLY_AGGREGATABLE([!output CLASS_NAME])
[!endif]

BEGIN_COM_MAP([!output CLASS_NAME])
	COM_INTERFACE_ENTRY([!output INTERFACE_NAME])
[!if INTERFACE_DUAL]
	COM_INTERFACE_ENTRY(IDispatch)
[!endif]
[!if SUPPORT_ERROR_INFO]
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
[!endif]
[!if CONNECTION_POINTS]
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
[!endif]
[!if OBJECT_WITH_SITE]
	COM_INTERFACE_ENTRY(IObjectWithSite)
[!endif]
[!if FREE_THREADED_MARSHALER]
	COM_INTERFACE_ENTRY_AGGREGATE(IID_IMarshal, m_pUnkMarshaler.p)
[!endif]
END_COM_MAP()

[!if CONNECTION_POINTS]
BEGIN_CONNECTION_POINT_MAP([!output CLASS_NAME])
	CONNECTION_POINT_ENTRY(__uuidof(_[!output INTERFACE_NAME]Events))
END_CONNECTION_POINT_MAP()
[!endif]
[!if SUPPORT_ERROR_INFO]
// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid);
[!endif]
[!else]
[!if CONNECTION_POINTS]
	__event __interface _[!output INTERFACE_NAME]Events;
[!endif]
[!endif]

[!if FREE_THREADED_MARSHALER]

	DECLARE_PROTECT_FINAL_CONSTRUCT()
	DECLARE_GET_CONTROLLING_UNKNOWN()

	HRESULT FinalConstruct()
	{
		return CoCreateFreeThreadedMarshaler(
			GetControllingUnknown(), &m_pUnkMarshaler.p);
	}

	void FinalRelease()
	{
		m_pUnkMarshaler.Release();
	}

	CComPtr<IUnknown> m_pUnkMarshaler;
[!else]

	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
[!endif]

public:

};

[!if !ATTRIBUTED]
OBJECT_ENTRY_AUTO(__uuidof([!output COCLASS]), [!output CLASS_NAME])
[!endif]
