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
#define WIN32_LEAN_AND_MEAN
#include "openmmt/targetver.h"
#include <Windows.h>
#include "openmmt/messages.h"
#include "libsys/api.h"

static const wchar_t *wstrOpenMMT  = L"OpenMMT";
static const wchar_t *wstrHelper   = L"OpenMMTx86";
static HWND hWndOpenMMT   = {0};
static HWND hWndHelper    = {0};

static LRESULT CALLBACK OpenMMTx86Proc(HWND hwnd, UINT msg, WPARAM wParam, 
  LPARAM lParam)
{
  switch(msg)
  {
  case WM_CREATE:
    break;
  case WM_CLOSE:
    DestroyWindow(hwnd);
    break;
  case WM_DESTROY:
    PostQuitMessage(0);
    break;
  case WM_QUERYENDSESSION:
    return TRUE;
  case WM_ENDSESSION:
    SysHooksStop();
    return 0;
  default:
    return DefWindowProc(hwnd, msg, wParam, lParam);
  }
  return 0;
}

int __stdcall WinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPSTR lpCmdLine, __in int nShowCmd )
{
  if (!(hWndOpenMMT = FindWindow(wstrOpenMMT, L"OpenMMT_Main")))
    return 0;

  WNDCLASSEX wcx = {0};

  wcx.cbSize           = sizeof(WNDCLASSEX);
  wcx.style            = 0;
  wcx.cbClsExtra       =
  wcx.cbWndExtra       = 0;
  wcx.lpfnWndProc      = OpenMMTx86Proc;
  wcx.hInstance        = hInstance;
  wcx.hIcon            = LoadIcon(NULL, IDI_APPLICATION);
  wcx.hCursor          = LoadCursor(NULL, IDC_ARROW);
  wcx.hbrBackground    = (HBRUSH)(COLOR_3DFACE + 1);
  wcx.lpszMenuName     = NULL;
  wcx.lpszClassName    = wstrHelper;
  wcx.hIconSm          = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wcx)) {
    return 0;
  }

  if (!(hWndHelper = CreateWindow(wstrHelper, L"OpenMMTx86Helper", WS_DISABLED, 
    CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
    NULL, hInstance, NULL))) {
    return 0;
  }

  if (!SysHooksStart(hWndOpenMMT))
    return 0;

  SysHookSetHelper(hWndHelper);
  SendMessage(hWndOpenMMT, TASKBAR_X86_SUPPORT_HELLO, (WPARAM)hWndHelper, NULL);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }

  if (IsWindow(hWndHelper))
    DestroyWindow(hWndHelper);

  SysHooksStop();

  return 0;
}

// EOF
