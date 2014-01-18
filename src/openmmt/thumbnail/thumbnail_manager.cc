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
#include "openmmt/precompiled_headers.h"
#include "openmmt/global_variables.h"
#include "openmmt/taskbar/taskbar.h"
#include "openmmt/thumbnail/thumbnail_manager.h"
#include "openmmt/option_variables.h"

ThumbnailManager::ThumbnailManager() :
  m_iThumbnailPosX(0),
  m_iThumbnailPosY(0),
  m_bThumbnailPresent(FALSE),
  m_hWndButtonSource(NULL)
{
}

ThumbnailManager::~ThumbnailManager()
{

}

BOOL ThumbnailManager::isThumbnailPresent()
{
  return m_bThumbnailPresent;
}

BOOL ThumbnailManager::isThumbnailed(ButtonPtr pBtn)
{
  return isThumbnailed(pBtn->GetButtonHandle());
}

BOOL ThumbnailManager::isThumbnailed(HWND hWndButton)
{
  return (m_hWndButtonSource == hWndButton);
}

void ThumbnailManager::CreateThumbnail(ButtonPtr pBtn)
{
  if (isThumbnailPresent())
    return;

  POINT pt = {0};

  CalcThumbnailWindowPos(pBtn, &pt);
  ClientToScreen(pBtn->GetTaskbarHandle(), &pt);

  m_bThumbnailPresent = TRUE;
  m_hWndButtonSource  = pBtn->GetButtonHandle();
  m_pThumbnail = ThumbnailPtr(new Thumbnail());
  m_pThumbnail->CreateThumbnailFromSource(pBtn->GetAppHandle(), pt.x, pt.y);

}

void ThumbnailManager::DestroyThumbnail()
{
  if (!isThumbnailPresent())
    return;

  m_pThumbnail->DestroyThumbnailWindow();
  m_pThumbnail = ThumbnailPtr();
  m_hWndButtonSource  = NULL;
  m_bThumbnailPresent = FALSE;
}

const HWND ThumbnailManager::GetThumbnailWindow()
{
  return m_pThumbnail->GetHandle();
}

const HWND ThumbnailManager::GetThumbnailedButton()
{
  return m_hWndButtonSource;
};

void ThumbnailManager::CalcThumbnailWindowPos(ButtonPtr pBtn, LPPOINT lpPoints)
{
  TaskbarPtr pBar(g_pMonitorManager->FindMonitorTaskbar(pBtn->GetTaskbarHandle()));

  if (pBar == TaskbarPtr())
    return;

  lpPoints->x = pBtn->GetX();
  lpPoints->y = pBtn->GetY();

  switch (pBar->GetPosition()) {
  case TASKBAR_BOTTOM:
    lpPoints->y -= 175 + g_iOptions_ThumbnailSpacer;
    break;

  case TASKBAR_TOP:
    lpPoints->y += 40 + g_iOptions_ThumbnailSpacer;
    break;

  case TASKBAR_RIGHT:
    lpPoints->x -= 235 + g_iOptions_ThumbnailSpacer;
    break;

  case TASKBAR_LEFT:
    lpPoints->x = 60 + g_iOptions_ThumbnailSpacer;
    break;
  }

  if (lpPoints->x == 0)
    lpPoints->x = 5;

  if (lpPoints->y == 0)
    lpPoints->y = 5;
}


// EOF


