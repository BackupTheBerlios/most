-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := chips/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 arch/$(MOST_ARCH)/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 digio/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 serial/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 control/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 timer/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 mm/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C chips;\
		   make $(1) -C arch/$(MOST_ARCH);\
		   make $(1) -C digio;\
	       make $(1) -C serial;\
	       make $(1) -C control;\
	       make $(1) -C timer;\
	       make $(1) -C mm;

-include $(MOST_ROOT)/etc/build/lib/Makefile
