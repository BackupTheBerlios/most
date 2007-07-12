/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef NET_SMC_91C94_DEF_H
#define NET_SMC_91C94_DEF_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup smc_91c94_def smc_91c94_def.h
 *
 * Definitions for smc 91c94
 *
 * @{
 */

#define SMC_BANK_SELECT      0xE    /**< BANK SELECT */
#define SMC_IO_EXTENT        0x10   /**< IO EXTEND */


/*
 * BANK 0
 * ========================================================================== 
 */

#define SMC_TCR              0x0        /**< Transmit Control Register RW */
#define SMC_EPHSR            0x2        /**< EPH Status Register R */
#define SMC_RCR              0x4        /**< Receive Control Register RW */
#define SMC_ECR              0x6        /**< Counter Register R */
#define SMC_MIR              0x8        /**< Memory Information Register R */
#define SMC_MCR              0xA        /**< Memory Configuration Register
                                         * l:RW h:R */
/*
 * 12 is reserved 
 */

/*
 * TCR:
 * ----------------------------------------------------------------------------- 
 */

#define SMC_TCR_TX_ENA       0x0001     /**< If this is 1, we can transmit */
#define SMC_TCR_PAD_ENA      0x0080     /**< Pads short packets to 64 bytes */
#define SMC_TCR_MON_CNS      0x0400     /**< Monitors the carrier status */
#define SMC_TCR_FDUPLX       0x0800     /**< Receive packets sent out */
#define SMC_TCR_STP_SQET     0x1000     /**< Stop transmitting if Signal
                                         * quality error */

#define SMC_TCR_CLEAR        0x0               /**< ? */
#define SMC_TCR_NORMAL       SMC_TCR_TX_ENA    /**< ? */

/*
 * EPHSR
 * ---------------------------------------------------------------------------- 
 */

#define SMC_EPHSR_TX_SUC     0x0001     /**< Transmit was sucessful */
#define SMC_EPHSR_16COL      0x0010     /**< 16 collisions reached */
#define SMC_EPHSR_LATCOL     0x0200     /**< Late collision detected */
#define SMC_EPHSR_LOSTCAR    0x0400     /**< Lost carrier sense */
#define SMC_EPHSR_LINK_OK    0x4000     /**< Is the link integrity ok ? */

/*
 * RCR
 * ------------------------------------------------------------------------------ 
 */

#define SMC_RCR_PRMS         0x0002     /**< Enable promiscuous mode */
#define SMC_RCR_ALMUL        0x0004     /**< Receive all multicast packets */
#define SMC_RCR_RX_ENA       0x0100     /**< If this is set, we can receive
                                         * packets */
#define SMC_RCR_STRIP_CRC    0x0200     /**< Strips CRC */
#define SMC_RCR_SOFT_RST     0x8000     /**< Resets the chip */

#define SMC_RCR_CLEAR        0x0        /**< ? */

/** ? */
#define SMC_RCR_NORMAL       ( SMC_RCR_RX_ENA | SMC_RCR_STRIP_CRC)


/*
 * BANK 1
 * =========================================================================== 
 */

#define SMC_CR               0x0        /**< Configuration Register RW */
#define SMC_BAR              0x2        /**< Base Address Register RW */
#define SMC_IAR0             0x4        /**< Individual Address Register RW */
#define SMC_IAR2             0x6        /**< ? */ 
#define SMC_IAR4             0x8        /**< ? */
#define SMC_GPR              0xA        /**< General Purpose Register RW */
#define SMC_CTR              0xC        /**< Control Register RW */

/*
 * CR
 * ------------------------------------------------------------------------------- 
 */

#define SMC_CR_AUI_SELECT    0x0100     /**< If 1 the AUI interface is used */

/*
 * CTR
 * ------------------------------------------------------------------------------ 
 */

#define SMC_CTR_TE_ENABLE    0x0020     /**< Transmit Error Enable */
#define SMC_CTR_CR_ENABLE    0x0040     /**< Counter Roll over Enable */
#define SMC_CTR_LE_ENABLE    0x0080     /**< Link Error Enable */
#define SMC_CTR_AUTO_RELEASE 0x0800     /**< Transmit pages are released */
#define SMC_CTR_PWRDN        0x2000     /**< Enter power down mode */

#define SMC_CTR_CLEAR        0x0        /**< ? */
/** ? */
#define SMC_CTR_NORMAL       ( SMC_CTR_TE_ENABLE |\
                               SMC_CTR_CR_ENABLE |\
                               SMC_CTR_LE_ENABLE |\
                               SMC_CTR_AUTO_RELEASE )

/*
 * BANK 2
 * =========================================================================== 
 */

#define SMC_MMUCR            0x0        /**< MMU Command Register W (R busy
                                         * bit) */
