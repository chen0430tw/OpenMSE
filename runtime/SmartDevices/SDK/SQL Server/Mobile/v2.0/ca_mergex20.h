
#pragma warning( disable: 4049 )  /* more than 64k source lines */

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 5.03.0286 */
/* at Thu Nov 11 11:46:39 2004
 */
/* Compiler settings for i:\SSCE\Replication\ClientAgent\inc\ca_mergex.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32 (32b run), ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __ca_mergex20_h__
#define __ca_mergex20_h__

/* Forward Declarations */ 

#ifndef __ISSCEParam_FWD_DEFINED__
#define __ISSCEParam_FWD_DEFINED__
typedef interface ISSCEParam ISSCEParam;
#endif 	/* __ISSCEParam_FWD_DEFINED__ */


#ifndef __ISSCEParams_FWD_DEFINED__
#define __ISSCEParams_FWD_DEFINED__
typedef interface ISSCEParams ISSCEParams;
#endif 	/* __ISSCEParams_FWD_DEFINED__ */


#ifndef __ISSCEError_FWD_DEFINED__
#define __ISSCEError_FWD_DEFINED__
typedef interface ISSCEError ISSCEError;
#endif 	/* __ISSCEError_FWD_DEFINED__ */


#ifndef __ISSCEErrors_FWD_DEFINED__
#define __ISSCEErrors_FWD_DEFINED__
typedef interface ISSCEErrors ISSCEErrors;
#endif 	/* __ISSCEErrors_FWD_DEFINED__ */


#ifndef __ISSCEMerge_FWD_DEFINED__
#define __ISSCEMerge_FWD_DEFINED__
typedef interface ISSCEMerge ISSCEMerge;
#endif 	/* __ISSCEMerge_FWD_DEFINED__ */


#ifndef __ISSCERDA_FWD_DEFINED__
#define __ISSCERDA_FWD_DEFINED__
typedef interface ISSCERDA ISSCERDA;
#endif 	/* __ISSCERDA_FWD_DEFINED__ */


#ifndef __ISSCEEngine_FWD_DEFINED__
#define __ISSCEEngine_FWD_DEFINED__
typedef interface ISSCEEngine ISSCEEngine;
#endif 	/* __ISSCEEngine_FWD_DEFINED__ */


#ifndef __Replication_FWD_DEFINED__
#define __Replication_FWD_DEFINED__

#ifdef __cplusplus
typedef class Replication Replication;
#else
typedef struct Replication Replication;
#endif /* __cplusplus */

#endif 	/* __Replication_FWD_DEFINED__ */


#ifndef __RemoteDataAccess_FWD_DEFINED__
#define __RemoteDataAccess_FWD_DEFINED__

#ifdef __cplusplus
typedef class RemoteDataAccess RemoteDataAccess;
#else
typedef struct RemoteDataAccess RemoteDataAccess;
#endif /* __cplusplus */

#endif 	/* __RemoteDataAccess_FWD_DEFINED__ */


#ifndef __Engine_FWD_DEFINED__
#define __Engine_FWD_DEFINED__

#ifdef __cplusplus
typedef class Engine Engine;
#else
typedef struct Engine Engine;
#endif /* __cplusplus */

#endif 	/* __Engine_FWD_DEFINED__ */


#ifndef __SSCEErrors_FWD_DEFINED__
#define __SSCEErrors_FWD_DEFINED__

#ifdef __cplusplus
typedef class SSCEErrors SSCEErrors;
#else
typedef struct SSCEErrors SSCEErrors;
#endif /* __cplusplus */

#endif 	/* __SSCEErrors_FWD_DEFINED__ */


#ifndef __SSCEError_FWD_DEFINED__
#define __SSCEError_FWD_DEFINED__

#ifdef __cplusplus
typedef class SSCEError SSCEError;
#else
typedef struct SSCEError SSCEError;
#endif /* __cplusplus */

#endif 	/* __SSCEError_FWD_DEFINED__ */


#ifndef __SSCEParams_FWD_DEFINED__
#define __SSCEParams_FWD_DEFINED__

#ifdef __cplusplus
typedef class SSCEParams SSCEParams;
#else
typedef struct SSCEParams SSCEParams;
#endif /* __cplusplus */

#endif 	/* __SSCEParams_FWD_DEFINED__ */


#ifndef __SSCEParam_FWD_DEFINED__
#define __SSCEParam_FWD_DEFINED__

#ifdef __cplusplus
typedef class SSCEParam SSCEParam;
#else
typedef struct SSCEParam SSCEParam;
#endif /* __cplusplus */

#endif 	/* __SSCEParam_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_ca_mergex_0000 */
/* [local] */ 

typedef /* [uuid] */  DECLSPEC_UUID("84EECA8F-CF83-4da5-9FED-5551F766A222") 
enum REPL_SECURITY_TYPE
    {	DB_AUTHENTICATION	= 0,
	NT_AUTHENTICATION	= 1
    }	REPL_SECURITY_TYPE;

typedef /* [uuid] */  DECLSPEC_UUID("192979A1-BB6F-4bd8-B9A8-F7D73C347C73") 
enum REPL_NETWORK_TYPE
    {	DEFAULT_NETWORK	= 0,
	TCPIP_SOCKETS	= 1,
	MULTI_PROTOCOL	= 2
    }	REPL_NETWORK_TYPE;

typedef /* [uuid] */  DECLSPEC_UUID("00BF5E8A-F327-4bcd-A538-240232AD8572") 
enum REPL_DBADDOPTION
    {	EXISTING_DATABASE	= 0,
	CREATE_DATABASE	= 1
    }	REPL_DBADDOPTION;

typedef /* [uuid] */  DECLSPEC_UUID("07209B4D-CF7E-4c25-B25E-7F122F00AEB0") 
enum REPL_DBDROPOPTION
    {	LEAVE_DATABASE	= 0,
	DROP_DATABASE	= 1
    }	REPL_DBDROPOPTION;

typedef /* [uuid] */  DECLSPEC_UUID("BE050EB1-1793-4be6-AD8C-E6CCC963726D") 
enum REPL_STATUS_RETURN_CODE
    {	SUCCESS	= 0,
	CANCEL	= 1
    }	REPL_STATUS_RETURN_CODE;

typedef /* [uuid] */  DECLSPEC_UUID("8ABB21BB-3FCC-45f3-B5B0-A02E9C53734F") 
enum REPL_VALIDATE_TYPE
    {	NO_VALIDATION	= 0,
	ROWCOUNT_ONLY	= 1
    }	REPL_VALIDATE_TYPE;

typedef /* [uuid] */  DECLSPEC_UUID("838BD54D-994F-451d-85C5-100FD7B75B1C") 
enum REPL_EXCHANGE_TYPE
    {	UPLOAD	= 1,
	BIDIRECTIONAL	= 3
    }	REPL_EXCHANGE_TYPE;

typedef /* [uuid] */  DECLSPEC_UUID("1786259B-3981-4c65-A9D8-B164F58D2585") 
enum REPL_DATASOURCE_TYPE
    {	SQL_SERVER_CE	= 7
    }	REPL_DATASOURCE_TYPE;

