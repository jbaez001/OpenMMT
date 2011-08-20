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
#include "libsys/sys.h"
#include "libsys/api.h"

SYSTEMHOOKS_API BOOL SysHooksCheckInstall(void)
{
  return g_bHooksInstalled;
}

SYSTEMHOOKS_API BOOL SysHooksStart(HWND hWnd)
{
  // If hooks are already running, lets just say that we installed them again.
  if (SysHooksCheckInstall())
    return TRUE;

  // Sanity check.
  if (!hWnd || !IsWindow(hWnd))
    return FALSE;

  HINSTANCE hMod = (HMODULE)GetModuleHandle(SYSTEMHOOKS_DLL);

  if (!hMod)
    return FALSE;

  if (!(g_hHookProc = SetWindowsHookEx(WH_CALLWNDPROCRET, &CallWndRetProc, hMod, 0)))
    return FALSE;

  g_bHooksInstalled = TRUE;
  g_hWndOpenMMT     = hWnd;

  return g_bHooksInstalled;
}

SYSTEMHOOKS_API void SysHookSetHelper(HWND hWnd)
{
  g_hWndHelper = hWnd;
}

SYSTEMHOOKS_API void SysHooksStop(void)
{
  if (g_hHookProc) {
    UnhookWindowsHookEx(g_hHookProc);
    g_bHooksInstalled = FALSE;
    g_hWndOpenMMT     = NULL;
    g_hWndHelper      = NULL;
  }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
  switch (ul_reason_for_call)
  {
  case DLL_PROCESS_ATTACH:
  case DLL_THREAD_ATTACH:
  case DLL_THREAD_DETACH:
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

// EOF

