-include $(MOST_ROOT)/etc/Config

LIB_NAME := nap
LIB_TYPE := sys
OBJ_NAMES := bootp/o/$(MOST_TARGET)/*.o\
             tftp/o/$(MOST_TARGET)/*.o\
             syslog/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C bootp;\
           make $(1) -C tftp;\
           make $(1) -C syslog;

-include $(MOST_ROOT)/etc/build/lib/Makefile
