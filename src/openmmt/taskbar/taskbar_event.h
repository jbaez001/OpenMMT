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
#ifndef OPENMMT_TASKBAR_TASKBAR_EVENT_H
#define OPENMMT_TASKBAR_TASKBAR_EVENT_H
#pragma once

class TaskbarEvent
{
public:
  static void CheckAppBarMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  static void OnActivate(HWND hWnd);
  static void OnEraseBackground(HWND hWnd);
  static void OnContextMenu(HWND hWnd, LPRECT lpRect, POINT pt);
  static void OnThemeChange(HWND hWnd);
  static void OnWindowMinMax(HWND hWnd, UINT nShowCmd);
  static void OnWindowPosChanged(HWND hWnd);
  static void OnWindowMinimize(HWND hWnd);
};

#endif // OPENMMT_TASKBAR_TASKBAR_EVENT_H

// EOF

