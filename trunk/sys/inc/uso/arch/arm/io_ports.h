/*
 * io_ports.h
 *
 *  Created on: 01.12.2013
 *      Author: maik
 */

#ifndef USO_IO_PORTS_ARM_H_
#define USO_IO_PORTS_ARM_H_

/** Read Byte */
#define USO_in_b(addr) *(volatile unsigned char *)(addr)

/** Write Byte */
#define USO_out_b(addr, data) *(volatile unsigned char*)(addr)=data


/**
 * Read Word(Short) Intel Byte Order
 *
 * @param addr
 * @return data
 */
//extern ACE_u16_t USO_in_w (ACE_u32_t addr);
#define USO_in_w(addr) *(volatile unsigned short *)(addr)


/**
 * Write Word(Short) Intel Byte Order
 *
 * @param addr
 * @param data
 */
//extern void USO_out_w (ACE_u32_t addr, ACE_u16_t data);
#define USO_out_w(addr, data) *(volatile unsigned short*)(addr)=data


/**
 * Read data word by word(Short)
 * Data words at the addr are in Motorola Byte Order!
 *
 * @param addr
 * @param buffer
 * @param len
 */
//extern void USO_in_w_stream (unsigned long addr, unsigned long buffer, unsigned short len);
#define USO_in_w_stream(addr, buffer, len)

/**
 * Write data word by word(Short)
 * Data Words at the addr are in Motorola Byte Order!
 *
 * @param addr
 * @param buffer
 * @param len
 */
//extern void USO_out_w_stream (unsigned long addr, unsigned long buffer, unsigned short len);
#define USO_out_w_stream(addr, buffer, len)


#endif /* IO_PORTS_H_ */
