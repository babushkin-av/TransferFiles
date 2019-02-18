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

    typedef  char           CHAR;                                                                   // An 8-bit Windows (ANSI) character.
    typedef  int            INT;                                                                    // A signed integer.
    typedef  long           LONG;                                                                   // A signed long integer.
    typedef  unsigned int   UINT;
    typedef  unsigned long  ULONG;

    typedef  int8_t         INT8                                                                    // An 8-bit signed integer.
    typedef  int16_t        INT16                                                                   // A 16-bit signed integer.
    typedef  int32_t        INT32                                                                   // A 32-bit signed integer.
    typedef  int64_t        INT64                                                                   // A 64-bit signed integer.
    typedef  uint8_t        UINT8                                                                   // An 8-bit unsigned integer.
    typedef  uint16_t       UINT16                                                                  // A 16-bit unsigned integer.
    typedef  uint32_t       UINT32                                                                  // A 32-bit unsigned integer.
    typedef  uint64_t       UINT64                                                                  // A 64-bit unsigned integer.

    typedef  uint8_t        BYTE;                                                                   // A byte (8 bits).
    typedef  uint16_t       WORD;                                                                   //
    typedef  uint32_t       DWORD;                                                                  // A 32-bit unsigned integer.
    typedef  uint32_t       DWORD32;                                                                // A 32-bit unsigned integer.
    typedef  uint64_t       QWORD;                                                                  // A 64-bit unsigned integer.
    typedef  uint64_t       DWORD64;                                                                // A 64-bit unsigned integer.
    typedef  uintptr_t      DWORD_PTR;                                                              // An unsigned long type for pointer precision.

#endif                                                                                              // Detecting Windows

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // WINDEF_WRAPPER_H

