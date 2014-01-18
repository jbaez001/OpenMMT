/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010-2014 Genscripts
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
#ifndef OPENMMT_TASKBAR_BUTTONS_BUTTON_EVENT_H
#define OPENMMT_TASKBAR_BUTTONS_BUTTON_EVENT_H
#pragma once

/**
 * Button events.
 */
class ButtonEvent
{
public:
  static ButtonPtr FindButton(HWND hWnd);
  static void OnEraseBackground(HWND hWnd);
  static void OnPaint(HWND hWnd);
  static void OnMouseMove(HWND hWnd, HDC hDC, LPRECT lpRect);
  static void OnMouseHoover(HWND hWnd, HDC hDC, LPRECT lpRect);
  static void OnMouseLeave(HWND hWnd, HDC hDC, LPRECT lpRect);
  static void OnMouseBeginLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect);
  static void OnMouseEndLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect);
  static void OnWindowDestroyed(HWND hWnd);
  static void OnThemeChange(HWND hWnd);
  static void OnContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam, POINT pt, BOOL bShifted);
};

#endif // OPENMMT_TASKBAR_BUTTONS_BUTTON_EVENT_H

// EOF

