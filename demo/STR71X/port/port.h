/*
 * FreeModbus Libary: STR71x Port
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
 * File: $Id: port.h,v 1.6 2006/06/15 15:27:16 wolti Exp $
 */

#ifndef _PORT_H
#define _PORT_H

#include "assert.h"
#include "FreeRTOS.h"

#include <stdbool.h>

/* work aroung a problem when inline is also defined in 71x_conf.h */
#ifdef INLINE
#undef INLINE
#endif

#define INLINE                      inline

#define ENTER_CRITICAL_SECTION( )   portENTER_CRITICAL( )
#define EXIT_CRITICAL_SECTION( )    portEXIT_CRITICAL( )

typedef unsigned char UCHAR;
typedef char    CHAR;

typedef unsigned short USHORT;

#endif
