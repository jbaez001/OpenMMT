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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"
#include "openmmt/windows/taskbar_interface.h"
#include "openmmt/windows/os.h"
#include "openmmt/template_classes/safe_release.h"

WinTaskbarInterface::WinTaskbarInterface() :
  g_iTaskbar2(NULL)
{
  HRESULT hr = S_OK;

  hr = CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER, 
    IID_ITaskbarList2, (void **)&g_iTaskbar2);

  if (hr != S_OK) {
    MessageBox(NULL, L"Could not initialize the proper ITaskbarList Interface", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  g_iTaskbar2->HrInit();
}

WinTaskbarInterface::~WinTaskbarInterface()
{
  SafeRelease(&g_iTaskbar2);
}

HRESULT WinTaskbarInterface::AddButton(HWND hWnd)
{
  return g_iTaskbar2->AddTab(hWnd);
}

HRESULT WinTaskbarInterface::RemoveButton(HWND hWnd)
{
  return g_iTaskbar2->DeleteTab(hWnd);
}

BOOL WinTaskbarInterface::HandleGetMinRect(HWND hWnd, LPREALRECT lpRect)
{
  ApplicationPtr pApp(g_pAppManager->FindApplication(hWnd));

  if (!pApp) 
    return FALSE;

  ButtonPtr pBtn(pApp->GetTaskbarButton());

  if (!pBtn)
    return FALSE;

  RECT rc = {0};

  if (!GetWindowRect(pBtn->GetButtonHandle(), &rc))
    return FALSE;

  lpRect->left   = (SHORT) rc.left;
  lpRect->top    = (SHORT) rc.top;
  lpRect->right  = (SHORT) rc.right;
  lpRect->bottom = (SHORT) rc.bottom;

  return TRUE;
}

// EOF

