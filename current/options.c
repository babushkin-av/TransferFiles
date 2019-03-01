/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: options.c (version 0.9).                                                                                     *
 *     Type:  (options.c).                                                                                                *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: scandir.h.                                                                                             *
 *     Desription: .                                                                                                      *
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

#include "options.h"

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

const char *OHelpStr =  " \r\n"
                        "   Usage: transferfiles [options] [file1] [file2] ... \r\n\r\n"
                        "   Options: \r\n\r\n"
                        "     -h, --help        Show this help-screen and exits. \r\n\r\n"
                        "     -V, --version     Show version info and exits. \r\n\r\n"
                        "     -q, --quiet       Suppress informational messages. However, \r\n"
                        "                       error reports will be displayed. \r\n\r\n"
                        "     -d, --debug       Show debug messages. This supersedes \r\n"
                        "                       '-q' and/or '--quiet' options. \r\n\r\n"
                        "     -f, --force       Try to recover from errors. \r\n\r\n"
                        "     -r, --recursive   Dive into directories recursively. \r\n\r\n"
                        "     -n  --nofollow    Do not follow symbolic links. \r\n\r\n"
                        "     -o  --overwrite   Overwrite existing files. \r\n\r\n"
                        "     -4, --ipv4,       This is a hint as to which Protocol to use \r\n"
                        "     -6, --ipv6        when you specify a host name. \r\n\r\n"
                        "     -T=[addr],        Work as a client sending files TO [addr] \r\n"
                        "     --to=[addr]       (which is localhost by default). \r\n\r\n"
                        "     -F=[addr],        Work as a server reciving files FROM [addr] \r\n"
                        "     --from=[addr]     (which is INADDR_ANY by default). \r\n\r\n"
                        "     -p=[port], \r\n"
                        "     --port=[port]     Set up a port number (which is 45678 by default). \r\n";

struct OPTIONS_DATA ODataDefault[] = {
     { OPTION_HELP,     "--help",       NULL,       "Show help-screen and exits.",         NULL },
     { OPTION_HELP,     "-h",           NULL,       "Show help-screen and exits.",         NULL },
     { OPTION_VERSION,  "--version",    NULL,       "Show version and exits.",             NULL },
     { OPTION_VERSION,  "-V",           NULL,       "Show version and exits.",             NULL },
     { OPTION_QUIET,    "--quiet",      NULL,       "Suppress messages.",                  NULL },
     { OPTION_QUIET,    "-q",           NULL,       "Suppress messages.",                  NULL },
     { OPTION_DEBUG,    "--debug",      NULL,       "Show debug messages.",                NULL },
     { OPTION_DEBUG,    "-d",           NULL,       "Show debug messages.",                NULL },
     { OPTION_FORCE,    "--force",      NULL,       "Try to recover from errors.",         NULL },
     { OPTION_FORCE,    "-f",           NULL,       "Try to recover from errors.",         NULL },
     { OPTION_RECURSIVE,"--recursive",  NULL,       "Dive into directories recursively.",  NULL },
     { OPTION_RECURSIVE,"-r",           NULL,       "Dive into directories recursively.",  NULL },
     { OPTION_NOFOLLOW, "--nofollow",   NULL,       "Do not follow symbolic links.",       NULL },
     { OPTION_NOFOLLOW, "-n",           NULL,       "Do not follow symbolic links.",       NULL },
     { OPTION_OVERWRITE,"--overwrite",  NULL,       "Overwrite existing files.",           NULL },
     { OPTION_OVERWRITE,"-o",           NULL,       "Overwrite existing files.",           NULL },
     { OPTION_IPV4,     "--ipv4",       NULL,       "Use IPv4.",                           NULL },
     { OPTION_IPV4,     "-4",           NULL,       "Use IPv4.",                           NULL },
     { OPTION_IPV6,     "--ipv6",       NULL,       "Use IPv6.",                           NULL },
     { OPTION_IPV6,     "-6",           NULL,       "Use IPv6.",                           NULL },
     { OPTION_SERVER,   "--from",       "=[addr]",  "Work as a server.",                   NULL },
     { OPTION_SERVER,   "-F",           "=[addr]",  "Work as a server.",                   NULL },
     { OPTION_CLIENT,   "--to",         "=[addr]",  "Work as a client.",                   NULL },
     { OPTION_CLIENT,   "-T",           "=[addr]",  "Work as a client.",                   NULL },
     { OPTION_PORT,     "--port",       "=[port]",  "Set up a port number.",               NULL },
     { OPTION_PORT,     "-p",           "=[port]",  "Set up a port number.",               NULL },
     { OPTION_LAST, NULL, NULL, NULL } };

