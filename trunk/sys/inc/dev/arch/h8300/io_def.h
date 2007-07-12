/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_H8300_IO_DEF_H
#define DEV_H8300_IO_DEF_H

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup h8300
 *
 * @{
 */

/** @defgroup io_def io_def.h
 *
 * @{
 */

/*------------- Definitions ------------------------------------------*/

#define BIT(x)    (1  << (x))   ///< Only use for 16 bit integer.

/**
 * \defgroup _SYSTEM_CONTROL_ System control
 * @{
 */

/**  
 * @name MDCR : Mode control register
 * The defines for the bits of the registers
 * @{
 */
#define H8_MDCR_RESERVE_RO1  BIT(7)|BIT(6)      ///< Reserved: read-only
                                                // bits, always read as 1
#define H8_MDCR_RESERVE_RO0  BIT(5)|BIT(4)|BIT(3)       ///< Reserved:
                                                        // read-only bits, 
                                                        // always read as
                                                        // 0
#define H8_MDCR_MDS2         BIT(2)     ///< Mode select (read only)
#define H8_MDCR_MDS1         BIT(1)     ///< Mode select (read only)
#define H8_MDCR_MDS0         BIT(0)     ///< Mode select (read only)

#define H8_MDCR_MODE1        H8_MDCR_MDS0
///< Address space: 1 Mbyte,init bus mode: 8 bits, on-chip RAM enabled

#define H8_MDCR_MODE2        H8_MDCR_MDS1
///< Address space: 1 Mbyte, init bus mode: 16 bits, on-chip RAM enabled

#define H8_MDCR_MODE3        H8_MDCR_MDS0|H8_MDCR_MDS1
///< Address space: 16 Mbyte, init bus mode: 8 bits, on-chip RAM enabled

#define H8_MDCR_MODE4        H8_MDCR_MDS2
///< Address space: 16 Mbyte, init bus mode: 16 bits, on-chip RAM enabled
// @}

/**  
 * @name SYSCR : System control register
 * The defines for the bits of the registers
 * @{
 */
#define H8_SYSCR_SSBY        BIT(7)     ///< SLEEP instruction causes
                                        // transition to software standby
                                        // mode
#define H8_SYSCR_STS2        BIT(6)     ///< Standby timer select
#define H8_SYSCR_STS1        BIT(5)     ///< Standby timer select
#define H8_SYSCR_STS0        BIT(4)     ///< Standby timer select
#define H8_SYSCR_UE          BIT(3)     ///< User bit enable, UI bit in
                                        // CCR is used as a user bit
#define H8_SYSCR_NMIEG       BIT(2)     ///< An interrupt is requested at
                                        // the rising edge of NMI
#define H8_SYSCR_RESERVE     BIT(1)     ///< Reserved: read-only bit,
                                        // always read as 1
#define H8_SYSCR_RAME        BIT(0)     ///< On-chip RAM is enabled

#define H8_SYSCR_WT0         (0x00)     ///< Waiting time = 8192 states
#define H8_SYSCR_WT1         H8_SYSCR_STS0      ///< Waiting time = 16384
                                                // states
#define H8_SYSCR_WT2         H8_SYSCR_STS1      ///< Waiting time = 32768
                                                // states
#define H8_SYSCR_WT3         H8_SYSCR_STS0|H8_SYSCR_STS1        ///<
                                                                // Waiting 
                                                                // time =
                                                                // 65536
                                                                // states
#define H8_SYSCR_WT4         H8_SYSCR_STS2      ///< Waiting time =
                                                // 131072 states
#define H8_SYSCR_WT5         H8_SYSCR_STS1|H8_SYSCR_STS2        ///<
                                                                // Waiting 
                                                                // time =
                                                                // 4
                                                                // states
  // @}
  // @}

  /**
   * \defgroup _INTERRUPT_CONTROL_ Interrupt controller
   * @{
   */

  /**  
   * @name ISCR : interupt sense control register
   * The defines for the bits of the registers
   * @{
   */
#define H8_ISCR_IRQ7SC       BIT(7)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ7
#define H8_ISCR_IRQ6SC       BIT(6)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ6
#define H8_ISCR_IRQ5SC       BIT(5)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ5
#define H8_ISCR_IRQ4SC       BIT(4)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ4
#define H8_ISCR_IRQ3SC       BIT(3)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ3
#define H8_ISCR_IRQ2SC       BIT(2)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ2
#define H8_ISCR_IRQ1SC       BIT(1)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ1
#define H8_ISCR_IRQ0SC       BIT(0)     ///< Interrupts are requested by
                                        // falling-edge input at IRQ0
// @}

/**  
 * @name IER : interupt enable register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_IER_IRQ7E         BIT(7)     ///< IRQ7 interrupt is enabled
#define H8_IER_IRQ6E         BIT(6)     ///< IRQ6 interrupt is enabled
#define H8_IER_IRQ5E         BIT(5)     ///< IRQ5 interrupt is enabled
#define H8_IER_IRQ4E         BIT(4)     ///< IRQ4 interrupt is enabled
#define H8_IER_IRQ3E         BIT(3)     ///< IRQ3 interrupt is enabled
#define H8_IER_IRQ2E         BIT(2)     ///< IRQ2 interrupt is enabled
#define H8_IER_IRQ1E         BIT(1)     ///< IRQ1 interrupt is enabled
#define H8_IER_IRQ0E         BIT(0)     ///< IRQ0 interrupt is enabled
#define H8_IER_IRQDIS        (0x00)     ///< All IRQ interrupts disabled
// @}

/**  
 * @name ISR : interupt status register               
 * The defines for the bits of the registers
 * @{
 */
#define H8_ISR_IRQ7F         BIT(7)     ///< Indicates IRQ7 interrupt
                                        // status request
#define H8_ISR_IRQ6F         BIT(6)     ///< Indicates IRQ6 interrupt
                                        // status request
#define H8_ISR_IRQ5F         BIT(5)     ///< Indicates IRQ5 interrupt
                                        // status request
#define H8_ISR_IRQ4F         BIT(4)     ///< Indicates IRQ4 interrupt
                                        // status request
#define H8_ISR_IRQ3F         BIT(3)     ///< Indicates IRQ3 interrupt
                                        // status request
#define H8_ISR_IRQ2F         BIT(2)     ///< Indicates IRQ2 interrupt
                                        // status request
#define H8_ISR_IRQ1F         BIT(1)     ///< Indicates IRQ1 interrupt
                                        // status request
#define H8_ISR_IRQ0F         BIT(0)     ///< Indicates IRQ0 interrupt
                                        // status request
  // @}

  /**  
   * @name IPRA : interupt priority register A          
   * The defines for the bits of the registers
   * @{
   */
#define H8_IPRA_IPRA7        BIT(7)     ///< IRQ0 requests have high
                                        // priority
#define H8_IPRA_IPRA6        BIT(6)     ///< IRQ1 requests have high
                                        // priority
#define H8_IPRA_IPRA5        BIT(5)     ///< IRQ23 requests have high
                                        // priority
#define H8_IPRA_IPRA4        BIT(4)     ///< IRQ4567 requests have high
                                        // priority
#define H8_IPRA_IPRA3        BIT(3)     ///< WDT and refresh controller
                                        // requests have high priority
#define H8_IPRA_IPRA2        BIT(2)     ///< ITU0 interrupt requests have
                                        // high priority
#define H8_IPRA_IPRA1        BIT(1)     ///< ITU1 interrupt requests have
                                        // high priority
#define H8_IPRA_IPRA0        BIT(0)     ///< ITU2 interrupt requests have
                                        // high priority
  // @}

  /**  
   * @name IPRB : interupt priority register B          
   * The defines for the bits of the registers
   * @{
   */
#define H8_IPRB_IPRB7        BIT(7)     ///< ITU3 interrupt requests to
                                        // high priority
#define H8_IPRB_IPRB6        BIT(6)     ///< ITU4 interrupt requests to
                                        // high priority
#define H8_IPRB_IPRB5        BIT(5)     ///< DMA ch0 and ch1 interrupt
                                        // requests to high priority
#define H8_IPRB_IPRB4        BIT(4)     ///< DMA ch2 and ch2 interrupt
                                        // requests to high priority
#define H8_IPRB_IPRB3        BIT(3)     ///< SCI ch0 interrupt requests
                                        // to high priority
#define H8_IPRB_IPRB2        BIT(2)     ///< SCI ch1 interrupt requests
                                        // to high priority
#define H8_IPRB_IPRB1        BIT(1)     ///< A/D converter interrupt
                                        // requests to high priority
#define H8_IPRB_RESERVE      BIT(0)     ///< Reserved: can be written and
                                        // read
// @}
// @}

/**
 * \defgroup _BUS_CONTROL_ Bus-controller
 * @{
 */

/**  
 * @name ABWCR : Bus width control register
   * The defines for the bits of the registers
   * @{
   */
#define H8_ABWCR_ABW7        BIT(7)     ///< Area 7 is 8-bit access area
#define H8_ABWCR_ABW6        BIT(6)     ///< Area 6 is 8-bit access area
#define H8_ABWCR_ABW5        BIT(5)     ///< Area 5 is 8-bit access area
#define H8_ABWCR_ABW4        BIT(4)     ///< Area 4 is 8-bit access area
#define H8_ABWCR_ABW3        BIT(3)     ///< Area 3 is 8-bit access area
#define H8_ABWCR_ABW2        BIT(2)     ///< Area 2 is 8-bit access area
#define H8_ABWCR_ABW1        BIT(1)     ///< Area 1 is 8-bit access area
#define H8_ABWCR_ABW0        BIT(0)     ///< Area 0 is 8-bit access area
  // @}

  /**  
   * @name ASTCR : Access state control register
   * The defines for the bits of the registers
   * @{
   */
#define H8_ASTCR_AST7        BIT(7)     ///< Area 7 is accessed in three
                                        // states
#define H8_ASTCR_AST6        BIT(6)     ///< Area 6 is accessed in three
                                        // states
#define H8_ASTCR_AST5        BIT(5)     ///< Area 5 is accessed in three
                                        // states
#define H8_ASTCR_AST4        BIT(4)     ///< Area 4 is accessed in three
                                        // states
#define H8_ASTCR_AST3        BIT(3)     ///< Area 3 is accessed in three
                                        // states
#define H8_ASTCR_AST2        BIT(2)     ///< Area 2 is accessed in three
                                        // states
#define H8_ASTCR_AST1        BIT(1)     ///< Area 1 is accessed in three
                                        // states
#define H8_ASTCR_AST0        BIT(0)     ///< Area 0 is accessed in three
                                        // states
// @}

/**  
 * @name WCR : Wait Control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_WCR_RESERVE      (BIT(7)|BIT(6)|BIT(5)|BIT(4))       ///<
                                                                // Reserved, 
                                                                // read
                                                                // only
                                                                // bit,
                                                                // always
                                                                // read as 
                                                                // 1
#define H8_WCR_WMS1          BIT(3)     ///< Wait mode select 1
#define H8_WCR_WMS0          BIT(2)     ///< Wait mode select 0
#define H8_WCR_WC1           BIT(1)     ///< Wait count 1
#define H8_WCR_WC0           BIT(0)     ///< Wait count 0

#define H8_WCR_MODE_PIN_AUTO H8_WCR_WMS0|H8_WCR_WMS1    ///< Pin
                                                        // auto-wait mode
#define H8_WCR_MODE_PIN      H8_WCR_WMS1        ///< Pin wait mode 1
#define H8_WCR_MODE_OFF      H8_WCR_WMS0        ///< No wait states
                                                // inserted by wait-state
                                                // controller
#define H8_WCR_MODE_PGM      (0x00)     ///< Programmable wait mode

#define H8_WCR_STATE_NO      (0x00)     ///< No wait states inserted by
                                        // wait-state controller
#define H8_WCR_STATE_1       H8_WCR_WC0 ///< 1 state inserted
#define H8_WCR_STATE_2       H8_WCR_WC1 ///< 2 state inserted
#define H8_WCR_STATE_3       H8_WCR_WC0|H8_WCR_WC1      ///< 3 state
                                                        // inserted
// @}

  /**  
   * @name WCER : Wait state control enable register
   * The defines for the bits of the registers
   * @{
   */
#define H8_WCER_WCE7         BIT(7)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 7
#define H8_WCER_WCE6         BIT(6)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 6
#define H8_WCER_WCE5         BIT(5)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 5
#define H8_WCER_WCE4         BIT(4)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 4
#define H8_WCER_WCE3         BIT(3)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 3
#define H8_WCER_WCE2         BIT(2)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 2
#define H8_WCER_WCE1         BIT(1)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 1
#define H8_WCER_WCE0         BIT(0)     ///< External three-state-access
                                        // wait-state control enabled for
                                        // area 0
// @}

/**  
 * @name BRCR : Bus release control register
 * The defines for the bits of the registers
 * @{
 */
#define H8_BRCR_RESERVE      BIT(7)|BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)   ///<
                                                                                // Reserved: 
                                                                                // read-only 
                                                                                // bits, 
                                                                                // always 
                                                                                // read 
                                                                                // as 
                                                                                // 1
#define H8_BRCR_BRLE         BIT(0)     ///< The bus can be released to
                                        // an external device
// @}
// @}

/**
 * \defgroup _REFRESH_CONTROL_ Refresh controller
 * @{
 */

/**  
 * @name RFSHCR : Refresh control register
 * The defines for the bits of the registers
 * @{
 */
#define H8_RFSHCR_SRFMD      BIT(7)     ///< DRAM or PSRAM self-refresh
                                        // is enabled in software standby
                                        // mode
#define H8_RFSHCR_PSRAME     BIT(6)     ///< PSRAM enable
#define H8_RFSHCR_DRAME      BIT(5)     ///< DRAM enable
#define H8_RFSHCR_CASWE      BIT(4)     ///< 2CAS mode
#define H8_RFSHCR_M9M8       BIT(3)     ///< 9-bit column address mode
#define H8_RFSHCR_RFSHE      BIT(2)     ///< Refresh signal output at the
                                        // RFSH pin is enabled
#define H8_RFSHCR_RESERVE    BIT(1)     ///< Reserved: read-only bit,
                                        // always reas as 1
#define H8_RFSHCR_RCYCE      BIT(0)     ///< Refresh cycles are enabled
                                        // for area 3

#define H8_RFSHCR_INTVAL     (0x00)     ///< Can be used as an interval
                                        // timer
#define H8_RFSHCR_CON_DRAM   H8_RFSHCR_DRAME    ///< DRAM can be
                                                // connected
#define H8_RFSHCR_CON_PSRAM  H8_RFSHCR_PSRAME   ///< PSRAM can be
                                                // connected
#define H8_RFSHCR_ILL        H8_RFSHCR_DRAME|H8_RFSHCR_PSRAME   ///<
                                                                // illegal 
                                                                // setting
// @}

/**  
 * @name RTMCSR : Refresh timer control/status register
 * The defines for the bits of the registers
 * @{
 */
#define H8_RTMCSR_CMF        BIT(7)     ///< Status flag indicating that
                                        // RTCNT has matched RTCOR
#define H8_RTMCSR_CMIE       BIT(6)     ///< The CMI interrupt requested
                                        // by CMF is enabled
#define H8_RTMCSR_CKS2       BIT(5)     ///< Clock select
#define H8_RTMCSR_CKS1       BIT(4)     ///< Clock select
#define H8_RTMCSR_CKS0       BIT(3)     ///< Clock select
#define H8_RTMCSR_RESERVE    BIT(2)|BIT(1)|BIT(0)       ///< Reserved:
                                                        // read-only bits, 
                                                        // always read as
                                                        // 1

#define H8_RTMCSR_CLK_DIS    (0x00)     ///< Clock disabled
#define H8_RTMCSR_CLK_2      H8_RTMCSR_CKS0     ///< f/2 clock source
#define H8_RTMCSR_CLK_8      H8_RTMCSR_CKS1     ///< f/8 clock source
#define H8_RTMCSR_CLK_32     H8_RTMCSR_CKS0|H8_RTMCSR_CKS1      ///< f/32
                                                                // clock
                                                                // source
#define H8_RTMCSR_CLK_128    H8_RTMCSR_CKS2     ///< f/128 clock source
#define H8_RTMCSR_CLK_512    H8_RTMCSR_CKS0|H8_RTMCSR_CKS2      ///<
                                                                // f/512
                                                                // clock
                                                                // source
#define H8_RTMCSR_CLK_2048   H8_RTMCSR_CKS1|H8_RTMCSR_CKS2      ///<
                                                                // f/2048
                                                                // clock
                                                                // source
#define H8_RTMCSR_CLK_4096   H8_RTMCSR_CKS0|H8_RTMCSR_CKS1|H8_RTMCSR_CKS2       ///<
                                                                                // f/4096 
                                                                                // clock 
                                                                                // source
// @}
// @}

/**
 * \defgroup _DMA_CONTROL_ DMA-Controller 
 * @{
 */

/**  
 * @name DTCR : Data Transfer Control Register
 * The defines for the bits of the registers
 * short address mode 
 * @{
 */
#define H8_DTCR_S_DTE        BIT(7)     ///< Data transfer is enabled
                                        // (short address mode)
#define H8_DTCR_S_DTSZ       BIT(6)     ///< Word-size transfer (short
                                        // address mode)
#define H8_DTCR_S_DTID       BIT(5)     ///< MAR is decremented after
                                        // each data transfer (short
                                        // address mode)
#define H8_DTCR_S_RPE        BIT(4)     ///< Repeat enable (short address
                                        // mode)
#define H8_DTCR_S_DTIE       BIT(3)     ///< The DEND interrupt requested
                                        // by DTE is enabled (short
                                        // address mode)
#define H8_DTCR_S_DTS2A      BIT(2)     ///< Data transfer select 2
                                        // channel A (short address mode)
#define H8_DTCR_S_DTS1A      BIT(1)     ///< Data transfer select 1
                                        // channel A (short address mode)
#define H8_DTCR_S_DTS0A      BIT(0)     ///< Data transfer select 0
                                        // channel A (short address mode)

#define H8_DTCR_S_DTS2B      BIT(2)     ///< Data transfer select 2
                                        // channel B (short address mode)
#define H8_DTCR_S_DTS1B      BIT(1)     ///< Data transfer select 1
                                        // channel B (short address mode)
#define H8_DTCR_S_DTS0B      BIT(0)     ///< Data transfer select 0
                                        // channel B (short address mode)

#define H8_DTCR_S_MODE_IO    (0x00)     ///< I/O mode (short address
                                        // mode)
#define H8_DTCR_S_MODE_REP   H8_DTCR_S_RPE      ///< Repeat mode (short
                                                // address mode)
#define H8_DTCR_S_MODE_IDLE  H8_DTCR_S_RPE|H8_DTCR_S_DTIE       ///< Idle
                                                                // mode
                                                                // (short
                                                                // address 
                                                                // mode)

#define H8_DTCR_SA_COMP_CH0  (0x00)
///< Compare match/input capture A interrupt from ITU channel 0 (short
// address mode, channel A)

#define H8_DTCR_SA_COMP_CH1  H8_DTCR_S_DTS0A
///< Compare match/input capture A interrupt from ITU channel 1 (short
// address mode, channel A)

#define H8_DTCR_SA_COMP_CH2  H8_DTCR_S_DTS1A
///< Compare match/input capture A interrupt from ITU channel 2 (short
// address mode, channel A)

#define H8_DTCR_SA_COMP_CH3  H8_DTCR_S_DTS0A|H8_DTCR_S_DTS1A
///< Compare match/input capture A interrupt from ITU channel 3 (short
// address mode, channel A)

#define H8_DTCR_SA_SCI_TXE   H8_DTCR_S_DTS2A
///< Transmit-data-empty interrupt from SCI channel 0 or 1 (short address 
// mode, channel A)

#define H8_DTCR_SA_SCI_RXF   H8_DTCR_S_DTS0A|H8_DTCR_S_DTS2A
///< Receive-data-full interrupt from SCI channel 0 or 1 (short address
// mode, channel A)

#define H8_DTCR_SA_TX_FUL    H8_DTCR_S_DTS0A|H8_DTCR_S_DTS1A|H8_DTCR_S_DTS2A
///< Transfer in full address mode (short address mode, channel A)

#define H8_DTCR_SB_COMP_CH0  (0x00)
///< Compare match/input capture A interrupt from ITU channel 0 (short
// address mode, channel B)

#define H8_DTCR_SB_COMP_CH1  H8_DTCR_S_DTS0B
///< Compare match/input capture A interrupt from ITU channel 1 (short
// address mode, channel B)

#define H8_DTCR_SB_COMP_CH2  H8_DTCR_S_DTS1B
///< Compare match/input capture A interrupt from ITU channel 2 (short
// address mode, channel B)

#define H8_DTCR_SB_COMP_CH3  H8_DTCR_S_DTS0B|H8_DTCR_S_DTS1B
///< Compare match/input capture A interrupt from ITU channel 3 (short
// address mode, channel B)

#define H8_DTCR_SB_SCI_TXE   H8_DTCR_S_DTS2B
///< Transmit-data-empty interrupt from SCI channel 0 or 1 (short address 
// mode, channel B)

#define H8_DTCR_SB_SCI_RXF   H8_DTCR_S_DTS0B|H8_DTCR_S_DTS2B
///< Receive-data-full interrupt from SCI channel 0 or 1 (short address
// mode, channel B)

#define H8_DTCR_SB_F_EDGE    H8_DTCR_S_DTS1B|H8_DTCR_S_DTS2B
///< Falling edge of DREQ input (short address mode, channel B)

#define H8_DTCR_SB_LOW_LEV   H8_DTCR_S_DTS0B|H8_DTCR_S_DTS1B|H8_DTCR_S_DTS2B
///< Low level of DREQ input (short address mode, channel B)
// @}

/**  
 * @name DTCR : Data Transfer Control Register
 * The defines for the bits of the registers
 * full address mode
 * @{
 */
#define H8_DTCR_F_DTEA       BIT(7)     ///< Data transfer is enabled
                                        // (full address mode, channel A)
#define H8_DTCR_F_DTSZA      BIT(6)     ///< Word-size transfer (full
                                        // address mode, channel A)
#define H8_DTCR_F_SAIDA      BIT(5)     ///< Source address
                                        // increment/decrement (full
                                        // address mode, channel A)
#define H8_DTCR_F_SAIDEA     BIT(4)     ///< Source address
                                        // increment/decrement enable
                                        // (full address mode, channel A)
#define H8_DTCR_F_DTIEA      BIT(3)     ///< The DEND interrupt requested
                                        // by DTE is enabled (full address 
                                        // mode, channel A)
#define H8_DTCR_F_DTS2A      BIT(2)     ///< Data transfer select 2 (full
                                        // address mode) for this mode
                                        // must be set to 1 (channel A)
#define H8_DTCR_F_DTS1A      BIT(1)     ///< Data transfer select 1 (full
                                        // address mode) for this mode
                                        // must be set to 1 (channel A)
#define H8_DTCR_F_DTS0A      BIT(0)     ///< Data transfer select 0 (full
                                        // address mode) block transfer
                                        // mode (channel A)

#define H8_DTCR_F_MARA_FIX   H8_DTCR_F_SAIDA    ///< MARA is held fix
                                                // (full address mode,
                                                // channel A)
#define H8_DTCR_F_MARA_INC   H8_DTCR_F_SAIDEA   ///< MARA is incremented
                                                // after each data
                                                // transfer (full address
                                                // mode, channel A)
#define H8_DTCR_F_MARA_DEC   H8_DTCR_F_SAIDA|H8_DTCR_F_SAIDEA   ///< MARA
                                                                // is
                                                                // decremented 
                                                                // after
                                                                // each
                                                                // data
                                                                // transfer 
                                                                // (full
                                                                // address 
                                                                // mode,
                                                                // channel 
                                                                // A)

#define H8_DTCR_F_DTMEB      BIT(7)     ///< Data transfer is enabled
                                        // (full address mode, channel B)
#define H8_DTCR_F_RESERVEB   BIT(6)     ///< Although reserved, this bit
                                        // can be written and read (full
                                        // address mode, channel B)
#define H8_DTCR_F_DAIDB      BIT(5)     // /< Destination
                                        // increment/decrement (full
                                        // address mode, channel B)
#define H8_DTCR_F_DAIDEB     BIT(4)     ///< Destination
                                        // increment/decrement enable
                                        // (full address mode, channel B)
#define H8_DTCR_F_TMSB       BIT(3)     ///< Source is the block area in
                                        // block transfer mode (full
                                        // address mode, channel B)
#define H8_DTCR_F_DTS2B      BIT(2)     ///< Data transfer select (full
                                        // address mode, channel B)
#define H8_DTCR_F_DTS1B      BIT(1)     ///< Data transfer select (full
                                        // address mode, channel B)
#define H8_DTCR_F_DTS0B      BIT(0)     ///< Data transfer select (full
                                        // address mode, channel B)

#define H8_DTCR_F_MARB_FIX   H8_DTCR_F_SAIDB    ///< MARB is held fix
                                                // (full address mode,
                                                // channel B)
#define H8_DTCR_F_MARB_INC   H8_DTCR_F_SAIDEB   ///< MARB is incremented
                                                // after each data
                                                // transfer (full address
                                                // mode, channel B)
#define H8_DTCR_F_MARB_DEC   H8_DTCR_F_SAIDB|H8_DTCR_F_SAIDEB   ///< MARB
                                                                // is
                                                                // decremented 
                                                                // after
                                                                // each
                                                                // data
                                                                // transfer 
                                                                // (full
                                                                // address 
                                                                // mode,
                                                                // channel 
                                                                // B)

#define H8_DTCR_FB_NM_BURST  (0x00)     ///< Auto request (burst mode) in
                                        // normal transfer mode (full
                                        // address mode, channel B)
#define H8_DTCR_FB_NM_CYCLE  H8_DTCR_F_DTS1B    ///< Auto request (cycle
                                                // steal mode) in normal
                                                // transfer mode (full
                                                // address mode, channel
                                                // B)
#define H8_DTCR_FB_NM_FE     H8_DTCR_F_DTS1B|H8_DTCR_F_DTS2B    ///<
                                                                // Falling 
                                                                // edge of 
                                                                // DREQ in 
                                                                // normal
                                                                // transfer 
                                                                // mode
                                                                // (full
                                                                // address 
                                                                // mode,
                                                                // channel 
                                                                // B)
#define H8_DTCR_FB_NM_LOW    H8_DTCR_F_DTS0B|H8_DTCR_F_DTS1B|H8_DTCR_F_DTS2B    ///<
                                                                                // Low 
                                                                                // level 
                                                                                // input 
                                                                                // at 
                                                                                // DREQ 
                                                                                // in 
                                                                                // normal 
                                                                                // transfer 
                                                                                // mode 
                                                                                // (full 
                                                                                // address 
                                                                                // mode, 
                                                                                // channel 
                                                                                // B)

#define H8_DTCR_FB_COMP_CH0  (0x00)     ///< Compare match/input capture
                                        // A interrupt from ITU channel 0
                                        // in block transfer mode (full
                                        // address mode, channel B)
#define H8_DTCR_FB_COMP_CH1  H8_DTCR_S_DTS0A    ///< Compare match/input
                                                // capture A interrupt
                                                // from ITU channel 1 in
                                                // block transfer mode
                                                // (full address mode,
                                                // channel B)
#define H8_DTCR_FB_COMP_CH2  H8_DTCR_S_DTS1A    ///< Compare match/input
                                                // capture A interrupt
                                                // from ITU channel 2 in
                                                // block transfer mode
                                                // (full address mode,
                                                // channel B)
#define H8_DTCR_FB_COMP_CH3  H8_DTCR_S_DTS0A|H8_DTCR_S_DTS1A    ///<
                                                                // Compare 
                                                                // match/input 
                                                                // capture 
                                                                // A
                                                                // interrupt 
                                                                // from
                                                                // ITU
                                                                // channel 
                                                                // 3 in
                                                                // block
                                                                // transfer 
                                                                // mode
                                                                // (full
                                                                // address 
                                                                // mode,
                                                                // channel 
                                                                // B)
#define H8_DTCR_FB_FE        H8_DTCR_F_DTS1B|H8_DTCR_F_DTS2B    ///<
                                                                // Falling 
                                                                // edge of 
                                                                // DREQ in 
                                                                // block
                                                                // transfer 
                                                                // mode
                                                                // (full
                                                                // address 
                                                                // mode,
                                                                // channel 
                                                                // B)
// @}
// @}

/**
 * \defgroup _ITU_CONTROL_ 16-bit integrated timer unit (ITU)
 * @{
 */

/**  
 * @name TSTR : Timer Start Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TSTR_RESERVE     (BIT(7)|BIT(6)|BIT(5))      ///< Reserved,
                                                        // always read as
                                                        // 1
#define H8_TSTR_STR4         BIT(4)     ///< TCNT4 is counting
#define H8_TSTR_STR3         BIT(3)     ///< TCNT3 is counting
#define H8_TSTR_STR2         BIT(2)     ///< TCNT2 is counting
#define H8_TSTR_STR1         BIT(1)     ///< TCNT1 is counting
#define H8_TSTR_STR0         BIT(0)     ///< TCNT0 is counting
// @}

/**  
 * @name TSNC : Timer Sync Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TSNC_RESERVE     (BIT(7)|BIT(6)|BIT(5))      ///< Reserved,
                                                        // always read as
                                                        // 1
#define H8_TSNC_TCNT4        BIT(4)     ///< TCNT4 can be synchronously
                                        // preset and cleared
#define H8_TSNC_TCNT3        BIT(3)     ///< TCNT3 can be synchronously
                                        // preset and cleared
#define H8_TSNC_TCNT2        BIT(2)     ///< TCNT2 can be synchronously
                                        // preset and cleared
#define H8_TSNC_TCNT1        BIT(1)     ///< TCNT1 can be synchronously
                                        // preset and cleared
#define H8_TSNC_TCNT0        BIT(0)     ///< TCNT0 can be synchronously
                                        // preset and cleared
// @}

/**  
 * @name TMDR : Timer Mode Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TMDR_RESERVE      BIT(7)     ///< Reserved: read only bits,
                                        // always read as 1
#define H8_TMDR_MDF          BIT(6)     ///< Channel 2 operates in phase
                                        // counting mode
#define H8_TMDR_FDIR         BIT(5)     ///< OVF is set to 1 in TSR2 when
                                        // TCNT2 overflows
#define H8_TMDR_PWM4         BIT(4)     ///< Channel 4 operates in PVM
                                        // mode
#define H8_TMDR_PWM3         BIT(3)     ///< Channel 3 operates in PVM
                                        // mode
#define H8_TMDR_PWM2         BIT(2)     ///< Channel 2 operates in PVM
                                        // mode
#define H8_TMDR_PWM1         BIT(1)     ///< Channel 1 operates in PVM
                                        // mode
#define H8_TMDR_PWM0         BIT(0)     ///< Channel 0 operates in PVM
                                        // mode
// @}

/**  
 * @name TFCR : Timer Function Control Register
 * The defines for the bits of the registers
 * @{
 */
#define H8_TFCR_RESERVE     (BIT(7)|BIT(6))     ///< Reserved: read only
                                                // bits, always read as 1
#define H8_TFCR_CMD1         BIT(5)     ///< Combination mode 1
#define H8_TFCR_CMD0         BIT(4)     ///< Combination mode 0
#define H8_TFCR_BFB4         BIT(3)     ///< Buffer mode B4
#define H8_TFCR_BFA4         BIT(2)     ///< Buffer mode A4
#define H8_TFCR_BFB3         BIT(1)     ///< Buffer mode B3
#define H8_TFCR_BFA3         BIT(0)     ///< Buffer mode A3

#define H8_TFCR_COMP         H8_TFCR_CMD1       ///< Channels 3 and 4
                                                // operate together in
                                                // complementary PWM mode
#define H8_TFCR_RESYNC       H8_TFCR_CMD0|H8_TFCR_CMD1  ///< Channels 3
                                                        // and 4 operate
                                                        // together in
                                                        // reset-synchronized 
                                                        // PWM mode
// @}

/**  
 * @name TOER : Timer Output master Enable Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TOER_RESERVE     (BIT(7)|BIT(6))     ///< Reserved, always
                                                // read as 1
#define H8_TOER_EXB4         BIT(5)     ///< TOCXB4 pin is enabled for
                                        // output acc. to TFCR
#define H8_TOER_EXA4         BIT(4)     ///< TOCXA4 pin is enabled for
                                        // output acc. to TFCR
#define H8_TOER_EB3          BIT(3)     ///< TIOCB3 is enabled for output
                                        // acc. to TIOR3 and TFCR
#define H8_TOER_EB4          BIT(2)     ///< TIOCB4 is enabled for output
                                        // acc. to TIOR4 and TFCR
#define H8_TOER_EA4          BIT(1)     ///< TIOCA4 is enabled for output
                                        // acc. to TIOR4, TMDR and TFCR
#define H8_TOER_EA3          BIT(0)     ///< TIOCA3 is enabled for output
                                        // acc. to TIOR3, TMDR and TFCR
// @}

/**  
 * @name TOCR : Timer Output Control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TOCR_RESERVE2    (BIT(7)|BIT(6)|BIT(5))      ///< Reserved,
                                                        // read only bits, 
                                                        // always read as
                                                        // 1
#define H8_TOCR_XTGD         BIT(4)     ///< External triggering is
                                        // enabled
#define H8_TOCR_RESERVE1    (BIT(3)|BIT(2))     ///< Reserved, read only
                                                // bits, always read as 1
#define H8_TOCR_OLS4         BIT(1)     ///< TIOCA3, TIOCA4 and TIOCB4
                                        // outputs are not inverted
#define H8_TOCR_OLS3         BIT(0)     ///< TIOCB3, TOCXA4 and TOCXB4
                                        // outputs are not inverted
// @}

/**  
 * @name TCR : Timer Control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TCR_RESERVE       BIT(7)     ///< Reserved, read only bit,
                                        // always read as 1
#define H8_TCR_CCLR1         BIT(6)     ///< Counter clear 1
#define H8_TCR_CCLR0         BIT(5)     ///< Counter clear 0
#define H8_TCR_CKEG1         BIT(4)     ///< Clock edge 1
#define H8_TCR_CKEG0         BIT(3)     ///< Clock edge 0
#define H8_TCR_TPSC2         BIT(2)     ///< Timer prescaler 2
#define H8_TCR_TPSC1         BIT(1)     ///< Timer prescaler 1
#define H8_TCR_TPSC0         BIT(0)     ///< Timer prescaler 0

#define H8_TCR_CLR_SYNC      H8_TCR_CCLR1|H8_TCR_CCLR0  ///< TCNT is
                                                        // cleared in
                                                        // synchronization 
                                                        // with other sync 
                                                        // timers
#define H8_TCR_CLR_A         H8_TCR_CCLR0       ///< TCNT is cleared by
                                                // GRA compare match or
                                                // input capture
#define H8_TCR_CLR_B         H8_TCR_CCLR1       // /< TCNT is cleared by
                                                // GRB compare match or
                                                // input capture
#define H8_TCR_CLR_NEVER     (0x00)     ///< TCNT is not cleared

#define H8_TCR_CLK_PN        H8_TCR_CKEG1       ///< Count falling edges
#define H8_TCR_CLK_N         H8_TCR_CKEG0       ///< Count raising edges
#define H8_TCR_CLK_P         (0x00)     ///< Count both edges

#define H8_TCR_CLK_E_D       H8_TCR_TPSC0|H8_TCR_TPSC1|H8_TCR_TPSC2     ///<
                                                                        // External 
                                                                        // clock 
                                                                        // D: 
                                                                        // TCLKD 
                                                                        // input
#define H8_TCR_CLK_E_C       H8_TCR_TPSC1|H8_TCR_TPSC2  ///< External
                                                        // clock C: TCLKC
                                                        // input
#define H8_TCR_CLK_E_B       H8_TCR_TPSC0|H8_TCR_TPSC2  ///< External
                                                        // clock B: TCLKB
                                                        // input
#define H8_TCR_CLK_E_A       H8_TCR_TPSC2       ///< External clock A:
                                                // TCLKA input

#define H8_TCR_CLK_I8        H8_TCR_TPSC0|H8_TCR_TPSC1  ///< Internal
                                                        // clock: f/8
#define H8_TCR_CLK_I4        H8_TCR_TPSC1       ///< Internal clock: f/4
#define H8_TCR_CLK_I2        H8_TCR_TPSC0       ///< Internal clock: f/2
#define H8_TCR_CLK_I         (0x00)     ///< Internal clock: f
// @}

/**  
 * @name TIOR : Timer I/O control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TIOR_RESERVE      BIT(7)|BIT(3)      ///< Reserved, read only
                                                // bits, always read as 1
#define H8_TIOR_IOB2         BIT(6)     ///< I/O control B2
#define H8_TIOR_IOB1         BIT(5)     ///< I/O control B1
#define H8_TIOR_IOB0         BIT(4)     ///< I/O control B0
#define H8_TIOR_IOA2         BIT(2)     ///< I/O control A2
#define H8_TIOR_IOA1         BIT(1)     ///< I/O control A1
#define H8_TIOR_IOA0         BIT(0)     ///< I/O control A0

#define H8_TIOR_GRB_OUT_DIS  (0x00)     ///< GRB is an output register,
                                        // no output at compare match
#define H8_TIOR_GRB_OUT_LOW  H8_TIOR_IOB0       ///< GRB is an output
                                                // register, 0 output at
                                                // GRB compare match
#define H8_TIOR_GRB_OUT_HIGH H8_TIOR_IOB1       ///< GRB is an output
                                                // register, 1 output at
                                                // GRB compare match
#define H8_TIOR_GRB_OUT_TOG  H8_TIOR_IOB0|H8_TIOR_IOB1  ///< GRB is an
                                                        // output
                                                        // register,
                                                        // output toggles
                                                        // at GRB compare
                                                        // match
#define H8_TIOR_GRB_IN_R     H8_TIOR_IOB2       ///< GRB is an input
                                                // register, GRB captures
                                                // rising edge of input
#define H8_TIOR_GRB_IN_F     H8_TIOR_IOB0|H8_TIOR_IOB2  ///< GRB is an
                                                        // input register, 
                                                        // GRB captures
                                                        // falling edge of 
                                                        // input
#define H8_TIOR_GRB_IN_B     H8_TIOR_IOB1|H8_TIOR_IOB2  ///< GRB is an
                                                        // input register, 
                                                        // GRB captures
                                                        // both edges of
                                                        // input

#define H8_TIOR_GRA_OUT_DIS  (0x00)     ///< GRA is an output register,
                                        // no output at compare match
#define H8_TIOR_GRA_OUT_LOW  H8_TIOR_IOA0       ///< GRA is an output
                                                // register, 0 output at
                                                // GRA compare match
#define H8_TIOR_GRA_OUT_HIGH H8_TIOR_IOA1       ///< GRA is an output
                                                // register, 1 output at
                                                // GRA compare match
#define H8_TIOR_GRA_OUT_TOG  H8_TIOR_IOA0|H8_TIOR_IOA1  ///< GRA is an
                                                        // output
                                                        // register,
                                                        // output toggles
                                                        // at GRB compare
                                                        // match
#define H8_TIOR_GRA_IN_R     H8_TIOR_IOA2       ///< GRA is an input
                                                // register, GRA captures
                                                // rising edge of input
#define H8_TIOR_GRA_IN_F     H8_TIOR_IOA0|H8_TIOR_IOA2  ///< GRA is an
                                                        // input register, 
                                                        // GRA captures
                                                        // falling edge of 
                                                        // input
#define H8_TIOR_GRA_IN_B     H8_TIOR_IOA1|H8_TIOR_IOA2  ///< GRA is an
                                                        // input register, 
                                                        // GRA captures
                                                        // both edges of
                                                        // input
// @}

/**  
 * @name TSR : Timer Status Register 
 * The defines for the bits of the registers
   * @{
   */
#define H8_TSR_RESERVE      (BIT(7)|BIT(6)|BIT(5)|BIT(4)|BIT(3))        ///<
                                                                        // Reserved, 
                                                                        // read 
                                                                        // only 
                                                                        // bits, 
                                                                        // always 
                                                                        // read 
                                                                        // as 
                                                                        // 1
#define H8_TSR_OVF           BIT(2)     ///< This status flag indicates
                                        // TCNT overflow or underflow
#define H8_TSR_IMFB          BIT(1)     ///< This status flag indicates
                                        // GRB compare match or input
                                        // capture events
#define H8_TSR_IMFA          BIT(0)     ///< This status flag indicates
                                        // GRA compare match or input
                                        // capture events
// @}

/**  
 * @name TIER : Timer I/O Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_TIER_RESERVE     (BIT(7)|BIT(6)|BIT(5)|BIT(4)|BIT(3))        ///<
                                                                        // Reserved, 
                                                                        // read 
                                                                        // only 
                                                                        // bits, 
                                                                        // always 
                                                                        // read 
                                                                        // as 
                                                                        // 1
#define H8_TIER_OVIE         BIT(2)     ///< OVI interrupt requested by
                                        // OVF is enabled
#define H8_TIER_IMIEB        BIT(1)     ///< IMIB interrupt requested by
                                        // IMFB is enabled
#define H8_TIER_IMIEA        BIT(0)     ///< IMIA interrupt requested by
                                        // IMFA is enabled
// @}
// @}

/**
 * \defgroup _TPC_CONTROL_ Programmable timing pattern unit (TPC)
 * @{
 */

/**  
 * @name TPMR : TPC output mode register
 * The defines for the bits of the registers
 * @{
 */
#define H8_TPMR_RESERVE      BIT(7)|BIT(6)|BIT(5)|BIT(4)        ///<
                                                                // Reserved: 
                                                                // read-only 
                                                                // bits,
                                                                // always
                                                                // read as 
                                                                // 1
#define H8_TPMR_G3NOV        BIT(3)     ///< Non-overlapping TPC output
                                        // in group 3
#define H8_TPMR_G2NOV        BIT(2)     ///< Non-overlapping TPC output
                                        // in group 2
#define H8_TPMR_G1NOV        BIT(1)     ///< Non-overlapping TPC output
                                        // in group 1
#define H8_TPMR_G0NOV        BIT(0)     ///< Non-overlapping TPC output
                                        // in group 0
// @}

/**  
 * @name TPCR : TPC output control register
 * The defines for the bits of the registers
 * @{
 */
#define H8_TPCR_G3CMS1       BIT(7)     ///< Group 3 compare match select
#define H8_TPCR_G3CMS0       BIT(6)     ///< Group 3 compare match select
#define H8_TPCR_G2CMS1       BIT(5)     ///< Group 2 compare match select
#define H8_TPCR_G2CMS0       BIT(4)     ///< Group 2 compare match select
#define H8_TPCR_G1CMS1       BIT(3)     ///< Group 1 compare match select
#define H8_TPCR_G1CMS0       BIT(2)     ///< Group 1 compare match select
#define H8_TPCR_G0CMS1       BIT(1)     ///< Group 0 compare match select
#define H8_TPCR_G0CMS0       BIT(0)     ///< Group 0 compare match select

#define H8_TPCR_G3ITU0       (0x00)     ///< TPC output group 3 is
                                        // triggered by compare match in
                                        // ITU channel 0
#define H8_TPCR_G3ITU1       H8_TPCR_G3CMS0     ///< TPC output group 3
                                                // is triggered by compare 
                                                // match in ITU channel 1
#define H8_TPCR_G3ITU2       H8_TPCR_G3CMS1     ///< TPC output group 3
                                                // is triggered by compare 
                                                // match in ITU channel 2
#define H8_TPCR_G3ITU3       H8_TPCR_G3CMS0|H8_TPCR_G3CMS1      ///< TPC
                                                                // output
                                                                // group 3 
                                                                // is
                                                                // triggered 
                                                                // by
                                                                // compare 
                                                                // match
                                                                // in ITU
                                                                // channel 
                                                                // 3

#define H8_TPCR_G2ITU0       (0x00)     ///< TPC output group 2 is
                                        // triggered by compare match in
                                        // ITU channel 0
#define H8_TPCR_G2ITU1       H8_TPCR_G2CMS0     ///< TPC output group 2
                                                // is triggered by compare 
                                                // match in ITU channel 1
#define H8_TPCR_G2ITU2       H8_TPCR_G2CMS1     ///< TPC output group 2
                                                // is triggered by compare 
                                                // match in ITU channel 2
#define H8_TPCR_G2ITU3       H8_TPCR_G2CMS0|H8_TPCR_G2CMS1      ///< TPC
                                                                // output
                                                                // group 2 
                                                                // is
                                                                // triggered 
                                                                // by
                                                                // compare 
                                                                // match
                                                                // in ITU
                                                                // channel 
                                                                // 3

#define H8_TPCR_G1ITU0       (0x00)     ///< TPC output group 1 is
                                        // triggered by compare match in
                                        // ITU channel 0
#define H8_TPCR_G1ITU1       H8_TPCR_G1CMS0     ///< TPC output group 1
                                                // is triggered by compare 
                                                // match in ITU channel 1
#define H8_TPCR_G1ITU2       H8_TPCR_G1CMS1     ///< TPC output group 1
                                                // is triggered by compare 
                                                // match in ITU channel 2
#define H8_TPCR_G1ITU3       H8_TPCR_G1CMS0|H8_TPCR_G1CMS1      ///< TPC
                                                                // output
                                                                // group 1 
                                                                // is
                                                                // triggered 
                                                                // by
                                                                // compare 
                                                                // match
                                                                // in ITU
                                                                // channel 
                                                                // 3

#define H8_TPCR_G0ITU0       (0x00)     ///< TPC output group 0 is
                                        // triggered by compare match in
                                        // ITU channel 0
#define H8_TPCR_G0ITU1       H8_TPCR_G0CMS0     ///< TPC output group 0
                                                // is triggered by compare 
                                                // match in ITU channel 1
#define H8_TPCR_G0ITU2       H8_TPCR_G0CMS1     ///< TPC output group 0
                                                // is triggered by compare 
                                                // match in ITU channel 2
#define H8_TPCR_G0ITU3       H8_TPCR_G0CMS0|H8_TPCR_G0CMS1      ///< TPC
                                                                // output
                                                                // group 0 
                                                                // is
                                                                // triggered 
                                                                // by
                                                                // compare 
                                                                // match
                                                                // in ITU
                                                                // channel 
                                                                // 3
// @}

/**  
 * @name NDERB : Next data enable register B
 * The defines for the bits of the registers
 * @{
 */
#define H8_NDERB_NDER15      BIT(7)     ///< TPC output TP15 is enabled
#define H8_NDERB_NDER14      BIT(6)     ///< TPC output TP14 is enabled
#define H8_NDERB_NDER13      BIT(5)     ///< TPC output TP13 is enabled
#define H8_NDERB_NDER12      BIT(4)     ///< TPC output TP12 is enabled
#define H8_NDERB_NDER11      BIT(3)     ///< TPC output TP11 is enabled
#define H8_NDERB_NDER10      BIT(2)     ///< TPC output TP10 is enabled
#define H8_NDERB_NDER9       BIT(1)     ///< TPC output TP9 is enabled
#define H8_NDERB_NDER8       BIT(0)     ///< TPC output TP8 is enabled
// @}

/**  
 * @name NDERA : Next data enable register A
 * The defines for the bits of the registers
 * @{
 */
#define H8_NDERA_NDER7       BIT(7)     ///< TPC output TP7 is enabled
#define H8_NDERA_NDER6       BIT(6)     ///< TPC output TP6 is enabled
#define H8_NDERA_NDER5       BIT(5)     ///< TPC output TP5 is enabled
#define H8_NDERA_NDER4       BIT(4)     ///< TPC output TP4 is enabled
#define H8_NDERA_NDER3       BIT(3)     ///< TPC output TP3 is enabled
#define H8_NDERA_NDER2       BIT(2)     ///< TPC output TP2 is enabled
#define H8_NDERA_NDER1       BIT(1)     ///< TPC output TP1 is enabled
#define H8_NDERA_NDER0       BIT(0)     ///< TPC output TP0 is enabled
// @}
// @}

/**
 * \defgroup _WATCHDOG_CONTROL_ Watchdog timer
 * @{
 */

/**  
 * @name TCSR : Timer control/status register
 * The defines for the bits of the registers
 * @{
 */
#define H8_TCSR_OVF          BIT(7)     ///< Indicates that the timer has
                                        // overflowed from 0xff to 0x00
#define H8_TCSR_WTIT         BIT(6)     ///< Watchdog timer: generates a
                                        // reset signal
#define H8_TCSR_TME          BIT(5)     ///< TCNT is counting
#define H8_TCSR_RESERVE      BIT(4)|BIT(3)      ///< Reserved: read-only
                                                // bits, always read as 1
#define H8_TCSR_CKS2         BIT(2)     ///< Clock select
#define H8_TCSR_CKS1         BIT(1)     ///< Clock select
#define H8_TCSR_CKS0         BIT(0)     ///< Clock select

#define H8_TCSR_CLK_2        (0x00)     ///< f/2 Clock source
#define H8_TCSR_CLK_32       H8_TCSR_CKS0       ///< f/32 clock source
#define H8_TCSR_CLK_64       H8_TCSR_CKS1       ///< f/64 clock source
#define H8_TCSR_CLK_128      H8_TCSR_CKS0|H8_TCSR_CKS1  ///< f/128 clock
                                                        // source
#define H8_TCSR_CLK_256      H8_TCSR_CKS2       ///< f/256 clock source
#define H8_TCSR_CLK_512      H8_TCSR_CKS0|H8_TCSR_CKS2  ///< f/512 clock
                                                        // source
#define H8_TCSR_CLK_2048     H8_TCSR_CKS1|H8_TCSR_CKS2  ///< f/2048 clock
                                                        // source
#define H8_TCSR_CLK_4096     H8_TCSR_CKS0|H8_TCSR_CKS1|H8_TCSR_CKS2     ///<
                                                                        // f/4096 
                                                                        // clock 
                                                                        // source
// @}

/**  
 * @name RSTCSR : Reset control/status register
 * The defines for the bits of the registers
 * @{
 */
#define H8_RSTCSR_WRST       BIT(7)     ///< Set when TCNT overflow
                                        // generates a reset signal during 
                                        // watchdog timer operation
#define H8_RSTCSR_RSTOE      BIT(6)     ///< Reset signal is output
                                        // externally
#define H8_RSTCSR_RESERVE    BIT(5)|BIT(4)|BIT(3)|BIT(2)||BIT(1)||BIT(0)        ///<
                                                                                // Reserved: 
                                                                                // read-only 
                                                                                // bits, 
                                                                                // always 
                                                                                // read 
                                                                                // as 
                                                                                // 1
// @}
// @}

/**
 * \defgroup _SCI_CONTROL_ Serial communication interface (SCI)
 * @{
 */

/**  
 * @name SMR: Serial Mode Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_SMR_CA            BIT(7)     ///< Communication mode
#define H8_SMR_CHR           BIT(6)     ///< Character length
#define H8_SMR_PE            BIT(5)     ///< Parity bit added and checked
#define H8_SMR_OE            BIT(4)     ///< Parity mode
#define H8_SMR_STOP          BIT(3)     ///< Stop bit length
#define H8_SMR_MP            BIT(2)     ///< Multiprocessor mode
#define H8_SMR_CKS1          BIT(1)     ///< Clock select 1
#define H8_SMR_CKS0          BIT(0)     ///< Clock select 0

#define H8_SMR_SYNC          H8_SMR_CA  ///< Synchronous mode
#define H8_SMR_ASYNC         0x00       ///< Asynchronous mode
#define H8_SMR_CLEN7         H8_SMR_CHR ///< 7-bit data
#define H8_SMR_CLEN8         0x00       ///< 8-bit data
#define H8_SMR_NOPAR         0x00       ///< No parity
#define H8_SMR_EVENPAR       H8_SMR_PE  ///< Even parity mode enabled
#define H8_SMR_ODDPAR        (H8_SMR_PE|H8_SMR_OE)      ///< Odd parity
                                                        // mode enabled
#define H8_SMR_1STOP         0x00       ///< One stop bit
#define H8_SMR_2STOP         H8_SMR_STOP        ///< Two stop bits
#define H8_SMR_NO_MP         0x00       ///< No multiprocessor format
                                        // selected
#define H8_SMR_MPMODE        H8_SMR_MP  ///< Multiprocessor format
                                        // selected
#define H8_SMR_CLK64         (H8_SMR_CKS1|H8_SMR_CKS0)  ///< Clock source
                                                        // f/64
#define H8_SMR_CLK16         H8_SMR_CKS1        ///< Clock source f/16
#define H8_SMR_CLK4          H8_SMR_CKS0        ///< Clock source f/4
#define H8_SMR_CLK1          0x00       ///< Clock source f/1
// @}

/**  
 * @name SCR: Serial Control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_SCR_TIE           BIT(7)     ///< Transmit interrupt enable
#define H8_SCR_RIE           BIT(6)     ///< Receive interrupt enable
#define H8_SCR_TE            BIT(5)     ///< Transmit enable
#define H8_SCR_RE            BIT(4)     ///< Receive enable
#define H8_SCR_MPIE          BIT(3)     ///< Multiprocessor interrupt
                                        // enable
#define H8_SCR_TEIE          BIT(2)     ///< Transmit end interrupt
                                        // enable
#define H8_SCR_CKE1          BIT(1)     ///< Clock enable 1
#define H8_SCR_CKE0          BIT(0)     ///< Clock enable 0
// @}

/**  
 * @name SSR: Serial Status Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_SSR_TDRE          BIT(7)     ///< Transmit data empty
#define H8_SSR_RDRF          BIT(6)     ///< Receive data full
#define H8_SSR_ORER          BIT(5)     ///< Overrun error
#define H8_SSR_FER           BIT(4)     ///< Framing error
#define H8_SSR_PER           BIT(3)     ///< Parity error
#define H8_SSR_TEND          BIT(2)     ///< Transmit end
#define H8_SSR_MPB           BIT(1)     ///< Multiprocessor bit received
#define H8_SSR_MPBT          BIT(0)     ///< Multiprocessor bit transfer
                                        // (MPB to be transmitted)
// @}

/**  
 * @name BBR: Baud Rate Register 
 * Determinates the serial communication bit rate.
 * @{
 */
/** 
 * \brief The calculation of the BRR value
 * 
 * <pre>
 * async: 
 *                      f                  
 *   BBR=(BYTE ) ---------------- *10^6 -1 
 *                64*(2^(2n-1))*B          
 *
 *
 * sync:
 *
 *                      f                  
 *   BBR=(BYTE ) ---------------- *10^6 -1 
 *                8*(2^(2n-1))*B           
 *
 *
 * B ... Bit rate (bit/s)                          
 * f ... Operating frequence (MHz)                 
 * n ... Baud rate generator input clock (n=0 to 3)
 * 
 *       n | Clock    CKS1  CKS0
 *      -------------------------
 *       0 |   f        0     0                    
 *       1 |  f/4       0     1                    
 *       2 |  f/16      1     0                    
 *       3 |  f/64      1     1                     
 * </pre>
 */
#define H8_BRR
// @}
// @}

/**
 * \defgroup _AD_CONTROL_ A/D Converter 
 * @{
 */

/**  
 * @name ADCSR : AD Control Status Register            
 * The defines for the bits of the registers
 * @{
 */
#define H8_ADCSR_ADF         BIT(7)     ///< A/D conversion ends
#define H8_ADCSR_ADIE        BIT(6)     ///< A/D end interrupt request
                                        // (ADI) is enabled
#define H8_ADCSR_ADST        BIT(5)     ///< A/D conversion starts
#define H8_ADCSR_SCAN        BIT(4)     ///< Scan mode
#define H8_ADCSR_CKS         BIT(3)     ///< Clock select: Conversion
                                        // time = 134 states
#define H8_ADCSR_CH2         BIT(2)     ///< Channel select
#define H8_ADCSR_CH1         BIT(1)     ///< Channel select
#define H8_ADCSR_CH0         BIT(0)     ///< Channel select

#define H8_ADCSR_CHS0        (0x00)     ///< Single mode: AN0 Scan mode:
                                        // AN0
#define H8_ADCSR_CHS1        H8_ADCSR_CH0       ///< Single mode: AN1
                                                // Scan mode: AN0, AN1
#define H8_ADCSR_CHS2        H8_ADCSR_CH1       ///< Single mode: AN2
                                                // Scan mode: AN0 to AN2
#define H8_ADCSR_CHS3        H8_ADCSR_CH0|H8_ADCSR_CH1  ///< Single mode:
                                                        // AN3 Scan mode:
                                                        // AN0 to AN3
#define H8_ADCSR_CHS4        H8_ADCSR_CH2       ///< Single mode: AN4
                                                // Scan mode: AN4
#define H8_ADCSR_CHS5        H8_ADCSR_CH0|H8_ADCSR_CH2  ///< Single mode:
                                                        // AN5 Scan mode:
                                                        // AN4, AN5
#define H8_ADCSR_CHS6        H8_ADCSR_CH1|H8_ADCSR_CH2  ///< Single mode:
                                                        // AN6 Scan mode:
                                                        // AN4 to AN6
#define H8_ADCSR_CHS7        H8_ADCSR_CH0|H8_ADCSR_CH1|H8_ADCSR_CH2     ///<
                                                                        // Single 
                                                                        // mode: 
                                                                        // AN7 
                                                                        // Scan 
                                                                        // mode: 
                                                                        // AN4 
                                                                        // to 
                                                                        // AN7
// @}

/**  
 * @name ADCR : AD Control Register 
 * The defines for the bits of the registers
 * @{
 */
#define H8_ADCR_TRGE         BIT(7)     ///< A/D conversion starts at the
                                        // falling edge of the ext.
                                        // trigger signal
#define H8_ADCR_RESERVE      BIT(6)|BIT(5)|BIT(4)|BIT(3)|BIT(2)|BIT(1)|BIT(0)   ///<
                                                                                // Reserved: 
                                                                                // read 
                                                                                // only 
                                                                                // bits, 
                                                                                // always 
                                                                                // read 
                                                                                // as 
                                                                                // 1
// @}
// @}

// @}

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

#endif
