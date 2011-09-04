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
#include "openmmt/thumbnail/thumbnail.h"
#include "openmmt/option_variables.h"

UINT IDT_THUMBNAIL_DESTROY_TIMER = 5;

Thumbnail::Thumbnail() :
  m_hThumbnail(NULL),
  m_hWndSource(NULL),
  m_hWnd(NULL), 
  m_bRegistered(FALSE)
{
 
}

Thumbnail::~Thumbnail()
{
  UnregisterThumbnail();
}

CONST HWND Thumbnail::GetHandle()
{
  return m_hWnd;
}

CONST HWND Thumbnail::GetSource()
{
  return m_hWndSource;
}

BOOL Thumbnail::IsThumbnailRegistered()
{
  return m_bRegistered;
}

void Thumbnail::CreateThumbnailFromSource(HWND hWnd, int x, int y)
{
  if (m_bRegistered)
    return;

  m_bRegistered = TRUE;
  m_hWndSource  = hWnd;

  CreateThumbnailWindow(x, y);
  RegisterThumbnail();
}

void Thumbnail::DestroyThumbnailWindow()
{
  // Sanity check
  KillTimer(m_hWnd, IDT_THUMBNAIL_DESTROY_TIMER);
  UnregisterThumbnail();
  DestroyWindow(m_hWnd);

  m_hWndSource = NULL;
  m_hWnd = NULL;
  m_bRegistered = FALSE;
}

void Thumbnail::CreateThumbnailWindow(INT x, INT y)
{
  if (IsWindow(m_hWnd))
    return;

  std::vector<wchar_t> m_WindowTitle(GetWindowTextLength(m_hWndSource)+5);
  GetWindowText(m_hWndSource, &m_WindowTitle[0], (int)m_WindowTitle.size());

  m_hWnd = CreateWindowEx(WS_EX_WINDOWEDGE|WS_EX_TOOLWINDOW|WS_EX_TOPMOST, g_ThumbnailClassname, &m_WindowTitle[0],
    WS_OVERLAPPED|WS_CAPTION, x, y, 235, 175, NULL, NULL, g_hInstance, NULL);

  if (!m_hWnd) {
    MessageBox(NULL, L"Could not create thumbnail window", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  // ApplicationPtr app = g_pAppManager->FindApplication(m_hWndSource);

  // if (app != ApplicationPtr()) 
  //  SendMessage(m_hWnd, WM_SETICON, ICON_SMALL, (LPARAM) app->GetIcon());
}

void Thumbnail::RegisterThumbnail()
{
  HRESULT hr = DwmRegisterThumbnail(m_hWnd, 
    m_hWndSource, &m_hThumbnail);

  if (!SUCCEEDED(hr))
    return;

  SIZE pSize;
  hr = DwmQueryThumbnailSourceSize(m_hThumbnail, &pSize);

  if (!SUCCEEDED(hr))
    return;

  RECT destRect = { 0, 0, 235, 175};

  DWM_THUMBNAIL_PROPERTIES dskThumbProps;
  dskThumbProps.dwFlags = DWM_TNP_SOURCECLIENTAREAONLY | DWM_TNP_VISIBLE | DWM_TNP_OPACITY | DWM_TNP_RECTDESTINATION;
  dskThumbProps.fSourceClientAreaOnly = FALSE; 
  dskThumbProps.fVisible = TRUE;
  dskThumbProps.opacity = 255;
  dskThumbProps.rcDestination = destRect;

  DwmUpdateThumbnailProperties(m_hThumbnail, &dskThumbProps);

  if (g_bOptions_EnableThumbnailAnimation)
    AnimateWindow(m_hWnd, 200, AW_BLEND);
  else
    ShowWindow(m_hWnd, SW_SHOW);
}

void Thumbnail::UnregisterThumbnail()
{
  DwmUnregisterThumbnail(m_hThumbnail);
}

// EOF
