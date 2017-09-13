/*
 * FreeModbus Libary: A portable Modbus implementation for Modbus ASCII/RTU.
 * Copyright (c) 2006 Christian Walter <wolti@sil.at>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * File: $Id: mb.c,v 1.28 2010/06/06 13:54:40 wolti Exp $
 */

#include "mb.h"

#include "port.h"

#include "mbconfig.h"
#include "mbeventtype.h"
#include "mbframe.h"
#include "mbfunc.h"
#include "mbinstance.h"
#include "mbport.h"

#if MB_RTU_ENABLED == 1
#include "mbrtu.h"
#endif
#if MB_ASCII_ENABLED == 1
#include "mbascii.h"
#endif
#if MB_TCP_ENABLED == 1
#include "mbtcp.h"
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

#ifndef MB_PORT_HAS_CLOSE
#define MB_PORT_HAS_CLOSE 0
#endif

/* ----------------------- Static variables ---------------------------------*/
/* An array of Modbus functions handlers which associates Modbus function
 * codes with implementing functions.
 */
static const xMBFunctionHandler xFuncHandlersDefault[MB_FUNC_HANDLERS_MAX] =
{
#if MB_FUNC_OTHER_REP_SLAVEID_ENABLED > 0
    {MB_FUNC_OTHER_REPORT_SLAVEID, eMBFuncReportSlaveID},
#endif
#if MB_FUNC_READ_INPUT_ENABLED > 0
    {MB_FUNC_READ_INPUT_REGISTER, eMBFuncReadInputRegister},
#endif
#if MB_FUNC_READ_HOLDING_ENABLED > 0
    {MB_FUNC_READ_HOLDING_REGISTER, eMBFuncReadHoldingRegister},
#endif
#if MB_FUNC_WRITE_MULTIPLE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_REGISTERS, eMBFuncWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_WRITE_HOLDING_ENABLED > 0
    {MB_FUNC_WRITE_REGISTER, eMBFuncWriteHoldingRegister},
#endif
#if MB_FUNC_READWRITE_HOLDING_ENABLED > 0
    {MB_FUNC_READWRITE_MULTIPLE_REGISTERS, eMBFuncReadWriteMultipleHoldingRegister},
#endif
#if MB_FUNC_READ_COILS_ENABLED > 0
    {MB_FUNC_READ_COILS, eMBFuncReadCoils},
#endif
#if MB_FUNC_WRITE_COIL_ENABLED > 0
    {MB_FUNC_WRITE_SINGLE_COIL, eMBFuncWriteCoil},
#endif
#if MB_FUNC_WRITE_MULTIPLE_COILS_ENABLED > 0
    {MB_FUNC_WRITE_MULTIPLE_COILS, eMBFuncWriteMultipleCoils},
#endif
#if MB_FUNC_READ_DISCRETE_INPUTS_ENABLED > 0
    {MB_FUNC_READ_DISCRETE_INPUTS, eMBFuncReadDiscreteInputs},
#endif
};

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBInit( struct xMBInstance * xInstance, const struct xMBCallbacks * xCallbacks,
         eMBMode eMode, uint8_t ucSlaveAddress, uint8_t ucPort,
         uint32_t ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    /* check preconditions */
    if( ( ucSlaveAddress == MB_ADDRESS_BROADCAST ) ||
        ( ucSlaveAddress < MB_ADDRESS_MIN ) || ( ucSlaveAddress > MB_ADDRESS_MAX ) )
    {
        eStatus = MB_EINVAL;
    }
    else
    {
        xInstance->ucMBAddress = ucSlaveAddress;
        xInstance->xCallbacks = xCallbacks;
        memcpy(xInstance->xFuncHandlers, xFuncHandlersDefault, sizeof(xFuncHandlersDefault));

        switch ( eMode )
        {
#if MB_RTU_ENABLED > 0
        case MB_RTU:
            xInstance->pvMBFrameStartCur = eMBRTUStart;
            xInstance->pvMBFrameStopCur = eMBRTUStop;
            xInstance->peMBFrameSendCur = eMBRTUSend;
            xInstance->peMBFrameReceiveCur = eMBRTUReceive;
            xInstance->pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? xMBPortSerialClose : NULL;
            xInstance->pxMBFrameCBByteReceived = xMBRTUReceiveFSM;
            xInstance->pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM;
            xInstance->pxMBPortCBTimerExpired = xMBRTUTimerT35Expired;

            eStatus = eMBRTUInit( xInstance, xInstance->ucMBAddress, ucPort, ulBaudRate, eParity );
            break;
#endif
#if MB_ASCII_ENABLED > 0
        case MB_ASCII:
            xInstance->pvMBFrameStartCur = eMBASCIIStart;
            xInstance->pvMBFrameStopCur = eMBASCIIStop;
            xInstance->peMBFrameSendCur = eMBASCIISend;
            xInstance->peMBFrameReceiveCur = eMBASCIIReceive;
            xInstance->pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? xMBPortSerialClose : NULL;
            xInstance->pxMBFrameCBByteReceived = xMBASCIIReceiveFSM;
            xInstance->pxMBFrameCBTransmitterEmpty = xMBASCIITransmitFSM;
            xInstance->pxMBPortCBTimerExpired = xMBASCIITimerT1SExpired;

            eStatus = eMBASCIIInit( xInstance, xInstance->ucMBAddress, ucPort, ulBaudRate, eParity );
            break;
#endif
        default:
            eStatus = MB_EINVAL;
        }

        if( eStatus == MB_ENOERR )
        {
            if( !xMBPortEventInit( xInstance ) )
            {
                /* port dependent event module initalization failed. */
                eStatus = MB_EPORTERR;
            }
            else
            {
                xInstance->eMBCurrentMode = eMode;
                xInstance->eMBState = STATE_DISABLED;
            }
        }
    }
    return eStatus;
}

