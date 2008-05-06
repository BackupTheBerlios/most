-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

makegoal_lib = make $(1) -C sys/src;\
	       	   make $(1) -C usr/src;

ifeq ($(MOST_BOARD),mdc)
makegoal_bsp = make $(1) -C bsp/mdc/usr/putboot;\
	       	   make $(1) -C bsp/mdc/usr/boot;\
			   make $(1) -C bsp/mdc/usr/frame;\
			   make $(1) -C bsp/mdc/usr/test;
else
ifeq ($(MOST_BOARD),sam)
makegoal_bsp = make $(1) -C bsp/sam/usr/test;
else
$(error No board defined!)
endif 
endif

.PHONY : default all clean mrproper

default:
	@$(ECHO) "Please use <make all | clean | mrproper>."

all :
	@$(call makegoal_lib, libs)
	@$(call makegoal_bsp, bin)

clean :
	@$(call makegoal_lib, clean)
	@$(call makegoal_bsp, clean)

mrproper :
	@$(call makegoal_lib, mrproper)
	@$(call makegoal_bsp, mrproper)

