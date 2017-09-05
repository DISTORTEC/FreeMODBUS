/*
 * FreeModbus Libary: MCF5235 Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 * Parts of crt0.S Copyright (c) 1995, 1996, 1998 Cygnus Support
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
 * File: $Id: portserial.c,v 1.1 2007/02/19 00:56:14 wolti Exp $
 */

/* ----------------------- System includes --------------------------------- */
#include "port.h"
#include "m523xbcc.h"

/* ----------------------- Modbus includes --------------------------------- */
#include "mb.h"
#include "mbport.h"

/* ----------------------- Defines ----------------------------------------- */
#define BAUDRATE_VALUE(fsys, baud)  ( ( fsys )/(32UL * baud) )

/* ----------------------- Static variables -------------------------------- */
bool            bTXEnabled;
bool            bRXEnabled;

/* ----------------------- Start implementation ---------------------------- */

void
vMBPortSerialEnable( bool xRxEnable, bool xTxEnable )
{
    uint8_t           ucUCR = 0;
    uint8_t           ucIMR = 0;

    if( xRxEnable )
    {
        ucUCR |= MCF_UART_UCR_RXC( 0x1 );
        ucIMR |= MCF_UART_UIMR_RXRDY_FU;
        bRXEnabled = true;
    }
    else
    {
        ucUCR |= MCF_UART_UCR_RXC( 0x2 );
        bRXEnabled = false;
    }
    if( xTxEnable )
    {
        ucUCR |= MCF_UART_UCR_TXC( 0x1 );
        ucIMR |= MCF_UART_UIMR_TXRDY;
        bTXEnabled = true;
    }
    else
    {
        ucUCR |= MCF_UART_UCR_TXC( 0x2 );
        bTXEnabled = false;
    }
    MCF_UART_UCR0 = ucUCR;
    MCF_UART_UIMR0 = ucIMR;
}

bool
xMBPortSerialInit( uint8_t ucPort, uint32_t ulBaudRate, uint8_t ucDataBits, eMBParity eParity )
{
    bool            bStatus = true;
    uint8_t           ucMode = 0;

    ( void )ucPort;

    switch ( eParity )
    {
    case MB_PAR_EVEN:
        ucMode |= MCF_UART_UMR_PM( 0x0 );
        break;
    case MB_PAR_ODD:
        ucMode |= MCF_UART_UMR_PM( 0x0 ) | MCF_UART_UMR_PT;
        break;
    case MB_PAR_NONE:
        ucMode |= MCF_UART_UMR_PM( 0x3 );
        break;
    default:
        bStatus = false;
    }

    switch ( ucDataBits )
    {
    case 8:
        ucMode |= MCF_UART_UMR_BC( 0x3 );
        break;
    case 7:
        ucMode |= MCF_UART_UMR_BC( 0x2 );
        break;
    default:
        bStatus = false;
    }
    if( bStatus == true )
    {
        /* UART 0: Reset transmitter, receiver and mode register pointer */
        MCF_UART_UCR0 = MCF_UART_UCR_MISC( 0x3 );
        MCF_UART_UCR0 = MCF_UART_UCR_MISC( 0x2 );
        MCF_UART_UCR0 = MCF_UART_UCR_MISC( 0x1 );

        /* UART 0: Configure parity, databits and stopbits. */
        MCF_UART_UMR0 = ucMode;
        MCF_UART_UMR0 = MCF_UART_UMR_SB( 0x7 );

        /* UART 0 Clocking */
        MCF_UART_UCSR0 = MCF_UART_UCSR_RCS( 0xd ) | MCF_UART_UCSR_TCS( 0xd );
        MCF_UART_UBG10 = ( uint8_t ) ( BAUDRATE_VALUE( FSYS_2, ulBaudRate ) >> 8U );
        MCF_UART_UBG20 = ( uint8_t ) ( BAUDRATE_VALUE( FSYS_2, ulBaudRate ) & 0xFFU );

        /* UART 0: Enable interrupts */
        MCF_INTC0_ICR13 = MCF_INTC0_ICRn_IL( 0x2 ) | MCF_INTC0_ICRn_IP( 0x1 );
        MCF_INTC0_IMRL &= ~MCF_INTC0_IMRL_INT_MASK13;

        /* UART 0 Miscellaneous */
        MCF_UART_UACR0 = 0;

        /* UART 0: Enable pins */
        MCF_GPIO_PAR_UART = MCF_GPIO_PAR_UART_PAR_U0RXD | MCF_GPIO_PAR_UART_PAR_U0TXD;
        vMBPortSerialEnable( false, false );
    }

    return true;
}

bool
xMBPortSerialPutByte( CHAR ucByte )
{
    MCF_UART_UTB0 = ( uint8_t ) ucByte;
    return true;
}

bool
xMBPortSerialGetByte( CHAR * pucByte )
{
    *pucByte = ( CHAR ) MCF_UART_URB0;
    return true;
}

__declspec( interrupt )
     void            prvvMBPortSerialISR(  )
{
    if( bTXEnabled && ( MCF_UART_UISR0 & MCF_UART_UISR_TXRDY ) )
    {
        ( void )pxMBFrameCBTransmitterEmpty(  );
    }
    if( bRXEnabled && ( MCF_UART_UISR0 & MCF_UART_UISR_RXRDY_FU ) )
    {
        ( void )pxMBFrameCBByteReceived(  );
    }
}
