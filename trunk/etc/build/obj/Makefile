-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

export INC_PATH_OPT CCFLAGS_OPT DEFINES_OPT

.PHONY : default help obj asm clean mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@make help --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile
	@$(ECHO) "---------------------- build/obj/Makefile -----------------------------------------------------"
	@$(ECHO) "help       : print this help message, -> Makefile"
	@$(ECHO) "obj, asm   : -> Makefile"
	@$(ECHO) "clean      : remove all target build files in current directory, -> Makefile" 
	@$(ECHO) "mrproper   : remove all build files in current directory, -> Makefile(clean)"
	@$(ECHO) "---------------------------------------------------------------------------------------------"

obj: 
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

asm :
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile

clean : 
	@make $@ --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile
	-$(REMOVE) o/$(MOST_TARGET)
	-$(REMOVE) d/$(MOST_TARGET)
	-$(REMOVE) s/$(MOST_CPU)

mrproper :
	@make clean --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile
	-$(REMOVE) o
	-$(REMOVE) d
	-$(REMOVE) s
