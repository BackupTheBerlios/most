/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support
 * ----------------------------------------------------------------------------
 * Copyright (c) 2008, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */  
    
//------------------------------------------------------------------------------
/// \unit
///
/// !Purpose
///
/// Interface for configuration the Two Wire Interface (TWI) peripheral.
///
/// !Usage
///
/// -# Configures a TWI peripheral to operate in master mode, at the given
/// frequency (in Hz) using TWI_ConfigureMaster().
/// -# or if hardware possible, configures a TWI peripheral to operate in 
/// slave mode, at the given frequency (in Hz) using TWI_ConfigureSlave().
/// -# Sends a STOP condition on the TWI using TWI_Stop().
/// -# Starts a read operation on the TWI bus with the specified slave using
/// TWI_StartRead(). Data must then be read using TWI_ReadByte() whenever 
/// a byte is available (poll using TWI_ByteReceived()).
/// -# Starts a write operation on the TWI to access the selected slave using
/// TWI_StartWrite(). A byte of data must be provided to start the write;
/// other bytes are written next. 
/// -# Sends a byte of data to one of the TWI slaves on the bus using TWI_WriteByte().
/// This function must be called once before TWI_StartWrite() with the first byte of data
/// to send, then it shall be called repeatedly after that to send the remaining bytes.
/// -# Check if a byte has been received and can be read on the given TWI
/// peripheral using TWI_ByteReceived(). 
/// Check if a byte has been sent using TWI_ByteSent().
/// -# Check if the current transmission is complete (the STOP has been sent)
/// using TWI_TransferComplete().
/// -# Enables & disable the selected interrupts sources on a TWI peripheral
/// using TWI_EnableIt() and TWI_DisableIt().
/// -# Get current status register of the given TWI peripheral using
/// TWI_GetStatus(). Get current status register of the given TWI peripheral, but
/// masking interrupt sources which are not currently enabled using 
/// TWI_GetMaskedStatus().
//------------------------------------------------------------------------------
    
#ifndef DEV_AT91_TWI_H
#define DEV_AT91_TWI_H
    
#include "dev/arch/at91sam7x/AT91SAM7X256.h"
    
// Missing AT91C_TWI_TXRDY definition.
#ifndef AT91C_TWI_TXRDY
#define AT91C_TWI_TXRDY   AT91C_TWI_TXRDY_MASTER
#endif  /*  */
    
// Missing AT91C_TWI_TXCOMP definition.
#ifndef AT91C_TWI_TXCOMP
#define AT91C_TWI_TXCOMP  AT91C_TWI_TXCOMP_MASTER
#endif  /*  */
    
/// Returns 1 if the TXRDY bit (ready to transmit data) is set in the given
/// status register value.
#define TWI_STATUS_TXRDY(status) ((status & AT91C_TWI_TXRDY) == AT91C_TWI_TXRDY)
    
/// Returns 1 if the RXRDY bit (ready to receive data) is set in the given
/// status register value.
#define TWI_STATUS_RXRDY(status) ((status & AT91C_TWI_RXRDY) == AT91C_TWI_RXRDY)
    
/// Returns 1 if the TXCOMP bit (transfer complete) is set in the given
/// status register value.
#define TWI_STATUS_TXCOMP(status) ((status & AT91C_TWI_TXCOMP) == AT91C_TWI_TXCOMP)
    
/**
 * Configures a TWI peripheral to operate in master mode, at the given
 * frequency (in Hz). The duty cycle of the TWI clock is set to 50%.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param twck : Desired TWI clock frequency.
 * @param mck : Master clock frequency.
 */ 
extern void DEV_at91_TWI_configure_master (AT91S_TWI * pTwi, unsigned int twck, unsigned int mck);
 
#ifdef AT91C_TWI_SVEN           // TWI slave
/**
 * Configures a TWI peripheral to operate in slave mode
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern void DEV_at91_TWI_configure_slave (AT91S_TWI * pTwi, unsigned char slaveAddress);

#endif  /*  */
    
