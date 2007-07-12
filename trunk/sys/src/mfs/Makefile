-include $(MOST_ROOT)/etc/Config

LIB_NAME := mfs
LIB_TYPE := sys
OBJ_NAMES := vfs/obj/$(MOST_TARGET)/*.o\
			 sysfs/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C vfs;\
		   make $(1) -C sysfs;
		   	
-include $(MOST_ROOT)/etc/Makefile.lib

