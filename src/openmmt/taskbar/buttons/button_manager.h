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
#ifndef OPENMMT_TASKBAR_BUTTONS_BUTTON_MANAGER_H
#define OPENMMT_TASKBAR_BUTTONS_BUTTON_MANAGER_H
#pragma once

// Forward declarations. 
class Application;
class Button;

#include "openmmt/taskbar/buttons/button.h"
#include "openmmt/template_classes/smart_vector_list.h"

/**
 * Button manager controller.
 */
class ButtonManager
{
  friend class Taskbar;

private:
  INT m_Width;            /**< Taskbar's width.  */
  INT m_Height;           /**< Taskbar's height. */
  INT m_MaxButtons;       /**< Maximum buttons we can hold. */
  INT m_TotalButtons;     /**< Amount of buttons we are carrying. */
  INT m_BtnWidth;         /**< Button's width.  */
  INT m_BtnHeight;        /**< Button's height. */
  BOOL  m_bClosing;       /**< We are closing. Do not do anything. */
  BOOL  m_bHorizontal;    /**< Is the taskbar horizontal? */
  HWND  m_hWndLastActive; /**< Handle to the last active application. */
  HTHEME m_hTheme;        /**< Taskbar's theme. */

  typedef std::pair<int, ButtonPtr> ButtonPair; /**< Button pair for map. */
  std::map<int, ButtonPtr> buttons_;  /**< Button map. */

public:
  ButtonManager();

  /**
   * Class destructor.
   */
  ~ButtonManager();

  /**
   * Retrieves a button by index.
   */
  ButtonPtr GetButton(int index);

  /**
   * Retrieves a button by window handle.
   */
  ButtonPtr GetButton(HWND hWnd);

  /**
   * Retrieves a button by application handle.
   */
  ButtonPtr GetButton(ApplicationPtr app);

  /**
   * Retrieves a button for an application based on the application's
   * handle.
   */
  ButtonPtr GetButtonForApp(HWND hWnd);

  /**
   * Creates a button for an application.
   *
   *  @param app  Handle to the application.
   */
  void CreateButton(ApplicationPtr app);

  /**
   * Calculates the X coordinates of a button based on index.
   */
  INT GetCoordinatesAtIndex(INT index);

  /**
   * Removes a button instance.
   *
   * @param btn   Button to remove.
   */
  void RemoveButton(ButtonPtr btn);

  /**
   * Reindex the buttons.
   */
  void ReIndexButtons();

  /**
   * Redraw the buttons.
   */
  void RedrawButtons();

  /**
   * Changes the layout to either horizontal or vertical.
   */
  void SetLayout(BOOL bHorizontal);

  void SetDimensions(LONG mWidth, LONG mHeight);

  void ActivateApp(HWND hWnd);

  void SetFirstActive();
};

#endif // OPENMMT_TASKBAR_BUTTONS_BUTTON_MANAGER_H

// EOF
