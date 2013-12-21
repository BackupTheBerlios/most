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

    .file     "cpu.S"
    .section  .text
    .align    4

    .global  USO_enable
USO_enable:
    sti
    ret

    .global  USO_disable
USO_disable:
    pushf
    cli
    pop %eax
    ret

    .global  USO_restore
USO_restore:
    push 4(%esp)
    popf
    ret

    .global  USO_context_switch
USO_context_switch:       /* param 1( &old_thread->cpu ) and param 2( &new_thread->cpu )  are on the stack*/
    // &old_thread->cpu nach eax laden
    push %eax
    mov 8(%esp), %eax

    mov %ebx, 4(%eax)
    pop %ebx
    mov %ebx, (%eax)
    mov %ecx, 8(%eax)
    mov %edx, 12(%eax)
    mov %esi, 16(%eax)
    mov %edi, 20(%eax)
    mov %ebp, 24(%eax)
    pop 32(%eax)       // pop return adress from stack and save it in old->pc
    pop %ebx           // pop first param
    pop %ecx           // pop second param
    pushf              // push flags to stack
    pop 36(%eax)       // pop flags to ps
    mov %esp, 28(%eax)  // save sp

    // &new_thread->cpu nach eax laden
    mov %ecx, %eax

    mov 28(%eax), %esp   // get new sp
    push %ecx            // restore stack param 2
    push %ebx            //               param 1
    push 32(%eax)       //  push pc on the stack, after ret we hopefully run on it
    mov 4(%eax), %ebx
    mov 8(%eax), %ecx
    mov 12(%eax), %edx
    mov 16(%eax), %esi
    mov 20(%eax), %edi
    mov 24(%eax), %ebp
    push 36(%eax)       // save ps on stack
    popf                // and pop flags from stack
    mov (%eax), %eax

    ret

    .END
