/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: network.c (version 0.9).                                                                                     *
 *     Type:                                                                                                              *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: network.h, base.h.                                                                                     *
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

#include "network.h"

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/


/**************************************************************************************************************************
 * ======================================= *** NetworkConfigureInit() Function *** ====================================== *
 **************************************************************************************************************************/

struct addrinfo* NetworkConfigureInit(const char *Host, const char *Port, struct CONNECT_INFO *iConn){

    static struct addrinfo *Handle = NULL;

    if( iConn )
    {   int gaiResult = getaddrinfo(Host,Port,&(iConn->AddrInfo),&Handle);
        if( gaiResult )
        {
            strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(iConn->Socket.ErrCode=gaiResult),APP_NAME_MAX);
            Handle = NULL;
    };  };
return(Handle); }

/**************************************************************************************************************************
 * ======================================= *** NetworkConfigureInit() Function *** ====================================== *
 **************************************************************************************************************************/

size_t NetworkConfigureInitAll(const char *Host, const char *Port, const struct addrinfo AddrInfo*, struct NETWORK_DATA *Net){

    size_t nConnections = 0;

    if( (AddrInfo)&&(Net) )
    {
        struct addrinfo *Handle = NULL;

        int gaiResult = getaddrinfo(Host,Port,AddrInfo,&Handle);
        if( gaiResult )
            strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(iConn->Socket.ErrCode=gaiResult),APP_NAME_MAX);

        for(size_t n=0; (Handle)&&(n<NETWORK_MAX_CONN); n++)
        {
            struct CONNECT_INFO *NewConnection = (Net->iConnection[n]);

            if( !NewConnection )
                if( NewConnection = (struct CONNECT_INFO*)malloc(sizeof(struct CONNECT_INFO)) )
                    Net->iConnection[n] = NewConnection;

            if( NewConnection )  memset(NewConnection,0,sizeof(struct CONNECT_INFO));

            if( NetworkConfigureNext(Handle,NewConnection) )  nConnections++;

            Handle = (Handle->ai_next);
        };
    };
return(nConnections); }

/**************************************************************************************************************************
 * ====================================== *** NetworkConfigureNext() Function *** ======================================= *
 **************************************************************************************************************************/

bool NetworkConfigureNext(struct addrinfo *Handle, struct CONNECT_INFO *iConn){

#   define     nFlagsMax            (2)
    const int  Flags[nFlagsMax] = { (NI_NUMERICHOST|NI_NUMERICSERV),(NI_NAMEREQD) };

    int     Result = 0;
    int  gaiResult = 0;

    if( (Handle)&&(iConn) )
    {
        struct HOST_INFO *Host  = &(iConn->HostInfo);

        do
        {   if( gaiResult = getnameinfo((Handle->ai_addr),(Handle->ai_addrlen),&(Host->HostName[0]),APP_NAME_MAX,&(Host->PortName[0]),NI_MAXSERV,Flags[Result]) )
            {
                strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(iConn->Socket.ErrCode=gaiResult),APP_NAME_MAX);  break;
            }
            if( !Result )
            {
                strcpy(&(Host->HostNum[0]),&(Host->HostName[0]));
                strcpy(&(Host->PortNum[0]),&(Host->PortName[0]));
            };
        }while( (++Result) < nFlagsMax );

        if( Result )
        {
            struct protoent *ProtocolName = getprotobynumber(Handle->ai_protocol);

            if( ProtocolName )
            {
                strcpy(&(Host->Protocol[0]),(ProtocolName->p_name));
                endprotoent();
            };
            if( (void*)(Handle) != (void*)&(iConn->AddrInfo) )
                memcpy(&(iConn->AddrInfo),Handle,sizeof(struct addrinfo));

            if( (void*)(Handle->ai_addr) != (void*)&(iConn->AddrSpace.Raw) )
                memcpy(&(iConn->AddrSpace.Raw),(Handle->ai_addr),(Handle->ai_addrlen));

            iConn->AddrInfo.ai_addr = &(iConn->AddrSpace.Raw);
            iConn->AddrInfo.ai_next = NULL;
    };  };
#   undef     nFlagsMax

return((bool)Result); }

/**************************************************************************************************************************
 * ===================================== *** NetworkConfigureSocket() Function *** ====================================== *
 **************************************************************************************************************************/

int NetworkConfigureSocket(struct CONNECT_INFO *iConn, bool fServer){

    int Result = CONNECTION_NULL;

    if( iConn )
    {   struct addrinfo *Handle = &(iConn->AddrInfo);

        int hSock = socket((Handle->ai_family),(Handle->ai_socktype),(Handle->ai_protocol));
        if( hSock >= 0 )
            switch(fServer)
            {
                case false:
                    if( connect(hSock,(Handle->ai_addr),(Handle->ai_addrlen)) )  break;
                    iConn->Status = CONNECTION_ACTIVE;
                    break;

                default:
                    if( bind(hSock,(Handle->ai_addr),(Handle->ai_addrlen)) )  break;
                    if( listen(hSock,0) )  break;
                    iConn->Status = CONNECTION_LISTENER;
                    break;
            };
        strerror_r((iConn->Socket.ErrCode=errno),&(iConn->Socket.ErrMsg[0]),APP_NAME_MAX);

        if( (iConn->Status) )
        {
            iConn->Socket.Flags  = fcntl(hSock,F_GETFD);
            iConn->Socket.Status = fcntl(hSock,F_GETFL);
            iConn->Socket.Start  = time(NULL);
        };
        iConn->Socket.Handle = hSock;
        Result               = (iConn->Status);
    };
return(Result); }

