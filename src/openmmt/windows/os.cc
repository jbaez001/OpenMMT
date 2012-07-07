/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010-2012 Genscripts
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

/**
 * OSIsWin7Server08R2
 *  Returns TRUE if the operating system running is Windows Server 2008 R2 
 *  through Windows 7.
 */
BOOL OSIsWin7Server08R2()
{
  OSVERSIONINFO m_vInfo = {0};

  m_vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if ((GetVersionEx(&m_vInfo)) && (m_vInfo.dwMajorVersion == 6) && 
    (m_vInfo.dwMinorVersion == 1))
      return TRUE;

  return FALSE;
}

/**
 * OSIsServer08Vista
 *  Returns true if the operating system running is Windows Vista through
 *  Windows Server 2008.
 */
BOOL OSIsServer08Vista()
{
  OSVERSIONINFO m_vInfo = {0};

  m_vInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if ((GetVersionEx(&m_vInfo)) && (m_vInfo.dwMajorVersion == 6) && 
    (m_vInfo.dwMinorVersion == 0))
    return TRUE;

  return FALSE;
}


// EOF
