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
 * File: $Id: portother.c,v 1.1 2006/08/01 20:58:49 wolti Exp $
 */

/* ----------------------- Standard includes --------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>

#include "port.h"
#include "portinstance.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbconfig.h"

/* ----------------------- Defines ------------------------------------------*/
#define NELEMS( x ) ( sizeof( ( x ) )/sizeof( ( x )[0] ) )

/* ----------------------- Start implementation -----------------------------*/
void
vMBPortLogLevel( struct xMBInstance * xInstance, eMBPortLogLevel eNewLevelMax )
{
    ( ( struct PortInstance* )xInstance )->eLevelMax = eNewLevelMax;
}

void
vMBPortLogFile( struct xMBInstance * xInstance, FILE * fNewLogFile )
{
    ( ( struct PortInstance* )xInstance )->fLogFile = fNewLogFile;
}

void
vMBPortLog( struct xMBInstance * xInstance, eMBPortLogLevel eLevel, const char * szModule, const char * szFmt, ... )
{
    char            szBuf[512];
    int             i;
    va_list         args;
    FILE           *fOutput = ( ( struct PortInstance* )xInstance )->fLogFile == NULL ? stderr : ( ( struct PortInstance* )xInstance )->fLogFile;

    static const char *arszLevel2Str[] = { "ERROR", "WARN", "INFO", "DEBUG" };

    i = snprintf( szBuf, NELEMS( szBuf ), "%s: %s: ", arszLevel2Str[eLevel], szModule );

    if( i != 0 )
    {
        va_start( args, szFmt );
        i += vsnprintf( &szBuf[i], NELEMS( szBuf ) - i, szFmt, args );
        va_end( args );
    }

    if( i != 0 )
    {
        if( eLevel <= ( ( struct PortInstance* )xInstance )->eLevelMax )
        {
            fputs( szBuf, fOutput );
        }
    }
}

void
vMBPortEnterCritical( struct xMBInstance * xInstance )
{
    if( pthread_mutex_lock( &( ( struct PortInstance* )xInstance )->xLockOther ) != 0 )
    {
        vMBPortLog( xInstance, MB_LOG_ERROR, "OTHER", "Locking primitive failed: %s\n", strerror( errno ) );
    }
}

void
vMBPortExitCritical( struct xMBInstance * xInstance )
{
    if( pthread_mutex_unlock( &( ( struct PortInstance* )xInstance )->xLockOther ) != 0 )
    {
        vMBPortLog( xInstance, MB_LOG_ERROR, "OTHER", "Locking primitive failed: %s\n", strerror( errno ) );
    }
}
