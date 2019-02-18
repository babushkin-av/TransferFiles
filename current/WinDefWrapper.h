/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: WinDefWrapper.h (version 0.1).                                                                               *
 *     Dependency: none.                                                                                                  *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Desription: basic definitions.                                                                                     *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     Copyleft, 2017-2019, <feedback@babushkin.ru.net>, Alexander Babushkin.                                             *
 *                                                                                                                        *
 *     This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser Public   *
 * License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later *
 * version.                                                                                                               *
 *     This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied *
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser Public License for more details.  *
 *                                                                                                                        *
 **************************************************************************************************************************/
#ifndef WINDEF_WRAPPER_H
#define WINDEF_WRAPPER_H

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#if defined(_WIN32) || defined(_WIN64) // Detecting Windows

#include <WinDef.h>
#include <WinNT.h>
#include <BaseTsd.h>

#else

#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#endif // Detecting Windows

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#if defined(_WIN32) || defined(_WIN64)                                                              // Detecting Windows

#else

#   define   CONST      const                                                                       // A variable whose value is to remain constant during execution.

    typedef  int        BOOL;                                                                       // A Boolean variable (should be TRUE or FALSE).
    typedef  uint8_t    BYTE;                                                                       // A byte (8 bits).
    typedef  uint16_t   WORD;                                                                       //
    typedef  uint32_t   DWORD;                                                                      // A 32-bit unsigned integer.
    typedef  uintptr_t  DWORD_PTR;                                                                  // An unsigned long type for pointer precision.

    typedef  char       CHAR;                                                                       // An 8-bit Windows (ANSI) character.


#endif                                                                                              // Detecting Windows

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // WINDEF_WRAPPER_H

