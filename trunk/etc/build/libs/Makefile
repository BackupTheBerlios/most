-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

.PHONY : default help libs clean mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(call makegoal, help)
	@$(ECHO) "---------------------- build/libs/Makefile -------------------------------------------------------"
	@$(ECHO) "help     : print this help message"
	@$(ECHO) "libs     : -> Makefile(lib)"
	@$(ECHO) "clean    : remove all target libraries in parent directory, -> Makefile"
	@$(ECHO) "mrproper : remove all libraries in parent directory, -> Makefile"
	@$(ECHO) "---------------------------------------------------------------------------------------------"

libs :
	@$(call makegoal, lib)

clean :
	@$(call makegoal, $@)
	-$(REMOVE) ../lib/$(MOST_BUILD)/$(MOST_TARGET_TYPE)

mrproper:
	@$(call makegoal, $@)
	-$(REMOVE) ../lib
