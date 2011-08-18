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
#include "openmmt/graphics/direct2d/direct2d.h"

ID2D1Factory *m_pD2DFactory;
IWICImagingFactory *m_pWICFactory;

BOOL InitializeDirect2D()
{
  HRESULT hr =
    CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER,
    IID_PPV_ARGS(&m_pWICFactory));

  if (!SUCCEEDED(hr))
    return FALSE;

  hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, 
    &m_pD2DFactory);

  if (!SUCCEEDED(hr))
    return FALSE;

  return TRUE;
}

void ReleaseDirect2D()
{
  SafeRelease(&m_pD2DFactory);
  SafeRelease(&m_pWICFactory);
}

// EOF
