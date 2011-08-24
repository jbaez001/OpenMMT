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

#include "openmmt/windows/theme_manager.h"

std::auto_ptr<ThemeManager> g_pThemeManager;

ThemeManager::ThemeManager(HWND hWnd) : 
  hTaskbarTheme(NULL),
  hTaskbandTheme(NULL)
{
  if (IsCompositionActive()) {
    hTaskbarTheme = OpenThemeData(hWnd, L"TASKBAR2COMPOSITED::TASKBAR");
  } else {
    hTaskbarTheme = OpenThemeData(hWnd, L"TASKBAR2::TASKBAR");
  }

  if (!hTaskbarTheme)
    abort();
}


ThemeManager::~ThemeManager()
{
  if (hTaskbarTheme)
    CloseThemeData(hTaskbarTheme);

  if (hTaskbandTheme)
    CloseThemeData(hTaskbandTheme);
}

HTHEME ThemeManager::GetTaskbarTheme()
{
  return hTaskbarTheme;
}

HTHEME ThemeManager::GetTaskbandTheme(HWND hWnd)
{
  if (hTaskbandTheme)
    return hTaskbandTheme;

  if (IsCompositionActive()) {
    hTaskbandTheme = OpenThemeData(hWnd, L"TASKBAND2COMPOSITED::TASKBAND2");
  }
  else {
    hTaskbandTheme = OpenThemeData(hWnd, L"TASKBAND2");
  }

  if (!hTaskbandTheme)
    abort();

  return hTaskbandTheme;
}
// EOF
