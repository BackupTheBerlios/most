-include $(MOST_ROOT)/etc/Config

LIB_NAME := uso
LIB_TYPE := sys
OBJ_NAMES := arch/obj/$(MOST_TARGET)/*.o\
			 log/obj/$(MOST_TARGET)/*.o\
			 ipc/obj/$(MOST_TARGET)/*.o\
			 kernel/obj/$(MOST_TARGET)/*.o\
			 list/obj/$(MOST_TARGET)/*.o\
			 mm/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch;\
	       make $(1) -C log;\
	       make $(1) -C ipc;\
	       make $(1) -C kernel;\
	       make $(1) -C list;\
	       make $(1) -C mm;

-include $(MOST_ROOT)/etc/Makefile.lib

