#include <dev/arch/at91/AT91SAM7X256.h>

#include "arch/cpu.h"

#define TOP_OF_MEMORY    (AT91C_ISRAM + AT91C_ISRAM_SIZE)
#define ABT_STACK_SIZE   0x100
#define IRQ_STACK_SIZE   0x100


/* Application startup entry point */
        .globl reset_handler
        .align 4

/*------------------------------------------------------------------------------
 *- Function             : reset_handler
 *- Treatments           : Reset Interrupt Handler.
 *- Called Functions     : lowlevel_init
 *                         main
 *------------------------------------------------------------------------------*/
.section .reset
reset_handler:
	ldr     pc, =_low_level_init

/*------------------------------------------------------------------------------
 *- Low level Init is performed in a C function: lowlevel_init
 *- Init Stack Pointer to a valid memory area before calling lowlevel_init
 *------------------------------------------------------------------------------*/
/*- Temporary stack in internal RAM for Low Level Init execution */
.section .text

_low_level_init:
	ldr      r2, =_lp_cpu_init
        ldmia    r2, {r0, r1}
        mov      sp, r1
        mov      lr, pc
        bx       r0                      /* Branch on C function (interworking) */

/*------------------------------------------------------------------------------
 *- Remap SRAM at 0x0
 *------------------------------------------------------------------------------*/
_remap:
        ldr r2, _lp_remap
        mov r0, #AT91C_MC_RCB
        str r0, [r2]

/*------------------------------------------------------------------------------
 *- Setup the stack for each mode
 *------------------------------------------------------------------------------*/
_stack_init:
	ldr      r2, =_lp_stack_init
        ldmia    r2, {r0, r1, r2}

/*- Set up Abort Mode and set ABT Mode Stack */
        msr      CPSR_c, #ARM_MODE_ABT | I_BIT | F_BIT
        mov      sp, r0
        sub      r0, r0, r1

/*- Set up Interrupt Mode and set IRQ Mode Stack */
        msr      CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT
        mov      sp, r0
        sub      r0, r0, r2

/*- Set up Supervisor Mode and set Supervisor Mode Stack */
        msr      CPSR_c, #ARM_MODE_SVC | I_BIT | F_BIT
        mov      sp, r0

/*------------------------------------------------------------------------------
 *- Branch to the SAM_init()
 *------------------------------------------------------------------------------*/
_branch_SAM_init:
        ldr      r0, = SAM_init
        mov      lr, pc
        bx       r0

/*------------------------------------------------------------------------------
 *- Litteral pools
 *------------------------------------------------------------------------------*/
_lp_cpu_init:
        .word    SAM_cpu_init
        .word    TOP_OF_MEMORY              /* Default SVC stack after power up */ 

_lp_remap:
        .word    AT91C_MC_RCR

_lp_stack_init:
        .word    TOP_OF_MEMORY             /* Top of the stack */
        .word    ABT_STACK_SIZE            /* ABT stack size */
        .word    IRQ_STACK_SIZE            /* IRQ stack size */
