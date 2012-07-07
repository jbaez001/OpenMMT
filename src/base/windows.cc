#include "base/windows.h"


/**
 * WndShouldDisplay. Check to see if a Window should be displayed on the
 * taskbar.
 *
 * @param hWnd    Window in question.
 */
BOOL WndShouldDisplay(HWND hWnd)
{
  if (!hWnd || !IsWindowVisible(hWnd) || GetParent(hWnd))
    return FALSE;

  LONG_PTR dwFlags = GetWindowLongPtr(hWnd, GWL_STYLE);
  
  if (dwFlags & WS_DISABLED)
    return FALSE;

  LONG_PTR dwExFlags = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
  HWND hWndOwner = GetWindow(hWnd, GW_OWNER);

  if ((!(dwExFlags & WS_EX_TOOLWINDOW) && !hWndOwner) || 
    ((dwExFlags & WS_EX_APPWINDOW) && hWndOwner)) {
    return TRUE;
  }

  return FALSE;
}