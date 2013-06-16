/* ----------------------------------------------------------------------------
 *         ATMEL Microcontroller Software Support  -  ROUSSET  -
 * ----------------------------------------------------------------------------
 * Copyright (c) 2006, Atmel Corporation

 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaiimer below.
 * 
 * - Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the disclaimer below in the documentation and/or
 * other materials provided with the distribution. 
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
/// \dir
/// !Purpose
/// 
/// Interface for configuration the %DMA controller(DMAC).
///
/// !Usage
///
/// -# Enable or disable the a DMAC controller with 
///    DMA_Enable() and or DMA_Disable().
/// -# Enable or disable %Dma interrupt using DMA_EnableIt()
///    or DMA_DisableIt().
/// -# Get %Dma interrupt status by DMA_GetStatus().
/// -# Enable or disable specified %Dma channel with 
///    DMA_EnableChannel() or DMA_DisableChannel().
/// -# Get %Dma channel status by DMA_GetChannelStatus().
/// -# Configure source and/or destination start address with
///    DMA_SetSourceAddr() and/or DMA_SetDestAddr().
/// -# Set %Dma descriptor address using DMA_SetDescriptorAddr().
/// -# Set source transfer buffer size with DMA_SetSourceBufferSize().
/// -# Configure source and/or destination transfer mode with
///    DMA_SetSourceBufferMode() and/or DMA_SetDestBufferMode().
/// -# Configure source and/or destination Picture-In-Picutre 
///    mode with DMA_SPIPconfiguration() and/or DMA_DPIPconfiguration().
//------------------------------------------------------------------------------
    
#ifndef DEV_AT91_DMA_H
#define DEV_AT91_DMA_H
    
#define DMA_CHANNEL_0            0
#define DMA_CHANNEL_1            1
#define DMA_CHANNEL_2            2
#define DMA_CHANNEL_3            3
#define DMA_CHANNEL_4            4
#define DMA_CHANNEL_5            5
#define DMA_CHANNEL_6            6
#define DMA_CHANNEL_7            7
    
#if defined(at91sam9rl64)
#define DMA_CHANNEL_NUM            (DMA_CHANNEL_1 + 1)
#elif defined(at91sam9m10)
#define DMA_CHANNEL_NUM            (DMA_CHANNEL_7 + 1)
#endif  /*  */
    
#define DMA_TRANSFER_SIGNLE      0
#define DMA_TRANSFER_LLI         1  
#define DMA_TRANSFER_RELOAD      2
#define DMA_TRANSFER_CONTIGUOUS  3
     
#define DMA_ENA                  (1 << 0)
#define DMA_DIS                  (1 << 0)
#define DMA_SUSP                 (1 << 8)
#define DMA_KEEPON               (1 << 24)
    
#define DMA_BTC                  (1 << 0)
#define DMA_CBTC                 (1 << 8)
#define DMA_ERR                  (1 << 16)
    
#if defined(at91sam9m10)
#define AT91C_SRC_DSCR AT91C_HDMA_SRC_DSCR
#define AT91C_DST_DSCR AT91C_HDMA_DST_DSCR
#define AT91C_SRC_INCR AT91C_HDMA_SRC_ADDRESS_MODE
#define AT91C_DST_INCR AT91C_HDMA_DST_ADDRESS_MODE
#define AT91C_SRC_PER  AT91C_HDMA_SRC_PER
#define AT91C_DST_PER  AT91C_HDMA_DST_PER
#define AT91C_SRC_REP  AT91C_HDMA_SRC_REP
#define AT91C_DST_REP  AT91C_HDMA_DST_REP
#define AT91C_SRC_PIP  AT91C_HDMA_SRC_PIP
#define AT91C_DST_PIP  AT91C_HDMA_DST_PIP
    
#define AT91C_BTC             (0xFF <<  0) 
#define AT91C_CBTC            (0xFF <<  8) 
#define AT91C_ERR             (0xFF << 16) 
#endif  /*  */
     
/**
 * Enables a DMAC peripheral
 */ 
extern void DEV_at91_DMA_enable (void);
 
 /**
  * Disables DMAC peripheral
  */ 
extern void DEV_at91_DMA_disable (void);
 
/**
 * Enables DMAC channel
 * @param channel : Particular channel number.
 */ 
