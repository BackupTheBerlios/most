-include $(MOST_ROOT)/etc/Config

LIB_NAME := mfs
LIB_TYPE := sys
OBJ_NAMES := vfs/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 sysfs/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C vfs;\
		   make $(1) -C sysfs;
		   	
-include $(MOST_ROOT)/etc/build/lib/Makefile

