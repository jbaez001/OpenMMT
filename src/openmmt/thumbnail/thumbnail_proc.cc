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
#include "openmmt/global_variables.h"
#include "openmmt/option_variables.h"
#include "openmmt/taskbar/buttons/button_event.h"
#include "openmmt/thumbnail/thumbnail.h"
#include "openmmt/windows/windows.h"

BOOL PtIsInBounds(LPPOINT lpPoints, LPRECT lpRect)
{
  if ((lpPoints->x >= lpRect->left) && (lpPoints->x <=lpRect->right) &&
    (lpPoints->y >= lpRect->top) && (lpPoints->y <= lpRect->bottom))
    return TRUE;

  return FALSE;
}

LRESULT CALLBACK ThumbnailProc(HWND hWnd, UINT msg, WPARAM wParam, 
                               LPARAM lParam)
{
  switch(msg)
  {
  case WM_ERASEBKGND:
    {
      if (IsCompositionActive()) {
        WndSetBlur(hWnd, true);
        WndExtendFrame(hWnd);
      }
    }
    return TRUE;

  case WM_TIMER:
    {
      POINT pt = {0};
      GetCursorPos(&pt);

      RECT thumbRect = {0};
      RECT btnRect   = {0};

      GetWindowRect(hWnd, &thumbRect);
      GetWindowRect(g_pThumbnailManager->GetThumbnailedButton(), &btnRect);

      if (!PtIsInBounds(&pt, &thumbRect) && !PtIsInBounds(&pt, &btnRect)) {
        KillTimer(hWnd, IDT_THUMBNAIL_DESTROY_TIMER);
        g_pThumbnailManager->DestroyThumbnail();
      }
    }
    return 0;

  case WM_LBUTTONUP:
    {
      TaskbarPtr pTaskbar(g_pMonitorManager->FindMonitorTaskbar(g_pThumbnailManager->GetThumbnailedButton()));

      if (pTaskbar == TaskbarPtr())
        break;

      ButtonPtr pBtn(pTaskbar->GetButton(g_pThumbnailManager->GetThumbnailedButton()));
      
      if (pBtn == ButtonPtr())
        break;

      if (!pBtn->IsState(BTN_ACTIVE))
        ButtonEvent::OnMouseEndLeftClick(pBtn->GetButtonHandle(), NULL, NULL);
      else
        g_pThumbnailManager->DestroyThumbnail();
    }
    break;

  case WM_MOUSELEAVE:
      SetTimer(hWnd, IDT_THUMBNAIL_DESTROY_TIMER, g_iOptions_ThumbnailDestroyTimer, NULL);
    break;

  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;

  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

// EOF
