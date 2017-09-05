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
  * File: $Id: port.h,v 1.2 2006/06/15 15:29:41 wolti Exp $
  */

#ifndef _PORT_H
#define _PORT_H

#include <stdbool.h>

#define	INLINE                    inline

#define ENTER_CRITICAL_SECTION( ) prvvPortEnterCritical()
#define EXIT_CRITICAL_SECTION( )  prvvPortExitCritical()

#define assert( x )

inline void     prvvPortEnterCritical(  );
inline void     prvvPortExitCritical(  );

#endif
