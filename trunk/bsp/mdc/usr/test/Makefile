-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_APP),test)
$(warning Wrong app defined!)
endif

LIB_NAMES_USR := -l_tst\
	             -l_dpy\
	             -l_act\
	             -l_sen

ARCH_PATH := ../../sys/src/arch/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)
INIT_PATH := ../../sys/src/init/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES_APP := $(ARCH_PATH)/spi.o\
	             $(ARCH_PATH)/lcd.o\
	             $(ARCH_PATH)/eth.o\
	             $(ARCH_PATH)/rand.o\
	             $(INIT_PATH)/bsp_commands.o\
	             $(INIT_PATH)/config.o\
	             $(INIT_PATH)/download.o\
	             $(INIT_PATH)/net.o

## check max file size : c bytes, k kilobytes, M megabytes
MAX_BIN_SIZE:=320k

-include $(MOST_ROOT)/bsp/mdc/usr/Makefile
-include $(MOST_ROOT)/etc/build/prog/Makefile
