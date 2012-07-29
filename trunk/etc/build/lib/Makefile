-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

export LIB_NAME LIB_TYPE OBJ_NAMES

.PHONY : default help lib obj clean mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(call makegoal, help)
	@$(ECHO) "---------- build/lib/Makefile: LIB = $(LIB_NAME)------------------------------------"
	@$(ECHO) "help      : print this help mesage, -> Makefile"
	@$(ECHO) "lib(obj)  : -> Makefile"
	@$(ECHO) "obj       : -> Makefile"
	@$(ECHO) "clean     : remove target library in parent directory, -> Makefile"
	@$(ECHO) "mrproper  : remove target library in parent directory, -> Makefile"
	@$(ECHO) "---------------------------------------------------------------------------------------------"

lib : obj
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

obj :
	@$(call makegoal, $@)

clean : 
	@$(call makegoal, $@)
	-$(REMOVE) ../lib/$(MOST_TARGET)/lib_$(LIB_NAME).a

mrproper:
	@$(call makegoal, $@)
	-$(REMOVE) ../lib/$(MOST_TARGET)/lib_$(LIB_NAME).a
