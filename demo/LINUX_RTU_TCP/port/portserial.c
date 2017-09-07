/*
 * FreeModbus Libary: Linux Port
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
 * File: $Id: portserial.c,v 1.3 2006/10/12 08:35:34 wolti Exp $
 */

/* ----------------------- Standard includes --------------------------------*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <unistd.h>

#include "port.h"
#include "portinstance.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"

/* ----------------------- Function prototypes ------------------------------*/
static bool     prvbMBPortSerialRead( struct xMBInstance * xInstance, uint8_t * pucBuffer, uint16_t usNBytes, uint16_t * usNBytesRead );
static bool     prvbMBPortSerialWrite( struct xMBInstance * xInstance, uint8_t * pucBuffer, uint16_t usNBytes );

/* ----------------------- Begin implementation -----------------------------*/
void
vMBPortSerialEnable( struct xMBInstance * xInstance, bool bEnableRx, bool bEnableTx )
{
    /* it is not allowed that both receiver and transmitter are enabled. */
    assert( !bEnableRx || !bEnableTx );

    if( bEnableRx )
    {
        ( void )tcflush( ( ( struct PortInstance* )xInstance )->iSerialFd, TCIFLUSH );
        ( ( struct PortInstance* )xInstance )->uiRxBufferPos = 0;
        ( ( struct PortInstance* )xInstance )->bRxEnabled = true;
    }
    else
    {
        ( ( struct PortInstance* )xInstance )->bRxEnabled = false;
    }
    if( bEnableTx )
    {
        ( ( struct PortInstance* )xInstance )->bTxEnabled = true;
        ( ( struct PortInstance* )xInstance )->uiTxBufferPos = 0;
    }
    else
    {
        ( ( struct PortInstance* )xInstance )->bTxEnabled = false;
    }
}

bool
xMBPortSerialInit( struct xMBInstance * xInstance, uint8_t ucPort, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity )
{
    char            szDevice[16];
    bool            bStatus = true;

    struct termios  xNewTIO;
    speed_t         xNewSpeed;

    snprintf( szDevice, 16, "/dev/pts/%d", ucPort );

    if( ( ( ( struct PortInstance* )xInstance )->iSerialFd = open( szDevice, O_RDWR | O_NOCTTY ) ) < 0 )
    {
        vMBPortLog( xInstance, MB_LOG_ERROR, "SER-INIT", "Can't open serial port %s: %s\n", szDevice,
                    strerror( errno ) );
    }
    else if( tcgetattr( ( ( struct PortInstance* )xInstance )->iSerialFd, &( ( struct PortInstance* )xInstance )->xOldTIO ) != 0 )
    {
        vMBPortLog( xInstance, MB_LOG_ERROR, "SER-INIT", "Can't get settings from port %s: %s\n", szDevice,
                    strerror( errno ) );
    }
    else
    {
        bzero( &xNewTIO, sizeof( struct termios ) );

        xNewTIO.c_iflag |= IGNBRK | INPCK;
        xNewTIO.c_cflag |= CREAD | CLOCAL;
        switch ( eParity )
        {
        case MB_PAR_NONE:
            break;
        case MB_PAR_EVEN:
            xNewTIO.c_cflag |= PARENB;
            break;
        case MB_PAR_ODD:
            xNewTIO.c_cflag |= PARENB | PARODD;
            break;
        default:
            bStatus = false;
        }
        switch ( ucDataBits )
        {
        case 8:
            xNewTIO.c_cflag |= CS8;
            break;
        case 7:
            xNewTIO.c_cflag |= CS7;
            break;
        default:
            bStatus = false;
        }
        switch ( ulBaudRate )
        {
        case 9600:
            xNewSpeed = B9600;
            break;
        case 19200:
            xNewSpeed = B19200;
            break;
        case 38400:
            xNewSpeed = B38400;
            break;
        case 57600:
            xNewSpeed = B57600;
            break;
        case 115200:
            xNewSpeed = B115200;
            break;
        default:
            bStatus = false;
        }
        if( bStatus )
        {
            if( cfsetispeed( &xNewTIO, xNewSpeed ) != 0 )
            {
                vMBPortLog( xInstance, MB_LOG_ERROR, "SER-INIT", "Can't set baud rate %ld for port %s: %s\n",
                            ulBaudRate, strerror( errno ) );
            }
            else if( cfsetospeed( &xNewTIO, xNewSpeed ) != 0 )
            {
                vMBPortLog( xInstance, MB_LOG_ERROR, "SER-INIT", "Can't set baud rate %ld for port %s: %s\n",
                            ulBaudRate, szDevice, strerror( errno ) );
            }
            else if( tcsetattr( ( ( struct PortInstance* )xInstance )->iSerialFd, TCSANOW, &xNewTIO ) != 0 )
            {
                vMBPortLog( xInstance, MB_LOG_ERROR, "SER-INIT", "Can't set settings for port %s: %s\n",
                            szDevice, strerror( errno ) );
            }
            else
            {
                vMBPortSerialEnable( xInstance, false, false );
                bStatus = true;
            }
        }
    }
    return bStatus;
}

bool
xMBPortSerialSetTimeout( struct xMBInstance * xInstance, uint32_t ulNewTimeoutMs )
{
    if( ulNewTimeoutMs > 0 )
    {
        ( ( struct PortInstance* )xInstance )->ulTimeoutMs = ulNewTimeoutMs;
    }
    else
    {
        ( ( struct PortInstance* )xInstance )->ulTimeoutMs = 1;
    }
    return true;
}

