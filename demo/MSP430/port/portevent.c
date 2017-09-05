/*
 * FreeModbus Libary: MSP430 Port
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
 * File: $Id: portevent.c,v 1.1 2006/11/19 03:00:34 wolti Exp $
 */

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

/* ----------------------- Variables ----------------------------------------*/
static eMBEventType eQueuedEvent;
static bool     xEventInQueue;

/* ----------------------- Start implementation -----------------------------*/
bool
xMBPortEventInit( void )
{
    xEventInQueue = false;
    return true;
}

bool
xMBPortEventPost( eMBEventType eEvent )
{
    xEventInQueue = true;
    eQueuedEvent = eEvent;
    return true;
}

bool
xMBPortEventGet( eMBEventType * eEvent )
{
    bool            xEventHappened = false;

    if( xEventInQueue )
    {
        *eEvent = eQueuedEvent;
        xEventInQueue = false;
        xEventHappened = true;
    }
    return xEventHappened;
}
