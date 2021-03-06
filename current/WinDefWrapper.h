/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: . . . . . WinDefWrapper.h                                                                                    *
 *     Status: . . . . version 0.2;                                                                                       *
 *     Dependency: . . none;                                                                                              *
 *     Distribution: . source/object code;                                                                                *
 *     OS family:  . . Windows [XP+], Linux [2.6.16+], BSD [4.4+];                                                        *
 *     Platform: . . . x86/x86-64, ARM;                                                                                   *
 *     License:  . . . LGPL [2.1+].                                                                                       *
 *                                                                                                                        *
 *     Short desription:                                                                                                  *
 *         basic definitions.                                                                                             *
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
 * =============================================== *** Header files *** ================================================= *
 **************************************************************************************************************************/

#if defined(_WIN32) || defined(_WIN64)                                                              // Detecting Windows

#include <WinDef.h>
#include <WinNT.h>
#include <BaseTsd.h>

#else

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

/**************************************************************************************************************************
 * ============================================= *** Global variables *** =============================================== *
 **************************************************************************************************************************/

#   ifndef     CONST
#   define     CONST           const                                                                // A variable whose value is to remain constant during execution.
#   endif   // CONST

#   ifndef     FALSE
#   define     FALSE           false
#   endif   // FALSE

#   ifndef     TRUE
#   define     TRUE            true
#   endif   // TRUE

    typedef    bool            BOOL,          *PBOOL,         *LPBOOL;                              // A Boolean variable (should be TRUE or FALSE).

    typedef    void            VOID,          *PVOID,         *LPVOID;                              // Any type (and a pointer to it).
    typedef    char            CHAR,          *PCHAR,         *LPCHAR;                              // An 8-bit Windows (ANSI) character.
    typedef    short           SHORT,         *PSHORT,        *LPSHORT;                             // A short integer.
    typedef    int             INT,           *PINT,          *LPINT;                               // A signed integer.
    typedef    long            LONG,          *PLONG,         *LPLONG;                              // A signed long integer.
    typedef    unsigned char   UCHAR,         *PUCHAR,        *LPUCHAR;                             // An unsigned CHAR.
    typedef    unsigned short  USHORT,        *PUSHORT,       *LPUSHORT;                            // An unsigned SHORT.
    typedef    unsigned int    UINT,          *PUINT,         *LPUINT;                              // An unsigned INT.
    typedef    unsigned long   ULONG,         *PULONG,        *LPULONG;                             // An unsigned LONG.

    typedef    int8_t          INT8,          *PINT8;                                               // An 8-bit signed integer (and a pointer to it).
    typedef    int16_t         INT16,         *PINT16;                                              // A 16-bit signed integer (and a pointer to it).
    typedef    int32_t         INT32,         *PINT32;                                              // A 32-bit signed integer (and a pointer to it).
    typedef    int64_t         INT64,         *PINT64;                                              // A 64-bit signed integer (and a pointer to it).
    typedef    intptr_t        INT_PTR,       *PINT_PTR;                                            // A signed long type for pointer precision.
    typedef    uint8_t         UINT8,         *PUINT8;                                              // An unsigned INT8 (and a pointer to it).
    typedef    uint16_t        UINT16,        *PUINT16;                                             // An unsigned INT16 (and a pointer to it).
    typedef    uint32_t        UINT32,        *PUINT32;                                             // An unsigned INT32 (and a pointer to it).
    typedef    uint64_t        UINT64,        *PUINT64;                                             // An unsigned INT64 (and a pointer to it).
    typedef    uintptr_t       UINT_PTR,      *PUINT_PTR;                                           // An unsigned INT_PTR (and a pointer to it).

    typedef    int32_t         LONG32,        *PLONG32;                                             // A 32-bit signed integer (and a pointer to it).
    typedef    int64_t         LONG64,        *PLONG64;                                             // A 64-bit signed integer (and a pointer to it).
    typedef    intptr_t        LONG_PTR,      *PLONG_PTR;                                           // A signed long type for pointer precision.
    typedef    uint32_t        ULONG32,       *PULONG32;                                            // An unsigned LONG32 (and a pointer to it).
    typedef    uint64_t        ULONG64,       *PULONG64;                                            // An unsigned LONG64 (and a pointer to it).
    typedef    uintptr_t       ULONG_PTR,     *PULONG_PTR;                                          // An unsigned LONG_PTR (and a pointer to it).

    typedef    uint8_t         BYTE,          *PBYTE,         *LPBYTE;                              // A byte (and a pointer to it).
    typedef    uint16_t        WORD,          *PWORD,         *LPWORD;                              // A 16-bit unsigned integer (and a pointer to it).
    typedef    uint32_t        DWORD,         *PDWORD,        *LPDWORD;                             // A 32-bit unsigned integer (and a pointer to it).
    typedef    uint64_t        QWORD,         *PQWORD,        *LPQWORD;                             // A 64-bit unsigned integer (and a pointer to it).

    typedef    uint32_t        DWORD32,       *PDWORD32;                                            // A 32-bit unsigned integer.
    typedef    uint64_t        DWORD64,       *PDWORD64;                                            // A 64-bit unsigned integer.
    typedef    uintptr_t       DWORD_PTR,     *PDWORD_PTR;                                          // An unsigned long type for pointer precision.


#   ifdef      UNICODE                                                                              // Detecting UNICODE.
    typedef    WCHAR           TCHAR;
#   else                                                                                            // A WCHAR if UNICODE is defined, a CHAR otherwise.
    typedef     CHAR           TCHAR;
#   endif   // UNICODE

    typedef                     CHAR          *PSTR,          *LPSTR;
    typedef              CONST  CHAR          *PCSTR,         *LPCSTR;
    typedef    wchar_t         WCHAR,         *PWSTR,         *LPWSTR;
    typedef              CONST WCHAR          *PCWSTR,        *LPCWSTR;
    typedef                    TCHAR          *PTSTR,         *LPTSTR;
    typedef              CONST TCHAR          *PCTSTR,        *LPCTSTR;

#endif                                                                                              // Detecting Windows

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // WINDEF_WRAPPER_H

