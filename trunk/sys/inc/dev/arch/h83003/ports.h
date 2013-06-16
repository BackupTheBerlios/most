/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_ISA_IO_H
#define DEV_ISA_IO_H

/** @addtogroup dev
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

/** @defgroup isa_io isa_io.h
 *
 * ISA_BUS IO architecture.
 * This is used by the smc91c94 and maybe by other ISA
 * bus compatible devices.
 *
 * @{
 */

/*-------------- Interface -----------------------------------------------*/

/** Read Byte */
#define DEV_IN_B(addr) *(volatile unsigned char*)(addr)

/** Write Byte */
#define DEV_OUT_B(x,addr) *(volatile unsigned char*)(addr)=x

/**
 * Read Word(Short) Intel Byte Order
 *
 * @param ioaddr
 * @return word  
 */
extern unsigned short DEV_in_w (unsigned long ioaddr);

/**
 * Write Word(Short) Intel Byte Order
 *
 * @param ioaddr 
 * @param word  
 */
extern void DEV_out_w (unsigned short word, unsigned long ioaddr);

/**
 * Read n Words(Short)
 * Data Bytes are in Motorola Byte Order
 *
 * @param ioaddr
 * @param buffer
 * @param len  
 */
extern void DEV_in_nw (unsigned long ioaddr, unsigned long buffer, unsigned short len);

/**
 * Write n Words(Short)
 * Data Bytes are in Motorola Byte Order
 *
 * @param ioaddr
 * @param buffer
 * @param len 
 */
extern void DEV_out_nw (unsigned long ioaddr, unsigned long buffer, unsigned short len);

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
