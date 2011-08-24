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
#include "openmmt/monitors/monitor.h"

Monitor::Monitor(HMONITOR hMonitor, UINT Id) :
  m_hMonitor(hMonitor),
  m_Id(Id),
  m_Width(0),
  m_Height(0),
  m_X(0),
  m_Y(0),
  bPrimary(FALSE)
{
  m_hMonitorInfo.cbSize = sizeof(m_hMonitorInfo);

  GetWorkSpace();
  m_MonitorName = std::wstring(m_hMonitorInfo.szDevice);
}

Monitor::~Monitor()
{
  /* Make sure that we do not modify the working space of the main
   * monitor.
   */
  if (!bPrimary)
    ModifyWorkSpace(true);
}

std::tr1::shared_ptr<RECT> Monitor::GetWorkSpaceRect()
{
  std::tr1::shared_ptr<RECT> lpRect(new RECT);

  lpRect->bottom =
  lpRect->left   =
  lpRect->right  =
  lpRect->top    = 0;

  if (GetMonitorInfo(m_hMonitor, &m_hMonitorInfo)) {
    lpRect->bottom = m_hMonitorInfo.rcWork.bottom;
    lpRect->left   = m_hMonitorInfo.rcWork.left;
    lpRect->right  = m_hMonitorInfo.rcWork.right;
    lpRect->top    = m_hMonitorInfo.rcWork.top;
  }

  return lpRect;
}

void Monitor::GetWorkSpace()
{
  if (!GetMonitorInfo(m_hMonitor, &m_hMonitorInfo)) {
    MessageBox(NULL, L"Could not retrieve monitor info.", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  m_Width  = m_hMonitorInfo.rcMonitor.right - m_hMonitorInfo.rcMonitor.left;
  m_Height = m_hMonitorInfo.rcMonitor.bottom - m_hMonitorInfo.rcMonitor.top;
  m_X      = m_hMonitorInfo.rcMonitor.left;
  m_Y      = m_hMonitorInfo.rcMonitor.top;

  if ((m_X == 0) && (m_Y == 0))
    bPrimary = TRUE; 
}

static BOOL CALLBACK EnumChildProc(HWND hwndChild, LPARAM lParam) 
{
  UNREFERENCED_PARAMETER(lParam);
  DestroyWindow(hwndChild);
  return TRUE;
}

void Monitor::UpdateMonitor()
{
  GetWorkSpace();

  if (m_Taskbar != TaskbarPtr()) {
    // Check to see if the primary monitors where changed.
    if (bPrimary) {
      m_Taskbar->ClearApplications();
      HWND hwTaskbar = m_Taskbar->GetWindowHandle();
      EnumChildWindows(hwTaskbar, &EnumChildProc, NULL);
      RemoveTaskbarReference();
      DestroyWindow(hwTaskbar);
    } else {
      // Let the taskbar update itself.
      UpdateTaskbarArea(m_Taskbar->GetPosition());
    }
  } else {
    Initialize();
  }
}

void Monitor::Initialize()
{
  // Determine if its the primary monitor. If so, declare it as such. If not,
  // then we will create a taskbar for it.
  if (!bPrimary) {
    m_Taskbar = TaskbarPtr(new Taskbar());
    UpdateTaskbarArea(g_WindowsTaskbarPos);
    m_Taskbar->Initialize();
  }
}

void Monitor::UpdateTaskbarArea(INT pos)
{
  m_hMonitorInfo.rcWork = m_hMonitorInfo.rcMonitor;
  m_Taskbar->SetWorkSpace(&m_hMonitorInfo.rcMonitor, &m_hMonitorInfo.rcWork, pos);
  ModifyWorkSpace(false);
}

void Monitor::ModifyWorkSpace(bool bReset)
{
  SystemParametersInfo(SPI_SETWORKAREA, 1, 
    (bReset == true) ? &m_hMonitorInfo.rcMonitor : &m_hMonitorInfo.rcWork,
    SPIF_SENDCHANGE);
}

const UINT Monitor::GetId()
{
  return m_Id;
}

const HMONITOR Monitor::GetMonitorHandle()
{
  return m_hMonitor;
}

const LONG Monitor::GetHeight()
{
  return m_Height;
}

const LONG Monitor::GetWidth()
{
  return m_Width;
}

BOOL Monitor::IsPrimary()
{
  return bPrimary;
}

BOOL Monitor::IsValid()
{
  return (GetMonitorInfo(m_hMonitor, &m_hMonitorInfo));
}

TaskbarPtr Monitor::GetTaskbar()
{
  return m_Taskbar;
}

void Monitor::RemoveTaskbarReference()
{
  if (m_Taskbar != TaskbarPtr()) {
    m_Taskbar = TaskbarPtr();
  }
}
// EOF
