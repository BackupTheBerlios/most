## gdbinit , copy .gdbinit to debug/.gdbinit and start emacs in debug dir

file test.elf
target remote localhost:3333

#set remote hardware-breakpoint-limit 2
#set remote hardware-watchpoint-limit 2

break SAM_init
continue
