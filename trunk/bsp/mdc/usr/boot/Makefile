-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_APP),boot)
$(warning Wrong app defined!)
endif

LIB_NAMES_USR := -l_dpy

ARCH_PATH := ../../sys/src/arch/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)
INIT_PATH := ../../sys/src/init/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES_APP := $(ARCH_PATH)/interrupts.o\
	             $(ARCH_PATH)/spi.o\
	             $(ARCH_PATH)/ee.o\
	             $(ARCH_PATH)/lcd.o\
	             $(ARCH_PATH)/eth.o\
	             $(INIT_PATH)/bsp_commands.o\
	             $(INIT_PATH)/config.o\
	             $(INIT_PATH)/download.o\
	             $(INIT_PATH)/net.o

## check max file size : c bytes, k kilobytes, M megabytes
MAX_BIN_SIZE:=128k

-include $(MOST_ROOT)/bsp/mdc/usr/Makefile
-include $(MOST_ROOT)/etc/build/prog/Makefile
