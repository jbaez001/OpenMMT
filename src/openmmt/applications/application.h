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
#ifndef OPENMMT_APPLICATIONS_APPLICATION_H
#define OPENMMT_APPLICATIONS_APPLICATION_H
#pragma once

class Application
{
public:

  /**
   * Class constructor.
   */
  Application(HWND hWnd);

  /**
   * Class destructor.
   */
  ~Application();

  /**
   * Returns the HWND of the application.
   */
  const HWND GetId();

  /**
   * Returns the application's taskbar if any.
   */
  TaskbarPtr GetTaskbar();

  /**
   * Returns the application's taskbar button.
   */
  ButtonPtr GetTaskbarButton();

  /**
   * Updates the application.
   */
  void Update();

private:
  HWND m_hWnd;        /**< Application's handle. */
  HWND m_hWndButton;  /**< Handle to the taskbar button. */
  LONG m_X;           /**< Application's x coordinate. */
  LONG m_Y;           /**< Application's y coordinate. */
  TaskbarPtr m_Taskbar;
};

#endif // OPENMMT_APPLICATIONS_APPLICATION_H

// EOF
