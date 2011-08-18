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

  HMODULE hMod = GetModuleHandle(SYSTEMHOOKS_DLL);

  if (hMod)
    return FALSE;

  if (!(g_hHookProc = SetWindowsHookEx(WH_CALLWNDPROCRET, &CallWndRetProc, (HINSTANCE)hMod, 0)))
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

