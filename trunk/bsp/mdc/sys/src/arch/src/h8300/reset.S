;
;  Copyright (C) 2004-2006  Michael Feiner
; 

	.h8300h
	.file    "reset.s"	
	.global  reset

reset:
	mov.b	#0x3c,r0l
	mov.b	r0l,@_h8_PCDDR:8
	mov.l	#_stack_end,er7      
	jmp	    _MDC_init

	.global _MDC_jump_app	

_MDC_jump_app:		
	jmp	    0x20000

	.section	.ramcode
	.global _MDC_jump_boot	

_MDC_jump_boot:		
	jmp	    0x10000
					
	.END
