/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef SAM_RESET_H
#define SAM_RESET_H

/** @addtogroup arch
 *
 * @{
 */

/** @defgroup reset reset.h
 *
 * @{
 */

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

/* Take care! This Stack sizes must match the definitions in *.ld! */
#define SAM_IDLE_STACK_SIZE         0x280
#define SAM_UND_STACK_SIZE          0x80
#define SAM_ABT_STACK_SIZE          0x80
#define SAM_IRQ_STACK_SIZE          0x80
#define SAM_TOTAL_STACK_SIZE        0x400

#define SAM_BOOT_BASE    0x100000
#define SAM_APP_BASE     0x114000


/** @}
 */

/** @}
 */

#endif
