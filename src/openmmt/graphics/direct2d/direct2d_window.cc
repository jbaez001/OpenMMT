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
#include "openmmt/graphics/direct2d/direct2d_window.h"

Direct2DWindow::Direct2DWindow() :
  m_pRenderTarget(NULL),
  m_hDevicehWnd(NULL)
{

}

Direct2DWindow::~Direct2DWindow()
{
  ReleaseDeviceResources();
}

void Direct2DWindow::SetDevice(HWND hWnd)
{
  m_hDevicehWnd = hWnd;
}

BOOL Direct2DWindow::IsDeviceSet()
{
  return (IsWindow(m_hDevicehWnd));
}

HRESULT Direct2DWindow::CreateDeviceResources()
{
  HRESULT hr = S_OK;

  if (!m_pRenderTarget) {

    GetClientRect(m_hDevicehWnd, &m_rcDeviceRect);

    D2D1_SIZE_U size = D2D1::SizeU(m_rcDeviceRect.right - m_rcDeviceRect.left, 
      m_rcDeviceRect.bottom - m_rcDeviceRect.top);

    D2D1_PIXEL_FORMAT rtFormat =
      D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM,
        D2D1_ALPHA_MODE_PREMULTIPLIED);

    D2D1_RENDER_TARGET_PROPERTIES rtProperties = D2D1::RenderTargetProperties(
      D2D1_RENDER_TARGET_TYPE_DEFAULT, rtFormat, 0, 0, D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE,
      D2D1_FEATURE_LEVEL_DEFAULT);

    rtProperties.usage = D2D1_RENDER_TARGET_USAGE_GDI_COMPATIBLE;
    rtProperties.pixelFormat = rtFormat;

    hr =  m_pD2DFactory->CreateHwndRenderTarget(rtProperties,
      D2D1::HwndRenderTargetProperties(m_hDevicehWnd, size), &m_pRenderTarget);

  }

  CreateExtraResources();
  return hr;
}

void Direct2DWindow::ReleaseDeviceResources()
{
  SafeRelease(&m_pRenderTarget);
}

void Direct2DWindow::BeginDraw()
{
  m_pRenderTarget->BeginDraw();
}

HRESULT Direct2DWindow::EndDraw()
{
  HRESULT hr = m_pRenderTarget->EndDraw();

  // If a presentation error happens, and it may be recoverable, Windows
  // will kick back this message to us. This means that we should re-create
  // the device to try to render again.
  //
  // Reference:
  //  MSDN: http://msdn.microsoft.com/en-us/library/dd370979(VS.85).aspx
  if (hr == D2DERR_RECREATE_TARGET)
    ReleaseDeviceResources();

  // We pass it on so the classes that implement this interface will be
  // able to know if they need to call CreateExtraResources or not.
  return hr;
}

HRESULT Direct2DWindow::QueryDeviceContext()
{
  return m_pRenderTarget->QueryInterface(&m_pInteropTarget);
}

// EOF

