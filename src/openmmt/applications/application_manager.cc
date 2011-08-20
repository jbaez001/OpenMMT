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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"
#include "openmmt/applications/application_manager.h"
#include "openmmt/monitors/monitor.h"
#include "openmmt/windows/windows.h"

ApplicationManager::ApplicationManager()
{
  m_Thumbnail.reset(new Thumbnail());
}

ApplicationManager::~ApplicationManager()
{
  for (ObjectIterator it = vector_.begin();
    it != vector_.end(); ++it) {
#if defined(_DEBUG)
      BOOST_ASSERT(it->use_count() == 1);
#endif
  }
}

ApplicationPtr ApplicationManager::FindApplication(HWND hWnd)
{  
  for (std::vector<boost::shared_ptr<Application>>::iterator it = vector_.begin(); 
    it != vector_.end(); ++it) {
      if (it->get()->GetId() == hWnd) {
        return ApplicationPtr(*it);
      }
  }

  return ApplicationPtr();
}


void ApplicationManager::UpdateAllWindows()
{
  EnumWindows(&ApplicationManager::EnumWindowsProc, 0);
}

void ApplicationManager::CreateApplication(HWND hWnd)
{
  ApplicationPtr app = FindApplication(hWnd);

  if (app != ApplicationPtr())
    return;

  if (WndOpenMMTWindow(hWnd))
    return;

  app = ApplicationPtr(new Application(hWnd));
  AddObject(app);

  TaskbarPtr bar = app->GetTaskbar();

  if (bar != TaskbarPtr()) {
    bar->AddApplication(app);
  }
}

void ApplicationManager::RemoveApplication(HWND hWnd)
{
  ApplicationPtr app = FindApplication(hWnd);

  if ((app != ApplicationPtr()) && (app->GetTaskbar() != TaskbarPtr()))  {
    app->GetTaskbar()->RemoveApplication(app);
  }
   
  if (app != ApplicationPtr()) {
    RemoveObject(app);
  }
}

void ApplicationManager::CheckWindow(const HWND hWnd)
{
  if (!hWnd)
    return;

  ApplicationPtr app = FindApplication(hWnd);

  if (app != ApplicationPtr()) {
    if (!IsWindowVisible(hWnd) || !WndShouldDisplay(hWnd))
      g_pAppManager->RemoveApplication(hWnd);
  } else if (!IsWindowVisible(hWnd) || !WndShouldDisplay(hWnd)) {
    return;
  }

  MonitorPtr monitor = g_pMonitorManager->FindMonitor(hWnd);

  if (monitor == MonitorPtr()) {
    dprintf("Unable to find monitor for %08X\n", hWnd);
    return;
  }
  

  if (app != ApplicationPtr()) {
    app->Update();
  } else if (!monitor->IsPrimary()) {
      g_pAppManager->CreateApplication(hWnd);
  }
}

BOOL CALLBACK ApplicationManager::EnumWindowsProc(HWND hWnd, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);

  if (g_pAppManager)
    g_pAppManager->CheckWindow(hWnd);

  return TRUE;
}

BOOL ApplicationManager::IsThumbnailRegistered()
{
  return m_Thumbnail->IsThumbnailRegistered();
}

BOOL ApplicationManager::IsThumbnailed(HWND hWnd)
{
  if (!hWnd) 
    return FALSE;
 
  dprintf("Source: %08X vs %08X\n", m_Thumbnail->GetSource(), hWnd);

  return (hWnd == m_Thumbnail->GetSource());
}

void ApplicationManager::CreateThumbnailWindow(HWND hWnd, int x, int y)
{
  if (IsThumbnailRegistered())
    return;

  if (IsWindow(hWnd))
    m_Thumbnail->CreateThumbnailFromSource(hWnd, x, y);
}

void ApplicationManager::CloseThumbnailWindow()
{
  if (!IsThumbnailRegistered())
    return;

  m_Thumbnail->DestroyThumbnailWindow();
}


// EOF