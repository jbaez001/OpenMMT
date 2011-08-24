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
#ifndef OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_WINDOW_H
#define OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_WINDOW_H
#pragma once

#include "openmmt/graphics/direct2d/direct2d.h"

/** 
 * Direct2DWindow Interface.
 */
class Direct2DWindow
{
  // Public members.
public:
  ID2D1HwndRenderTarget *m_pRenderTarget; /**< Direct2D HWND Render Interface. */
  ID2D1GdiInteropRenderTargetPtr m_pInteropTarget; /**< Device context interface. */
  HWND m_hDevicehWnd;    /**< Handle to the window. */
  HDC  m_hDeviceHDC;     /**< Device's context. */
  RECT m_rcDeviceRect;   /**< Rectangle of the device. */

  // Public methods.
public:

  /**
   * Constructor.
   */
  Direct2DWindow();

  /**
   * Destructor.
   */
  ~Direct2DWindow();

  /**
   * Is the device set?
   */
  BOOL IsDeviceSet();

  /**
   * Sets the device. This MUST be called after the constructor.
   */
  void SetDevice(HWND hWnd);

  /**
   * Creates the device-dependent resources.
   */
  HRESULT CreateDeviceResources();

  /**
   * Releases the device-dependent resources.
   *
   */
  void ReleaseDeviceResources();

  
  /**
   * Called at the beginning of each draw method.
   */
  void BeginDraw();

  /**
   * Called at the end of each draw method.
   */
  HRESULT EndDraw();

  /**
   * Queries the device context. In this case, it's nothing more than
   * a simple m_pRenderTarget->QueryInterface(&m_pInteropTarget).
   */
  HRESULT QueryDeviceContext();

  /**
   * Creates extra device-dependent resources specified by the inheriting
   * class. This method is called immediately after CreateDeviceResources.
   */
  virtual void CreateExtraResources() = 0;

  /**
   * Releases extra device-dependent resources specified by the
   * inherited class. This method is called immediately after 
   * ReleaseDeviceResources.
   */
  virtual void ReleaseExtraResources() = 0;

  /**
   * Paints the background.
   */
  virtual void OnEraseBackground() = 0;

  /**
   * Paints the device. At the end of this method, the implementing
   * class should call ValidateRect(m_hDevicehWnd, NULL).
   */
  virtual void OnPaint() = 0;

};

#endif // OPENMMT_GRAPHICS_DIRECT2D_DIRECT2D_WINDOW_H

// EOF