#if MB_TCP_ENABLED > 0
eMBErrorCode
eMBTCPInit( struct xMBInstance * xInstance, const struct xMBCallbacks * xCallbacks, uint16_t ucTCPPort )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( ( eStatus = eMBTCPDoInit( xInstance, ucTCPPort ) ) != MB_ENOERR )
    {
        xInstance->eMBState = STATE_DISABLED;
    }
    else if( !xMBPortEventInit( xInstance ) )
    {
        /* Port dependent event module initalization failed. */
        eStatus = MB_EPORTERR;
    }
    else
    {
        memcpy(xInstance->xFuncHandlers, xFuncHandlersDefault, sizeof(xFuncHandlersDefault));
        xInstance->xCallbacks = xCallbacks;
        xInstance->pvMBFrameStartCur = eMBTCPStart;
        xInstance->pvMBFrameStopCur = eMBTCPStop;
        xInstance->peMBFrameReceiveCur = eMBTCPReceive;
        xInstance->peMBFrameSendCur = eMBTCPSend;
        xInstance->pvMBFrameCloseCur = MB_PORT_HAS_CLOSE ? vMBTCPPortClose : NULL;
        xInstance->ucMBAddress = MB_TCP_PSEUDO_ADDRESS;
        xInstance->eMBCurrentMode = MB_TCP;
        xInstance->eMBState = STATE_DISABLED;
    }
    return eStatus;
}
#endif

eMBErrorCode
eMBRegisterCB( struct xMBInstance * xInstance, uint8_t ucFunctionCode, pxMBFunctionHandler pxHandler )
{
    int             i;
    eMBErrorCode    eStatus;

    if( ( 0 < ucFunctionCode ) && ( ucFunctionCode <= 127 ) )
    {
        ENTER_CRITICAL_SECTION( xInstance );
        if( pxHandler != NULL )
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( ( xInstance->xFuncHandlers[i].pxHandler == NULL ) ||
                    ( xInstance->xFuncHandlers[i].pxHandler == pxHandler ) )
                {
                    xInstance->xFuncHandlers[i].ucFunctionCode = ucFunctionCode;
                    xInstance->xFuncHandlers[i].pxHandler = pxHandler;
                    break;
                }
            }
            eStatus = ( i != MB_FUNC_HANDLERS_MAX ) ? MB_ENOERR : MB_ENORES;
        }
        else
        {
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                if( xInstance->xFuncHandlers[i].ucFunctionCode == ucFunctionCode )
                {
                    xInstance->xFuncHandlers[i].ucFunctionCode = 0;
                    xInstance->xFuncHandlers[i].pxHandler = NULL;
                    break;
                }
            }
            /* Remove can't fail. */
            eStatus = MB_ENOERR;
        }
        EXIT_CRITICAL_SECTION( xInstance );
    }
    else
    {
        eStatus = MB_EINVAL;
    }
    return eStatus;
}


