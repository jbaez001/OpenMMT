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
#include "openmmt/windows/os.h"
#include "openmmt/windows/windows.h"
#include <dbt.h>

/**
 * WndRegisterClass. Registers a window class/
 *
 * @param lpszClassName
 * @param lpfnWndProc
 *
 */
BOOL WndRegisterClass(LPCWSTR lpszClassName, WNDPROC lpfnWndProc)
{
  WNDCLASSEX wcx;

  wcx.cbSize           = sizeof(WNDCLASSEX);
  wcx.style            = 0;
  wcx.cbClsExtra       =
  wcx.cbWndExtra       = 0;
  wcx.lpfnWndProc      = lpfnWndProc;
  wcx.hInstance        = g_hInstance;
  wcx.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
  wcx.hCursor          = LoadCursor(NULL, IDC_ARROW);
  wcx.hbrBackground    = (HBRUSH)(COLOR_3DFACE + 1);
  wcx.lpszMenuName     = NULL;
  wcx.lpszClassName    = lpszClassName;
  wcx.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);

  return (RegisterClassEx(&wcx) ? TRUE : FALSE);
}

/**
 * WndExtendFrame. Extends the frame of into the client area.
 *
 * @param hWnd
 */
BOOL WndExtendFrame(HWND hWnd)
{
  if (!IsCompositionActive())
    return FALSE;

  MARGINS margins = { -1, -1, -1, -1 };
  HRESULT hr = DwmExtendFrameIntoClientArea(hWnd, &margins);

  // TODO: Log failures.
  if (!SUCCEEDED(hr)) {
    return FALSE; 
  }

  return TRUE;
}

/**
 * WndSetBlur. Enables or disables blur accordingly. 
 *
 * @param hWnd
 * @param bEnable
 *
 */
BOOL WndSetBlur(HWND hWnd, bool bEnable)
{
  if (!IsCompositionActive())
    return FALSE;

  DWM_BLURBEHIND sDwmBlur = {0};

  sDwmBlur.dwFlags  = (DWM_BB_TRANSITIONONMAXIMIZED|DWM_BB_ENABLE);
  sDwmBlur.fEnable  = bEnable;
  sDwmBlur.fTransitionOnMaximized = TRUE;

  HRESULT hr = DwmEnableBlurBehindWindow(hWnd, &sDwmBlur);

  // TODO: Log failures.
  if (!SUCCEEDED(hr)) {
    return FALSE;
  }

  return TRUE;
}

/**
 * WndShouldDisplay. Check to see if a Window should be displayed on the
 * taskbar.
 *
 * @param hWnd    Window in question.
 */
BOOL WndShouldDisplay(HWND hWnd)
{
  if (!hWnd || !IsWindowVisible(hWnd) || GetParent(hWnd))
    return FALSE;

  LONG_PTR dwExFlags = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
  HWND hWndOwner = GetWindow(hWnd, GW_OWNER);

  if (((!(dwExFlags & WS_EX_TOOLWINDOW)) && (!hWndOwner)) ||
    (dwExFlags & WS_EX_APPWINDOW) && (hWndOwner))
    return TRUE;

  return FALSE;
}

/**
 * Is the window an OpenMMT window?
 */
BOOL WndOpenMMTWindow(HWND hWnd)
{
  TCHAR strWndClass[MAX_PATH] = {0};
  if (GetClassName(hWnd, strWndClass, MAX_PATH)) {
    if ((wcscmp(strWndClass, g_ThumbnailClassname) == 0) ||
       (wcscmp(strWndClass, g_TaskbarButtonClassName) == 0))
      return TRUE;
  }
  return FALSE;
}

BOOL WndGetWindowsTaskbarHandle()
{
  static HWND hwShellTray = FindWindowEx(0, NULL, L"Shell_TrayWnd", NULL);
  static HWND hwReBar     = FindWindowEx(hwShellTray, NULL, L"ReBarWindow32", NULL);
  g_hWndWindowsTaskbar    = FindWindowEx(hwReBar, NULL, L"MSTaskSwWClass", NULL);

  if (g_hWndWindowsTaskbar) {
    // Update the global variables related to the taskbar's size and position.
    WndGetWindowsTaskbarSize();
    return TRUE;
  }

  return FALSE;
}

/**
 * WndCheckAndDestroy.
 *
 */
BOOL WndCheckAndDestroy(HWND hWnd)
{
  if (IsWindow(hWnd))
    return DestroyWindow(hWnd);

  return FALSE;
}

