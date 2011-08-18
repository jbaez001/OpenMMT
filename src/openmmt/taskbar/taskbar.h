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
#ifndef OPENMMT_TASKBAR_TASKBAR_H
#define OPENMMT_TASKBAR_TASKBAR_H
#pragma once

// Forward declarations.
class Application;
class ButtonManager;

#include "openmmt/applications/application.h"
#include "openmmt/graphics/direct2d/direct2d_window.h"
#include "openmmt/taskbar/buttons/button_manager.h"
#include "openmmt/template_classes/smart_vector_list.h"

enum TaskbarPosition {
  TASKBAR_BOTTOM = 1,
  TASKBAR_RIGHT,
  TASKBAR_TOP,
  TASKBAR_LEFT
};

/**
 * Taskbar class.
 */
class Taskbar : public SmartVectorList<Application, Taskbar>,
                public ButtonManager,
                public Direct2DWindow
{
  /** Private class members. */
private:
  HWND m_hWnd;          /**< Taskbar's window handle. */
  LONG m_X;             /**< Taskbar's x position. */
  LONG m_Y;             /**< Taskbar's y position. */
  LONG m_Width;         /**< Taskbar's width.  */
  LONG m_Height;        /**< Taskbar's height. */
  INT m_Position;       /**< Taskbar's position (top,bottom,left,right) */
  BOOL m_bAlwaysOnTop;  /**< Always on top. */
  BOOL m_bAutoHide;     /**< Autohide. */
  BOOL m_bFullScreen;   /**< An application is in full screen. */
  RECT m_AppBarRect;    /**< Rect of the app bar. */

  /** Private class methods. */
private:

  /**
   * Creates the taskbar.
   */
  void CreateTaskbar();

  /**
   * Registers an app bar with the system. Do we really need to do this?
   */
  void RegisterAppBar();

  /**
   * Unregisters an app bar with the system.
   */
  void UnregisterAppBar();

  UINT GetTaskbarEdge();

public:

  /**
   * Creates a taskbar.
   */
  Taskbar();

  /**
   * Class destructor.
   */
  ~Taskbar();

  void SetWorkSpace(LPRECT rcMonitor, LPRECT rcWork, INT mPosition);

  /**
   * Initializes the taskbar. This must be called in the following order:
   *    1. Constructor.
   *    2. SetWorkSpace.
   *    3. Initialize.
   */
  void Initialize();

  /**
   * Adds an application to the taskbar and also creates a button for it.
   *
   * @param app
   */
  void AddApplication(ApplicationPtr app);

  /**
   * Removes an application from the taskbar and its respective taskbar
   * button along with it.
   */
  void RemoveApplication(ApplicationPtr app);

  /**
   * Removes all applications from this taskbar.
   *
   */
  void ClearApplications();


  /**
   * Returns the width of the taskbar.
   */
  const LONG GetWidth();

  /**
   * Returns the height of the taskbar.
   */
  const LONG GetHeight();

  /**
   * Returns the handle of the taskbar.
   *
   */
  const HWND GetWindowHandle();

  /**
   * Gets the current position of the taskbar.
   */
  const INT GetPosition();

  /**
   * Checks the current position of the taskbar. The valid positions
   * are declared on this header.
   */
  BOOL IsPosition(INT pos);

  BOOL IsAutoHidden();

  /**
   * Sets the position of this taskbar.
   */
  void SetPosition(INT pos);

  /**
   * Creates extra resources. Currently, this does nothing.
   */
  void CreateExtraResources();

  /**
   * Releases extra resources. The taskbar creates no extra resources
   * so this method does nothing.
   */
  void ReleaseExtraResources();

  /**
   * Background paint procedure.
   */
  void OnEraseBackground();

  /**
   * Paint procedure.
   */
  void OnPaint();

  /**
   * Called when the Windows theme or composition is changed.
   */
  void OnThemeChange();

  /**
   * Enables or disables auto-hide.
   */
  void SetAutoHide(BOOL bAutoHide);

  /**
   * Enables or disables always-on-top.
   */
  void SetAlwaysOnTop(BOOL bAlwaysOnTop);

  /**
   * An application has entered full screen.
   */
  void AppEnterFullScreen(HWND hWnd);

  /**
   * An application has left full screen.
   */
  void AppLeaveFullScreen(HWND hWnd);
};

#endif // OPENMMT_TASKBAR_TASKBAR_H

// EOF
