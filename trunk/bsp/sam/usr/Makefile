-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_BOARD),sam)
$(warning Wrong board defined!)
endif

LIB_NAMES_SYS := -l_cli\
		 -l_nap\
		 -l_mfs\
		 -l_net\
		 -l_dev\
		 -l_uso\
		 -l_ace

LIB_NAMES_USR := -l_dpy\
                 -l_tst

LDFLAGS_OPT :=

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

OBJ_NAMES := 	../../sys/src/arch/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		../../sys/src/init/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		src/main/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o	

makegoal_src = make $(1) -C ../../sys/src;\
		make $(1) -C src;

#WORD_WITH:=16
WORD_WITH:=32

STRIP_FLAGS:= --target=$(MOST_SYSTEM)$(WORD_WITH)-$(MOST_ENDIAN)$(MOST_CPU)
STRIP_SECTIONS:= --strip-debug -s -R .bss -R .stack -R .heap -R .comment -R .ARM.attributes  

CODE_SECTIONS:= -j .text -j .rodata
DATA_SECTIONS:= -j .data


