/*
 * FreeModbus Libary: Win32 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttcp.c,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

/*
 * Design Notes:
 *
 * The xMBPortTCPInit function allocates a socket and binds the socket to
 * all available interfaces ( bind with INADDR_ANY ). In addition it
 * creates an array of event objects which is used to check the state of
 * the clients. On event object is used to handle new connections or
 * closed ones. The other objects are used on a per client basis for
 * processing.
 */

 /**********************************************************
 *	Linux TCP support.
 *	Based on Walter's project.
 *	Modified by Steven Guo <gotop167@163.com>
 ***********************************************************/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <assert.h>

#include "port.h"
#include "portinstance.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- MBAP Header --------------------------------------*/
#define MB_TCP_UID          6
#define MB_TCP_LEN          4
#define MB_TCP_FUNC         7

/* ----------------------- Defines  -----------------------------------------*/
#define MB_TCP_DEFAULT_PORT 502 /* TCP listening port. */
#define MB_TCP_POOL_TIMEOUT 50  /* pool timeout for event waiting. */
#define MB_TCP_READ_TIMEOUT 1000        /* Maximum timeout to wait for packets. */
#define MB_TCP_READ_CYCLE   100 /* Time between checking for new data. */

#define MB_TCP_DEBUG        1   /* Set to 1 for additional debug output. */

#define EV_CONNECTION       0
#define EV_CLIENT           1
#define EV_NEVENTS          EV_CLIENT + 1

/* ----------------------- External functions -------------------------------*/
int8_t           *WsaError2String( int dwError );

/* ----------------------- Static functions ---------------------------------*/
bool            prvMBTCPPortAddressToString( SOCKET xSocket, int8_t * szAddr, uint16_t usBufSize );
int8_t           *prvMBTCPPortFrameToString( uint8_t * pucFrame, uint16_t usFrameLen );
static bool     prvbMBPortAcceptClient( struct xMBInstance * xInstance );
static void     prvvMBPortReleaseClient( struct xMBInstance * xInstance );


/* ----------------------- Begin implementation -----------------------------*/

bool
xMBTCPPortInit( struct xMBInstance * xInstance, uint16_t usTCPPort )
{
    uint16_t          usPort;
    struct sockaddr_in serveraddr;

    if( usTCPPort == 0 )
    {
        usPort = MB_TCP_DEFAULT_PORT;
    }
    else
    {
        usPort = ( uint16_t ) usTCPPort;
    }
    memset( &serveraddr, 0, sizeof( serveraddr ) );
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl( INADDR_ANY );
    serveraddr.sin_port = htons( usPort );
    if( ( ( ( struct PortInstance* )xInstance )->xListenSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) == -1 )
    {
        fprintf( stderr, "Create socket failed.\r\n" );
        return false;
    }
    else if( bind( ( ( struct PortInstance* )xInstance )->xListenSocket, ( struct sockaddr * )&serveraddr, sizeof( serveraddr ) ) == -1 )
    {
        fprintf( stderr, "Bind socket failed.\r\n" );
        return false;
    }
    else if( listen( ( ( struct PortInstance* )xInstance )->xListenSocket, 5 ) == -1 )
    {
        fprintf( stderr, "Listen socket failed.\r\n" );
        return false;
    }
    FD_ZERO( &( ( struct PortInstance* )xInstance )->allset );
    FD_SET( ( ( struct PortInstance* )xInstance )->xListenSocket, &( ( struct PortInstance* )xInstance )->allset );
    return true;
}

void
vMBTCPPortClose( struct xMBInstance * xInstance )
{
    // Close all client sockets.
    if( ( ( struct PortInstance* )xInstance )->xClientSocket != SOCKET_ERROR )
    {
        prvvMBPortReleaseClient( xInstance );
    }
    // Close the listener socket.
    if( ( ( struct PortInstance* )xInstance )->xListenSocket != SOCKET_ERROR )
    {
        close( ( ( struct PortInstance* )xInstance )->xListenSocket );
    }
}

void
vMBTCPPortDisable( struct xMBInstance * xInstance )
{
    /* Close all client sockets. */
    if( ( ( struct PortInstance* )xInstance )->xClientSocket != SOCKET_ERROR )
    {
        prvvMBPortReleaseClient( xInstance );
    }
}

