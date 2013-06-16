/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef DEV_H83003_REG_H
#define DEV_H83003_REG_H

/** @addtogroup dev
 *
 * @{
 */

/** @addtogroup arch
 *
 * @{
 */

/** @addtogroup h8
 *
 * @{
 */

/** @defgroup H83003_REG H83003_REG.h
 *
 * @{
 */

/*------------- Representation ------------------------------------------*/

/**
 * \addtogroup _SYSTEM_CONTROL_
 * @{
 */
extern volatile unsigned char H8_MDCR;  ///< Mode control ** 8 bit not
                                        // used
extern volatile unsigned char H8_SYSCR; ///< System control ** 8 bit used
// @}

/**
 * \addtogroup _INTERRUPT_CONTROL_
 * @{
 */
extern volatile unsigned char H8_ISCR;  ///< IRQ sense control register
                                        // ** 8 bit used
extern volatile unsigned char H8_IER;   ///< IRQ enable ** 8 bit used
extern volatile unsigned char H8_ISR;   ///< IRQ status ** 8 bit used
extern volatile unsigned char H8_IPRA;  ///< Interrupt priority register
                                        // A ** 8 bit used
extern volatile unsigned char H8_IPRB;  ///< Interrupt priority register
                                        // B ** 8 bit used
// @}

/**
 * \addtogroup _BUS_CONTROL_
 * @{
 */
extern volatile unsigned char H8_ABWCR; ///< Bus width control ** 8 bit
                                        // used
extern volatile unsigned char H8_ASTCR; ///< Access state control ** 8
                                        // bit used
extern volatile unsigned char H8_WCR;   ///< Wait control ** 8 bit used
extern volatile unsigned char H8_WCER;  ///< Wait state controller enable
                                        // ** 8 bit used
extern volatile unsigned char H8_BRCR;  ///< Bus release control ** 8 bit
                                        // used
// @}


/**
 * \addtogroup _REFRESH_CONTROL_
 * @{
 */
extern volatile unsigned char H8_RFSHCR;        ///< Refresh control ** 8
                                                // bit not used
extern volatile unsigned char H8_RTMCSR;        ///< Refresh timer
                                                // control/status ** 8 bit 
                                                // not used
extern volatile unsigned char H8_RTCNT; ///< Refresh timer counter ** 8
                                        // bit not used
extern volatile unsigned char H8_RTCOR; ///< Refresh time constant ** 8
                                        // bit not used
// @}


/**
 * \addtogroup _DMA_CONTROL_ 
 * @{
 */
extern volatile unsigned char H8_DMAC_MAR0AR;   ///< Memory address AR ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0AE;   ///< Memory address AE ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0AH;   ///< Memory address AH ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0AL;   ///< Memory address AL ch
                                                // 0 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR0A;    ///< Memory address A ch
                                                // 0 ** 32 bit used
extern volatile unsigned char H8_DMAC_ETCR0AH;  ///< Execute transfer cnt
                                                // AH ch 0 ** 8 bit used
extern volatile unsigned char H8_DMAC_ETCR0AL;  ///< Execute transfer cnt
                                                // AL ch 0 ** 8 bit used
extern volatile unsigned short H8_DMAC_ETCR0A;  ///< Execute transfer cnt
                                                // A ch 0 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR0A;   ///< I/O address A ch 0
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR0A;   ///< Data transfer
                                                // control A ch 0 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR0BR;   ///< Memory address BR ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0BE;   ///< Memory address BE ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0BH;   ///< Memory address BH ch
                                                // 0 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR0BL;   ///< Memory address BL ch
                                                // 0 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR0B;    ///< Memory address B ch
                                                // 0 ** 32 bit used
extern volatile unsigned char H8_DMAC_ETCR0BH;  ///< Execute transfer cnt
                                                // BH ch 0 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR0BL;  ///< Execute transfer cnt
                                                // BL ch 0 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR0B;  ///< Execute transfer cnt
                                                // B ch 0 ** 16 bit used
extern volatile unsigned char H8_DMAC_IOAR0B;   ///< I/O address B ch 0
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR0B;   ///< Data transfer
                                                // control B ch 0 ** 8 bit 
                                                // used