HICON WndGetApplicationIcon(HWND hWnd)
{
  HICON m_hIcon = {0};

  m_hIcon = (HICON) SendMessage(hWnd, WM_GETICON, ICON_BIG, 0);

  if (m_hIcon == NULL) {
    m_hIcon = (HICON) GetClassLongPtr(hWnd, GCLP_HICON);
  } if (m_hIcon == NULL) {
    m_hIcon = (HICON) GetClassLongPtr(hWnd, GCLP_HICONSM);
  } if (m_hIcon == NULL) {
    m_hIcon = (HICON) SendMessage(hWnd, WM_GETICON, ICON_SMALL2, 0);
  } if (m_hIcon == NULL) {
    m_hIcon = (HICON) SendMessage(hWnd, WM_GETICON, ICON_SMALL, 0);
  }if (m_hIcon == NULL) {
    m_hIcon = LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION));
  }

  return m_hIcon;
}

// Device notification handle.
static HDEVNOTIFY lMonitorDevNotify = NULL;

/**
 * WndStartDeviceMonitor. Monitors any changes related to the monitors.
 *
 * Bug #165 [ Taskbar not closed when monitor is unplugged ]
 * http://redmine.genscripts.net/issues/165
 */
void WndStartDeviceMonitor()
{
  // Make sure that there is a handle to our main OpenMMT window before
  // doing anything.
  if (g_hWndOpenMMT) {

    // Build the interface for the notification filter.
    DEV_BROADCAST_DEVICEINTERFACE  nFilter = {0};
    
    nFilter.dbcc_size       = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
    nFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
    nFilter.dbcc_classguid  = g_MonitorGUID;

    lMonitorDevNotify = RegisterDeviceNotification(g_hWndOpenMMT, &nFilter, 
      DEVICE_NOTIFY_WINDOW_HANDLE);

    if (lMonitorDevNotify == NULL) {
      dprintf("RegisterDeviceNotification failed.\n");
    }
  }
}

/**
 * WndStopDeviceMonitor. Stops the monitor device notifications.
 *
 */
void WndStopDeviceMonitor()
{
  UnregisterDeviceNotification(lMonitorDevNotify);
}

void WndGetWindowsTaskbarSize()
{
  APPBARDATA sAppBar = {0};

  sAppBar.cbSize = sizeof(APPBARDATA);
  sAppBar.hWnd   = g_hWndWindowsTaskbar;

  // Query the Windows taskbar to see what we are working with.
  if ((BOOL)SHAppBarMessage(ABM_GETTASKBARPOS, &sAppBar)) {
    g_WindowsTaskbarWidth   = (INT)(sAppBar.rc.right - sAppBar.rc.left);
    g_WindowsTaskbarHeight  = (INT)(sAppBar.rc.bottom - sAppBar.rc.top);

    dprintf("Windows Taskbar Size: %ix%i\n", g_WindowsTaskbarWidth, 
      g_WindowsTaskbarHeight);

    // TODO: Find a better way to detect this. Users can edit the height
    // of the Windows 7 taskbar; however, I don't think they can decrease
    // the size of it to lower than 40 pixels.
    if (OSIsWin7Server08R2() && (g_WindowsTaskbarHeight < 40)) {
      dprintf("Taskbar is using small icons.\n");
      g_bUsingSmallIcons = TRUE;
    }

    switch (sAppBar.uEdge)
    {
    case ABE_LEFT:
      g_WindowsTaskbarPos = TASKBAR_LEFT;
      break;

    case ABE_TOP:
      g_WindowsTaskbarPos = TASKBAR_TOP;
      break;

    case ABE_RIGHT:
      g_WindowsTaskbarPos = TASKBAR_RIGHT;
      break;

      // We group ABE_BOTTOM and "default" just in case something weird
      // happens.
    case ABE_BOTTOM:
    default:
      g_WindowsTaskbarPos = TASKBAR_BOTTOM;
      break;
    }
  }
}

static NOTIFYICONDATA mTrayIcon = {0};

void WndTrayIconAdd()
{
  mTrayIcon.cbSize            = sizeof(NOTIFYICONDATA);
  mTrayIcon.hWnd              = g_hWndOpenMMT;
  mTrayIcon.uFlags            = NIF_MESSAGE|NIF_ICON|NIF_STATE;
  mTrayIcon.uCallbackMessage  = OPENMMT_TRAY_MESSAGE;
  mTrayIcon.hIcon             = LoadIcon(g_hInstance, MAKEINTRESOURCE(IDR_MAINFRAME));
  mTrayIcon.dwState           = 0;
  mTrayIcon.dwStateMask       = 0;
  mTrayIcon.uVersion          = NOTIFYICON_VERSION_4;

  Shell_NotifyIcon(NIM_ADD, &mTrayIcon);
}

void WndTrayIconRemove()
{
  Shell_NotifyIcon(NIM_DELETE, &mTrayIcon);
}

/**
 * WndRunMessageThread. Runs the message loop.
 */
void WndRunMessageThread()
{
  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

}

// EOF
