-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

ifeq ($(MOST_CPU),h8300)
TARGET_CCFLAGS := -mh
TARGET_DEFINES := -DMOST_CPU=1
TARGET_LDFLAGS := -m h8300h
else
ifeq ($(MOST_CPU),i386)
TARGET_CCFLAGS :=
TARGET_DEFINES := -DMOST_CPU=2
TARGET_LDFLAGS := 
else
ifeq ($(MOST_CPU),arm)
TARGET_CCFLAGS := -mcpu=arm7tdmi 
TARGET_DEFINES := -DMOST_CPU=3 
TARGET_LDFLAGS :=
else
ifeq ($(MOST_CPU),m68k)
TARGET_CCFLAGS :=
TARGET_DEFINES := -DMOST_CPU=4
TARGET_LDFLAGS := 
else
$(error No cpu defined!)
endif 
endif
endif
endif



ifeq ($(MOST_ENDIAN),little)
TARGET_DEFINES += -DMOST_ENDIAN=3210
else
ifeq ($(MOST_ENDIAN),big)
TARGET_DEFINES += -DMOST_ENDIAN=0123
else
$(error No endian defined!)
endif
endif

ifeq ($(MOST_BUILD_LOG_FILE),on)
BUILD_LOG_FILE := $(MOST_ROOT)/build.log
BUILD_ERR := 2>>$(BUILD_LOG_FILE)
REMOVE_BUILD_LOG := -$(REMOVE) $(BUILD_LOG_FILE) 
SHOW_BUILD_LOG := -@$(WC) $(BUILD_LOG_FILE)
endif

ifeq ($(MOST_BUILD_LOG),off)
BUILD_LOG :=@
endif

EXECPREF:=$(MOST_CROSS)/bin/$(MOST_TARGET)-

CC			:= $(EXECPREF)gcc
LD			:= $(EXECPREF)ld
AR			:= $(EXECPREF)ar
AS			:= $(EXECPREF)as
STRIP		:= $(EXECPREF)strip
OBJCOPY		:= $(EXECPREF)objcopy
OBJDUMP		:= $(EXECPREF)objdump

INC_PATH 	:= -I$(MOST_ROOT)/sys/inc -I$(MOST_ROOT)/usr/inc $(INC_PATH_OPT)

CCFLAGS		:= $(MOST_BUILD) $(TARGET_CCFLAGS) $(CCFLAGS_OPT) 
CCFLAGS		+= -fno-builtin -std=iso9899:199x -Wall -pedantic -Werror

DEFINES		:= $(TARGET_DEFINES) $(DEFINES_OPT)

LIB_PATH	:= $(LIB_PATH_OPT)
LIB_PATH	+= -L$(MOST_ROOT)/usr/lib/$(MOST_TARGET)
LIB_PATH	+= -L$(MOST_ROOT)/sys/lib/$(MOST_TARGET)
LIB_PATH	+= -L$(MOST_CROSS)/lib/$(MOST_LIB_GCC)

LDFLAGS		:= --sort-common --warn-common $(TARGET_LDFLAGS) -T$(PROG_NAME).ld $(LDFLAGS_OPT)
LDFLAGS		+= --cref -Map $(PROG_NAME).map

# Use absolute path names for files so that gdb can find the files
WORKING_DIR:=$(shell pwd)/

