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
 * File: $Id: port.h,v 1.1 2006/08/01 20:58:49 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

#include <stdbool.h>
#include <stdint.h>

#define INLINE

#ifdef __cplusplus
extern "C" {
#endif

struct xMBInstance;

/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION( xInstance ) vMBPortEnterCritical( xInstance )
#define EXIT_CRITICAL_SECTION( xInstance ) vMBPortExitCritical( xInstance )
#define MB_PORT_HAS_CLOSE   1
/* ----------------------- Type definitions ---------------------------------*/
typedef int     SOCKET;

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (~0)

typedef enum
{
    MB_LOG_ERROR = 0,
    MB_LOG_WARN = 1,
    MB_LOG_INFO = 2,
    MB_LOG_DEBUG = 3
} eMBPortLogLevel;

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPortEnterCritical( struct xMBInstance * xInstance );
void            vMBPortExitCritical( struct xMBInstance * xInstance );
void            vMBPortLog( struct xMBInstance * xInstance, eMBPortLogLevel eLevel, const char * szModule,
                            const char * szFmt, ... );
void            vMBPortTimerPoll( struct xMBInstance * xInstance );
bool            xMBPortSerialPoll( struct xMBInstance * xInstance );
bool            xMBPortTCPPoll( struct xMBInstance * xInstance );
bool            xMBPortSerialSetTimeout( struct xMBInstance * xInstance, uint32_t dwTimeoutMs );

#ifdef __cplusplus
}
#endif
#endif
