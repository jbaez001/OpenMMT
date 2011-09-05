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
#include "libsys/sys.h"
#include "libsys/api.h"
#include "openmmt/messages.h"
#include <cmath>
#include <stdio.h>

// Global declarations
#pragma data_seg(".SHARDATA")
  BOOL  g_bHooksInstalled     = FALSE;
  BOOL  g_bWindowsSnap        = TRUE;
  INT32 g_iWindowSnapMargin   = 100;
  HHOOK g_hHookProc   = {0};
  HHOOK g_hWindowSnap = {0};
  HWND  g_hWndOpenMMT = {0};
  HWND  g_hWndHelper  = {0};
#pragma data_seg()

#pragma comment(linker, "/SECTION:.SHARDATA,RWS")

// Taken from openmmt/windows.cc
// TODO: Create a generic "base" library and import this from it.
static BOOL WndShouldDisplay(HWND hWnd)
{
  if (!hWnd || !IsWindowVisible(hWnd) || GetParent(hWnd))
    return FALSE;

  LONG_PTR dwExFlags = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
  HWND hWndOwner = GetWindow(hWnd, GW_OWNER);

  if (((!(dwExFlags & WS_EX_TOOLWINDOW)) && (!hWndOwner)) ||
    (dwExFlags & WS_EX_APPWINDOW) && (hWndOwner))
    return TRUE;

  if (hWndOwner && (WS_EX_CONTROLPARENT))
    return TRUE;

  return FALSE;
}

static void SendTaskbarMsg(UINT mMsg, HWND hWnd, LPARAM lParam = NULL)
{
  // If a window is being destroyed or has been destroyed, chances are
  // that the window is not going to be visible anymore.
  if ((mMsg != TASKBAR_WINDOW_DESTROY) && (mMsg != TASKBAR_WINDOW_DESTROY) &&
    (!WndShouldDisplay(hWnd)))
    return;

  // Check to see if OpenMMT is still running, if not terminate the hooks.
  if (!IsWindow(g_hWndOpenMMT)) {
    SysHooksStop();

    // Check to see if the helper is still running, if so, tell it to die.
    if (g_hWndHelper && IsWindow(g_hWndHelper))
      PostMessage(g_hWndHelper, WM_QUIT, NULL, NULL);

    return;
  }

  // Note that we use PostMessage instead of SendMessage.
  PostMessage(g_hWndOpenMMT, mMsg, (WPARAM)hWnd, lParam);
}


// Window Snaps, oh my!
static BOOL IsCloseToEdge(LONG iPos, LONG iEdge) {
  return (abs(iPos - iEdge) < g_iWindowSnapMargin);
}

LRESULT CALLBACK CallWndRetProc(int nCode, WPARAM wParam, LPARAM lParam)
{
  if (nCode >= 0) {
    PCWPRETSTRUCT lpCwp = (PCWPRETSTRUCT)lParam;

    switch (lpCwp->message)
    {
    case WM_CREATE:
        SendTaskbarMsg(TASKBAR_WINDOW_ACTIVATE, lpCwp->hwnd);
        break;
         
    case WM_ACTIVATE:
      {
        // If the window is being deactivated, then lets not bother
        // openmmt with it.
        if (LOWORD(lpCwp->wParam) == WA_INACTIVE)
          break;

        SendTaskbarMsg(TASKBAR_WINDOW_ACTIVATE, lpCwp->hwnd);
      }
      break;

    case WM_WINDOWPOSCHANGED:
        SendTaskbarMsg(TASKBAR_WINDOW_POSCHANGED, lpCwp->hwnd, lpCwp->lParam);
      break;

    case WM_DESTROY:
    case WM_CLOSE:
        SendTaskbarMsg(TASKBAR_WINDOW_DESTROY, lpCwp->hwnd);
      break;

    case WM_SYSCOMMAND:
      {
        switch (lpCwp->wParam)
        {
        case SC_MINIMIZE:
            SendTaskbarMsg(TASKBAR_WINDOW_MINIMIZE, lpCwp->hwnd);
          break;

        case SC_MAXIMIZE:
          SendTaskbarMsg(TASKBAR_WINDOW_MAXIMIZE, lpCwp->hwnd);
          break;
        }
      }
      break;

    case WM_SETFOCUS:
        SendTaskbarMsg(TASKBAR_WINDOW_SETFOCUS, lpCwp->hwnd);
      break;
      
    case WM_ACTIVATEAPP:
      {
        if ((BOOL)lpCwp->wParam == FALSE)
          SendTaskbarMsg(TASKBAR_WINDOW_KILLFOCUS, lpCwp->hwnd);
      }
      break;

      /*
    case WM_MOVE:
      {
        RECT rc = {0};
        RECT workRect = {0};

        if (!GetWindowRect(lpCwp->hwnd, &rc) ||
            !SystemParametersInfo(SPI_GETWORKAREA, 0, &workRect, 0))
          break;

        // POINT ptCords = { LOWORD(lpCwp->lParam), HIWORD(lpCwp->lParam) };

        if (IsCloseToEdge(rc.left, workRect.left))
          OffsetRect(&rc, (workRect.left - rc.left), 0);

        if (IsCloseToEdge(rc.top, workRect.top))
          OffsetRect(&rc, 0, (workRect.top - rc.top));

        if (IsCloseToEdge(rc.right, workRect.right))
          OffsetRect(&rc, (workRect.right - rc.right), 0);

        if (IsCloseToEdge(rc.bottom, workRect.bottom))
          OffsetRect(&rc, 0, (workRect.bottom - rc.bottom));

        POINT pt = { rc.left, rc.top };

        ClientToScreen(lpCwp->hwnd, &pt);
        SetWindowPos(lpCwp->hwnd, NULL, pt.x, pt.y, 
          (rc.right - rc.left), (rc.bottom -rc.top), 
          SWP_ASYNCWINDOWPOS);
      }
      break;*/
    }
  }
  return CallNextHookEx(0, nCode, wParam, lParam);
}

// EOF

