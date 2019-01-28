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
    STATUS_REGISTER,
    STATUS_SERVER_WAIT,
    STATUS_SERVER_RECEIVE,
    STATUS_CLIENT_GREETINGS,
    STATUS_CLIENT_SEND,
    STATUS_LAST
};

struct APP_OPTIONS {
    unsigned int             oFlags;                           // The Application flags;
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
size_t        Main_SetupNewConnections(struct addrinfo *Handle, struct NETWORK_DATA *Net, unsigned int oFlags);

bool          MainQueue_Registering(int Handle, struct CONNECT_INFO *NewConnection, unsigned int oFlags);

/**************************************************************************************************************************
 * ============================================== *** main() function *** =============================================== *
 **************************************************************************************************************************/

int main(int argc, char *argv[], char *env[]){

    unsigned int  oFlags = OPTION_LAST;      // Initializing flags;

    /* Memory allocation ----------------------------------------------------------------------------------- */

    if( MainData = (struct MAIN_DATA*)calloc(1,sizeof(struct MAIN_DATA)) )
        if( MainData->Options.oIndexes = (unsigned int*)calloc(GetOptionIndex(oFlags),sizeof(unsigned int)) )
            if( MainData->Network.iConnection[0] = (struct CONNECT_INFO*)calloc(1,sizeof(struct CONNECT_INFO)) )
            oFlags = OPTION_NULL;

    if( oFlags )  error(errno,errno," Fatal! Can`t allocate memory!  (%d) ",errno);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Initializing "MainData" structures and flags -------------------------------------------------------- */

    if( !SignalHandlerInit(&(MainData->Signals)) )                                                            // Initializating signal handler.
        error(errno,errno," Setting up the signal handler failed.  (%d) ",errno);

    GetTimeStart(&(MainData->Time),NULL);                                                                     // Initializating timer.

    GetSystemInfo(&(MainData->SysInfo));                                                                      // Get some system info.

    MainData->Options.iMax   = GetOptionIndex(OPTION_LAST);
    MainData->Options.iFiles = Main_ParsingCommandLine(&(MainData->Options),&argv[0]);                        // Parsing command-line.

    oFlags = (MainData->Options.oFlags);
#if defined (APP_DEBUG)
    MainData->Options.oFlags = oFlags = (oFlags|OPTION_DEBUG);
#endif
    if( oFlags & OPTION_DEBUG )
    {
        MainData->Options.oFlags = oFlags = (oFlags|OPTION_QUIET)^OPTION_QUIET;
        Main_ShowDebugInfo(&(MainData->SysInfo),&(MainData->Time),&(MainData->Options));                      // Show some debug info.
    };
    free(MainData->Options.oIndexes);

    if( !( oFlags = Main_Configuring((MainData->Network.iConnection[0]),&(MainData->Options)) ) )
        exit(EXIT_FAILURE);

    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Configuring the network connections ----------------------------------------------------------------- */
    {
        struct addrinfo *Handle = NetworkConfigureInit((MainData->Options.rHost),
                                                       (MainData->Options.rPort),
                                                       (MainData->Network.iConnection[0]));

        MainData->Network.nConnections = Main_SetupNewConnections(Handle,&(MainData->Network),oFlags);

        if( Handle )  freeaddrinfo(Handle);
    };
    if( !(MainData->Network.nConnections) )
    {
Exit02: puts(" There is no more connections left... ");
        MainData->Options.oFlags |= OPTION_LAST;
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Main message-loop queue ----------------------------------------------------------------------------- */
    while( (oFlags=MainData->Options.oFlags) < OPTION_LAST )
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

                MainData->Status = STATUS_REGISTER;
                break;

            case STATUS_REGISTER:
                if( oFlags & OPTION_DEBUG )  printf(" Registering sockets: ");
                {
                    size_t nRegistered = 0;

                    struct CONNECT_INFO *NewConnection = (MainData->Network.iConnection[0]);

                    for(size_t i=0, iMax=(MainData->Network.nConnections); i<iMax; i++)
                    {
                        if( MainQueue_Registering(ePollHandle,NewConnection,oFlags) )  nRegistered++;
                    };
                    if( !( MainData->Network.nConnections = nRegistered ) )  goto Exit02;
                };
                if( oFlags & OPTION_DEBUG )  puts(" ... OK \r\n");
                MainData->Status++;
                break;

        };
        if( (ePollReady = epoll_wait(ePollHandle,&ePollEvent,1,1000)) < 0 )  break;
    };/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    error(EXIT_SUCCESS,errno," Exit program (%d)",errno);

