/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_CPU_I386_H
#define USO_CPU_I386_H

/** @addtogroup uso
 * @{
 */

/** @defgroup uso_i386 arch/i386.h
 *
 * i386 architecture interface.
 * @{
 */

#include <ace/stddef.h>

/**
 * Kind of stack.
 *
 */
#define USO_STACK_HI2LOW
#undef USO_STACK_LOW2HI

/**
 * Type of the CCR(Condition Code Register).
 */
typedef volatile unsigned long USO_cpu_status_t;

/**
 * Type of one entitiy of the stack array.
 */
typedef unsigned long USO_stack_t;

/**
 * Type of the CPU Registers.
 *
 * Programm Counter,
 * Stack Pointer,
 * Other Registers.
 */
typedef volatile unsigned long USO_cpu_register_t;

/**
 * Initial value for the CCR.
 * It is used to initialize a thread.
 * All interrupts should be enabled!
 * IRQs einschalten (IF = 1)
 */
#define USO_CPU_PS_INIT ((USO_cpu_status_t)0x202)

/**
 * CPU struct.
 * 
 * All registers of the CPU
 * which have to be saved when making a context switch.
 */
struct USO_cpu
{
    USO_cpu_register_t eax;
    USO_cpu_register_t ebx;
    USO_cpu_register_t ecx;
    USO_cpu_register_t edx;
    USO_cpu_register_t esi; /** Soure index */
    USO_cpu_register_t edi; /** Destination index */
    USO_cpu_register_t ebp; /** Base pointer */
    USO_cpu_register_t sp; /** Stack pointer (esp)*/
    USO_cpu_register_t pc; /** Programm counter (eip) */
    USO_cpu_status_t ps; /** Contition code register (eflags)*/
};


/** @}
 */

/** @}
 */


#endif
