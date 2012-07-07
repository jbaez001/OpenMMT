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
#ifndef OPENMMT_WINDOWS_TASKBAR_INTERFACE_H
#define OPENMMT_WINDOWS_TASKBAR_INTERFACE_H
#pragma once

typedef struct {
  SHORT left;
  SHORT top;
  SHORT right;
  SHORT bottom;
} REALRECT, *LPREALRECT;

typedef struct {
  HWND hWnd;
  REALRECT rc;
} DOCUMENT_ME_RIGHT_MICROSOFT, *LPDOCUMENT_ME_RIGHT_MICROSOFT;

class WinTaskbarInterface
{
public:
  WinTaskbarInterface();
  ~WinTaskbarInterface();

  HRESULT AddButton(HWND hWnd);
  HRESULT RemoveButton(HWND hWnd);
  BOOL    HandleGetMinRect(HWND hWnd, LPREALRECT lpRect);

private:
  ITaskbarList2 *g_iTaskbar2;

};

#endif // OPENMMT_WINDOWS_TASKBAR_INTERFACE_H

// EOF

