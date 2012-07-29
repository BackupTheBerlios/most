-include $(MOST_ROOT)/etc/Config

LIB_NAME := dpy
LIB_TYPE := usr
OBJ_NAMES := gu256x64ser/o/$(MOST_TARGET)/*.o\
			 nokia6100/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C gu256x64ser;\
           make $(1) -C nokia6100;

-include $(MOST_ROOT)/etc/build/lib/Makefile
