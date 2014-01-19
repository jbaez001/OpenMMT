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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"
#include "openmmt/option_variables.h"
#include "openmmt/resource.h"
#include "openmmt/taskbar/buttons/button_event.h"
#include "openmmt/thumbnail/thumbnail.h"

ButtonPtr ButtonEvent::FindButton(HWND hWnd)
{
  TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar(hWnd);

  if (bar) {
    return bar->GetButton(hWnd);
  }

  return NULL;
}

void ButtonEvent::OnEraseBackground(HWND hWnd)
{
  ButtonPtr btn(FindButton(hWnd));

  if (btn) 
    btn->OnEraseBackground();
  
}

void ButtonEvent::OnPaint(HWND hWnd)
{
  ButtonPtr btn(FindButton(hWnd));

  if (btn)
    btn->OnPaint();

}

void ButtonEvent::OnMouseMove(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn(FindButton(hWnd));

  if (btn)
    btn->OnMouseMove();

}

void ButtonEvent::OnMouseHoover(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  if (g_bOptions_EnableThumbnails) {
    ButtonPtr btn(FindButton(hWnd));

    if (!btn)
      return;
    
    if (g_pThumbnailManager->isThumbnailPresent()) {
      if (g_pThumbnailManager->GetThumbnailedButton() != btn->GetButtonHandle()) {
        g_pThumbnailManager->DestroyThumbnail();
        g_pThumbnailManager->CreateThumbnail(btn);
      }
    } else {
      g_pThumbnailManager->CreateThumbnail(btn);
    }

    btn->OnMouseHoover(); 
  }
}

void ButtonEvent::OnMouseLeave(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn(FindButton(hWnd));

  if (!btn)
    return;

  if (g_pThumbnailManager->isThumbnailed(btn)) {
    SetTimer(g_pThumbnailManager->GetThumbnailWindow(), 
      IDT_THUMBNAIL_DESTROY_TIMER, g_iOptions_ThumbnailDestroyTimer, NULL);
  }

  btn->OnMouseLeave();
}

void ButtonEvent::OnMouseBeginLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);

  ButtonPtr btn(FindButton(hWnd));

  if (btn)
    btn->AddState(BTN_PRESSED);
}

void ButtonEvent::OnMouseEndLeftClick(HWND hWnd, HDC hDC, LPRECT lpRect)
{
  UNREFERENCED_PARAMETER(hDC);
  UNREFERENCED_PARAMETER(lpRect);


  ButtonPtr btn(FindButton(hWnd));
  TaskbarPtr pTaskbar(g_pMonitorManager->FindMonitorTaskbar(hWnd));
  BOOL bFullScreen = FALSE;

  if (pTaskbar) {
    if ((pTaskbar->AppIsFullScreen()) && (pTaskbar->GetFullScreenApp() == btn->GetAppHandle()))
      bFullScreen = TRUE;
  }

  if (!btn)
    return;

  // If the button is that of the selected app, minimize it.
  if (btn->IsState(BTN_ACTIVE)) {
    SetForegroundWindow(btn->GetAppHandle());
    if (!bFullScreen) {
      PostMessage(btn->GetAppHandle(), WM_SYSCOMMAND, SC_MINIMIZE, MAKELPARAM(0, 0));
      btn->ClearState(BTN_ACTIVE);
    }
  } else {
    // If the window is minimized, let's restore it.
    if (IsIconic(btn->GetAppHandle()))
      ShowWindow(btn->GetAppHandle(), SW_RESTORE);

    // Bring the window to the foreground.
    BringWindowToTop(btn->GetAppHandle());
    RedrawWindow(btn->GetAppHandle(), NULL, NULL, RDW_INVALIDATE);

    btn->AddState(BTN_ACTIVE);
  }
  btn->AddState(BTN_NORMAL);

  if (g_pThumbnailManager->isThumbnailed(btn))
    g_pThumbnailManager->DestroyThumbnail();

  // Cancel Hover.
  TRACKMOUSEEVENT mousey = {0};
  mousey.cbSize = sizeof(TRACKMOUSEEVENT);
  mousey.dwFlags = TME_CANCEL|TME_HOVER;
  mousey.hwndTrack = btn->GetButtonHandle();
  _TrackMouseEvent(&mousey);
}

void ButtonEvent::OnWindowDestroyed(HWND hWnd)
{
  UNREFERENCED_PARAMETER(hWnd);
}

void ButtonEvent::OnThemeChange(HWND hWnd)
{
  ButtonPtr btn(FindButton(hWnd));

  if (btn)
    btn->OnThemeChange();
}

void ButtonEvent::OnContextMenu(HWND hWnd, WPARAM wParam, LPARAM lParam, POINT pt, BOOL bShifted)
{

}
// EOF

