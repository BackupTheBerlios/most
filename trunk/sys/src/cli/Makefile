-include $(MOST_ROOT)/etc/Config

LIB_NAME := cli
LIB_TYPE := sys
OBJ_NAMES := commands/o/$(MOST_TARGET)/*.o\
			 interpreter/o/$(MOST_TARGET)/*.o\
			 history/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C commands;\
	       make $(1) -C interpreter;\
	       make $(1) -C history;

-include $(MOST_ROOT)/etc/Makefile.lib