typedef /* [uuid] */  DECLSPEC_UUID("6EBDDB49-3023-4d46-B615-5797630DE886") 
enum RDA_TRACKOPTION
    {	TRACKINGOFF	= 0,
	TRACKINGON	= 1,
	TRACKINGON_INDEXES	= 2,
	TRACKINGOFF_INDEXES	= 3
    }	RDA_TRACKOPTION;

typedef /* [uuid] */  DECLSPEC_UUID("F7BE38B7-4801-497f-B874-A5BF05E919ED") 
enum RDA_BATCHOPTION
    {	BATCHINGOFF	= 0,
	BATCHINGON	= 1
    }	RDA_BATCHOPTION;

typedef /* [uuid] */  DECLSPEC_UUID("D5CE33BD-2F37-42fa-A29F-92BE5E40CE90") 
enum SSCE_ENCRYPTOPTION
    {	ENCRYPTCOPY	= 0,
	ENCRYPTOFF	= 1,
	ENCRYPTON	= 2
    }	SSCE_ENCRYPTOPTION;



extern RPC_IF_HANDLE __MIDL_itf_ca_mergex_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ca_mergex_0000_v0_0_s_ifspec;

#ifndef __ISSCEParam_INTERFACE_DEFINED__
#define __ISSCEParam_INTERFACE_DEFINED__

/* interface ISSCEParam */
/* [unique][helpstring][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEParam;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E55854EE-7C9A-48b5-BEFC-00C49E48FD95")
    ISSCEParam : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Param( 
            /* [retval][out] */ VARIANT __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Param( 
            /* [in] */ VARIANT __RPC_FAR *newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEParamVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEParam __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEParam __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEParam __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEParam __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEParam __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEParam __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEParam __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Param )( 
            ISSCEParam __RPC_FAR * This,
            /* [retval][out] */ VARIANT __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Param )( 
            ISSCEParam __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *newVal);
        
        END_INTERFACE
    } ISSCEParamVtbl;

    interface ISSCEParam
    {
        CONST_VTBL struct ISSCEParamVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEParam_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEParam_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEParam_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEParam_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEParam_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEParam_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEParam_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEParam_get_Param(This,pVal)	\
    (This)->lpVtbl -> get_Param(This,pVal)

#define ISSCEParam_put_Param(This,newVal)	\
    (This)->lpVtbl -> put_Param(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEParam_get_Param_Proxy( 
    ISSCEParam __RPC_FAR * This,
    /* [retval][out] */ VARIANT __RPC_FAR *pVal);


void __RPC_STUB ISSCEParam_get_Param_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEParam_put_Param_Proxy( 
    ISSCEParam __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *newVal);


void __RPC_STUB ISSCEParam_put_Param_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEParam_INTERFACE_DEFINED__ */


#ifndef __ISSCEParams_INTERFACE_DEFINED__
#define __ISSCEParams_INTERFACE_DEFINED__

/* interface ISSCEParams */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEParams;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("E6F19D30-106C-4564-8BCB-62375E413CDB")
    ISSCEParams : public IDispatch
    {
    public:
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *cMembers) = 0;
        
        virtual /* [hidden][id] */ HRESULT STDMETHODCALLTYPE ClearParams( void) = 0;
        
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ ISSCEParam __RPC_FAR *__RPC_FAR *ppobj) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE AddParam( 
            /* [in] */ VARIANT __RPC_FAR *pvarParam) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ISSCEParam __RPC_FAR *pISSCEParam) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEParamsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEParams __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEParams __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEParams __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NewEnum )( 
            ISSCEParams __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ISSCEParams __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *cMembers);
        
        /* [hidden][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearParams )( 
            ISSCEParams __RPC_FAR * This);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ ISSCEParam __RPC_FAR *__RPC_FAR *ppobj);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddParam )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ VARIANT __RPC_FAR *pvarParam);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ISSCEParams __RPC_FAR * This,
            /* [in] */ ISSCEParam __RPC_FAR *pISSCEParam);
        
        END_INTERFACE
    } ISSCEParamsVtbl;

    interface ISSCEParams
    {
        CONST_VTBL struct ISSCEParamsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEParams_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEParams_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEParams_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEParams_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEParams_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEParams_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEParams_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEParams_get_NewEnum(This,ppvObject)	\
    (This)->lpVtbl -> get_NewEnum(This,ppvObject)

#define ISSCEParams_get_Count(This,cMembers)	\
    (This)->lpVtbl -> get_Count(This,cMembers)

#define ISSCEParams_ClearParams(This)	\
    (This)->lpVtbl -> ClearParams(This)

#define ISSCEParams_get_Item(This,Index,ppobj)	\
    (This)->lpVtbl -> get_Item(This,Index,ppobj)

#define ISSCEParams_AddParam(This,pvarParam)	\
    (This)->lpVtbl -> AddParam(This,pvarParam)

#define ISSCEParams_Add(This,pISSCEParam)	\
    (This)->lpVtbl -> Add(This,pISSCEParam)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEParams_get_NewEnum_Proxy( 
    ISSCEParams __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject);


void __RPC_STUB ISSCEParams_get_NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEParams_get_Count_Proxy( 
    ISSCEParams __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *cMembers);


void __RPC_STUB ISSCEParams_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][id] */ HRESULT STDMETHODCALLTYPE ISSCEParams_ClearParams_Proxy( 
    ISSCEParams __RPC_FAR * This);


void __RPC_STUB ISSCEParams_ClearParams_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEParams_get_Item_Proxy( 
    ISSCEParams __RPC_FAR * This,
    /* [in] */ VARIANT Index,
    /* [retval][out] */ ISSCEParam __RPC_FAR *__RPC_FAR *ppobj);


void __RPC_STUB ISSCEParams_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEParams_AddParam_Proxy( 
    ISSCEParams __RPC_FAR * This,
    /* [in] */ VARIANT __RPC_FAR *pvarParam);


void __RPC_STUB ISSCEParams_AddParam_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEParams_Add_Proxy( 
    ISSCEParams __RPC_FAR * This,
    /* [in] */ ISSCEParam __RPC_FAR *pISSCEParam);


void __RPC_STUB ISSCEParams_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEParams_INTERFACE_DEFINED__ */


#ifndef __ISSCEError_INTERFACE_DEFINED__
#define __ISSCEError_INTERFACE_DEFINED__

