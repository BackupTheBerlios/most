/*

  Copyright (C) 2004-2006  Michael Feiner
 

--------------------------------------------------
 Comments: 

 CPU Architecture dependent functions which have
 to be written in assembler.
 If your target is h8300 this file is copied
 or linked to cpu.s .
--------------------------------------------------
   extern void USO_enable(void)
   extern USO_cpu_status_t USO_disable(void)
   extern void USO_restore(USO_cpu_status_t)
   extern void USO_context_switch(USO_cpu_t *, USO_cpu_t *)
*/

#define I_BIT            0x80
#define UI_BIT	         0x40

   .h8300h
   .file    "cpu.S"
   .section  .text

   .align   1
   .global  _USO_enable
_USO_enable:
   andc.b   #~(I_BIT | UI_BIT),ccr        
   rts

   .align   1
   .global  _USO_disable
_USO_disable:	
   stc.b	ccr,r0l
   orc.b	#(I_BIT | UI_BIT),ccr
   rts

   .align   1
   .global  _USO_restore
_USO_restore:
   ldc.b    r0l,ccr
   rts

   .align   1
   .global  _USO_context_switch
_USO_context_switch:       /* er0 &old_thread->cpu er1 &new_thread->cpu */  
   mov.l    er2,@er0
   mov.l    er3,@(4,er0)
   mov.l    er4,@(8,er0)
   mov.l    er5,@(12,er0)
   mov.l    er6,@(16,er0) 
   mov.l    sp,@(20,er0)   /* save the stack pointer to old_thread->cpu.sp */ 
   pop.l    er2            /* pop the return address from the stack */
   mov.l    er2,@(24,er0)  /* move it to old_thread->cpu.pc */
   stc.b    ccr,r2l        /* get the cpu status */
   mov.b    r2l,@(28,er0)  /* and store it in old_thread->cpu.ps */

   mov.b    @(28,er1),r2l  /* get the new cpu status */
   ldc.b    r2l,ccr        /* and restor it */
   mov.l    @(24,er1),er2  /* copy new_thread->cpu.pc to a temp register */
   push.l   er2            /* and push it on the stack, after rts we will run on it */
   mov.l    @(20,er1),sp   /* copy new_thread->cpu.sp to sp */
   mov.l    @(16,er1),er6  
   mov.l    @(12,er1),er5
   mov.l    @(8,er1),er4
   mov.l    @(4,er1),er3
   mov.l    @er1,er2
   rts                     /* run on the new thread */
    
   .END
