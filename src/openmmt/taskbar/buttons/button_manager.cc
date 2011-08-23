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
#include "openmmt/applications/application.h"
#include "openmmt/taskbar/buttons/button_manager.h"

static const INT m_iDefaultButtonSpacing = 2;

ButtonManager::ButtonManager() :
  m_bClosing(FALSE),
  m_TotalButtons(0),
  m_hWndLastActive(NULL),
  m_hTheme(NULL)
{
}

ButtonManager::~ButtonManager()
{
}

ButtonPtr ButtonManager::GetButton(int index)
{
  std::map<int, ButtonPtr>::const_iterator it = buttons_.find(index);
  return it->second;
}

ButtonPtr ButtonManager::GetButton(HWND hWnd)
{
  for (std::map<int, ButtonPtr>::const_iterator it = buttons_.begin();
    it != buttons_.end(); ++it) {
      if (it->second->GetButtonHandle() == hWnd) {
        return it->second;
      }
  }
  return ButtonPtr();
}

ButtonPtr ButtonManager::GetButton(ApplicationPtr app)
{
  if (app == ApplicationPtr()) 
    return ButtonPtr();
  
  for (std::map<int, ButtonPtr>::const_iterator it = buttons_.begin();
    it != buttons_.end(); ++it) {
      if (it->second->GetAppHandle() == app->GetId()) {
        return it->second;
      }
  }
  return ButtonPtr();
}

ButtonPtr ButtonManager::GetButtonForApp(HWND hWnd)
{
  for (std::map<int, ButtonPtr>::const_iterator it = buttons_.begin();
    it != buttons_.end(); ++it) {
      if (it->second->GetAppHandle() == hWnd) {
        return it->second;
      }
  }
  return ButtonPtr();
}

INT ButtonManager::GetCoordinatesAtIndex(INT index)
{
  return (index * ((m_bHorizontal == TRUE ? m_BtnWidth : m_BtnHeight) + m_iDefaultButtonSpacing));
}

