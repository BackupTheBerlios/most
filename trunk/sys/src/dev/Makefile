-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := arch/o/$(MOST_TARGET)/*.o\
			 arch/$(MOST_ARCH)/o/$(MOST_TARGET)/*.o\
			 digio/o/$(MOST_TARGET)/*.o\
			 serial/o/$(MOST_TARGET)/*.o\
			 timer/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch;\
		   make $(1) -C arch/$(MOST_ARCH);\
		   make $(1) -C digio;\
	       make $(1) -C serial;\
	       make $(1) -C timer;

-include $(MOST_ROOT)/etc/Makefile.lib
