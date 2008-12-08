#include <dev/arch/at91/AT91SAM7X256.h>

#include "arch/cpu.h"

.section .vectors
.arm

/* Exception vectors (should be a branch to be detected as a valid code by the rom */
_exception_vectors:
reset_vector:
        ldr      pc, =reset_handler
undef_vector:
        b        undef_vector    /* Undefined Instruction */
swi_vector:
        b        swi_vector      /* Software Interrupt */
pabt_vector:
        ldr      pc, =SAM_prefetch_abort    /* Prefetch Abort */
dabt_vector:
        ldr      pc, =SAM_data_abort     /* Data Abort */
rsvd_vector:
        b        rsvd_vector     /* reserved */
irq_vector:
        b        irq_handler     /* IRQ : read the AIC */
fiq_vector:
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
irq_handler:
/*- Save LR_irq in IRQ stack */
        stmfd    sp!, {lr}
/*- Save r0 and SPSR in IRQ stack */
        mrs      r14, SPSR
        stmfd    sp!, {r0,r14}

/*- Write in the IVR to support Protect Mode */
/*- No effect in Normal Mode */
/*- De-assert the NIRQ and clear the source in Protect Mode */
        ldr      r14, =AT91C_BASE_AIC
        ldr      r0 , [r14, #AIC_IVR]
        str      r14, [r14, #AIC_IVR]

/*- Switch in Supervisor Mode , dont enable interrupt */
        msr      CPSR_c, #ARM_MODE_SVC | I_BIT

/*- Save scratch/used registers and LR in User Stack */
        stmfd    sp!, {r1-r3, r12, r14}


/*- Branch to the routine pointed by the AIC_IVR */
        mov      r14, pc
        bx       r0

/*- Restore scratch/used registers and LR from User Stack */
       ldmia    sp!, {r1-r3, r12, r14}

/*-  and switch back in IRQ mode */
        msr      CPSR_c, #ARM_MODE_IRQ | I_BIT

/*- Mark the End of Interrupt on the AIC */
        ldr      r14, =AT91C_BASE_AIC
        str      r14, [r14, #AIC_EOICR]

/*- Restore SPSR_irq and r0 from IRQ stack */
        ldmia    sp!, {r0,r14}
        msr      SPSR_cxsf, r14

/*- Restore LR_irq from IRQ stack and finish interrupt */
       ldmia    sp!, {lr}
       subs		pc, lr, #4
