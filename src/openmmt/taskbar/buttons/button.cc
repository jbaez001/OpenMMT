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
#include "openmmt/taskbar/buttons/button.h"
#include "openmmt/windows/os.h"
#include "openmmt/windows/windows.h"

static HWND GetProperHandle(HWND hWnd)
{
  HWND hwParentWindow = (HWND) GetWindowLongPtr(hWnd, GWLP_HWNDPARENT);

  // If the child or descendant window controls the parent, then we must
  // control the parent's icon in the Windows taskbar instead of a child
  // one that doesn't even probably exist.
  if ((hwParentWindow) && (GetWindowLongPtr(hWnd, GWL_EXSTYLE) & 
    WS_EX_CONTROLPARENT))
    return hwParentWindow;

  return hWnd;
}

Button::Button(HWND hWndApp, HWND hWndTaskbar, INT x, INT y, 
               INT width, INT height, int index) :
  m_hWnd(NULL),
  m_hWndApp(hWndApp),
  m_hWndTaskbar(hWndTaskbar),
  m_Width(width),
  m_Height(height),
  m_X(x),
  m_Y(y),
  m_Index(index),
  m_bMayHoover(FALSE),
  m_bFlashing(FALSE),
  m_pIconBmp(NULL),
  m_btnState(BTN_NORMAL),
  m_hIcon(WndGetApplicationIcon(hWndApp)),
  m_hTheme(NULL),
  m_AverageColor(NULL),
  m_pBitmap(NULL)
{
}

Button::~Button()
{
  if (m_hTheme)
    CloseThemeData(m_hTheme);

  if (IsWindow(m_hWnd))
    DestroyWindow(m_hWnd);

  ReleaseExtraResources();

  if (IsWindow(m_hWndApp))
    g_pWinTaskbar->AddButton(GetProperHandle(m_hWndApp));
}

const HWND Button::GetId()
{
  return GetButtonHandle();
}

const HWND Button::GetButtonHandle()
{
  return m_hWnd;
}

const HWND Button::GetAppHandle()
{
  return m_hWndApp;
}

const HWND Button::GetTaskbarHandle()
{
  return m_hWndTaskbar;
}

const INT Button::GetIndex()
{
  return m_Index;
}

const INT Button::GetX()
{
  return m_X;
}

const INT Button::GetY()
{
  return m_Y;
}

void Button::SetPoints(INT x, INT y)
{
  m_X = x;
  m_Y = y;
}

void Button::CreateButton()
{
  m_hWnd = CreateWindowEx(WS_EX_LEFT|WS_EX_LTRREADING|WS_EX_RIGHTSCROLLBAR, 
    g_TaskbarButtonClassName, NULL, WS_CHILD|WS_CLIPSIBLINGS|\
    WS_CLIPCHILDREN|WS_MAXIMIZEBOX, m_X, m_Y, m_Width, m_Height, m_hWndTaskbar, 
    NULL, g_hInstance, NULL);

  // TODO: Research a better way to notify the user.
  if (!m_hWnd) {
    MessageBox(NULL, L"Could not create button window", L"OpenMMT", MB_OK|MB_ICONERROR);
    abort();
  }

  OnThemeChange();

  ShowWindow(m_hWnd, SW_SHOW);

  if (IsWindow(m_hWndApp)) {
    g_pWinTaskbar->RemoveButton(GetProperHandle(m_hWndApp));
  }

  if (m_hWndApp == GetForegroundWindow()) {
    TaskbarPtr bar(g_pMonitorManager->FindMonitorTaskbar(m_hWndApp));
    if (bar != TaskbarPtr()) {
      bar->ActivateApp(m_hWndApp);
    }
  }
}

void Button::OnMouseMove()
{
  if (!m_bMayHoover) {
    m_bMayHoover = true;
    TrackMouseHoover();
  }
}

void Button::TrackMouseHoover()
{
  TRACKMOUSEEVENT mousey = {0};
  mousey.cbSize = sizeof(TRACKMOUSEEVENT);
  mousey.dwFlags = TME_HOVER|TME_LEAVE;
  mousey.dwHoverTime = HOVER_DEFAULT;
  mousey.hwndTrack = m_hWnd;

  _TrackMouseEvent(&mousey);
}

void Button::OnMouseHoover()
{

}

void Button::OnMouseLeave()
{
  m_bMayHoover = false;
}

