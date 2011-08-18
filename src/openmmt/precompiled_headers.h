
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define STRICT

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

// Misc Windows Headers
#include <tchar.h>   
#include <dwmapi.h>  
#include <commctrl.h>
#include <objidl.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <shobjidl.h>

// STL
#include <vector>
#include <map>

// Boost
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>

// OpenMMT Pointer Types
#include "openmmt/pointer_types.h"

#ifdef _DEBUG
# include "mlib/mlib.hpp"
#endif

// Surely there is a better way of doing this without breaking VS++.
#ifdef _DEBUG
# define dprintf(format, ...)       \
  {                                 \
  printf("[%s] ", __FUNCTION__);   \
  printf(format, ##__VA_ARGS__);  \
  }
#else
# define dprintf(format, ...)
#endif

// Libraries needed.
#pragma comment( lib, "Dwmapi.lib" )    // Desktop Windows Manager API Library
#pragma comment( lib, "UxTheme.lib" )   // uxtheme
#pragma comment( lib, "comctl32.lib" )  // Common Control Classes
#pragma comment( lib, "D2d1.lib" )      // Direct 2D

// EOF

