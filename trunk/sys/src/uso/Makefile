-include $(MOST_ROOT)/etc/Config

LIB_NAME := uso
LIB_TYPE := sys
OBJ_NAMES := arch/o/$(MOST_TARGET)/*.o\
			 log/o/$(MOST_TARGET)/*.o\
			 ipc/o/$(MOST_TARGET)/*.o\
			 kernel/o/$(MOST_TARGET)/*.o\
			 list/o/$(MOST_TARGET)/*.o\
			 mm/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch;\
	       make $(1) -C log;\
	       make $(1) -C ipc;\
	       make $(1) -C kernel;\
	       make $(1) -C list;\
	       make $(1) -C mm;

-include $(MOST_ROOT)/etc/build/lib/Makefile

