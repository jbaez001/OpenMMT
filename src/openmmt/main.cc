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
#include "openmmt/openmmt.h"

CAppModule _Module;

int __stdcall wWinMain( __in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in LPWSTR lpCmdLine, __in int nShowCmd )
{
  InitCommonControls();

  _Module.Init(NULL, hInstance);
  OpenMMT_Run(hInstance);
  _Module.Term();

  return 0;
}

// EOF

