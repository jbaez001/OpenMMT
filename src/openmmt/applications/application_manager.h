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
#ifndef OPENMMT_APPLICATIONS_APPLICATION_MANAGER_H
#define OPENMMT_APPLICATIONS_APPLICATION_MANAGER_H
#pragma once

#include "openmmt/applications/application.h"
#include "openmmt/template_classes/smart_vector_list.h"

class ApplicationManager : public SmartVectorList<Application, 
                                             ApplicationManager>
{
public:

  /**
   * Class constructor.
   */
  ApplicationManager();

  /**
   * Class destructor.
   */
  ~ApplicationManager();

  /**
   * Finds an application within our vector.
   */
  ApplicationPtr FindApplication(HWND hWnd);

  /**
   * Updates all windows accordingly.
   */
  void UpdateAllWindows();

  /**
   * Creates an application instance if we are unable to find one
   * for the handle provided.
   */
  void CreateApplication(HWND hWnd);

  /**
   * Removes an application from the application manager.
   */
  void RemoveApplication(HWND hWnd);

  /**
   * Checks to see if we need to call CreateApplication and does so
   * accordingly.
   */
  void CheckWindow(HWND hWnd);

  /**
   * Finds a button pointer by handle.
   */
  ButtonPtr FindButton(HWND hWnd);

  /**
   * Enumerates all windows. This is invoked by the UpdateAllWindows() method.
   * Only called at startup after all taskbars have been created.
   */
  static BOOL CALLBACK EnumWindowsProc(HWND hWnd, LPARAM lParam);
};

#endif // OPENMMT_APPLICATIONS_APPLICATION_MANAGER_H

// EOF
