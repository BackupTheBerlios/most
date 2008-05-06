-include $(MOST_ROOT)/etc/Config
#-include Config

PROG_NAME := test

ifneq ($(MOST_CPU),arm)
$(error Wrong target defined!)
endif

LIB_NAMES_SYS := 		 -l_cli\
		 -l_mfs\
		 -l_dev\
		 -l_uso\
		 -l_ace\
		 -l_uso\
		 -l_dev

LIB_NAMES_USR :=

LDFLAGS_OPT := #-Ttext $(SAM_TEXT) -Tdata $(SAM_DATA)

LIB_PATH_OPT := 
LIB_NAMES_OPT :=

OBJ_NAMES := 	../../sys/src/arch/o/$(MOST_TARGET)/*.o\
				../../sys/src/init/o/$(MOST_TARGET)/*.o\
				src/main/o/$(MOST_TARGET)/*.o	
				
makegoal = make $(1) -C ../../sys/src;\
		   make $(1) -C src;
           

STRIP_SECTIONS:= --strip-debug --strip-unneeded -s -R .stack -R .heap 

-include $(MOST_ROOT)/etc/Makefile.prog
