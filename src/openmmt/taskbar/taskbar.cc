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
#include "openmmt/taskbar/taskbar.h"
#include "openmmt/windows/os.h"
#include "openmmt/windows/windows.h"

static const INT kDefaultTaskbarHorizontalHeight = 40;
static const INT kDefaultTaskbarVerticalWidth    = 62;

Taskbar::Taskbar() :
  m_hWnd(NULL),
  m_X(0),
  m_Y(0),
  m_Width(0),
  m_Height(0),
  m_Position(TASKBAR_BOTTOM),
  m_bAlwaysOnTop(FALSE),
  m_bAutoHide(FALSE),
  m_bFullScreen(FALSE)
{
  ZeroMemory(&m_AppBarRect, sizeof(RECT));
}

Taskbar::~Taskbar()
{
  dprintf("Taskbar %08x is closing.\n", m_hWnd);

  WndCheckAndDestroy(m_hWnd);

  if (m_hTheme)
    CloseThemeData(m_hTheme);

  UnregisterAppBar();
}

UINT Taskbar::GetTaskbarEdge()
{
  switch (m_Position)
  {
  case TASKBAR_BOTTOM:
    return ABE_BOTTOM;
  case TASKBAR_LEFT:
    return ABE_LEFT;
  case TASKBAR_TOP:
    return ABE_TOP;
  case TASKBAR_RIGHT:
    return ABE_RIGHT;
  default:
    return ABE_BOTTOM;
  }
}
void Taskbar::RegisterAppBar()
{
  APPBARDATA bar = {0};
  bar.cbSize = sizeof(APPBARDATA);
  bar.hWnd = m_hWnd;
  bar.uCallbackMessage = g_uiAppBarMsg;

  // Register the app bar and get ready to request a position if the
  // registration is successful.
  if ((BOOL)SHAppBarMessage(ABM_NEW, &bar) == TRUE) {
    dprintf("AppBar created. %08X\n", m_hWnd);

    bar.uEdge = GetTaskbarEdge();
    bar.rc = m_AppBarRect;
    SHAppBarMessage(ABM_QUERYPOS, &bar);

    // Check if the position is approved. However, because I am unsure 
    // about how this would react with more than 1 monitor, I am going 
    // to skip this check and go ahead and set the AppBar Pos.
    if ((m_AppBarRect.left == bar.rc.left)  && 
      (m_AppBarRect.top    == bar.rc.top)   && 
      (m_AppBarRect.right  == bar.rc.right) &&
      (m_AppBarRect.bottom == bar.rc.bottom)) {
        dprintf("AppBar rect approved for Taskbar %08X\n", m_hWnd);
    }
    else {
      dprintf("AppBar rect was not approved for Taskbar %08X\n", m_hWnd);
    }
    SHAppBarMessage(ABM_SETPOS, &bar);
    SHAppBarMessage(ABM_ACTIVATE, &bar);
  }
}

void Taskbar::UnregisterAppBar()
{
  APPBARDATA bar = {0};
  bar.cbSize = sizeof(APPBARDATA);
  bar.hWnd = m_hWnd;

  SHAppBarMessage(ABM_REMOVE, &bar);
  dprintf("Appbar Unregistered.\n");
}

void Taskbar::SetWorkSpace(LPRECT rcMonitor, LPRECT rcWork, INT mPosition)
{
  BOOL bHorizontal = true;
  m_Position = mPosition;

  // Horizontal alignment.
  if ((m_Position == TASKBAR_TOP) || (m_Position==TASKBAR_BOTTOM)) {
    m_Height = (g_WindowsTaskbarPos == TASKBAR_BOTTOM) || 
      (g_WindowsTaskbarPos == TASKBAR_TOP) ? g_WindowsTaskbarHeight : 
      kDefaultTaskbarHorizontalHeight;
    m_Width  = rcMonitor->right - rcMonitor->left;
    m_X      = rcMonitor->left;

    if (m_Position == TASKBAR_BOTTOM) {
      rcWork->bottom = rcMonitor->bottom - m_Height;
      m_Y = rcWork->bottom;
    } else {
      rcWork->top = rcMonitor->top + m_Height;
      m_Y = rcMonitor->top;
    }
  } else {
    // Vertical alignment.
    bHorizontal = false;
    m_Height    = rcMonitor->bottom - rcMonitor->top;
    m_Width     = (g_WindowsTaskbarPos == TASKBAR_LEFT) ||
      (g_WindowsTaskbarPos == TASKBAR_RIGHT) ? g_WindowsTaskbarWidth : 
      kDefaultTaskbarVerticalWidth;
    m_Y         = rcMonitor->top;

    if (m_Position == TASKBAR_LEFT) {
      rcWork->left = rcMonitor->left + m_Width;
      m_X = rcMonitor->left;
    } else {
      rcWork->right = rcMonitor->right - m_Width;
      m_X = rcWork->right;
    }
  }

  m_AppBarRect.left   = rcWork->left;
  m_AppBarRect.top    = rcWork->top;
  m_AppBarRect.right  = rcWork->right;
  m_AppBarRect.bottom = rcWork->bottom;

  SetDimensions(m_Width, m_Height);
  SetLayout(bHorizontal);

  // If the taskbar window already exists, then we need to change the
  // position and size of the taskbar accordingly. Also, we need to make
  // sure that we release the Direct2D device resources. We all so need
  // to call the method RedrawButtons() from the ButtonManager interface
  // so the buttons will be repositioned and their size/position will be
  // updated as well.
  if (IsWindow(m_hWnd)) {
    SetWindowPos(m_hWnd, HWND_TOPMOST, m_X, m_Y, m_Width, m_Height, 
      SWP_SHOWWINDOW);

    ReleaseDeviceResources();
    InvalidateRect(m_hWnd, NULL, TRUE);
    RedrawButtons();
  }
}

