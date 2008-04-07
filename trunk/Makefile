-include $(MOST_ROOT)/etc/Commands

makegoal_lib = make $(1) -C sys/src;\
	       	   make $(1) -C usr/src;

makegoal_mdc = make $(1) -C bsp/mdc/usr/putboot;\
	       	   make $(1) -C bsp/mdc/usr/boot;\
			   make $(1) -C bsp/mdc/usr/frame;\
			   make $(1) -C bsp/mdc/usr/test;

.PHONY : default all clean mrproper

default:
	@$(ECHO) "Please use <make all | clean | mrproper>."

all :
	@$(call makegoal_lib, libs)
	@$(call makegoal_mdc, bin)

clean :
	@$(call makegoal_lib, clean)
	@$(call makegoal_mdc, clean)

mrproper :
	@$(call makegoal_lib, mrproper)
	@$(call makegoal_mdc, mrproper)