extern volatile unsigned char H8_DMAC_MAR1AR;   ///< Memory address AR ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1AE;   ///< Memory address AE ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1AH;   ///< Memory address AH ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1AL;   ///< Memory address AL ch
                                                // 1 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR1A;    ///< Memory address A ch
                                                // 1 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR1AH;  ///< Execute transfer cnt
                                                // AH ch 1 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR1AL;  ///< Execute transfer cnt
                                                // AL ch 1 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR1A;  ///< Execute transfer cnt
                                                // A ch 1 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR1A;   ///< I/O address A ch 1
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR1A;   ///< Data transfer
                                                // control A ch 1 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR1BR;   ///< Memory address BR ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1BE;   ///< Memory address BE ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1BH;   ///< Memory address BH ch
                                                // 1 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR1BL;   ///< Memory address BL ch
                                                // 1 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR1B;    ///< Memory address B ch
                                                // 1 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR1BH;  ///< Execute transfer cnt
                                                // BH ch 1 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR1BL;  ///< Execute transfer cnt
                                                // BL ch 1 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR1B;  ///< Execute transfer cnt
                                                // B ch 1 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR1B;   ///< I/O address B ch 1
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR1B;   ///< Data transfer
                                                // control B ch 1 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR2AR;   ///< Memory address AR ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2AE;   ///< Memory address AE ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2AH;   ///< Memory address AH ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2AL;   ///< Memory address AL ch
                                                // 2 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR2A;    ///< Memory address A ch
                                                // 2 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR2AH;  ///< Execute transfer cnt
                                                // AH ch 2 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR2AL;  ///< Execute transfer cnt
                                                // AL ch 2 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR2A;  ///< Execute transfer cnt
                                                // A ch 2 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR2A;   ///< I/O address A ch 2
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR2A;   ///< Data transfer
                                                // control A ch 2 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR2BR;   ///< Memory address BR ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2BE;   ///< Memory address BE ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2BH;   ///< Memory address BH ch
                                                // 2 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR2BL;   ///< Memory address BL ch
                                                // 2 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR2B;    ///< Memory address B ch
                                                // 2 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR2BH;  ///< Execute transfer cnt
                                                // BH ch 2 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR2BL;  ///< Execute transfer cnt
                                                // BL ch 2 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR2B;  ///< Execute transfer cnt
                                                // B ch 2 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR2B;   ///< I/O address B ch 2
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR2B;   ///< Data transfer
                                                // control B ch 2 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR3AR;   ///< Memory address AR ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3AE;   ///< Memory address AE ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3AH;   ///< Memory address AH ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3AL;   ///< Memory address AL ch
                                                // 3 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR3A;    ///< Memory address A ch
                                                // 3 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR3AH;  ///< Execute transfer cnt
                                                // AH ch 3 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR3AL;  ///< Execute transfer cnt
                                                // AL ch 3 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR3A;  ///< Execute transfer cnt
                                                // A ch 3 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR3A;   ///< I/O address A ch 3
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR3A;   ///< Data transfer
                                                // control A ch 3 ** 8 bit 
                                                // not used

extern volatile unsigned char H8_DMAC_MAR3BR;   ///< Memory address BR ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3BE;   ///< Memory address BE ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3BH;   ///< Memory address BH ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_DMAC_MAR3BL;   ///< Memory address BL ch
                                                // 3 ** 8 bit not used
extern volatile unsigned long H8_DMAC_MAR3B;    ///< Memory address B ch
                                                // 3 ** 32 bit not used
extern volatile unsigned char H8_DMAC_ETCR3BH;  ///< Execute transfer cnt
                                                // BH ch 3 ** 8 bit not
                                                // used
extern volatile unsigned char H8_DMAC_ETCR3BL;  ///< Execute transfer cnt
                                                // BL ch 3 ** 8 bit not
                                                // used
extern volatile unsigned short H8_DMAC_ETCR3B;  ///< Execute transfer cnt
                                                // B ch 3 ** 16 bit not
                                                // used
extern volatile unsigned char H8_DMAC_IOAR3B;   ///< I/O address B ch 3
                                                // ** 8 bit not used
extern volatile unsigned char H8_DMAC_DTCR3B;   ///< Data transfer
                                                // control B ch 3 ** 8 bit 
                                                // not used
// @}


/**
 * \defgroup _IO_PORTS_ I/O ports
 * @{
 */
extern volatile unsigned char H8_P4DDR; ///< Port 4 data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_P4DR;  ///< Port 4 data ** 8 bit used
extern volatile unsigned char H8_P4PCR; ///< Port 4 input pull-up control
                                        // ** 8 bit used 

extern volatile unsigned char H8_P5DDR; ///< Port 5 data direction ** 8
                                        // bit not used 
