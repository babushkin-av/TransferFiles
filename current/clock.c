/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: clock.c (version 0.9).                                                                                       *
 *     Type: module.                                                                                                      *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: base.h.                                                                                                *
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

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#include "clock.h"

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#define TIME_FORMAT_DEFAULT "%T"
#define DATE_FORMAT_DEFAULT "%d.%m.%Y"


/**************************************************************************************************************************
 * =========================================== *** GetLocalTime() Function *** ========================================== *
 **************************************************************************************************************************/

int GetLocalTime(struct APP_CLOCK *Time, const char *strFormat){

    int        result = 0;
    struct tm *pTmp   = NULL;

    if( Time )
        if( time(&(Time->Now)) )
        {   struct tm *pTmp = localtime(&(Time->Now));
            if( pTmp )
            {   memcpy(&(Time->Local),pTmp,sizeof(struct tm));
                if( !strFormat )  strFormat = TIME_FORMAT_DEFAULT;
                result = strftime(&(Time->String[0]),APP_NAME_MAX,strFormat,pTmp);
        };  };
return(result); }

/**************************************************************************************************************************
 * =========================================== *** GetTimeStart() Function *** ========================================== *
 **************************************************************************************************************************/

int GetTimeStart(struct APP_CLOCK *Time, const char *strFormat){

    int result = GetLocalTime(Time,strFormat);
    if( result )  Time->Start = Time->Now;

return(result); }

/**************************************************************************************************************************
 * =========================================== *** GetTimeDiff() Function *** =========================================== *
 **************************************************************************************************************************/

int GetTimeDiff(struct APP_CLOCK *Time, const char *strFormat){

    int result = 0;

    if( Time )
        if( time(&(Time->Now)) )
        {
            Time->Now = (Time->Now) - (Time->Start);

            struct tm *pTmp = gmtime(&(Time->Now));
            if( pTmp )
            {   memcpy(&(Time->Local),pTmp,sizeof(struct tm));
                if( !strFormat )  strFormat = TIME_FORMAT_DEFAULT;
                result = strftime(&(Time->String[0]),APP_NAME_MAX,strFormat,pTmp);
        };  };
return(result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

