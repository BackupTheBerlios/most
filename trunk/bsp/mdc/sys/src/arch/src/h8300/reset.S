;
;  Copyright (C) 2004-2006  Michael Feiner
; 

	.h8300h
	.file    "reset.s"	
	.global  reset

    .section .unref

reset:
	mov.b	#0x3c,r0l
	mov.b	r0l,@_H8_PCDDR:8
	mov.l	#_stack_end,er7      
	mov.l	#_stack_start,er0
	mov.b	#0xA5,r1l
_mark_stack_empty:
	mov.b	r1l,@er0
	add		#1,er0
	cmp.l	er7,er0
	bne		_mark_stack_empty
	jmp	    _MDC_init

	.global _MDC_jump_app	

_MDC_jump_app:		
	jmp	    0x20000

	.section	.ramcode
	.global _MDC_jump_boot	

_MDC_jump_boot:		
	jmp	    0x10000
					
	.END
