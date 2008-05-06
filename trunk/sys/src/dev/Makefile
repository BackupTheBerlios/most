-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := arch/o/$(MOST_TARGET)/*.o\
			 digio/o/$(MOST_TARGET)/*.o\
			 serial/o/$(MOST_TARGET)/*.o\
			 timer/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch;\
		   make $(1) -C digio;\
	       make $(1) -C serial;\
	       make $(1) -C timer;

ifeq ($(MOST_ARCH),h8)
OBJ_NAMES += arch/$(MOST_CPU)/o/$(MOST_TARGET)/*.o
makegoal += make $(1) -C arch/$(MOST_CPU);
else
ifeq ($(MOST_ARCH),at91)
OBJ_NAMES += arch/$(MOST_ARCH)/o/$(MOST_TARGET)/*.o
makegoal += make $(1) -C arch/$(MOST_ARCH);
else
$(error No arch defined!)
endif 
endif

-include $(MOST_ROOT)/etc/Makefile.lib
