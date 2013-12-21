-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

ifeq ($(MOST_CPU),h8300)
ifeq ($(MOST_CPU_TYPE),h8300h)
TARGET_CCFLAGS := -mh
TARGET_DEFINES := -DMOST_CPU_H8300
TARGET_LDFLAGS := -m $(MOST_CPU_TYPE)
endif
else
ifeq ($(MOST_CPU),i386)
TARGET_CCFLAGS := -m32 -fno-stack-protector -nostdinc
TARGET_DEFINES := -DMOST_CPU_I386
TARGET_LDFLAGS := -m$(MOST_CPU_TYPE)
else
ifeq ($(MOST_CPU),arm)
TARGET_CCFLAGS := -mcpu=$(MOST_CPU_TYPE) -m$(MOST_ENDIAN)-endian
TARGET_DEFINES := -DMOST_CPU_ARM 
TARGET_LDFLAGS :=
else
ifeq ($(MOST_CPU),m68k)
TARGET_CCFLAGS :=
TARGET_DEFINES := -DMOST_CPU_M68K
TARGET_LDFLAGS := 
else
$(error No cpu defined!)
endif 
endif
endif
endif



ifeq ($(MOST_ENDIAN),little)
TARGET_DEFINES += -DMOST_ENDIAN_LITTLE
else
ifeq ($(MOST_ENDIAN),big)
TARGET_DEFINES += -DMOST_ENDIAN_BIG
else
$(error No endian defined!)
endif
endif

LOG_FILE := $(MOST_ROOT)/build.log

ifeq ($(MOST_BUILD_LOG_FILE),on)
BUILD_LOG_FILE := $(LOG_FILE)
BUILD_ERR := 2>>$(LOG_FILE)
SHOW_BUILD_LOG := -@$(WC) $(LOG_FILE)
endif

ifeq ($(MOST_BUILD_MSG),off)
BUILD_MSG :=@
endif

ifeq ($(MOST_BUILD),debug)
TARGET_DEFINES += -DMOST_BUILD_DEBUG
BUILD_CCFLAGS:=$(MOST_BUILD_DEBUG_FLAGS)
REMOVE_CCFLAGS:=
REMOVE_LDFLAGS:=
else
ifeq ($(MOST_BUILD),run)
TARGET_DEFINES += -DMOST_BUILD_RUN
BUILD_CCFLAGS:=$(MOST_BUILD_RUN_FLAGS)
ifeq ($(MOST_REMOVE_UNUSED),on)
REMOVE_CCFLAGS:=-ffunction-sections
REMOVE_LDFLAGS:=--gc-sections
else
REMOVE_CCFLAGS:=
REMOVE_LDFLAGS:=
endif
else
$(error No build variant defined! (run, debug))
endif
endif


PROG_NAME := $(MOST_BUILD)/$(MOST_APP)

EXECPREF:=$(MOST_CROSS)/bin/$(MOST_TARGET)-

CC			:= $(EXECPREF)gcc
LD			:= $(EXECPREF)ld
AR			:= $(EXECPREF)ar
AS			:= $(EXECPREF)as
STRIP		:= $(EXECPREF)strip
OBJCOPY		:= $(EXECPREF)objcopy
OBJDUMP		:= $(EXECPREF)objdump

INC_PATH 	:= -I$(MOST_ROOT)/sys/inc -I$(MOST_ROOT)/usr/inc $(INC_PATH_OPT)

CCFLAGS		:= $(BUILD_CCFLAGS) $(TARGET_CCFLAGS) $(REMOVE_CCFLAGS) $(CCFLAGS_OPT) 
CCFLAGS		+= -fno-builtin -std=iso9899:199x -Wall -Werror
#CCFLAGS		+= -fno-builtin -std=iso9899:199x -Wall -pedantic -Werror

DEFINES		:= $(TARGET_DEFINES) $(DEFINES_OPT)

LIB_PATH	:= $(LIB_PATH_OPT)
LIB_PATH	+= -L$(MOST_ROOT)/usr/lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)
LIB_PATH	+= -L$(MOST_ROOT)/sys/lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)
LIB_PATH	+= -L$(MOST_CROSS)/lib/$(MOST_LIB_GCC)