#define SMC_PNR              0x2        /**< Packet Number Register RW */
#define SMC_ARR              0x3        /**< Allocation Result Register R */
#define SMC_FIFO             0x4        /**< FIFO Ports Register R */
#define SMC_PTR              0x6        /**< Pointer Register RW */
#define SMC_DATA1            0x8        /**< Data Register RW */
#define SMC_DATA2            0xA
#define SMC_IST              0xC        /**< Interrupt Status Register R */
#define SMC_ACK              0xC        /**< Interrupt Acknowledge Register
                                         * W */
#define SMC_MSK              0xD        /**< Interrupt Mask Register RW */

/*
 * MMUCR
 * ---------------------------------------------------------------------------- 
 */

#define SMC_MMUCR_NOP        0x0000     /**< No Operation */
#define SMC_MMUCR_BUSY       0x0001     /**< Only readable bit in the
                                         * register */
#define SMC_MMUCR_ALLOC      0x0020     /**< Or with number of 256 byte
                                         * packets */
#define SMC_MMUCR_RESET      0x0040     /**< Reset mmu to initial state */
#define SMC_MMUCR_REMOVE     0x0060     /**< Remove the current rx packet */
#define SMC_MMUCR_RELEASE    0x0080     /**< Remove and release the current
                                         * rx packet */
#define SMC_MMUCR_FREEPKT    0x00A0     /**< Release packet in PNR register */
#define SMC_MMUCR_ENQUEUE    0x00C0     /**< Enqueue the packet for transmit 
                                         */

/*
 * FIFO
 * ----------------------------------------------------------------------------- 
 */

#define SMC_FIFO_TEMPTY      0x0080     /**< No transmit packets in TX queue 
                                         */
#define SMC_FIFO_REMPTY      0x8000     /**< No receive packets in RX queue */

/*
 * PTR
 * ------------------------------------------------------------------------------ 
 */

#define SMC_PTR_READ         0x2000     /**< If 1 read, if 0 write operation 
                                         */
#define SMC_PTR_AUTOINCR     0x4000     /**< Auto incrementation */
#define SMC_PTR_RCV          0x8000     /**< If 1 address refers to the
                                         * receive area, else write */

/*
 * IST ACK
 * MSK---------------------------------------------------------------------------- 
 */

#define SMC_IM_RCV_INT       0x01       /**< Receive Interrupt */
#define SMC_IAM_TX_INT       0x02       /**< Packet send */
#define SMC_IAM_TX_EMPTY_INT 0x04       /**< TX queue empty */
#define SMC_IM_ALLOC_INT     0x08       /**< MMU TX page allocation
                                         * completed */
#define SMC_IAM_RX_OVRN_INT  0x10       /**< Receiver overruns */
#define SMC_IM_EPH_INT       0x20       /**< Ethernet protocol handler */
#define SMC_IAM_ERCV_INT     0x40       /**< Early receive interrupt */

/** ? */
#define SMC_INT_MASK   (SMC_IM_EPH_INT | SMC_IAM_RX_OVRN_INT | SMC_IM_RCV_INT)


/*
 * BANK 3
 * =========================================================================== 
 */

#define SMC_MT1              0x0        /**< Multicast table RW */
#define SMC_MT2              0x2        /**< Multicast table RW */
#define SMC_MT3              0x4        /**< Multicast table RW */
#define SMC_MT4              0x6        /**< Multicast table RW */
#define SMC_MGMT             0x8        /**< Managment Interface RW */
#define SMC_REV              0xA        /**< Revison Register ( hi: chip id
                                         * low: rev # ) R */
#define SMC_ERCV             0xC        /**< Early RCV Register this is NOT
                                         * on SMC9192 RW */

/*
 * REV
 * ------------------------------------------------------------------------------ 
 */

#define SMC_REV_91C92        3    /**< ? */
#define SMC_REV_91C94        4    /**< ? */
#define SMC_REV_91C95        5    /**< ? */
#define SMC_REV_91C100       7    /**< ? */

/*===================================================================================*/



/*
 * Receive Frame status word 
 */

#define SMC_RFS_MULTICAST     0x0001    /**< Receive frame was multicast */
#define SMC_RFS_TOOSHORT      0x0400    /**< Receive frame is shorter than
                                         * 64 byte */
#define SMC_RFS_TOOLONG       0x0800    /**< Receive frame is longer than
                                         * 1518 bytes */
#define SMC_RFS_ODDFRAME      0x1000    /**< Received frame has an odd
                                         * number of bytes */
#define SMC_RFS_BADCRC        0x2000    /**< Frame had CRC error */
#define SMC_RFS_ALGNERR       0x8000    /**< Frame had alignment error */

/** ? */
#define SMC_RFS_ERRORS        (SMC_RFS_ALGNERR | SMC_RFS_BADCRC | SMC_RFS_TOOLONG | SMC_RFS_TOOSHORT)



/*===================================================================================*/

/** @}
 */

/** @}
 */

#endif
