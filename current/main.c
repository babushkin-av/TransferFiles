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

/**************************************************************************************************************************
 * ============================================== *** Global variables *** ============================================== *
 **************************************************************************************************************************/


enum APP_STATUS {
    STATUS_INITIALIZE,
    STATUS_CONFIGURE,
    STATUS_CLIENT_WAIT,
    STATUS_CLIENT_CONNECT,
    STATUS_CLIENT_HANDSHAKE,
    STATUS_CLIENT_SENDFILES,
    STATUS_CLIENT_GOODBYE,
    STATUS_SERVER_WAIT,
    STATUS_SERVER_CONNECT,
    STATUS_SERVER_HANDSHAKE,
    STATUS_SERVER_RECVFILES,
    STATUS_LAST
};


struct MAIN_DATA {
    unsigned int             Flags;                            // The Application flags;
    unsigned int             Files;                            // Index of the given files;
    char                    *Host;                             //
    char                    *Port;                             //
    enum   APP_STATUS        Status;                           // Status of the message-loop;
    struct APP_CLOCK         Time;                             // Current time;
    struct sigaction         Signals;
    struct NETWORK_DATA      Network;
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

    /* Memory allocation */
    if( !( MainData = (struct MAIN_DATA*)calloc(1,sizeof(struct MAIN_DATA)) ) )
        error(errno,errno," Fatal! Can`t allocate memory!  (%d) ",errno);

    if( argc > 1 )
    {
        struct CONNECT_INFO   *NewConnection = &(MainData->Network.iConnection[0]);

        unsigned int  oMax;
        unsigned int  oID;         // Option ID;
        unsigned int  oCurrent;    // Current index;
        unsigned int *oIndexes;    // Temporary buffer for the indexes;

        /* Initializing indexes ---------------------------------------------------------------------------- */
        if( !( oIndexes = (unsigned int*)calloc(oMax=GetOptionIndex(OPTION_LAST),sizeof(unsigned int)) ) )
            error(errno,errno," Fatal! Can`t allocate memory!  (%d) ",errno);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        /* Parsing command-line ---------------------------------------------------------------------------- */
        for(oCurrent=1; oID=GetOptionID(argv[oCurrent]); oCurrent++)
            switch(oID)                                                                                       // Recognizing a string:
            {   case OPTION_HELP:      ShowHelp();  exit(EXIT_SUCCESS);  break;                               // - the case for a "help" option;
                case OPTION_VERSION:   ShowVersion();  exit(EXIT_SUCCESS);  break;                            // - the case for a "version" option;
                default:    oFlags = oFlags|oID;  *(oIndexes + GetOptionIndex(oID)) = oCurrent;  break;       // - the case for a default scenario.
            };
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        if( oFlags & OPTION_DEBUG ) /* Show some debug info ------------------------------------------------ */
        {
            ShowVersion();
            printf(" Allocating (%u+%u) bytes...  OK \r\n\r\n Parsing options: \r\n",sizeof(struct MAIN_DATA),oMax*sizeof(unsigned int));
            for(unsigned int i=1; i<oMax; i++)  if( oID = *(oIndexes+i) )  printf("\t % 2u:% 2u: %s \r\n",i,oID,GetOptionHelp(1<<(i-1)));
            oFlags = (oFlags|OPTION_QUIET)^OPTION_QUIET;
        };
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        switch( oFlags & (OPTION_IPV4|OPTION_IPV6) ) /* Initializing the network structures and flags ------ */
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
                                 MainData->Host = GetOptionVar(OPTION_CLIENT);
                                 break;

            default:             error(EINVAL,EINVAL," Error: ambigous options: '%s' and '%s'. ",             // - detecting ambiguous options.
                                     argv[ *(oIndexes + GetOptionIndex(OPTION_SERVER)) ],
                                     argv[ *(oIndexes + GetOptionIndex(OPTION_CLIENT)) ]);  break;
        };
        if( oFlags & OPTION_PORT )  MainData->Port = GetOptionVar(OPTION_PORT);                               //
        if( !(MainData->Port) )     MainData->Port = "45678\0";                                               //