void ButtonManager::CreateButton(ApplicationPtr app)
{
  ButtonPtr btn;

  if (app->GetTaskbar() == TaskbarPtr()) {
    MessageBox(NULL, L"Could not retrieve taskbar for an application", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  // Check to see if we have reached the maximum number of alloted buttons.
  // If so, it's time that we reshape.
  if ((m_TotalButtons+1) >= m_MaxButtons) {

  }

  if (m_bHorizontal) {
    btn = ButtonPtr(new Button(app->GetId(), app->GetTaskbar()->GetWindowHandle(), 
      GetCoordinatesAtIndex(m_TotalButtons), 0, m_BtnWidth, m_BtnHeight, m_TotalButtons));
  } else {
    btn = ButtonPtr(new Button(app->GetId(), app->GetTaskbar()->GetWindowHandle(), 
      0, GetCoordinatesAtIndex(m_TotalButtons), m_BtnWidth, m_BtnHeight, m_TotalButtons));
  }

  buttons_.insert(ButtonPair(m_TotalButtons++, btn));
  btn->CreateButton();
  UpdateWindow(btn->GetButtonHandle());
}

void ButtonManager::RemoveButton(ButtonPtr btn)
{
  if (btn == ButtonPtr())
    return;

  if (g_pAppManager->IsThumbnailed(btn->GetAppHandle())) 
    g_pAppManager->CloseThumbnailWindow();
  
  std::map<int, ButtonPtr>::iterator it = buttons_.find(btn->GetIndex());
  buttons_.erase(it);
  m_TotalButtons--;
}

void ButtonManager::ReIndexButtons()
{
  if (m_bClosing)
    return;

  std::map<int, ButtonPtr> tmpButtons_;
  int cur_index = 0;
  for (std::map<int, ButtonPtr>::iterator it = buttons_.begin();
    it != buttons_.end(); ++it) {
      if (cur_index != it->first) {
        m_TotalButtons--;
        ButtonPtr btn_next = GetButton(cur_index+1);
        if (btn_next != ButtonPtr()) {
          tmpButtons_.insert(ButtonPair(cur_index, btn_next));
          btn_next->SetIndex(cur_index);
          m_TotalButtons++;
        }
      } else {
        tmpButtons_.insert(ButtonPair(cur_index, it->second));
      }
      cur_index++;
  }

  // Gotta love smart pointers. Boost++;
  buttons_ = tmpButtons_;
  RedrawButtons();
}

void ButtonManager::RedrawButtons()
{
  for (std::map<int, ButtonPtr>::iterator it = buttons_.begin();
    it != buttons_.end(); ++it) {
      if (m_bHorizontal) {
        it->second->SetPoints(GetCoordinatesAtIndex(it->second->GetIndex()), 0);
        SetWindowPos(it->second->GetButtonHandle(), HWND_TOP, 
          GetCoordinatesAtIndex(it->second->GetIndex()), 0, m_BtnWidth, m_BtnHeight, 
          SWP_NOACTIVATE|SWP_NOOWNERZORDER);
      } else {
        it->second->SetPoints(0, GetCoordinatesAtIndex(it->second->GetIndex()));
        SetWindowPos(it->second->GetButtonHandle(), HWND_TOP, 
          0, GetCoordinatesAtIndex(it->second->GetIndex()), m_BtnWidth, m_BtnHeight, 
          SWP_NOACTIVATE|SWP_NOOWNERZORDER);
      }

      // We want to release the resources created by the device so they
      // can be recreated once again with the proper dimensions.
      it->second->ReleaseExtraResources();
      it->second->ReleaseDeviceResources();
      InvalidateRect(it->second->GetButtonHandle(), NULL, true);
  }
}

void ButtonManager::SetDimensions(LONG mWidth, LONG mHeight)
{
  m_Width  = mWidth;
  m_Height = mHeight;

  dprintf("Dimensions changed to %ldx%ld\n", mWidth, mHeight);
}

void ButtonManager::SetLayout(BOOL bHorizontal)
{
  m_bHorizontal = bHorizontal;

  // TODO: FFS. Clean this up.
  if (g_pMonitorManager->IsFunkeyDPI()) {
    m_BtnWidth  = !m_bHorizontal ? m_Width : 70;
    m_BtnHeight = m_bHorizontal ? m_Height : 50;
  } else {
    m_BtnWidth  = !m_bHorizontal ? m_Width : 60;
    m_BtnHeight = m_bHorizontal ? m_Height : 45;
  }

  m_MaxButtons = (bHorizontal ? (m_Width / m_BtnWidth) : 
    (m_Height / m_BtnHeight));

  if (!m_hTheme) {
      m_BtnHeight -= 2;
      m_BtnWidth  -= 2;
  }

  if (g_bUsingSmallIcons) {
    if (m_bHorizontal)
      m_BtnWidth  -= 10;
    else
      m_BtnHeight -= 10;
  }

  dprintf("Button dimensions changed to %ldx%ld\n", m_BtnWidth, m_BtnHeight);
}

void ButtonManager::ActivateApp(HWND hWnd)
{
  ButtonPtr btn;

  if (!m_hWndLastActive) {
    m_hWndLastActive = hWnd;
  } else if (m_hWndLastActive == hWnd) {
    return;
  }

  ButtonPtr old_btn = GetButtonForApp(m_hWndLastActive);

  if (old_btn != ButtonPtr()) {
    old_btn->ClearState(BTN_ACTIVE);
    InvalidateRect(old_btn->GetButtonHandle(), NULL, TRUE);
  }

  btn = GetButtonForApp(hWnd);

  if (btn != ButtonPtr()) {
    btn->AddState(BTN_ACTIVE);
    m_hWndLastActive = hWnd;
  }
  
}

void ButtonManager::SetFirstActive()
{
  if (buttons_.empty())
    return;

  ButtonPtr pBtn(buttons_.begin()->second);

  if (pBtn != ButtonPtr()) 
    ActivateApp(pBtn->GetAppHandle());
}

// EOF
