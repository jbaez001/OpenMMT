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
#include "openmmt/global_variables.h"
#include "openmmt/monitors/monitor_manager.h"

MonitorManager::MonitorManager() :
  m_bFunkeyDPI(FALSE),
  m_iMonitors(0)
{
  ScanDPI();
}

MonitorManager::~MonitorManager()
{
  ClearMonitors(TRUE);
}

void MonitorManager::ClearMonitors(BOOL bExit)
{
  /**
    * Remove all references the monitor has to a taskbar so the taskbar
    * can be destroyed properly and we won't have any lingering references.
    */
  for (ObjectIterator it = vector_.begin();
    it != vector_.end(); ++it) {

      if (!it->get()->HasMainTaskbar()) {
        /** Remove application references from the taskbar. */
        TaskbarPtr bar = it->get()->GetTaskbar();
        bar->ClearApplications();

        /** Remove the taskbar reference of the monitor. */
        it->get()->RemoveTaskbarReference();
      }
  }
  vector_.clear();
}

void MonitorManager::FlushMonitors()
{
  ClearMonitors();
  g_pMonitorManager->EnumerateMonitors();
}

void MonitorManager::EnumerateMonitors()
{
  // TODO: Dump current monitor list first.
  EnumDisplayMonitors(NULL, NULL, &MonitorManager::MonEnumProc, 0);
}

void MonitorManager::CreateMonitor(const HMONITOR hMonitor)
{
  for (ObjectIterator it = vector_.begin();
    it != vector_.end(); ++it) {
      if (it->get()->GetMonitorHandle() == hMonitor) {
        it->get()->UpdateMonitor();
        return;
      }
  }

  MonitorPtr mon(new Monitor(hMonitor, m_iMonitors++));
  AddObject(mon);

  mon->Initialize();
}

void MonitorManager::ScanDPI()
{
  if (GetSystemMetrics(SM_CXICON) != 32)
    m_bFunkeyDPI = TRUE;
}

void MonitorManager::SetFirstActive()
{
  for (ObjectIterator it = vector_.begin();
    it != vector_.end(); ++it) {

      if (!it->get()->HasMainTaskbar()) {
        TaskbarPtr bar = it->get()->GetTaskbar();
        bar->SetFirstActive();
      }
  }
}

MonitorPtr MonitorManager::FindMonitor(const HWND hWnd)
{
  const HMONITOR hMonitor = MonitorFromWindow(hWnd, MONITOR_DEFAULTTONULL);

  if (!hMonitor) 
    return MonitorPtr();

  for (ObjectIterator it = vector_.begin();
    it != vector_.end(); ++it) {
      if (hMonitor == it->get()->GetMonitorHandle()) {
        return MonitorPtr(*it);
      }
  }

  return MonitorPtr();
}

TaskbarPtr MonitorManager::FindMonitorTaskbar(const HWND hWnd)
{
  MonitorPtr mon = FindMonitor(hWnd);

  if (mon != MonitorPtr())
    return mon->GetTaskbar();

  return TaskbarPtr();
}

BOOL MonitorManager::IsFunkeyDPI()
{
  return m_bFunkeyDPI;
}

BOOL CALLBACK MonitorManager::MonEnumProc(HMONITOR hMonitor, HDC hdcMonitor, 
  LPRECT lprcMonitor, LPARAM dwData)
{
  UNREFERENCED_PARAMETER(dwData);
  UNREFERENCED_PARAMETER(lprcMonitor);
  UNREFERENCED_PARAMETER(hdcMonitor);

  g_pMonitorManager->CreateMonitor(hMonitor);

  return TRUE;
}

// EOF