/* interface ISSCEError */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEError;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9C967ADE-FC3C-4472-A19A-401F2405A2DD")
    ISSCEError : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Description( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Source( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Number( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_NativeError( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Params( 
            /* [retval][out] */ ISSCEParams __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE SetDescription( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE SetSource( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE SetNumber( 
            /* [in] */ long newVal) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE SetNativeError( 
            /* [in] */ long newVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEErrorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEError __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEError __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEError __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Description )( 
            ISSCEError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Source )( 
            ISSCEError __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Number )( 
            ISSCEError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NativeError )( 
            ISSCEError __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Params )( 
            ISSCEError __RPC_FAR * This,
            /* [retval][out] */ ISSCEParams __RPC_FAR *__RPC_FAR *pVal);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetDescription )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetSource )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetNumber )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ long newVal);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SetNativeError )( 
            ISSCEError __RPC_FAR * This,
            /* [in] */ long newVal);
        
        END_INTERFACE
    } ISSCEErrorVtbl;

    interface ISSCEError
    {
        CONST_VTBL struct ISSCEErrorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEError_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEError_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEError_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEError_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEError_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEError_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEError_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEError_get_Description(This,pVal)	\
    (This)->lpVtbl -> get_Description(This,pVal)

#define ISSCEError_get_Source(This,pVal)	\
    (This)->lpVtbl -> get_Source(This,pVal)

#define ISSCEError_get_Number(This,pVal)	\
    (This)->lpVtbl -> get_Number(This,pVal)

#define ISSCEError_get_NativeError(This,pVal)	\
    (This)->lpVtbl -> get_NativeError(This,pVal)

#define ISSCEError_get_Params(This,pVal)	\
    (This)->lpVtbl -> get_Params(This,pVal)

#define ISSCEError_SetDescription(This,newVal)	\
    (This)->lpVtbl -> SetDescription(This,newVal)

#define ISSCEError_SetSource(This,newVal)	\
    (This)->lpVtbl -> SetSource(This,newVal)

#define ISSCEError_SetNumber(This,newVal)	\
    (This)->lpVtbl -> SetNumber(This,newVal)

#define ISSCEError_SetNativeError(This,newVal)	\
    (This)->lpVtbl -> SetNativeError(This,newVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEError_get_Description_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEError_get_Description_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEError_get_Source_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEError_get_Source_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEError_get_Number_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEError_get_Number_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEError_get_NativeError_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEError_get_NativeError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEError_get_Params_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [retval][out] */ ISSCEParams __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ISSCEError_get_Params_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEError_SetDescription_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEError_SetDescription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEError_SetSource_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEError_SetSource_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEError_SetNumber_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISSCEError_SetNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEError_SetNativeError_Proxy( 
    ISSCEError __RPC_FAR * This,
    /* [in] */ long newVal);


void __RPC_STUB ISSCEError_SetNativeError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEError_INTERFACE_DEFINED__ */


#ifndef __ISSCEErrors_INTERFACE_DEFINED__
#define __ISSCEErrors_INTERFACE_DEFINED__

/* interface ISSCEErrors */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEErrors;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3FE59143-B820-4d41-A6A9-8429959FCA52")
    ISSCEErrors : public IDispatch
    {
    public:
        virtual /* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE get_NewEnum( 
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Count( 
            /* [retval][out] */ long __RPC_FAR *cMembers) = 0;
        
        virtual /* [restricted][id] */ HRESULT STDMETHODCALLTYPE ClearErrors( void) = 0;
        
        virtual /* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE get_Item( 
            /* [in] */ VARIANT Index,
            /* [retval][out] */ ISSCEError __RPC_FAR *__RPC_FAR *ppobj) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE AddError( 
            /* [in] */ BSTR bstrDescription,
            /* [in] */ BSTR bstrSource,
            /* [in] */ long lNumber,
            /* [in] */ long lNativeError,
            /* [in] */ long lParams,
            /* [in] */ VARIANT __RPC_FAR *pvarParam) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE Add( 
            /* [in] */ ISSCEError __RPC_FAR *pISSCEError) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEErrorsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEErrors __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEErrors __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEErrors __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [restricted][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_NewEnum )( 
            ISSCEErrors __RPC_FAR * This,
            /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Count )( 
            ISSCEErrors __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *cMembers);
        
        /* [restricted][id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ClearErrors )( 
            ISSCEErrors __RPC_FAR * This);
        
        /* [hidden][id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Item )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ VARIANT Index,
            /* [retval][out] */ ISSCEError __RPC_FAR *__RPC_FAR *ppobj);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddError )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ BSTR bstrDescription,
            /* [in] */ BSTR bstrSource,
            /* [in] */ long lNumber,
            /* [in] */ long lNativeError,
            /* [in] */ long lParams,
            /* [in] */ VARIANT __RPC_FAR *pvarParam);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Add )( 
            ISSCEErrors __RPC_FAR * This,
            /* [in] */ ISSCEError __RPC_FAR *pISSCEError);
        
        END_INTERFACE
    } ISSCEErrorsVtbl;

    interface ISSCEErrors
    {
        CONST_VTBL struct ISSCEErrorsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEErrors_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEErrors_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEErrors_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEErrors_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEErrors_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEErrors_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEErrors_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEErrors_get_NewEnum(This,ppvObject)	\
    (This)->lpVtbl -> get_NewEnum(This,ppvObject)

#define ISSCEErrors_get_Count(This,cMembers)	\
    (This)->lpVtbl -> get_Count(This,cMembers)

#define ISSCEErrors_ClearErrors(This)	\
    (This)->lpVtbl -> ClearErrors(This)

#define ISSCEErrors_get_Item(This,Index,ppobj)	\
    (This)->lpVtbl -> get_Item(This,Index,ppobj)

#define ISSCEErrors_AddError(This,bstrDescription,bstrSource,lNumber,lNativeError,lParams,pvarParam)	\
    (This)->lpVtbl -> AddError(This,bstrDescription,bstrSource,lNumber,lNativeError,lParams,pvarParam)

#define ISSCEErrors_Add(This,pISSCEError)	\
    (This)->lpVtbl -> Add(This,pISSCEError)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [restricted][id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_get_NewEnum_Proxy( 
    ISSCEErrors __RPC_FAR * This,
    /* [retval][out] */ LPUNKNOWN __RPC_FAR *ppvObject);


void __RPC_STUB ISSCEErrors_get_NewEnum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_get_Count_Proxy( 
    ISSCEErrors __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *cMembers);


void __RPC_STUB ISSCEErrors_get_Count_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [restricted][id] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_ClearErrors_Proxy( 
    ISSCEErrors __RPC_FAR * This);


void __RPC_STUB ISSCEErrors_ClearErrors_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden][id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_get_Item_Proxy( 
    ISSCEErrors __RPC_FAR * This,
    /* [in] */ VARIANT Index,
    /* [retval][out] */ ISSCEError __RPC_FAR *__RPC_FAR *ppobj);


void __RPC_STUB ISSCEErrors_get_Item_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_AddError_Proxy( 
    ISSCEErrors __RPC_FAR * This,
    /* [in] */ BSTR bstrDescription,
    /* [in] */ BSTR bstrSource,
    /* [in] */ long lNumber,
    /* [in] */ long lNativeError,
    /* [in] */ long lParams,
    /* [in] */ VARIANT __RPC_FAR *pvarParam);


void __RPC_STUB ISSCEErrors_AddError_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEErrors_Add_Proxy( 
    ISSCEErrors __RPC_FAR * This,
    /* [in] */ ISSCEError __RPC_FAR *pISSCEError);


void __RPC_STUB ISSCEErrors_Add_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEErrors_INTERFACE_DEFINED__ */


#ifndef __ISSCEMerge_INTERFACE_DEFINED__
#define __ISSCEMerge_INTERFACE_DEFINED__

/* interface ISSCEMerge */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEMerge;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("7EB61672-6301-43d9-8D21-C2D5723AF10B")
    ISSCEMerge : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Distributor( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Distributor( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Distributor( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DistributorAddress( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DistributorAddress( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_DistributorAddress( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DistributorNetwork( 
            /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DistributorNetwork( 
            /* [in] */ REPL_NETWORK_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_DistributorNetwork( 
            /* [in] */ REPL_NETWORK_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DistributorSecurityMode( 
            /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DistributorSecurityMode( 
            /* [in] */ REPL_SECURITY_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_DistributorSecurityMode( 
            /* [in] */ REPL_SECURITY_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DistributorLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DistributorLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_DistributorLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_DistributorPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_DistributorPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_DistributorPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorRecords( 
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ExchangeType( 
            /* [retval][out] */ REPL_EXCHANGE_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ExchangeType( 
            /* [in] */ REPL_EXCHANGE_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_ExchangeType( 
            /* [in] */ REPL_EXCHANGE_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetURL( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetURL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetURL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LoginTimeout( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LoginTimeout( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_LoginTimeout( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ProfileName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_ProfileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_ProfileName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Publisher( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Publisher( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Publisher( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherAddress( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherAddress( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherAddress( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherNetwork( 
            /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherNetwork( 
            /* [in] */ REPL_NETWORK_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherNetwork( 
            /* [in] */ REPL_NETWORK_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherDatabase( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherDatabase( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherDatabase( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherSecurityMode( 
            /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherSecurityMode( 
            /* [in] */ REPL_SECURITY_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherSecurityMode( 
            /* [in] */ REPL_SECURITY_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_PublisherPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_PublisherPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Publication( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Publication( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Publication( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_QueryTimeout( 
            /* [retval][out] */ short __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_QueryTimeout( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_QueryTimeout( 
            /* [in] */ short newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SubscriberConnectionString( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_SubscriberConnectionString( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_SubscriberConnectionString( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherChanges( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_PublisherConflicts( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SubscriberChanges( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_SubscriberConflicts( 
            /* [retval][out] */ long __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Validate( 
            /* [retval][out] */ REPL_VALIDATE_TYPE __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Validate( 
            /* [in] */ REPL_VALIDATE_TYPE newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Validate( 
            /* [in] */ REPL_VALIDATE_TYPE newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_HostName( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_HostName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_HostName( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_Subscriber( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_Subscriber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_Subscriber( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyServer( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyServer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyServer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Initialize( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE AddSubscription( 
            /* [defaultvalue][in] */ REPL_DBADDOPTION DBAddOption = CREATE_DATABASE) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE DropSubscription( 
            /* [defaultvalue][in] */ REPL_DBDROPOPTION DBDropOption = DROP_DATABASE) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Run( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Terminate( void) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE ReInitializeSubscription( 
            /* [defaultvalue][in] */ VARIANT_BOOL UploadBeforeReinit = FALSE) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEMergeVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEMerge __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEMerge __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEMerge __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Distributor )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Distributor )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Distributor )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DistributorAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DistributorAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_DistributorAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DistributorNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DistributorNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_NETWORK_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_DistributorNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_NETWORK_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DistributorSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DistributorSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_SECURITY_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_DistributorSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_SECURITY_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DistributorLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DistributorLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_DistributorLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_DistributorPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_DistributorPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_DistributorPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ErrorRecords )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ExchangeType )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_EXCHANGE_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ExchangeType )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_EXCHANGE_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_ExchangeType )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_EXCHANGE_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetURL )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetURL )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetURL )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LoginTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LoginTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_LoginTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ProfileName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_ProfileName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_ProfileName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Publisher )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Publisher )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Publisher )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherAddress )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_NETWORK_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherNetwork )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_NETWORK_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherDatabase )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherDatabase )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherDatabase )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_SECURITY_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherSecurityMode )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_SECURITY_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_PublisherPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_PublisherPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Publication )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Publication )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Publication )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_QueryTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ short __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_QueryTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_QueryTimeout )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ short newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SubscriberConnectionString )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_SubscriberConnectionString )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_SubscriberConnectionString )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherChanges )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_PublisherConflicts )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SubscriberChanges )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_SubscriberConflicts )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ long __RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Validate )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ REPL_VALIDATE_TYPE __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Validate )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_VALIDATE_TYPE newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Validate )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ REPL_VALIDATE_TYPE newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_HostName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_HostName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_HostName )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Subscriber )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_Subscriber )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_Subscriber )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyServer )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyServer )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyServer )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyLogin )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyPassword )( 
            ISSCEMerge __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Initialize )( 
            ISSCEMerge __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AddSubscription )( 
            ISSCEMerge __RPC_FAR * This,
            /* [defaultvalue][in] */ REPL_DBADDOPTION DBAddOption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DropSubscription )( 
            ISSCEMerge __RPC_FAR * This,
            /* [defaultvalue][in] */ REPL_DBDROPOPTION DBDropOption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Run )( 
            ISSCEMerge __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Terminate )( 
            ISSCEMerge __RPC_FAR * This);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ReInitializeSubscription )( 
            ISSCEMerge __RPC_FAR * This,
            /* [defaultvalue][in] */ VARIANT_BOOL UploadBeforeReinit);
        
        END_INTERFACE
    } ISSCEMergeVtbl;

    interface ISSCEMerge
    {
        CONST_VTBL struct ISSCEMergeVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEMerge_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEMerge_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEMerge_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEMerge_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEMerge_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEMerge_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEMerge_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEMerge_get_Distributor(This,pVal)	\
    (This)->lpVtbl -> get_Distributor(This,pVal)

#define ISSCEMerge_put_Distributor(This,newVal)	\
    (This)->lpVtbl -> put_Distributor(This,newVal)

#define ISSCEMerge_putref_Distributor(This,newVal)	\
    (This)->lpVtbl -> putref_Distributor(This,newVal)

#define ISSCEMerge_get_DistributorAddress(This,pVal)	\
    (This)->lpVtbl -> get_DistributorAddress(This,pVal)

#define ISSCEMerge_put_DistributorAddress(This,newVal)	\
    (This)->lpVtbl -> put_DistributorAddress(This,newVal)

#define ISSCEMerge_putref_DistributorAddress(This,newVal)	\
    (This)->lpVtbl -> putref_DistributorAddress(This,newVal)

#define ISSCEMerge_get_DistributorNetwork(This,pVal)	\
    (This)->lpVtbl -> get_DistributorNetwork(This,pVal)

#define ISSCEMerge_put_DistributorNetwork(This,newVal)	\
    (This)->lpVtbl -> put_DistributorNetwork(This,newVal)

#define ISSCEMerge_putref_DistributorNetwork(This,newVal)	\
    (This)->lpVtbl -> putref_DistributorNetwork(This,newVal)

#define ISSCEMerge_get_DistributorSecurityMode(This,pVal)	\
    (This)->lpVtbl -> get_DistributorSecurityMode(This,pVal)

#define ISSCEMerge_put_DistributorSecurityMode(This,newVal)	\
    (This)->lpVtbl -> put_DistributorSecurityMode(This,newVal)

#define ISSCEMerge_putref_DistributorSecurityMode(This,newVal)	\
    (This)->lpVtbl -> putref_DistributorSecurityMode(This,newVal)

#define ISSCEMerge_get_DistributorLogin(This,pVal)	\
    (This)->lpVtbl -> get_DistributorLogin(This,pVal)

#define ISSCEMerge_put_DistributorLogin(This,newVal)	\
    (This)->lpVtbl -> put_DistributorLogin(This,newVal)

#define ISSCEMerge_putref_DistributorLogin(This,newVal)	\
    (This)->lpVtbl -> putref_DistributorLogin(This,newVal)

#define ISSCEMerge_get_DistributorPassword(This,pVal)	\
    (This)->lpVtbl -> get_DistributorPassword(This,pVal)

#define ISSCEMerge_put_DistributorPassword(This,newVal)	\
    (This)->lpVtbl -> put_DistributorPassword(This,newVal)

#define ISSCEMerge_putref_DistributorPassword(This,newVal)	\
    (This)->lpVtbl -> putref_DistributorPassword(This,newVal)

#define ISSCEMerge_get_ErrorRecords(This,pVal)	\
    (This)->lpVtbl -> get_ErrorRecords(This,pVal)

#define ISSCEMerge_get_ExchangeType(This,pVal)	\
    (This)->lpVtbl -> get_ExchangeType(This,pVal)

#define ISSCEMerge_put_ExchangeType(This,newVal)	\
    (This)->lpVtbl -> put_ExchangeType(This,newVal)

#define ISSCEMerge_putref_ExchangeType(This,newVal)	\
    (This)->lpVtbl -> putref_ExchangeType(This,newVal)

#define ISSCEMerge_get_InternetURL(This,pVal)	\
    (This)->lpVtbl -> get_InternetURL(This,pVal)

#define ISSCEMerge_put_InternetURL(This,newVal)	\
    (This)->lpVtbl -> put_InternetURL(This,newVal)

#define ISSCEMerge_putref_InternetURL(This,newVal)	\
    (This)->lpVtbl -> putref_InternetURL(This,newVal)

#define ISSCEMerge_get_InternetLogin(This,pVal)	\
    (This)->lpVtbl -> get_InternetLogin(This,pVal)

#define ISSCEMerge_put_InternetLogin(This,newVal)	\
    (This)->lpVtbl -> put_InternetLogin(This,newVal)

#define ISSCEMerge_putref_InternetLogin(This,newVal)	\
    (This)->lpVtbl -> putref_InternetLogin(This,newVal)

#define ISSCEMerge_get_InternetPassword(This,pVal)	\
    (This)->lpVtbl -> get_InternetPassword(This,pVal)

#define ISSCEMerge_put_InternetPassword(This,newVal)	\
    (This)->lpVtbl -> put_InternetPassword(This,newVal)

#define ISSCEMerge_putref_InternetPassword(This,newVal)	\
    (This)->lpVtbl -> putref_InternetPassword(This,newVal)

#define ISSCEMerge_get_LoginTimeout(This,pVal)	\
    (This)->lpVtbl -> get_LoginTimeout(This,pVal)

#define ISSCEMerge_put_LoginTimeout(This,newVal)	\
    (This)->lpVtbl -> put_LoginTimeout(This,newVal)

#define ISSCEMerge_putref_LoginTimeout(This,newVal)	\
    (This)->lpVtbl -> putref_LoginTimeout(This,newVal)

#define ISSCEMerge_get_ProfileName(This,pVal)	\
    (This)->lpVtbl -> get_ProfileName(This,pVal)

#define ISSCEMerge_put_ProfileName(This,newVal)	\
    (This)->lpVtbl -> put_ProfileName(This,newVal)

#define ISSCEMerge_putref_ProfileName(This,newVal)	\
    (This)->lpVtbl -> putref_ProfileName(This,newVal)

#define ISSCEMerge_get_Publisher(This,pVal)	\
    (This)->lpVtbl -> get_Publisher(This,pVal)

#define ISSCEMerge_put_Publisher(This,newVal)	\
    (This)->lpVtbl -> put_Publisher(This,newVal)

#define ISSCEMerge_putref_Publisher(This,newVal)	\
    (This)->lpVtbl -> putref_Publisher(This,newVal)

#define ISSCEMerge_get_PublisherAddress(This,pVal)	\
    (This)->lpVtbl -> get_PublisherAddress(This,pVal)

#define ISSCEMerge_put_PublisherAddress(This,newVal)	\
    (This)->lpVtbl -> put_PublisherAddress(This,newVal)

#define ISSCEMerge_putref_PublisherAddress(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherAddress(This,newVal)

#define ISSCEMerge_get_PublisherNetwork(This,pVal)	\
    (This)->lpVtbl -> get_PublisherNetwork(This,pVal)

#define ISSCEMerge_put_PublisherNetwork(This,newVal)	\
    (This)->lpVtbl -> put_PublisherNetwork(This,newVal)

#define ISSCEMerge_putref_PublisherNetwork(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherNetwork(This,newVal)

#define ISSCEMerge_get_PublisherDatabase(This,pVal)	\
    (This)->lpVtbl -> get_PublisherDatabase(This,pVal)

#define ISSCEMerge_put_PublisherDatabase(This,newVal)	\
    (This)->lpVtbl -> put_PublisherDatabase(This,newVal)

#define ISSCEMerge_putref_PublisherDatabase(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherDatabase(This,newVal)

#define ISSCEMerge_get_PublisherSecurityMode(This,pVal)	\
    (This)->lpVtbl -> get_PublisherSecurityMode(This,pVal)

#define ISSCEMerge_put_PublisherSecurityMode(This,newVal)	\
    (This)->lpVtbl -> put_PublisherSecurityMode(This,newVal)

#define ISSCEMerge_putref_PublisherSecurityMode(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherSecurityMode(This,newVal)

#define ISSCEMerge_get_PublisherLogin(This,pVal)	\
    (This)->lpVtbl -> get_PublisherLogin(This,pVal)

#define ISSCEMerge_put_PublisherLogin(This,newVal)	\
    (This)->lpVtbl -> put_PublisherLogin(This,newVal)

#define ISSCEMerge_putref_PublisherLogin(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherLogin(This,newVal)

#define ISSCEMerge_get_PublisherPassword(This,pVal)	\
    (This)->lpVtbl -> get_PublisherPassword(This,pVal)

#define ISSCEMerge_put_PublisherPassword(This,newVal)	\
    (This)->lpVtbl -> put_PublisherPassword(This,newVal)

#define ISSCEMerge_putref_PublisherPassword(This,newVal)	\
    (This)->lpVtbl -> putref_PublisherPassword(This,newVal)

#define ISSCEMerge_get_Publication(This,pVal)	\
    (This)->lpVtbl -> get_Publication(This,pVal)

#define ISSCEMerge_put_Publication(This,newVal)	\
    (This)->lpVtbl -> put_Publication(This,newVal)

#define ISSCEMerge_putref_Publication(This,newVal)	\
    (This)->lpVtbl -> putref_Publication(This,newVal)

#define ISSCEMerge_get_QueryTimeout(This,pVal)	\
    (This)->lpVtbl -> get_QueryTimeout(This,pVal)

#define ISSCEMerge_put_QueryTimeout(This,newVal)	\
    (This)->lpVtbl -> put_QueryTimeout(This,newVal)

#define ISSCEMerge_putref_QueryTimeout(This,newVal)	\
    (This)->lpVtbl -> putref_QueryTimeout(This,newVal)

#define ISSCEMerge_get_SubscriberConnectionString(This,pVal)	\
    (This)->lpVtbl -> get_SubscriberConnectionString(This,pVal)

#define ISSCEMerge_put_SubscriberConnectionString(This,newVal)	\
    (This)->lpVtbl -> put_SubscriberConnectionString(This,newVal)

#define ISSCEMerge_putref_SubscriberConnectionString(This,newVal)	\
    (This)->lpVtbl -> putref_SubscriberConnectionString(This,newVal)

#define ISSCEMerge_get_PublisherChanges(This,pVal)	\
    (This)->lpVtbl -> get_PublisherChanges(This,pVal)

#define ISSCEMerge_get_PublisherConflicts(This,pVal)	\
    (This)->lpVtbl -> get_PublisherConflicts(This,pVal)

#define ISSCEMerge_get_SubscriberChanges(This,pVal)	\
    (This)->lpVtbl -> get_SubscriberChanges(This,pVal)

#define ISSCEMerge_get_SubscriberConflicts(This,pVal)	\
    (This)->lpVtbl -> get_SubscriberConflicts(This,pVal)

#define ISSCEMerge_get_Validate(This,pVal)	\
    (This)->lpVtbl -> get_Validate(This,pVal)

#define ISSCEMerge_put_Validate(This,newVal)	\
    (This)->lpVtbl -> put_Validate(This,newVal)

#define ISSCEMerge_putref_Validate(This,newVal)	\
    (This)->lpVtbl -> putref_Validate(This,newVal)

#define ISSCEMerge_get_HostName(This,pVal)	\
    (This)->lpVtbl -> get_HostName(This,pVal)

#define ISSCEMerge_put_HostName(This,newVal)	\
    (This)->lpVtbl -> put_HostName(This,newVal)

#define ISSCEMerge_putref_HostName(This,newVal)	\
    (This)->lpVtbl -> putref_HostName(This,newVal)

#define ISSCEMerge_get_Subscriber(This,pVal)	\
    (This)->lpVtbl -> get_Subscriber(This,pVal)

#define ISSCEMerge_put_Subscriber(This,newVal)	\
    (This)->lpVtbl -> put_Subscriber(This,newVal)

#define ISSCEMerge_putref_Subscriber(This,newVal)	\
    (This)->lpVtbl -> putref_Subscriber(This,newVal)

#define ISSCEMerge_get_InternetProxyServer(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyServer(This,pVal)

#define ISSCEMerge_put_InternetProxyServer(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyServer(This,newVal)

#define ISSCEMerge_putref_InternetProxyServer(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyServer(This,newVal)

#define ISSCEMerge_get_InternetProxyLogin(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyLogin(This,pVal)

#define ISSCEMerge_put_InternetProxyLogin(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyLogin(This,newVal)

#define ISSCEMerge_putref_InternetProxyLogin(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyLogin(This,newVal)

#define ISSCEMerge_get_InternetProxyPassword(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyPassword(This,pVal)

#define ISSCEMerge_put_InternetProxyPassword(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyPassword(This,newVal)

#define ISSCEMerge_putref_InternetProxyPassword(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyPassword(This,newVal)

#define ISSCEMerge_Initialize(This)	\
    (This)->lpVtbl -> Initialize(This)

#define ISSCEMerge_AddSubscription(This,DBAddOption)	\
    (This)->lpVtbl -> AddSubscription(This,DBAddOption)

#define ISSCEMerge_DropSubscription(This,DBDropOption)	\
    (This)->lpVtbl -> DropSubscription(This,DBDropOption)

#define ISSCEMerge_Run(This)	\
    (This)->lpVtbl -> Run(This)

#define ISSCEMerge_Terminate(This)	\
    (This)->lpVtbl -> Terminate(This)

#define ISSCEMerge_ReInitializeSubscription(This,UploadBeforeReinit)	\
    (This)->lpVtbl -> ReInitializeSubscription(This,UploadBeforeReinit)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_Distributor_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_Distributor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_Distributor_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_Distributor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_Distributor_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_Distributor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_DistributorAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_DistributorAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_DistributorAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_DistributorAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_DistributorAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_DistributorAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_DistributorNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_DistributorNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_DistributorNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_NETWORK_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_DistributorNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_DistributorNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_NETWORK_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_DistributorNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_DistributorSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_DistributorSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_DistributorSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_SECURITY_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_DistributorSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_DistributorSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_SECURITY_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_DistributorSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_DistributorLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_DistributorLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_DistributorLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_DistributorLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_DistributorLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_DistributorLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_DistributorPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_DistributorPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_DistributorPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_DistributorPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_DistributorPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_DistributorPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_ErrorRecords_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_ErrorRecords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_ExchangeType_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_EXCHANGE_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_ExchangeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_ExchangeType_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_EXCHANGE_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_ExchangeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_ExchangeType_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_EXCHANGE_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_ExchangeType_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetURL_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetURL_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetURL_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_LoginTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_LoginTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_LoginTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB ISSCEMerge_put_LoginTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_LoginTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB ISSCEMerge_putref_LoginTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_ProfileName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_ProfileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_ProfileName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_ProfileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_ProfileName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_ProfileName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_Publisher_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_Publisher_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_Publisher_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_Publisher_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_Publisher_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_Publisher_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_PublisherAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherAddress_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherAddress_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_NETWORK_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_NETWORK_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_PublisherNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherNetwork_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_NETWORK_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherNetwork_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherDatabase_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherDatabase_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_PublisherDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherDatabase_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_SECURITY_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_SECURITY_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_PublisherSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherSecurityMode_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_SECURITY_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherSecurityMode_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_PublisherLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_PublisherPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_PublisherPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_PublisherPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_PublisherPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_Publication_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_Publication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_Publication_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_Publication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_Publication_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_Publication_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_QueryTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ short __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_QueryTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_QueryTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB ISSCEMerge_put_QueryTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_QueryTimeout_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ short newVal);


void __RPC_STUB ISSCEMerge_putref_QueryTimeout_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_SubscriberConnectionString_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_SubscriberConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_SubscriberConnectionString_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_SubscriberConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_SubscriberConnectionString_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_SubscriberConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherChanges_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherChanges_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_PublisherConflicts_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_PublisherConflicts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_SubscriberChanges_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_SubscriberChanges_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_SubscriberConflicts_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ long __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_SubscriberConflicts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_Validate_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ REPL_VALIDATE_TYPE __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_Validate_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_VALIDATE_TYPE newVal);


void __RPC_STUB ISSCEMerge_put_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_Validate_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ REPL_VALIDATE_TYPE newVal);


void __RPC_STUB ISSCEMerge_putref_Validate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_HostName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_HostName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_HostName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_HostName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_HostName_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_HostName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_Subscriber_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_Subscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_Subscriber_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_Subscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_Subscriber_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_Subscriber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetProxyServer_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetProxyServer_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetProxyServer_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetProxyLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetProxyLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetProxyLogin_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_get_InternetProxyPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCEMerge_get_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_put_InternetProxyPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_put_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_putref_InternetProxyPassword_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCEMerge_putref_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_Initialize_Proxy( 
    ISSCEMerge __RPC_FAR * This);


void __RPC_STUB ISSCEMerge_Initialize_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_AddSubscription_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [defaultvalue][in] */ REPL_DBADDOPTION DBAddOption);


void __RPC_STUB ISSCEMerge_AddSubscription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_DropSubscription_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [defaultvalue][in] */ REPL_DBDROPOPTION DBDropOption);


void __RPC_STUB ISSCEMerge_DropSubscription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_Run_Proxy( 
    ISSCEMerge __RPC_FAR * This);


void __RPC_STUB ISSCEMerge_Run_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_Terminate_Proxy( 
    ISSCEMerge __RPC_FAR * This);


void __RPC_STUB ISSCEMerge_Terminate_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEMerge_ReInitializeSubscription_Proxy( 
    ISSCEMerge __RPC_FAR * This,
    /* [defaultvalue][in] */ VARIANT_BOOL UploadBeforeReinit);


void __RPC_STUB ISSCEMerge_ReInitializeSubscription_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEMerge_INTERFACE_DEFINED__ */


#ifndef __ISSCERDA_INTERFACE_DEFINED__
#define __ISSCERDA_INTERFACE_DEFINED__

/* interface ISSCERDA */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCERDA;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("95661E4D-9108-424e-A590-0010FC73E9DB")
    ISSCERDA : public IDispatch
    {
    public:
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_LocalConnectionString( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_LocalConnectionString( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_LocalConnectionString( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetURL( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetURL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetURL( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorRecords( 
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyServer( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyServer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyServer( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyLogin( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyLogin( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_InternetProxyPassword( 
            /* [retval][out] */ BSTR __RPC_FAR *pVal) = 0;
        
        virtual /* [id][propput] */ HRESULT STDMETHODCALLTYPE put_InternetProxyPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id][propputref] */ HRESULT STDMETHODCALLTYPE putref_InternetProxyPassword( 
            /* [in] */ BSTR newVal) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Pull( 
            /* [in] */ BSTR LocalTableName,
            /* [in] */ BSTR SQLSelectString,
            /* [in] */ BSTR OLEDBConnectionString,
            /* [defaultvalue][in] */ RDA_TRACKOPTION TrackOption = TRACKINGOFF,
            /* [defaultvalue][in] */ BSTR ErrorTableName = L"") = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE Push( 
            /* [in] */ BSTR LocalTableName,
            /* [in] */ BSTR OLEDBConnectionString,
            /* [defaultvalue][in] */ RDA_BATCHOPTION BatchOption = BATCHINGOFF) = 0;
        
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE SubmitSQL( 
            /* [in] */ BSTR SQLString,
            /* [in] */ BSTR OLEDBConnectionString) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCERDAVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCERDA __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCERDA __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCERDA __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_LocalConnectionString )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_LocalConnectionString )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_LocalConnectionString )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetURL )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetURL )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetURL )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ErrorRecords )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyServer )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyServer )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyServer )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyLogin )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_InternetProxyPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [retval][out] */ BSTR __RPC_FAR *pVal);
        
        /* [id][propput] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *put_InternetProxyPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id][propputref] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *putref_InternetProxyPassword )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR newVal);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Pull )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR LocalTableName,
            /* [in] */ BSTR SQLSelectString,
            /* [in] */ BSTR OLEDBConnectionString,
            /* [defaultvalue][in] */ RDA_TRACKOPTION TrackOption,
            /* [defaultvalue][in] */ BSTR ErrorTableName);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Push )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR LocalTableName,
            /* [in] */ BSTR OLEDBConnectionString,
            /* [defaultvalue][in] */ RDA_BATCHOPTION BatchOption);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *SubmitSQL )( 
            ISSCERDA __RPC_FAR * This,
            /* [in] */ BSTR SQLString,
            /* [in] */ BSTR OLEDBConnectionString);
        
        END_INTERFACE
    } ISSCERDAVtbl;

    interface ISSCERDA
    {
        CONST_VTBL struct ISSCERDAVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCERDA_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCERDA_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCERDA_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCERDA_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCERDA_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCERDA_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCERDA_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCERDA_get_LocalConnectionString(This,pVal)	\
    (This)->lpVtbl -> get_LocalConnectionString(This,pVal)

#define ISSCERDA_put_LocalConnectionString(This,newVal)	\
    (This)->lpVtbl -> put_LocalConnectionString(This,newVal)

#define ISSCERDA_putref_LocalConnectionString(This,newVal)	\
    (This)->lpVtbl -> putref_LocalConnectionString(This,newVal)

#define ISSCERDA_get_InternetURL(This,pVal)	\
    (This)->lpVtbl -> get_InternetURL(This,pVal)

#define ISSCERDA_put_InternetURL(This,newVal)	\
    (This)->lpVtbl -> put_InternetURL(This,newVal)

#define ISSCERDA_putref_InternetURL(This,newVal)	\
    (This)->lpVtbl -> putref_InternetURL(This,newVal)

#define ISSCERDA_get_InternetLogin(This,pVal)	\
    (This)->lpVtbl -> get_InternetLogin(This,pVal)

#define ISSCERDA_put_InternetLogin(This,newVal)	\
    (This)->lpVtbl -> put_InternetLogin(This,newVal)

#define ISSCERDA_putref_InternetLogin(This,newVal)	\
    (This)->lpVtbl -> putref_InternetLogin(This,newVal)

#define ISSCERDA_get_InternetPassword(This,pVal)	\
    (This)->lpVtbl -> get_InternetPassword(This,pVal)

#define ISSCERDA_put_InternetPassword(This,newVal)	\
    (This)->lpVtbl -> put_InternetPassword(This,newVal)

#define ISSCERDA_putref_InternetPassword(This,newVal)	\
    (This)->lpVtbl -> putref_InternetPassword(This,newVal)

#define ISSCERDA_get_ErrorRecords(This,pVal)	\
    (This)->lpVtbl -> get_ErrorRecords(This,pVal)

#define ISSCERDA_get_InternetProxyServer(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyServer(This,pVal)

#define ISSCERDA_put_InternetProxyServer(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyServer(This,newVal)

#define ISSCERDA_putref_InternetProxyServer(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyServer(This,newVal)

#define ISSCERDA_get_InternetProxyLogin(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyLogin(This,pVal)

#define ISSCERDA_put_InternetProxyLogin(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyLogin(This,newVal)

#define ISSCERDA_putref_InternetProxyLogin(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyLogin(This,newVal)

#define ISSCERDA_get_InternetProxyPassword(This,pVal)	\
    (This)->lpVtbl -> get_InternetProxyPassword(This,pVal)

#define ISSCERDA_put_InternetProxyPassword(This,newVal)	\
    (This)->lpVtbl -> put_InternetProxyPassword(This,newVal)

#define ISSCERDA_putref_InternetProxyPassword(This,newVal)	\
    (This)->lpVtbl -> putref_InternetProxyPassword(This,newVal)

#define ISSCERDA_Pull(This,LocalTableName,SQLSelectString,OLEDBConnectionString,TrackOption,ErrorTableName)	\
    (This)->lpVtbl -> Pull(This,LocalTableName,SQLSelectString,OLEDBConnectionString,TrackOption,ErrorTableName)

#define ISSCERDA_Push(This,LocalTableName,OLEDBConnectionString,BatchOption)	\
    (This)->lpVtbl -> Push(This,LocalTableName,OLEDBConnectionString,BatchOption)

#define ISSCERDA_SubmitSQL(This,SQLString,OLEDBConnectionString)	\
    (This)->lpVtbl -> SubmitSQL(This,SQLString,OLEDBConnectionString)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_LocalConnectionString_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_LocalConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_LocalConnectionString_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_LocalConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_LocalConnectionString_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_LocalConnectionString_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetURL_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetURL_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetURL_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetURL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_ErrorRecords_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_ErrorRecords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetProxyServer_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetProxyServer_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetProxyServer_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetProxyServer_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetProxyLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetProxyLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetProxyLogin_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetProxyLogin_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCERDA_get_InternetProxyPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [retval][out] */ BSTR __RPC_FAR *pVal);


void __RPC_STUB ISSCERDA_get_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propput] */ HRESULT STDMETHODCALLTYPE ISSCERDA_put_InternetProxyPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_put_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propputref] */ HRESULT STDMETHODCALLTYPE ISSCERDA_putref_InternetProxyPassword_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR newVal);


void __RPC_STUB ISSCERDA_putref_InternetProxyPassword_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCERDA_Pull_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR LocalTableName,
    /* [in] */ BSTR SQLSelectString,
    /* [in] */ BSTR OLEDBConnectionString,
    /* [defaultvalue][in] */ RDA_TRACKOPTION TrackOption,
    /* [defaultvalue][in] */ BSTR ErrorTableName);


void __RPC_STUB ISSCERDA_Pull_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCERDA_Push_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR LocalTableName,
    /* [in] */ BSTR OLEDBConnectionString,
    /* [defaultvalue][in] */ RDA_BATCHOPTION BatchOption);


