DIR          := higher_order/NMSSM_higgs
MODNAME      := higher_order_NMSSM_higgs
WITH_$(MODNAME) := yes

LIB_higher_order_NMSSM_higgs_MK  := \
		$(DIR)/module.mk

LIB_higher_order_NMSSM_higgs_SRC := \
		$(DIR)/nmssm_twoloophiggs.cpp \
		$(DIR)/nmssm2loop.f

LIB_higher_order_NMSSM_higgs_HDR := \
		$(DIR)/nmssm_twoloophiggs.hpp \
		$(DIR)/nmssm2loop.h

LIB_higher_order_NMSSM_higgs_OBJ := \
		$(patsubst %.cpp, %.o, $(filter %.cpp, $(LIB_higher_order_NMSSM_higgs_SRC))) \
		$(patsubst %.f, %.o, $(filter %.f, $(LIB_higher_order_NMSSM_higgs_SRC)))

LIB_higher_order_NMSSM_higgs_DEP := $(LIB_higher_order_NMSSM_higgs_OBJ:.o=.d)

LIB_higher_order_NMSSM_higgs     := $(DIR)/lib$(MODNAME)$(MODULE_LIBEXT)

LIB_higher_order_NMSSM_higgs_INSTALL_DIR := $(INSTALL_DIR)/$(DIR)

.PHONY:         all-$(MODNAME) clean-$(MODNAME) clean-$(MODNAME)-dep \
		clean-$(MODNAME)-lib clean-$(MODNAME)-obj distclean-$(MODNAME)

all-$(MODNAME): $(LIB_higher_order_NMSSM_higgs)
		@true

ifneq ($(INSTALL_DIR),)
install-src::
		install -d $(LIB_higher_order_NMSSM_higgs_INSTALL_DIR)
		install -m u=rw,g=r,o=r $(LIB_higher_order_NMSSM_higgs_SRC) $(LIB_higher_order_NMSSM_higgs_INSTALL_DIR)
		install -m u=rw,g=r,o=r $(LIB_higher_order_NMSSM_higgs_HDR) $(LIB_higher_order_NMSSM_higgs_INSTALL_DIR)
		install -m u=rw,g=r,o=r $(LIB_higher_order_NMSSM_higgs_MK) $(LIB_higher_order_NMSSM_higgs_INSTALL_DIR)
endif

clean-$(MODNAME)-dep:
		-rm -f $(LIB_higher_order_NMSSM_higgs_DEP)

clean-$(MODNAME)-lib:
		-rm -f $(LIB_higher_order_NMSSM_higgs)

clean-$(MODNAME)-obj:
		-rm -f $(LIB_higher_order_NMSSM_higgs_OBJ)

clean-$(MODNAME): clean-$(MODNAME)-dep clean-$(MODNAME)-lib clean-$(MODNAME)-obj
		@true

distclean-$(MODNAME): clean-$(MODNAME)

clean-obj::     clean-$(MODNAME)-obj

clean::         clean-$(MODNAME)

distclean::     distclean-$(MODNAME)

$(LIB_higher_order_NMSSM_higgs_DEP) $(LIB_higher_order_NMSSM_higgs_OBJ): CPPFLAGS += $(GSLFLAGS) $(EIGENFLAGS) $(BOOSTFLAGS) $(SQLITEFLAGS) $(TSILFLAGS)

ifneq (,$(findstring yes,$(ENABLE_LOOPTOOLS)$(ENABLE_FFLITE)))
$(LIB_higher_order_NMSSM_higgs_DEP) $(LIB_higher_order_NMSSM_higgs_OBJ): CPPFLAGS += $(LOOPFUNCFLAGS)
endif

ifeq ($(ENABLE_SHARED_LIBS),yes)
$(LIB_higher_order_NMSSM_higgs): $(LIB_higher_order_NMSSM_higgs_OBJ)
		$(MODULE_MAKE_LIB_CMD) $@ $^ $(BOOSTTHREADLIBS) $(GSLLIBS) $(LAPACKLIBS) $(BLASLIBS) $(FLIBS) $(SQLITELIBS) $(TSILLIBS) $(THREADLIBS)
else
$(LIB_higher_order_NMSSM_higgs): $(LIB_higher_order_NMSSM_higgs_OBJ)
		$(MODULE_MAKE_LIB_CMD) $@ $^
endif

ALLDEP += $(LIB_higher_order_NMSSM_higgs_DEP)
ALLLIB += $(LIB_higher_order_NMSSM_higgs)