-include $(MOST_ROOT)/etc/Config

LIB_NAME := nap
LIB_TYPE := sys
OBJ_NAMES := bootp/obj/$(MOST_TARGET)/*.o\
			tftp/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C bootp;\
		   make $(1) -C tftp;	

-include $(MOST_ROOT)/etc/Makefile.lib
