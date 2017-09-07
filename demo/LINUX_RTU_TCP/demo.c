/*
 * FreeModbus Libary: Linux Demo Application
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
 * File: $Id: demo.c,v 1.2 2006/10/12 08:12:06 wolti Exp $
 */

#include "portinstance.h"

/* ----------------------- Standard includes --------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "mbcallbacks.h"

/* ----------------------- Defines ------------------------------------------*/
#define PROG            "freemodbus"

eMBErrorCode
eMBRegInputCB( struct xMBInstance * xInstance, uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_INPUT_START )
        && ( usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - ( ( struct PortInstance* )xInstance )->usRegInputStart );
        while( usNRegs > 0 )
        {
            *pucRegBuffer++ = ( unsigned char )( ( ( struct PortInstance* )xInstance )->usRegInputBuf[iRegIndex] >> 8 );
            *pucRegBuffer++ = ( unsigned char )( ( ( struct PortInstance* )xInstance )->usRegInputBuf[iRegIndex] & 0xFF );
            iRegIndex++;
            usNRegs--;
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }

    return eStatus;
}

eMBErrorCode
eMBRegHoldingCB( struct xMBInstance * xInstance, uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNRegs, eMBRegisterMode eMode )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    int             iRegIndex;

    if( ( usAddress >= REG_HOLDING_START ) &&
        ( usAddress + usNRegs <= REG_HOLDING_START + REG_HOLDING_NREGS ) )
    {
        iRegIndex = ( int )( usAddress - ( ( struct PortInstance* )xInstance )->usRegHoldingStart );
        switch ( eMode )
        {
            /* Pass current register values to the protocol stack. */
        case MB_REG_READ:
            while( usNRegs > 0 )
            {
                *pucRegBuffer++ = ( uint8_t ) ( ( ( struct PortInstance* )xInstance )->usRegHoldingBuf[iRegIndex] >> 8 );
                *pucRegBuffer++ = ( uint8_t ) ( ( ( struct PortInstance* )xInstance )->usRegHoldingBuf[iRegIndex] & 0xFF );
                iRegIndex++;
                usNRegs--;
            }
            break;

            /* Update current register values with new values from the
             * protocol stack. */
        case MB_REG_WRITE:
            while( usNRegs > 0 )
            {
                ( ( struct PortInstance* )xInstance )->usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;
                ( ( struct PortInstance* )xInstance )->usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;
                iRegIndex++;
                usNRegs--;
            }
        }
    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}


eMBErrorCode
eMBRegCoilsCB( struct xMBInstance * xInstance, uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNCoils, eMBRegisterMode eMode )
{
    (void)xInstance;
    (void)pucRegBuffer;
    (void)usAddress;
    (void)usNCoils;
    (void)eMode;
    return MB_ENOREG;
}

eMBErrorCode
eMBRegDiscreteCB( struct xMBInstance * xInstance, uint8_t * pucRegBuffer, uint16_t usAddress, uint16_t usNDiscrete )
{
    (void)xInstance;
    (void)pucRegBuffer;
    (void)usAddress;
    (void)usNDiscrete;
    return MB_ENOREG;
}

/* ----------------------- Static variables ---------------------------------*/
static const struct xMBCallbacks xCallbacks1 =
{
    eMBRegInputCB,
    eMBRegHoldingCB,
    eMBRegCoilsCB,
    eMBRegDiscreteCB,
};
static const struct xMBCallbacks xCallbacks2 =
{
    eMBRegInputCB,
    eMBRegHoldingCB,
    eMBRegCoilsCB,
    eMBRegDiscreteCB,
};

static struct PortInstance xInstance1 = PORTINSTANCE_INITIALIZER;
static struct PortInstance xInstance2 = PORTINSTANCE_INITIALIZER;

/* ----------------------- Static functions ---------------------------------*/
static bool     bCreatePollingThread( struct PortInstance * xInstance );
static eThreadState eGetPollingThreadState( struct PortInstance * xInstance  );
static void     vSetPollingThreadState( struct PortInstance * xInstance, eThreadState eNewState );
static void    *pvPollingThread( void *pvParameter );

