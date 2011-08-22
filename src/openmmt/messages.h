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
#ifndef OPENMMT_MESSAGES_H
#define OPENMMT_MESSAGES_H
#pragma once

/**
 * Custom app messages for OpenMMT. The majority of these will be sent 
 * from the systemhooks dll to the main OpenMMT controller window.
 */
enum OpenMMTMessages {
  TASKBAR_WINDOW_ACTIVATE = WM_APP, /**< A window is about to be activated.      */
  TASKBAR_WINDOW_CREATE,            /**< A window is about to be created.        */
  TASKBAR_WINDOW_DESTROY,           /**< A window is about to be destroyed.      */
  TASKBAR_WINDOW_MOVESIZE,          /**< A window is being moved or resized.     */
  TASKBAR_WINDOW_POSCHANGED,        /**< A window's position has just changed.   */
  TASKBAR_WINDOW_SETFOCUS,          /**< A window is gaining keyboard focus.     */
  TASKBAR_WINDOW_MINMAX,            /**< A window's pos/size is about to change. */
  TASKBAR_WINDOW_MINIMIZE,          /**< A window is being minimized.            */
  TASKBAR_WINDOW_MAXIMIZE,          /**< A window is being maximized.            */
  TASKBAR_X86_SUPPORT_HELLO,        /**< Our x86 helper said hello.              */
  TASKBAR_X86_SUPPORT_BYEBYE,       /**< Our x86 helper is leaving us. :'(       */
  OPENMMT_TRAY_MESSAGE              /**< System tray icon message.               */
};

#endif // OPENMMT_MESSAGES_H