/*! \ingroup port_win32tcp
 *
 * \brief Pool the listening socket and currently connected Modbus TCP clients
 *   for new events.
 * \internal
 *
 * This function checks if new clients want to connect or if already connected
 * clients are sending requests. If a new client is connected and there are
 * still client slots left (The current implementation supports only one)
 * then the connection is accepted and an event object for the new client
 * socket is activated (See prvbMBPortAcceptClient() ).
 * Events for already existing clients in \c FD_READ and \c FD_CLOSE. In case of
 * an \c FD_CLOSE the client connection is released (See prvvMBPortReleaseClient() ).
 * In case of an \c FD_READ command the existing data is read from the client
 * and if a complete frame has been received the Modbus Stack is notified.
 *
 * \return false in case of an internal I/O error. For example if the internal
 *   event objects are in an invalid state. Note that this does not include any
 *   client errors. In all other cases returns true.
 */
bool
xMBPortTCPPoll( struct xMBInstance * xInstance )
{
    int             n;
    fd_set          fread;
    struct timeval  tval;

    tval.tv_sec = 0;
    tval.tv_usec = 5000;
    int             ret;
    uint16_t          usLength;

    if( ( ( struct PortInstance* )xInstance )->xClientSocket == INVALID_SOCKET )
    {
        /* Accept to client */
        if( ( n = select( ( ( struct PortInstance* )xInstance )->xListenSocket + 1, &( ( struct PortInstance* )xInstance )->allset, NULL, NULL, NULL ) ) < 0 )
        {
            if( errno == EINTR )
            {
                ;
            }
            else
            {
                ;
            }
        }
        if( FD_ISSET( ( ( struct PortInstance* )xInstance )->xListenSocket, &( ( struct PortInstance* )xInstance )->allset ) )
        {
            ( void )prvbMBPortAcceptClient( xInstance );
        }
    }
    while( true )
    {
        FD_ZERO( &fread );
        FD_SET( ( ( struct PortInstance* )xInstance )->xClientSocket, &fread );
        if( ( ( ret = select( ( ( struct PortInstance* )xInstance )->xClientSocket + 1, &fread, NULL, NULL, &tval ) ) == SOCKET_ERROR )
            || !ret )
        {
            continue;
        }
        if( ret > 0 )
        {
            if( FD_ISSET( ( ( struct PortInstance* )xInstance )->xClientSocket, &fread ) )
            {
                if( ( ( ret =
                        recv( ( ( struct PortInstance* )xInstance )->xClientSocket, &( ( struct PortInstance* )xInstance )->aucTCPBuf[( ( struct PortInstance* )xInstance )->usTCPBufPos], ( ( struct PortInstance* )xInstance )->usTCPFrameBytesLeft,
                              0 ) ) == SOCKET_ERROR ) || ( !ret ) )
                {
                    close( ( ( struct PortInstance* )xInstance )->xClientSocket );
                    ( ( struct PortInstance* )xInstance )->xClientSocket = INVALID_SOCKET;
                    return true;
                }
                ( ( struct PortInstance* )xInstance )->usTCPBufPos += ret;
                ( ( struct PortInstance* )xInstance )->usTCPFrameBytesLeft -= ret;
                if( ( ( struct PortInstance* )xInstance )->usTCPBufPos >= MB_TCP_FUNC )
                {
                    /* Length is a byte count of Modbus PDU (function code + data) and the
                     * unit identifier. */
                    usLength = ( ( struct PortInstance* )xInstance )->aucTCPBuf[MB_TCP_LEN] << 8U;
                    usLength |= ( ( struct PortInstance* )xInstance )->aucTCPBuf[MB_TCP_LEN + 1];

                    /* Is the frame already complete. */
                    if( ( ( struct PortInstance* )xInstance )->usTCPBufPos < ( MB_TCP_UID + usLength ) )
                    {
                        ( ( struct PortInstance* )xInstance )->usTCPFrameBytesLeft = usLength + MB_TCP_UID - ( ( struct PortInstance* )xInstance )->usTCPBufPos;
                    }
                    /* The frame is complete. */
                    else if( ( ( struct PortInstance* )xInstance )->usTCPBufPos == ( MB_TCP_UID + usLength ) )
                    {
                        ( void )xMBPortEventPost( xInstance, EV_FRAME_RECEIVED );
                        return true;
                    }
                    /* This can not happend because we always calculate the number of bytes
                     * to receive. */
                    else
                    {
                        assert( ( ( struct PortInstance* )xInstance )->usTCPBufPos <= ( MB_TCP_UID + usLength ) );
                    }
                }
            }
        }
    }
    return true;
}

