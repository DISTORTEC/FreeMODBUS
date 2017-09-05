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
 * File: $Id: port.h,v 1.5 2006/06/26 19:25:17 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

#include <windows.h>
#include <tchar.h>
#include <assert.h>

#define	INLINE

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/

#define ENTER_CRITICAL_SECTION( )
#define EXIT_CRITICAL_SECTION( )
#define MB_PORT_HAS_CLOSE	1

/* ----------------------- Type definitions ---------------------------------*/

typedef enum
{
    MB_LOG_DEBUG,
    MB_LOG_INFO,
    MB_LOG_WARN,
    MB_LOG_ERROR
} eMBPortLogLevel;

/* ----------------------- Function prototypes ------------------------------*/

void            vMBPortLog( eMBPortLogLevel eLevel, const TCHAR * szModule,
                            const TCHAR * szFmt, ... );
void            vMBPortTimerPoll(  );
bool            xMBPortSerialPoll(  );
bool            xMBPortSerialSetTimeout( DWORD dwTimeoutMs );

#ifdef __cplusplus
}
#endif
#endif
