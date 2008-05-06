/*

  Copyright (C) 2008  Michael Feiner
 

--------------------------------------------------
 Comments: 

 CPU Architecture dependent functions which have
 to be written in assembler.
--------------------------------------------------
   extern void USO_enable(void)
   extern USO_cpu_status_t USO_disable(void)
   extern void USO_restore(USO_cpu_status_t)
   extern void USO_context_switch(USO_cpu_t *, USO_cpu_t *)
*/

#define I_BIT            0x80
#define F_BIT            0x40

	.arm
	.file	"cpu.S"	
	.section .text
	.align   2
   	.global  USO_enable
USO_enable:
	mrs r0, CPSR
	bic r0, r0, #I_BIT
	msr CPSR, r0
	bx	lr			

   	.global  USO_disable
USO_disable:	
	mrs r0, CPSR
	mov r1, r0
	orr r1, r1, #I_BIT
	msr CPSR, r1
	bx	lr			

   	.global  USO_restore
USO_restore:
	msr CPSR, r0
	bx	lr			

   	.global  USO_context_switch
USO_context_switch:       /* r0 &old_thread->cpu r1 &new_thread->cpu */  
	stmia   r0!,{r2-r14}
    mrs     r2,cpsr
    str     r2,[r0]
    ldr     r2,[r1,#(13*4)]
    msr	    cpsr,r2
    ldmia   r1,{r2-r14}
	bx		lr
	
   	.END
