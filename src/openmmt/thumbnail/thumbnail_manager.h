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
#ifndef OPENMMT_THUMBNAIL_THUMBNAIL_MANAGER_H
#define OPENMMT_THUMBNAIL_THUMBNAIL_MANAGER_H
#pragma once

#include "openmmt/taskbar/buttons/button.h"
#include "openmmt/thumbnail/thumbnail.h"

class Button;
class Thumbnail;

typedef std::tr1::shared_ptr<Thumbnail> ThumbnailPtr;

/**
 * ThumbnailManager class.
 */
class ThumbnailManager
{
public:
  ThumbnailManager();
  ~ThumbnailManager();

  /**
   * Checks to see if a thumbnail is present.
   */
  BOOL isThumbnailPresent();

  /**
   * Checks to see if the specified button is the one that is
   * currently thumbnailed.
   */
  BOOL isThumbnailed(ButtonPtr pBtn);
  BOOL isThumbnailed(HWND hWndButton);

  /**
   * Creates a thumbnail for the specified button.
   */
  void CreateThumbnail(ButtonPtr pBtn);

  /**
   * Destroys the thumbnail for the button.
   */
  void DestroyThumbnail();

  /**
   * Returns the handle to the thumbnail window.
   */
  const HWND GetThumbnailWindow();

  /**
   * Returns the handle to the thumbnailed button.
   */
  const HWND GetThumbnailedButton();

private:
  INT           m_iThumbnailPosX;
  INT           m_iThumbnailPosY;
  BOOL          m_bThumbnailPresent;
  HWND          m_hWndButtonSource;
  ThumbnailPtr  m_pThumbnail;

  /**
   * Calculates the position of that the thumbnail window should be placed
   * at.
   */
  void CalcThumbnailWindowPos(ButtonPtr pBtn, LPPOINT lpPoints);
};

#endif // OPENMMT_THUMBNAIL_THUMBNAIL_MANAGER_H

// EOF
