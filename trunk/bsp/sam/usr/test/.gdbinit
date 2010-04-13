## gdbinit

file ./test.unstripped
target remote localhost:3333

monitor reset init
monitor sleep 500
set $pc=0x0

monitor flash write_image erase usr/test/test.bin 0x100000 bin
#load test.elf

break SAM_init
#continue