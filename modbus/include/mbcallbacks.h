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

#ifndef _MB_CALLBACKS_H
#define _MB_CALLBACKS_H

#include "mberrorcode.h"
#include "mbregistermode.h"

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct xMBInstance;

/*! \defgroup modbus_registers Modbus Registers
 * \code #include "mbcallbacks.h" \endcode
 * The protocol stack does not internally allocate any memory for the
 * registers. This makes the protocol stack very small and also usable on
 * low end targets. In addition the values don't have to be in the memory
 * and could for example be stored in a flash.<br>
 * Whenever the protocol stack requires a value it calls one of the callback
 * function with the register address and the number of registers to read
 * as an argument. The application should then read the actual register values
 * (for example the ADC voltage) and should store the result in the supplied
 * buffer.<br>
 * If the protocol stack wants to update a register value because a write
 * register function was received a buffer with the new register values is
 * passed to the callback function. The function should then use these values
 * to update the application register values.
 */

/* ----------------------- Type definitions ---------------------------------*/
/*! \ingroup modbus
 * \brief Struct containing Modbus callbacks.
 */
struct xMBCallbacks
{
    /*! \ingroup modbus_registers
     * \brief Callback function used if the value of a <em>Input Register</em>
     *   is required by the protocol stack. The starting register address is given
     *   by \c usAddress and the last register is given by <tt>usAddress +
     *   usNRegs - 1</tt>.
     *
     * \param xInstance The pointer to instance struct.
     * \param pucRegBuffer A buffer where the callback function should write
     *   the current value of the modbus registers to.
     * \param usAddress The starting address of the register. Input registers
     *   are in the range 1 - 65535.
     * \param usNRegs Number of registers the callback function must supply.
     *
     * \return The function must return one of the following error codes:
     *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
     *       Modbus response is sent.
     *   - eMBErrorCode::MB_ENOREG If the application can not supply values
     *       for registers within this range. In this case a
     *       <b>ILLEGAL DATA ADDRESS</b> exception frame is sent as a response.
     *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
     *       currently not available and the application dependent response
     *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
     *       exception is sent as a response.
     *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
     *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
     */
    eMBErrorCode( *eMBRegInputCB ) ( struct xMBInstance * xInstance,
                                     uint8_t * pucRegBuffer, uint16_t usAddress,
                                     uint16_t usNRegs );

    /*! \ingroup modbus_registers
     * \brief Callback function used if a <em>Holding Register</em> value is
     *   read or written by the protocol stack. The starting register address
     *   is given by \c usAddress and the last register is given by
     *   <tt>usAddress + usNRegs - 1</tt>.
     *
     * \param xInstance The pointer to instance struct.
     * \param pucRegBuffer If the application registers values should be updated the
     *   buffer points to the new registers values. If the protocol stack needs
     *   to now the current values the callback function should write them into
     *   this buffer.
     * \param usAddress The starting address of the register.
     * \param usNRegs Number of registers to read or write.
     * \param eMode If eMBRegisterMode::MB_REG_WRITE the application register
     *   values should be updated from the values in the buffer. For example
     *   this would be the case when the Modbus master has issued an
     *   <b>WRITE SINGLE REGISTER</b> command.
     *   If the value eMBRegisterMode::MB_REG_READ the application should copy
     *   the current values into the buffer \c pucRegBuffer.
     *
     * \return The function must return one of the following error codes:
     *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
     *       Modbus response is sent.
     *   - eMBErrorCode::MB_ENOREG If the application can not supply values
     *       for registers within this range. In this case a
     *       <b>ILLEGAL DATA ADDRESS</b> exception frame is sent as a response.
     *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
     *       currently not available and the application dependent response
     *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
     *       exception is sent as a response.
     *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
     *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
     */
    eMBErrorCode( *eMBRegHoldingCB ) ( struct xMBInstance * xInstance,
                                       uint8_t * pucRegBuffer, uint16_t usAddress,
                                       uint16_t usNRegs, eMBRegisterMode eMode );

    /*! \ingroup modbus_registers
     * \brief Callback function used if a <em>Coil Register</em> value is
     *   read or written by the protocol stack. If you are going to use
     *   this function you might use the functions xMBUtilSetBits(  ) and
     *   xMBUtilGetBits(  ) for working with bitfields.
     *
     * \param xInstance The pointer to instance struct.
     * \param pucRegBuffer The bits are packed in bytes where the first coil
     *   starting at address \c usAddress is stored in the LSB of the
     *   first byte in the buffer <code>pucRegBuffer</code>.
     *   If the buffer should be written by the callback function unused
     *   coil values (I.e. if not a multiple of eight coils is used) should be set
     *   to zero.
     * \param usAddress The first coil number.
     * \param usNCoils Number of coil values requested.
     * \param eMode If eMBRegisterMode::MB_REG_WRITE the application values should
     *   be updated from the values supplied in the buffer \c pucRegBuffer.
     *   If eMBRegisterMode::MB_REG_READ the application should store the current
     *   values in the buffer \c pucRegBuffer.
     *
     * \return The function must return one of the following error codes:
     *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
     *       Modbus response is sent.
     *   - eMBErrorCode::MB_ENOREG If the application does not map an coils
     *       within the requested address range. In this case a
     *       <b>ILLEGAL DATA ADDRESS</b> is sent as a response.
     *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
     *       currently not available and the application dependent response
     *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
     *       exception is sent as a response.
     *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
     *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
     */
    eMBErrorCode( *eMBRegCoilsCB ) ( struct xMBInstance * xInstance,
                                     uint8_t * pucRegBuffer, uint16_t usAddress,
                                     uint16_t usNCoils, eMBRegisterMode eMode );

    /*! \ingroup modbus_registers
     * \brief Callback function used if a <em>Input Discrete Register</em> value is
     *   read by the protocol stack.
     *
     * If you are going to use his function you might use the functions
     * xMBUtilSetBits(  ) and xMBUtilGetBits(  ) for working with bitfields.
     *
     * \param xInstance The pointer to instance struct.
     * \param pucRegBuffer The buffer should be updated with the current
     *   coil values. The first discrete input starting at \c usAddress must be
     *   stored at the LSB of the first byte in the buffer. If the requested number
     *   is not a multiple of eight the remaining bits should be set to zero.
     * \param usAddress The starting address of the first discrete input.
     * \param usNDiscrete Number of discrete input values.
     * \return The function must return one of the following error codes:
     *   - eMBErrorCode::MB_ENOERR If no error occurred. In this case a normal
     *       Modbus response is sent.
     *   - eMBErrorCode::MB_ENOREG If no such discrete inputs exists.
     *       In this case a <b>ILLEGAL DATA ADDRESS</b> exception frame is sent
     *       as a response.
     *   - eMBErrorCode::MB_ETIMEDOUT If the requested register block is
     *       currently not available and the application dependent response
     *       timeout would be violated. In this case a <b>SLAVE DEVICE BUSY</b>
     *       exception is sent as a response.
     *   - eMBErrorCode::MB_EIO If an unrecoverable error occurred. In this case
     *       a <b>SLAVE DEVICE FAILURE</b> exception is sent as a response.
     */
    eMBErrorCode( *eMBRegDiscreteCB ) ( struct xMBInstance * xInstance,
                                        uint8_t * pucRegBuffer, uint16_t usAddress,
                                        uint16_t usNDiscrete );
};

#ifdef __cplusplus
}
#endif
#endif
