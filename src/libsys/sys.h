/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010 Genscripts
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
#ifndef SYSTEMHOOKS_SYS_H
#define SYSTEMHOOKS_SYS_H

#include "openmmt/targetver.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern BOOL  g_bHooksInstalled;
extern HHOOK g_hHookProc;
extern HHOOK g_hHookCbt;
extern HWND  g_hWndOpenMMT;
extern HWND  g_hWndHelper;


extern LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam);
extern LRESULT CALLBACK CbtRetProc(int nCode, WPARAM wParam, LPARAM lParam);

#endif // SYSTEMHOOKS_SYS_H

//EOF

