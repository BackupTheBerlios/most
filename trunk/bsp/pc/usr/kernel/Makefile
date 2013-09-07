-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_APP),kernel)
$(warning Wrong app defined!)
endif

LIB_NAMES_USR :=

ARCH_PATH := ../../sys/src/arch/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)
INIT_PATH := ../../sys/src/init/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES_APP := $(ARCH_PATH)/kbd.o

## check max file size : c bytes, k kilobytes, M megabytes
MAX_BIN_SIZE:=1M

-include $(MOST_ROOT)/bsp/pc/usr/Makefile
-include $(MOST_ROOT)/etc/build/prog/Makefile
