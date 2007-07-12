-include $(MOST_ROOT)/etc/Config

LIB_NAME := ace
LIB_TYPE := sys
OBJ_NAMES := ctype/obj/$(MOST_TARGET)/*.o\
			 stdio/obj/$(MOST_TARGET)/*.o\
			 stdlib/obj/$(MOST_TARGET)/*.o\
			 string/obj/$(MOST_TARGET)/*.o

makegoal = make $(1) -C ctype;\
	       make $(1) -C stdio;\
	       make $(1) -C stdlib;\
	       make $(1) -C string;

-include $(MOST_ROOT)/etc/Makefile.lib
