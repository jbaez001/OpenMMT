/* Genscripts Library - Error.hpp
 * Copyright (C) 2009  Genscripts
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
#ifndef __MLIB_ERROR_HPP
#   define __MLIB_ERROR_HPP

#pragma message("Genscripts Library: Compiling Error.hpp")

#include <strsafe.h>

namespace Gen {
    /**
     * Calls Calls GetLastError() and formats a message to return to the user.
     *  User is responsible for freeing up the memory returned by this function via LocalFree();
     *
     *  @param dwCode       Contains error code passed by GetLastError().  So pass it only if you called GetLastError().
     */
    inline LPSTR GetLastErrorMessage(DWORD dwCode = NULL) 
    {
        DWORD dwErrorCode       = dwCode ? dwCode : GetLastError();
        LPVOID lpMessageBuffer  = NULL;

        FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, dwErrorCode, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMessageBuffer, 0, NULL);

        return (LPSTR)lpMessageBuffer;
    }
}

#endif /* __MLIB_ERROR_HPP */

