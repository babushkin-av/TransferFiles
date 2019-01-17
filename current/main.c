/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: main.c (version 0.8).                                                                                        *
 *     Type: main program.                                                                                                *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU General Public License version 3.                                                                     *
 *     Dependency: options module (options.h, options.c), network module (network.h, network.c)                           *
 *     Desription: main program.                                                                                          *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     Copyleft, 2017-2018, <feedback@babushkin.ru.net>, Alexander Babushkin.                                             *
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
#include <sys/types.h>
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

struct MAIN_DATA {
    unsigned int             Flags;                            // The Application flags;
    unsigned int             Files;                            // Index of the given files;
    char                    *Host;                             //
    char                    *Port;                             //
    enum   APP_STATUS        Status;                           // Status of the message-loop;
    struct APP_CLOCK         Time;                             // Current time;
    struct sigaction         Signals;                          //
    struct utsname           SysInfo;                          // System information;
    struct NETWORK_DATA      Network;                          //
} *MainData;

/**************************************************************************************************************************
 * ============================================ *** Function prototypes *** ============================================= *
 **************************************************************************************************************************/

#if defined (APP_DEBUG)
#    define ErrMsg(str)          fprintf(stderr,"\r\n {%s,%u}: %s (%d): %s \r\n",__FILE__,__LINE__,(str),errno,strerror(errno))
#else
#    define ErrMsg(str)          fprintf(stderr,"\r\n %s (%d): %s \r\n",(str),errno,strerror(errno))
#endif

bool SignalHandlerInit(struct sigaction *Action);
void SignalHandler(int signo);

/**************************************************************************************************************************
 * =============================================== *** main function *** ================================================ *
 **************************************************************************************************************************/

