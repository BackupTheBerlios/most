;
;  Copyright (C) 2004-2006  Michael Feiner
; 

	.h8300h
	.file   "vectors.S"

.MACRO  EXCEPTION n, label
    .section   .reset
    jmp        @\label
.ENDM

.MACRO  RAM_VECTOR n, label
    .section   .reset
     jmp        @_vectors_start + 4*\n
.ENDM
	
	EXCEPTION	 0  , reset
	RAM_VECTOR   1  , Reserved01
	RAM_VECTOR   2  , Reserved02
	RAM_VECTOR   3  , Reserved03
	RAM_VECTOR   4  , Reserved04
	RAM_VECTOR   5  , Reserved05
	RAM_VECTOR   6  , Reserved06
	RAM_VECTOR   7  , NMI
	RAM_VECTOR   8  , Trap01
	RAM_VECTOR   9  , Trap02
	RAM_VECTOR   10 , Trap03
	RAM_VECTOR   11 , Trap04
	
   	RAM_VECTOR   12 , IRQ0
	RAM_VECTOR   13 , IRQ1
	RAM_VECTOR   14 , IRQ2
	RAM_VECTOR   15 , IRQ3
	RAM_VECTOR   16 , IRQ4
	RAM_VECTOR   17 , IRQ5
	RAM_VECTOR   18 , IRQ6
	RAM_VECTOR   19 , IRQ7

   ;; Internal interrupts
	RAM_VECTOR   20 , WOVI
	RAM_VECTOR   21 , CMI
	RAM_VECTOR   22 , Reserved22
	RAM_VECTOR   23 , Reserved23

   ;; ITU channel 0
	RAM_VECTOR	 24 , IMIA0
	RAM_VECTOR   25 , IMIB0
	RAM_VECTOR	 26 , OVI0
	RAM_VECTOR   27 , Reserved27

   ;; ITU channel 1
	RAM_VECTOR   28 , IMIA1
	RAM_VECTOR   29 , IMIB1
	RAM_VECTOR   30 , OVI1
	RAM_VECTOR   31 , Reserved31

   ;; ITU channel 2
	RAM_VECTOR   32 , IMIA2
	RAM_VECTOR   33 , IMIB2
	RAM_VECTOR   34 , OVI2
	RAM_VECTOR   35 , Reserved35

   ;; ITU channel 3
	RAM_VECTOR   36 , IMIA3
	RAM_VECTOR   37 , IMIB3
	RAM_VECTOR   38 , OVI3
	RAM_VECTOR   39 , Reserved39

   ;; ITU channel 4
	RAM_VECTOR   40 , IMIA4
	RAM_VECTOR   41 , IMIB4
	RAM_VECTOR   42 , OVI4
	RAM_VECTOR   43 , Reserved43
	
   ;; DMAC group 0
   	RAM_VECTOR   44 , DEND0A
	RAM_VECTOR   45 , DEND0B
	RAM_VECTOR   46 , DEND1A
	RAM_VECTOR   47 , DEND1B

   ;; DMAC group 1
	RAM_VECTOR   48 , DEND2A
	RAM_VECTOR   49 , DEND2B
	RAM_VECTOR   50 , DEND3A
	RAM_VECTOR   51 , DEND3B

   ;; SCI channel 0
	RAM_VECTOR   52 , ERI0
	RAM_VECTOR   53 , RXI0
	RAM_VECTOR   54 , TXI0
	RAM_VECTOR   55 , TXE0
	               
   ;; SCI channel 1
	RAM_VECTOR   56 , ERI1
	RAM_VECTOR   57 , RXI1
	RAM_VECTOR   58 , TXI1
	RAM_VECTOR   59 , TXE1

   ;; A/D
	RAM_VECTOR   60 , ADI
	RAM_VECTOR   61 , Reserved61
	RAM_VECTOR   62 , Reserved62
	RAM_VECTOR   63 , Reserved63

	.END