void Taskbar::Initialize()
{
  CreateTaskbar();
  RegisterAppBar();

  // We want to exclude the taskbar from being hidden when Aero Peek
  // is activated. For some odd reason, "DWMWA_EXCLUDED_FROM_PEEK" is
  // listed on MSDN but not defined on the W7 SDK that I have. Therefore,
  // I am going to use DWMA_LAST since its the equivalent of the 
  // DWMWA_EXCLUDED_FROM_PEEK macro.
  DWMWINDOWATTRIBUTE ncrp = DWMWA_LAST;
  DwmSetWindowAttribute(m_hWnd, DWMWA_LAST, &ncrp, sizeof(ncrp));

  // Paint the background in case the user does not have any applications
  // running in this monitor region.
  OnEraseBackground();
}

void Taskbar::AddApplication(ApplicationPtr app)
{
  AddObject(app);
  CreateButton(app);
}

void Taskbar::RemoveApplication(ApplicationPtr app)
{
  if (app == ApplicationPtr())
    return;
  
  ButtonPtr btn = GetButton(app);

  if (btn != ButtonPtr())
    RemoveButton(btn);

  RemoveObject(app);
}

void Taskbar::ClearApplications()
{
  // Setting this to TRUE will stop the ButtonManager interface from doing
  // any further drawing or re-arranging of buttons.
  m_bClosing = TRUE;
  vector_.clear();
}

void Taskbar::CreateTaskbar()
{ 
  m_hWnd = CreateWindowEx(WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR|\
    WS_EX_TOOLWINDOW|WS_EX_TOPMOST|WS_EX_CONTROLPARENT, g_TaskbarClassName, NULL, 
    WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_POPUP, m_X, m_Y, m_Width, m_Height, NULL, NULL, 
    g_hInstance, NULL);

  // TODO: This should not happen, but even so, we must find a way to properly
  // notify the user and perhaps try again.
  if (!m_hWnd) {
    MessageBox(NULL, L"Could not create taskbar.", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  OnThemeChange();
  ShowWindow(m_hWnd, SW_SHOW);
}

const LONG Taskbar::GetWidth()
{
  return m_Width;
}

const LONG Taskbar::GetHeight()
{
  return m_Height;
}

const HWND Taskbar::GetWindowHandle()
{
  return m_hWnd;
}

const INT Taskbar::GetPosition()
{
  return m_Position;
}


BOOL Taskbar::IsPosition(INT pos)
{
  return (m_Position == pos);
}

BOOL Taskbar::IsAutoHidden()
{
  return m_bAutoHide;
}

void Taskbar::SetPosition(INT pos)
{
  m_Position = pos;
}


void Taskbar::CreateExtraResources()
{

}

void Taskbar::ReleaseExtraResources()
{

}

void Taskbar::OnEraseBackground()
{
  // If the device is not set, we reset it.
  if (!IsDeviceSet()) 
    SetDevice(m_hWnd);

  // Begin painting.
  if (SUCCEEDED(CreateDeviceResources())) {
    BeginDraw();

    WndSetBlur(m_hWnd, true);
    m_pRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));
 
    if (SUCCEEDED(QueryDeviceContext())) {
      // Declare the device context and get it.
      HDC hDC = {0};
      HRESULT hr = m_pInteropTarget->GetDC(D2D1_DC_INITIALIZE_MODE_CLEAR, &hDC);

      if (SUCCEEDED(hr)) {
        if (m_hTheme) {
          // Check to see if the taskbar theme is transparent. If it is,
          // we will draw the transparent background portion of the theme.
          if (IsThemeBackgroundPartiallyTransparent(m_hTheme, m_Position, 0)) 
            DrawThemeParentBackground(m_hWnd, hDC, &m_rcDeviceRect);

          DrawThemeBackground(m_hTheme, hDC, m_Position, 0, &m_rcDeviceRect, 0);
        } else {
          // If we failed to open a theme for some odd reason, we will
          // fall back to the system defaults. This usually means that
          // the user is using a legacy theme like the Windows Classic
          // or one of the high contrast ones.
          FillRect(hDC, &m_rcDeviceRect, GetSysColorBrush(COLOR_3DFACE));

          UINT grfFlags = BF_TOP;

          if (m_Position == TASKBAR_LEFT)
            grfFlags = BF_RIGHT;
          else if (m_Position == TASKBAR_TOP)
            grfFlags = BF_BOTTOM;
          else if (m_Position == TASKBAR_RIGHT)
            grfFlags = BF_LEFT;

          DrawEdge(hDC, &m_rcDeviceRect, BDR_RAISED, grfFlags);
        }
      }
      // Release the device context.
      m_pInteropTarget->ReleaseDC(NULL);
    }
    EndDraw();
  }
}

