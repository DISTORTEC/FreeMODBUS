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
 * File: $Id: mbascii.c,v 1.17 2010/06/06 13:47:07 wolti Exp $
 */

#include "mbascii.h"

#include "port.h"

#include "mbbytepos.h"
#include "mbinstance.h"
#include "mbport.h"
#include "mbrcvstate.h"
#include "mbsndstate.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#if MB_ASCII_ENABLED > 0

/* ----------------------- Defines ------------------------------------------*/
#define MB_ASCII_DEFAULT_CR     '\r'    /*!< Default CR character for Modbus ASCII. */
#define MB_ASCII_DEFAULT_LF     '\n'    /*!< Default LF character for Modbus ASCII. */
#define MB_SER_PDU_SIZE_MIN     3       /*!< Minimum size of a Modbus ASCII frame. */
#define MB_SER_PDU_SIZE_LRC     1       /*!< Size of LRC field in PDU. */
#define MB_SER_PDU_ADDR_OFF     0       /*!< Offset of slave address in Ser-PDU. */
#define MB_SER_PDU_PDU_OFF      1       /*!< Offset of Modbus-PDU in Ser-PDU. */

/* ----------------------- Static functions ---------------------------------*/
static uint8_t    prvucMBCHAR2BIN( uint8_t ucCharacter );

static uint8_t    prvucMBBIN2CHAR( uint8_t ucByte );

static uint8_t    prvucMBLRC( uint8_t * pucFrame, uint16_t usLen );

/* ----------------------- Start implementation -----------------------------*/
eMBErrorCode
eMBASCIIInit( struct xMBInstance * xInstance, uint8_t ucSlaveAddress,
              uint8_t ucPort, uint32_t ulBaudRate, eMBParity eParity )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    ( void )ucSlaveAddress;

    ENTER_CRITICAL_SECTION( xInstance );
    xInstance->ucMBLFCharacter = MB_ASCII_DEFAULT_LF;

    if( xMBPortSerialInit( xInstance, ucPort, ulBaudRate, 7, eParity ) != true )
    {
        eStatus = MB_EPORTERR;
    }
    else if( xMBPortTimersInit( xInstance, MB_ASCII_TIMEOUT_SEC * 20000UL ) != true )
    {
        eStatus = MB_EPORTERR;
    }

    EXIT_CRITICAL_SECTION( xInstance );

    return eStatus;
}

void
eMBASCIIStart( struct xMBInstance * xInstance )
{
    ENTER_CRITICAL_SECTION( xInstance );
    vMBPortSerialEnable( xInstance, true, false );
    xInstance->eRcvState = STATE_RX_IDLE;
    xInstance->eSndState = STATE_TX_IDLE;
    EXIT_CRITICAL_SECTION( xInstance );

    /* No special startup required for ASCII. */
    ( void )xMBPortEventPost( xInstance, EV_READY );
}

void
eMBASCIIStop( struct xMBInstance * xInstance )
{
    ENTER_CRITICAL_SECTION( xInstance );
    vMBPortSerialEnable( xInstance, false, false );
    vMBPortTimersDisable( xInstance );
    EXIT_CRITICAL_SECTION( xInstance );
}