    /* Closing all connections */
    for(struct CONNECT_INFO *OldConn=(MainData->Network.iConnection[0]),
                            *MaxConn=(MainData->Network.iConnection[NETWORK_MAX_CONN]); OldConn<MaxConn; OldConn++)
        if( OldConn )
        {   NetworkConfigureClose(-1,OldConn);  free(OldConn);   };

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
    {   Action->sa_handler = (void*)(SignalHandler);

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
        case SIGTERM:  exit(EXIT_SUCCESS);  break;
        case SIGTSTP:
        case SIGINT:
        case SIGQUIT:
        case SIGHUP:
        default:       MainData->Options.oFlags|=OPTION_LAST;  break;
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
    unsigned int *pID;

    if( Options )
        if( pID=(Options->oIndexes) )
            for(unsigned int oID; oID=GetOptionID(ArgV[oCurrent]); oCurrent++ )                               // Recognizing a string:
                switch(oID)
                {
                    case OPTION_HELP:     ShowHelp();  return(0);                                             // - the case for a "help" option;
                    case OPTION_VERSION:  ShowVersion();  return(0);                                          // - the case for a "version" option;
                    default:                                                                                  // - the case for a default scenario:
                        Options->oFlags|= oID;                                                                //
                       *(pID + GetOptionIndex(oID)) = oCurrent;                                               //
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
        unsigned int oFlags = (Options->oFlags);

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
            case 0:              oFlags = (oFlags|OPTION_SERVER);

            case OPTION_SERVER:  NewConnection->AddrInfo.ai_flags|= AI_PASSIVE;                               // - the case for a "server" option;
                                 if( !(oFlags & OPTION_QUIET) )  puts("\r\n Creating a server: ");
                                 Options->rHost = GetOptionVar(OPTION_SERVER);
                                 break;

            case OPTION_CLIENT:  if( !(oFlags & OPTION_QUIET) )  puts("\r\n Creating a client: ");          // - the case for a "client" option;
                                 Options->rHost = GetOptionVar(OPTION_CLIENT);
                                 break;

            default:             puts("\r\n Error: ambigous options. Can`t create client and server at the same time. \r\n");
                                 return(OPTION_NULL);                                                         // - detecting errors.
        };
        Options->oFlags = oFlags;

        if( oFlags & OPTION_PORT )  Options->rPort = GetOptionVar(OPTION_PORT);                               //
        if( !(Options->rPort) )     Options->rPort = "45678\0";                                               // Default port.

        return(oFlags);
    };
return(OPTION_NULL); }

/**************************************************************************************************************************
 * ===================================== *** Main_SetupConnection() function *** ======================================== *
 **************************************************************************************************************************/

size_t Main_SetupNewConnections(struct addrinfo *Handle, struct NETWORK_DATA *Net, unsigned int oFlags){

    size_t nConnections = 0;

    if( (Handle)&&(Net) )
    {
        bool fServer = (oFlags & OPTION_SERVER);
        bool fQuiet  = (oFlags & OPTION_QUIET);

        do
        {   struct CONNECT_INFO *NewConnection = (Net->iConnection[nConnections]);

            if( !NewConnection )
                if( NewConnection = (struct CONNECT_INFO*)malloc(sizeof(struct CONNECT_INFO)) )
                    Net->iConnection[nConnections] = NewConnection;

            if( NewConnection )  memset(NewConnection,0,sizeof(struct CONNECT_INFO));

            if( !fQuiet )
            {    if(fServer)  printf("    + Configuring connection: ");  else
                              printf("    + Connecting to: ");
            };
            if( NetworkConfigureNext(Handle,NewConnection) )
            {
                if( !fQuiet )
                {   printf(" [ %s @%s ] ",&(NewConnection->HostInfo.HostNum[0]),
                                          &(NewConnection->HostInfo.PortNum[0]));
                    fflush(stdout);
                };
                switch( NetworkConfigureSocket(NewConnection,fServer) )
                {
                    case CONNECTION_ACTIVE:    Net->nActive++;
                    case CONNECTION_LISTENER:  Net->nConnections = (nConnections++);
                    default:                   break;
            };  };
            if( !fQuiet )  printf("- (%d) %s.\r\n",(NewConnection->Socket.ErrCode),
                                                  &(NewConnection->Socket.ErrMsg[0]));
            if( (!fServer)&&(nConnections) )  break;

        }while( (Handle=Handle->ai_next)&&(nConnections<NETWORK_MAX_CONN) );
    };
return(nConnections); };

/**************************************************************************************************************************
 * ==================================== *** MainQueue_Registering() function *** ======================================== *
 **************************************************************************************************************************/

bool MainQueue_Registering(int Handle, struct CONNECT_INFO *NewConnection, unsigned int oFlags){

    bool Result = false;
    struct epoll_event Event;                                                                //

    if( NewConnection )
    {
        Event.data.ptr = NewConnection;
        if( oFlags & OPTION_SERVER )  Event.events = EPOLLIN;  else
                                      Event.events = EPOLLOUT;

        if( !epoll_ctl(Handle,EPOLL_CTL_ADD,(NewConnection->Socket.Handle),&Event) )
        {
            NewConnection->Status|= CONNECTION_REGISTERED;  Result = true;
        };
        strerror_r((NewConnection->Socket.ErrCode=errno),&(NewConnection->Socket.ErrMsg[0]),APP_NAME_MAX);
    };
return(Result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

