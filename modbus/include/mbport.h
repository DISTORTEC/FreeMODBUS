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
 * File: $Id: mbport.h,v 1.19 2010/06/06 13:54:40 wolti Exp $
 */

#ifndef _MB_PORT_H
#define _MB_PORT_H

#include "mbeventtype.h"
#include "mbparity.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xMBInstance;

/* ----------------------- Supporting functions -----------------------------*/
bool xMBPortEventInit( struct xMBInstance * xInstance );

bool xMBPortEventPost( struct xMBInstance * xInstance, eMBEventType eEvent );

bool xMBPortEventGet( struct xMBInstance * xInstance,
                      /*@out@ */ eMBEventType * eEvent );

/* ----------------------- Serial port functions ----------------------------*/

bool xMBPortSerialInit( struct xMBInstance * xInstance,
                                   uint8_t ucPort, uint32_t ulBaudRate,
                                   uint8_t ucDataBits, eMBParity eParity );

void xMBPortSerialClose( struct xMBInstance * xInstance );

void vMBPortSerialEnable( struct xMBInstance * xInstance, bool xRxEnable,
                          bool xTxEnable );

bool xMBPortSerialGetByte( struct xMBInstance * xInstance, uint8_t * pucByte );

bool xMBPortSerialPutByte( struct xMBInstance * xInstance, uint8_t ucByte );

/* ----------------------- Timers functions ---------------------------------*/
bool xMBPortTimersInit( struct xMBInstance * xInstance,
                        uint16_t usTimeOut50us );

void xMBPortTimersClose( struct xMBInstance * xInstance );

void vMBPortTimersEnable( struct xMBInstance * xInstance );

void vMBPortTimersDisable( struct xMBInstance * xInstance );

void vMBPortTimersDelay( struct xMBInstance * xInstance,
                         uint16_t usTimeOutMS );

/* ----------------------- TCP port functions -------------------------------*/
bool xMBTCPPortInit( struct xMBInstance * xInstance, uint16_t usTCPPort );

void vMBTCPPortClose( struct xMBInstance * xInstance );

void vMBTCPPortDisable( struct xMBInstance * xInstance );

bool xMBTCPPortGetRequest( struct xMBInstance * xInstance,
                           uint8_t **ppucMBTCPFrame, uint16_t * usTCPLength );

bool xMBTCPPortSendResponse( struct xMBInstance * xInstance,
                             const uint8_t *pucMBTCPFrame,
                             uint16_t usTCPLength );

#ifdef __cplusplus
}
#endif
#endif
