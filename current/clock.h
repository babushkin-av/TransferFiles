/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: clock.h (version 0.8).                                                                                       *
 *     Type: basic definitions (clock.h).                                                                                 *
 *     Distribution: source/object code.                                                                                  *
       License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: base.h.                                                                                                *
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
#ifndef CLOCK_H
#define CLOCK_H

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#include "base.h"

#include <time.h>

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

struct APP_CLOCK {
    time_t                   Start;
    time_t                   Now;
    struct tm                Local;
    char                     String[APP_NAME_MAX];
};

/*

struct CLOCK_INFO {
    FILETIME                 Start;
    FILETIME                 Now;
    SYSTEMTIME               Local;
    TCHAR                    String[APP_NAME_MAX];
};

*/

/**************************************************************************************************************************
 * ============================================ *** Function Prototypes *** ============================================= *
 **************************************************************************************************************************/

#ifdef __cplusplus
extern "С" {
#endif
/* ---------------------------------------------------------------------------------------------------------------------- */

int GetLocalTime(struct APP_CLOCK *Time, const char *strFormat);

/* ---------------------------------------------------------------------------------------------------------------------- */

int GetTimeStart(struct APP_CLOCK *Time, const char *strFormat);

/* ---------------------------------------------------------------------------------------------------------------------- */

int GetTimeDiff(struct APP_CLOCK *Time, const char *strFormat);

/* ---------------------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
};
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // CLOCK_H

