;
;  Copyright (C) 2004-2006  Michael Feiner
; 

	.h8300h
	.file   "vectors.s"	
		
.MACRO  VECTOR n, label
    .section   .vectors
    jmp        @\label
.ENDM

.MACRO  STD_VECTOR n, label
    .section   .vectors
    jmp        std_vector
.ENDM

    .section   .text
    .global    std_vector
    
std_vector:
	rte
	
	VECTOR       0  , reset
	STD_VECTOR   1  , Reserved01
	STD_VECTOR   2  , Reserved02
	STD_VECTOR   3  , Reserved03
	STD_VECTOR   4  , Reserved04
	STD_VECTOR   5  , Reserved05
	STD_VECTOR   6  , Reserved06
	STD_VECTOR   7  , NMI
	STD_VECTOR   8  , Trap01
	STD_VECTOR   9  , Trap02
	STD_VECTOR   10 , Trap03
	STD_VECTOR   11 , Trap04
	
   	STD_VECTOR   12 , IRQ0  
	STD_VECTOR   13 , IRQ1
	STD_VECTOR   14 , IRQ2
	STD_VECTOR   15 , IRQ3
	STD_VECTOR   16 , IRQ4
	STD_VECTOR   17 , IRQ5
	STD_VECTOR   18 , IRQ6
	VECTOR       19 , _MDC_IRQ7_ISR   ;Eth0 

   ;; Internal interrupts
	STD_VECTOR   20 , WOVI            ; Watch Dog
	VECTOR       21 , _MDC_CMI_ISR    ; Refresh controller, Timer
	STD_VECTOR   22 , Reserved22
	STD_VECTOR   23 , Reserved23

   ;; ITU channel 0
	STD_VECTOR	 24 , IMIA0
	STD_VECTOR   25 , IMIB0
	STD_VECTOR	 26 , OVI0
	STD_VECTOR   27 , Reserved27

   ;; ITU channel 1
	STD_VECTOR   28 , IMIA1
	STD_VECTOR   29 , IMIB1
	STD_VECTOR   30 , OVI1
	STD_VECTOR   31 , Reserved31

   ;; ITU channel 2
	STD_VECTOR   32 , IMIA2
	STD_VECTOR   33 , IMIB2
	STD_VECTOR   34 , OVI2
	STD_VECTOR   35 , Reserved35

   ;; ITU channel 3
	STD_VECTOR   36 , IMIA3
	STD_VECTOR   37 , IMIB3
	STD_VECTOR   38 , OVI3
	STD_VECTOR   39 , Reserved39

   ;; ITU channel 4
	STD_VECTOR   40 , IMIA4
	STD_VECTOR   41 , IMIB4
	STD_VECTOR   42 , OVI4
	STD_VECTOR   43 , Reserved43
	
   ;; DMAC group 0
   	STD_VECTOR   44 , DEND0A
	STD_VECTOR   45 , DEND0B
	STD_VECTOR   46 , DEND1A
	STD_VECTOR   47 , DEND1B

   ;; DMAC group 1
	STD_VECTOR   48 , DEND2A
	STD_VECTOR   49 , DEND2B
	STD_VECTOR   50 , DEND3A
	STD_VECTOR   51 , DEND3B

   ;; SCI channel 0
	VECTOR       52 , _MDC_ERI0_ISR    
	VECTOR       53 , _MDC_RXI0_ISR    
	VECTOR       54 , _MDC_TXI0_ISR    
	VECTOR       55 , _MDC_TXE0_ISR    
	               
   ;; SCI channel 1
	VECTOR       56 , _MDC_ERI1_ISR    
	VECTOR       57 , _MDC_RXI1_ISR    
	VECTOR       58 , _MDC_TXI1_ISR    
	VECTOR       59 , _MDC_TXE1_ISR    

   ;; A/D
	STD_VECTOR   60 , ADI
	STD_VECTOR   61 , Reserved61
	STD_VECTOR   62 , Reserved62
	STD_VECTOR   63 , Reserved63

	.END
