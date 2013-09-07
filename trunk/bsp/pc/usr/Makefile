ifneq ($(MOST_BOARD),pc)
$(warning Wrong board defined!)
endif

LIB_NAMES_SYS := -l_nap\
                 -l_cli\
                 -l_mfs\
                 -l_net\
                 -l_dev\
                 -l_uso\
                 -l_ace

MAIN_PATH := src/main/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES := $(ARCH_PATH)/reset.o\
             $(ARCH_PATH)/vectors.o\
             $(ARCH_PATH)/cpu.o\
             $(ARCH_PATH)/debug.o\
             $(ARCH_PATH)/exceptions.o\
             $(ARCH_PATH)/mm_paging.o\
             $(ARCH_PATH)/mm_phys.o\
             $(ARCH_PATH)/pic.o\
             $(ARCH_PATH)/ticks.o\
             $(INIT_PATH)/init.o\
             $(INIT_PATH)/start.o\
             $(MAIN_PATH)/*.o

OBJ_NAMES += $(OBJ_NAMES_APP)

LDFLAGS_OPT := -Ttext=0x100000

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

makegoal_src = make $(1) -C ../../sys/src;\
		make $(1) -C src;

#WORD_WITH:=16
WORD_WITH:=32

STRIP_FLAGS:= --target=$(MOST_SYSTEM)$(WORD_WITH)-$(MOST_CPU)
STRIP_SECTIONS:= --strip-debug -s -R .bss -R .stack -R .heap -R .comment  

CODE_SECTIONS:= -j .text -j .rodata
DATA_SECTIONS:= -j .data


