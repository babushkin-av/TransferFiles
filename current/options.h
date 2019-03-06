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

    unsigned int GetOptionID(const char *argv);

/* ---------------------------------------------------------------------------------------------------------------------- */

    const char* GetOptionHelp(const unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    char* GetOptionVar(const unsigned int ID);

/* ---------------------------------------------------------------------------------------------------------------------- */

    size_t SetOptionVar(const unsigned int ID, char *Var);

/* ---------------------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
};
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // OPTIONS_H

