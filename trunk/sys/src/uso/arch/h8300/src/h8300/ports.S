/*
 * ports.S
 *
 *  Created on: 01.12.2013
 *      Author: maik
 */


   .h8300h
   .file    "ports.S"
   .section .text

   .align   1
   .global  _USO_in_w
_USO_in_w:
    push.w  r1
    mov.b   @(0,er0),r1l
    mov.b   @(1,er0),r1h
    mov.w   r1,r0
    pop.w   r1
    rts

   .align   1
   .global  _USO_out_w
_USO_out_w:
    push.w  r2
    mov.b   r1l,r2h
    mov.b   r1h,r2l
    mov.w   r2,@er0
    pop.w   r2
    rts

   .align   1
   .global  _USO_in_w_stream
_USO_in_w_stream:
    push.w  r4
__in_w_stream:
    mov.w   @er0,r4
    mov.w   r4,@er1
    inc.l   #2,er1
    dec.w   #1,r2
    bne __in_w_stream
    pop.w   r4
    rts

   .align   1
   .global  _USO_out_w_stream
_USO_out_w_stream:
    push.w  r4
__out_w_stream:
    mov.w   @er1+,r4
    mov.w   r4,@er0
    dec.w   #1,r2
    bne __out_w_stream
    pop.w   r4
    rts

   .END

