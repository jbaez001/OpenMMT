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
#include "openmmt/applications/application.h"
#include "openmmt/windows/windows.h"

Application::Application(HWND hWnd) :
  m_hWnd(hWnd),
  m_hWndButton(NULL),
  m_X(0),
  m_Y(0)
{
  m_Taskbar = g_pMonitorManager->FindMonitorTaskbar(m_hWnd);
}

Application::~Application()
{
}

const HWND Application::GetId()
{
  return m_hWnd;
}

TaskbarPtr Application::GetTaskbar()
{
  return m_Taskbar;
}

void Application::Update()
{
  if (m_Taskbar == TaskbarPtr()) {
    g_pMonitorManager->FindMonitorTaskbar(m_hWnd);
  } else {
    TaskbarPtr new_bar = g_pMonitorManager->FindMonitorTaskbar(m_hWnd);

    if ((new_bar == TaskbarPtr()) || 
      (m_Taskbar->GetWindowHandle() != new_bar->GetWindowHandle())) {
      g_pAppManager->RemoveApplication(m_hWnd);
      m_Taskbar = TaskbarPtr();
    }
  }
}

ButtonPtr Application::GetTaskbarButton()
{
  if (m_Taskbar == TaskbarPtr())
    return ButtonPtr();

  return m_Taskbar->GetButtonFromApp(m_hWnd);
}

// EOF