eMBErrorCode
eMBClose( struct xMBInstance * xInstance )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( xInstance->eMBState == STATE_DISABLED )
    {
        if( xInstance->pvMBFrameCloseCur != NULL )
        {
            xInstance->pvMBFrameCloseCur( xInstance );
        }
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBEnable( struct xMBInstance * xInstance )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    if( xInstance->eMBState == STATE_DISABLED )
    {
        /* Activate the protocol stack. */
        xInstance->pvMBFrameStartCur( xInstance );
        xInstance->eMBState = STATE_ENABLED;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBDisable( struct xMBInstance * xInstance )
{
    eMBErrorCode    eStatus;

    if( xInstance->eMBState == STATE_ENABLED )
    {
        xInstance->pvMBFrameStopCur( xInstance );
        xInstance->eMBState = STATE_DISABLED;
        eStatus = MB_ENOERR;
    }
    else if( xInstance->eMBState == STATE_DISABLED )
    {
        eStatus = MB_ENOERR;
    }
    else
    {
        eStatus = MB_EILLSTATE;
    }
    return eStatus;
}

eMBErrorCode
eMBPoll( struct xMBInstance * xInstance )
{
    int             i;
    eMBErrorCode    eStatus = MB_ENOERR;
    eMBEventType    eEvent;

    /* Check if the protocol stack is ready. */
    if( xInstance->eMBState != STATE_ENABLED )
    {
        return MB_EILLSTATE;
    }

    /* Check if there is a event available. If not return control to caller.
     * Otherwise we will handle the event. */
    if( xMBPortEventGet( xInstance, &eEvent ) == true )
    {
        switch ( eEvent )
        {
        case EV_READY:
            break;

        case EV_FRAME_RECEIVED:
            eStatus = xInstance->peMBFrameReceiveCur( xInstance, &xInstance->ucRcvAddress, &xInstance->ucMBFrame, &xInstance->usLength );
            if( eStatus == MB_ENOERR )
            {
                /* Check if the frame is for us. If not ignore the frame. */
                if( ( xInstance->ucRcvAddress == xInstance->ucMBAddress ) || ( xInstance->ucRcvAddress == MB_ADDRESS_BROADCAST ) )
                {
                    ( void )xMBPortEventPost( xInstance, EV_EXECUTE );
                }
            }
            break;

        case EV_EXECUTE:
            xInstance->ucFunctionCode = xInstance->ucMBFrame[MB_PDU_FUNC_OFF];
            xInstance->eException = MB_EX_ILLEGAL_FUNCTION;
            for( i = 0; i < MB_FUNC_HANDLERS_MAX; i++ )
            {
                /* No more function handlers registered. Abort. */
                if( xInstance->xFuncHandlers[i].ucFunctionCode == 0 )
                {
                    break;
                }
                else if( xInstance->xFuncHandlers[i].ucFunctionCode == xInstance->ucFunctionCode )
                {
                    xInstance->eException = xInstance->xFuncHandlers[i].pxHandler( xInstance, xInstance->ucMBFrame, &xInstance->usLength );
                    break;
                }
            }

            /* If the request was not sent to the broadcast address we
             * return a reply. */
            if( xInstance->ucRcvAddress != MB_ADDRESS_BROADCAST )
            {
                if( xInstance->eException != MB_EX_NONE )
                {
                    /* An exception occured. Build an error frame. */
                    xInstance->usLength = 0;
                    xInstance->ucMBFrame[xInstance->usLength++] = ( uint8_t )( xInstance->ucFunctionCode | MB_FUNC_ERROR );
                    xInstance->ucMBFrame[xInstance->usLength++] = xInstance->eException;
                }
                if( ( xInstance->eMBCurrentMode == MB_ASCII ) && MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS )
                {
                    vMBPortTimersDelay( xInstance, MB_ASCII_TIMEOUT_WAIT_BEFORE_SEND_MS );
                }
                eStatus = xInstance->peMBFrameSendCur( xInstance, xInstance->ucMBAddress, xInstance->ucMBFrame, xInstance->usLength );
            }
            break;

        case EV_FRAME_SENT:
            break;
        }
    }
    return eStatus;
}
