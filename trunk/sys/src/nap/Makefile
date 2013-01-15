-include $(MOST_ROOT)/etc/Config

LIB_NAME := nap
LIB_TYPE := sys
OBJ_NAMES := bootp/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
	         tftp/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
	         syslog/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
	         ymodem/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C bootp;\
	       make $(1) -C tftp;\
	       make $(1) -C syslog;\
	       make $(1) -C ymodem;

-include $(MOST_ROOT)/etc/build/lib/Makefile
