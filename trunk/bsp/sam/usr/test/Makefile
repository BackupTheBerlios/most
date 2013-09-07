-include $(MOST_ROOT)/etc/Config

ifneq ($(MOST_APP),test)
$(warning Wrong app defined!)
endif

LIB_NAMES_USR := -l_dpy\
                 -l_tst\
                 -l_sen\
                 -l_act

ARCH_PATH := ../../sys/src/arch/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)
INIT_PATH := ../../sys/src/init/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

OBJ_NAMES_APP := $(ARCH_PATH)/adc.o\
	             $(ARCH_PATH)/eth.o\
	             $(ARCH_PATH)/mmc.o\
	             $(ARCH_PATH)/pwm.o\
	             $(ARCH_PATH)/rtc.o\
	             $(ARCH_PATH)/spi.o\
	             $(INIT_PATH)/bsp_commands.o\
	             $(INIT_PATH)/config.o\
	             $(INIT_PATH)/download.o\
	             $(INIT_PATH)/events.o\
	             $(INIT_PATH)/net.o

## check max file size : c bytes, k kilobytes, M megabytes
MAX_BIN_SIZE:=176k

-include $(MOST_ROOT)/bsp/sam/usr/Makefile
-include $(MOST_ROOT)/etc/build/prog/Makefile
