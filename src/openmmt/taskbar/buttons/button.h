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
#ifndef OPENMMT_TASKBAR_BUTTONS_BUTTON_H
#define OPENMMT_TASKBAR_BUTTONS_BUTTON_H
#pragma once

#include "openmmt/graphics/direct2d/direct2d_window.h"

enum ButtonState {
  BTN_NORMAL          = 0x02,     /**< Normal state of button. */
  BTN_PRESSED         = 0x04,     /**< Button is pressed. */
  BTN_ACTIVE          = 0x08,     /**< Button is active. */
  BTN_MAY_START_DRAG  = 0x10,     /**< User may start dragging button. */
  BTN_DRAGGING        = 0x20      /**< User is dragging button. */
};

class Button : 
  public Direct2DWindow
{
public:

  /**
   * Class constructor.
   *
   * @param hWndApp       Application handle.
   * @param hWndTaskbar   Taskbar handle.
   * @param x             x-coordinate.
   * @param y             y-coordinate.
   * @param width         Width of the button.
   * @param height        Height of the button.
   * @param hIcon         Application's icon.
   * @param index         Button index.
   */
  Button(HWND hWndApp, HWND hWndTaskbar, INT x, INT y, INT width, 
    INT height, int index);

  /**
   * Class destructor.
   */
  ~Button();

  /**
   * See GetButtonHandle.
   */
  const HWND GetId();

  /**
   * Returns the handle to the button window.
   */
  const HWND GetButtonHandle();

  /**
   * Returns the handle to the application.
   */
  const HWND GetAppHandle();

  /**
   * Returns the handle to the taskbar.
   */
  const HWND GetTaskbarHandle();

  /**
   * Returns the index of the button.
   */
  const INT GetIndex();

  /**
   * Returns the x-coordinate of the button.
   */
  const INT GetX();

  /**
   * Returns the y-coordinate of the button.
   */
  const INT GetY();

  /**
   * Creates the actual button. Must be called after the constructor.
   */
  void CreateButton();

  /**
   * Draws the button.
   *
   * TODO: Move this completely to the button event manager.
   */
  void Draw(HDC hdc, const LPRECT rect);

  /**
   * Self explanatory events.
   */
  void TrackMouseHoover();
  void OnMouseMove();
  void OnMouseHoover();
  void OnMouseLeave();

  /**
   * Basically the same as RemoveTaskbarButton.
   */
  void Persist();

  /**
   * Removes the application's icon from the Windows taskbar.
   */
  void RemoveTaskbarButton();

  /**
   * Sets the index of the button.
   */
  void SetIndex(INT index);

  /**
   * Adds a state to the button. Events like BTN_NORMAL and BTN_PRESSED
   * cannot co-exist. If one is added, the other is automatically removed.
   */
  void AddState(INT state);

  /**
   * Removes a state from the button.
   */
  void ClearState(INT state);

  /**
   * Sets the coordinates of the button. These are used for reference by
   * the thumbnail window.
   */
  void SetPoints(INT x, INT y);

  /**
   * Checks to see if a button is on a certain state or has a certain state
   * activated.
   */
  BOOL IsState(int state);

  void CreateExtraResources();
  void ReleaseExtraResources();
  void OnEraseBackground();
  void OnPaint();
  void OnThemeChange();
  void Flash();

  private:
    HWND m_hWnd;            /**< Button handle. */
    HWND m_hWndApp;         /**< Application's handle. */
    HWND m_hWndTaskbar;     /**< Application's taskbar handle. */
    INT m_X;                /**< Button's x coordinate. */
    INT m_Y;                /**< Button's y coordinate. */
    INT m_Width;            /**< Width. */
    INT m_Height;           /**< Height. */
    INT m_Index;            /**< Button's index/position. */
    INT m_btnState;         /**< Current state of the button. */
    BOOL m_bMayHoover;        /**< The cursor may hoover above the button. */
    BOOL m_bFlashing;       /**< Is the button flashing? */
    HICON m_hIcon;          /**< Application's icon. */
    HTHEME m_hTheme;        /**< Button's theme. */
    IWICBitmap *m_pIconBmp; /**< Application's icon. WIC Bitmap. */
    ID2D1Bitmap *m_pBitmap; /**< Same as above, but as a format that Direct2D can use. */
    COLORREF m_AverageColor; /**< Icon's average color. */
};

#endif // OPENMMT_TASKBAR_BUTTONS_BUTTON_H

// EOF
