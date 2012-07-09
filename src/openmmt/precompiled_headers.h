/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010-2012 Genscripts
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *  
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#define STRICT
#define _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_SECURE_NO_WARNINGS

#include "targetver.h"

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>

// Misc Windows Headers 
#include <dwmapi.h>  
#include <objidl.h>
#include <gdiplus.h>
#include <shellapi.h>
#include <shobjidl.h>

// STL
#include <vector>
#include <map>

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
#pragma comment( lib, "Wtsapi32.lib")   // WTSRegisterSessionNotification
// EOF

