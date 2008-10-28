-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

makegoal = make $(1) -C usr/test;

.PHONY : default all clean mrproper ocd-start ocd-stop

default:
	@$(ECHO) "Please use <make all | clean | mrproper | ocd-start | ocd-stop>."

all :
	@$(call makegoal, hex)
	@$(call makegoal, bin)

clean :
	@$(call makegoal, clean)

mrproper :
	@$(call makegoal, mrproper)

ocd-start:
	openocd -f $(MOST_ROOT)/ocd/daemon.cfg\
	     -f $(MOST_ROOT)/ocd/interfaces/olimex-arm-usb-ocd.cfg\
	     -f ocd/targets/sam7x256.cfg\
	     -c "init" -c "reset"

ocd-debug:
	openocd -f $(MOST_ROOT)/ocd/daemon.cfg\
	     -f $(MOST_ROOT)/ocd/interfaces/olimex-arm-usb-ocd.cfg\
	     -f ocd/targets/sam7x256.cfg\
	     -c "init" -c "reset" -d

ocd-stop:
	killall openocd
