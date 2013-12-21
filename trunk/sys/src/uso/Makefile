-include $(MOST_ROOT)/etc/Config

LIB_NAME := uso
LIB_TYPE := sys
OBJ_NAMES := arch/$(MOST_CPU)/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 log/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 ipc/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 kernel/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 list/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 mm/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C log;\
	       make $(1) -C ipc;\
	       make $(1) -C kernel;\
	       make $(1) -C list;\
	       make $(1) -C mm;

makegoal_arch = make $(1) -C arch/$(MOST_CPU);

makegoal_arch_mrproper = make $(1) -C arch/i386;\
						make $(1) -C arch/h8300;\
						make $(1) -C arch/arm;

-include $(MOST_ROOT)/etc/build/lib/Makefile

