/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: main.c (version 0.9).                                                                                        *
 *     Type: main program.                                                                                                *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU GPL (ver.3+).                                                                                         *
 *     Dependency:                                                                                                        *
 *                 -                                                                                                      *
 *                 - options module (options.h, options.c),                                                               *
 *                 - clock module (clock.h, clock.c),                                                                     *
 *                 - network module (network.h, network.c).                                                               *
 *     Desription: .                                                                                                      *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     Copyleft, 2017-2019, <feedback@babushkin.ru.net>, Alexander Babushkin.                                             *
 *                                                                                                                        *
 *     This library is free software; you can redistribute it and/or modify it under the terms of the GNU General Public  *
 * License as published by the Free Software Foundation; either version 3 of the License, or (at your option) any later   *
 * version.                                                                                                               *
 *     This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied *
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details. *
 *                                                                                                                        *
 **************************************************************************************************************************/

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#include "base.h"
#include "clock.h"
#include "options.h"
#include "network.h"

#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <error.h>
#include <sys/stat.h>
#include <sys/utsname.h>

/**************************************************************************************************************************
 * ============================================== *** Global variables *** ============================================== *
 **************************************************************************************************************************/

enum APP_STATUS {
    STATUS_QUEUEINIT,
    STATUS_CONFIGURE,
    STATUS_LAST
};

struct APP_OPTIONS {
    unsigned int             aFlags;                           // The Application flags;
    unsigned int             iFiles;                           // Index of the given files;
    unsigned int             iMax;                             //
    unsigned int            *oIndexes;                         // Temporary buffer for the indexes;
    char                    *rHost;                            // Remote Host;
    char                    *rPort;                            // Remote port.
};

struct MAIN_DATA {
    enum   APP_STATUS        Status;                           // Status of the message-loop;
    struct APP_CLOCK         Time;                             // Current time;
    struct APP_OPTIONS       Options;                          //
    struct utsname           SysInfo;                          // System information;
    struct sigaction         Signals;                          //
    struct NETWORK_DATA      Network;                          //
} *MainData;

/**************************************************************************************************************************
 * ============================================ *** Function prototypes *** ============================================= *
 **************************************************************************************************************************/

bool SignalHandlerInit(struct sigaction *Action);
void SignalHandler(int signo);
bool GetSystemInfo(struct utsname *SysInfo);

unsigned int  Main_ParsingCommandLine(struct APP_OPTIONS *Options, char **ArgV);
int           Main_ShowDebugInfo(struct utsname *SysInfo, struct APP_CLOCK *Time, struct APP_OPTIONS *Options);
int           Main_ShowOptionsInfo(unsigned int *oIndexes, unsigned int oMax);
unsigned int  Main_Configuring(struct CONNECT_INFO *NewConnection, struct APP_OPTIONS *Options);
/**************************************************************************************************************************
 * ============================================== *** main() function *** =============================================== *
 **************************************************************************************************************************/

