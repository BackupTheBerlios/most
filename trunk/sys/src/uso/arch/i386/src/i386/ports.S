/*
 * ports.S
 *
 *  Created on: 01.12.2013
 *      Author: maik
 *  not done now !!! just dummy
 */


    .file    "ports.S"
    .section .text
    .align   4


    .global  _USO_in_w_stream
_USO_in_w_stream:
    push %ebp
    mov %esp, %ebp
__in_w_stream:
    mov %ebp, %esp
    pop %ebp
    ret

   .global  _USO_out_w_stream
_USO_out_w_stream:
    push %ebp
    mov %esp, %ebp
__out_w_stream:
    mov %ebp, %esp
    pop %ebp
    ret

   .END

