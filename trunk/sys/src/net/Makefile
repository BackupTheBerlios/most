-include $(MOST_ROOT)/etc/Config

LIB_NAME := net
LIB_TYPE := sys

OBJ_NAMES :=  	eth/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		 		netif/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		 		ipv4/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		 		mm/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
		 		udp/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = 	   	make $(1) -C eth;\
	   			make $(1) -C netif;\
	   			make $(1) -C ipv4;\
	   			make $(1) -C mm;\
	   			make $(1) -C udp;

-include $(MOST_ROOT)/etc/build/lib/Makefile
