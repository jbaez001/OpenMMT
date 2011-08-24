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
#include "openmmt/graphics/direct2d/direct2d.h"
#include "openmmt/windows/taskbar_interface.h"
#include "openmmt/windows/windows.h"

// Procedure declarations
extern LRESULT CALLBACK OpenMMTProc(HWND hwnd, UINT msg, WPARAM wParam, 
                                    LPARAM lParam);
extern LRESULT CALLBACK TaskbarProc(HWND hWnd, UINT msg, WPARAM wParam, 
                                    LPARAM lParam);
extern LRESULT CALLBACK ButtonProc(HWND hwnd, UINT msg, WPARAM wParam, 
                                   LPARAM lParam);
extern LRESULT CALLBACK ThumbnailProc(HWND hwnd, UINT msg, WPARAM wParam, 
                                      LPARAM lParam);

static void InitializeAndRegisterClasses()
{
  if (!WndRegisterClass(g_OpenMMTClassName, OpenMMTProc) ||
      !WndRegisterClass(g_TaskbarClassName, TaskbarProc) ||
      !WndRegisterClass(g_ThumbnailClassname, ThumbnailProc) ||
      !WndRegisterClass(g_TaskbarButtonClassName, ButtonProc)) {
        MessageBox(NULL, L"Could not register window classes", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }
}

void _InitializeInternals()
{
  InitializeAndRegisterClasses();
  CoInitialize(0);

  InitializeTaskbarInterface();

  // Load up the common control classes.
  INITCOMMONCONTROLSEX icex = {0};

  icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
  icex.dwICC = (ICC_COOL_CLASSES|ICC_BAR_CLASSES);

  if (!InitCommonControlsEx(&icex)) {
    abort();
  }

  // TODO: Notify the user properly.
  if (!InitializeDirect2D()) {
    abort();
  }
}

void _Shutdown()
{
  ReleaseDirect2D();
  ReleaseTaskbarInterfaces();
  CoUninitialize();
}

// EOF