/**
 * Sends a STOP condition on the TWI.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern void DEV_at91_TWI_stop (AT91S_TWI * pTwi);
 
/**
 * Starts a read operation on the TWI bus with the specified slave, and returns
 * immediately. Data must then be read using TWI_ReadByte() whenever a byte is
 * available (poll using TWI_ByteReceived()).
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param address : Slave address on the bus.
 * @param iaddress : Optional internal address bytes.
 * @param isize : Number of internal address bytes.
 */ 
extern void DEV_at91_TWI_start_read (AT91S_TWI * pTwi, unsigned char address,
                                     unsigned int iaddress, unsigned char isize);
 
/**
 * Reads a byte from the TWI bus. The read operation must have been started
 * using TWI_StartRead() and a byte must be available (check with
 * TWI_ByteReceived()).
 * Returns the byte read.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern unsigned char DEV_at91_TWI_read_byte (AT91S_TWI * pTwi);
 
/**
 * Sends a byte of data to one of the TWI slaves on the bus. This function
 * must be called once before TWI_StartWrite() with the first byte of data
 * to send, then it shall be called repeatedly after that to send the
 * remaining bytes.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param byte : Byte to send.
 */ 
extern void DEV_at91_TWI_write_byte (AT91S_TWI * pTwi, unsigned char byte);
 
/**
 * Starts a write operation on the TWI to access the selected slave, then
 * returns immediately. A byte of data must be provided to start the write;
 * other bytes are written next.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param address : Address of slave to acccess on the bus.
 * @param iaddress : Optional slave internal address.
 * @param isize : Number of internal address bytes.
 * @param byte : First byte to send.
 */ 
extern void DEV_at91_TWI_start_write (AT91S_TWI * pTwi, unsigned char address,
                                      unsigned int iaddress, unsigned char isize,
                                      unsigned char byte);
 
/**
 * Returns 1 if a byte has been received and can be read on the given TWI
 * peripheral; otherwise, returns 0. This function resets the status register
 * of the TWI.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern unsigned char DEV_at91_TWI_byte_received (AT91S_TWI * pTwi);
 
/**
 * Returns 1 if a byte has been sent, so another one can be stored for
 * transmission; otherwise returns 0. This function clears the status register
 * of the TWI.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern unsigned char DEV_at91_TWI_byte_sent (AT91S_TWI * pTwi);
 
/**
 * Returns 1 if the current transmission is complete (the STOP has been sent);
 * otherwise returns 0.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern unsigned char DEV_at91_TWI_transfer_complete (AT91S_TWI * pTwi);
 
/**
 * Enables the selected interrupts sources on a TWI peripheral.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param sources : Bitwise OR of selected interrupt sources.
 */ 
extern void DEV_at91_TWI_enable_IT (AT91S_TWI * pTwi, unsigned int sources);
 
/**
 * Disables the selected interrupts sources on a TWI peripheral.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 * @param sources : Bitwise OR of selected interrupt sources.
 */ 
extern void DEV_at91_TWI_disable_IT (AT91S_TWI * pTwi, unsigned int sources);
 
/**
 * Returns the current status register of the given TWI peripheral. This
 * resets the internal value of the status register, so further read may yield
 * different values.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern unsigned int DEV_at91_TWI_get_status (AT91S_TWI * pTwi);
 
/**
 * Returns the current status register of the given TWI peripheral, but
 * masking interrupt sources which are not currently enabled.
 * This resets the internal value of the status register, so further read may
 * yield different values.
 * @param pTwi  Pointer to an AT91S_TWI instance.
 */ 
extern unsigned int DEV_at91_TWI_get_masked_status (AT91S_TWI * pTwi);
 
/**
 * Sends a STOP condition. STOP Condition is sent just after completing
 * the current byte transmission in master read mode.
 * @param pTwi : Pointer to an AT91S_TWI instance.
 */ 
extern void DEV_at91_TWI_send_stop_condition (AT91S_TWI * pTwi);
 
#endif // DEV_AT91_TWI_H
