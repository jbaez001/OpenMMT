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
#ifndef OPENMMT_THUMBNAIL_THUMBNAIL_H
#define OPENMMT_THUMBNAIL_THUMBNAIL_H
#pragma once

class Thumbnail
{
private:
  HTHUMBNAIL m_hThumbnail;  /**< Handle to the thumbnail. */
  HWND m_hWndSource;        /**< Handle to the source. */
  HWND m_hWnd;              /**< Handle to the thumbnail window. */
  BOOL m_bRegistered;       /**< Is there a thumbnail registered? */

public:
  Thumbnail();
  ~Thumbnail();

  CONST HWND GetHandle();
  CONST HWND GetSource();

  BOOL IsThumbnailRegistered();
  void CreateThumbnailFromSource(HWND hWnd, int x, int y);
  void DestroyThumbnailWindow();

private:
  void CreateThumbnailWindow(int x, int y);
  void RegisterThumbnail();
  void UnregisterThumbnail();
};

#endif // OPENMMT_THUMBNAIL_THUMBNAIL_H

// EOF
