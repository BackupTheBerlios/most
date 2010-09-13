/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#include "ace/arch/cpu.h"
#include "ace/stddef.h"

#ifndef USO_CPU_H
#define USO_CPU_H

/** @defgroup cpu cpu.h
 *
 * Cpu architecture interface.
 * @{
 */

/*------------- Representation ------------------------------------------*/

/** Stack grows from hi addresses to lo addresses */
#define USO_HI2LOW 21

/** Stack grows from low addresses to hi addresses */
#define USO_LOW2HI 12


#ifdef ACE_CPU
/*---------------------- H8300 cpu  --------------------------------------*/
#if ACE_CPU==ACE_CPU_H8300


#define USO_STACK USO_HI2LOW

typedef unsigned char USO_cpu_status_t;
typedef unsigned long USO_stack_t;
typedef unsigned long USO_cpu_register_t;

#define USO_CPU_PS_INIT ((USO_cpu_status_t)0x00)

struct USO_cpu
{
    USO_cpu_register_t er2;
    USO_cpu_register_t er3;
    USO_cpu_register_t er4;
    USO_cpu_register_t er5;
    USO_cpu_register_t er6;
    USO_cpu_register_t sp;
    USO_cpu_register_t pc;
    USO_cpu_status_t ps;
};
/*---------------------- I386 cpu ------------------------------*/
#elif ACE_CPU==ACE_CPU_I386

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
/*---------------------- ARM cpu --------------------------------------*/
#elif ACE_CPU==ACE_CPU_ARM

#define USO_STACK USO_HI2LOW

typedef unsigned long USO_cpu_status_t;
typedef unsigned long USO_stack_t;
typedef unsigned long USO_cpu_register_t;

/* Normal user code, no full access to CPSR  */
#define ARM_MODE_USR     0x10   /* PC, R14...R0, CPSR */

/* All exception modes and system mode are privileged modes, full access to CPSR */

/* Fast interrupt, not handled by the OS */
#define ARM_MODE_FIQ     0x11   /* PC, R14fiq...R8fiq, R7...R0, CPSR, SPSRfiq */

/* Interrupts */
#define ARM_MODE_IRQ     0x12   /* PC, R14irq, R13irq, R12...R0, CPSR, SPSRirq */

/* Supervisor mode, privilegierte exception mode, Eintritt z.B. durch SWI */
#define ARM_MODE_SVC     0x13   /* PC, R14svc, R13scv, R12...R0, CPSR, SPSRsvc */

/* Prefetch-, Data-abort, tritt auf wenn eine Datenanforderung nicht erfuellt werden kann */
#define ARM_MODE_ABT     0x17   /* PC, R14abt, R13abt, R12...R0, CPSR, SPSRabt */

/* Undefined instruction, Eintritt durch Auftreten eines unbekanten Befehls */
#define ARM_MODE_UND     0x1B   /* PC, R14und, R13und, R12...R0, CPSR, SPSRund */

/* System mode, privilegierter code, modify CPSR to enter */
/* System mode was introduced to handle nested interrupts, MOST does not support nested interrupts */
#define ARM_MODE_SYS     0x1F   /* PC, R14...R0, CPSR */

#define I_BIT            0x80
#define F_BIT            0x40

#define USO_CPU_PS_INIT ((USO_cpu_status_t)ARM_MODE_SVC)

struct USO_cpu
{
    USO_cpu_register_t r2;
    USO_cpu_register_t r3;
    USO_cpu_register_t r4;
    USO_cpu_register_t r5;
    USO_cpu_register_t r6;
    USO_cpu_register_t r7;
    USO_cpu_register_t r8;
    USO_cpu_register_t r9;
    USO_cpu_register_t r10;
    USO_cpu_register_t r11;
    USO_cpu_register_t r12;
    USO_cpu_register_t sp;
    USO_cpu_register_t pc; // lr contains the return address
    USO_cpu_status_t ps;
    USO_cpu_status_t sps;
};

#else
#error "ACE CPU not supported!" 
#endif
#else
#error "ACE CPU not defined!" 
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
