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
 */

#ifndef _PORTINSTANCE_H
#define _PORTINSTANCE_H

#include "port.h"

#include "mbconfig.h"
#include "mbeventtype.h"
#include "mbinstance.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <termios.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ----------------------- Defines ------------------------------------------*/
#define REG_INPUT_START 1000
#define REG_INPUT_NREGS 4
#define REG_HOLDING_START 2000
#define REG_HOLDING_NREGS 130

#if MB_ASCII_ENABLED == 1
#define BUF_SIZE    513         /* must hold a complete ASCII frame. */
#else
#define BUF_SIZE    256         /* must hold a complete RTU frame. */
#endif

#define PORTINSTANCE_INITIALIZER { \
    {}, \
    NULL, \
    PTHREAD_MUTEX_INITIALIZER, \
    PTHREAD_MUTEX_INITIALIZER, \
    {}, \
    {}, \
    0, \
    MB_LOG_DEBUG, \
    0, \
    -1, \
    0, \
    0, \
    0, \
    0, \
    {}, \
    REG_HOLDING_START, \
    {}, \
    REG_INPUT_START, \
    {}, \
    false, \
    false, \
    false, \
    false, \
    false, \
}

/* ----------------------- Type definitions ---------------------------------*/
typedef enum
{
    STOPPED,
    RUNNING,
    SHUTDOWN
} eThreadState;

struct PortInstance
{
    struct xMBInstance xInstance;

    FILE *fLogFile ;

    pthread_mutex_t xLock;
    pthread_mutex_t xLockOther;

    struct termios xOldTIO;

    struct timeval xTimeLast;

    eMBEventType eQueuedEvent;
    eMBPortLogLevel eLevelMax;
    eThreadState ePollThreadState;

    int iSerialFd;
    int uiRxBufferPos;
    int uiTxBufferPos;

    uint32_t ulTimeOut;
    uint32_t ulTimeoutMs;

    uint16_t usRegHoldingBuf[REG_HOLDING_NREGS];
    uint16_t usRegHoldingStart;
    uint16_t usRegInputBuf[REG_INPUT_NREGS];
    uint16_t usRegInputStart;

    uint8_t ucBuffer[BUF_SIZE];

    bool bTimeoutEnable;
    bool bRxEnabled;
    bool bTxEnabled;
    bool bDoExit;
    bool xEventInQueue;
};

#ifdef __cplusplus
}
#endif
#endif
