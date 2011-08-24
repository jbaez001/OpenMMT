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
#ifndef OPENMMT_WINDOWS_WINDOWS_H
#define OPENMMT_WINDOWS_WINDOWS_H
#pragma once

extern BOOL WndRegisterClass(LPCWSTR lpszClassName, WNDPROC lpfnWndProc);
extern BOOL WndExtendFrame(HWND hWnd);
extern BOOL WndSetBlur(HWND hWnd, bool bEnable);
extern BOOL WndShouldDisplay(HWND hWnd);
extern BOOL WndOpenMMTWindow(HWND hWnd);
extern BOOL WndGetWindowsTaskbarHandle();
extern BOOL WndCheckAndDestroy(HWND hWnd);
extern HICON WndGetApplicationIcon(HWND hWnd);

extern void WndStartDeviceMonitor();
extern void WndStopDeviceMonitor();
extern void WndGetWindowsTaskbarSize();
extern void WndRunMessageThread();

#endif // OPENMMT_WINDOWS_WINDOWS_H

// EOF
