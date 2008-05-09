-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

makegoal = make $(1) -C usr/test;

.PHONY : default all clean mrproper

default:
	@$(ECHO) "Please use <make all | clean | mrproper>."

all :
	@$(call makegoal, hex)

clean :
	@$(call makegoal, clean)

mrproper :
	@$(call makegoal, mrproper)
