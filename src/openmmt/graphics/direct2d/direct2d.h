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
#ifndef OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_H
#define OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_H
#pragma once

#include <D2D1.h>
#include <wincodec.h>
#include <comdef.h>
#include "openmmt/template_classes/safe_release.h"

extern ID2D1Factory *m_pD2DFactory;
extern IWICImagingFactory *m_pWICFactory;
extern BOOL InitializeDirect2D();
extern void ReleaseDirect2D();

_COM_SMARTPTR_TYPEDEF(ID2D1GdiInteropRenderTarget,
                      __uuidof(ID2D1GdiInteropRenderTarget));

#endif // OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_H
