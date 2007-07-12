-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_CPU),h8300)
$(error Wrong target defined!)
endif

LDFLAGS_OPT :=

LIB_PATH_OPT := -L$(MOST_ROOT)/usr/lib/$(MOST_TARGET) 
LIB_NAMES_OPT :=

PROG_NAME := frame

OBJ_NAMES := src/main/obj/$(MOST_TARGET)/*.o\
	../../sys/src/arch/obj/$(MOST_TARGET)/*.o\
	../../sys/src/init/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C src;\
           make $(1) -C ../../sys/src;

-include $(MOST_ROOT)/etc/Makefile.prog