int main(int argc, char *argv[], char *env[]){

    unsigned int  oFlags = OPTION_NULL;      // Initializing flags;

#if defined (APP_DEBUG)
    oFlags = oFlags|OPTION_DEBUG;
#endif

    /* Memory allocation ----------------------------------------------------------------------------------- */
    if( !( MainData = (struct MAIN_DATA*)calloc(1,sizeof(struct MAIN_DATA)) ) )
Exit01: error(errno,errno," Fatal! Can`t allocate memory!  (%d) ",errno);
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Get some system info -------------------------------------------------------------------------------- */
    {
        GetTimeStart(&(MainData->Time),NULL);                                                                  // Initializating timer.
        uname(&(MainData->SysInfo));

        char *ThisHost = &(MainData->SysInfo.nodename[0]);
        if( !ThisHost )  gethostname(ThisHost,sizeof(MainData->SysInfo.nodename));
    };
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Initializating signal handler ----------------------------------------------------------------------- */
    {
        MainData->Signals.sa_handler = (SignalHandler);
        if( !(SignalHandlerInit(&(MainData->Signals))) )
            error(errno,errno," Setting up the signal handler failed.  (%d) ",errno);
    };
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Parsing command-line ----------------------------------------------------------------- */
    {
        unsigned int  oID;                                                                                    // Option ID;
        unsigned int  oCurrent = 1;                                                                           // Current index.
        unsigned int  oMax     = GetOptionIndex(OPTION_LAST);                                                 // Maximum number of Indexes;
        unsigned int *oIndexes = (unsigned int*)calloc(oMax,sizeof(unsigned int));                            // Temporary buffer for the indexes;

        if( !oIndexes )  goto Exit01;

        while( oID = GetOptionID(argv[oCurrent]) )                                                            // Recognizing a string:
        {
            if( oID == OPTION_HELP )  {   ShowHelp();  exit(EXIT_SUCCESS);   };                               // - the case for a "help" option;
            if( oID == OPTION_VERSION )  {   ShowVersion();  exit(EXIT_SUCCESS);   };                         // - the case for a "version" option;
                                                                                                              // - the case for a default scenario:
            oFlags = oFlags|oID;                                                                              //
            *(oIndexes + GetOptionIndex(oID)) = oCurrent;                                                     //
            oCurrent++;                                                                                       //
        };
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        if( oFlags & OPTION_DEBUG ) /* Show some debug info ------------------------------------------------ */
        {
            ShowVersion();
            printf(" Program startup: %s; \r\n",&(MainData->Time.String[0]));
            printf(" On %s host: %s \r\n",&(MainData->SysInfo.sysname[0]),
                                          &(MainData->SysInfo.nodename[0]));
            printf(" Allocating: %u+%u bytes. \r\n\r\n",sizeof(struct MAIN_DATA),oMax*sizeof(unsigned int));

            puts(" Parsing options: ");
            for(unsigned int i=1; i<oMax; i++)
                if( oID = *(oIndexes+i) )
                    printf("     % 2u:% 2u: %s \r\n",i,oID,GetOptionHelp(1<<(i-1)));

            oFlags = (oFlags|OPTION_QUIET)^OPTION_QUIET;
        };
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        /* Initializing the network structures and flags --------------------------------------------------- */
        struct CONNECT_INFO *NewConnection = &(MainData->Network.iConnection[0]);                             //

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
                                 MainData->Host = GetOptionVar(OPTION_SERVER);  break;

            case OPTION_CLIENT:  if( !(oFlags & OPTION_QUIET) )  puts("\r\n Creating a client... ");          // - the case for a "client" option;
                                 MainData->Host = GetOptionVar(OPTION_CLIENT);  break;

            default:             error(EINVAL,EINVAL," Error: ambigous options: '%s' and '%s'. ",             // - detecting ambiguous options.
                                     argv[ *(oIndexes + GetOptionIndex(OPTION_SERVER)) ],
                                     argv[ *(oIndexes + GetOptionIndex(OPTION_CLIENT)) ]);  break;
        };
        if( oFlags & OPTION_PORT )  MainData->Port = GetOptionVar(OPTION_PORT);                               //
        if( !(MainData->Port) )     MainData->Port = "45678\0";                                               // Default port.

        NewConnection->AddrInfo.ai_protocol = IPPROTO_TCP;                                                    // Specifing the protocol and
        NewConnection->AddrInfo.ai_socktype = SOCK_STREAM;                                                    // the preferred socket type.

        MainData->Flags = oFlags;
        MainData->Files = oCurrent;
        free(oIndexes);
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Configuring the network connections ----------------------------------------------------------------- */
    {
        size_t    numConnections = 0;
        struct addrinfo *pHandle = NetworkConfigureInit((MainData->Host),(MainData->Port),&(MainData->Network.iConnection[0]));
        bool             fServer = (oFlags & OPTION_SERVER);

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
        MainData->Flags |= OPTION_LAST;
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Main message-loop queue ----------------------------------------------------------------------------- */
    while( (oFlags=MainData->Flags) < OPTION_LAST )
    {
        static struct epoll_event  ePollEvent;                                                                //
        int                        ePollHandle;                                                               //
        int                        ePollReady;                                                                //

        switch(MainData->Status)
        {
            case STATUS_QUEUEINIT:
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

//                    if( (ePollEvent.events) & (EPOLLIN|EPOLLOUT) )
//                        if( ( NewConnection->Socket.Handle = accept((Listener->Socket.Handle),(NewConnection->AddrInfo.ai_addr),&(NewConnection->AddrInfo.ai_addrlen)) ) < 0 )
//                    if( NewConnection->Socket.Handle >= 0 )
//                    {
//                        if( NetworkConfigureNext(&(NewConnection->AddrInfo),NewConnection) )
//                            if( !(oFlags & OPTION_QUIET) )  {   printf("     Connecting to: [ %s ] ... OK ",&(NewConnection->HostInfo.HostName[0]));  fflush(stdout);   };
//                    };
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

    bool result = false;
    const int  Signals[5] = {   SIGQUIT, SIGHUP, SIGTERM, SIGTSTP, SIGINT   };

    if(Action)
    {   Action->sa_flags = (SA_RESETHAND|SA_NODEFER);

        if( (result = (bool)(sigfillset(&(Action->sa_mask))+1)) )
            for(unsigned int i=0; i<5; i++)
                if( !(result = (bool)(sigaction(Signals[i],Action,NULL)+1)) )  break;
    };
return(result); }

/**************************************************************************************************************************
 * ========================================== *** SignalHandler function *** ============================================ *
 **************************************************************************************************************************/

void SignalHandler(int signo){

    switch(signo)
    {
        case SIGQUIT:
        case SIGHUP:
        case SIGTERM:
        case SIGTSTP:
        case SIGINT:
            MainData->Flags|=OPTION_LAST;
        default:
            break;
    };
}

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

