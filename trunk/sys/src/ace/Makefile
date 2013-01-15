-include $(MOST_ROOT)/etc/Config

LIB_NAME := ace
LIB_TYPE := sys
OBJ_NAMES := ctype/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 stdio/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 stdlib/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o\
			 string/$(MOST_BUILD)/o/$(MOST_TARGET_TYPE)/*.o

makegoal = make $(1) -C ctype;\
	       make $(1) -C stdio;\
	       make $(1) -C stdlib;\
	       make $(1) -C string;

-include $(MOST_ROOT)/etc/build/lib/Makefile