LOCAL_SOURCES		:= $(wildcard src/*.c)
LOCAL_OBJECTS		:= $(subst src,o/$(MOST_TARGET),$(subst .c,.o,$(LOCAL_SOURCES)))
LOCAL_ASMFILES		:= $(subst src,s/$(MOST_CPU),$(subst .c,.s,$(LOCAL_SOURCES)))
LOCAL_SUBMAKES		:= $(subst src,d/$(MOST_TARGET),$(subst .c,.d,$(LOCAL_SOURCES)))
LOCAL_ASM_SOURCES	:= $(wildcard src/$(MOST_CPU)/*.S)
LOCAL_ASM_OBJECTS	:= $(subst src/$(MOST_CPU),o/$(MOST_TARGET),$(subst .S,.o,$(LOCAL_ASM_SOURCES)))

SOURCES:=$(foreach ENTRY, $(LOCAL_SOURCES), $(WORKING_DIR)$(ENTRY))
OBJECTS:=$(foreach ENTRY, $(LOCAL_OBJECTS), $(WORKING_DIR)$(ENTRY))
ASMFILES:=$(foreach ENTRY, $(LOCAL_ASMFILES), $(WORKING_DIR)$(ENTRY))
SUBMAKES:=$(foreach ENTRY, $(LOCAL_SUBMAKES), $(WORKING_DIR)$(ENTRY))
ASM_SOURCES:=$(foreach ENTRY, $(LOCAL_ASM_SOURCES), $(WORKING_DIR)$(ENTRY))
ASM_OBJECTS:=$(foreach ENTRY, $(LOCAL_ASM_OBJECTS), $(WORKING_DIR)$(ENTRY))

.PHONY : default help clean asm obj lib prog bin srec hex info

default:
	@$(ECHO) "!!! This Makefile is called from other Makefiles !!!"

help :
	@$(ECHO) "---------------------- build/Makefile -------------------------------------------------------"
	@$(ECHO) "help  : !!! This Makefile is called from other Makefiles !!!"
	@$(ECHO) "clean : remove build log file"
	@$(ECHO) "asm   : build assembler files for all C-files in src directory for configured cpu"
	@$(ECHO) "obj   : build objects for all C- and assembler-files in src directory for configured cpu" 
	@$(ECHO) "lib   : build library"
	@$(ECHO) "prog  : build executeable(elf, coff) and stripped executeable"
	@$(ECHO) "bin   : build raw binary file (code, data) from stripped executeable)"
	@$(ECHO) "srec  : build srec file from stripped executeable"
	@$(ECHO) "hex   : build hex file from stripped executeable"
	@$(ECHO) "info  : print info for stripped executeable and for executeable to build log file"
	@$(ECHO) "---------------------------------------------------------------------------------------------"

clean:
	$(REMOVE_BUILD_LOG) 

$(BUILD_LOG_FILE):
	@$(DATE) >$(BUILD_LOG_FILE)

asm : $(BUILD_LOG_FILE) s/$(MOST_CPU) d/$(MOST_TARGET) $(ASMFILES)
	$(SHOW_BUILD_LOG)

obj : $(BUILD_LOG_FILE) o/$(MOST_TARGET) d/$(MOST_TARGET) $(OBJECTS) $(ASM_OBJECTS)
	$(SHOW_BUILD_LOG)

lib : $(BUILD_LOG_FILE) ../../lib/$(MOST_TARGET)
	@$(ECHO) "archive $(LIB_NAME)"
	$(BUILD_LOG)$(AR) cr ../../lib/$(MOST_TARGET)/lib_$(LIB_NAME).a $(OBJ_NAMES) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

prog : $(BUILD_LOG_FILE)
	@$(ECHO) "link $(PROG_NAME)"
	$(BUILD_LOG)$(LD) $(LDFLAGS) $(OBJ_NAMES) $(LIB_PATH) $(LIB_NAMES) -o $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(BUILD_LOG)$(STRIP) $(STRIP_FLAGS) $(STRIP_SECTIONS)\
	 -o $(PROG_NAME).stripped $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

bin : $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).bin"
	$(BUILD_LOG)$(OBJCOPY) -O binary -j .text -j .rodata $(PROG_NAME).stripped $(PROG_NAME).code $(BUILD_ERR)
	$(BUILD_LOG)$(OBJCOPY) -O binary -j .data $(PROG_NAME).stripped $(PROG_NAME).data $(BUILD_ERR)
	$(BUILD_LOG)cat $(PROG_NAME).code $(PROG_NAME).data > $(PROG_NAME).bin $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

srec : $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).srec"
	$(BUILD_LOG)$(OBJCOPY) -O srec $(PROG_NAME).stripped $(PROG_NAME).srec $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

hex: $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).hex"
	$(BUILD_LOG)$(OBJCOPY) -O ihex $(PROG_NAME).stripped $(PROG_NAME).hex $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

info : $(BUILD_LOG_FILE)
	@$(OBJDUMP) -h $(PROG_NAME).stripped
	$(BUILD_LOG)$(OBJDUMP) -h $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)


$(WORKING_DIR)o/$(MOST_TARGET)/%.o: $(WORKING_DIR)src/%.c
	@$(ECHO) "compile $<"
	$(BUILD_LOG)$(CC) -c $(CCFLAGS) $(INC_PATH) $(DEFINES) -MP -MMD -MF "d/$(MOST_TARGET)/$(patsubst %.c,%.d,$(<F))" $< -o $@ $(BUILD_ERR)

$(WORKING_DIR)o/$(MOST_TARGET)/%.o: $(WORKING_DIR)src/$(MOST_CPU)/%.S
	@$(ECHO) "compile $<"
	$(BUILD_LOG)$(CC) -c $(CCFLAGS) $(INC_PATH) $(DEFINES) -D__ASSEMBLY__ -MP -MMD -MF "d/$(MOST_TARGET)/$(patsubst %.S,%.d,$(<F))" $< -o $@ $(BUILD_ERR)

$(WORKING_DIR)s/$(MOST_CPU)/%.s : $(WORKING_DIR)src/%.c
	@$(ECHO) "compile $<"
	$(BUILD_LOG)$(CC) -S $(CCFLAGS) $(INC_PATH) $(DEFINES) -MP -MMD -MF "d/$(MOST_TARGET)/$(patsubst %.c,%.d,$(<F))" $< -o $@ $(BUILD_ERR)

s/$(MOST_CPU) : s
	@$(MAKEDIR) s/$(MOST_CPU)

s :
	@$(MAKEDIR) s

o/$(MOST_TARGET) : o
	@$(MAKEDIR) o/$(MOST_TARGET)

o :
	@$(MAKEDIR) o

d/$(MOST_TARGET) : d
	@$(MAKEDIR) d/$(MOST_TARGET)

d :
	@$(MAKEDIR) d

../../lib/$(MOST_TARGET) : ../../lib
	@$(MAKEDIR) ../../lib/$(MOST_TARGET)

../../lib :
	@$(MAKEDIR) ../../lib

-include $(SUBMAKES)	
