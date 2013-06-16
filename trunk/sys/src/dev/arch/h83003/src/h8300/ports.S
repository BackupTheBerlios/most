   .h8300h
   .file    "ports.S"
   .section .text

   .align   1
   .global  _DEV_in_w
_DEV_in_w:
	push.w	r1
	mov.b	@(0,er0),r1l
	mov.b	@(1,er0),r1h
	mov.w   r1,r0
	pop.w	r1
	rts

   .align   1
   .global  _DEV_out_w
_DEV_out_w:
	push.w  r2
	mov.b	r0l,r2h
	mov.b   r0h,r2l
	mov.w	r2,@er1
	pop.w   r2
	rts

   /* void in_nw (ioaddr, buffer, len) */
   .align   1
   .global  _DEV_in_nw
_DEV_in_nw:
	push.w	r4
__in_nw1:
	mov.w   @er0,r4
	mov.w   r4,@er1
	inc.l   #2,er1
	dec.w   #1,r2
	bne	__in_nw1
	pop.w	r4
	rts

   /* void out_nw(ioaddr, buf, len) */
   .align   1
   .global  _DEV_out_nw
_DEV_out_nw:
	push.w	r4
__out_nw1:
	mov.w   @er1+,r4
	mov.w   r4,@er0
	dec.w   #1,r2
	bne	__out_nw1
	pop.w	r4
	rts

	.END
