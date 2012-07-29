OUTPUT_FORMAT("coff-h8300")
OUTPUT_ARCH(h8300h)
ENTRY("reset")

_ram_size	= 128K;
_stack_size	= 1K;

MEMORY
{
	rom(rx)		: o = 0x020000, l = 384K
	ram(rw)		: o = 0xe00000, l = 128K
	tiny(rw)	: o = 0xff8000, l = 0x0007f00
	eight(rw)	: o = 0xffff00, l = 0x0000100
}
INCLUDE ../H83003_REG.ld

SECTIONS                                
{
	.text :
	{
		_code_start = . ;
		_text_start = . ;
 		*(.vectors)                     
		*(.text)                                
		_text_end = . ;
	} > rom

	.rodata :
	{
		_rodata_start	= . ;
		*(.rodata)
		_rodata_end		= . ;
		_code_end = . ;
	} > rom

	.data :
	{
		_ram_start	= . ;
		_data_start	= . ;
		_vectors_start = . ;
 		*(.ramvect)                     
		*(.ramcode)
		*(.data)
		_data_end = . ;
	} > ram

	.bss :
	{
		_bss_start = . ;
		*(.bss)
		*(COMMON)
		_bss_end = . ;
	} > ram

	.heap (NOLOAD):
	{
		_heap_start = . ;
		*(.heap)
		. = _ram_start  + _ram_size - _stack_size - 0;
		_heap_end = .;
	} > ram

	.stack (NOLOAD):
	{
		_stack_start = . ;
		*(.stack)
		. = _ram_start + _ram_size - 0;
		_stack_end = . ;
		_ram_end = . ;
	} > ram

	.tiny :
	{
		*(.tiny)
	} > tiny

	.stab 0 (NOLOAD):
	{
		[ .stab ]
	}

	.stabstr 0 (NOLOAD):
	{
		[ .stabstr ]
	}
}
