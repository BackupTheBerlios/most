-include $(MOST_ROOT)/etc/Config

LIB_NAME := dpy
LIB_TYPE := usr
OBJ_NAMES := gu256x64ser/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 lcd/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 led/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C gu256x64ser;\
           make $(1) -C lcd;\
           make $(1) -C led;

-include $(MOST_ROOT)/etc/build/lib/Makefile
