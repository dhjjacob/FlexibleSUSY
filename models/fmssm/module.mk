DIR          := models/fmssm
MODNAME      := libfmssm

LIBFMSSM_SRC  :=
LIBFMSSM_GENERATED_SRC :=
LIBFMSSM_INC  :=

ifneq ($(findstring lattice,$(ALGORITHMS)),)
LIBFMSSM_GENERATED_SRC += \
		$(DIR)/fmssm_lattice_rge.f \
		$(DIR)/fmssm_lattice_constraints.f \
		$(DIR)/fmssm_lattice_numerical_constraints_functions.f \
		$(DIR)/fmssm_lattice_numerical_constraints_dependence.cpp

LIBFMSSM_INC  += \
		$(DIR)/fmssm_lattice_translator.inc

LIBFMSSM_SRC  += \
		$(DIR)/fmssm_lattice.cpp \
		$(DIR)/fmssm_lattice_mz_constraint.cpp \
		$(DIR)/fmssm_lattice_numerical_mz_constraint.cpp \
		$(DIR)/fmssm_lattice_msusy_constraint.cpp \
		$(DIR)/fmssm_lattice_numerical_msusy_constraint.cpp \
		$(DIR)/fmssm_lattice_mx_constraint.cpp \
		$(DIR)/fmssm_oneloop.cpp \
		$(DIR)/fmssm_lattice_numerical_constraints.cpp \
		$(LIBFMSSM_GENERATED_SRC)
endif

LIBFMSSM_OBJ  := \
		$(patsubst %.cpp, %.o, $(filter %.cpp, $(LIBFMSSM_SRC))) \
		$(patsubst %.f, %.o, $(filter %.f, $(LIBFMSSM_SRC)))

LIBFMSSM_DEP  := \
		$(LIBFMSSM_OBJ:.o=.d)

LIBFMSSM      := $(DIR)/$(MODNAME)$(LIBEXT)

.PHONY:         all-$(MODNAME) clean-$(MODNAME) distclean-$(MODNAME)

all-$(MODNAME): $(LIBFMSSM)

clean-$(MODNAME):
		rm -rf $(LIBFMSSM_OBJ)

distclean-$(MODNAME): clean-$(MODNAME)
		rm -rf $(LIBFMSSM_DEP)
		rm -rf $(LIBFMSSM)
		rm -rf $(LIBFMSSM_GENERATED_SRC)
		rm -rf $(LIBFMSSM_INC)

clean::         clean-$(MODNAME)

distclean::     distclean-$(MODNAME)

$(DIR)/%.cpp : $(DIR)/%.cpp.m
	$(MATH) -run 'filename="$@"; << $<; Quit[]'

$(DIR)/%.f : $(DIR)/%.f.m
	$(MATH) -run 'filename="$@"; << $<; Quit[]'

$(DIR)/%.inc : $(DIR)/%.inc.m
	$(MATH) -run 'filename="$@"; << $<; Quit[]'

ifneq ($(findstring lattice,$(ALGORITHMS)),)
$(LIBFMSSM_DEP) $(LIBFMSSM_OBJ): CPPFLAGS += $(EIGENFLAGS) $(GSLFLAGS) $(BOOSTFLAGS)
endif

$(LIBFMSSM): $(LIBFMSSM_OBJ)
		$(MAKELIB) $@ $^

ALLDEP += $(LIBFMSSM_DEP)
ALLLIB += $(LIBFMSSM)
