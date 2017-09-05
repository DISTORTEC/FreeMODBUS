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
 * File: $Id: mbascii.h,v 1.8 2006/12/07 22:10:34 wolti Exp $
 */

#ifndef _MB_ASCII_H
#define _MB_ASCII_H

#include "mbconfig.h"
#include "mberrorcode.h"
#include "mbparity.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#if MB_ASCII_ENABLED > 0
eMBErrorCode    eMBASCIIInit( uint8_t slaveAddress, uint8_t ucPort,
                              uint32_t ulBaudRate, eMBParity eParity );
void            eMBASCIIStart( void );
void            eMBASCIIStop( void );

eMBErrorCode    eMBASCIIReceive( uint8_t * pucRcvAddress, uint8_t ** pucFrame,
                                 uint16_t * pusLength );
eMBErrorCode    eMBASCIISend( uint8_t slaveAddress, const uint8_t * pucFrame,
                              uint16_t usLength );
bool            xMBASCIIReceiveFSM( void );
bool            xMBASCIITransmitFSM( void );
bool            xMBASCIITimerT1SExpired( void );
#endif

#ifdef __cplusplus
}
#endif
#endif
