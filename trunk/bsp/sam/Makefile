-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/Commands

OPEN_OCD := 	openocd -f $(MOST_ROOT)/etc/ocd/daemon.cfg\
	     -f $(MOST_ROOT)/etc/ocd/interfaces/$(MOST_JTAG).cfg\
	     -f ocd/targets/sam7x256.cfg\
	     -c "init" -c "reset"

makegoal = make $(1) -C usr/test;

.PHONY : default all clean mrproper ocd-start ocd-start-d ocd-stop

default:
	@$(ECHO) "Please use <make all | clean | mrproper | ocd-start | ocd-start-d | ocd-stop>."

all :
	@$(call makegoal, hex)
	@$(call makegoal, bin)

clean :
	@$(call makegoal, clean)

mrproper :
	@$(call makegoal, mrproper)

ocd-start:
	$(OPEN_OCD)
	
ocd-start-d:
	$(OPEN_OCD) -d

ocd-stop:
	killall openocd
