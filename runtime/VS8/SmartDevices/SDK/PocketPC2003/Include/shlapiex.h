//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
//
// Use of this source code is subject to the terms of the Microsoft end-user
// license agreement (EULA) under which you licensed this SOFTWARE PRODUCT.
// If you did not accept the terms of the EULA, you are not authorized to use
// this source code. For a copy of the EULA, please see the LICENSE.RTF on your
// install media.
//

#ifndef _INC_SHELLAPI_EX
#define _INC_SHELLAPI_EX


//
// Define API decoration for direct importing of DLL references.
//
#ifndef WINSHELLAPI
#if !defined(_SHELL32_)
#define WINSHELLAPI DECLSPEC_IMPORT
#else
#define WINSHELLAPI
#endif
#endif // WINSHELLAPI


#include <pshpack1.h>

#ifdef __cplusplus
extern "C" {            /* Assume C declarations for C++ */
#endif  /* __cplusplus */

#include <shsdkstc.h>
BOOL WINAPI SHChangeNotifyRegisterI( HWND hwnd, SHCHANGENOTIFYENTRY *pshcne );
BOOL WINAPI SHFileNotifyFreeI( FILECHANGENOTIFY *pfcn );
BOOL WINAPI SHFileNotifyRemoveI( HWND hwnd );

#ifdef __cplusplus
}
#endif  /* __cplusplus */

#include <poppack.h>

#endif  /* _INC_SHELLAPI_EX */
