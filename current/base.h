/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: base.h (version 0.9).                                                                                        *
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
#ifndef TRANSFERFILES_BASE_H
#define TRANSFERFILES_BASE_H

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#define _DEFAULT_SOURCE   1
#define _XOPEN_SOURCE     700

#include <limits.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#define      APP_NAME_MAX          _POSIX_PATH_MAX

#ifndef      APP_NAME
#    define  APP_NAME              "TransferFiles"
#endif

#ifndef      APP_VERSION
#    define  APP_VERSION           "0.1"
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // TRANSFERFILES_BASE_H

