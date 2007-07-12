-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := arch/$(MOST_CPU)/obj/$(MOST_TARGET)/*.o\
			 arch/obj/$(MOST_TARGET)/*.o\
			 digio/obj/$(MOST_TARGET)/*.o\
			 serial/obj/$(MOST_TARGET)/*.o\
			 timer/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch/$(MOST_CPU);\
		   make $(1) -C arch;\
		   make $(1) -C digio;\
	       make $(1) -C serial;\
	       make $(1) -C timer;

-include $(MOST_ROOT)/etc/Makefile.lib
