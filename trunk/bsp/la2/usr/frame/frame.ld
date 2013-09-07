OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset_handler)

cram_size	= 16K;
sram_size	= 2M;
stack_size	= 1K;

MEMORY
{
	flash(rx)		: o = 0x40020000, l = 896K
	cram(rwx)       : o = 0x00000000, l = 16K
	sram(rwx)		: o = 0x48000000, l = 2M
}

SECTIONS
{
    .text : 
    {
        code_start = .;
        text_start = .;
        KEEP(*(.reset))
        KEEP(*(.unref))
        *(.text)                                
        *(.text*)                                
        text_end = .;
    } > flash

    .rodata :
    {
        rodata_start = . ;
        *(.rodata)
        *(.rodata*)
        . = ALIGN(4);
        rodata_end = . ;
        code_end = . ;
    } > flash

	.data : AT (ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata))
	{
		cram_start = .;
		data_start = .;
		KEEP(*(.vectors))
		*(.data)
        *(.data*)
		data_end = .;
	} > cram

	.stack (NOLOAD):
	{
		. = cram_start + cram_size - stack_size - 0;
		stack_start = . ;
		*(.stack)
		. = cram_start + cram_size - 0;
		stack_end = . ;
		cram_end = . ;
	} > cram

	.ramc : AT (ADDR(.text) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data))
	{
		sram_start = .;
		ramc_start = .;
		*(.ramcode)
		ramc_end = .;
	} > sram

	.bss (NOLOAD) :
	{ 
		. = ALIGN(4);
		bss_start = .;
		*(.bss)
        *(COMMON)
		bss_end = .;
	} > sram
	
	.heap (NOLOAD):
	{
		. = ALIGN(4);
		heap_start = . ;
		*(.heap)
		. = sram_start + sram_size - 0;
		heap_end = .;
		sram_end = . ;
	} > sram
	
}
