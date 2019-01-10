/**************************************************************************************************************************
 *                                                                                                                        *
 *     File: network.h (version 0.8).                                                                                     *
 *     Type: basic definitions (network.h).                                                                               *
 *     Distribution: source/object code.                                                                                  *
       License: GNU Lesser Public License version 2.1.                                                                    *
 *     Dependency: base.h.                                                                                                *
 *     Desription: basic definitions.                                                                                     *
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
#ifndef NETWORK_H
#define NETWORK_H

/**************************************************************************************************************************
 * =============================================== *** Header Files *** ================================================= *
 **************************************************************************************************************************/

#include "base.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

/**************************************************************************************************************************
 * ============================================== *** Global Variables *** ============================================== *
 **************************************************************************************************************************/

#define NETWORK_MAX_CONN       (4)
#define NETWORK_DATA_BUF       (4096)
#define NETWORK_PORT_NUM       (40678 + IPPORT_USERRESERVED)

union SOCKET_ADDR {
    struct sockaddr_in         IPv4;
    struct sockaddr_in6        IPv6;
    struct sockaddr            Raw;
};

struct SOCKET_INFO {
    int                        Handle;
    time_t                     Start;
    size_t                     BytesSend;
    size_t                     BytesRecv;
    int                        ErrCode;
    char                       ErrMsg[APP_NAME_MAX];
};

struct HOST_INFO {
    char                       HostName[APP_NAME_MAX];
    char                       PortName[NI_MAXSERV];
    char                       HostNum[APP_NAME_MAX];
    char                       PortNum[NI_MAXSERV];
    char                       Protocol[NI_MAXSERV];
};

struct CONNECT_INFO {
    struct SOCKET_INFO         Socket;
    union  SOCKET_ADDR         AddrSpace;
    struct addrinfo            AddrInfo;
    struct HOST_INFO           HostInfo;
};

struct NETWORK_DATA {
    size_t                     nConnections;                         // All connections;
    struct CONNECT_INFO        iConnection[NETWORK_MAX_CONN];
};

/**************************************************************************************************************************
 * ============================================ *** Function Prototypes *** ============================================= *
 **************************************************************************************************************************/

#ifdef __cplusplus
extern "С" {
#endif
/* ---------------------------------------------------------------------------------------------------------------------- */

struct addrinfo* NetworkConfigureInit(char *Host, char *Port, struct CONNECT_INFO *iConn);

/* ---------------------------------------------------------------------------------------------------------------------- */

bool NetworkConfigureNext(struct addrinfo *Handle, struct CONNECT_INFO *iConn);

/* ---------------------------------------------------------------------------------------------------------------------- */
#ifdef __cplusplus
};
#endif

/**************************************************************************************************************************
 * ====================================================================================================================== *
 **************************************************************************************************************************/
#endif // NETWORK_H

