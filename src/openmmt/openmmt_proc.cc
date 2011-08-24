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
#include "openmmt/messages.h"
#include "openmmt/resource.h"
#include "openmmt/taskbar/taskbar_event.h"
#include "libsys/api.h"
#include <dbt.h>

/**
 * IsMonitor. Checks to see if the broadcasted device notification belongs
 *  to a monitor interface.
 *
 * @param dB      The device being broadcasted.
 *
 * @return TRUE   If it is a monitor.
 * @return FALSE  If it's not a monitor or something went wrong.
 */
static BOOL IsMonitor(PDEV_BROADCAST_HDR dB)
{

  // Check to see that the pointer is valid. If it is, we only care
  // about device interfaces, everything else we will ignore.
  if ((!dB) || (dB->dbch_devicetype != DBT_DEVTYP_DEVICEINTERFACE))
    return FALSE;

  PDEV_BROADCAST_DEVICEINTERFACE pInterface = 
    (PDEV_BROADCAST_DEVICEINTERFACE) dB;

  // Check the GUID of the device interface that was added/removed. If it
  // was that of a monitor, return TRUE.
  if (pInterface->dbcc_classguid == g_MonitorGUID) 
    return TRUE;

  return FALSE;
}

LRESULT CALLBACK OpenMMTProc(HWND hWnd, UINT msg, WPARAM wParam, 
                               LPARAM lParam)
{
  // Shell hooks.
  if (msg == g_uMsgShellHook) {
    switch(wParam)
    {
    // A window is being minimized.
    case HSHELL_GETMINRECT:
      break;

    // A window is being activated.
    case HSHELL_WINDOWACTIVATED:
      {
        TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar((HWND)wParam);

        if (bar != TaskbarPtr()) {
          bar->ActivateApp((HWND)wParam);
        }
      }
      break;

    // A window is being flashed.
    case HSHELL_FLASH:
      {
        TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar((HWND)lParam);
        if (bar == TaskbarPtr())
          break;

        ButtonPtr btn = bar->GetButtonFromApp((HWND)lParam);

        if (btn == ButtonPtr())
          break;

        btn->Flash();
      }
      break;

    default:
      break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }

  switch(msg)
  {
  case WM_DEVICECHANGE:
    {
      switch (wParam)
      {
      // We only care about devices that where added or removed.
      case DBT_DEVICEARRIVAL:
      case DBT_DEVICEREMOVECOMPLETE:
        {
          if (IsMonitor((PDEV_BROADCAST_HDR)lParam))
            g_pMonitorManager->FlushMonitors();
        }
        break;
      }
    }
    break;

  // Regular Window Messages (WM).
  case WM_CREATE:
    break;

  case WM_CLOSE:
    DestroyWindow(hWnd);
    break;

  case WM_DESTROY:
    PostQuitMessage(0);
    break;

  case WM_QUERYENDSESSION:
    return TRUE;

  case WM_ENDSESSION:
    {
      SysHooksStop();

      if (g_hWndHelper)
        SendMessage(g_hWndHelper, WM_DESTROY, NULL, NULL);
    }
    break;

  // OpenMMT Messages.
  case TASKBAR_WINDOW_CREATE:
    {
      g_pAppManager->CheckWindow((HWND)wParam);
    }
    return TRUE;

  case TASKBAR_WINDOW_ACTIVATE:
  case TASKBAR_WINDOW_SETFOCUS:
    {
      g_pAppManager->CheckWindow((HWND)wParam);
      TaskbarPtr bar = g_pMonitorManager->FindMonitorTaskbar((HWND)wParam);

      if (bar != TaskbarPtr()) {
        bar->ActivateApp((HWND)wParam);
      }
    }
    return TRUE;

  case TASKBAR_WINDOW_DESTROY:
    {
      g_pAppManager->RemoveApplication((HWND) wParam);
    }
    return TRUE;

  case TASKBAR_WINDOW_MINMAX:
    {
      TaskbarEvent::OnWindowMinMax((HWND) wParam, (UINT)lParam);
    }
    return TRUE;

  case TASKBAR_WINDOW_MINIMIZE:
    {
      TaskbarEvent::OnWindowMinimize((HWND) wParam);
    }
    return TRUE;

  case TASKBAR_WINDOW_POSCHANGED:
    {
      g_pAppManager->CheckWindow((HWND)wParam);
      TaskbarEvent::OnWindowPosChanged((HWND) wParam);
      return TRUE;
    }

  case TASKBAR_X86_SUPPORT_HELLO:
    {
      g_hWndHelper = (HWND)wParam;
    }
    return TRUE;

 #if 0
  case WM_COMMAND:
    {
      switch (LOWORD(wParam))
      {
      case ID_CONTEXTMENU_CLOSE:
        PostQuitMessage(0);
        break;

      default:
        break;
      }
    }
    break;

  case OPENMMT_TRAY_MESSAGE:
    {
      if (LOWORD(lParam) == WM_RBUTTONUP) {
        HMENU hMenu = LoadMenu(g_hInstance, MAKEINTRESOURCE(IDR_MENU1));

        if (!hMenu)
          break;

        HMENU hPopUpMenu = GetSubMenu(hMenu, 0);

        if (!hPopUpMenu)
          break;
        POINT pt = {0};

        GetCursorPos(&pt);
        TrackPopupMenu(hPopUpMenu, TPM_LEFTALIGN|TPM_LEFTBUTTON, pt.x, pt.y,
          0, hWnd, NULL);
        DestroyMenu(hPopUpMenu);
      }
    }
    break;
#endif

  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

// EOF
