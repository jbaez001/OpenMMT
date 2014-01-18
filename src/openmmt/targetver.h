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
#ifndef OPENMMT_TARGETVER_H
#define OPENMMT_TARGETVER_H

#ifdef __INTEL_COMPILER
#   pragma warning( disable : 181  ) // argument is incompatible with corresponding format string conversion
#endif

// We define the minimum supported platform to be Windows Vista SP2.
// Windows Vista SP2 has the Platform Upgrade that implements Direct2D.
#define NTDDI_VERSION  NTDDI_VISTASP2
#define WINVER         0x0600 
#define _WIN32_WINNT   0x0600
#define _WIN32_IE      0x0800

#include <SDKDDKVer.h>

#endif // OPENMMT_TARGETVER_H

// EOF 
