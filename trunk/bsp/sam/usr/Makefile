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

LDFLAGS_OPT := #-Ttext $(SAM_TEXT) -Tdata $(SAM_DATA)

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

OBJ_NAMES := 	../../sys/src/arch/o/$(MOST_TARGET)/*.o\
		../../sys/src/init/o/$(MOST_TARGET)/*.o\
		src/main/o/$(MOST_TARGET)/*.o	

makegoal_src = make $(1) -C ../../sys/src;\
		make $(1) -C src;

makegoal_inc = make $(1) -C ../../sys/inc/doc;


STRIP_FLAGS:= --target=$(MOST_SYSTEM)$(MOST_BIT)-$(MOST_ENDIAN)$(MOST_CPU)
STRIP_SECTIONS:= --strip-debug --strip-unneeded -s -R .stack -R .heap 

