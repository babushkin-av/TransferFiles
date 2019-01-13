/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: network.c (version 0.8).                                                                                     *
 *     Type:                                                                                                              *
 *     Distribution: source/object code.                                                                                  *
 *     License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: network.h, base.h.                                                                                     *
 *     Desription: .                                                                                                      *
 *                                                                                                                        *
 **************************************************************************************************************************
 *                                                                                                                        *
 *     Copyleft, 2017-2018, <feedback@babushkin.ru.net>, Alexander Babushkin.                                             *
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
 * ========================================= *** NetworkConfigure() Function *** ======================================== *
 **************************************************************************************************************************/

struct addrinfo* NetworkConfigureInit(char *Host, char *Port, struct CONNECT_INFO *iConn){

    static struct addrinfo *resTmp;
    int gaiResult;

    if( gaiResult = getaddrinfo(Host,Port,&(iConn->AddrInfo),&resTmp) )
    {
        strncpy(&(iConn->Socket.ErrMsg[0]),gai_strerror(gaiResult),APP_NAME_MAX);
        iConn->Socket.ErrCode = gaiResult;
        resTmp = NULL;
    };
return(resTmp); }

/**************************************************************************************************************************
 * ========================================== *** NetworkCreate() Function *** ========================================== *
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
    if( gaiResult = getnameinfo((Handle->ai_addr),(Handle->ai_addrlen),&(WorkingHost->HostName[0]),APP_NAME_MAX,&(WorkingHost->PortName[0]),NI_MAXSERV,NI_NAMEREQD|NI_NUMERICSERV) )
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
 * ====================================================================================================================== *
 **************************************************************************************************************************/

