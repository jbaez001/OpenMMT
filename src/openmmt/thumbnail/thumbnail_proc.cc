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
#include "openmmt/windows/windows.h"

LRESULT CALLBACK ThumbnailProc(HWND hWnd, UINT msg, WPARAM wParam, 
                               LPARAM lParam)
{
  switch(msg)
  {
  case WM_ERASEBKGND:
    {
      RECT rc = {0};
      GetClientRect(hWnd, &rc);

      if (IsCompositionActive()) {
        WndSetBlur(hWnd, true);
        WndExtendFrame(hWnd);

        HBRUSH brush = CreateSolidBrush(RGB(0,0,0));
        FillRect((HDC)wParam, &rc, brush);

        DeleteObject(brush);
      }
    }
    return TRUE;
  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

// EOF
