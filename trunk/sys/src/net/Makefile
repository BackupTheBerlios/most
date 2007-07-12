-include $(MOST_ROOT)/etc/Config

LIB_NAME := net
LIB_TYPE := sys
OBJ_NAMES :=  arch/obj/$(MOST_TARGET)/*.o\
		 eth/obj/$(MOST_TARGET)/*.o\
		 netif/obj/$(MOST_TARGET)/*.o\
		 ipv4/obj/$(MOST_TARGET)/*.o\
		 mm/obj/$(MOST_TARGET)/*.o\
		 udp/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C arch;\
	   make $(1) -C eth;\
	   make $(1) -C netif;\
	   make $(1) -C ipv4;\
	   make $(1) -C mm;\
	   make $(1) -C udp;

-include $(MOST_ROOT)/etc/Makefile.lib