extern volatile unsigned char H8_P5DR;  ///< Port 5 data ** 8 bit not
                                        // used 
extern volatile unsigned char H8_P5PCR; ///< Port 5 input pull-up control
                                        // ** 8 bit not used 

extern volatile unsigned char H8_P6DDR; ///< Port 6 data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_P6DR;  ///< Port 6 data ** 8 bit used

extern volatile unsigned char H8_P7DR;  ///< Port 7 data ** 8 bit used

extern volatile unsigned char H8_P8DDR; ///< Port 8 data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_P8DR;  ///< Port 8 data ** 8 bit used

extern volatile unsigned char H8_P9DDR; ///< Port 9 data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_P9DR;  ///< Port 9 data ** 8 bit used

extern volatile unsigned char H8_PADDR; ///< Port A data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_PADR;  ///< Port A data ** 8 bit used

extern volatile unsigned char H8_PBDDR; ///< Port B data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_PBDR;  ///< Port B data ** 8 bit used

extern volatile unsigned char H8_PCDDR; ///< Port C data direction ** 8
                                        // bit used 
extern volatile unsigned char H8_PCDR;  ///< Port C data ** 8 bit used
// @}


/**
 * \addtogroup _ITU_CONTROL_ 
 * @{
 */
extern volatile unsigned char H8_ITU_TSTR;      ///< Timer start common
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TSNC;      ///< Timer synchro common
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TMDR;      ///< Timer mode common **
                                                // 8 bit used
extern volatile unsigned char H8_ITU_TFCR;      ///< Timer func ctrl
                                                // common ** 8 bit not
                                                // used
extern volatile unsigned char H8_ITU_TOER;      ///< Timer out master
                                                // enab common ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_TOCR;      ///< Timer outp ctrl
                                                // common ** 8 bit not
                                                // used

extern volatile unsigned char H8_ITU_TCR0;      ///< Timer control ch 0
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIOR0;     ///< Timer I/O ctrl ch 0
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIER0;     ///< Timer intrpt enab ch
                                                // 0 ** 8 bit used
extern volatile unsigned char H8_ITU_TSR0;      ///< Timer status ch 0 **
                                                // 8 bit used
extern volatile unsigned char H8_ITU_TCNT0H;    ///< Timer counter(high)
                                                // ch 0 ** 8 bit not used
extern volatile unsigned char H8_ITU_TCNT0L;    ///< Timer counter(low)
                                                // ch 0 ** 8 bit not used
extern volatile unsigned short H8_ITU_TCNT0;    ///< Timer counter ch 0
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRA0H;     ///< Timer general
                                                // A(high) ch 0 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRA0L;     ///< Timer general A(low)
                                                // ch 0 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRA0;     ///< Timer general A ch 0
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_GRB0H;     ///< Timer general
                                                // B(high) ch 0 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRB0L;     ///< Timer general B(low)
                                                // ch 0 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRB0;     ///< Timer general B ch 0
                                                // ** 16 bit not used

extern volatile unsigned char H8_ITU_TCR1;      ///< Timer control ch 1
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIOR1;     ///< Timer I/O ctrl ch 1
                                                // ** 8 bit not used
extern volatile unsigned char H8_ITU_TIER1;     ///< Timer intrpt enab ch
                                                // 1 ** 8 bit used
extern volatile unsigned char H8_ITU_TSR1;      ///< Timer status ch 1 **
                                                // 8 bit used
extern volatile unsigned char H8_ITU_TCNT1H;    ///< Timer counter(high)
                                                // ch 1 ** 8 bit not used
extern volatile unsigned char H8_ITU_TCNT1L;    ///< Timer counter(low)
                                                // ch 1 ** 8 bit not used
extern volatile unsigned short H8_ITU_TCNT1;    ///< Timer counter ch 1
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRA1H;     ///< Timer general
                                                // A(high) ch 1 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRA1L;     ///< Timer general A(low)
                                                // ch 1 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRA1;     ///< Timer general A ch 1
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRB1H;     ///< Timer general
                                                // B(high) ch 1 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRB1L;     ///< Timer general B(low)
                                                // ch 1 ** 8 bit not used 
extern volatile unsigned short H8_ITU_GRB1;     ///< Timer general B ch 1
                                                // ** 8 bit used

extern volatile unsigned char H8_ITU_TCR2;      ///< Timer control ch 2
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIOR2;     ///< Timer I/O ctrl H ch
                                                // 2 ** 8 bit used