eMBErrorCode
eMBASCIIReceive( struct xMBInstance * xInstance, uint8_t * pucRcvAddress,
                 uint8_t ** pucFrame, uint16_t * pusLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;

    ENTER_CRITICAL_SECTION( xInstance );
    assert( xInstance->usRcvBufferPos < MB_SER_SIZE_MAX );

    /* Length and CRC check */
    if( ( xInstance->usRcvBufferPos >= MB_SER_PDU_SIZE_MIN )
        && ( prvucMBLRC( ( uint8_t * ) xInstance->ucBuf, xInstance->usRcvBufferPos ) == 0 ) )
    {
        /* Save the address field. All frames are passed to the upper layed
         * and the decision if a frame is used is done there.
         */
        *pucRcvAddress = xInstance->ucBuf[MB_SER_PDU_ADDR_OFF];

        /* Total length of Modbus-PDU is Modbus-Serial-Line-PDU minus
         * size of address field and CRC checksum.
         */
        *pusLength = ( uint16_t )( xInstance->usRcvBufferPos - MB_SER_PDU_PDU_OFF - MB_SER_PDU_SIZE_LRC );

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
eMBASCIISend( struct xMBInstance * xInstance, uint8_t ucSlaveAddress,
              const uint8_t * pucFrame, uint16_t usLength )
{
    eMBErrorCode    eStatus = MB_ENOERR;
    uint8_t           usLRC;

    ENTER_CRITICAL_SECTION( xInstance );
    /* Check if the receiver is still in idle state. If not we where too
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

        /* Calculate LRC checksum for Modbus-Serial-Line-PDU. */
        usLRC = prvucMBLRC( ( uint8_t * ) xInstance->pucSndBufferCur, xInstance->usSndBufferCount );
        xInstance->ucBuf[xInstance->usSndBufferCount++] = usLRC;

        /* Activate the transmitter. */
        xInstance->eSndState = STATE_TX_START;
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
xMBASCIIReceiveFSM( struct xMBInstance * xInstance )
{
    bool            xNeedPoll = false;
    uint8_t           ucByte;
    uint8_t           ucResult;

    assert( xInstance->eSndState == STATE_TX_IDLE );

    ( void )xMBPortSerialGetByte( xInstance, ( uint8_t * ) & ucByte );
    switch ( xInstance->eRcvState )
    {
        /* A new character is received. If the character is a ':' the input
         * buffer is cleared. A CR-character signals the end of the data
         * block. Other characters are part of the data block and their
         * ASCII value is converted back to a binary representation.
         */
    case STATE_RX_RCV:
        /* Enable timer for character timeout. */
        vMBPortTimersEnable( xInstance );
        if( ucByte == ':' )
        {
            /* Empty receive buffer. */
            xInstance->eBytePos = BYTE_HIGH_NIBBLE;
            xInstance->usRcvBufferPos = 0;
        }
        else if( ucByte == MB_ASCII_DEFAULT_CR )
        {
            xInstance->eRcvState = STATE_RX_WAIT_EOF;
        }
        else
        {
            ucResult = prvucMBCHAR2BIN( ucByte );
            switch ( xInstance->eBytePos )
            {
                /* High nibble of the byte comes first. We check for
                 * a buffer overflow here. */
            case BYTE_HIGH_NIBBLE:
                if( xInstance->usRcvBufferPos < MB_SER_SIZE_MAX )
                {
                    xInstance->ucBuf[xInstance->usRcvBufferPos] = ( uint8_t )( ucResult << 4 );
                    xInstance->eBytePos = BYTE_LOW_NIBBLE;
                    break;
                }
                else
                {
                    /* not handled in Modbus specification but seems
                     * a resonable implementation. */
                    xInstance->eRcvState = STATE_RX_IDLE;
                    /* Disable previously activated timer because of error state. */
                    vMBPortTimersDisable( xInstance );
                }
                break;

            case BYTE_LOW_NIBBLE:
                xInstance->ucBuf[xInstance->usRcvBufferPos] |= ucResult;
                xInstance->usRcvBufferPos++;
                xInstance->eBytePos = BYTE_HIGH_NIBBLE;
                break;
            }
        }
        break;

    case STATE_RX_WAIT_EOF:
        if( ucByte == xInstance->ucMBLFCharacter )
        {
            /* Disable character timeout timer because all characters are
             * received. */
            vMBPortTimersDisable( xInstance );
            /* Receiver is again in idle state. */
            xInstance->eRcvState = STATE_RX_IDLE;

            /* Notify the caller of eMBASCIIReceive that a new frame
             * was received. */
            xNeedPoll = xMBPortEventPost( xInstance, EV_FRAME_RECEIVED );
        }
        else if( ucByte == ':' )
        {
            /* Empty receive buffer and back to receive state. */
            xInstance->eBytePos = BYTE_HIGH_NIBBLE;
            xInstance->usRcvBufferPos = 0;
            xInstance->eRcvState = STATE_RX_RCV;

            /* Enable timer for character timeout. */
            vMBPortTimersEnable( xInstance );
        }
        else
        {
            /* Frame is not okay. Delete entire frame. */
            xInstance->eRcvState = STATE_RX_IDLE;
        }
        break;

    case STATE_RX_IDLE:
        if( ucByte == ':' )
        {
            /* Enable timer for character timeout. */
            vMBPortTimersEnable( xInstance );
            /* Reset the input buffers to store the frame. */
            xInstance->usRcvBufferPos = 0;;
            xInstance->eBytePos = BYTE_HIGH_NIBBLE;
            xInstance->eRcvState = STATE_RX_RCV;
        }
        break;
    default:
        assert( ( xInstance->eRcvState == STATE_RX_RCV ) ||  ( xInstance->eRcvState == STATE_RX_WAIT_EOF ) ||
                ( xInstance->eRcvState == STATE_RX_IDLE ) );
    }

    return xNeedPoll;
}

bool
xMBASCIITransmitFSM( struct xMBInstance * xInstance )
{
    bool            xNeedPoll = false;
    uint8_t           ucByte;

    assert( xInstance->eRcvState == STATE_RX_IDLE );
    switch ( xInstance->eSndState )
    {
        /* Start of transmission. The start of a frame is defined by sending
         * the character ':'. */
    case STATE_TX_START:
        ucByte = ':';
        xMBPortSerialPutByte( xInstance, ( int8_t )ucByte );
        xInstance->eSndState = STATE_TX_DATA;
        xInstance->eBytePos = BYTE_HIGH_NIBBLE;
        break;

        /* Send the data block. Each data byte is encoded as a character hex
         * stream with the high nibble sent first and the low nibble sent
         * last. If all data bytes are exhausted we send a '\r' character
         * to end the transmission. */
    case STATE_TX_DATA:
        if( xInstance->usSndBufferCount > 0 )
        {
            switch ( xInstance->eBytePos )
            {
            case BYTE_HIGH_NIBBLE:
                ucByte = prvucMBBIN2CHAR( ( uint8_t )( *xInstance->pucSndBufferCur >> 4 ) );
                xMBPortSerialPutByte( xInstance, ( int8_t ) ucByte );
                xInstance->eBytePos = BYTE_LOW_NIBBLE;
                break;

            case BYTE_LOW_NIBBLE:
                ucByte = prvucMBBIN2CHAR( ( uint8_t )( *xInstance->pucSndBufferCur & 0x0F ) );
                xMBPortSerialPutByte( xInstance, ( int8_t )ucByte );
                xInstance->pucSndBufferCur++;
                xInstance->eBytePos = BYTE_HIGH_NIBBLE;
                xInstance->usSndBufferCount--;
                break;
            }
        }
        else
        {
            xMBPortSerialPutByte( xInstance, MB_ASCII_DEFAULT_CR );
            xInstance->eSndState = STATE_TX_END;
        }
        break;

        /* Finish the frame by sending a LF character. */
    case STATE_TX_END:
        xMBPortSerialPutByte( xInstance, ( int8_t )xInstance->ucMBLFCharacter );
        /* We need another state to make sure that the CR character has
         * been sent. */
        xInstance->eSndState = STATE_TX_NOTIFY;
        break;

        /* Notify the task which called eMBASCIISend that the frame has
         * been sent. */
    case STATE_TX_NOTIFY:
        xInstance->eSndState = STATE_TX_IDLE;
        xNeedPoll = xMBPortEventPost( xInstance, EV_FRAME_SENT );

        /* Disable transmitter. This prevents another transmit buffer
         * empty interrupt. */
        vMBPortSerialEnable( xInstance, true, false );
        xInstance->eSndState = STATE_TX_IDLE;
        break;

        /* We should not get a transmitter event if the transmitter is in
         * idle state.  */
    case STATE_TX_IDLE:
        /* enable receiver/disable transmitter. */
        vMBPortSerialEnable( xInstance, true, false );
        break;
    default:
        assert( ( xInstance->eSndState == STATE_TX_START ) || ( xInstance->eSndState == STATE_TX_DATA ) ||
                ( xInstance->eSndState == STATE_TX_END ) || ( xInstance->eSndState == STATE_TX_NOTIFY ) ||
                ( xInstance->eSndState == STATE_TX_IDLE ) );
    }

    return xNeedPoll;
}

bool
xMBASCIITimerT1SExpired( struct xMBInstance * xInstance )
{
    switch ( xInstance->eRcvState )
    {
        /* If we have a timeout we go back to the idle state and wait for
         * the next frame.
         */
    case STATE_RX_RCV:
    case STATE_RX_WAIT_EOF:
        xInstance->eRcvState = STATE_RX_IDLE;
        break;

    default:
        assert( ( xInstance->eRcvState == STATE_RX_RCV ) || ( xInstance->eRcvState == STATE_RX_WAIT_EOF ) );
        break;
    }
    vMBPortTimersDisable( xInstance );

    /* no context switch required. */
    return false;
}


static          uint8_t
prvucMBCHAR2BIN( uint8_t ucCharacter )
{
    if( ( ucCharacter >= '0' ) && ( ucCharacter <= '9' ) )
    {
        return ( uint8_t )( ucCharacter - '0' );
    }
    else if( ( ucCharacter >= 'A' ) && ( ucCharacter <= 'F' ) )
    {
        return ( uint8_t )( ucCharacter - 'A' + 0x0A );
    }
    else
    {
        return 0xFF;
    }
}

static          uint8_t
prvucMBBIN2CHAR( uint8_t ucByte )
{
    if( ucByte <= 0x09 )
    {
        return ( uint8_t )( '0' + ucByte );
    }
    else if( ( ucByte >= 0x0A ) && ( ucByte <= 0x0F ) )
    {
        return ( uint8_t )( ucByte - 0x0A + 'A' );
    }
    else
    {
        /* Programming error. */
        assert( 0 );
    }
    return '0';
}


static          uint8_t
prvucMBLRC( uint8_t * pucFrame, uint16_t usLen )
{
    uint8_t           ucLRC = 0;  /* LRC char initialized */

    while( usLen-- )
    {
        ucLRC += *pucFrame++;   /* Add buffer byte without carry */
    }

    /* Return twos complement */
    ucLRC = ( uint8_t ) ( -( ( int8_t ) ucLRC ) );
    return ucLRC;
}

#endif
