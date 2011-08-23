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
#include "openmmt/windows/windows.h"
#include "libsys/api.h"

// Initialization functions outside this scope.
extern void _InitializeInternals();
extern void _Shutdown();

#if defined(_WIN64)

// Name of our helper.
const wchar_t *wstrHelper = L"openmmt32.exe";

static void StartHelper()
{
  if (GetModuleFileName(NULL, g_strExePath, MAX_PATH) == 0)
    abort();

  std::wstring full_path(g_strExePath);
  std::wstring cur_dir = full_path.substr(0, full_path.rfind('\\'));

  HINSTANCE hResult = ShellExecute(NULL, 
    L"open", wstrHelper, NULL, cur_dir.c_str(), SW_NORMAL);

  if ((INT)hResult <= 32) {
    MessageBox(NULL, L"Error starting openmmt32.exe", L"OpenMMT Alert", 0);
    abort();
  }
}

#endif

void OpenMMT_Run(HINSTANCE hInstance)
{
  // A copy of OpenMMT is already running.
  if (FindWindow(g_OpenMMTClassName, L"OpenMMT_Main")) {
    MessageBox(NULL, L"A copy of OpenMMT is already running.", L"OpenMMT", MB_OK);
    return;
  }
#if defined(DEBUG)
  boost::scoped_ptr<Gen::CGenConsole> pConsole(new Gen::CGenConsole(TRUE));
  pConsole->OpenConsole();
#endif

  if (!WndGetWindowsTaskbarHandle())
    return;

  g_hInstance = hInstance;
  _InitializeInternals();

  g_hWndOpenMMT = CreateWindow(g_OpenMMTClassName, L"OpenMMT_Main", 
    WS_DISABLED, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 
    NULL, NULL, g_hInstance, NULL);

  if (!g_hWndOpenMMT)
    return;

  g_uMsgShellHook = RegisterWindowMessage(L"SHELLHOOK");
  g_uiAppBarMsg   = RegisterWindowMessage(L"OPENMMTAPPBAR");

  RegisterShellHookWindow(g_hWndOpenMMT);

  g_pMonitorManager = new MonitorManager();
  g_pAppManager     = new ApplicationManager();

  g_pMonitorManager->EnumerateMonitors();
  g_pAppManager->UpdateAllWindows();
  g_pMonitorManager->SetFirstActive();

  if (SysHooksStart(g_hWndOpenMMT)) {
    dprintf("libsys started sucessfully.\n");
#if defined(_WIN64)
    StartHelper();
#endif
    WndStartDeviceMonitor();
    WndRunMessageThread();
    WndStopDeviceMonitor();
    SysHooksStop();

    // Tell the helper to close if its still open.
#if  defined(_WIN64)
    if (IsWindow(g_hWndHelper)) {
      SendMessage(g_hWndHelper, WM_CLOSE, 0, 0);
    }
#endif
  } else {
    abort();
  }

  // Monitor manager must be destroyed first. 
  delete g_pMonitorManager;
  delete g_pAppManager;

  _Shutdown();
}

// EOF