/**************************************************************************************************************************
 * ===================================== *** NetworkConfigureAccept() Function *** ====================================== *
 **************************************************************************************************************************/

bool NetworkConfigureAccept(struct CONNECT_INFO *iConnOriginal, struct CONNECT_INFO *iConnNew){

    bool Result = false;

    if( (iConnOriginal)&&(iConnNew) )
    {
        struct addrinfo *Handle = &(iConnNew->AddrInfo);

        memcpy(Handle,&(iConnOriginal->AddrInfo),sizeof(struct addrinfo));

        Handle->ai_addr    = &(iConnNew->AddrSpace.Raw);
        Handle->ai_addrlen = sizeof(union SOCKET_ADDR);

        int hSock = accept((iConnOriginal->Socket.Handle),(Handle->ai_addr),&(Handle->ai_addrlen));
        if( hSock >= 0 )
        {
            Result = true;
            NetworkConfigureNext(Handle,iConnNew);

            iConnNew->Status        = CONNECTION_ACTIVE;
            iConnNew->Socket.Flags  = fcntl(hSock,F_GETFD);
            iConnNew->Socket.Status = fcntl(hSock,F_GETFL);
            iConnNew->Socket.Start  = time(NULL);
        };
        iConnNew->Socket.Handle = hSock;
        strerror_r((iConnNew->Socket.ErrCode=errno),&(iConnNew->Socket.ErrMsg[0]),APP_NAME_MAX);
    };
return(Result); }

/**************************************************************************************************************************
 * ==================================== *** NetworkConfigureRegister() Function *** ===================================== *
 **************************************************************************************************************************/

bool NetworkConfigureRegister(int Handle, struct CONNECT_INFO *NewConnection, bool fServer){

    bool Result = false;

    if( NewConnection )
    {
        struct epoll_event Event = {   .data.ptr = NewConnection, /* */
                                       .events   = EPOLLOUT       /* */ };

        if( fServer )  Event.events = EPOLLIN;

        if( epoll_ctl(Handle,EPOLL_CTL_ADD,(NewConnection->Socket.Handle),&Event) )
            strerror_r((NewConnection->Socket.ErrCode=errno),&(NewConnection->Socket.ErrMsg[0]),APP_NAME_MAX);  else
        {
            NewConnection->Status|= CONNECTION_REGISTERED;  Result = true;
    };  };
return(Result); }

/**************************************************************************************************************************
 * ==================================== *** NetworkConfigureRegister() Function *** ===================================== *
 **************************************************************************************************************************/

bool NetworkConfigureUnregister(int Handle, struct CONNECT_INFO *OldConnection){

    bool Result = false;

    if( OldConnection )
    {
        struct epoll_event Event = {   .data.ptr = OldConnection, /* */
                                       .events   = EPOLLERR       /* */ };

        if( epoll_ctl(Handle,EPOLL_CTL_DEL,(OldConnection->Socket.Handle),&Event) )
            strerror_r((OldConnection->Socket.ErrCode=errno),&(OldConnection->Socket.ErrMsg[0]),APP_NAME_MAX);  else
        {
            OldConnection->Status&= CONNECTION_REGISTERMASK;  Result = true;
    };  };
return(Result); }

/**************************************************************************************************************************
 * ====================================== *** NetworkConfigureClose() Function *** ====================================== *
 **************************************************************************************************************************/

bool NetworkConfigureClose(int EpollHandle, struct CONNECT_INFO *OldConnection){

    bool Result = false;

    if( OldConnection )
    {
        if( (OldConnection->Status) & CONNECTION_REGISTERED )
            NetworkConfigureUnregister(EpollHandle,OldConnection);

        int Handle = (OldConnection->Socket.Handle);

        if( Handle >= 0 )
        {
            close(Handle);
            strerror_r((OldConnection->Socket.ErrCode=errno),&(OldConnection->Socket.ErrMsg[0]),APP_NAME_MAX);
            OldConnection->Socket.Handle = Handle = -1;
        };
        Result = true;
    };
return(Result); }

/**************************************************************************************************************************
 * ===================================== *** NetworkConfigureDestroy() Function *** ===================================== *
 **************************************************************************************************************************/

bool NetworkConfigureDestroy(int EpollHandle, struct CONNECT_INFO *OldConnection){

    bool Result = false;

    if( OldConnection )
    {
        if( (OldConnection->Status) & CONNECTION_REGISTERED )
            NetworkConfigureUnregister(EpollHandle,OldConnection);

        int Handle = (OldConnection->Socket.Handle);

        if( Handle >= 0 )
        {
            const struct linger Option = {   .l_onoff  = 0, /* blocking bit */                                // This structure is used for SO_LINGER option.
                                             .l_linger = 1  /* timeout period, in seconds */ };

            setsockopt(Handle,SOL_SOCKET,SO_LINGER,&Option,sizeof(struct linger));                            // This function is used to set the socket option.
            close(Handle);

            memset(OldConnection,0,sizeof(struct CONNECT_INFO));
            free(OldConnection);
        };
        Result = true;
    };
return(Result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

