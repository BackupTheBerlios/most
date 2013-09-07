-include $(MOST_ROOT)/etc/build/Commands

LIB_NAMES := $(LIB_NAMES_USR) $(LIB_NAMES_SYS) -lgcc 

export LIB_PATH_OPT LIB_NAMES LDFLAGS_OPT

export PROG_NAME OBJ_NAMES MAX_BIN_SIZE STRIP_FLAGS STRIP_SECTIONS CODE_SECTIONS DATA_SECTIONS

OPEN_OCD := openocd -s $(MOST_ROOT)/etc/ocd/ -f $(MOST_ROOT)/bsp/$(MOST_BOARD)/openocd.cfg

.PHONY : default help prog bin srec hex app info obj clean mrproper ocd-debug ocd-reset ocd-program

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(call makegoal_src, help)
	@$(call makegoal_inc, help)
	@$(ECHO) "----------build/prog/Makefile: PROG = $(PROG_NAME)-----------------------------------"
	@$(ECHO) "help                 : print this help message, -> Makefile" 
	@$(ECHO) "prog(obj)            : -> Makefile" 
	@$(ECHO) "bin(prog)            : -> Makefile" 
	@$(ECHO) "srec(prog)           : -> Makefile" 
	@$(ECHO) "hex(prog)            : -> Makefile" 
	@$(ECHO) "app($(MOST_FILE))    : build application"
	@$(ECHO) "info                 : -> Makefile" 
	@$(ECHO) "obj                  : -> Makefile" 
	@$(ECHO) "clean                : remove $(MOST_BUILD), -> Makefile" 
	@$(ECHO) "mrproper             : remove run debug,-> Makefile" 
	@$(ECHO) "ocd-debug            : run ocd debugger" 
	@$(ECHO) "ocd-reset            : reset targert" 
	@$(ECHO) "ocd-program          : program target" 
	@$(ECHO) "------------------------------------------------------------------------------------"

prog : $(MOST_BUILD) obj
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

bin : prog
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

srec : prog
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

hex : prog
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

app : $(MOST_FILE)

info :
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

obj :
	@$(call makegoal_src, $@)

$(MOST_BUILD) :
	@$(MAKEDIR) $(MOST_BUILD)
	

clean :
	-$(REMOVE) $(MOST_BUILD)
	@$(call makegoal_src, $@)

mrproper :
	-$(REMOVE) run
	-$(REMOVE) debug
	@$(call makegoal_src, $@)

ocd-debug:
	$(OPEN_OCD) -f ocd/debug.cfg

ocd-reset:
	$(OPEN_OCD) -f ocd/reset.cfg

ocd-program:
	$(OPEN_OCD) -f ocd/$(MOST_BUILD)-program.cfg