void __RPC_STUB ISSCERDA_Push_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id] */ HRESULT STDMETHODCALLTYPE ISSCERDA_SubmitSQL_Proxy( 
    ISSCERDA __RPC_FAR * This,
    /* [in] */ BSTR SQLString,
    /* [in] */ BSTR OLEDBConnectionString);


void __RPC_STUB ISSCERDA_SubmitSQL_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCERDA_INTERFACE_DEFINED__ */


#ifndef __ISSCEEngine_INTERFACE_DEFINED__
#define __ISSCEEngine_INTERFACE_DEFINED__

/* interface ISSCEEngine */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_ISSCEEngine;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F169FDA9-47EC-40b3-8794-DFA5BDD13CE1")
    ISSCEEngine : public IDispatch
    {
    public:
        virtual /* [id] */ HRESULT STDMETHODCALLTYPE CompactDatabase( 
            /* [in] */ BSTR SourceConnection,
            /* [in] */ BSTR DestinationConnection) = 0;
        
        virtual /* [id][propget] */ HRESULT STDMETHODCALLTYPE get_ErrorRecords( 
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal) = 0;
        
        virtual /* [hidden] */ HRESULT STDMETHODCALLTYPE CreateDatabase( 
            /* [in] */ BSTR LocalConnection) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISSCEEngineVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISSCEEngine __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISSCEEngine __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfoCount )( 
            ISSCEEngine __RPC_FAR * This,
            /* [out] */ UINT __RPC_FAR *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetTypeInfo )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo __RPC_FAR *__RPC_FAR *ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetIDsOfNames )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR __RPC_FAR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID __RPC_FAR *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Invoke )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS __RPC_FAR *pDispParams,
            /* [out] */ VARIANT __RPC_FAR *pVarResult,
            /* [out] */ EXCEPINFO __RPC_FAR *pExcepInfo,
            /* [out] */ UINT __RPC_FAR *puArgErr);
        
        /* [id] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CompactDatabase )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ BSTR SourceConnection,
            /* [in] */ BSTR DestinationConnection);
        
        /* [id][propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_ErrorRecords )( 
            ISSCEEngine __RPC_FAR * This,
            /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);
        
        /* [hidden] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDatabase )( 
            ISSCEEngine __RPC_FAR * This,
            /* [in] */ BSTR LocalConnection);
        
        END_INTERFACE
    } ISSCEEngineVtbl;

    interface ISSCEEngine
    {
        CONST_VTBL struct ISSCEEngineVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISSCEEngine_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISSCEEngine_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISSCEEngine_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISSCEEngine_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define ISSCEEngine_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define ISSCEEngine_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define ISSCEEngine_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define ISSCEEngine_CompactDatabase(This,SourceConnection,DestinationConnection)	\
    (This)->lpVtbl -> CompactDatabase(This,SourceConnection,DestinationConnection)

#define ISSCEEngine_get_ErrorRecords(This,pVal)	\
    (This)->lpVtbl -> get_ErrorRecords(This,pVal)

#define ISSCEEngine_CreateDatabase(This,LocalConnection)	\
    (This)->lpVtbl -> CreateDatabase(This,LocalConnection)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [id] */ HRESULT STDMETHODCALLTYPE ISSCEEngine_CompactDatabase_Proxy( 
    ISSCEEngine __RPC_FAR * This,
    /* [in] */ BSTR SourceConnection,
    /* [in] */ BSTR DestinationConnection);


void __RPC_STUB ISSCEEngine_CompactDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [id][propget] */ HRESULT STDMETHODCALLTYPE ISSCEEngine_get_ErrorRecords_Proxy( 
    ISSCEEngine __RPC_FAR * This,
    /* [retval][out] */ ISSCEErrors __RPC_FAR *__RPC_FAR *pVal);


void __RPC_STUB ISSCEEngine_get_ErrorRecords_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [hidden] */ HRESULT STDMETHODCALLTYPE ISSCEEngine_CreateDatabase_Proxy( 
    ISSCEEngine __RPC_FAR * This,
    /* [in] */ BSTR LocalConnection);


void __RPC_STUB ISSCEEngine_CreateDatabase_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISSCEEngine_INTERFACE_DEFINED__ */



#ifndef __SSCE_LIBRARY_DEFINED__
#define __SSCE_LIBRARY_DEFINED__

/* library SSCE */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_SSCE;

EXTERN_C const CLSID CLSID_Replication;

#ifdef __cplusplus

class DECLSPEC_UUID("4BF456A0-3B51-4e62-AA9E-7127A84551D0")
Replication;
#endif

EXTERN_C const CLSID CLSID_RemoteDataAccess;

#ifdef __cplusplus

class DECLSPEC_UUID("114E00DD-C185-419e-9610-F4F9126559A7")
RemoteDataAccess;
#endif

EXTERN_C const CLSID CLSID_Engine;

#ifdef __cplusplus

class DECLSPEC_UUID("A4138508-7379-4698-88B7-49132C702184")
Engine;
#endif

EXTERN_C const CLSID CLSID_SSCEErrors;

#ifdef __cplusplus

class DECLSPEC_UUID("444C03B3-490A-48c2-81B6-E77366CB62F9")
SSCEErrors;
#endif

EXTERN_C const CLSID CLSID_SSCEError;

#ifdef __cplusplus

class DECLSPEC_UUID("08505AEA-F293-4750-98AF-3C1B0077FE3F")
SSCEError;
#endif

EXTERN_C const CLSID CLSID_SSCEParams;

#ifdef __cplusplus

class DECLSPEC_UUID("474E7D08-67F7-4433-877D-61AC4F2B5879")
SSCEParams;
#endif

EXTERN_C const CLSID CLSID_SSCEParam;

#ifdef __cplusplus

class DECLSPEC_UUID("3CF68253-1219-48de-975A-FBF161E50EE4")
SSCEParam;
#endif
#endif /* __SSCE_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

unsigned long             __RPC_USER  VARIANT_UserSize(     unsigned long __RPC_FAR *, unsigned long            , VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  VARIANT_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, VARIANT __RPC_FAR * ); 
void                      __RPC_USER  VARIANT_UserFree(     unsigned long __RPC_FAR *, VARIANT __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