void Button::AddState(INT state)
{
  if (!IsState(state))
    m_btnState |= state;
  
  if ((state & BTN_NORMAL) && IsState(BTN_PRESSED)) {
    ClearState(BTN_PRESSED);
  } else if ((state & BTN_PRESSED) && IsState(BTN_NORMAL)) {
    ClearState(BTN_NORMAL);
  }

  // If the button is flashing, stop it.
  if (m_bFlashing)
    m_bFlashing = FALSE;

  InvalidateRect(m_hWnd, NULL, TRUE);
}

void Button::SetIndex(INT index)
{
  m_Index = index;
}

BOOL Button::IsState(int state)
{
  if (m_btnState & state)
    return TRUE;

  return FALSE;
}

void Button::ClearState(INT state)
{
  if (IsState(state)) {
    m_btnState &= ~state;
  }
}

void Button::CreateExtraResources()
{
  if (m_pBitmap)
    return;

  m_hIcon = WndGetApplicationIcon(m_hWndApp);

  if (SUCCEEDED(m_pWICFactory->CreateBitmapFromHICON(m_hIcon, &m_pIconBmp))) {
    IWICFormatConverter *pConverter = NULL;
    HRESULT hr = m_pWICFactory->CreateFormatConverter(&pConverter);

    if (SUCCEEDED(hr)) {
      // Convert icon to 32bppPBGRA.
      hr = pConverter->Initialize(m_pIconBmp, GUID_WICPixelFormat32bppPBGRA,
        WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeMedianCut);
    }
    if (SUCCEEDED(hr)) {
      hr = m_pRenderTarget->CreateBitmapFromWicBitmap(pConverter, &m_pBitmap);
    }
    SafeRelease(&m_pIconBmp);
    SafeRelease(&pConverter);
  }
}
void Button::ReleaseExtraResources()
{
  SafeRelease(&m_pIconBmp);
  SafeRelease(&m_pBitmap);
}

void Button::OnEraseBackground()
{
  // Nothing to do here.
}

