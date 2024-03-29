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
 * File: $Id: mbrtu.c,v 1.18 2007/09/12 10:15:56 wolti Exp $
 */

#include "mbrtu.h"

#include "port.h"

#include "mbcrc.h"
#include "mbinstance.h"
#include "mbport.h"
#include "mbrcvstate.h"
#include "mbsndstate.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#if MB_RTU_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_PDU_SIZE_MIN     4       /*!< Minimum size of a Modbus RTU frame. */
#define MB_SER_PDU_SIZE_CRC     2       /*!< Size of CRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBRTUInit( struct xMBInstance * xInstance, uint8_t ucSlaveAddress,
            uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint32_t           usTimerT35_50us;

    ( void )ucSlaveAddress;
    ENTER_CRITICAL_SECTION( xInstance );

    /* Modbus RTU uses 8 Databits. */
    if( xMBPortSerialInit( xInstance, ucPort, ulBaudRate, 8, eParity ) != true )
    {
        eStatus = MB_EPORTERR;
    }
    else
    {
        /* If baudrate > 19200 then we should use the fixed timer values
         * t35 = 1750us. Otherwise t35 must be 3.5 times the character time.
         */
        if( ulBaudRate > 19200 )
        {
            usTimerT35_50us = 35;       /* 1800us. */
        }
        else
        {
            /* The timer reload value for a character is given by:
             *
             * ChTimeValue = Ticks_per_1s / ( Baudrate / 11 )
             *             = 11 * Ticks_per_1s / Baudrate
             *             = 220000 / Baudrate
             * The reload for t3.5 is 1.5 times this value and similary
             * for t3.5.
             */
            usTimerT35_50us = ( 7UL * 220000UL ) / ( 2UL * ulBaudRate );
        }
        if( xMBPortTimersInit( xInstance, ( uint16_t ) usTimerT35_50us ) != true )
        {
            eStatus = MB_EPORTERR;
        }
    }
    EXIT_CRITICAL_SECTION( xInstance );

    return eStatus;
}

void
eMBRTUStart( struct xMBInstance * xInstance )
{
    ENTER_CRITICAL_SECTION( xInstance );
    /* Initially the receiver is in the state STATE_RX_INIT. we start
     * the timer and if no character is received within t3.5 we change
     * to STATE_RX_IDLE. This makes sure that we delay startup of the
     * modbus protocol stack until the bus is free.
     */
    xInstance->eRcvState = STATE_RX_INIT;
    xInstance->eSndState = STATE_TX_IDLE;
    vMBPortSerialEnable( xInstance, true, false );
    vMBPortTimersEnable( xInstance );

    EXIT_CRITICAL_SECTION( xInstance );
}

void
eMBRTUStop( struct xMBInstance * xInstance )
{
    ENTER_CRITICAL_SECTION( xInstance );
    vMBPortSerialEnable( xInstance, false, false );
    vMBPortTimersDisable( xInstance );
    EXIT_CRITICAL_SECTION( xInstance );
}

