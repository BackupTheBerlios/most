## gdbinit

file ./test.unstripped
target remote localhost:3333

monitor reset init

#monitor flash write_image erase test.bin 0x100000 bin
#monitor flash write_bank 0 test.bin 0x100000
#load test.elf

break SAM_main