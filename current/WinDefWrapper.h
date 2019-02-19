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
#include <stdint.h>

#endif // Detecting Windows

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#if defined(_WIN32) || defined(_WIN64)                                                              // Detecting Windows

#else

#   define   VOID           void                                                                    // Any type.
#   define   CONST          const                                                                   // A variable whose value is to remain constant during execution.

    typedef  int            BOOL;                                                                   // A Boolean variable (should be TRUE or FALSE).

    typedef  char           CHAR;                                                                   // An 8-bit Windows (ANSI) character.
    typedef  short          SHORT;                                                                  // A short integer.
    typedef  int            INT;                                                                    // A signed integer.
    typedef  long           LONG;                                                                   // A signed long integer.
    typedef  unsigned char  UCHAR                                                                   // An unsigned CHAR.
    typedef  unsigned short USHORT;                                                                 // An unsigned SHORT.
    typedef  unsigned int   UINT;                                                                   // An unsigned INT.
    typedef  unsigned long  ULONG;                                                                  // An unsigned LONG.

    typedef  int8_t         INT8                                                                    // An 8-bit signed integer.
    typedef  int16_t        INT16                                                                   // A 16-bit signed integer.
    typedef  int32_t        INT32                                                                   // A 32-bit signed integer.
    typedef  int64_t        INT64                                                                   // A 64-bit signed integer.
    typedef  intptr_t       INT_PTR;                                                                // A signed long type for pointer precision.
    typedef  uint8_t        UINT8                                                                   // An unsigned INT8.
    typedef  uint16_t       UINT16                                                                  // An unsigned INT16.
    typedef  uint32_t       UINT32                                                                  // An unsigned INT32.
    typedef  uint64_t       UINT64                                                                  // An unsigned INT64.
    typedef  uintptr_t      UINT_PTR;                                                               // An unsigned INT_PTR.

    typedef  int32_t        LONG32;                                                                 // A 32-bit signed integer.
    typedef  int64_t        LONG64;                                                                 // A 64-bit signed integer.
    typedef  intptr_t       LONG_PTR;                                                               // A signed long type for pointer precision.
    typedef  uint32_t       ULONG32;                                                                // An unsigned LONG32.
    typedef  uint64_t       ULONG64;                                                                // An unsigned LONG64.
    typedef  uintptr_t      ULONG_PTR;                                                              // An unsigned LONG_PTR.

    typedef  LONG32        *LPLONG32;                                                               // A pointer to a LONG32.
    typedef  LONG64        *LPLONG64;                                                               // A pointer to a LONG64.
    typedef  LONG_PTR      *LPLONG_PTR;                                                             // A pointer to a LONG_PTR.
    typedef  ULONG32       *LPULONG32;                                                              // A pointer to a ULONG32.
    typedef  ULONG64       *LPULONG64;                                                              // A pointer to a ULONG64.
    typedef  ULONG_PTR     *LPULONG_PTR;                                                            // A pointer to a ULONG_PTR.

    typedef  uint8_t        BYTE;                                                                   // A byte (8 bits).
    typedef  uint16_t       WORD;                                                                   // A 16-bit unsigned integer.
    typedef  uint32_t       DWORD;                                                                  // A 32-bit unsigned integer.
    typedef  uint64_t       QWORD;                                                                  // A 64-bit unsigned integer.

    typedef  BYTE          *LPBYTE;                                                                 // A pointer to a BYTE.
    typedef  WORD          *LPWORD;                                                                 // A pointer to a WORD.
    typedef  DWORD         *LPDWORD;                                                                // A pointer to a DWORD.
    typedef  QWORD         *LPQWORD;                                                                // A pointer to a QWORD.

    typedef  uint32_t       DWORD32;                                                                // A 32-bit unsigned integer.
    typedef  uint64_t       DWORD64;                                                                // A 64-bit unsigned integer.
    typedef  uintptr_t      DWORD_PTR;                                                              // An unsigned long type for pointer precision.

    typedef  DWORD32       *LPDWORD32;                                                              // A pointer to a DWORD32.
    typedef  DWORD64       *LPDWORD64;                                                              // A pointer to a DWORD64.
    typedef  DWORD_PTR     *LPDWORD_PTR;                                                            // A pointer to a DWORD_PTR.

#endif                                                                                              // Detecting Windows

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // WINDEF_WRAPPER_H

