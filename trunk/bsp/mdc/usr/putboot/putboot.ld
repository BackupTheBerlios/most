OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("reset")

_ram_size	= 128K;
_stack_size	= 1K;

MEMORY
{
	rom(rx)		: o = 0x000000, l = 64K
	ram(rw)		: o = 0xe00000, l = 128K
	tiny(rw)	: o = 0xff8000, l = 0x0007f00
	eight(rw)	: o = 0xffff00, l = 0x0000100
}

SECTIONS                                
{
	.text :
	{
		_code_start = . ;
		_text_start = . ;
 		*(.exceptn)                     
		*(.text)                                
		_text_end = . ;
	} > rom

	.rodata :
	{
		_rodata_start = . ;
		*(.rodata)
		_rodata_end	= . ;
		_code_end = . ;
	} > rom

	.data :
	{
		_ram_start	= . ;
		_data_start	= . ;
		_vectors_start = . ;
 		*(.vectors)                     
		*(.ramcode)
		*(.data)
		_data_end = . ;
	} > ram

	_signature = LOADADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data);

	.bss :
	{
		_bss_start = . ;
		*(.bss)
		*(COMMON)
		_bss_end = . ;
	} > ram

/*
	.noinit (NOLOAD):
	{
		_noinit_start = . ;
		*(.noinit)
		_noinit_end	= . ;
	} > ram
*/
	.heap (NOLOAD):
	{
		_heap_start = . ;
		*(.heap)
		. = _ram_start  + _ram_size - _stack_size - 0;
		_heap_end = . ;
	} > ram

	.stack (NOLOAD):
	{
		_stack_start = . ;
		*(.stack)
		. = _ram_start + _ram_size - 0;
		_stack_end = . ;
		_ram_end = . ;
	} > ram