extern volatile unsigned char H8_ITU_TIER2;     ///< Timer intrpt enab ch
                                                // 2 ** 8 bit used
extern volatile unsigned char H8_ITU_TSR2;      ///< Timer status ch 2 **
                                                // 8 bit used
extern volatile unsigned char H8_ITU_TCNT2H;    ///< Timer counter(high)
                                                // ch 2 ** 8 bit not used
extern volatile unsigned char H8_ITU_TCNT2L;    ///< Timer counter(low)
                                                // ch 2 ** 8 bit not used
extern volatile unsigned short H8_ITU_TCNT2;    ///< Timer counter ch 2
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRA2H;     ///< Timer general
                                                // A(high) ch 2 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRA2L;     ///< Timer general A(low)
                                                // ch 2 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRA2;     ///< Timer general A ch 2
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRB2H;     ///< Timer general
                                                // B(high) ch 2 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRB2L;     ///< Timer general B(low)
                                                // ch 2 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRB2;     ///< Timer general B ch 2
                                                // ** 16 bit used

extern volatile unsigned char H8_ITU_TCR3;      ///< Timer control ch 3
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIOR3;     ///< Timer I/O ctrl ch 3
                                                // ** 8 bit not used
extern volatile unsigned char H8_ITU_TIER3;     ///< Timer intrpt enab ch
                                                // 3 ** 8 bit not used
extern volatile unsigned char H8_ITU_TSR3;      ///< Timer status ch 3 **
                                                // 8 bit not used
extern volatile unsigned char H8_ITU_TCNT3H;    ///< Timer counter(high)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned char H8_ITU_TCNT3L;    ///< Timer counter(low)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned short H8_ITU_TCNT3;    ///< Timer counter ch 3
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_GRA3H;     ///< Timer general
                                                // A(high) ch 3 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRA3L;     ///< Timer general A(low)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRA3;     ///< Timer general A ch 3
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_GRB3H;     ///< Timer general
                                                // B(high) ch 3 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRB3L;     ///< Timer general B(low)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRB3;     ///< Timer general B ch 3
                                                // ** 16 bit used
extern volatile unsigned char H8_ITU_BRA3H;     ///< Timer buffer A(high)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned char H8_ITU_BRA3L;     ///< Timer buffer A(low)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned short H8_ITU_BRA3;     ///< Timer buffer A ch 3
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_BRB3H;     ///< Timer buffer B(high)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned char H8_ITU_BRB3L;     ///< Timer buffer B(low)
                                                // ch 3 ** 8 bit not used
extern volatile unsigned short H8_ITU_BRB3;     ///< Timer buffer B ch 3
                                                // ** 16 bit not used

extern volatile unsigned char H8_ITU_TCR4;      ///< Timer control ch 4
                                                // ** 8 bit used
extern volatile unsigned char H8_ITU_TIOR4;     ///< Timer I/O ctrl ch 4
                                                // ** 8 bit not used
extern volatile unsigned char H8_ITU_TIER4;     ///< Timer intrpt enab ch
                                                // 4 ** 8 bit not used
extern volatile unsigned char H8_ITU_TSR4;      ///< Timer status ch 4 **
                                                // 8 bit not used
extern volatile unsigned char H8_ITU_TCNT4H;    ///< Timer counter(high)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned char H8_ITU_TCNT4L;    ///< Timer counter(low)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned short H8_ITU_TCNT4;    ///< Timer counter ch 4
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_GRA4H;     ///< Timer general
                                                // A(high) ch 4 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRA4L;     ///< Timer general A(low)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRA4;     ///< Timer general A ch 4
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_GRB4H;     ///< Timer general
                                                // B(high) ch 4 ** 8 bit
                                                // not used
extern volatile unsigned char H8_ITU_GRB4L;     ///< Timer general B(low)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned short H8_ITU_GRB4;     ///< Timer general B ch 4
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_BRA4H;     ///< Timer buffer A(high)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned char H8_ITU_BRA4L;     ///< Timer buffer A(low)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned short H8_ITU_BRA4;     ///< Timer buffer A ch 4
                                                // ** 16 bit not used
extern volatile unsigned char H8_ITU_BRB4H;     ///< Timer buffer B(high)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned char H8_ITU_BRB4L;     ///< Timer buffer B(low)
                                                // ch 4 ** 8 bit not used
extern volatile unsigned short H8_ITU_BRB4;     ///< Timer buffer B ch 4
                                                // ** 16 bit not used
// @}


