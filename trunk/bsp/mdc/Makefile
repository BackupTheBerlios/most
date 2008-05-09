-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

makegoal = make $(1) -C usr/putboot;\
	       make $(1) -C usr/boot;\
		   make $(1) -C usr/frame;\
		   make $(1) -C usr/test;

.PHONY : default all clean mrproper

default:
	@$(ECHO) "Please use <make all | clean | mrproper>."

all :
	@$(call makegoal, bin)

clean :
	@$(call makegoal, clean)

mrproper :
	@$(call makegoal, mrproper)
			   