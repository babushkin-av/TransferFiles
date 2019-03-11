/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: . . . . . Options.c                                                                                          *
 *     Status: . . . . version 0.9;                                                                                       *
 *     Dependency: . . none;                                                                                              *
 *     Distribution: . source/object code;                                                                                *
 *     OS family:  . . Windows [XP+], Linux [2.6.26+], BSD [4.4+];                                                        *
 *     Platform: . . . x86/x86-64, ARM;                                                                                   *
 *     License:  . . . LGPL [2.1+].                                                                                       *
 *                                                                                                                        *
 *     Short desription:                                                                                                  *
 *          .                                                                                                             *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     This is options.c file.                                                                                            *
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

#include "Options.h"

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

struct OPTIONS_DATA  ODataDefault[] = {
     { OPTION_HELP,      OPTION_FLAG_HELP,       "-h",           NULL,       "Show help-screen and exits.",         NULL },
     { OPTION_HELP,      OPTION_FLAG_HELP,       "--help",       NULL,       "Show help-screen and exits.",         NULL },
     { OPTION_VERSION,   OPTION_FLAG_VERSION,    "-V",           NULL,       "Show version and exits.",             NULL },
     { OPTION_VERSION,   OPTION_FLAG_VERSION,    "--version",    NULL,       "Show version and exits.",             NULL },
     { OPTION_QUIET,     OPTION_FLAG_QUIET,      "-q",           NULL,       "Suppress messages.",                  NULL },
     { OPTION_QUIET,     OPTION_FLAG_QUIET,      "--quiet",      NULL,       "Suppress messages.",                  NULL },
     { OPTION_DEBUG,     OPTION_FLAG_DEBUG,      "-d",           NULL,       "Show debug messages.",                NULL },
     { OPTION_DEBUG,     OPTION_FLAG_DEBUG,      "--debug",      NULL,       "Show debug messages.",                NULL },
     { OPTION_FORCE,     OPTION_FLAG_FORCE,      "-f",           NULL,       "Try to recover from errors.",         NULL },
     { OPTION_FORCE,     OPTION_FLAG_FORCE,      "--force",      NULL,       "Try to recover from errors.",         NULL },
     { OPTION_RECURSIVE, OPTION_FLAG_RECURSIVE,  "-r",           NULL,       "Dive into directories recursively.",  NULL },
     { OPTION_RECURSIVE, OPTION_FLAG_RECURSIVE,  "--recursive",  NULL,       "Dive into directories recursively.",  NULL },
     { OPTION_NOFOLLOW,  OPTION_FLAG_NOFOLLOW,   "-n",           NULL,       "Do not follow symbolic links.",       NULL },
     { OPTION_NOFOLLOW,  OPTION_FLAG_NOFOLLOW,   "--nofollow",   NULL,       "Do not follow symbolic links.",       NULL },
     { OPTION_OVERWRITE, OPTION_FLAG_OVERWRITE,  "-o",           NULL,       "Overwrite existing files.",           NULL },
     { OPTION_OVERWRITE, OPTION_FLAG_OVERWRITE,  "--overwrite",  NULL,       "Overwrite existing files.",           NULL },
     { OPTION_IPV4,      OPTION_FLAG_IPV4,       "-4",           NULL,       "Use IPv4.",                           NULL },
     { OPTION_IPV4,      OPTION_FLAG_IPV4,       "--ipv4",       NULL,       "Use IPv4.",                           NULL },
     { OPTION_IPV6,      OPTION_FLAG_IPV6,       "-6",           NULL,       "Use IPv6.",                           NULL },
     { OPTION_IPV6,      OPTION_FLAG_IPV6,       "--ipv6",       NULL,       "Use IPv6.",                           NULL },
     { OPTION_SERVER,    OPTION_FLAG_SERVER,     "-F",           "=[addr]",  "Work as a server.",                   NULL },
     { OPTION_SERVER,    OPTION_FLAG_SERVER,     "--from",       "=[addr]",  "Work as a server.",                   NULL },
     { OPTION_CLIENT,    OPTION_FLAG_CLIENT,     "-T",           "=[addr]",  "Work as a client.",                   NULL },
     { OPTION_CLIENT,    OPTION_FLAG_CLIENT,     "--to",         "=[addr]",  "Work as a client.",                   NULL },
     { OPTION_PORT,      OPTION_FLAG_PORT,       "-p",           "=[port]",  "Set up a port number.",               NULL },
     { OPTION_PORT,      OPTION_FLAG_PORT,       "--port",       "=[port]",  "Set up a port number.",               NULL },
     { OPTION_LAST,      OPTION_FLAG_LAST,       NULL,           NULL,       NULL,                                  NULL } };

