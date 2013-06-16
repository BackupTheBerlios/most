## gdbinit , copy .gdbinit to debug/.gdbinit and start emacs in debug dir

file boot.elf
target remote localhost:3333

#set remote hardware-breakpoint-limit 2
#set remote hardware-watchpoint-limit 2

break LA2_init
continue
