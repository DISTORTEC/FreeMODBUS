/*
 * FreeModbus Libary: BSD Socket Library Port
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
 * File: $Id: port.h,v 1.1 2007/09/12 10:15:56 wolti Exp $
 */

 /**********************************************************
 *	Linux TCP support.
 *	Based on Walter's project.
 *	Modified by Steven Guo <gotop167@163.com>
 ***********************************************************/

#ifndef _PORT_H
#define _PORT_H

#include <assert.h>
#include <stdbool.h>

#define	INLINE

#ifdef __cplusplus
extern "C" {
#endif
/* ----------------------- Defines ------------------------------------------*/
#define ENTER_CRITICAL_SECTION( )
#define EXIT_CRITICAL_SECTION( )
#define MB_PORT_HAS_CLOSE	1
/* ----------------------- Type definitions ---------------------------------*/
typedef int     SOCKET;

#define SOCKET_ERROR (-1)
#define INVALID_SOCKET (~0)
typedef unsigned char UCHAR;
typedef unsigned char BYTE;
typedef char    CHAR;
typedef unsigned short USHORT;
typedef short   SHORT;

typedef enum
{
    MB_LOG_DEBUG,
    MB_LOG_INFO,
    MB_LOG_WARN,
    MB_LOG_ERROR
} eMBPortLogLevel;

/* ----------------------- Function prototypes ------------------------------*/

void            TcpvMBPortLog( eMBPortLogLevel eLevel, const CHAR * szModule, const CHAR * szFmt,
                               ... );

#ifdef __cplusplus
}
#endif
#endif