/**
 * \addtogroup _TPC_CONTROL_ 
 * @{
 */
extern volatile unsigned char H8_TPC_TPMR;      ///< TPC output mode ** 8
                                                // bit not used
extern volatile unsigned char H8_TPC_TPCR;      ///< TPC output control
                                                // ** 8 bit not used
extern volatile unsigned char H8_TPC_NDERB;     ///< Next data enable B
                                                // ** 8 bit not used
extern volatile unsigned char H8_TPC_NDERA;     ///< Next data enable A
                                                // ** 8 bit not used
extern volatile unsigned char H8_TPC_NDRB1;     ///< Next data B1 ** 8
                                                // bit not used
extern volatile unsigned char H8_TPC_NDRA1;     ///< Next data A1 ** 8
                                                // bit not used
extern volatile unsigned char H8_TPC_NDRB2;     ///< Next data B2 ** 8
                                                // bit not used
extern volatile unsigned char H8_TPC_NDRA2;     ///< Next data A2 ** 8
                                                // bit not used
// @}


/**
 * \addtogroup _WATCHDOG_CONTROL_
 * @{
   */
extern volatile unsigned char H8_WDT_TCSR;      ///< Timer control/status
                                                // ** 8 bit not used 
extern volatile unsigned short H8_WDT_TCSR_WRITE;       ///< Timer
                                                        // control/status
                                                        // write ** 16 bit 
                                                        // not used 
extern volatile unsigned char H8_WDT_TCNT;      ///< Timer counter ** 8
                                                // bit not used 
extern volatile unsigned short H8_WDT_TCNT_WRITE;       ///< Timer
                                                        // counter write
                                                        // ** 16 bit not
                                                        // used 
extern volatile unsigned char H8_WDT_RSTCSR;    ///< Reset control/status
                                                // ** 8 bit not used 
extern volatile unsigned short H8_WDT_RSTCSR_WR;        ///< Reset
                                                        // control/status
                                                        // write ** 16 bit 
                                                        // not used 
// @}


/**
 * \addtogroup _SCI_CONTROL_ 
 * @{
   */
extern volatile unsigned char H8_SCI_SMR0;      ///< Serial mode ch 0 **
                                                // 8 bit used
extern volatile unsigned char H8_SCI_BRR0;      ///< Bit rate ch 0 ** 8
                                                // bit used
extern volatile unsigned char H8_SCI_SCR0;      ///< Serial control ch 0
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_TDR0;      ///< Transmit data ch 0
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_SSR0;      ///< Serial status ch 0
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_RDR0;      ///< Receive data ch 0 **
                                                // 8 bit used

extern volatile unsigned char H8_SCI_SMR1;      ///< Serial mode ch 1 **
                                                // 8 bit used
extern volatile unsigned char H8_SCI_BRR1;      ///< Bit rate ch 1 ** 8
                                                // bit used
extern volatile unsigned char H8_SCI_SCR1;      ///< Serial control ch 1
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_TDR1;      ///< Transmit data ch 1
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_SSR1;      ///< Serial status ch 1
                                                // ** 8 bit used
extern volatile unsigned char H8_SCI_RDR1;      ///< Receive data ch 1 **
                                                // 8 bit used
// @}


/**
 * \addtogroup _AD_CONTROL_ 
 * @{
 */
extern volatile unsigned char H8_ADDRAH;        ///< Data register A high
                                                // ** 8 bit used
extern volatile unsigned char H8_ADDRAL;        ///< Data register A low
                                                // ** 8 bit used
extern volatile unsigned char H8_ADDRBH;        ///< Data register B high
                                                // ** 8 bit used
extern volatile unsigned char H8_ADDRBL;        ///< Data register B low
                                                // ** 8 bit used
extern volatile unsigned char H8_ADDRCH;        ///< Data register C high
                                                // ** 8 bit not used
extern volatile unsigned char H8_ADDRCL;        ///< Data register C low
                                                // ** 8 bit not used
extern volatile unsigned char H8_ADDRDH;        ///< Data register D high
                                                // ** 8 bit not used
extern volatile unsigned char H8_ADDRDL;        ///< Data register D low
                                                // ** 8 bit not used
extern volatile unsigned char H8_ADCSR; ///< Control/status ** 8 bit used
extern volatile unsigned char H8_ADCR;  ///< Control ** 8 bit not used
// @}

/*------------------------------------------------------------------------*/

/** @}
 */

/** @}
 */

/** @}
 */

/** @}
 */

#endif
