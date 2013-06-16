makegoal = make $(1) -C usr/$(MOST_APP)

makegoal_mrproper =  make $(1) -C usr/putboot;\
					make $(1) -C usr/boot;\
					make $(1) -C usr/frame;\
					make $(1) -C usr/test;

-include $(MOST_ROOT)/etc/build/bsp/Makefile
