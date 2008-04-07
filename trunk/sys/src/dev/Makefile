-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := arch/$(MOST_CPU)/o/$(MOST_TARGET)/*.o\
			 arch/o/$(MOST_TARGET)/*.o\
			 digio/o/$(MOST_TARGET)/*.o\
			 serial/o/$(MOST_TARGET)/*.o\
			 timer/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch/$(MOST_CPU);\
		   make $(1) -C arch;\
		   make $(1) -C digio;\
	       make $(1) -C serial;\
	       make $(1) -C timer;

-include $(MOST_ROOT)/etc/Makefile.lib
