#
# file: FreeMODBUS-sources.cmake
#
# author: Copyright (C) 2019 Kamil Szczygiel https://distortec.com https://freddiechopin.info
#
# This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
# distributed with this file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

add_library(FreeMODBUS STATIC
		${CMAKE_CURRENT_LIST_DIR}/modbus/ascii/mbascii.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfunccoils.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncdiag.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncdisc.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncholding.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncinput.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbfuncother.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/functions/mbutils.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbcrc.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/rtu/mbrtu.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/tcp/mbtcp.c
		${CMAKE_CURRENT_LIST_DIR}/modbus/mb.c)
target_compile_options(FreeMODBUS PRIVATE
		-Wno-unused-parameter)
target_include_directories(FreeMODBUS PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/modbus/ascii
		${CMAKE_CURRENT_LIST_DIR}/modbus/rtu
		${CMAKE_CURRENT_LIST_DIR}/modbus/tcp)
target_include_directories(FreeMODBUS PUBLIC
		${CMAKE_CURRENT_LIST_DIR}/modbus/include)
