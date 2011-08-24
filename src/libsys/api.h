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
#ifndef SYSTEMHOOKS_API_H
#define SYSTEMHOOKS_API_H

#if defined(SYSTEMHOOKS_EXPORTS)
# define SYSTEMHOOKS_API __declspec(dllexport) 
#else
# define SYSTEMHOOKS_API __declspec(dllimport)
#endif

#if defined(OPENMMT_HELPER)
# define SYSTEMHOOKS_DLL  L"libsys32.dll"
#else
# define SYSTEMHOOKS_DLL  L"libsys.dll"
#endif

extern "C" {
  /**
   * Checks to see if the system hooks are already installed.
   */
  SYSTEMHOOKS_API BOOL SysHooksCheckInstall(void);

  /**
   * Installs the the system hooks.
   *
   * @param hWnd    OpenMMT's Handle.
   */
  SYSTEMHOOKS_API BOOL SysHooksStart(HWND hWnd);

  /**
   * Sets the helper.
   */
  SYSTEMHOOKS_API void SysHookSetHelper(HWND hWnd);

  /**
   * Stops the system hooks.
   */
  SYSTEMHOOKS_API void SysHooksStop(void);
};

#endif // SYSTEMHOOKS_API_H

// EOF

