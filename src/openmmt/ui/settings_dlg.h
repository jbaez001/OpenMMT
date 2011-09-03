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
#ifndef OPENMMT_UI_SETTINGS_DLG_H
#define OPENMMT_UI_SETTINGS_DLG_H

#include "openmmt/resource.h"
#include <atlddx.h>

class DlgSettings : public CDialogImpl<DlgSettings>,
                    public CWinDataExchange<DlgSettings>
{
public:
  enum { IDD = IDD_OPENMMT_DIALOG };

  BEGIN_MSG_MAP(DlgAbout)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    COMMAND_ID_HANDLER(IDOK, OnOk)
    COMMAND_ID_HANDLER(IDAPPLY, OnApply)
    COMMAND_ID_HANDLER(IDCANCEL, OnClose)
  END_MSG_MAP()

  LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);
  LRESULT OnOk(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
  LRESULT OnApply(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);
  LRESULT OnClose(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL &bHandled);

private:
  CButton m_btnSetThumbnails;
  CButton m_btnSetTrayIcon;
};

#endif // OPENMMT_UI_SETTINGS_DLG_H

// EOF