LDFLAGS		:= --sort-common --warn-common $(TARGET_LDFLAGS) $(REMOVE_LDFLAGS) -T$(MOST_APP).ld $(LDFLAGS_OPT)
LDFLAGS		+= --cref -Map $(PROG_NAME).map

# Use absolute path names for files so that gdb can find the files
WORKING_DIR:=$(shell pwd)

LOCAL_SOURCES		:= $(wildcard src/*.c)
LOCAL_OBJECTS		:= $(subst src,$(MOST_BUILD)/o/$(MOST_TARGET_TYPE),$(subst .c,.o,$(LOCAL_SOURCES)))
LOCAL_ASMFILES		:= $(subst src,$(MOST_BUILD)/s/$(MOST_CPU_TYPE),$(subst .c,.s,$(LOCAL_SOURCES)))
LOCAL_SUBMAKES		:= $(subst src,$(MOST_BUILD)/d/$(MOST_TARGET_TYPE),$(subst .c,.d,$(LOCAL_SOURCES)))
LOCAL_ASM_SOURCES	:= $(wildcard src/$(MOST_CPU)/*.S)
LOCAL_ASM_OBJECTS	:= $(subst src/$(MOST_CPU),$(MOST_BUILD)/o/$(MOST_TARGET_TYPE),$(subst .S,.o,$(LOCAL_ASM_SOURCES)))

SOURCES:=$(foreach ENTRY, $(LOCAL_SOURCES), $(WORKING_DIR)/$(ENTRY))
OBJECTS:=$(foreach ENTRY, $(LOCAL_OBJECTS), $(WORKING_DIR)/$(ENTRY))
ASMFILES:=$(foreach ENTRY, $(LOCAL_ASMFILES), $(WORKING_DIR)/$(ENTRY))
SUBMAKES:=$(foreach ENTRY, $(LOCAL_SUBMAKES), $(WORKING_DIR)/$(ENTRY))
ASM_SOURCES:=$(foreach ENTRY, $(LOCAL_ASM_SOURCES), $(WORKING_DIR)/$(ENTRY))
ASM_OBJECTS:=$(foreach ENTRY, $(LOCAL_ASM_OBJECTS), $(WORKING_DIR)/$(ENTRY))

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
	-$(REMOVE) $(LOG_FILE) 

$(BUILD_LOG_FILE):
	@$(DATE) >$(LOG_FILE)

asm : $(BUILD_LOG_FILE) $(MOST_BUILD)/s/$(MOST_CPU_TYPE) $(MOST_BUILD)/d/$(MOST_TARGET_TYPE) $(ASMFILES)
	$(SHOW_BUILD_LOG)

obj : $(BUILD_LOG_FILE) $(MOST_BUILD)/o/$(MOST_TARGET_TYPE) $(MOST_BUILD)/d/$(MOST_TARGET_TYPE) $(OBJECTS) $(ASM_OBJECTS)
	$(SHOW_BUILD_LOG)

lib : $(BUILD_LOG_FILE) ../../lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)
	@$(ECHO) "archive $(LIB_NAME)"
	$(BUILD_MSG)$(AR) cr ../../lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)/lib_$(LIB_NAME).a $(OBJ_NAMES) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

prog : $(BUILD_LOG_FILE)
	@$(ECHO) "link $(PROG_NAME)"
	$(BUILD_MSG)$(LD) $(LDFLAGS) $(OBJ_NAMES) $(LIB_PATH) $(LIB_NAMES) -o $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(BUILD_MSG)$(STRIP) $(STRIP_FLAGS) $(STRIP_SECTIONS)\
	 -o $(PROG_NAME).stripped $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

bin : $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).bin"
	$(BUILD_MSG)$(OBJCOPY) -O binary $(CODE_SECTIONS) $(PROG_NAME).stripped $(PROG_NAME).code $(BUILD_ERR)
	$(BUILD_MSG)$(OBJCOPY) -O binary $(DATA_SECTIONS) $(PROG_NAME).stripped $(PROG_NAME).data $(BUILD_ERR)
	$(BUILD_MSG)$(CAT) $(PROG_NAME).code $(PROG_NAME).data > $(PROG_NAME).bin $(BUILD_ERR)
	$(BUILD_MSG)$(LS) $(PROG_NAME).bin $(BUILD_ERR)
	@$(ECHO) "check size of $(PROG_NAME).bin (< $(MAX_BIN_SIZE))"
	$(BUILD_MSG)$(TEST) -z "$$($(FIND) $(PROG_NAME).bin -size +$(MAX_BIN_SIZE))" $(BUILD_ERR)
	@$(ECHO) "size of $(PROG_NAME).bin OK"
	$(SHOW_BUILD_LOG)

srec : $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).srec"
	$(BUILD_MSG)$(OBJCOPY) -O srec $(PROG_NAME).stripped $(PROG_NAME).srec $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

hex: $(BUILD_LOG_FILE)
	@$(ECHO) "generate $(PROG_NAME).hex"
	$(BUILD_MSG)$(OBJCOPY) -O ihex $(PROG_NAME).stripped $(PROG_NAME).hex $(BUILD_ERR)
	$(SHOW_BUILD_LOG)

info : $(BUILD_LOG_FILE)
	@$(ECHO) "info $(PROG_NAME)"
	$(BUILD_MSG)$(OBJDUMP) -h $(PROG_NAME).stripped
	$(BUILD_MSG)$(OBJDUMP) -h $(PROG_NAME).$(MOST_SYSTEM) $(BUILD_ERR)
	$(SHOW_BUILD_LOG)


$(WORKING_DIR)/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/%.o: $(WORKING_DIR)/src/%.c
	@$(ECHO) "compile $<"
	$(BUILD_MSG)$(CC) -c $(CCFLAGS) $(INC_PATH) $(DEFINES) -MP -MMD -MF "$(MOST_BUILD)/d/$(MOST_TARGET_TYPE)/$(patsubst %.c,%.d,$(<F))" $< -o $@ $(BUILD_ERR)

$(WORKING_DIR)/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/%.o: $(WORKING_DIR)/src/$(MOST_CPU)/%.S
	@$(ECHO) "compile $<"
	$(BUILD_MSG)$(CC) -c $(CCFLAGS) $(INC_PATH) $(DEFINES) -D__ASSEMBLY__ -MP -MMD -MF "$(MOST_BUILD)/d/$(MOST_TARGET_TYPE)/$(patsubst %.S,%.d,$(<F))" $< -o $@ $(BUILD_ERR)

$(WORKING_DIR)/$(MOST_BUILD)/s/$(MOST_CPU_TYPE)/%.s : $(WORKING_DIR)/src/%.c
	@$(ECHO) "compile $<"
	$(BUILD_MSG)$(CC) -S $(CCFLAGS) $(INC_PATH) $(DEFINES) -MP -MMD -MF "$(MOST_BUILD)/d/$(MOST_TARGET_TYPE)/$(patsubst %.c,%.d,$(<F))" $< -o $@ $(BUILD_ERR)


$(MOST_BUILD)/s/$(MOST_CPU_TYPE) : $(MOST_BUILD)/s
	@$(MAKEDIR) $(MOST_BUILD)/s/$(MOST_CPU_TYPE)

$(MOST_BUILD)/s : $(MOST_BUILD)
	@$(MAKEDIR) $(MOST_BUILD)/s

$(MOST_BUILD)/o/$(MOST_TARGET_TYPE) : $(MOST_BUILD)/o
	@$(MAKEDIR) $(MOST_BUILD)/o/$(MOST_TARGET_TYPE)

$(MOST_BUILD)/o : $(MOST_BUILD)
	@$(MAKEDIR) $(MOST_BUILD)/o

$(MOST_BUILD)/d/$(MOST_TARGET_TYPE) : $(MOST_BUILD)/d
	@$(MAKEDIR) $(MOST_BUILD)/d/$(MOST_TARGET_TYPE)

$(MOST_BUILD)/d : $(MOST_BUILD)
	@$(MAKEDIR) $(MOST_BUILD)/d

$(MOST_BUILD) :
	@$(MAKEDIR) $(MOST_BUILD)
	

../../lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE) : ../../lib/$(MOST_BUILD)
	@$(MAKEDIR) ../../lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)

../../lib/$(MOST_BUILD) : ../../lib  
	@$(MAKEDIR) ../../lib/$(MOST_BUILD)

../../lib :  
	@$(MAKEDIR) ../../lib

-include $(SUBMAKES)	
