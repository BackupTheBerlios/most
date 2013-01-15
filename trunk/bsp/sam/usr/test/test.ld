OUTPUT_FORMAT("elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(reset_handler)

sram_size	= 64K;
stack_size	= 1K;

MEMORY
{
	flash(rx)		: o = 0x110000, l = 192K
	sram(rw)		: o = 0x200000, l = 64K
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
		sram_start = .;
		data_start = .;
		KEEP(*(.vectors))
		*(.ramcode)
		*(.data)
        *(.data*)
		data_end = .;
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
