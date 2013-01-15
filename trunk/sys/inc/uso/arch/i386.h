/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef USO_I386_H
#define USO_I386_H

#include <ace/stddef.h>

/** @addtogroup uso
 * @{
 */

/** @defgroup uso_i386 arch/i386.h
 *
 * i386 architecture interface.
 * @{
 */


/**
 * Kind of stack.
 *
 */
#define USO_STACK USO_HI2LOW

/**
 * Type of the CCR(Condition Code Register).
 */
typedef unsigned char USO_cpu_status_t;

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
typedef unsigned long USO_cpu_register_t;

/**
 * Initial value for the CCR.
 * It is used to initialize a thread.
 * All interrupts should be enabled!
 *
 */
#define USO_CPU_PS_INIT ((USO_cpu_status_t)0x00)

/**
 * CPU struct.
 * 
 * All registers of the CPU
 * which have to be saved when making a context switch.
 */
struct USO_cpu
{
    /** Programm counter */
    USO_cpu_register_t pc;

    /** Stack pointer */
    USO_cpu_register_t sp;

    /** Contition code register */
    USO_cpu_status_t ps;
};

/** @}
 */

/** @}
 */


#endif
