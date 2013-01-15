-include $(MOST_ROOT)/etc/Config

LIB_NAME := cli
LIB_TYPE := sys
OBJ_NAMES := commands/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 exe/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 interpreter/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C commands;\
		   make $(1) -C exe;\
	       make $(1) -C interpreter;

-include $(MOST_ROOT)/etc/build/lib/Makefile
