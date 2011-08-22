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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"
#include "openmmt/resource.h"
#include "openmmt/taskbar/taskbar_event.h"
#include "openmmt/windows/windows.h"

void TaskbarEvent::CheckAppBarMsg(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(hWnd);
  UNREFERENCED_PARAMETER(uMsg);
  UNREFERENCED_PARAMETER(wParam);
  UNREFERENCED_PARAMETER(lParam);
}

void TaskbarEvent::OnActivate(HWND hWnd)
{
  UNREFERENCED_PARAMETER(hWnd);
}

void TaskbarEvent::OnEraseBackground(HWND hWnd)
{
  TaskbarPtr bar(g_pMonitorManager->FindMonitorTaskbar(hWnd));

  if (bar == TaskbarPtr())
    return;

  bar->OnEraseBackground();
}

void TaskbarEvent::OnContextMenu(HWND hWnd, LPRECT lpRect, POINT pt)
{

  UNREFERENCED_PARAMETER(lpRect);

  HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU_BAR));
  
  if (!hMenu)
    return;

  HMENU hPopUpMenu = GetSubMenu(hMenu, 0);

  if (!hPopUpMenu)
    return;

  #if 0
  TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar(hWnd);

  if (bar != TaskbarPtr()) {
    MENUITEMINFO mInfo = {0};

    mInfo.cbSize = sizeof(MENUITEMINFO);
    mInfo.fMask  = MIIM_STATE;
    mInfo.fState = MFS_CHECKED;
   
    if (bar->IsPosition(TASKBAR_LEFT))
      SetMenuItemInfo(hPopUpMenu, ID_POSITION_LEFT, FALSE, &mInfo);
    else if (bar->IsPosition(TASKBAR_TOP))
      SetMenuItemInfo(hPopUpMenu, ID_POSITION_TOP, FALSE, &mInfo);
    else if (bar->IsPosition(TASKBAR_RIGHT))
      SetMenuItemInfo(hPopUpMenu, ID_POSITION_RIGHT, FALSE, &mInfo); 
    else
      SetMenuItemInfo(hPopUpMenu, ID_POSITION_BOTTOM, FALSE, &mInfo);

    if (bar->IsAutoHidden())
      SetMenuItemInfo(hMenu, ID_TASKBARPOPUP_AUTOHIDE, FALSE, &mInfo);
     
  }
#endif

  ClientToScreen(hWnd, &pt);
  TrackPopupMenu(hPopUpMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, 0, 
    hWnd, NULL);
  DestroyMenu(hPopUpMenu);
}

void TaskbarEvent::OnThemeChange(HWND hWnd)
{
  TaskbarPtr bar(g_pMonitorManager->FindMonitorTaskbar(hWnd));

  if (bar == TaskbarPtr())
    return;

  bar->OnThemeChange();
  bar->OnEraseBackground();
}

void TaskbarEvent::OnWindowMinMax(HWND hWnd, UINT nShowCmd)
{

}

void TaskbarEvent::OnWindowPosChanged(HWND hWnd)
{
  
  RECT rt = {0};

  GetWindowRect(hWnd, &rt);

  LONG lWidth  = rt.right - rt.left;
  LONG lHeight = rt.bottom - rt.top;

  MonitorPtr mon = g_pMonitorManager->FindMonitor(hWnd);

  if (mon != MonitorPtr()) {
    TaskbarPtr bar = mon->GetTaskbar();

    if (bar == TaskbarPtr())
      return;

    if ((mon->GetWidth() == lWidth) && (mon->GetHeight() == lHeight))
      bar->AppEnterFullScreen(hWnd);
    else
      bar->AppLeaveFullScreen(hWnd);
  }
}

void TaskbarEvent::OnWindowMinimize(HWND hWnd)
{
  ApplicationPtr pApp(g_pAppManager->FindApplication(hWnd));

  if (pApp == ApplicationPtr())
    return;

  pApp->Update();

  ButtonPtr pBtn(pApp->GetTaskbarButton());

  // Check to see if the button is currently active. If so, clear the
  // button's state and re-draw it.
  if (pBtn != ButtonPtr()) {
    if (pBtn->IsState(BTN_ACTIVE)) {
      pBtn->ClearState(BTN_ACTIVE);
      pBtn->OnPaint();
    }
  }
}

// EOF
