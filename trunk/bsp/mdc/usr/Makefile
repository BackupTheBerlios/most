ifneq ($(MOST_BOARD),mdc)
$(warning Wrong board defined!)
endif

LIB_NAMES_SYS := -l_cli\
	             -l_mfs\
	             -l_nap\
	             -l_net\
	             -l_dev\
	             -l_uso\
	             -l_ace

MAIN_PATH := src/main/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES := $(ARCH_PATH)/reset.o\
	         $(ARCH_PATH)/vectors_ram.o\
	         $(ARCH_PATH)/vectors.o\
	         $(ARCH_PATH)/cpu.o\
	         $(ARCH_PATH)/digio.o\
	         $(ARCH_PATH)/pins.o\
	         $(ARCH_PATH)/sci.o\
	         $(ARCH_PATH)/ticks.o\
	         $(INIT_PATH)/init.o\
	         $(INIT_PATH)/start.o\
	         $(INIT_PATH)/events.o\
	         $(MAIN_PATH)/*.o

OBJ_NAMES += $(OBJ_NAMES_APP)

LDFLAGS_OPT := --relax 

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

makegoal_src = make $(1) -C ../../sys/src;\
	       make $(1) -C src;

STRIP_FLAGS:= --target=$(MOST_SYSTEM)-$(MOST_CPU)
STRIP_SECTIONS:= --strip-debug -s -R .bss -R .stack -R .heap -R .comment -R .eight 

CODE_SECTIONS:=-j .text -j .rodata
DATA_SECTIONS:=-j .data
