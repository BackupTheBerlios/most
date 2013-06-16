/*
 * vectors.S
 *
 *  Created on: 29.04.2013
 *      Author: maik
 */


#include <arch/reset.h>

        .globl exception_vectors
        .align 4

.section .vectors
.arm


/* Exception vectors (should be a branch to be detected as a valid code by the rom */
exception_vectors:
reset_vector:
                ldr         pc, =reset_handler       // 0x00 Reset handler
undef_vector:
                ldr         pc, =LA2_undefined_instruction             // 0x04 Undefined Instruction
swi_vector:
                ldr         pc, =LA2_software_interrupt               // 0x08 Software Interrupt
pabt_vector:
                ldr         pc, =LA2_prefetch_abort              // 0x0C Prefetch Abort
dabt_vector:
                ldr         pc, =LA2_data_abort              // 0x10 Data Abort
rsvd_vector:
                b           rsvd_vector              // 0x14 reserved
irq_vector:
		        b         	irq_handler              // 0x18 IRQ
fiq_vector:
				ldr         pc, =LA2_default_fiq_handler               // 0x1C FIQ

fiq_handler:
				b     		fiq_handler


irq_handler:
/*- Save r0 , r1, lr and SPSR in IRQ stack */
        stmfd    sp!, {r0-r1, lr}
        mrs      r0, SPSR
        stmfd    sp!, {r0}

	    ldr         r0, =GIC_BASE_ADRESS
  		ldr         r1, [r0, #GIC_IVR]
        str     	r1, [r0, #GIC_IVR]
        str      	r0, [r0, #GIC_EOICR]

/*- Switch in Supervisor Mode , dont enable interrupt */
        msr      CPSR_c, #ARM_MODE_SVC | I_BIT

/*- Save scratch/used registers and LR in Supervisor Stack */
        stmfd    sp!, {r2-r4, r12, lr}


/*- Branch to the routine pointed by the GIC_IVR */
        mov      lr, pc
        bx       r1

/*- Restore scratch/used registers and LR from Supervisor Stack */
        ldmfd    sp!, {r2-r4, r12, lr}

/*-  and switch back in IRQ mode */
        msr      CPSR_c, #ARM_MODE_IRQ | I_BIT

/*- Restore SPSR_irq and r0,r1, lr from IRQ stack */
        ldmfd    sp!, {r0}
        msr      SPSR, r0
        ldmfd    sp!, {r0-r1, lr}

/*- Finish interrupt */
        subs	 pc, lr, #4

.end
