-include $(MOST_ROOT)/etc/Commands

.PHONY : default doc doc_wiz clean_doc

default:
	@$(ECHO) "Please use <make doc | make doc-wiz | make clean_doc> for generating, removing api documentation."

doc :
	doxygen

doc_wiz :
	doxywizard Doxyfile
	
clean_doc :
	-$(REMOVE) html
	-$(REMOVE) latex
		