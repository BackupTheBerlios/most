#include <dev/arch/at91sam7x/AT91SAM7X256.h>

#include "arch/reset.h"

#define TOP_OF_MEMORY    (AT91C_ISRAM + AT91C_ISRAM_SIZE)
#define STACK_INIT_VALUE 0xA5A5A5A5 /* must be the same as in uso/stack.h */

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
.arm

//reset_handler:
//	ldr     pc, =_low_level_init

reset_handler:
				b     		_low_level_init
undefvec:
                b           undefvec            // 0x04 Undefined Instruction
swivec:
                b           swivec              // 0x08 Software Interrupt
pabtvec:
                b           pabtvec             // 0x0C Prefetch Abort
dabtvec:
                b           dabtvec             // 0x10 Data Abort
rsvdvec:
                b           rsvdvec             // 0x14 reserved
irqvec:
				b         	irqvec    			// 0x18 IRQ
fiqvec:
		        b         	fiqvec              // 0x1C FIQ

/*------------------------------------------------------------------------------
 *- Low level Init is performed in a C function: lowlevel_init
 *- Init Stack Pointer to a valid memory area before calling lowlevel_init
 *------------------------------------------------------------------------------*/
/*- Temporary stack in internal RAM for Low Level Init execution */

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
 *- Mark the stack as empty
 *------------------------------------------------------------------------------*/
		ldr      r2, =_lp_stack_mark_empty
        ldmia    r2, {r0-r3}
		sub		 r0, r0, r1
_stack_mark_empty:
		str		 r2, [r0]
		add		 r0, r0, #4
		sub		 r1, r1, #4
		cmp		 r1, r3
		bne		 _stack_mark_empty

/*------------------------------------------------------------------------------
 *- Setup the stack for each mode
 *------------------------------------------------------------------------------*/
_stack_init:
		ldr      r2, =_lp_stack_init
        ldmia    r2, {r0-r3}

/*- Set up Undefined Mode and set IRQ Mode Stack */
        msr      CPSR_c, #ARM_MODE_IRQ | I_BIT | F_BIT
        mov      sp, r0
        sub      r0, r0, r1

/*- Set up Abort Mode and set ABT Mode Stack */
        msr      CPSR_c, #ARM_MODE_ABT | I_BIT | F_BIT
        mov      sp, r0
        sub      r0, r0, r2

/*- Set up Interrupt Mode and set UND Mode Stack */
        msr      CPSR_c, #ARM_MODE_UND | I_BIT | F_BIT
        mov      sp, r0
        sub      r0, r0, r3

/*- Set up Supervisor Mode and set Supervisor Mode Stack */
        msr      CPSR_c, #ARM_MODE_SVC | I_BIT
        mov      sp, r0

/*------------------------------------------------------------------------------
 *- Branch to the SAM_init()
 *------------------------------------------------------------------------------*/
_branch_SAM_init:
        ldr     r0, = SAM_init
        mov     lr, pc
        bx      r0


	.globl SAM_jump_app
    .align 4
SAM_jump_app:
/*  first do remap (remap command toggles the memmory mapping) */
        ldr      r2, _lp_remap
        mov      r0, #AT91C_MC_RCB
        str      r0, [r2]
/*  then jump to app base */
        ldr      pc, =SAM_APP_BASE


	.globl SAM_jump_boot
    .align 4
SAM_jump_boot:
/*  first do remap (remap command toggles the memmory mapping) */
        ldr      r2, _lp_remap
        mov      r0, #AT91C_MC_RCB
        str      r0, [r2]
/*  then jump to boot base */
        ldr      pc, =SAM_BOOT_BASE

/*------------------------------------------------------------------------------
 *- Litteral pools
 *------------------------------------------------------------------------------*/
_lp_cpu_init:
        .word    SAM_cpu_init
        .word    TOP_OF_MEMORY              /* Default SVC stack after power up */ 

_lp_remap:
        .word    AT91C_MC_RCR

_lp_stack_init:
        .word    TOP_OF_MEMORY                 /* Top of the stack */
        .word    SAM_IRQ_STACK_SIZE            /* IRQ stack size */
        .word    SAM_ABT_STACK_SIZE            /* ABT stack size */
        .word    SAM_UND_STACK_SIZE            /* UND stack size */

_lp_stack_mark_empty:
        .word    TOP_OF_MEMORY            	   /* Top of the stack */
		.word    SAM_TOTAL_STACK_SIZE          /* Stack size of stack section in ld! */
		.word	 STACK_INIT_VALUE
		.word    0x0

