-include $(MOST_ROOT)/etc/build/Commands

.PHONY : default doc wiz mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(ECHO) "---------------------- build/doc/Makefile ------------------------------------------"
	@$(ECHO) "help     : print this help message"
	@$(ECHO) "doc      : execute doxygen in current dirrectory"
	@$(ECHO) "wiz      : doxywizard Doxyfile"
	@$(ECHO) "mrproper : -> remove html and latex directories in current directory"
	@$(ECHO) "------------------------------------------------------------------------------------"

doc :
	doxygen

wiz :
	doxywizard Doxyfile

mrproper :
	-$(REMOVE) html
	-$(REMOVE) latex