/*!
 * \ingroup port_win32tcp
 * \brief Receives parts of a Modbus TCP frame and if complete notifies
 *    the protocol stack.
 * \internal
 *
 * This function reads a complete Modbus TCP frame from the protocol stack.
 * It starts by reading the header with an initial request size for
 * usTCPFrameBytesLeft = MB_TCP_FUNC. If the header is complete the
 * number of bytes left can be calculated from it (See Length in MBAP header).
 * Further read calls are issued until the frame is complete.
 *
 * \return \c true if part of a Modbus TCP frame could be processed. In case
 *   of a communication error the function returns \c false.
 */

bool
xMBTCPPortGetRequest( struct xMBInstance * xInstance, uint8_t ** ppucMBTCPFrame, uint16_t * usTCPLength )
{
    *ppucMBTCPFrame = &( ( struct PortInstance* )xInstance )->aucTCPBuf[0];
    *usTCPLength = ( ( struct PortInstance* )xInstance )->usTCPBufPos;

    /* Reset the buffer. */
    ( ( struct PortInstance* )xInstance )->usTCPBufPos = 0;
    ( ( struct PortInstance* )xInstance )->usTCPFrameBytesLeft = MB_TCP_FUNC;
    return true;
}

bool
xMBTCPPortSendResponse( struct xMBInstance * xInstance, const uint8_t * pucMBTCPFrame, uint16_t usTCPLength )
{
    bool            bFrameSent = false;
    bool            bAbort = false;
    int             res;
    int             iBytesSent = 0;
    int             iTimeOut = MB_TCP_READ_TIMEOUT;

    do
    {
        res = send( ( ( struct PortInstance* )xInstance )->xClientSocket, &pucMBTCPFrame[iBytesSent], usTCPLength - iBytesSent, 0 );
        switch ( res )
        {
        case -1:
            if( iTimeOut > 0 )
            {
                iTimeOut -= MB_TCP_READ_CYCLE;
                usleep( MB_TCP_READ_CYCLE );
            }
            else
            {
                bAbort = true;
            }
            break;
        case 0:
            prvvMBPortReleaseClient( xInstance );
            bAbort = true;
            break;
        default:
            iBytesSent += res;
            break;
        }
    }
    while( ( iBytesSent != usTCPLength ) && !bAbort );

    bFrameSent = iBytesSent == usTCPLength ? true : false;

    return bFrameSent;
}

void
prvvMBPortReleaseClient( struct xMBInstance * xInstance )
{
    ( void )recv( ( ( struct PortInstance* )xInstance )->xClientSocket, &( ( struct PortInstance* )xInstance )->aucTCPBuf[0], MB_TCP_BUF_SIZE, 0 );

    ( void )close( ( ( struct PortInstance* )xInstance )->xClientSocket );
    ( ( struct PortInstance* )xInstance )->xClientSocket = INVALID_SOCKET;
}

bool
prvbMBPortAcceptClient( struct xMBInstance * xInstance )
{
    SOCKET          xNewSocket;
    bool            bOkay;

    /* Check if we can handle a new connection. */

    if( ( ( struct PortInstance* )xInstance )->xClientSocket != INVALID_SOCKET )
    {
        fprintf( stderr, "can't accept new client. all connections in use.\n" );
        bOkay = false;
    }
    else if( ( xNewSocket = accept( ( ( struct PortInstance* )xInstance )->xListenSocket, NULL, NULL ) ) == INVALID_SOCKET )
    {
        bOkay = false;
    }
    else
    {
        ( ( struct PortInstance* )xInstance )->xClientSocket = xNewSocket;
        ( ( struct PortInstance* )xInstance )->usTCPBufPos = 0;
        ( ( struct PortInstance* )xInstance )->usTCPFrameBytesLeft = MB_TCP_FUNC;
        bOkay = true;
    }
    return bOkay;
}
