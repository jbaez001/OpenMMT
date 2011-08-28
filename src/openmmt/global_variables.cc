/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010-2011 Genscripts
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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"

HINSTANCE     g_hInstance = NULL;

MonitorManager      *g_pMonitorManager = NULL;
ApplicationManager  *g_pAppManager     = NULL;
WinTaskbarInterface *g_pWinTaskbar     = NULL;

const wchar_t *g_OpenMMTClassName       = L"OpenMMT";
const wchar_t *g_TaskbarClassName       = L"OpenMMT_Taskbar";
const wchar_t *g_ThumbnailClassname     = L"OpenMMT_Thumbnail";
const wchar_t *g_TaskbarButtonClassName = L"OpenMMT_Button";

wchar_t g_strExePath[MAX_PATH] = {0};

// Monitor class GUID.
// E6F07B5F-EE97-4a90-B076-33F57BF4EAA7
const GUID g_MonitorGUID = { 0xe6f07b5f, 0xee97, 0x4a90, 0xb0, 0x76, 0x33, 0xf5, 0x7b, 0xf4, 0xea, 0xa7 };

HWND g_hWndWindowsTaskbar = {0};
HWND g_hWndOpenMMT        = {0};
HWND g_hWndHelper         = {0};

INT g_WindowsTaskbarWidth  = 0;
INT g_WindowsTaskbarHeight = 0;
INT g_WindowsTaskbarPos    = 0;

UINT g_uMsgShellHook       = 0;
UINT g_uiAppBarMsg         = 0;

BOOL g_bUsingSmallIcons    = FALSE;

// EOF
