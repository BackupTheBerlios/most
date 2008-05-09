-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

makegoal_lib = make $(1) -C sys/src;\
	       	   make $(1) -C usr/src;

makegoal_bsp = make $(1) -C bsp/$(MOST_BOARD);

.PHONY : default all clean mrproper

default:
	@$(ECHO) "Please use <make all | clean | mrproper>."

all :
	@$(call makegoal_lib, libs)
	@$(call makegoal_bsp, all)

clean :
	@$(call makegoal_lib, clean)
	@$(call makegoal_bsp, clean)

mrproper :
	@$(call makegoal_lib, mrproper)
	@$(call makegoal_bsp, mrproper)

