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
#include "openmmt/windows/os.h"

/**
 * ITaskbarList2. Interface that extends ITaskbarList.
 *
 *  Minimum Client: Windows XP
 *  Minimum Server: Windows Server 2003
 *
 * MSDN: http://msdn.microsoft.com/en-us/library/bb774638(v=VS.85).aspx
 */
static ITaskbarList3* g_iTaskbar2 = NULL;

/**
 * ITaskbarList3. Interface that extends ITaskbarList2.
 *
 *  Minimum Client: Windows 7
 *  Minimum Server: Windows Server 2008 R2
 *
 * MSDN: http://msdn.microsoft.com/en-us/library/dd391692(v=VS.85).aspx
 */
static ITaskbarList3* g_iTaskbar3 = NULL;


/**
 * InitializeTaskbarInterface. Initializes the proper ITaskbarList interface
 * depending on the version of the OS running.
 */
void InitializeTaskbarInterface()
{
  HRESULT hr = S_OK;
  if (OSIsWin7Server08R2()) {
    // Should we use CLSCTX_ALL instead?
    hr = CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER, 
      IID_ITaskbarList3, (void **)&g_iTaskbar3);
    g_iTaskbar3->HrInit();
  } else {
    hr = CoCreateInstance(CLSID_TaskbarList, 0, CLSCTX_INPROC_SERVER, 
      IID_ITaskbarList2, (void **)&g_iTaskbar2);
    g_iTaskbar2->HrInit();
  }
  if (hr != S_OK) {
    MessageBox(NULL, L"Could not initialize the proper ITaskbarList Interface", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }
}


// EOF

