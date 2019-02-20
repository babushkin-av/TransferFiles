/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: WinDefWrapper.h (version 0.1).                                                                               *
 *     Dependency: none.                                                                                                  *
 *     Distribution: source/object code.                                                                                  *
 *     Operating System: Windows [XP+], Linux [2.6.16+], BSD [4.4+].                                                      *
 *     Platform: x86/x86-64, ARM.                                                                                         *
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
    typedef  BOOL          *PBOOL;                                                                  // A pointer to a BOOL.
    typedef  PBOOL          LPBOOL;                                                                 // A pointer to a BOOL.

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

    typedef  INT8          *PINT8;                                                                  // A pointer to an INT8.
    typedef  INT16         *PINT16;                                                                 // A pointer to an INT16.
    typedef  INT32         *PINT32;                                                                 // A pointer to an INT32.
    typedef  INT64         *PINT64;                                                                 // A pointer to an INT64.
    typedef  INT_PTR       *PINT_PTR;                                                               // A pointer to an INT_PTR.
    typedef  UINT8         *PUINT8;                                                                 // A pointer to an UINT8.
    typedef  UINT16        *PUINT16;                                                                // A pointer to an UINT16.
    typedef  UINT32        *PUINT32;                                                                // A pointer to an UINT32.
    typedef  UINT64        *PUINT64;                                                                // A pointer to an UINT64.
    typedef  UINT_PTR      *PUINT_PTR;                                                              // A pointer to an UINT_PTR.

    typedef  int32_t        LONG32;                                                                 // A 32-bit signed integer.
    typedef  int64_t        LONG64;                                                                 // A 64-bit signed integer.
    typedef  intptr_t       LONG_PTR;                                                               // A signed long type for pointer precision.
    typedef  uint32_t       ULONG32;                                                                // An unsigned LONG32.
    typedef  uint64_t       ULONG64;                                                                // An unsigned LONG64.
    typedef  uintptr_t      ULONG_PTR;                                                              // An unsigned LONG_PTR.

    typedef  LONG32        *PLONG32;                                                                // A pointer to a LONG32.
    typedef  LONG64        *PLONG64;                                                                // A pointer to a LONG64.
    typedef  LONG_PTR      *PLONG_PTR;                                                              // A pointer to a LONG_PTR.
    typedef  ULONG32       *PULONG32;                                                               // A pointer to a ULONG32.
    typedef  ULONG64       *PULONG64;                                                               // A pointer to a ULONG64.
    typedef  ULONG_PTR     *PULONG_PTR;                                                             // A pointer to a ULONG_PTR.

    typedef  uint8_t        BYTE;                                                                   // A byte (8 bits).
    typedef  uint16_t       WORD;                                                                   // A 16-bit unsigned integer.
    typedef  uint32_t       DWORD;                                                                  // A 32-bit unsigned integer.
    typedef  uint64_t       QWORD;                                                                  // A 64-bit unsigned integer.

    typedef  BYTE          *PBYTE;                                                                  // A pointer to a BYTE.
    typedef  WORD          *PWORD;                                                                  // A pointer to a WORD.
    typedef  DWORD         *PDWORD;                                                                 // A pointer to a DWORD.
    typedef  QWORD         *PQWORD;                                                                 // A pointer to a QWORD.

    typedef  PBYTE          LPBYTE;                                                                 // A pointer to a BYTE.
    typedef  PWORD          LPWORD;                                                                 // A pointer to a WORD.
    typedef  PDWORD         LPDWORD;                                                                // A pointer to a DWORD.
    typedef  PQWORD         LPQWORD;                                                                // A pointer to a QWORD.

    typedef  uint32_t       DWORD32;                                                                // A 32-bit unsigned integer.
    typedef  uint64_t       DWORD64;                                                                // A 64-bit unsigned integer.
    typedef  uintptr_t      DWORD_PTR;                                                              // An unsigned long type for pointer precision.

    typedef  DWORD32       *PDWORD32;                                                               // A pointer to a DWORD32.
    typedef  DWORD64       *PDWORD64;                                                               // A pointer to a DWORD64.
    typedef  DWORD_PTR     *PDWORD_PTR;                                                             // A pointer to a DWORD_PTR.

#endif                                                                                              // Detecting Windows

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // WINDEF_WRAPPER_H