/**************************************************************************************************************************
 * ============================================= *** Function Prototypes *** ============================================ *
 **************************************************************************************************************************/

char*  GetOption(const unsigned int ID, const bool Long);
size_t GetOptionMaxWidth(const bool Long);

/**************************************************************************************************************************
 * ============================================= *** GetOption() Function *** =========================================== *
 **************************************************************************************************************************/

char* GetOption(const unsigned int ID, const bool Long){

    char  *Result = NULL;
    size_t oSign;

    if( Long )  oSign = 2;  else  oSign = 1;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (Result=(char*)(DataBase->Option)); DataBase++)
        if( (DataBase->ID) == ID)  if( !strncmp(Result,"--",oSign) )  break;

return(Result); }

/**************************************************************************************************************************
 * ======================================== *** GetOptionMaxWidth() Function *** ======================================== *
 **************************************************************************************************************************/

size_t GetOptionMaxWidth(const bool Long){

    size_t Result = 0;
    size_t oSign;
    char  *dbOption;

    if( Long )  oSign = 2;  else  oSign = 1;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (dbOption=(char*)(DataBase->Option)); DataBase++)
        if( !strncmp(dbOption,"--",oSign) )
        {
            size_t OptionWidth = strlen(DataBase->Option);
            char  *dbOptionExt = (char*)(DataBase->OptionExt);

            if( dbOptionExt ) OptionWidth = OptionWidth + strlen(dbOptionExt);
            if( OptionWidth > Result )  Result = OptionWidth;
        };
return(Result); }

/**************************************************************************************************************************
 * =========================================== *** GetOptionID() Function *** =========================================== *
 **************************************************************************************************************************/

unsigned int GetOptionID(const char *argv){

    unsigned int  Result;

    if(argv)
        for(struct OPTIONS_DATA *DataBase = &ODataDefault[0]; (Result=(DataBase->ID)) < OPTION_LAST; DataBase++)
        {
            size_t strLength = _POSIX_PATH_MAX;
            char  *CurrentOption, *CurrentParam;

            if( !(CurrentOption=(char*)(DataBase->Option)) ) break;
            if( (CurrentParam=(char*)(DataBase->OptionExt)) )
            {
                strLength = strlen(CurrentOption);

                char *Separator = (char*)(argv + strLength);
                if( *Separator == *CurrentParam )  SetOptionVar(Result,(Separator+1));
            };
            if(!strncmp(argv,CurrentOption,strLength))  return(Result);
        };
return(OPTION_NULL); }

/**************************************************************************************************************************
 * ========================================== *** GetOptionFlag() Function *** ========================================== *
 **************************************************************************************************************************/

unsigned int GetOptionFlag(const unsigned int ID){

    unsigned int Result = 0;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (DataBase->Option); DataBase++)
        if( (DataBase->ID) == ID) {   Result = (unsigned int)(DataBase->Flag);  break;   };

return(Result); }

/**************************************************************************************************************************
 * ========================================== *** GetOptionHelp() Function *** ========================================== *
 **************************************************************************************************************************/

const char* GetOptionHelp(const unsigned int ID){

    char *Result = NULL;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (DataBase->Option); DataBase++)
        if( (DataBase->ID) == ID) {   Result = (char*)(DataBase->Help);  break;   };

return((const char*)Result); }

/**************************************************************************************************************************
 * =========================================== *** GetOptionVar() Function *** ========================================== *
 **************************************************************************************************************************/

char* GetOptionVar(const unsigned int ID){

    char *Result = NULL;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (DataBase->Option); DataBase++)
        if( (DataBase->ID) == ID) {   Result = (char*)(DataBase->Var);  break;   };

return(Result); }

/**************************************************************************************************************************
 * =========================================== *** SetOptionVar() Function *** ========================================== *
 **************************************************************************************************************************/

size_t SetOptionVar(const unsigned int ID, char *Var){

    size_t Result = 0;

    for(struct OPTIONS_DATA *DataBase=&ODataDefault[0]; (DataBase->Option); DataBase++)
        if( (DataBase->ID) == ID) {   DataBase->Var = Var;  Result++;   };

return(Result); }

/**************************************************************************************************************************
 * ============================================ *** FormatHelp() Function *** =========================================== *
 **************************************************************************************************************************/

size_t FormatHelp(void){

    size_t OptionMaxWidth = GetOptionMaxWidth((const bool)0);

    printf("  %z  \r\n",OptionMaxWidth);

return(0); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

