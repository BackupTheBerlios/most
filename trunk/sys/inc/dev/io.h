/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef DEV_IO_H
#define DEV_IO_H

/** @defgroup io io.h
 *
 * IO architecture low level.
 * This is used by the network layer,
 * i think this comes from the i386 IO system.
 * Think if it is really necessary.
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
 * Read n Words(Short) Motorola Byte Order
 *
 * @param ioaddr
 * @param buffer
 * @param len  
 */
extern void DEV_in_nw (unsigned long ioaddr, unsigned long buffer, unsigned short len);

/**
 * Write n Words(Short) Motorola Byte Order 
 *
 * @param ioaddr
 * @param buffer
 * @param len 
 */
extern void DEV_out_nw (unsigned long ioaddr, unsigned long buffer, unsigned short len);

/*------------------------------------------------------------------------*/

/** @}
 */


#endif
