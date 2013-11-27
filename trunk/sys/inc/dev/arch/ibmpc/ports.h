/*  
 * Copyright (c) 2006-2008 The tyndur Project. All rights reserved.
 *
 * This code is derived from software contributed to the tyndur Project
 * by Kevin Wolf.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT HOLDERS OR 
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */  

#ifndef IBMPC_PORTS_H_
#define IBMPC_PORTS_H_

#include <ace/stddef.h>

static inline ACE_u16_t inw(ACE_u16_t port)
{
    ACE_u16_t result;
    __asm__ volatile ("inw %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline ACE_u8_t inb(ACE_u16_t port)
{
    ACE_u8_t result;
    __asm__ volatile ("inb %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}

static inline ACE_u32_t inl(ACE_u16_t port)
{
    ACE_u32_t result;
    __asm__ volatile ("inl %1, %0" : "=a" (result) : "Nd" (port));
    return result;
}



static inline void outw(ACE_u16_t port, ACE_u16_t data)
{
    __asm__ volatile ("outw %0, %1" : : "a" (data), "Nd" (port));
}

static inline void outb(ACE_u16_t port, ACE_u8_t data)
{
    __asm__ volatile ("outb %0, %1" : : "a" (data), "Nd" (port));
}

static inline void outl(ACE_u16_t port, ACE_u32_t data)
{
    __asm__ volatile ("outl %0, %1" : : "a"(data), "Nd" (port));
}

/* Ein Byte an einen IO Port senden und fuer langsame Ports kurz verzoegern */
static inline void outb_wait(ACE_u16_t port, ACE_u8_t data)
{
    __asm__ volatile ("outb %0, %1\njmp 1f\n1: jmp 1f\n1:" : : "a" (data), "Nd" (port));
}

#endif

