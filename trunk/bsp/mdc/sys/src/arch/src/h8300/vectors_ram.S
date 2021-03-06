;
;  Copyright (C) 2004-2006  Michael Feiner
; 

	.h8300h
	.file   "vectors_ram.S"
		
.MACRO  EXCEPTION n, label
    .section   .vectors
    jmp        @\label
.ENDM

.MACRO  STD_EXCEPTION n, label
    .section   .vectors
    jmp        std_exception
.ENDM

    .section   .unref
    .global	   std_exception

std_exception:
	bra      std_exception
	
	EXCEPTION       0  , reset
	STD_EXCEPTION   1  , Reserved01
	STD_EXCEPTION   2  , Reserved02
	STD_EXCEPTION   3  , Reserved03
	STD_EXCEPTION   4  , Reserved04
	STD_EXCEPTION   5  , Reserved05
	STD_EXCEPTION   6  , Reserved06
	STD_EXCEPTION   7  , NMI
	STD_EXCEPTION   8  , Trap01
	STD_EXCEPTION   9  , Trap02
	STD_EXCEPTION   10 , Trap03
	STD_EXCEPTION   11 , Trap04
	
   	STD_EXCEPTION   12 , IRQ0  
	STD_EXCEPTION   13 , IRQ1
	STD_EXCEPTION   14 , IRQ2
	STD_EXCEPTION   15 , IRQ3
	STD_EXCEPTION   16 , IRQ4
	STD_EXCEPTION   17 , IRQ5
	STD_EXCEPTION   18 , IRQ6
	EXCEPTION       19 , _MDC_IRQ7_ISR   ;Eth0 

   ;; Internal interrupts
	STD_EXCEPTION   20 , WOVI            ; Watch Dog
	EXCEPTION       21 , _MDC_CMI_ISR    ; Refresh controller, Timer
	STD_EXCEPTION   22 , Reserved22
	STD_EXCEPTION   23 , Reserved23

   ;; ITU channel 0
	STD_EXCEPTION	 24 , IMIA0
	STD_EXCEPTION   25 , IMIB0
	STD_EXCEPTION	 26 , OVI0
	STD_EXCEPTION   27 , Reserved27

   ;; ITU channel 1
	STD_EXCEPTION   28 , IMIA1
	STD_EXCEPTION   29 , IMIB1
	STD_EXCEPTION   30 , OVI1
	STD_EXCEPTION   31 , Reserved31

   ;; ITU channel 2
	STD_EXCEPTION   32 , IMIA2
	STD_EXCEPTION   33 , IMIB2
	STD_EXCEPTION   34 , OVI2
	STD_EXCEPTION   35 , Reserved35

   ;; ITU channel 3
	STD_EXCEPTION   36 , IMIA3
	STD_EXCEPTION   37 , IMIB3
	STD_EXCEPTION   38 , OVI3
	STD_EXCEPTION   39 , Reserved39

   ;; ITU channel 4
	STD_EXCEPTION   40 , IMIA4
	STD_EXCEPTION   41 , IMIB4
	STD_EXCEPTION   42 , OVI4
	STD_EXCEPTION   43 , Reserved43
	
   ;; DMAC group 0
   	STD_EXCEPTION   44 , DEND0A
	STD_EXCEPTION   45 , DEND0B
	STD_EXCEPTION   46 , DEND1A
	STD_EXCEPTION   47 , DEND1B

   ;; DMAC group 1
	STD_EXCEPTION   48 , DEND2A
	STD_EXCEPTION   49 , DEND2B
	STD_EXCEPTION   50 , DEND3A
	STD_EXCEPTION   51 , DEND3B

   ;; SCI channel 0
	EXCEPTION       52 , _MDC_ERI0_ISR    
	EXCEPTION       53 , _MDC_RXI0_ISR    
	EXCEPTION       54 , _MDC_TXI0_ISR    
	EXCEPTION       55 , _MDC_TXE0_ISR    
	               
   ;; SCI channel 1
	EXCEPTION       56 , _MDC_ERI1_ISR    
	EXCEPTION       57 , _MDC_RXI1_ISR    
	EXCEPTION       58 , _MDC_TXI1_ISR    
	EXCEPTION       59 , _MDC_TXE1_ISR    

   ;; A/D
	STD_EXCEPTION   60 , ADI
	STD_EXCEPTION   61 , Reserved61
	STD_EXCEPTION   62 , Reserved62
	STD_EXCEPTION   63 , Reserved63

	.END
