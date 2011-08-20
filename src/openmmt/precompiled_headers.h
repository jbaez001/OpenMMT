
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define STRICT

#include "targetver.h"

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

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
#pragma comment( lib, "Shell32.lib" )   // ShellExecute
#pragma comment( lib, "Gdi32.lib" )     // GDI+
// EOF

