/* Genscripts Library - Console.hpp
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
#ifndef __MLIB_CONSOLE_HPP__
#   define __MLIB_CONSOLE_HPP__

#pragma message("Genscripts Library: Compiling Console.hpp")

#include <io.h>
#include <fcntl.h>

namespace Gen {
    /**
     * CGenConsole. Class used to open a Console Window.
     */
    class CGenConsole
    {
    private:
        FILE *m_fdOut;          /* Output Stream */
        FILE *m_fdIn;           /* Input Stream */
        BOOL bCloseOnExit;      /* Should we close the console on exit? */
        BOOL bConsole;          /* Is the console open? */

        /**
         * Initialize. Initializes this class instance.
         */
        void Initialize()
        {
            m_fdIn = m_fdOut = NULL;
            bConsole = false;
        }

    public:

        /**
         * Constructor #1. Console does not close on exit.
         */
        CGenConsole() : bCloseOnExit(false)
        {
            Initialize();
        }

        /**
         * Constructor #2. User tells us if we should close the console on exit.
         *
         * @param bCloseConsole     Set to true if the console should be closed on class deconstruction.
         */
        CGenConsole(BOOL bCloseConsole)  : bCloseOnExit(bCloseConsole)
        {
            Initialize();
        }

        /**
         * Deconstructor. Class deconstructor.
         */
        ~CGenConsole()
        {
            if (bConsole && bCloseOnExit) 
                FreeConsole();
        }

        /**
         * OpenConsole.     Used to open a console window.
         *
         * @return True     If the console was open.
         * @return False    If the console could not be open.
         */
        bool OpenConsole()
        {
          bConsole = AllocConsole();
            if (!bConsole)
                return false;

            HANDLE m_StdHandle = GetStdHandle(STD_OUTPUT_HANDLE);
            int m_StreamHandle =_open_osfhandle((intptr_t)m_StdHandle, _O_TEXT);
            m_fdOut =_fdopen(m_StreamHandle, "w");

            m_StdHandle = GetStdHandle(STD_INPUT_HANDLE);
            m_StreamHandle =_open_osfhandle((intptr_t)m_StdHandle, _O_TEXT);
            m_fdIn =_fdopen(m_StreamHandle, "r");
            setvbuf(m_fdIn, NULL, _IONBF, 1);
            setvbuf(m_fdOut, NULL, _IONBF, 1);

            *stdin = *m_fdIn;
            *stdout = *m_fdOut; 

            printf("Genscripts Library Console.\n\n");

            return true;
        }

        /**
         * CloseConsole.    Used to close a console window.
         *
         * @return True     If the console was closed.
         * @return False    If we could not close the console.
         */
        bool CloseConsole()
        {
          bConsole = FreeConsole();
            if (bConsole)
                return true;

            return false;
        }
    };
}


#endif /* __MLIB_CONSOLE_HPP__ */

/* EOF */

