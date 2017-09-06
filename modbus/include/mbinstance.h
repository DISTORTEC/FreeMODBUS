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
 */

#ifndef _MBINSTANCE_H
#define _MBINSTANCE_H

#include "mbbytepos.h"
#include "mbconfig.h"
#include "mbrcvstate.h"
#include "mbsndstate.h"

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define MB_SER_SIZE_MAX     256     /*!< Maximum size of a Modbus frame. */

/* ----------------------- Type definitions ---------------------------------*/
/*! \ingroup modbus
 * \brief Struct representing Modbus instance.
 */
struct xMBInstance
{
    #if ( MB_ASCII_ENABLED > 0 ) || ( MB_RTU_ENABLED > 0 )
    volatile uint8_t ucBuf[MB_SER_SIZE_MAX];
    #endif

    #if ( MB_ASCII_ENABLED > 0 ) || ( MB_RTU_ENABLED > 0 )
    volatile uint8_t *pucSndBufferCur;
    #endif

    #if ( MB_ASCII_ENABLED > 0 ) || ( MB_RTU_ENABLED > 0 )
    volatile eMBRcvState eRcvState;
    volatile eMBSndState eSndState;
    #endif
    #if MB_ASCII_ENABLED > 0
    volatile eMBBytePos eBytePos;
    #endif

    #if ( MB_ASCII_ENABLED > 0 ) || ( MB_RTU_ENABLED > 0 )
    volatile uint16_t usSndBufferCount;
    volatile uint16_t usRcvBufferPos;
    #endif

    #if MB_ASCII_ENABLED > 0
    volatile uint8_t ucMBLFCharacter;
    #endif
};

#ifdef __cplusplus
}
#endif
#endif
