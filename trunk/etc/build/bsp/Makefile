-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

.PHONY : default help apps clean mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(call makegoal, help)
	@$(ECHO) "----------build/bsp/Makefile : BOARD = $(MOST_BOARD) -------------------------------"
	@$(ECHO) "help     : print this help message, -> Makefile"
	@$(ECHO) "app      : -> Makefile(app)"
	@$(ECHO) "clean    : -> Makefile"
	@$(ECHO) "mrproper : -> Makefile"
	@$(ECHO) "------------------------------------------------------------------------------------"

app  :
	@$(call makegoal, app)

clean :
	@$(call makegoal, clean)

mrproper :
	@$(call makegoal_mrproper, mrproper)

