-include $(MOST_ROOT)/etc/Config

LIB_NAME := dev
LIB_TYPE := sys
OBJ_NAMES := chips/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			arch/$(MOST_ARCH)/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			serial/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			control/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			timer/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			mm/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C chips;\
			make $(1) -C serial;\
			make $(1) -C control;\
			make $(1) -C timer;\
			make $(1) -C mm;

makegoal_arch = make $(1) -C arch/$(MOST_ARCH);

makegoal_arch_mrproper = make $(1) -C arch/h83003;\
						make $(1) -C arch/at91sam7x;\
						make $(1) -C arch/at91sam7a2;\
						make $(1) -C arch/ibmpc;

-include $(MOST_ROOT)/etc/build/lib/Makefile
