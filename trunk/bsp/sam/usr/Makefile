-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_BOARD),sam)
$(error Wrong board defined!)
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

makegoal_inc = make $(1) -C ../../sys/inc/doc;


STRIP_FLAGS:= --target=$(MOST_SYSTEM)$(MOST_BIT)-$(MOST_ENDIAN)$(MOST_CPU)
STRIP_SECTIONS:= --strip-debug --strip-unneeded -s -R .stack -R .heap 