void Button::OnPaint()
{
  
  // Check to see if the application that the button represents is still
  // valid. If not, get rid of the button. It really shouldn't come down
  // to this. If this procedure is being called as such, then something
  // is wrong.
  if (!IsWindow(m_hWndApp)) {

    TaskbarPtr bar(g_pMonitorManager->FindMonitorTaskbar(m_hWndTaskbar));

    if (bar != TaskbarPtr())
      bar->RemoveButton(bar->GetButtonFromApp(m_hWndApp));

    // No need to paint, so we split here.
    return;
  }

  if (!IsDeviceSet())
    SetDevice(m_hWnd);

  if (SUCCEEDED(CreateDeviceResources())) {
    // TODO: We need to implement error checks on CreateExtraResources,
    // then proceed with this method based on the results.
    CreateExtraResources();

    // Get the image size and create a rectangle to position it on.
    D2D1_SIZE_F imgSize  = m_pBitmap->GetSize();
    int factor = g_pMonitorManager->IsFunkeyDPI() ? 30 : 32;

    // If the icons are not FxF, then scale them we must.
    //   New Width  = (ImgWidth  * (Max Width  / ImgWidth))
    //   New Height = (ImgHeight * (Max Height / ImgHeight))
    // So far the only application with an icon larger than 32x32 that I've
    // found was Adobe Photoshop.
    if ((imgSize.width > factor) || (imgSize.height > factor)) {
      imgSize.width  *= (factor / imgSize.width);
      imgSize.height *= (factor / imgSize.height);
    }

    // The size of the taskbar and buttons is a lot smaller than that of
    // Windows 7 and Windows 2008 Server R2. Therefore, I believe that we
    // should reduce the icon size from 32x32 to 16x16 to properly fit
    // in the taskbar buttons.
    //
    // On another note, if for some reason the user is running Windows 7,
    // and for some ungodly reason, this same user commits blasphemy by
    // using the small taskbar on Windows 7, we need to do this as well.
    if (OSIsServer08Vista() || g_bUsingSmallIcons) {
      imgSize.width  /= 2;
      imgSize.height /= 2;
    }

    FLOAT x = 0, y = 0;
    if (g_pMonitorManager->IsFunkeyDPI()) {
      x = ((m_rcDeviceRect.right - imgSize.width) / 3);
      y = ((m_rcDeviceRect.bottom - imgSize.height) / 4);
    } else {
      x = ((m_rcDeviceRect.right - imgSize.width) / 2);
      y = ((m_rcDeviceRect.bottom - imgSize.height) / 2);
    }

    // If the button is pressed, let's increment the coordinates by 1 to
    // give it a "feel" of being "pushed."
    if (m_btnState & BTN_PRESSED) {
      x++;
      y++;
    }

    // Destination in which we're going to place the rectangle on.
    D2D1_RECT_F destRect = {0};
    destRect.left   = x;
    destRect.top    = y;
    destRect.right  = (x + imgSize.width);
    destRect.bottom = (y + imgSize.height);

    // We are ready to start rendering.
    BeginDraw();

    m_pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
    m_pRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));

    // Query the ID2D1GdiInteropRenderTarget interface.
    if (SUCCEEDED(QueryDeviceContext())) {
      HDC hDC = {0};
      // Get the device context so we can use UxTheme functions.
      if (SUCCEEDED(m_pInteropTarget->GetDC(D2D1_DC_INITIALIZE_MODE_CLEAR, &hDC))) {
        if (m_hTheme) {
          // Themes:
          //    TaskBand2Composite::TaskBand2
          //    TaskBand2
          //
          // Basic Parts:
          //    1   = BackgroundBottom
          //    2   = BackgroundTop
          //    3   = BackgroundLeft
          //    4   = BackgroundRight
          //    5   = TaskItemButton
          //
          // TaskItemButton States:
          //    1   = Normal
          //    2   = Hover
          //    3   = Pressed
          //    4   = NormalSelected
          //    5   = HoverSelected
          //    6   = PressedSelected
          //    7   = FlashButton
          //    8   = NormalPinned
          //    9   = HoverPinned
          //    10  = PressedPinned
          int iPardId  = 5; // TaskItemButton
          int iStateId = 1;

          if (IsState(BTN_ACTIVE)) 
            iStateId = IsState(BTN_PRESSED) ? 6 : 4;

          if (IsThemeBackgroundPartiallyTransparent(m_hTheme, iPardId, iStateId))
            DrawThemeParentBackground(m_hWnd, hDC, &m_rcDeviceRect);
         
          // TODO: Come up with a way to draw the correct background instead
          // of always just the bottom.
          DrawThemeBackground(m_hTheme, hDC, 1, 0, &m_rcDeviceRect, 0);
          DrawThemeBackground(m_hTheme, hDC, iPardId, iStateId, &m_rcDeviceRect, 0);

          // Button is flashing.
          if (m_bFlashing)
            DrawThemeBackground(m_hTheme, hDC, iPardId, 7, &m_rcDeviceRect, 0);

        } else {
          // We have failed to open a theme, so let's fall back to the system
          // basics.
          FillRect(hDC, &m_rcDeviceRect, GetSysColorBrush(COLOR_3DFACE));

          // Draw the edges of a pressed button.
          if ((IsState(BTN_PRESSED)) || (IsState(BTN_ACTIVE))) {
            DrawEdge(hDC, &m_rcDeviceRect, EDGE_SUNKEN, BF_TOPLEFT);
            DrawEdge(hDC, &m_rcDeviceRect, EDGE_RAISED, BF_BOTTOMRIGHT);
          } else {
            // Draw the edges for a regular button.
            DrawEdge(hDC, &m_rcDeviceRect, EDGE_RAISED, BF_TOPLEFT);
            DrawEdge(hDC, &m_rcDeviceRect, EDGE_RAISED, BF_BOTTOMRIGHT);
          }
          // TODO: Add a hatch brush at around 50-60% to mark active buttons. 
        }
        // Release the device context.
        m_pInteropTarget->ReleaseDC(NULL);
      }
    }

    // Draw the application icon.
    m_pRenderTarget->DrawBitmap(m_pBitmap, destRect, 1.0f, D2D1_BITMAP_INTERPOLATION_MODE_LINEAR);

    if (EndDraw() == D2DERR_RECREATE_TARGET)
      ReleaseExtraResources();
  } 
}

void Button::OnThemeChange()
{
  if (m_hTheme)
    CloseThemeData(m_hTheme);

  if (OSIsWin7Server08R2()) {
    if (IsCompositionActive()) {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBAND2COMPOSITED::TASKBAND2");
    }
    else {
      m_hTheme = OpenThemeData(m_hWnd, L"TASKBAND2");
    }
  } else if (OSIsServer08Vista()) {
    // There is no composited task band for Windows Vista - Windows Server 2008
    // as far as I know.
    m_hTheme = OpenThemeData(m_hWnd, L"TASKBAND::TOOLBAR");
  }
  if (!m_hTheme) {
    dprintf("Could not open theme for button. %08X\n", m_hWnd);
  }

  ReleaseDeviceResources();
  ReleaseExtraResources();
}

void Button::Flash()
{
  m_bFlashing = m_bFlashing ? FALSE : TRUE;
  OnPaint();
}

// EOF
