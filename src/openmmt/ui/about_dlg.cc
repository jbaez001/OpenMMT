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
#include "openmmt/precompiled_headers.h"
#include "openmmt/ui/about_dlg.h"

LRESULT DlgAbout::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDR_MAINFRAME));
  
  this->SetIcon(hIcon);
  CenterWindow();
  return TRUE;
}

LRESULT DlgAbout::OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  EndDialog(IDCANCEL);
  return 0;
}

// EOF

