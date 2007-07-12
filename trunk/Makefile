-include $(MOST_ROOT)/etc/Commands

makegoal_lib = make $(1) -C sys/src;\
	       	   make $(1) -C usr/src;

makegoal_mdc = make $(1) -C bsp/mdc/usr/putboot;\
	       	   make $(1) -C bsp/mdc/usr/boot;\
			   make $(1) -C bsp/mdc/usr/frame;\
			   make $(1) -C bsp/mdc/usr/test;
			   
.PHONY : default all clean

default:
	@$(ECHO) "Please use <make all | clean>."

all :
	@$(call makegoal_lib, lib)
	@$(call makegoal_mdc, bin)

clean :
	@$(call makegoal_lib, clean_lib)
	@$(call makegoal_mdc, clean_prog)
	       