void Taskbar::OnPaint()
{

}

// A dirty way to delegate the composition change message.
static BOOL CALLBACK ChildNotification(HWND hWnd, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  PostMessage(hWnd, WM_DWMCOMPOSITIONCHANGED, NULL, NULL);
  return TRUE;
}

void Taskbar::OnThemeChange()
{
  if (m_hTheme)
    CloseThemeData(m_hTheme);

  if (OSIsWin7Server08R2()) {
    if (IsCompositionActive()) {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBAR2COMPOSITED::TASKBAR");
    } else {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBAR2::TASKBAR");
    }
  } else if (OSIsServer08Vista()) {
    if (IsCompositionActive()) {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBARCOMPOSITED::TASKBAR");
    } else {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBAR");
    }
  }

  // Notify the child windows that something has changed. The reason that we
  // do this, is the message WM_DWMCOMPOSITIONCHANGED does not get sent to
  // child windows, only the parents. As such, we need to pass down the message
  // to the windows acting as taskbar buttons so they can properly set the
  // necessary theme.
  EnumChildWindows(m_hWnd, &ChildNotification, NULL);

  // TODO: Find a way to properly notify the user. Perhaps via a tray notice?
  // or maybe create a separate window for log messages.
  if (!m_hTheme) {
    dprintf("COULD NOT OPEN THEME\n");
  }
}

void Taskbar::SetAutoHide(BOOL bAutoHide)
{
  m_bAutoHide    = bAutoHide;

#if 0
  APPBARDATA sAppBar = {0};
  sAppBar.cbSize = sizeof(APPBARDATA);
  sAppBar.hWnd   = m_hWnd;
  sAppBar.uEdge  = GetTaskbarEdge();
  sAppBar.lParam = (LPARAM)m_bAutoHide;

  if (bAutoHide) {
    sAppBar.lParam |= ABS_AUTOHIDE;

    UnregisterAppBar();
    SHAppBarMessage(ABM_SETAUTOHIDEBAR, &sAppBar);
  } else {
    RegisterAppBar();
  }
  if (m_bAlwaysOnTop)
    sAppBar.lParam |= ABS_ALWAYSONTOP;

  SHAppBarMessage(ABM_SETSTATE, &sAppBar);
#endif
}

void Taskbar::SetAlwaysOnTop(BOOL bAlwaysOnTop)
{
  APPBARDATA sAppBar = {0};

  sAppBar.cbSize = sizeof(APPBARDATA);
  sAppBar.hWnd   = m_hWnd;

  if (bAlwaysOnTop)
    sAppBar.lParam |= ABS_ALWAYSONTOP;
  if (m_bAutoHide)
    sAppBar.lParam |= ABS_AUTOHIDE;

  m_bAlwaysOnTop = bAlwaysOnTop;
  SHAppBarMessage(ABM_SETSTATE, &sAppBar);
}

void Taskbar::AppEnterFullScreen(HWND hWnd)
{
  if (m_bFullScreen)
    return;

  dprintf("%08X has entered full screen.\n", hWnd);

  SetWindowPos(m_hWnd, HWND_BOTTOM, m_X, m_Y, m_Width, m_Height, 
    SWP_HIDEWINDOW);
  m_bFullScreen = TRUE;
}

void Taskbar::AppLeaveFullScreen(HWND hWnd)
{
  if (!m_bFullScreen)
    return;

  dprintf("%08X has left full screen.\n", hWnd);

  SetWindowPos(m_hWnd, HWND_TOPMOST, m_X, m_Y, m_Width, m_Height, 
    SWP_SHOWWINDOW);
  m_bFullScreen = FALSE;
}

// EOF
