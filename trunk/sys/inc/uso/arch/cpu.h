/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/arch/cpu.h"

#ifndef USO_CPU_H
#define USO_CPU_H

/** @defgroup cpu cpu.h
 *
 * Cpu architecture interface.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** Stack grows from hi addresses to lo addresses */
#define USO_HI2LOW 1

/** Stack grows from low addresses to hi addresses */
#define USO_LOW2HI 2


/*---------------------- I386 target ------------------------------*/

#ifdef ACE_CPU
#if ACE_CPU==ACE_CPU_I386

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


/*---------------------- H8300 target--------------------------------------*/

#elif ACE_CPU==ACE_CPU_H8300


#define USO_STACK USO_HI2LOW

typedef unsigned char USO_cpu_status_t;
typedef unsigned long USO_stack_t;
typedef unsigned long USO_cpu_register_t;

#define USO_CPU_PS_INIT ((USO_cpu_status_t)0x00)

struct USO_cpu
{
    USO_cpu_register_t pc;
    USO_cpu_register_t er0;
    USO_cpu_register_t er1;
    USO_cpu_register_t er2;
    USO_cpu_register_t er3;
    USO_cpu_register_t er4;
    USO_cpu_register_t er5;
    USO_cpu_register_t er6;
    USO_cpu_register_t sp;
    USO_cpu_status_t ps;
};

#endif
#endif

/*------------------------------------------------------------------------*/

/**
 * CPU type.
 */
typedef struct USO_cpu USO_cpu_t;


/*-------------- Interface -----------------------------------------------*/

/**
 * Enable all interrupts.
 */
extern void USO_enable (void);

/**
 * Disable all interrupts and return the CCR.
 */
extern USO_cpu_status_t USO_disable (void);

/**
 * Restore the CCR.
 *
 * @param status : Saved status which has to be written to ccr.  
 */
extern void USO_restore (USO_cpu_status_t status);

/**
 * Do a context switch.
 *
 * @param old : from old
 * @param new : to new
 */
extern void USO_context_switch (USO_cpu_t * old, USO_cpu_t * new);

/*------------------------------------------------------------------------*/

/** @}
 */


#endif