void
vMBPortClose( struct xMBInstance * xInstance )
{
    if( ( ( struct PortInstance* )xInstance )->iSerialFd != -1 )
    {
        ( void )tcsetattr( ( ( struct PortInstance* )xInstance )->iSerialFd, TCSANOW, &( ( struct PortInstance* )xInstance )->xOldTIO );
        ( void )close( ( ( struct PortInstance* )xInstance )->iSerialFd );
        ( ( struct PortInstance* )xInstance )->iSerialFd = -1;
    }
}

bool
prvbMBPortSerialRead( struct xMBInstance * xInstance, uint8_t * pucBuffer, uint16_t usNBytes, uint16_t * usNBytesRead )
{
    bool            bResult = true;
    ssize_t         res;
    fd_set          rfds;
    struct timeval  tv;

    tv.tv_sec = 0;
    tv.tv_usec = 50000;
    FD_ZERO( &rfds );
    FD_SET( ( ( struct PortInstance* )xInstance )->iSerialFd, &rfds );

    /* Wait until character received or timeout. Recover in case of an
     * interrupted read system call. */
    do
    {
        if( select( ( ( struct PortInstance* )xInstance )->iSerialFd + 1, &rfds, NULL, NULL, &tv ) == -1 )
        {
            if( errno != EINTR )
            {
                bResult = false;
            }
        }
        else if( FD_ISSET( ( ( struct PortInstance* )xInstance )->iSerialFd, &rfds ) )
        {
            if( ( res = read( ( ( struct PortInstance* )xInstance )->iSerialFd, pucBuffer, usNBytes ) ) == -1 )
            {
                bResult = false;
            }
            else
            {
                *usNBytesRead = ( uint16_t ) res;
                break;
            }
        }
        else
        {
            *usNBytesRead = 0;
            break;
        }
    }
    while( bResult == true );
    return bResult;
}

bool
prvbMBPortSerialWrite( struct xMBInstance * xInstance, uint8_t * pucBuffer, uint16_t usNBytes )
{
    ssize_t         res;
    size_t          left = ( size_t ) usNBytes;
    size_t          done = 0;

    while( left > 0 )
    {
        if( ( res = write( ( ( struct PortInstance* )xInstance )->iSerialFd, pucBuffer + done, left ) ) == -1 )
        {
            if( errno != EINTR )
            {
                break;
            }
            /* call write again because of interrupted system call. */
            continue;
        }
        done += res;
        left -= res;
    }
    return left == 0 ? true : false;
}

bool
xMBPortSerialPoll( struct xMBInstance * xInstance )
{
    bool            bStatus = true;
    uint16_t          usBytesRead;
    int             i;

    while( ( ( struct PortInstance* )xInstance )->bRxEnabled )
    {
        if( prvbMBPortSerialRead( xInstance, &( ( struct PortInstance* )xInstance )->ucBuffer[0], BUF_SIZE, &usBytesRead ) )
        {
            if( usBytesRead == 0 )
            {
                /* timeout with no bytes. */
                break;
            }
            else if( usBytesRead > 0 )
            {
                for( i = 0; i < usBytesRead; i++ )
                {
                    /* Call the modbus stack and let him fill the buffers. */
                    ( void )xInstance->pxMBFrameCBByteReceived( ( struct xMBInstance* )xInstance );
                }
                ( ( struct PortInstance* )xInstance )->uiRxBufferPos = 0;
            }
        }
        else
        {
            vMBPortLog( xInstance, MB_LOG_ERROR, "SER-POLL", "read failed on serial device: %s\n",
                        strerror( errno ) );
            bStatus = false;
        }
    }
    if( ( ( struct PortInstance* )xInstance )->bTxEnabled )
    {
        while( ( ( struct PortInstance* )xInstance )->bTxEnabled )
        {
            ( void )xInstance->pxMBFrameCBTransmitterEmpty( ( struct xMBInstance* )xInstance );
            /* Call the modbus stack to let him fill the buffer. */
        }
        if( !prvbMBPortSerialWrite( xInstance, &( ( struct PortInstance* )xInstance )->ucBuffer[0], ( ( struct PortInstance* )xInstance )->uiTxBufferPos ) )
        {
            vMBPortLog( xInstance, MB_LOG_ERROR, "SER-POLL", "write failed on serial device: %s\n",
                        strerror( errno ) );
            bStatus = false;
        }
    }

    return bStatus;
}

bool
xMBPortSerialPutByte( struct xMBInstance * xInstance, uint8_t ucByte )
{
    assert( ( ( struct PortInstance* )xInstance )->uiTxBufferPos < BUF_SIZE );
    ( ( struct PortInstance* )xInstance )->ucBuffer[( ( struct PortInstance* )xInstance )->uiTxBufferPos] = ucByte;
    ( ( struct PortInstance* )xInstance )->uiTxBufferPos++;
    return true;
}

bool
xMBPortSerialGetByte( struct xMBInstance * xInstance, uint8_t * pucByte )
{
    assert( ( ( struct PortInstance* )xInstance )->uiRxBufferPos < BUF_SIZE );
    *pucByte = ( ( struct PortInstance* )xInstance )->ucBuffer[( ( struct PortInstance* )xInstance )->uiRxBufferPos];
    ( ( struct PortInstance* )xInstance )->uiRxBufferPos++;
    return true;
}
