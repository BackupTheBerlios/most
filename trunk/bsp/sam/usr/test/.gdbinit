## gdbinit

file ./boot.elf
target remote localhost:3333

#set remote hardware-breakpoint-limit 2
#set remote hardware-watchpoint-limit 2

break SAM_init
continue
