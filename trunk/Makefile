-include $(MOST_ROOT)/etc/Config
-include $(MOST_ROOT)/etc/build/Commands

makegoal_lib_src = make $(1) -C sys/src;\
					make $(1) -C usr/src;

makegoal_lib_doc = make $(1) -C sys/inc/doc;\
					make $(1) -C usr/inc/doc;

makegoal_bsp_src = make $(1) -C bsp/$(MOST_BOARD);

makegoal_bsp_doc = \
					make $(1) -C bsp/mdc/sys/inc/doc;\
					make $(1) -C bsp/sam/sys/inc/doc;\
					make $(1) -C bsp/la2/sys/inc/doc;\
					make $(1) -C bsp/pc/sys/inc/doc;

makegoal_bsp_mrproper = \
					make $(1) -C bsp/mdc;\
					make $(1) -C bsp/sam;\
					make $(1) -C bsp/la2;\
					make $(1) -C bsp/pc;
					
.PHONY : default help all doc clean mrproper

default:
	@$(ECHO) "Please use <make help> for more information."

help :
	@$(call makegoal_lib_doc, help)
	@$(call makegoal_bsp_doc, help)
	@$(call makegoal_lib_src, help)
	@$(call makegoal_bsp_src, help)
	@$(ECHO) "---------------------- Makefile -------------------------------------------------------"
	@$(ECHO) "help     : print this help message, -> Makefile"
	@$(ECHO) "all      : sys/src, usr/src -> Makefile(libs), bsp/$(MOST_BOARD) -> Makefile(app)"
	@$(ECHO) "doc      : sys/inc/doc, usr/inc/doc , bsp/$(MOST_BOARD)/sys/inc/doc -> Makefile"
	@$(ECHO) "clean    : sys/src, usr/src, bsp/$(MOST_BOARD) -> Makefile"
	@$(ECHO) "mrproper : sys/src, usr/src, sys/inc/doc, usr/inc/doc, bsp/$(MOST_BOARD), bsp/$(MOST_BOARD)/sys/inc/doc -> Makefile"
	@$(ECHO) "---------------------------------------------------------------------------------------------"

all :
	@make clean --no-print-directory -f $(MOST_ROOT)/etc/build/Makefile
	@$(call makegoal_lib_src, libs)
	@$(call makegoal_bsp_src, app)

doc :
	@$(call makegoal_lib_doc, doc)
	@$(call makegoal_bsp_doc, doc)

clean :
	@$(call makegoal_lib_src, clean)
	@$(call makegoal_bsp_src, clean)

mrproper :
	@$(call makegoal_lib_src, mrproper)
	@$(call makegoal_lib_doc, mrproper)
	@$(call makegoal_bsp_mrproper, mrproper)
	@$(call makegoal_bsp_doc, mrproper)