extern void DEV_at91_DMA_enable_channel (unsigned int channel);
 
/**
 * Disables a DMAC channel
 * @param channel : Particular channel number.
 */ 
extern void DEV_at91_DMA_disable_channel (unsigned int channel);
 
/**
 * Resume DMAC channel from an stall state.
 * @param channel : Particular channel number.
 */ 
extern void DEV_at91_DMA_keepon_channel (unsigned int channel);
 
/**
 * Clear automatic mode for multi-buffer transfer.
 * @param channel : Particular channel number.
 */ 
extern void DEV_at91_DMA_clear_auto_mode (unsigned int channel);
 
/**
 * Return DMAC channel status
 */ 
extern unsigned int DEV_at91_DMA_get_channel_status (void);
 
/**
 * Return DMA Interrupt Status
 */ 
extern unsigned int DEV_at91_DMA_get_status (void);
 
/**
 * Return DMA Interrupt Mask Status
 */ 
extern unsigned int DEV_at91_DMA_get_interrupt_mask (void);
 
/**
 * Returns the current status register of the given DMA peripheral, but
 * masking interrupt sources which are not currently enabled.
 */ 
extern unsigned int DEV_at91_DMA_get_masked_status (void);
 
/**
 * Enable DMA interrupt
 * @param flag : IT to be enabled
 */ 
extern void DEV_at91_DMA_enable_IT (unsigned int flag);
 
/**
 * Disable DMA interrupt
 * @param flag : IT to be enabled
 */ 
extern void DEV_at91_DMA_disable_IT (unsigned int flag);
 
/**
 * Set DMA source address used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param sources : sources address.
 */ 
extern void DEV_at91_DMA_set_source_addr (unsigned char channel, unsigned int address);
 
/**
 * Set DMA destination address used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param sources : destination address.
 */ 
extern void DEV_at91_DMA_set_destination_addr (unsigned char channel, unsigned int address);
 
/**
 * Set DMA descriptor address used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param sources : destination address.
 */ 
extern void DEV_at91_DMA_set_descriptor_addr (unsigned char channel, unsigned int address);
 
/**
 * Set DMA control A register used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param size : Dma transfer size in byte.
 * @param sourceWidth : Single transfer width for source.
 * @param destWidth : Single transfer width for destination.
 * @param done : Transfer done field.
 */ 
extern void DEV_at91_DMA_set_source_buffer_size (unsigned char channel, unsigned int size,
                                                 unsigned char sourceWidth,
                                                 unsigned char desDMAdth, unsigned char done);
 
/**
 * Set DMA transfer mode for source used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param transferMode : Transfer buffer mode (single, LLI, reload or contiguous)
 * @param addressingType : Type of addrassing mode
 *                        0 : incrementing, 1: decrementing, 2: fixed.
 */ 
extern void DEV_at91_DMA_set_source_buffer_mode (unsigned char channel, unsigned char transferMode,
                                                 unsigned char addressingType);
  
/**
 * Set DMA transfer mode for destination used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param transferMode : Transfer buffer mode (single, LLI, reload or contiguous)
 * @param addressingType : Type of addrassing mode
 *                        0 : incrementing, 1: decrementing, 2: fixed.
 */ 
extern void DEV_at91_DMA_set_dest_buffer_mode (unsigned char channel, unsigned char transferMode,
                                               unsigned char addressingType);
 
/**
 * Set DMA configuration registe used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param value : configuration value.
 */ 
extern void DEV_at91_DMA_set_configuration (unsigned char channel, unsigned int value);
 
/**
 * Set DMA source PIP configuration used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param pipHole : stop on done mode.
 * @param pipBoundary : lock mode.
 */ 
extern void DEV_at91_DMA_spip_configuration (unsigned char channel, unsigned int pipHole,
                                             unsigned int pipBoundary);
 
/**
 * Set DMA destination PIP configuration used by a HDMA channel.
 * @param channel : Particular channel number.
 * @param pipHole : stop on done mode.
 * @param pipBoundary : lock mode.
 */ 
extern void DEV_at91_DMA_dpip_configuration (unsigned char channel, unsigned int pipHole,
                                             unsigned int pipBoundary);
 
#endif //DEV_AT91_DMA_H
