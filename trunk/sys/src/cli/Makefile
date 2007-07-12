-include $(MOST_ROOT)/etc/Config

LIB_NAME := cli
LIB_TYPE := sys
OBJ_NAMES := commands/obj/$(MOST_TARGET)/*.o\
			 interpreter/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C commands;\
	       make $(1) -C interpreter;

-include $(MOST_ROOT)/etc/Makefile.lib