/* ----------------------- Start implementation -----------------------------*/
bool
bSetSignal( int iSignalNr, void ( *pSigHandler ) ( int ) )
{
    bool            bResult;
    struct sigaction xNewSig, xOldSig;

    xNewSig.sa_handler = pSigHandler;
    sigemptyset( &xNewSig.sa_mask );
    xNewSig.sa_flags = 0;
    if( sigaction( iSignalNr, &xNewSig, &xOldSig ) != 0 )
    {
        bResult = false;
    }
    else
    {
        bResult = true;
    }
    return bResult;
}

void
vSigShutdown( int xSigNr )
{
    switch ( xSigNr )
    {
    case SIGQUIT:
    case SIGINT:
    case SIGTERM:
        vSetPollingThreadState( &xInstance1, SHUTDOWN );
        vSetPollingThreadState( &xInstance2, SHUTDOWN );
        xInstance1.bDoExit = true;
        xInstance2.bDoExit = true;
    }
}

int
main( void )
{
    int             iExitCode;
    int8_t            cCh;

    const uint8_t     ucSlaveID[] = { 0xAA, 0xBB, 0xCC };
    if( !bSetSignal( SIGQUIT, vSigShutdown ) ||
        !bSetSignal( SIGINT, vSigShutdown ) || !bSetSignal( SIGTERM, vSigShutdown ) )
    {
        fprintf( stderr, "%s: can't install signal handlers: %s!\n", PROG, strerror( errno ) );
        iExitCode = EXIT_FAILURE;
    }
    else if( eMBInit( &xInstance1.xInstance, &xCallbacks1, MB_RTU, 0x0A, 2, 38400, MB_PAR_EVEN ) != MB_ENOERR )
    {
        fprintf( stderr, "%s: can't initialize modbus stack 1!\n", PROG );
        iExitCode = EXIT_FAILURE;
    }
    else if( eMBTCPInit( &xInstance2.xInstance, &xCallbacks2, MB_TCP_PORT_USE_DEFAULT ) != MB_ENOERR )
    {
        fprintf( stderr, "%s: can't initialize modbus stack!\r\n", PROG );
        iExitCode = EXIT_FAILURE;
    }
    else if( eMBSetSlaveID( &xInstance1.xInstance, 0x34, true, ucSlaveID, 3 ) != MB_ENOERR )
    {
        fprintf( stderr, "%s: can't set slave id!\n", PROG );
        iExitCode = EXIT_FAILURE;
    }
    else if( eMBSetSlaveID( &xInstance2.xInstance, 0x34, true, ucSlaveID, 3 ) != MB_ENOERR )
    {
        fprintf( stderr, "%s: can't set slave id!\n", PROG );
        iExitCode = EXIT_FAILURE;
    }
    else
    {
        vSetPollingThreadState( &xInstance1, STOPPED );
        vSetPollingThreadState( &xInstance2, STOPPED );

        /* CLI interface. */
        printf( "Type 'q' for quit or 'h' for help!\n" );
        xInstance1.bDoExit = false;
        xInstance2.bDoExit = false;
        do
        {
            printf( "> " );
            cCh = getchar(  );

            switch ( cCh )
            {
            case 'q':
                xInstance1.bDoExit = true;
                xInstance2.bDoExit = true;
                break;
            case 'd':
                vSetPollingThreadState( &xInstance1, SHUTDOWN );
                vSetPollingThreadState( &xInstance2, SHUTDOWN );
                break;
            case 'e':
                if( bCreatePollingThread( &xInstance1 ) != true )
                {
                    printf( "Can't start protocol stack 1! Already running?\n" );
                }
                if( bCreatePollingThread( &xInstance2 ) != true )
                {
                    printf( "Can't start protocol stack 2! Already running?\n" );
                }
                break;
            case 's':
                switch ( eGetPollingThreadState( &xInstance1 ) )
                {
                case RUNNING:
                    printf( "Protocol stack 1 is running.\n" );
                    break;
                case STOPPED:
                    printf( "Protocol stack 1 is stopped.\n" );
                    break;
                case SHUTDOWN:
                    printf( "Protocol stack 1 is shuting down.\n" );
                    break;
                }
                switch ( eGetPollingThreadState( &xInstance2 ) )
                {
                case RUNNING:
                    printf( "Protocol stack 2 is running.\n" );
                    break;
                case STOPPED:
                    printf( "Protocol stack 2 is stopped.\n" );
                    break;
                case SHUTDOWN:
                    printf( "Protocol stack 2 is shuting down.\n" );
                    break;
                }
                break;
            case 'h':
                printf( "FreeModbus demo application help:\n" );
                printf( "  'd' ... disable protocol stacks.\n" );
                printf( "  'e' ... enabled the protocol stacks.\n" );
                printf( "  's' ... show current status.\n" );
                printf( "  'q' ... quit application.\n" );
                printf( "  'h' ... this information.\n" );
                printf( "\n" );
                printf( "Copyright 2006 Christian Walter <wolti@sil.at>\n" );
                break;
            default:
                if( !xInstance1.bDoExit && ( cCh != '\n' ) )
                {
                    printf( "illegal command '%c'!\n", cCh );
                }
                if( !xInstance2.bDoExit && ( cCh != '\n' ) )
                {
                    printf( "illegal command '%c'!\n", cCh );
                }
                break;
            }

            /* eat up everything untill return character. */
            while( !xInstance1.bDoExit && ( cCh != '\n' ) )
            {
                cCh = getchar(  );
            }
        }
        while( !xInstance1.bDoExit );

        /* Release hardware resources. */
        ( void )eMBClose( &xInstance1.xInstance );
        iExitCode = EXIT_SUCCESS;
    }
    return iExitCode;
}

