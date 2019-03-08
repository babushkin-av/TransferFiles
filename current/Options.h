/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: . . . . . Options.h                                                                                          *
 *     Status: . . . . version 0.9;                                                                                       *
 *     Dependency: . . none;                                                                                              *
 *     Distribution: . source/object code;                                                                                *
 *     OS family:  . . Windows [XP+], Linux [2.6.16+], BSD [4.4+];                                                        *
 *     Platform: . . . x86/x86-64, ARM;                                                                                   *
 *     License:  . . . LGPL [2.1+].                                                                                       *
 *                                                                                                                        *
 *     Short desription:                                                                                                  *
 *          .                                                                                                             *
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


enum OPTION_IDs {
    OPTION_NULL,                                                                                    //
    OPTION_HELP,                                                                                    //  Show help-screen and exits.
    OPTION_VERSION,                                                                                 //  Show version and exits.
    OPTION_QUIET,                                                                                   //  Suppress messages.
    OPTION_DEBUG,                                                                                   //  Show debug messages.
    OPTION_FORCE,                                                                                   //  Try to recover from errors.
    OPTION_RECURSIVE,                                                                               //  Dive into directories recursively.
    OPTION_NOFOLLOW,                                                                                //  Do not follow symbolic links.
    OPTION_OVERWRITE,                                                                               //  Overwrite existing files.
    OPTION_IPV4,                                                                                    //  Use IPv4.
    OPTION_IPV6,                                                                                    //  Use IPv6.
    OPTION_SERVER,                                                                                  //  Work as a server.
    OPTION_CLIENT,                                                                                  //  Work as a client.
    OPTION_PORT,                                                                                    //  Set up a port number.
    OPTION_LAST                                                                                     //
};

enum OPTION_FLAGs {
    OPTION_FLAG_NULL          = ( OPTION_NULL ),                                                    //    (0);
    OPTION_FLAG_HELP          = ( 1 << ( OPTION_HELP      - 1 ) ),                                  //    (1);
    OPTION_FLAG_VERSION       = ( 1 << ( OPTION_VERSION   - 1 ) ),                                  //    (2);
    OPTION_FLAG_QUIET         = ( 1 << ( OPTION_QUIET     - 1 ) ),                                  //    (4);
    OPTION_FLAG_DEBUG         = ( 1 << ( OPTION_DEBUG     - 1 ) ),                                  //    (8);
    OPTION_FLAG_FORCE         = ( 1 << ( OPTION_FORCE     - 1 ) ),                                  //   (16);
    OPTION_FLAG_RECURSIVE     = ( 1 << ( OPTION_RECURSIVE - 1 ) ),                                  //   (32);
    OPTION_FLAG_NOFOLLOW      = ( 1 << ( OPTION_NOFOLLOW  - 1 ) ),                                  //   (64);
    OPTION_FLAG_OVERWRITE     = ( 1 << ( OPTION_OVERWRITE - 1 ) ),                                  //  (128);
    OPTION_FLAG_IPV4          = ( 1 << ( OPTION_IPV4      - 1 ) ),                                  //  (256);
    OPTION_FLAG_IPV6          = ( 1 << ( OPTION_IPV6      - 1 ) ),                                  //  (512);
    OPTION_FLAG_SERVER        = ( 1 << ( OPTION_SERVER    - 1 ) ),                                  // (1024);
    OPTION_FLAG_CLIENT        = ( 1 << ( OPTION_CLIENT    - 1 ) ),                                  // (2048);
    OPTION_FLAG_PORT          = ( 1 << ( OPTION_PORT      - 1 ) ),                                  // (4096);
    OPTION_FLAG_LAST          = ( OPTION_NULL )                                                     //    (0);

/**************************************************************************************************************************
 * ============================================= *** Function Prototypes *** ============================================ *
 **************************************************************************************************************************/

#ifdef __cplusplus
extern "С" {
#endif
/* ---------------------------------------------------------------------------------------------------------------------- */

    unsigned int GetOptionID(const char *argv);

/* ---------------------------------------------------------------------------------------------------------------------- */

    unsigned int GetOptionFlag(const unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    const char* GetOptionHelp(const unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    char* GetOptionVar(const unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    size_t SetOptionVar(const unsigned int ID, char *Var);

/* ---------------------------------------------------------------------------------------------------------------------- */

    size_t ShowHelp(void);

/* ---------------------------------------------------------------------------------------------------------------------- */

    int ShowVersion(void);

/* ---------------------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
};
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // OPTIONS_H

