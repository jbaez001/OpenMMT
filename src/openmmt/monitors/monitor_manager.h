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
#ifndef OPENMMT_MONITORS_MONITOR_MANAGER_H
#define OPENMMT_MONITORS_MONITOR_MANAGER_H
#pragma once

#include "openmmt/monitors/monitor.h"
#include "openmmt/taskbar/taskbar.h"
#include "openmmt/template_classes/smart_vector_list.h"

/**
 * MonitorManager. Class to manage monitors.
 */
class MonitorManager : public SmartVectorList<Monitor, MonitorManager>
{
public:
  /**
   * Class constructor.
   */
  MonitorManager();

  /**
   * Class destructor.
   */
  ~MonitorManager();

  /**
   * Clears the current monitor lists.
   *
   * @param bExit   Set to TRUE if calling this from the destructor.
   */
  void ClearMonitors(BOOL bExit = FALSE);

  /**
   * Monitor enumerating callback function.
   */
  static BOOL CALLBACK MonEnumProc(HMONITOR hMonitor, HDC hdcMonitor, 
    LPRECT lprcMonitor, LPARAM dwData);

  /**
   * Flushes the monitor list and re-enumerates them.
   */
  void FlushMonitors();

  /**
   * Enumerates the monitors.
   */
  void EnumerateMonitors();

  /**
   * Creates a monitor and inserts it into the vector.
   */
  void CreateMonitor(const HMONITOR hMonitor);

  /**
   * Finds the owning monitor of an application.
   */
  MonitorPtr FindMonitor(const HWND hWnd);

  /**
   * Finds the Taskbar of an application.
   */
  TaskbarPtr FindMonitorTaskbar(const HWND hWnd);

  /**
   * Determine the DPI settings.
   */
  void ScanDPI();

  /**
   * Are we using some sort of funkey DPI setting?
   */
  BOOL IsFunkeyDPI();

private:
  BOOL m_bFunkeyDPI;    /**< Funkey DPI?*/
  INT m_iMonitors;      /**< Total monitors managing. */

};

#endif // OPENMMT_MONITORS_MONITOR_MANAGER_H

//EOF
