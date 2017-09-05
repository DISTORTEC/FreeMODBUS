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

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#define INLINE

#ifdef __cplusplus
extern "C" {
#endif
/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION( ) vMBPortEnterCritical()
#define EXIT_CRITICAL_SECTION( ) vMBPortExitCritical()
#define MB_PORT_HAS_CLOSE   1
/* ----------------------- Type definitions ---------------------------------*/
    typedef enum
{
    MB_LOG_ERROR = 0,
    MB_LOG_WARN = 1,
    MB_LOG_INFO = 2,
    MB_LOG_DEBUG = 3
} eMBPortLogLevel;

typedef unsigned char UCHAR;
typedef char    CHAR;
typedef unsigned short USHORT;

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPortEnterCritical( void );
void            vMBPortExitCritical( void );
void            vMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule,
                            const CHAR * szFmt, ... );
void            vMBPortTimerPoll(  );
bool            xMBPortSerialPoll(  );
bool            xMBPortSerialSetTimeout( uint32_t dwTimeoutMs );

#ifdef __cplusplus
}
#endif
#endif
