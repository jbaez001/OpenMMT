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
#include "openmmt/global_variables.h"
#include "openmmt/resource.h"
#include "openmmt/taskbar/taskbar_event.h"
#include "openmmt/taskbar/buttons/button_event.h"

LRESULT CALLBACK ButtonProc(HWND hWnd, UINT msg, WPARAM wParam, 
                            LPARAM lParam)
{
  switch(msg)
  {
    // All rendering will be handled by WM_PAINT.
  case WM_ERASEBKGND:
    return TRUE;

  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;

  case WM_DESTROY:
      ButtonEvent::OnWindowDestroyed(hWnd);
    break;

  case WM_PAINT:
    {
      PAINTSTRUCT ps;
      BeginPaint(hWnd, &ps);
      ButtonEvent::OnPaint(hWnd);
      EndPaint(hWnd, &ps);
    }
    break;

  case WM_MOUSEMOVE:
      ButtonEvent::OnMouseMove(hWnd, (HDC)wParam, NULL);
    break;

  case WM_LBUTTONDOWN:
      ButtonEvent::OnMouseBeginLeftClick(hWnd, (HDC)wParam, NULL);
    break;

  case WM_LBUTTONUP:
      ButtonEvent::OnMouseEndLeftClick(hWnd, (HDC)wParam, NULL);
    break;

  case WM_CONTEXTMENU:
    {
      ButtonPtr pBtn(g_pAppManager->FindButton(hWnd));
      
      if (pBtn == ButtonPtr())
        break;

      SetWindowPos(pBtn->GetAppHandle(), 0, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOMOVE);
      PostMessage(pBtn->GetAppHandle(), 0x0313, 0, lParam);
    }
    break;

  case WM_MOUSEHOVER:
    {
      // If the user is holding the shift key, we will bypass any mouse
      // hovering events for now. 
      if (!(GetKeyState(VK_SHIFT) & 0x8000))
        ButtonEvent::OnMouseHoover(hWnd, (HDC)wParam, NULL);
    }
    break;

  case WM_MOUSELEAVE:
      ButtonEvent::OnMouseLeave(hWnd, (HDC)wParam, NULL);;
    break;

  case WM_THEMECHANGED:
  case WM_DWMCOMPOSITIONCHANGED:
      ButtonEvent::OnThemeChange(hWnd);
    break;

  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

// EOF
