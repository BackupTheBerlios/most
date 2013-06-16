
#include <arch/reset.h>

// Before Remap the internal RAM it's 0x300000
// After  Remap the internal RAM it's 0x000000
// Internal RAM size is 16K = 0x4000

#define TOP_OF_MEMORY_REBOOT   (0x00300000 + 0x4000)
#define TOP_OF_MEMORY_REMAP    (0x00000000 + 0x4000)
#define STACK_INIT_VALUE 0xA5A5A5A5 /* must be the same as in uso/stack.h */

/* Application startup entry point */
        .globl reset_handler
        .align 4

.section .reset
.arm

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

_low_level_init:
   ldr   r1, =NB_CS
   ldr   r4, =AMC_CSR0_VAL            // Flash register value
   ldr   r3, =AMC_BASE_ADRESS
   str   r4, [r3, #AMC_CSR0]     // set the Flash register (CS0)
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 1 CS to configure

   ldr   r4, =AMC_CSR1_VAL            // SRAM register value
   str   r4, [r3, #AMC_CSR1]     // set the SRAM register (CS1)
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 2 CS to configure

   ldr   r4, =AMC_CSR2_VAL            // CS2 register value
   str   r4, [r3, #AMC_CSR2]     // set the CS2 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 3 CS to configure

   ldr   r4, =AMC_CSR3_VAL            // CS3 register value
   str   r4, [r3, #AMC_CSR3]     // set the CS3 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 4 CS to configure

   ldr   r4, =AMC_CSR4_VAL            // CS4 register value
   str   r4, [r3, #AMC_CSR4]     // set the CS4 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 5 CS to configure

   ldr   r4, =AMC_CSR5_VAL            // CS5 register value
   str   r4, [r3, #AMC_CSR5]     // set the CS5 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 6 CS to configure

   ldr   r4, =AMC_CSR6_VAL            // CS6 register value
   str   r4, [r3, #AMC_CSR6]     // set the CS6 register
   sub   r1, r1, #1
   cmp   r1, #0
   beq   _set_MCR_reg            // go to MCR_reg flag if 7 CS to configure

   ldr   r4, =AMC_CSR7_VAL            // CS7 register value
   str   r4, [r3, #AMC_CSR7]     // set the CS7 register

_set_MCR_reg:
   ldr   r4, =AMC_MCR_VAL            // Memory Controller Register value
   str   r4, [r3, #AMC_MCR]      // set the MCR register

//--------------------------------------------
//- Remap Command and jump on ABSOLUTE address
//--------------------------------------------
   ldr   r12, _remap             // load the address where to jump
   ldr   r3, =AMC_BASE_ADRESS   // point at the AMC base address
   mov   r4, #AMC_RCB            // value to write in MC_RCR
   str   r4, [r3, #AMC_RCR]      // remap command
// Before the remapping takes place, jump to Flash at its new address
   mov   pc, r12                 // jump and break the pipeline

//------------------------------------------------------------------------------
//- The Reset Handler after Remap
//-------------------------------
//- From here, the code is executed from its link address, ie. 0x100 0000.
//------------------------------------------------------------------------------

_remap:
        .word    _remap_addr

_remap_addr:

/*------------------------------------------------------------------------------
 *- Setup the stack for each mode
 *------------------------------------------------------------------------------*/
_stack_init:
		ldr      r2, =_lp_stack_init
        ldmia    r2, {r0-r3}

/*- Set up Undefined Mode and set IRQ Mode Stack */
 		mov      r4, #ARM_MODE_IRQ | I_BIT | F_BIT
        msr      CPSR_c, r4
        mov      sp, r0
        sub      r0, r0, r1

/*- Set up Abort Mode and set ABT Mode Stack */
        mov      r4, #ARM_MODE_ABT | I_BIT | F_BIT
        msr      CPSR_c, r4
        mov      sp, r0
        sub      r0, r0, r2

/*- Set up Interrupt Mode and set UND Mode Stack */
        mov      r4, #ARM_MODE_UND | I_BIT | F_BIT
        msr      CPSR_c, r4
        mov      sp, r0
        sub      r0, r0, r3

/*- Set up Supervisor Mode and set Supervisor Mode Stack */
        mov      r4, #ARM_MODE_SVC | I_BIT | F_BIT
        msr      CPSR_c, r4
        mov      sp, r0

/*------------------------------------------------------------------------------
 *- Mark the stack as empty
 *------------------------------------------------------------------------------*/
		ldr      r4, =_lp_stack_mark_empty
        ldmia    r4, {r0-r3}
		sub		 r0, r0, r1
_stack_mark_empty:
		str		 r2, [r0]
		add		 r0, r0, #4
		sub		 r1, r1, #4
		cmp		 r1, r3
		bne		 _stack_mark_empty

/*------------------------------------------------------------------------------
 *- Branch to the LA2_init()
 *------------------------------------------------------------------------------*/
_branch_LA2_init:
        ldr     r0, =LA2_init
        mov     lr, pc
        bx      r0



/*------------------------------------------------------------------------------
 *- Litteral pools
 *------------------------------------------------------------------------------*/


_lp_stack_init:
        .word    TOP_OF_MEMORY_REMAP           /* Top of the stack */
        .word    LA2_IRQ_STACK_SIZE            /* IRQ stack size */
        .word    LA2_ABT_STACK_SIZE            /* ABT stack size */
        .word    LA2_UND_STACK_SIZE            /* UND stack size */

_lp_stack_mark_empty:
        .word    TOP_OF_MEMORY_REMAP       	   /* Top of the stack */
		.word    LA2_TOTAL_STACK_SIZE          /* Stack size of stack section in ld! */
		.word	 STACK_INIT_VALUE
		.word    0x0



