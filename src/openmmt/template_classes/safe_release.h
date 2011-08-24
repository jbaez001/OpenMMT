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
#ifndef OPENMMT_TEMPLATE_CLASSES_SAFE_RELEASE_H
#define OPENMMT_TEMPLATE_CLASSES_SAFE_RELEASE_H

/**
 * SafeRelease template from MSDN:
 * http://msdn.microsoft.com/en-us/library/dd370994(v=VS.85).aspx
 */
template<class Interface>
inline void SafeRelease(Interface **ppInterfaceToRelease)
{
  if (*ppInterfaceToRelease != NULL) {
    (*ppInterfaceToRelease)->Release();
    (*ppInterfaceToRelease) = NULL;
  }
}

#endif // OPENMMT_TEMPLATE_CLASSES_SAFE_RELEASE_H

// EOF

