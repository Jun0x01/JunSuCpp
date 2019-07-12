
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars
#include <afxcontrolbars.h>

// JuSuCpp Message
#define JUN_WM_REFRESH_WORKSPACEVIEW  (WM_USER+200)
#define JUN_WM_REFRESH_LAYERSVIEW     (WM_USER+201)
#define JUN_WM_REFRESH_MAPVIEW        (WM_USER+202)
#define JUN_WM_SCENCE_PAUSE           (WM_USER+203)
#define JUN_WM_SCENCE_RESUME          (WM_USER+204)
#define JUN_WM_OUTPUT_LOG             (WM_USER+205)

#define JUN_WM_PARAM_MAPLAYERS        (0x01)
#define JUN_WM_PARAM_SCENELAYERS      (JUN_WM_PARAM_MAPLAYERS + 0x01)









