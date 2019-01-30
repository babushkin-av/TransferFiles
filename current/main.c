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
    STATUS_READY,
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
    struct NETWORK_DATA      Network;                          //
} *MainData;

/**************************************************************************************************************************
 * ============================================ *** Function prototypes *** ============================================= *
 **************************************************************************************************************************/

bool SignalHandlerInit(void);
void SignalHandler(int signo);
bool GetSystemInfo(struct utsname *SysInfo);

unsigned int  Main_ParsingCommandLine(struct APP_OPTIONS *Options, char **ArgV);
int           Main_ShowDebugInfo(const struct utsname *SysInfo, const struct APP_CLOCK *Time, const struct APP_OPTIONS *Options);
int           Main_ShowOptionsInfo(unsigned int *oIndexes, const unsigned int oMax);
unsigned int  Main_Configuring(struct CONNECT_INFO *NewConnection, struct APP_OPTIONS *Options);
size_t        Main_SetupNewConnections(struct addrinfo *Handle, struct NETWORK_DATA *Net, const unsigned int oFlags);

int           MainQueue_Create(const bool fDebug);
size_t        MainQueue_RegisterNewConnections(int Handle, struct NETWORK_DATA *Net, const unsigned int oFlags);
bool          MainQueue_ShowReadyMessage(struct APP_CLOCK *Time);

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

    if( !SignalHandlerInit() )                                                                                // Initializating signal handler.
        error(errno,errno," Error! Can`t setup signal handler!  (%d) ",errno);

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
    };
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    /* Main message-loop queue ----------------------------------------------------------------------------- */
    while( (oFlags=MainData->Options.oFlags) < OPTION_LAST )                                                  // <= while(...)
    {
        static struct epoll_event  ePollEvent;                                                                // Epoll events.
        static int                 ePollHandle;                                                               // Epoll instance.
        int                        ePollReady;                                                                // The  number of file descriptors ready for the requested I/O.

        switch(MainData->Status)                                                                              // <= switch(...)
        {
            case STATUS_QUEUEINIT:              /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
                ePollHandle = MainQueue_Create(oFlags & OPTION_DEBUG);                                        // Epoll initialization.
                MainData->Status = STATUS_REGISTER;
                break;

            case STATUS_REGISTER:               /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
                MainQueue_RegisterNewConnections(ePollHandle,&(MainData->Network),oFlags);
                MainData->Status = STATUS_READY;
                break;

            case STATUS_READY   :               /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
                if( !ePollReady )
                    if( !(oFlags & OPTION_QUIET) )
                    {
                        MainQueue_ShowReadyMessage(&(MainData->Time));  break;
                    };
                if( oFlags & OPTION_SERVER )  MainData->Status = STATUS_SERVER_RECEIVE;  else
                                              MainData->Status = STATUS_CLIENT_GREETINGS;
                break;

            case STATUS_SERVER_RECEIVE:

            default:  break;
        };                                                                                                    // <= switch(...)
        if( (ePollReady = epoll_wait(ePollHandle,&ePollEvent,1,1000)) < 0 )  break;                           // <= epoll_wait(...) - waiting for internet streams.
    };                                                                                                        // <= while(...)
    /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

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

bool SignalHandlerInit(void){

    static struct sigaction Action;                                                                           //
    const int  Signals[5] = {   SIGQUIT, SIGHUP, SIGTERM, SIGTSTP, SIGINT   };

#if defined (APP_DEBUG)
    Action.sa_flags   = SA_RESETHAND;
#endif
    Action.sa_handler = (void*)(SignalHandler);
    sigfillset(&(Action.sa_mask));

    for(unsigned int i=0; i<5; i++)
        if( sigaction(Signals[i],&Action,NULL) )  return(false);

return(true); }

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

int Main_ShowDebugInfo(const struct utsname *SysInfo, const struct APP_CLOCK *Time, const struct APP_OPTIONS *Options){

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

int Main_ShowOptionsInfo(unsigned int *oIndexes, const unsigned int oMax){

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

size_t Main_SetupNewConnections(struct addrinfo *Handle, struct NETWORK_DATA *Net, const unsigned int oFlags){

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
 * ======================================= *** MainQueue_Create() function *** ========================================== *
 **************************************************************************************************************************/

int MainQueue_Create(const bool fDebug){

    int Handle = epoll_create((int)true);                                                                     // Epoll initialization.

    if( fDebug )  printf(" \r\n Message queue initialization... ");
    if( Handle >= 0 )  if( fDebug )  puts(" OK ");

return(Handle); }

/**************************************************************************************************************************
 * =============================== *** MainQueue_RegisterNewConnections() function *** ================================== *
 **************************************************************************************************************************/

size_t MainQueue_RegisterNewConnections(int Handle, struct NETWORK_DATA *Net, const unsigned int oFlags){

    size_t nConnections = 0;
    size_t nRegistered  = 0;

    if( Net )
    {
        bool fDebug = (oFlags & OPTION_DEBUG);

        if( fDebug )  puts(" \r\n Registering sockets: ");

        struct CONNECT_INFO *NewConnection;

        while( NewConnection=(Net->iConnection[nConnections]) )
        {
            bool Result = NetworkConfigureRegister(Handle,NewConnection,(oFlags & OPTION_SERVER));

            if( fDebug )  printf("    +% 2u.[%d] - (%d) %s; \r\n",nConnections,(NewConnection->Socket.Handle),
                                                                               (NewConnection->Socket.ErrCode),
                                                                              &(NewConnection->Socket.ErrMsg[0]));
            if( Result )  nRegistered++;  else
            {
                NetworkConfigureClose(Handle,NewConnection);
                free(NewConnection);
                Net->iConnection[nConnections] = NewConnection = NULL;
            };
            nConnections++;
        };
        if( fDebug )  puts(" ");
        Net->nConnections = nConnections;
        Net->nRegistered  = nRegistered;
    };
return(nRegistered); }

/**************************************************************************************************************************
 * ================================== *** MainQueue_ShowReadyMessage() function *** ===================================== *
 **************************************************************************************************************************/

bool MainQueue_ShowReadyMessage(struct APP_CLOCK *Time){

    bool Result = false;

    if( Time )
        if( GetTimeDiff(Time,"%M:%S") )
        {
            printf(" Waiting for connection: %s  \r",&(Time->String));  fflush(stdout);
            Result = true;
        };
return(Result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

