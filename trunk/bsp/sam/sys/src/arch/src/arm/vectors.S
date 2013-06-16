#include <dev/arch/at91sam7x/AT91SAM7X256.h>

#include "arch/reset.h"

.section .vectors
.arm

/* Exception vectors (should be a branch to be detected as a valid code by the rom */
_exception_vectors:
reset_vector:
        ldr      pc, =reset_handler                /* Reset */
undef_vector:
        ldr      pc, =SAM_undefined_instruction    /* Undefined Instruction */
swi_vector:
        ldr      pc, =SAM_software_interrupt      /* Software Interrupt */
pabt_vector:
        ldr      pc, =SAM_prefetch_abort           /* Prefetch Abort */
dabt_vector:
        ldr      pc, =SAM_data_abort               /* Data Abort */
rsvd_vector:
        b        rsvd_vector                       /* Reserved */
irq_vector:
        b        irq_handler                       /* IRQ : read the AIC */
fiq_vector:
        ldr      pc, =SAM_default_fiq_handler          /* Fast interrupt request */

 /*------------------------------------------------------------------------------
 *- Function             : fiq_handler
 *- Treatments           : FIQ Interrupt Handler.
 *- Called Functions     : 
 *------------------------------------------------------------------------------*/
fiq_handler:
		b        fiq_handler
	
/*------------------------------------------------------------------------------
 *- Function             : irq_handler
 *- Treatments           : IRQ Controller Interrupt Handler.
 *- Called Functions     : AIC_IVR[interrupt]
 *------------------------------------------------------------------------------*/

.section .vectors
.arm

irq_handler:
/*- Save r0 , r1, lr and SPSR in IRQ stack */
        stmfd    sp!, {r0-r1, lr}
        mrs      r0, SPSR
        stmfd    sp!, {r0}

/*- Write in the IVR to support Protect Mode */
/*- No effect in Normal Mode */
/*- De-assert the NIRQ and clear the source in Protect Mode */
        ldr      r0, =AT91C_BASE_AIC
        ldr      r1, [r0, #AIC_IVR]
        str      r1, [r0, #AIC_IVR]
        str      r0, [r0, #AIC_EOICR]

/*- Switch in Supervisor Mode , dont enable interrupt */
        msr      CPSR_c, #ARM_MODE_SVC | I_BIT

/*- Save scratch/used registers and LR in Supervisor Stack */
        stmfd    sp!, {r2-r4, r12, lr}


/*- Branch to the routine pointed by the AIC_IVR */
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
