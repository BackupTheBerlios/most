-include $(MOST_ROOT)/etc/Commands

makegoal = make $(1) -C ace/doc;\
	       make $(1) -C uso/doc;\
	       make $(1) -C dev/doc;\
	       make $(1) -C net/doc;\
	       make $(1) -C nap/doc;\
	       make $(1) -C cli/doc;


.PHONY : default doc clean

default:
	@$(ECHO) "Please use <make doc | make clean> for generating, removing api documentation."

doc :
	@$(call makegoal, $@)

clean :
	@$(call makegoal, $@)
	       