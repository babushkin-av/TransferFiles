/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: clock.c (version 0.8).                                                                                       *
 *     Type: module.                                                                                                      *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: base.h.                                                                                                *
 *     Desription: .                                                                                                      *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     Copyleft, 2017-2018, <feedback@babushkin.ru.net>, Alexander Babushkin.                                             *
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
 * ============================================== *** ShowHelp Function *** ============================================= *
 **************************************************************************************************************************/

int GetLocalTime(struct APP_CLOCK *Time, char *strFormat){

    int        result = 0;
    struct tm *pTmp   = NULL;

    if(Time)
        if( time(&(Time->Now)) )
            if( pTmp = localtime(&(Time->Now)) )
            {
                memcpy(&(Time->Local),pTmp,sizeof(struct tm));
                if( !strFormat )  strFormat = TIME_FORMAT_DEFAULT;
                result = strftime(&(Time->String[0]),APP_NAME_MAX,strFormat,pTmp);
            };
return(result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

