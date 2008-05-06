OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset_handler)

/*#include "project.h"*/

sram_size	= 64K;
stack_size	= 4K;

MEMORY
{
	flash(rx)		: o = 0x100000, l = 256K
	sram(rw)		: o = 0x200000, l = 64K
}

SECTIONS
{
	.text :
	{ 
		text_start = .;
		*(.reset)
		*(.text)
		*(.rodata)                 /* read-only data (constants) */
		*(.rodata*)
		. = ALIGN(4);
 		text_end = . ;
	} > flash

	/* collect all initialized .data sections that go into FLASH  */
	.data : AT ( ADDR (.text) + SIZEOF (.text) )
	{ 
		sram_start = . ;
		data_start = .;
		*(.vectors)
		*(.data)
		data_end = .;
	} > sram

	/* collect all uninitialized .bss sections that go into FLASH  */
	.bss (NOLOAD) :
	{ 
		. = ALIGN(4);
		bss_start = .;
		*(.bss)
		bss_end = .;
	} > sram
	
	.heap (NOLOAD):
	{
		. = ALIGN(4);
		heap_start = . ;
		*(.heap)
		. = sram_start + sram_size - stack_size - 0;
		heap_end = .;
	} > sram

	.stack (NOLOAD):
	{
		stack_start = . ;
		*(.stack)
		. = sram_start + sram_size - 0;
		stack_end = . ;
		sram_end = . ;
	} > sram
	
}