bool
bCreatePollingThread( struct PortInstance * xInstance )
{
    bool            bResult;
    pthread_t       xThread;

    if( eGetPollingThreadState( xInstance ) == STOPPED )
    {
        if( pthread_create( &xThread, NULL, pvPollingThread, xInstance ) != 0 )
        {
            bResult = false;
        }
        else
        {
            bResult = true;
        }
    }
    else
    {
        bResult = false;
    }

    return bResult;
}

void           *
pvPollingThread( void *pvParameter )
{
    struct PortInstance *xInstance = pvParameter;
    vSetPollingThreadState( xInstance, RUNNING );

    if( eMBEnable( ( struct xMBInstance* )xInstance ) == MB_ENOERR )
    {
        do
        {
            if( eMBPoll( ( struct xMBInstance* )xInstance ) != MB_ENOERR )
                break;
            if (xInstance == &xInstance1)
            {
                int i;
                for (i = 0; i < REG_INPUT_NREGS; ++i)
                    xInstance->usRegInputBuf[i] = ( uint16_t ) 0x1111;
            }
            else
            {
                int i;
                for (i = 0; i < REG_INPUT_NREGS; ++i)
                    xInstance->usRegInputBuf[i] = ( uint16_t ) 0x2222;
            }
        }
        while( eGetPollingThreadState( xInstance ) != SHUTDOWN );
    }

    ( void )eMBDisable( ( struct xMBInstance* )xInstance );

    vSetPollingThreadState( xInstance, STOPPED );

    return 0;
}

eThreadState
eGetPollingThreadState( struct PortInstance * xInstance )
{
    eThreadState eCurState;

    ( void )pthread_mutex_lock( &xInstance->xLock );
    eCurState = xInstance->ePollThreadState;
    ( void )pthread_mutex_unlock( &xInstance->xLock );

    return eCurState;
}

void
vSetPollingThreadState( struct PortInstance * xInstance, eThreadState eNewState )
{
    ( void )pthread_mutex_lock( &xInstance->xLock );
    xInstance->ePollThreadState = eNewState;
    ( void )pthread_mutex_unlock( &xInstance->xLock );
}
