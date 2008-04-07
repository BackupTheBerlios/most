-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_CPU),h8300)
$(error Wrong target defined!)
endif

LIB_NAMES_SYS := -l_nap\
		 -l_cli\
		 -l_mfs\
		 -l_net\
		 -l_dev\
		 -l_uso\
		 -l_ace

OBJ_NAMES := src/main/o/$(MOST_TARGET)/*.o\
	../../sys/src/arch/o/$(MOST_TARGET)/*.o\
	../../sys/src/init/o/$(MOST_TARGET)/*.o

LIB_NAMES_USR :=

LDFLAGS_OPT := --relax 

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

makegoal = make $(1) -C src;\
           make $(1) -C ../../sys/src;

STRIP_SECTIONS:= -s -R .noinit -R .stack -R .heap -R .eight 

-include $(MOST_ROOT)/etc/Makefile.prog