        NewConnection->AddrInfo.ai_protocol = IPPROTO_TCP;                                                    // Specifing the protocol and
        NewConnection->AddrInfo.ai_socktype = SOCK_STREAM;                                                    // the preferred socket type.

        MainData->Flags = oFlags;
        MainData->Files = oCurrent;
        free(oIndexes);
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

        /* Configuring the network connections ------------------------------------------------------------- */
        struct addrinfo *Handle = NetworkConfigureInit((MainData->Host),(MainData->Port),NewConnection);
        while( Handle )
        {
            errno = 0;

            if( !( NetworkConfigureNext(Handle,NewConnection) ) )  break;
            if( !(oFlags & OPTION_QUIET) )
            {
                char *ConfStr;
                if( oFlags & OPTION_CLIENT )  ConfStr = "Connecting to";  else
                                              ConfStr = "Configuring connection";
                printf("     %s: [ %s @%s ] ",ConfStr,&(NewConnection->HostInfo.HostName[0]),
                                                      &(NewConnection->HostInfo.PortName[0]));
                fflush(stdout);
            };
            int Socket = socket((Handle->ai_family),(Handle->ai_socktype),(Handle->ai_protocol));
            if( Socket >= 0 )
            {
                if( oFlags & OPTION_CLIENT )
                    connect(Socket,(Handle->ai_addr),(Handle->ai_addrlen));  else
                    if( !( bind(Socket,(Handle->ai_addr),(Handle->ai_addrlen)) ) )  listen(Socket,0);
            };
            Handle = Handle->ai_next;

            if( !(oFlags & OPTION_QUIET) )  printf("- (%d/%d) %s \r\n",Socket,errno,strerror(errno));
            if( errno )  {   close(Socket);  continue;   };

            NewConnection->Socket.Handle = Socket;
            NewConnection++;
            MainData->Network.nConnections++;
        };
        if( (!Handle)&&(!MainData->Network.nConnections) )
            error(EINVAL,EINVAL," Error: there is no valid connections (%d) %s /",(NewConnection->Socket.ErrCode),
                                                                                 &(NewConnection->Socket.ErrMsg[0]));
        /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    };

    /* Initializating signal handlers */
    MainData->Signals.sa_handler = (SignalHandler);
    if( !(SignalHandlerInit(&(MainData->Signals))) )  MainData->Flags|= OPTION_LAST;

    /* Main message-loop queue */
    while( (oFlags=MainData->Flags) < OPTION_LAST )
    {
        struct CONNECT_INFO       *Connection;
        static struct epoll_event  ePollEvent;
        int                        ePollHandle;
        int                        ePollReady;

        switch(MainData->Status)
        {
            case STATUS_INITIALIZE:
                if( !(oFlags & OPTION_QUIET) )  printf(" Network initialization... ");
                if( ( ePollHandle = epoll_create((int)true) ) < 0 )  break;
                if( !(oFlags & OPTION_QUIET) )  puts(" OK ");
                GetLocalTime(&(MainData->Time),NULL);       // Initializating timer
                MainData->Status = STATUS_CONFIGURE;
                break;

            case STATUS_CONFIGURE:
                for(size_t i=0, iMax=(MainData->Network.nConnections); i<iMax; i++)
                {
                    Connection = &(MainData->Network.iConnection[i]);

                    ePollEvent.events   = EPOLLIN|EPOLLOUT;
                    ePollEvent.data.ptr = Connection;
                    epoll_ctl(ePollHandle,EPOLL_CTL_ADD,(Connection->Socket.Handle),&ePollEvent);
                };
                if( oFlags & OPTION_SERVER )  MainData->Status = STATUS_SERVER_WAIT;  else
                                              MainData->Status = STATUS_CLIENT_WAIT;
                break;

            case STATUS_CLIENT_WAIT:
                printf(" ...");

            case STATUS_CLIENT_CONNECT:
                break;

        };
        if( (ePollReady = epoll_wait(ePollHandle,&ePollEvent,1,1000)) < 0 )  break;
    };

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
    {
        Action->sa_flags = (SA_RESETHAND|SA_NODEFER);

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