int main(int argc, char *argv[], char *env[]){

    unsigned int  oFlags = OPTION_LAST;      // Initializing flags;

    /* Memory allocation ----------------------------------------------------------------------------------- */

    if( MainData = (struct MAIN_DATA*)calloc(1,sizeof(struct MAIN_DATA)) )
        if( MainData->Options.oIndexes = (unsigned int*)calloc(GetOptionIndex(oFlags),sizeof(unsigned int)) )
            oFlags = OPTION_NULL;

    if( oFlags )  error(errno,errno," Fatal! Can`t allocate memory!  (%d) ",errno);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Initializing the network structures and flags ------------------------------------------------------- */

    if( !SignalHandlerInit(&(MainData->Signals)) )                                                            // Initializating signal handler.
        error(errno,errno," Setting up the signal handler failed.  (%d) ",errno);

    GetTimeStart(&(MainData->Time),NULL);                                                                     // Initializating timer.
    GetSystemInfo(&(MainData->SysInfo));                                                                      // Get some system info.

    MainData->Options.iMax = GetOptionIndex(OPTION_LAST);
    if( !( MainData->Options.iFiles = Main_ParsingCommandLine(&(MainData->Options),&argv[0]) ) )              // Parsing command-line.
        exit(EXIT_SUCCESS);

    oFlags = (MainData->Options.aFlags);
#if defined (APP_DEBUG)
    MainData->Options.aFlags = oFlags = (oFlags|OPTION_DEBUG);
#endif
    if( oFlags & OPTION_DEBUG )
    {
        MainData->Options.aFlags = oFlags = (oFlags|OPTION_QUIET)^OPTION_QUIET;
        Main_ShowDebugInfo(&(MainData->SysInfo),&(MainData->Time),&(MainData->Options));                      // Show some debug info.
    };
    free(MainData->Options.oIndexes);

    if( !( oFlags = Main_Configuring(&(MainData->Network.iConnection[0]),&(MainData->Options)) ) )
        exit(EXIT_FAILURE);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Configuring the network connections ----------------------------------------------------------------- */
    {
        size_t    numConnections = 0;
        struct addrinfo *pHandle = NetworkConfigureInit((MainData->Options.rHost),(MainData->Options.rPort),&(MainData->Network.iConnection[0]));
        bool             fServer = oFlags & OPTION_SERVER;

        for(struct addrinfo *Handle = pHandle; Handle; Handle = Handle->ai_next)
        {
            struct CONNECT_INFO *NewConnection  = &(MainData->Network.iConnection[numConnections]);            //
            errno   = 0;

            if( !(oFlags & OPTION_QUIET) )
                switch(fServer)
                {   case false:  printf("      Connecting to: ");  break;
                    default:     printf("      Configuring connection: ");  break;
                };
            if( NetworkConfigureNext(Handle,NewConnection) )
            {
                printf(" [ %s @%s ] ",&(NewConnection->HostInfo.HostNum[0]),
                                      &(NewConnection->HostInfo.PortNum[0]));
                fflush(stdout);
                if( NetworkConfigureSocket(NewConnection,fServer) )  numConnections++;
            };
            if( !(oFlags & OPTION_QUIET) )  printf("- (%d) %s.\r\n",(NewConnection->Socket.ErrCode),
                                                                   &(NewConnection->Socket.ErrMsg[0]));
            if( (!fServer)&&(numConnections) )  break;
        };
        if( pHandle )  freeaddrinfo(pHandle);
        MainData->Network.nConnections = numConnections;
    };
    if( !(MainData->Network.nConnections) )
    {
Exit02: puts(" There is no more connections left... ");
        MainData->Options.aFlags |= OPTION_LAST;
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Main message-loop queue ----------------------------------------------------------------------------- */
    while( (oFlags=MainData->Options.aFlags) < OPTION_LAST )
    {
        static struct epoll_event  ePollEvent;                                                                //
        int                        ePollHandle;                                                               //
        int                        ePollReady;                                                                //

        switch(MainData->Status)
        {
            case STATUS_QUEUEINIT:     /* - - - - - epoll() initialization - - - - - */
                if( oFlags & OPTION_DEBUG )  printf(" Message queue initialization... ");
                if( ( ePollHandle = epoll_create((int)true) ) < 0 )  break;
                if( oFlags & OPTION_DEBUG )  puts(" OK ");

                MainData->Status = STATUS_CONFIGURE;
                break;

            case STATUS_CONFIGURE:
                if( oFlags & OPTION_DEBUG )  printf(" Registering sockets: ");
                {
                    size_t nSuccess = 0;
                    size_t iConn    = 0;
                    size_t iConnMax = (MainData->Network.nConnections);

                    while( iConn < iConnMax )
                    {
                        struct CONNECT_INFO *Connection = &(MainData->Network.iConnection[iConn]);

                        ePollEvent.events   = EPOLLIN|EPOLLOUT;
                        ePollEvent.data.ptr = Connection;
                        switch( epoll_ctl(ePollHandle,EPOLL_CTL_ADD,(Connection->Socket.Handle),&ePollEvent) )
                        {
                            case 0:
                                if( oFlags & OPTION_DEBUG )
                                    printf(" %d.[%d],",iConn,(Connection->Socket.Handle));
                                nSuccess++;  break;
                            default:
                                if( oFlags & OPTION_DEBUG )
                                    printf(" %d:(%d) - %s, ",iConn,errno,strerror(errno));
                                close(Connection->Socket.Handle);
                                Connection->Socket.Handle = -1;
                        };
                        iConn++;
                    };
                    if( !( MainData->Network.nConnections = nSuccess ) )  goto Exit02;
                };
                if( oFlags & OPTION_DEBUG )  puts(" ... OK \r\n");
                MainData->Status++;
                break;

            default:
                if ( !ePollReady )
                {
                    if( GetTimeDiff(&(MainData->Time),NULL) > 0 )
                        if( !(oFlags & OPTION_QUIET) )
                        {
                            char *ConfStr;
                            if( oFlags & OPTION_CLIENT )  ConfStr = "data";  else
                                                          ConfStr = "connection";
                            printf(" Waiting for %s: %s ...  \r",ConfStr,&(MainData->Time.String[0]));
                            fflush(stdout);
                        };
                    break;
                };
                {
                    struct CONNECT_INFO *Listener      = (struct CONNECT_INFO*)ePollEvent.data.ptr;
                    struct CONNECT_INFO *NewConnection = &(MainData->Network.iConnection[ (MainData->Network.nConnections) ]);

                    if( (ePollEvent.events) & (EPOLLIN|EPOLLOUT) )
                    {
                        switch( (Listener->Status)&(INT_MAX) )
                        {
                            case CONNECTION_NULL:
                                break;

                            case CONNECTION_LISTENER:
                                if( NetworkConfigureAccept(Listener,NewConnection) )  MainData->Network.nConnections++;
                                if( !(oFlags & OPTION_QUIET) )
                                {   printf("     Connecting to: [ %s ] ... (%d) %s. \r\n",&(NewConnection->HostInfo.HostNum[0]),
                                                                                           (NewConnection->Socket.ErrCode),
                                                                                          &(NewConnection->Socket.ErrMsg[0]));
                                    fflush(stdout);
                                };
                                break;

                            case CONNECTION_ACTIVE:
                                break;
                        };
                    };
                    if( (ePollEvent.events) & (EPOLLERR|EPOLLHUP) )
                    {};
                };
        };
        if( (ePollReady = epoll_wait(ePollHandle,&ePollEvent,1,1000)) < 0 )  break;
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    error(EXIT_SUCCESS,errno," Exit program (%d)",errno);

    /* Closing all connections */
    memset(MainData,0,sizeof(struct MAIN_DATA));
    free(MainData);

return(EXIT_SUCCESS); }

/**************************************************************************************************************************
 * ======================================= *** SignalHandlerInit() function *** ========================================= *
 **************************************************************************************************************************/

bool SignalHandlerInit(struct sigaction *Action){

    bool       Result     = false;
    const int  Signals[5] = {   SIGQUIT, SIGHUP, SIGTERM, SIGTSTP, SIGINT   };

    if(Action)
    {   Action->sa_handler = (SignalHandler);
        Action->sa_flags   = (SA_RESETHAND|SA_NODEFER);

        if( (Result = (bool)(sigfillset(&(Action->sa_mask))+1)) )
            for(unsigned int i=0; i<5; i++)
                if( !(Result = (bool)(sigaction(Signals[i],Action,NULL)+1)) )  break;
    };
return(Result); }

/**************************************************************************************************************************
 * ========================================= *** SignalHandler() function *** =========================================== *
 **************************************************************************************************************************/

void SignalHandler(int SigNo){

    switch(SigNo)
    {
        case SIGQUIT:
        case SIGHUP:
        case SIGTERM:
        case SIGTSTP:
        case SIGINT:
            MainData->Options.aFlags|=OPTION_LAST;
        default:
            break;
    };
}

/**************************************************************************************************************************
 * =================================== *** Main_ParsingCommandLine() function *** ======================================= *
 **************************************************************************************************************************/

bool GetSystemInfo(struct utsname *SysInfo){

    bool Result = false;

    if( SysInfo )
        if( !uname(&(MainData->SysInfo)) )
        {
            char *ThisHost = &(SysInfo->nodename[0]);
            if( !*ThisHost )
                gethostname(ThisHost,sizeof(SysInfo->nodename));
            Result = true;
        };
return(Result); }

/**************************************************************************************************************************
 * =================================== *** Main_ParsingCommandLine() function *** ======================================= *
 **************************************************************************************************************************/

unsigned int Main_ParsingCommandLine(struct APP_OPTIONS *Options, char **ArgV){

    unsigned int  oCurrent = 1;                                                                               // Current index.
    unsigned int *pID=(Options->oIndexes);

    for(unsigned int oID; oID=GetOptionID(ArgV[oCurrent]); oCurrent++ )                                       // Recognizing a string:
        switch(oID)
        {
            case OPTION_HELP:     ShowHelp();  return(0);                                                 // - the case for a "help" option;
            case OPTION_VERSION:  ShowVersion();  return(0);                                              // - the case for a "version" option;
            default:                                                                                                   // - the case for a default scenario:
                Options->aFlags|= oID;                                                                              //
               *(pID + GetOptionIndex(oID)) = oCurrent;                                                     //
        };
return(oCurrent); }

/**************************************************************************************************************************
 * ====================================== *** Main_ShowDebugInfo() function *** ========================================= *
 **************************************************************************************************************************/

int Main_ShowDebugInfo(struct utsname *SysInfo, struct APP_CLOCK *Time, struct APP_OPTIONS *Options){

    int Result = 0;

    if( (SysInfo)&&(Time)&&(Options) )
    {
        Result+=ShowVersion();
        Result+=printf(" Program startup: %s; \r\n",&(Time->String[0]));

        Result+=printf(" On %s host: %s \r\n",&(SysInfo->sysname[0]),
                                          &(SysInfo->nodename[0]));
        Result+=printf(" Allocating: %u bytes. \r\n\r\n",sizeof(struct MAIN_DATA));
        Result+=puts(" Parsing options: ");

        Result+=Main_ShowOptionsInfo((Options->oIndexes),(Options->iMax));
    };
return(Result); }

/**************************************************************************************************************************
 * ===================================== *** Main_ShowOptionsInfo() function *** ======================================== *
 **************************************************************************************************************************/

int Main_ShowOptionsInfo(unsigned int *oIndexes, unsigned int oMax){

    int Result = 0;

    if( oIndexes )
        for(unsigned int i=0; i<oMax; i++,oIndexes++)
            if( *oIndexes )  Result+=printf("     % 2u:% 2u: %s \r\n",i,*oIndexes,GetOptionHelp(1<<(i-1)));

return(Result); }

/**************************************************************************************************************************
 * ===================================== *** Main_ShowOptionsInfo() function *** ======================================== *
 **************************************************************************************************************************/

unsigned int Main_Configuring(struct CONNECT_INFO *NewConnection, struct APP_OPTIONS *Options){

    if( (NewConnection)&&(Options) )
    {
        unsigned int oFlags = (Options->aFlags);

        NewConnection->AddrInfo.ai_protocol = IPPROTO_TCP;                                                    // Specifing the protocol and
        NewConnection->AddrInfo.ai_socktype = SOCK_STREAM;                                                    // the preferred socket type.

        switch( oFlags & (OPTION_IPV4|OPTION_IPV6) )
        {
            case OPTION_IPV4:    NewConnection->AddrInfo.ai_family = AF_INET;  break;                         // - the case for a "IPv4" option;
            case OPTION_IPV6:    NewConnection->AddrInfo.ai_family = AF_INET6;  break;                        // - the case for a "IPv6" option;
            default:             NewConnection->AddrInfo.ai_family = AF_UNSPEC;  break;                       // - the case for a default IP-protocol;
        };
        switch( oFlags & (OPTION_SERVER|OPTION_CLIENT) )
        {
            case 0:              oFlags = oFlags|OPTION_SERVER;

            case OPTION_SERVER:  NewConnection->AddrInfo.ai_flags|= AI_PASSIVE;                               // - the case for a "server" option;
                                 if( !(oFlags & OPTION_QUIET) )  puts("\r\n Creating a server... ");
                                 Options->rHost = GetOptionVar(OPTION_SERVER);
                                 break;

            case OPTION_CLIENT:  if( !(oFlags & OPTION_QUIET) )  puts("\r\n Creating a client... ");          // - the case for a "client" option;
                                 Options->rHost = GetOptionVar(OPTION_CLIENT);
                                 break;

            default:             puts("\r\n Error: ambigous options. Can`t create client and server at the same time. \r\n");
                                 return(OPTION_NULL);                                                         // - detecting errors.
        };
        Options->aFlags = oFlags;

        if( oFlags & OPTION_PORT )  Options->rPort = GetOptionVar(OPTION_PORT);                               //
        if( !(MainData->Options.rPort) )  Options->rPort = "45678\0";                                         // Default port.

        return(oFlags);
    };
return(OPTION_NULL); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

