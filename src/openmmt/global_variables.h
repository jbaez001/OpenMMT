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
#ifndef OPENMMT_GLOBAL_VARIABLES_H
#define OPENMMT_GLOBAL_VARIABLES_H
#pragma once

#include "openmmt/applications/application_manager.h"
#include "openmmt/monitors/monitor_manager.h"

extern HINSTANCE   g_hInstance;

extern MonitorManager*     g_pMonitorManager;
extern ApplicationManager* g_pAppManager;

extern const wchar_t* g_OpenMMTClassName;
extern const wchar_t* g_TaskbarClassName;
extern const wchar_t* g_ThumbnailClassname;
extern const wchar_t* g_TaskbarButtonClassName;

extern wchar_t g_strExePath[MAX_PATH];

extern const GUID g_MonitorGUID;

extern HWND g_hWndOpenMMT;
extern HWND g_hWndHelper;
extern HWND g_hWndWindowsTaskbar;

extern INT g_WindowsTaskbarWidth;
extern INT g_WindowsTaskbarHeight;
extern INT g_WindowsTaskbarPos;

extern UINT g_uMsgShellHook;
extern UINT g_uiAppBarMsg;

extern BOOL g_bUsingSmallIcons;

#endif // OPENMMT_GLOBAL_VARIABLES_H

// EOF 