/**************************************************************************************************************************
 * ============================================== *** ShowHelp Function *** ============================================= *
 **************************************************************************************************************************/

int ShowHelp(void){

return( puts(OHelpStr) ); }

/**************************************************************************************************************************
 * ============================================ *** ShowVersion Function *** ============================================ *
 **************************************************************************************************************************/

int ShowVersion(void){

return( printf("\r\n This is %s for %s %s (%s version), \r\n Copyleft (c), %s, feedback@babushkin.ru.net. \r\n\r\n",
    APP_NAME,OPTIONS_OS_STRING,OPTIONS_DEBUG_STRING,APP_VERSION,__DATE__) ); }

/**************************************************************************************************************************
 * ========================================= *** GetOptionIndex() Function *** ========================================== *
 **************************************************************************************************************************/

unsigned int GetOptionIndex(unsigned int ID){

    unsigned int Result = 0;

    while(ID){   ID>>=1; Result++;   };

return(Result); }

/**************************************************************************************************************************
 * =========================================== *** GetOptionID() Function *** =========================================== *
 **************************************************************************************************************************/

unsigned int GetOptionID(const char *argv){

    unsigned int  result;

    if(argv)
        for(struct OPTIONS_DATA *DataBase = &ODataDefault[0]; (result = (DataBase->ID)) < OPTION_LAST; DataBase++)
        {
            size_t strLength = _POSIX_PATH_MAX;
            char  *CurrentOption, *CurrentParam;

            if( !( CurrentOption = (char*)(DataBase->Option) ) ) break;
            if( CurrentParam = (char*)(DataBase->OptionExt) )
            {
                strLength = strlen(CurrentOption);

                char *Separator = (argv + strLength);
                if( *Separator == *CurrentParam )  SetOptionVar(result,(Separator+1));
            };
            if(!strncmp(argv,CurrentOption,strLength))  return(result);
        };
return(OPTION_NULL); }

/**************************************************************************************************************************
 * ========================================== *** GetOptionHelp() Function *** ========================================== *
 **************************************************************************************************************************/

const char* GetOptionHelp(const unsigned int ID){

    struct OPTIONS_DATA *DataBase = &ODataDefault[0];

    while(DataBase->Option)
    {
        if( (DataBase->ID) == ID)  break;
        DataBase++;
    };
return(DataBase->Help); }

/**************************************************************************************************************************
 * =========================================== *** GetOptionVar() Function *** ========================================== *
 **************************************************************************************************************************/

char* GetOptionVar(const unsigned int ID){

    struct OPTIONS_DATA *DataBase = &ODataDefault[0];

    while(DataBase->Option)
    {
        if( (DataBase->ID) == ID)  break;
        DataBase++;
    };
return(DataBase->Var); }

/**************************************************************************************************************************
 * =========================================== *** SetOptionVar() Function *** ========================================== *
 **************************************************************************************************************************/

size_t SetOptionVar(const unsigned int ID, char *Var){

    struct OPTIONS_DATA *DataBase = &ODataDefault[0];
    size_t               result   = 0;

    while(DataBase->Option)
    {
        if( (DataBase->ID) == ID)  {   DataBase->Var = Var;  result++;   };
        DataBase++;
    };
return(result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

