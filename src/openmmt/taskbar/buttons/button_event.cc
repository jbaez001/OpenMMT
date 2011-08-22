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
#include "openmmt/option_variables.h"
#include "openmmt/resource.h"
#include "openmmt/taskbar/buttons/button_event.h"
#include "openmmt/thumbnail/thumbnail.h"

ButtonPtr ButtonEvent::FindButton(HWND hWnd)
{
  TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar(hWnd);

  if (bar != TaskbarPtr()) {
    return bar->GetButton(hWnd);
  }
  return ButtonPtr();
}

void ButtonEvent::OnEraseBackground(HWND hWnd)
{
  ButtonPtr btn = FindButton(hWnd);
  if (btn == ButtonPtr()) 
    return;
  
  btn->OnEraseBackground();
}

void ButtonEvent::OnPaint(HWND hWnd)
{
  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  btn->OnPaint();
}

void ButtonEvent::OnMouseMove(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  btn->OnMouseMove();
}

void ButtonEvent::OnMouseHoover(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  if (g_bOptions_EnableThumbnails) {
    ButtonPtr btn = FindButton(hWnd);

    if (btn == ButtonPtr())
      return;

    if (!g_pAppManager->IsThumbnailRegistered()) {
      TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar(btn->GetTaskbarHandle());

      if (bar == TaskbarPtr())
        return;

      POINT pt = {0};

      pt.x = btn->GetX();
      pt.y = btn->GetY();
      
      if (bar->IsPosition(TASKBAR_BOTTOM))
        pt.y -=(175+8);
      else if (bar->IsPosition(TASKBAR_TOP))
        pt.y +=(40+8);
      else if (bar->IsPosition(TASKBAR_RIGHT))
        pt.x -= 235+8;
      else if (bar->IsPosition(TASKBAR_LEFT))
        pt.x = 60+8;

      if (pt.x == 0)
        pt.x = 5;
      if (pt.y == 0)
        pt.y = 5;

      ClientToScreen(btn->GetTaskbarHandle(), &pt);
      g_pAppManager->CreateThumbnailWindow(btn->GetAppHandle(), pt.x, pt.y);
    }
    btn->OnMouseHoover();
  }
}

void ButtonEvent::OnMouseLeave(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  if (g_pAppManager->IsThumbnailRegistered())
    g_pAppManager->CloseThumbnailWindow();

  btn->OnMouseLeave();
}

void ButtonEvent::OnMouseBeginLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  btn->AddState(BTN_PRESSED);
}

void ButtonEvent::OnMouseEndLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  // If the button is that of the selected app, minimize it.
  if (btn->IsState(BTN_ACTIVE)) {
    ShowWindowAsync(btn->GetAppHandle(), SW_MINIMIZE);
    btn->ClearState(BTN_ACTIVE);
  } else {
    // Is the window is minimized, let's restore it.
    if (IsIconic(btn->GetAppHandle()))
      ShowWindow(btn->GetAppHandle(), SW_RESTORE);

    // Bring the window to the foreground.
    SetForegroundWindow(btn->GetAppHandle());
    btn->AddState(BTN_ACTIVE);
  }
  btn->AddState(BTN_NORMAL);
}

void ButtonEvent::OnWindowDestroyed(HWND hWnd)
{
  UNREFERENCED_PARAMETER(hWnd);
}

void ButtonEvent::OnThemeChange(HWND hWnd)
{
  ButtonPtr btn = FindButton(hWnd);

  if (btn == ButtonPtr())
    return;

  btn->OnThemeChange();
}

void ButtonEvent::OnContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam, POINT pt, BOOL bShifted)
{
  if (bShifted) {
    TaskbarPtr pBar(g_pMonitorManager->FindMonitorTaskbar(hWnd));

    if (pBar != TaskbarPtr()) {
      ButtonPtr pBtn(pBar->GetButton(hWnd));
      if (pBtn != ButtonPtr()) {
        SetWindowPos(pBtn->GetAppHandle(), 0, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
        PostMessage(pBtn->GetAppHandle(), 0x0313, 0, lParam);
        pBtn->Persist();
        return;
      }
    }
  }

  HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU_BTN));
  
  if (!hMenu)
    return;

  HMENU hPopUpMenu = GetSubMenu(hMenu, 0);

  if (!hPopUpMenu)
    return;

  ClientToScreen(hWnd, &pt);
  TrackPopupMenu(hPopUpMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y, 0, 
    hWnd, NULL);
  DestroyMenu(hPopUpMenu);
}
// EOF

