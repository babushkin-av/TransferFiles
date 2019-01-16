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

struct addrinfo* NetworkConfigureInit(char *Host, char *Port, struct CONNECT_INFO *iConn){

    static struct addrinfo *resTmp = NULL;

    if( iConn )
    {   int gaiResult = getaddrinfo(Host,Port,&(iConn->AddrInfo),&resTmp);
        if( gaiResult )
        {
            strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(gaiResult),APP_NAME_MAX);
            iConn->Socket.ErrCode = gaiResult;
            resTmp = NULL;
    };  };
return(resTmp); }

/**************************************************************************************************************************
 * ====================================== *** NetworkConfigureNext() Function *** ======================================= *
 **************************************************************************************************************************/

bool NetworkConfigureNext(struct addrinfo *Handle, struct CONNECT_INFO *iConn){

    int gaiResult;
    struct HOST_INFO *WorkingHost = &(iConn->HostInfo);

    if( gaiResult = getnameinfo((Handle->ai_addr),(Handle->ai_addrlen),&(WorkingHost->HostNum[0]),APP_NAME_MAX,&(WorkingHost->PortNum[0]),NI_MAXSERV,NI_NUMERICHOST|NI_NUMERICSERV) )
    {
        strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(gaiResult),APP_NAME_MAX);
        iConn->Socket.ErrCode = gaiResult;
        return(false);
    };
    if( gaiResult = getnameinfo((Handle->ai_addr),(Handle->ai_addrlen),&(WorkingHost->HostName[0]),APP_NAME_MAX,&(WorkingHost->PortName[0]),NI_MAXSERV,NI_NAMEREQD) )
    {
        strcpy(&(WorkingHost->HostName[0]),&(WorkingHost->HostNum[0]));
        strcpy(&(WorkingHost->PortName[0]),&(WorkingHost->PortNum[0]));
    };
    if( (void*)(Handle) != (void*)&(iConn->AddrInfo) )  // Bug fix!
        memcpy(&(iConn->AddrInfo),Handle,sizeof(struct addrinfo));
    memcpy(&(iConn->AddrSpace.Raw),(Handle->ai_addr),(Handle->ai_addrlen));
    iConn->AddrInfo.ai_addr = &(iConn->AddrSpace.Raw);
    iConn->AddrInfo.ai_next = NULL;

return(true); }

/**************************************************************************************************************************
 * ====================================== *** NetworkConfigureNext() Function *** ======================================= *
 **************************************************************************************************************************/

bool NetworkConfigureSocket(struct CONNECT_INFO *iConn, bool fServer){

    bool Result = false;

    if( iConn )
    {   struct addrinfo *Handle = &(iConn->AddrInfo);
        int hSock = socket((Handle->ai_family),(Handle->ai_socktype),(Handle->ai_protocol));

        if( hSock >= 0 )
            switch(fServer)
            {   case 0:   if( !connect(hSock,(Handle->ai_addr),(Handle->ai_addrlen)) )  Result = true;  break;
                default:  if( !bind(hSock,(Handle->ai_addr),(Handle->ai_addrlen)) )  if( !listen(hSock,0) )  Result = true;
            };
        iConn->Socket.ErrCode = errno;
        strncpy(&(iConn->Socket.ErrMsg[0]),strerror(errno),APP_NAME_MAX);

        switch(Result)
        {   case false:  close(hSock);  hSock = -1;  break;
            case true:   iConn->Socket.Flags  = fcntl(hSock,F_GETFD);
                         iConn->Socket.Status = fcntl(hSock,F_GETFL);
        };
        iConn->Socket.Handle = hSock;
    };
return(Result); }

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/

