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
#include "openmmt/option_variables.h"
#include "openmmt/windows/windows.h"
#include "openmmt/ui/settings_dlg.h"

LRESULT DlgSettings::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
  CenterWindow();

  m_btnSetThumbnails = GetDlgItem(IDC_CHECK_THUMBNAILS);
  m_btnSetTrayIcon   = GetDlgItem(IDC_CHECK_TRAY_ICON);

  if (g_bOptions_EnableThumbnails)
    m_btnSetThumbnails.SetCheck(1);

  if (g_bOptions_EnableTrayIcon)
    m_btnSetTrayIcon.SetCheck(1);

  return TRUE;
}

LRESULT DlgSettings::OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  this->OnApply(wNotifyCode, wID, hWndCtl, bHandled);
  EndDialog(IDOK);

  return 0;
}

LRESULT DlgSettings::OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  if ((m_btnSetThumbnails.GetCheck() == 0) && (g_bOptions_EnableThumbnails)) {
    g_bOptions_EnableThumbnails = FALSE;
  } else if ((m_btnSetThumbnails.GetCheck()) && (!g_bOptions_EnableThumbnails)) {
    g_bOptions_EnableThumbnails = TRUE;
  }

  if ((m_btnSetTrayIcon.GetCheck() == 0) && (g_bOptions_EnableTrayIcon)) {
    g_bOptions_EnableTrayIcon = FALSE;
    WndTrayIconRemove();
  } else if ((m_btnSetTrayIcon.GetCheck()) && (!g_bOptions_EnableTrayIcon)) {
    g_bOptions_EnableTrayIcon = TRUE;
    WndTrayIconAdd();
  }

  return 0;
}

LRESULT DlgSettings::OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
  EndDialog(IDCANCEL);
  return 0;
}

// EOF