eMBErrorCode
eMBRTUReceive( struct xMBInstance * xInstance, uint8_t * pucRcvAddress,
               uint8_t ** pucFrame, uint16_t * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION( xInstance );
    assert( xInstance->usRcvBufferPos <= MB_SER_SIZE_MAX );

    /* Length and CRC check */
    if( ( xInstance->usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
        && ( usMBCRC16( ( uint8_t * ) xInstance->ucBuf, xInstance->usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = xInstance->ucBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( uint16_t )( xInstance->usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_CRC );

        /* Return the start of the Modbus PDU to the caller. */
        *pucFrame = ( uint8_t * ) & xInstance->ucBuf[MB_SER_PDU_PDU_OFF];
    }
    else
    {
        eStatus = MB_EIO;
    }

    EXIT_CRITICAL_SECTION( xInstance );
    return eStatus;
}

eMBErrorCode
eMBRTUSend( struct xMBInstance * xInstance, uint8_t ucSlaveAddress,
            const uint8_t * pucFrame, uint16_t usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint16_t          usCRC16;

    ENTER_CRITICAL_SECTION( xInstance );

    /* Check if the receiver is still in idle state. If not we where to
     * slow with processing the received frame and the master sent another
     * frame on the network. We have to abort sending the frame.
     */
    if( xInstance->eRcvState == STATE_RX_IDLE )
    {
        /* First byte before the Modbus-PDU is the slave address. */
        xInstance->pucSndBufferCur = ( uint8_t * ) pucFrame - 1;
        xInstance->usSndBufferCount = 1;

        /* Now copy the Modbus-PDU into the Modbus-Serial-Line-PDU. */
        xInstance->pucSndBufferCur[MB_SER_PDU_ADDR_OFF] = ucSlaveAddress;
        xInstance->usSndBufferCount += usLength;

        /* Calculate CRC16 checksum for Modbus-Serial-Line-PDU. */
        usCRC16 = usMBCRC16( ( uint8_t * ) xInstance->pucSndBufferCur, xInstance->usSndBufferCount );
        xInstance->ucBuf[xInstance->usSndBufferCount++] = ( uint8_t )( usCRC16 & 0xFF );
        xInstance->ucBuf[xInstance->usSndBufferCount++] = ( uint8_t )( usCRC16 >> 8 );

        /* Activate the transmitter. */
        xInstance->eSndState = STATE_TX_XMIT;
        vMBPortSerialEnable( xInstance, false, true );
    }
    else
    {
        eStatus = MB_EIO;
    }
    EXIT_CRITICAL_SECTION( xInstance );
    return eStatus;
}

bool
xMBRTUReceiveFSM( struct xMBInstance * xInstance )
{
    bool            xTaskNeedSwitch = false;
    uint8_t           ucByte = 0;

    assert( xInstance->eSndState == STATE_TX_IDLE );

    /* Always read the character. */
    ( void )xMBPortSerialGetByte( xInstance, ( uint8_t * ) & ucByte );

    switch ( xInstance->eRcvState )
    {
        /* If we have received a character in the init state we have to
         * wait until the frame is finished.
         */
    case STATE_RX_INIT:
        vMBPortTimersEnable( xInstance );
        break;

        /* In the error state we wait until all characters in the
         * damaged frame are transmitted.
         */
    case STATE_RX_ERROR:
        vMBPortTimersEnable( xInstance );
        break;

        /* In the idle state we wait for a new character. If a character
         * is received the t1.5 and t3.5 timers are started and the
         * receiver is in the state STATE_RX_RECEIVCE.
         */
    case STATE_RX_IDLE:
        xInstance->usRcvBufferPos = 0;
        xInstance->ucBuf[xInstance->usRcvBufferPos++] = ucByte;
        xInstance->eRcvState = STATE_RX_RCV;

        /* Enable t3.5 timers. */
        vMBPortTimersEnable( xInstance );
        break;

        /* We are currently receiving a frame. Reset the timer after
         * every character received. If more than the maximum possible
         * number of bytes in a modbus frame is received the frame is
         * ignored.
         */
    case STATE_RX_RCV:
        if( xInstance->usRcvBufferPos < MB_SER_SIZE_MAX )
        {
            xInstance->ucBuf[xInstance->usRcvBufferPos++] = ucByte;
        }
        else
        {
            xInstance->eRcvState = STATE_RX_ERROR;
        }
        vMBPortTimersEnable( xInstance );
        break;
    default:
        assert( ( xInstance->eRcvState == STATE_RX_INIT ) || ( xInstance->eRcvState == STATE_RX_IDLE ) ||
                ( xInstance->eRcvState == STATE_RX_RCV ) || ( xInstance->eRcvState == STATE_RX_ERROR ) );
    }
    return xTaskNeedSwitch;
}

bool
xMBRTUTransmitFSM( struct xMBInstance * xInstance )
{
    bool            xNeedPoll = false;

    assert( xInstance->eRcvState == STATE_RX_IDLE );

    switch ( xInstance->eSndState )
    {
        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMBPortSerialEnable( xInstance, true, false );
        break;

    case STATE_TX_XMIT:
        /* check if we are finished. */
        if( xInstance->usSndBufferCount != 0 )
        {
            xMBPortSerialPutByte( xInstance, ( int8_t )*xInstance->pucSndBufferCur );
            xInstance->pucSndBufferCur++;  /* next byte in sendbuffer. */
            xInstance->usSndBufferCount--;
        }
        else
        {
            xNeedPoll = xMBPortEventPost( xInstance, EV_FRAME_SENT );
            /* Disable transmitter. This prevents another transmit buffer
             * empty interrupt. */
            vMBPortSerialEnable( xInstance, true, false );
            xInstance->eSndState = STATE_TX_IDLE;
        }
        break;
    default:
        assert( ( xInstance->eSndState == STATE_TX_IDLE ) || ( xInstance->eSndState == STATE_TX_XMIT ) );
    }

    return xNeedPoll;
}

bool
xMBRTUTimerT35Expired( struct xMBInstance * xInstance )
{
    bool            xNeedPoll = false;

    switch ( xInstance->eRcvState )
    {
        /* Timer t35 expired. Startup phase is finished. */
    case STATE_RX_INIT:
        xNeedPoll = xMBPortEventPost( xInstance, EV_READY );
        break;

        /* A frame was received and t35 expired. Notify the listener that
         * a new frame was received. */
    case STATE_RX_RCV:
        xNeedPoll = xMBPortEventPost( xInstance, EV_FRAME_RECEIVED );
        break;

        /* An error occured while receiving the frame. */
    case STATE_RX_ERROR:
        break;

        /* Function called in an illegal state. */
    default:
        assert( ( xInstance->eRcvState == STATE_RX_INIT ) ||
                ( xInstance->eRcvState == STATE_RX_RCV ) || ( xInstance->eRcvState == STATE_RX_ERROR ) );
    }

    vMBPortTimersDisable( xInstance );
    xInstance->eRcvState = STATE_RX_IDLE;

    return xNeedPoll;
}

#endif
