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
#ifndef OPENMMT_MONITORS_MONITOR_H
#define OPENMMT_MONITORS_MONITOR_H
#pragma once

#include "openmmt/taskbar/taskbar.h"

/**
 * Monitor class.
 */
class Monitor
{
public:
  /**
   * Class constructor.
   */
  Monitor(HMONITOR hMonitor, UINT Id);

  /**
   * Class Destructor.
   */
  ~Monitor();

  std::tr1::shared_ptr<RECT> GetWorkSpaceRect();
  void GetWorkSpace();
  void UpdateMonitor();
  void Initialize();
  void RemoveTaskbarReference();
  void UpdateTaskbarArea(INT pos);

  const UINT GetId();
  const HMONITOR GetMonitorHandle();
  const LONG GetWidth();
  const LONG GetHeight();

  BOOL IsPrimary();
  BOOL HasMainTaskbar();
  BOOL IsValid();

  TaskbarPtr GetTaskbar();

private:
  MONITORINFOEX   m_hMonitorInfo; /**< Structure containing monitor's nfo */
  HMONITOR        m_hMonitor;     /**< Handle to the monitor. */
  TaskbarPtr      m_Taskbar;      /**< Taskbar. */
  std::wstring    m_MonitorName;  /**< Name of the monitor */
  UINT  m_Id;                     /**< Monitor's ID */
  LONG m_Width;                   /**< Monitor's width. */
  LONG m_Height;                  /**< Monitor's height. */
  BOOL bPrimary;                  /**< Primary monitor? */
  BOOL bMainTaskbar;              /**< Is the Windows taskbar in this monitor? */

  /***********************************************************************
   ** Begin Private Methods.
   ***********************************************************************/
  /**
   * ModifyWorkSpace. Modifies the workspace of a monitor.
   *
   * @param bReset    Set this to true to reset the workspace.
   */
  void ModifyWorkSpace(bool bReset);

};
#endif // OPENMMT_MONITORS_MONITOR_H

//EOF
