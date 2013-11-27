/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */


#ifndef USO_ARM_H
#define USO_ARM_H

#include <ace/stddef.h>

/** @addtogroup uso
 * @{
 */

/** @defgroup uso_arm arch/arm.h
 *
 * ARM architecture interface.
 * @{
 */

#define USO_STACK_HI2LOW
#undef USO_STACK_LOW2HI

typedef volatile unsigned long USO_cpu_status_t;
typedef unsigned long USO_stack_t;
typedef volatile unsigned long USO_cpu_register_t;

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
    USO_cpu_register_t pc;      // lr contains the return address
    USO_cpu_status_t ps;
    USO_cpu_status_t sps;
};


/** @}
 */

/** @}
 */


#endif
