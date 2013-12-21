/*
 * io_ports.h
 *
 *  Created on: 01.12.2013
 *      Author: maik
 */

#ifndef USO_IO_PORTS_I386_H_
#define USO_IO_PORTS_I386_H_

#include <ace/stddef.h>

static inline ACE_u8_t USO_in_b(ACE_u16_t port)
{
    ACE_u8_t result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline ACE_u16_t USO_in_w(ACE_u16_t port)
{
    ACE_u16_t result;
    __asm__ volatile ("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline ACE_u32_t USO_in_l(ACE_u16_t port)
{
    ACE_u32_t result;
    __asm__ volatile ("inl %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}



static inline void USO_out_b(ACE_u16_t port, ACE_u8_t data)
{
    __asm__ volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

static inline void USO_out_w(ACE_u16_t port, ACE_u16_t data)
{
    __asm__ volatile ("outw %0, %1" : : "a" (data), "Nd" (port));
}

static inline void USO_out_l(ACE_u16_t port, ACE_u32_t data)
{
    __asm__ volatile ("outl %0, %1" : : "a"(data), "Nd" (port));
}

/* Ein Byte an einen IO Port senden und fuer langsame Ports kurz verzoegern */
static inline void USO_out_b_wait(ACE_u16_t port, ACE_u8_t data)
{
    __asm__ volatile ("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port));
}

/**
 * Read data word by word(Short)
 * Data words at the addr are in Motorola Byte Order!
 *
 * @param addr
 * @param buffer
 * @param len
 */
extern void USO_in_w_stream (unsigned long port, unsigned long buffer, unsigned short len);

/**
 * Write data word by word(Short)
 * Data Words at the addr are in Motorola Byte Order!
 *
 * @param addr
 * @param buffer
 * @param len
 */
extern void USO_out_w_stream (unsigned long port, unsigned long buffer, unsigned short len);


#endif /* IO_PORTS_H_ */