/*
	.tiny :
	{
		*(.tiny)
	} > tiny
*/

	.eight (NOLOAD):
	{
		*(.eight)
	
		/* System control */
		
		_h8_MDCR           = 0xF1;  /* Mode control               **  8 bit not used */
		_h8_SYSCR          = 0xF2;  /* System control             **  8 bit     used */
	
		
		/* Interrupt controller */
		
		_h8_ISCR           = 0xF4;  /* IRQ sense control          **  8 bit     used */
		_h8_IER            = 0xF5;  /* IRQ enable                 **  8 bit     used */
		_h8_ISR            = 0xF6;  /* IRQ status                 **  8 bit     used */
		_h8_IPRA           = 0xF8;  /* Interrupt priority A       **  8 bit     used */  
		_h8_IPRB           = 0xF9;  /* Interrupt priority B       **  8 bit     used */
	
		
		/* Bus controller */
		
		_h8_ABWCR          = 0xEC;  /* Bus width control          **  8 bit     used */
		_h8_ASTCR          = 0xED;  /* Access state control       **  8 bit     used */
		_h8_WCR            = 0xEE;  /* Wait control               **  8 bit     used */
		_h8_WCER           = 0xEF;  /* Wait state controller enab **  8 bit     used */
		_h8_BRCR           = 0xF3;  /* Bus release control        **  8 bit     used */
		
		
		/* Refresh controller */
		
		_h8_RFSHCR         = 0xAC;  /* Refresh control            **  8 bit not used */
		_h8_RTMCSR         = 0xAD;  /* Refresh timer contrl/status**  8 bit not used */
		_h8_RTCNT          = 0xAE;  /* Refresh timer counter      **  8 bit not used */
		_h8_RTCOR          = 0xAF;  /* Refresh time constant      **  8 bit not used */
		
		
		/* DMA controller */
		
		_h8_DMAC_MAR0AR    = 0x20;  /* Memory address         0AR **  8 bit not used */
		_h8_DMAC_MAR0AE    = 0x21;  /* Memory address         0AE **  8 bit not used */
		_h8_DMAC_MAR0AH    = 0x22;  /* Memory address         0AH **  8 bit not used */
		_h8_DMAC_MAR0AL    = 0x23;  /* Memory address         0AL **  8 bit not used */
		_h8_DMAC_MAR0A     = 0x20;  /* Memory address         0A  ** 32 bit     used */
		_h8_DMAC_ETCR0AH   = 0x24;  /* Execute transfer count 0AH **  8 bit     used */
		_h8_DMAC_ETCR0AL   = 0x25;  /* Execute transfer count 0AL **  8 bit     used */
		_h8_DMAC_ETCR0A    = 0x24;  /* Execute transfer count 0A  ** 16 bit not used */
		_h8_DMAC_IOAR0A    = 0x26;  /* I/O address            0A  **  8 bit not used */
		_h8_DMAC_DTCR0A    = 0x27;  /* Data transfer control  0A  **  8 bit not used */
	
		_h8_DMAC_MAR0BR    = 0x28;  /* Memory address         0BR **  8 bit not used */
		_h8_DMAC_MAR0BE    = 0x29;  /* Memory address         0BE **  8 bit not used */
		_h8_DMAC_MAR0BH    = 0x2A;  /* Memory address         0BH **  8 bit not used */
		_h8_DMAC_MAR0BL    = 0x2B;  /* Memory address         0BL **  8 bit not used */
		_h8_DMAC_MAR0B     = 0x28;  /* Memory address         0B  ** 32 bit     used */
		_h8_DMAC_ETCR0BH   = 0x2C;  /* Execute transfer count 0BH **  8 bit not used */
		_h8_DMAC_ETCR0BL   = 0x2D;  /* Execute transfer count 0BL **  8 bit not used */
		_h8_DMAC_ETCR0B    = 0x2C;  /* Execute transfer count 0B  ** 16 bit     used */
		_h8_DMAC_IOAR0B    = 0x2E;  /* I/O address            0B  **  8 bit not used */ 
		_h8_DMAC_DTCR0B    = 0x2F;  /* Data transfer control  0B  **  8 bit     used */ 
	
		_h8_DMAC_MAR1AR    = 0x30;  /* Memory address         1AR **  8 bit not used */
		_h8_DMAC_MAR1AE    = 0x31;  /* Memory address         1AE **  8 bit not used */
		_h8_DMAC_MAR1AH    = 0x32;  /* Memory address         1AH **  8 bit not used */
		_h8_DMAC_MAR1AL    = 0x33;  /* Memory address         1AL **  8 bit not used */
		_h8_DMAC_MAR1A     = 0x30;  /* Memory address         1A  ** 32 bit not used */
		_h8_DMAC_ETCR1AH   = 0x34;  /* Execute transfer count 1AH **  8 bit not used */
		_h8_DMAC_ETCR1AL   = 0x35;  /* Execute transfer count 1AL **  8 bit not used */
		_h8_DMAC_ETCR1A    = 0x34;  /* Execute transfer count 1A  ** 16 bit not used */
		_h8_DMAC_IOAR1A    = 0x36;  /* I/O address            1A  **  8 bit not used */
		_h8_DMAC_DTCR1A    = 0x37;  /* Data transfer control  1A  **  8 bit not used */
	
		_h8_DMAC_MAR1BR    = 0x38;  /* Memory address         1BR **  8 bit not used */
		_h8_DMAC_MAR1BE    = 0x39;  /* Memory address         1BE **  8 bit not used */
		_h8_DMAC_MAR1BH    = 0x3A;  /* Memory address         1BH **  8 bit not used */
		_h8_DMAC_MAR1BL    = 0x3B;  /* Memory address         1BL **  8 bit not used */
		_h8_DMAC_MAR1B     = 0x38;  /* Memory address         1B  ** 32 bit not used */
		_h8_DMAC_ETCR1BH   = 0x3C;  /* Execute transfer count 1BH **  8 bit not used */
		_h8_DMAC_ETCR1BL   = 0x3D;  /* Execute transfer count 1BL **  8 bit not used */
		_h8_DMAC_ETCR1B    = 0x3C;  /* Execute transfer count 1B  ** 16 bit not used */
		_h8_DMAC_IOAR1B    = 0x3E;  /* I/O address            1B  **  8 bit not used */
		_h8_DMAC_DTCR1B    = 0x3F;  /* Data transfer control  1B  **  8 bit not used */
	
		_h8_DMAC_MAR2AR    = 0x40;  /* Memory address         2AR **  8 bit not used */
		_h8_DMAC_MAR2AE    = 0x41;  /* Memory address         2AE **  8 bit not used */
		_h8_DMAC_MAR2AH    = 0x42;  /* Memory address         2AH **  8 bit not used */
		_h8_DMAC_MAR2AL    = 0x43;  /* Memory address         2AL **  8 bit not used */
		_h8_DMAC_MAR2A     = 0x40;  /* Memory address         2A  ** 32 bit not used */
		_h8_DMAC_ETCR2AH   = 0x44;  /* Execute transfer count 2AH **  8 bit not used */
		_h8_DMAC_ETCR2AL   = 0x45;  /* Execute transfer count 2AL **  8 bit not used */
		_h8_DMAC_ETCR2A    = 0x44;  /* Execute transfer count 2A  ** 16 bit not used */
		_h8_DMAC_IOAR2A    = 0x46;  /* I/O address            2A  **  8 bit not used */
		_h8_DMAC_DTCR2A    = 0x47;  /* Data transfer control  2A  **  8 bit not used */
	
		_h8_DMAC_MAR2BR    = 0x48;  /* Memory address         2BR **  8 bit not used */
		_h8_DMAC_MAR2BE    = 0x49;  /* Memory address         2BE **  8 bit not used */
		_h8_DMAC_MAR2BH    = 0x4A;  /* Memory address         2BH **  8 bit not used */
		_h8_DMAC_MAR2BL    = 0x4B;  /* Memory address         2BL **  8 bit not used */
		_h8_DMAC_MAR2B     = 0x48;  /* Memory address         2B  ** 32 bit not used */
		_h8_DMAC_ETCR2BH   = 0x4C;  /* Execute transfer count 2BH **  8 bit not used */
		_h8_DMAC_ETCR2BL   = 0x4D;  /* Execute transfer count 2BL **  8 bit not used */
		_h8_DMAC_ETCR2B    = 0x4C;  /* Execute transfer count 2B  ** 16 bit not used */
		_h8_DMAC_IOAR2B    = 0x4E;  /* I/O address            2B  **  8 bit not used */
		_h8_DMAC_DTCR2B    = 0x4F;  /* Data transfer control  2B  **  8 bit not used */
	
		_h8_DMAC_MAR3AR    = 0x50;  /* Memory address         3AR **  8 bit not used */
		_h8_DMAC_MAR3AE    = 0x51;  /* Memory address         3AE **  8 bit not used */
		_h8_DMAC_MAR3AH    = 0x52;  /* Memory address         3AH **  8 bit not used */
		_h8_DMAC_MAR3AL    = 0x53;  /* Memory address         3AL **  8 bit not used */
		_h8_DMAC_MAR3A     = 0x50;  /* Memory address         3A  ** 32 bit not used */
		_h8_DMAC_ETCR3AH   = 0x54;  /* Execute transfer count 3AH **  8 bit not used */
		_h8_DMAC_ETCR3AL   = 0x55;  /* Execute transfer count 3AL **  8 bit not used */
		_h8_DMAC_ETCR3A    = 0x54;  /* Execute transfer count 3A  ** 16 bit not used */
		_h8_DMAC_IOAR3A    = 0x56;  /* I/O address            3A  **  8 bit not used */
		_h8_DMAC_DTCR3A    = 0x57;  /* Data transfer control  3A  **  8 bit not used */
	
		_h8_DMAC_MAR3BR    = 0x58;  /* Memory address         3BR **  8 bit not used */
		_h8_DMAC_MAR3BE    = 0x59;  /* Memory address         3BE **  8 bit not used */
		_h8_DMAC_MAR3BH    = 0x5A;  /* Memory address         3BH **  8 bit not used */
		_h8_DMAC_MAR3BL    = 0x5B;  /* Memory address         3BL **  8 bit not used */
		_h8_DMAC_MAR3B     = 0x58;  /* Memory address         3B  ** 32 bit not used */
		_h8_DMAC_ETCR3BH   = 0x5C;  /* Execute transfer count 3BH **  8 bit not used */
		_h8_DMAC_ETCR3BL   = 0x5D;  /* Execute transfer count 3BL **  8 bit not used */
		_h8_DMAC_ETCR3B    = 0x5C;  /* Execute transfer count 3B  ** 16 bit not used */
		_h8_DMAC_IOAR3B    = 0x5E;  /* I/O address            3B  **  8 bit not used */
		_h8_DMAC_DTCR3B    = 0x5F;  /* Data transfer control  3B  **  8 bit not used */
	
	
		/* I/O ports */ 
						
		_h8_P4DDR          = 0xC5;  /* Port 4 data direction      **  8 bit     used */
		_h8_P4DR           = 0xC7;  /* Port 4 data                **  8 bit     used */
		_h8_P4PCR          = 0xDA;  /* Port 4 input pull-up contr **  8 bit     used */
	
		_h8_P5DDR          = 0xC8;  /* Port 5 data direction      **  8 bit not used */
		_h8_P5DR           = 0xCA;  /* Port 5 data                **  8 bit not used */
		_h8_P5PCR          = 0xDB;  /* Port 5 input pull-up contr **  8 bit not used */
	
		_h8_P6DDR          = 0xC9;  /* Port 6 data direction      **  8 bit     used */ 
		_h8_P6DR           = 0xCB;  /* Port 6 data                **  8 bit     used */ 
	
		_h8_P7DR           = 0xCE;  /* Port 7 data                **  8 bit     used */ 
	
		_h8_P8DDR          = 0xCD;  /* Port 8 data direction      **  8 bit     used */  
		_h8_P8DR           = 0xCF;  /* Port 8 data                **  8 bit     used */  
						 
		_h8_P9DDR          = 0xD0;  /* Port 9 data direction      **  8 bit     used */  
		_h8_P9DR           = 0xD2;  /* Port 9 data                **  8 bit     used */  
	
		_h8_PADDR          = 0xD1;  /* Port A data direction      **  8 bit     used */  
		_h8_PADR           = 0xD3;  /* Port A data                **  8 bit     used */  
	
		_h8_PBDDR          = 0xD4;  /* Port B data direction      **  8 bit     used */  
		_h8_PBDR           = 0xD6;  /* Port B data                **  8 bit     used */  
	
		_h8_PCDDR          = 0xD5;  /* Port C data direction      **  8 bit     used */  
		_h8_PCDR           = 0xD7;  /* Port C data                **  8 bit     used */  
	
		
		/* 16-bit integrated timer unit */
						
		_h8_ITU_TSTR       = 0x60;  /* Timer start                **  8 bit     used */
		_h8_ITU_TSNC       = 0x61;  /* Timer syncro               **  8 bit     used */
		_h8_ITU_TMDR       = 0x62;  /* Timer mode                 **  8 bit     used */
		_h8_ITU_TFCR       = 0x63;  /* Timer function control     **  8 bit not used */
		_h8_ITU_TOER       = 0x90;  /* Timer output master enable **  8 bit not used */
		_h8_ITU_TOCR       = 0x91;  /* Timer output control       **  8 bit not used */
	
		_h8_ITU_TCR0       = 0x64;  /* Timer control         ch 0 **  8 bit     used */
		_h8_ITU_TIOR0      = 0x65;  /* Timer I/O ctrl        ch 0 **  8 bit     used */
		_h8_ITU_TIER0      = 0x66;  /* Timer intrpt enab     ch 0 **  8 bit     used */
		_h8_ITU_TSR0       = 0x67;  /* Timer status          ch 0 **  8 bit     used */
		_h8_ITU_TCNT0H     = 0x68;  /* Timer counter(high)   ch 0 **  8 bit not used */
		_h8_ITU_TCNT0L     = 0x69;  /* Timer counter(low)    ch 0 **  8 bit not used */
		_h8_ITU_TCNT0      = 0x68;  /* Timer counter         ch 0 ** 16 bit     used */
		_h8_ITU_GRA0H      = 0x6A;  /* Timer general A(high) ch 0 **  8 bit not used */
		_h8_ITU_GRA0L      = 0x6B;  /* Timer general A(low)  ch 0 **  8 bit not used */
		_h8_ITU_GRA0       = 0x6A;  /* Timer general A       ch 0 ** 16 bit not used */
		_h8_ITU_GRB0H      = 0x6C;  /* Timer general B(high) ch 0 **  8 bit not used */
		_h8_ITU_GRB0L      = 0x6D;  /* Timer general B(low)  ch 0 **  8 bit not used */
		_h8_ITU_GRB0       = 0x6C;  /* Timer general B       ch 0 ** 16 bit not used */
					   
		_h8_ITU_TCR1       = 0x6E;  /* Timer control         ch 1 **  8 bit     used */
		_h8_ITU_TIOR1      = 0x6F;  /* Timer I/O ctrl        ch 1 **  8 bit not used */
		_h8_ITU_TIER1      = 0x70;  /* Timer intrpt enab     ch 1 **  8 bit     used */
		_h8_ITU_TSR1       = 0x71;  /* Timer status          ch 1 **  8 bit     used */
		_h8_ITU_TCNT1H     = 0x72;  /* Timer counter(high)   ch 1 **  8 bit not used */
		_h8_ITU_TCNT1L     = 0x73;  /* Timer counter(low)    ch 1 **  8 bit not used */
		_h8_ITU_TCNT1      = 0x72;  /* Timer counter         ch 1 ** 16 bit     used */
		_h8_ITU_GRA1H      = 0x74;  /* Timer general A(high) ch 1 **  8 bit not used */
		_h8_ITU_GRA1L      = 0x75;  /* Timer general A(low)  ch 1 **  8 bit not used */
		_h8_ITU_GRA1       = 0x74;  /* Timer general A       ch 1 ** 16 bit     used */
		_h8_ITU_GRB1H      = 0x76;  /* Timer general B(high) ch 1 **  8 bit not used */
		_h8_ITU_GRB1L      = 0x77;  /* Timer general B(low)  ch 1 **  8 bit not used */   
		_h8_ITU_GRB1       = 0x76;  /* Timer general B       ch 1 **  8 bit     used */
						
		_h8_ITU_TCR2       = 0x78;  /* Timer control         ch 2 **  8 bit     used */
		_h8_ITU_TIOR2      = 0x79;  /* Timer I/O ctrl        ch 2 **  8 bit     used */
		_h8_ITU_TIER2      = 0x7A;  /* Timer intrpt enab     ch 2 **  8 bit     used */
		_h8_ITU_TSR2       = 0x7B;  /* Timer status          ch 2 **  8 bit     used */
		_h8_ITU_TCNT2H     = 0x7C;  /* Timer counter(high)   ch 2 **  8 bit not used */
		_h8_ITU_TCNT2L     = 0x7D;  /* Timer counter(low)    ch 2 **  8 bit not used */
		_h8_ITU_TCNT2      = 0x7C;  /* Timer counter         ch 2 ** 16 bit     used */
		_h8_ITU_GRA2H      = 0x7E;  /* Timer general A(high) ch 2 **  8 bit not used */
		_h8_ITU_GRA2L      = 0x7F;  /* Timer general A(low)  ch 2 **  8 bit not used */
		_h8_ITU_GRA2       = 0x7E;  /* Timer general A       ch 2 ** 16 bit     used */
		_h8_ITU_GRB2H      = 0x80;  /* Timer general B(high) ch 2 **  8 bit not used */
		_h8_ITU_GRB2L      = 0x81;  /* Timer general B(low)  ch 2 **  8 bit not used */
		_h8_ITU_GRB2       = 0x80;  /* Timer general B       ch 2 ** 16 bit     used */
				
		_h8_ITU_TCR3       = 0x82;  /* Timer control         ch 3 **  8 bit     used */
		_h8_ITU_TIOR3      = 0x83;  /* Timer I/O ctrl        ch 3 **  8 bit not used */
		_h8_ITU_TIER3      = 0x84;  /* Timer intrpt enab     ch 3 **  8 bit not used */
		_h8_ITU_TSR3       = 0x85;  /* Timer status          ch 3 **  8 bit not used */
		_h8_ITU_TCNT3H     = 0x86;  /* Timer counter(high)   ch 3 **  8 bit not used */
		_h8_ITU_TCNT3L     = 0x87;  /* Timer counter(low)    ch 3 **  8 bit not used */
		_h8_ITU_TCNT3      = 0x86;  /* Timer counter         ch 3 ** 16 bit not used */
		_h8_ITU_GRA3H      = 0x88;  /* Timer general A(high) ch 3 **  8 bit not used */
		_h8_ITU_GRA3L      = 0x89;  /* Timer general A(low)  ch 3 **  8 bit not used */
		_h8_ITU_GRA3       = 0x88;  /* Timer general A       ch 3 ** 16 bit     used */
		_h8_ITU_GRB3H      = 0x8A;  /* Timer general B(high) ch 3 **  8 bit not used */
		_h8_ITU_GRB3L      = 0x8B;  /* Timer general B(low)  ch 3 **  8 bit not used */
		_h8_ITU_GRB3       = 0x8A;  /* Timer general B       ch 3 ** 16 bit     used */
		_h8_ITU_BRA3H      = 0x8C;  /* Timer buffer A(high)  ch 3 **  8 bit not used */
		_h8_ITU_BRA3L      = 0x8D;  /* Timer buffer A(low)   ch 3 **  8 bit not used */
		_h8_ITU_BRA3       = 0x8C;  /* Timer buffer A        ch 3 ** 16 bit not used */
		_h8_ITU_BRB3H      = 0x8E;  /* Timer buffer B(high)  ch 3 **  8 bit not used */
		_h8_ITU_BRB3L      = 0x8F;  /* Timer buffer B(low)   ch 3 **  8 bit not used */
		_h8_ITU_BRB3       = 0x8E;  /* Timer buffer B        ch 3 ** 16 bit not used */
		
		_h8_ITU_TCR4       = 0x92;  /* Timer control         ch 4 **  8 bit     used */
		_h8_ITU_TIOR4      = 0x93;  /* Timer I/O ctrl        ch 4 **  8 bit not used */
		_h8_ITU_TIER4      = 0x94;  /* Timer intrpt enab     ch 4 **  8 bit not used */
		_h8_ITU_TSR4       = 0x95;  /* Timer status          ch 4 **  8 bit not used */
		_h8_ITU_TCNT4H     = 0x96;  /* Timer counter(high)   ch 4 **  8 bit not used */
		_h8_ITU_TCNT4L     = 0x97;  /* Timer counter(low)    ch 4 **  8 bit not used */
		_h8_ITU_TCNT4      = 0x96;  /* Timer counter         ch 4 ** 16 bit not used */
		_h8_ITU_GRA4H      = 0x98;  /* Timer general A(high) ch 4 **  8 bit not used */
		_h8_ITU_GRA4L      = 0x99;  /* Timer general A(low)  ch 4 **  8 bit not used */
		_h8_ITU_GRA4       = 0x98;  /* Timer general A       ch 4 ** 16 bit not used */
		_h8_ITU_GRB4H      = 0x9A;  /* Timer general B(high) ch 4 **  8 bit not used */
		_h8_ITU_GRB4L      = 0x9B;  /* Timer general B(low)  ch 4 **  8 bit not used */
		_h8_ITU_GRB4       = 0x9A;  /* Timer general B       ch 4 ** 16 bit not used */
		_h8_ITU_BRA4H      = 0x9C;  /* Timer buffer A(high)  ch 4 **  8 bit not used */
		_h8_ITU_BRA4L      = 0x9D;  /* Timer buffer A(low)   ch 4 **  8 bit not used */
		_h8_ITU_BRA4       = 0x9C;  /* Timer buffer A        ch 4 ** 16 bit not used */
		_h8_ITU_BRB4H      = 0x9E;  /* Timer buffer B(high)  ch 4 **  8 bit not used */
		_h8_ITU_BRB4L      = 0x9F;  /* Timer buffer B(low)   ch 4 **  8 bit not used */
		_h8_ITU_BRB4       = 0x9E;  /* Timer buffer B        ch 4 ** 16 bit not used */
						
		
		/* Programmable timing pattern controller */
		
		_h8_TPC_TPMR       = 0xA0;  /* TPC output mode            **  8 bit not used */
		_h8_TPC_TPCR       = 0xA1;  /* TPC output control         **  8 bit not used */
		_h8_TPC_NDERB      = 0xA2;  /* Next data enable B         **  8 bit not used */
		_h8_TPC_NDERA      = 0xA3;  /* Next data enable A         **  8 bit not used */
		_h8_TPC_NDRB1      = 0xA4;  /* Next data B1               **  8 bit not used */
		_h8_TPC_NDRA1      = 0xA5;  /* Next data A1               **  8 bit not used */
		_h8_TPC_NDRB2      = 0xA6;  /* Next data B2               **  8 bit not used */
		_h8_TPC_NDRA2      = 0xA7;  /* Next data A2               **  8 bit not used */
		
	
		/* Watchdog timer */
						
		_h8_WDT_TCSR       = 0xA8;  /* Timer control/status       **  8 bit not used */
		_h8_WDT_TCSR_WRITE = 0xA8;  /* Timer control/status write ** 16 bit not used */
		_h8_WDT_TCNT       = 0xA9;  /* Timer counter              **  8 bit not used */
		_h8_WDT_TCNT_WRITE = 0xA8;  /* Timer counter write        ** 16 bit not used */
		_h8_WDT_RSTCSR     = 0xAB;  /* Reset control/status       **  8 bit not used */
		_h8_WDT_RSTCSR_WR	= 0xAA;  /* Reset control/status write ** 16 bit not used */
						
						
		/* Serial communication */
						
		_h8_SCI_SMR0       = 0xB0;  /* Serial mode           ch 0 **  8 bit     used */
		_h8_SCI_BRR0       = 0xB1;  /* Bit rate              ch 0 **  8 bit     used */
		_h8_SCI_SCR0       = 0xB2;  /* Serial control        ch 0 **  8 bit     used */
		_h8_SCI_TDR0       = 0xB3;  /* Transmit data         ch 0 **  8 bit     used */
		_h8_SCI_SSR0       = 0xB4;  /* Serial status         ch 0 **  8 bit     used */
		_h8_SCI_RDR0       = 0xB5;  /* Receive data          ch 0 **  8 bit     used */
	
		_h8_SCI_SMR1       = 0xB8;  /* Serial mode           ch 1 **  8 bit     used */
		_h8_SCI_BRR1       = 0xB9;  /* Bit rate              ch 1 **  8 bit     used */
		_h8_SCI_SCR1       = 0xBA;  /* Serial control        ch 1 **  8 bit     used */
		_h8_SCI_TDR1       = 0xBB;  /* Transmit data         ch 1 **  8 bit     used */
		_h8_SCI_SSR1       = 0xBC;  /* Serial status         ch 1 **  8 bit     used */
		_h8_SCI_RDR1       = 0xBD;  /* Receive data          ch 1 **  8 bit     used */
		
		/* A/D converter */
	 
		_h8_ADDRAH         = 0xE0;  /* A/D Data register A high   **  8 bit     used */
		_h8_ADDRAL         = 0xE1;  /* A/D Data register A low    **  8 bit     used */
		_h8_ADDRBH         = 0xE2;  /* A/D Data register B high   **  8 bit     used */
		_h8_ADDRBL         = 0xE3;  /* A/D Data register B low    **  8 bit     used */
		_h8_ADDRCH         = 0xE4;  /* A/D Data register C high   **  8 bit not used */
		_h8_ADDRCL         = 0xE5;  /* A/D Data register C low    **  8 bit not used */
		_h8_ADDRDH         = 0xE6;  /* A/D Data register D high   **  8 bit not used */
		_h8_ADDRDL         = 0xE7;  /* A/D Data register D low    **  8 bit not used */
		_h8_ADCSR          = 0xE8;  /* A/D Control/status         **  8 bit     used */
		_h8_ADCR           = 0xE9;  /* A/D Control                **  8 bit not used */        
	} > eight

	.stab 0 (NOLOAD):
	{
		[ .stab ]
	}

	.stabstr 0 (NOLOAD):
	{
		[ .stabstr ]
	}
}
