/*************************************************************************
 * OpenMMT - Open Multi-Monitor Taskbar
 * Copyright (C) 2010-2012 Genscripts
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
#ifndef OPENMMT_POINTER_TYPES_H
#define OPENMMT_POINTER_TYPES_H

/** Forward declarations */
class Application;
class Button;
class Monitor;
class Taskbar;

#include <memory>

// OpenMMT
typedef std::shared_ptr<Application> ApplicationPtr;
typedef std::shared_ptr<Monitor> MonitorPtr;
typedef std::shared_ptr<Taskbar> TaskbarPtr;
typedef std::shared_ptr<Button> ButtonPtr;

#endif // OPENMMT_POINTER_TYPES_H

