/*
 *  Copyright (C) 2004-2006  Michael Feiner
 *
 */

#ifndef LA2_RESET_H
#define LA2_RESET_H

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
#define LA2_IDLE_STACK_SIZE         0x280
#define LA2_UND_STACK_SIZE          0x80
#define LA2_ABT_STACK_SIZE          0x80
#define LA2_IRQ_STACK_SIZE          0x80
#define LA2_TOTAL_STACK_SIZE        0x400

#define AMC_BASE_ADRESS     0xFFE00000     // AMC Base Address
#define AMC_CSR0            0x00           // Chip Select Register 0 offset
#define AMC_CSR1            0x04           // Chip Select Register 1 offset
#define AMC_CSR2            0x08           // Chip Select Register 2 offset
#define AMC_CSR3            0x0C           // Chip Select Register 3 offset
#define AMC_CSR4            0x10           // Chip Select Register 4 offset
#define AMC_CSR5            0x14           // Chip Select Register 5 offset
#define AMC_CSR6            0x18           // Chip Select Register 6 offset
#define AMC_CSR7            0x1C           // Chip Select Register 7 offset

#define AMC_RCR             0x20           // Remap Control Register offset
#define AMC_MCR             0x24           // Memory Control Register offset

#define AMC_RCB         	0x01

/*-------------------------------*/
/* SAM7Board Memories Definition */
/*-------------------------------*/
/* 1-Mbytes 16-bit Flash */

/*  Chip Select Definition */
/*  FLASH: 3 waitstaits */
/*  SRAM: 0 (81) und 1(A1) waitstaits does not work!, 2(A5) maybe works, and 3(A9) works */
#define AMC_CSR0_VAL  0x400030A9    /* FLASH : Base Address = 0x40000000 */
                                 /* CS Enable, Byte Select access, 0 tdf,PS=4M (1MB Flash + DM9000E), 3 waitstates, 16 bits */
#define AMC_CSR1_VAL  0x480030A9    /* RAM  : Base Address = 0x480000000 */
                                 /*  CS Enable, Byte Select access, 0 tdf, PS=4 (2MB SRAM), 2 waitstates, 16 bits  */
#define AMC_CSR2_VAL  0x500030A9    /* RAM  : Base Address = 0x408000000 */
                                 /*  CS Enable, Byte Select access, 0 tdf, PS=4 (2MB SRAM), 2 waitstates, 16 bits  */
#define AMC_CSR3_VAL  0x58000000    /* Base Address = 0x58000000 CS Disable */
#define AMC_CSR4_VAL  0x60000000    /* Base Address = 0x60000000 CS Disable */
#define AMC_CSR5_VAL  0x68000000    /* Base Address = 0x68000000 CS Disable */
#define AMC_CSR6_VAL  0x70000000    /* Base Address = 0x70000000 CS Disable */
#define AMC_CSR7_VAL  0x78000000    /* Base Address = 0x78000000 CS Disable */
#define AMC_MCR_VAL   0x00000017    /* Memory Control Register Value Early Read Protocol, All chip selects valid */

#define NB_CS 3

#define GIC_BASE_ADRESS		0xFFFFF000
#define GIC_FVR             (260)
#define GIC_IVR             (256)
#define GIC_EOICR           (304)

#define LA2_BOOT_BASE    0x40000000
#define LA2_APP_BASE     0x40020000


/** @}
 */

/** @}
 */

#endif
