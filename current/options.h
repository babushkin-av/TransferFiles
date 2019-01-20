/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: options.h (version 0.9).                                                                                     *
 *     Dependency: base.h.                                                                                                *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Desription: .                                                                                                      *
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
#ifndef OPTIONS_H
#define OPTIONS_H

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#include "base.h"

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#if defined (APP_DEBUG)
#    define  OPTIONS_DEBUG_STRING      "debug"
#else
#    define  OPTIONS_DEBUG_STRING      ""
#endif

#if defined (_WIN32) || defined (_WIN64)
#    define  OPTIONS_OS_STRING         "windows"
#elif defined (__unix__)
#    define  OPTIONS_OS_STRING         "linux"
#else
#    define  OPTIONS_OS_STRING         ""
#endif


enum OPTIONS_ID {
    OPTION_NULL          = ( 0 ),                              //    (0);
    OPTION_HELP          = ( 1 ),                              //    (1);  Show help-screen and exits.
    OPTION_VERSION       = ( 1 <<  1),                         //    (2);  Show version and exits.
    OPTION_QUIET         = ( 1 <<  2),                         //    (4);  Suppress messages.
    OPTION_DEBUG         = ( 1 <<  3),                         //    (8);  Show debug messages.
    OPTION_FORCE         = ( 1 <<  4),                         //   (16);  Try to recover from errors.
    OPTION_RECURSIVE     = ( 1 <<  5),                         //   (32);  Dive into directories recursively.
    OPTION_NOFOLLOW      = ( 1 <<  6),                         //   (64);  Do not follow symbolic links.
    OPTION_OVERWRITE     = ( 1 <<  7),                         //  (128);  Overwrite existing files.
    OPTION_IPV4          = ( 1 <<  8),                         //  (256);  Use IPv4.
    OPTION_IPV6          = ( 1 <<  9),                         //  (512);  Use IPv6.
    OPTION_SERVER        = ( 1 << 10),                         // (1024);  Work as a server.
    OPTION_CLIENT        = ( 1 << 11),                         // (2048);  Work as a client.
    OPTION_PORT          = ( 1 << 12),                         // (4096);  Set up a port number.
    OPTION_LAST          = ( 1 << 13)                          // (8192);
};

struct OPTIONS_DATA {
    unsigned int       ID;
    const char        *Option;
    const char        *OptionExt;
    const char        *Help;
    char              *Var;
};

/**************************************************************************************************************************
 * ============================================= *** Function Prototypes *** ============================================ *
 **************************************************************************************************************************/

#ifdef __cplusplus
extern "С" {
#endif
/* ---------------------------------------------------------------------------------------------------------------------- */

    int ShowHelp(void);

/* ---------------------------------------------------------------------------------------------------------------------- */

    int ShowVersion(void);

/* ---------------------------------------------------------------------------------------------------------------------- */

    unsigned int GetOptionIndex(unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    unsigned int GetOptionID(char *argv);

/* ---------------------------------------------------------------------------------------------------------------------- */

    const char* GetOptionHelp(unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    char* GetOptionVar(unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    size_t SetOptionVar(unsigned int ID, char *Var);

/* ---------------------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
};
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // OPTIONS_H

