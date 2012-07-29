-include $(MOST_ROOT)/etc/Config

LIB_NAME := ace
LIB_TYPE := sys
OBJ_NAMES := ctype/o/$(MOST_TARGET)/*.o\
			 stdio/o/$(MOST_TARGET)/*.o\
			 stdlib/o/$(MOST_TARGET)/*.o\
			 string/o/$(MOST_TARGET)/*.o

makegoal = make $(1) -C ctype;\
	       make $(1) -C stdio;\
	       make $(1) -C stdlib;\
	       make $(1) -C string;

-include $(MOST_ROOT)/etc/build/lib/Makefile
