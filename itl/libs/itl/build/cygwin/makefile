#------------------------------------------------------------------------------
# makefile for the inteval template library
#------------------------------------------------------------------------------

#--- Platformdependent variables ----------------------------------------------
#--- cygwin
EXE_TAG = .exe
#--- unix
#EXE_TAG = 


#------------------------------------------------------------------------------
# Locations
#------------------------------------------------------------------------------

#--- location of this makefile ------------------------------------------------
MAKE_PATH = .
#--- location of the interval template library rootpath -----------------------
# build.itl.libs.itl
ITL_ROOT_PATH = ../../../..
#--- root directory of the interval template library --------------------------
ITL_SRC_PATH        = $(ITL_ROOT_PATH)/boost/itl
ITL_XT_SRC_PATH     = $(ITL_ROOT_PATH)/boost/itl_xt
ITL_LIBS_PATH       = $(ITL_ROOT_PATH)/libs/itl
ITL_XT_LIBS_PATH    = $(ITL_ROOT_PATH)/libs/itl_xt
ITL_EXAMPLE_PATH    = $(ITL_LIBS_PATH)/example
ITL_XT_EXAMPLE_PATH = $(ITL_XT_LIBS_PATH)/example
ITL_TEST_PATH       = $(ITL_LIBS_PATH)/test
ITL_XT_TEST_PATH    = $(ITL_XT_LIBS_PATH)/test

#BOOST_PATH = /usr/include/boost-1_33_1
#BOOST_PATH = /usr/include/boost_1_35_0
BOOST_PATH = $(ITL_ROOT_PATH)

#--- binaries -----------------------------------------------------------------
BIN_PATH = $(ITL_ROOT_PATH)/bin
OBJ_PATH = $(BIN_PATH)/obj

#--- include pathes -----------------------------------------------------------
ITL_INCL = $(addprefix -I,$(ITL_SRC_PATH))
BOOST_INCL = $(addprefix -I,$(BOOST_PATH))
STD_INCL = -I/usr/include
#GCC_INCL = $(addprefix -I,$(CMP_PATH))/include
#CONCEPTGCC_INCL = -I/opt/conceptgcc-boostcon/include
CONCEPTGCC_INCL = -I/opt/conceptgcc-4.3.0-alpha-7/include/c++/4.3.0

INCLUDE  = $(CONCEPTGCC_INCL) $(ITL_INCL) $(BOOST_INCL)
#NCLUDE  = $(STD_INCL) $(ITL_INCL) $(BOOST_INCL)

#--- libraries ----------------------------------------------------------------
#LOAD_LIBS  = -L$(CMP_PATH)/lib
LOAD_LIBS  = -L/usr/lib 
#--- LIBS for 64bit processors: 
#LOAD_LIBS  = -L/usr/lib64 

#--- loadflags ----------------------------------------------------------------
LOAD_FLAGS = $(LOAD_LIBS) -lstdc++
LOAD_FLAGS4BOOST = $(LOAD_FLAGS) -lboost_date_time-gcc-mt-s 

#--- compileflags ---------------------------------------------------
#COMPILE_FLAGS = -O2 -DUSE_CONCEPTS
COMPILE_FLAGS = -O2

#--- compiler -----------------------------------------------------------------
#--- location of the compiler -------------------------------------------------
#CMP_PATH = /opt/conceptgcc-boostcon
#COMPILE = $(CMP_PATH)/bin/gcc
COMPILE = /opt/conceptgcc-4.3.0-alpha-7/bin/gcc
#COMPILE = gcc


all:
	make examples
	make boost_examples
	make tests

# The concept_gcc compiler does not yet digest my loki::typelist based tuple type
# tag 'all_concept' covers the parts that can be compiled
all_concept:
	make examples
	make itvset_shell
	make splititvmap_shell
#	make auto_itv_test
	
examples:
	make party
	make interval
	make interval_container
	make overlap_counter
	make history
	make amount_cube
	
boost_examples:
	make boost_party
	make user_groups
	make month_and_week_grid
	make man_power

all_examples:
	examples
	boost_examples

tests:
	make itvset_shell
	make splititvmap_shell
	make auto_itv_test
# labatea is cureently not compiling with gcc. It only compiles with msvc 
#	make labatea
	
devels:
	make validate

#--- projects -----------------------------------------------------------------
#--- group examples -----------------------------------------------------------

#--- project party ---------------------------------------------------------
EXAMPLE_DIR = example
party_PATH = $(ITL_EXAMPLE_PATH)/party
party_MAIN = $(party_PATH)/party.cpp
party_SOURCES = $(party_MAIN)
party_TARGET  = $(BIN_PATH)/party$(EXE_TAG)

party:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(party_SOURCES) $(LOAD_FLAGS) -o $(party_TARGET)

#--- project interval ---------------------------------------------------------
interval_PATH = $(ITL_EXAMPLE_PATH)/interval
interval_MAIN = $(interval_PATH)/interval.cpp
interval_SOURCES = $(interval_MAIN)
interval_TARGET  = $(BIN_PATH)/interval$(EXE_TAG)

interval:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(interval_SOURCES) $(LOAD_FLAGS) -o $(interval_TARGET)

#--- project interval_container --------------------------------------------------
interval_container_PATH = $(ITL_EXAMPLE_PATH)/interval_container
interval_container_MAIN = $(interval_container_PATH)/interval_container.cpp
interval_container_SOURCES = $(interval_container_MAIN)
interval_container_TARGET  = $(BIN_PATH)/interval_container$(EXE_TAG)

interval_container:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(interval_container_SOURCES) $(LOAD_FLAGS) -o $(interval_container_TARGET)

#--- project overlap_counter --------------------------------------------------
overlap_counter_PATH = $(ITL_EXAMPLE_PATH)/overlap_counter
overlap_counter_MAIN = $(overlap_counter_PATH)/overlap_counter.cpp
overlap_counter_SOURCES = $(overlap_counter_MAIN)
overlap_counter_TARGET  = $(BIN_PATH)/overlap_counter$(EXE_TAG)

overlap_counter:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(overlap_counter_SOURCES) $(LOAD_FLAGS) -o $(overlap_counter_TARGET)

#--- project history --------------------------------------------------
history_PATH = $(ITL_XT_EXAMPLE_PATH)/history
history_MAIN = $(history_PATH)/history.cpp
history_SOURCES = $(history_MAIN)
history_TARGET  = $(BIN_PATH)/history$(EXE_TAG)

history:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(history_SOURCES) $(LOAD_FLAGS) -o $(history_TARGET)

#--- project amount_cube --------------------------------------------------
amount_cube_PATH = $(ITL_XT_EXAMPLE_PATH)/amount_cube
amount_cube_MAIN = $(amount_cube_PATH)/amount_cube.cpp
amount_cube_SOURCES = $(amount_cube_MAIN)
amount_cube_TARGET  = $(BIN_PATH)/amount_cube$(EXE_TAG)

amount_cube:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(amount_cube_SOURCES) $(LOAD_FLAGS) -o $(amount_cube_TARGET)

#--- project boost_party --------------------------------------------------
boost_party_PATH = $(ITL_EXAMPLE_PATH)/boost_party
boost_party_MAIN = $(boost_party_PATH)/boost_party.cpp
boost_party_SOURCES = $(boost_party_MAIN)
boost_party_TARGET  = $(BIN_PATH)/boost_party$(EXE_TAG)

boost_party:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(boost_party_SOURCES) $(LOAD_FLAGS4BOOST) -o $(boost_party_TARGET)

#--- project user_groups --------------------------------------------------
user_groups_PATH = $(ITL_EXAMPLE_PATH)/user_groups
user_groups_MAIN = $(user_groups_PATH)/user_groups.cpp
user_groups_SOURCES = $(user_groups_MAIN)
user_groups_TARGET  = $(BIN_PATH)/user_groups$(EXE_TAG)

user_groups:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(user_groups_SOURCES) $(LOAD_FLAGS4BOOST) -o $(user_groups_TARGET)

#--- project month_and_week_grid --------------------------------------------------
month_and_week_grid_PATH = $(ITL_EXAMPLE_PATH)/month_and_week_grid
month_and_week_grid_MAIN = $(month_and_week_grid_PATH)/month_and_week_grid.cpp
month_and_week_grid_SOURCES = $(month_and_week_grid_MAIN)
month_and_week_grid_TARGET  = $(BIN_PATH)/month_and_week_grid$(EXE_TAG)

month_and_week_grid:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(month_and_week_grid_SOURCES) $(LOAD_FLAGS4BOOST) -o $(month_and_week_grid_TARGET)

#--- project man_power --------------------------------------------------
man_power_PATH = $(ITL_EXAMPLE_PATH)/man_power
man_power_MAIN = $(man_power_PATH)/man_power.cpp
man_power_SOURCES = $(man_power_MAIN)
man_power_TARGET  = $(BIN_PATH)/man_power$(EXE_TAG)

man_power:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(man_power_SOURCES) $(LOAD_FLAGS4BOOST) -o $(man_power_TARGET)

	
#--- project itvset_shell -----------------------------------------------------
itvset_shell_PATH = $(ITL_EXAMPLE_PATH)/itvset_shell
itvset_shell_MAIN = $(itvset_shell_PATH)/itvset_shell.cpp
itvset_shell_SOURCES = $(itvset_shell_MAIN)
itvset_shell_TARGET  = $(BIN_PATH)/itvset_shell$(EXE_TAG)

itvset_shell:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(itvset_shell_SOURCES) $(LOAD_FLAGS) -o $(itvset_shell_TARGET)

#--- project splititvmap_shell ------------------------------------------------
splititvmap_shell_PATH = $(ITL_EXAMPLE_PATH)/splititvmap_shell
splititvmap_shell_MAIN = $(splititvmap_shell_PATH)/splititvmap_shell.cpp
splititvmap_shell_SOURCES = $(splititvmap_shell_MAIN)
splititvmap_shell_TARGET  = $(BIN_PATH)/splititvmap_shell$(EXE_TAG)

splititvmap_shell:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(splititvmap_shell_SOURCES) $(LOAD_FLAGS) -o $(splititvmap_shell_TARGET)

	
#--- group tests --------------------------------------------------------------
#--- project auto_itv_test -----------------------------------------------------
auto_itv_test_PATH = $(ITL_XT_TEST_PATH)/auto_itv_test
auto_itv_test_MAIN = $(auto_itv_test_PATH)/auto_itv_test.cpp
auto_itv_test_SOURCES = $(auto_itv_test_MAIN)
auto_itv_test_TARGET  = $(BIN_PATH)/auto_itv_test$(EXE_TAG)

auto_itv_test:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(auto_itv_test_SOURCES) $(LOAD_FLAGS) -o $(auto_itv_test_TARGET)

#--- project labatea -----------------------------------------------------
labatea_PATH = $(ITL_TEST_PATH)/labatea
labatea_MAIN = $(labatea_PATH)/labatea.cpp
labatea_SOURCES = $(ITL_SRC_PATH)/validate/gentor/gentorprofile.cpp $(labatea_MAIN)
labatea_TARGET  = $(BIN_PATH)/labatea$(EXE_TAG)

labatea:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(labatea_SOURCES) $(LOAD_FLAGS) -o $(labatea_TARGET)

	
#--- group devels --------------------------------------------------------------
#--- project itvset_shell -----------------------------------------------------
DEVEL_DIR = devel
validate_PATH = $(ITL_ROOT_PATH)/$(DEVEL_DIR)/validate
validate_MAIN = $(validate_PATH)/validate.cpp
validate_SOURCES = $(ITL_SRC_PATH)/validate/gentor/gentorprofile.cpp $(validate_MAIN)
validate_TARGET  = $(BIN_PATH)/validate$(EXE_TAG)

validate:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(validate_SOURCES) $(LOAD_FLAGS) -o $(validate_TARGET)

#--- project template_match ---------------------------------------------
template_match_PATH = $(ITL_ROOT_PATH)/$(DEVEL_DIR)/template_match
template_match_MAIN = $(template_match_PATH)/template_match.cpp
template_match_SOURCES = $(template_match_MAIN)
template_match_TARGET  = $(BIN_PATH)/template_match$(EXE_TAG)

template_match:
	$(COMPILE) $(COMPILE_FLAGS)  $(INCLUDE) $(template_match_SOURCES) $(LOAD_FLAGS) -o $(template_match_TARGET)


show_temps:
	find $(ITL_LIBS_PATH) -name \*.pdb; \
	find $(ITL_LIBS_PATH) -name \*.exe; \
	find $(ITL_LIBS_PATH) -name \*.obj; \
	find $(ITL_LIBS_PATH) -name \*.pdb; \
	find $(ITL_LIBS_PATH) -name \*.idb; \
	find $(ITL_LIBS_PATH) -name \*.ilk; \
	find $(ITL_LIBS_PATH) -name \*.dep; \
	find $(ITL_LIBS_PATH) -name \*.manifest*; \
	find $(ITL_LIBS_PATH) -name BuildLog.htm; \
	find $(ITL_LIBS_PATH)/doc/html -name *; \
	find $(ITL_LIBS_PATH) -name \*.ncb; \
	find $(ITL_LIBS_PATH) -name \*.suo; \
	find $(ITL_LIBS_PATH) -name \*.user; \
	find $(ITL_LIBS_PATH) -name stdafx.h; \
	
win_clean:
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.exe; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.obj; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.pdb; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.idb; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.ilk; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.dep; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.manifest; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin \*.manifest.*; \
	../script/rm_temps $(ITL_LIBS_PATH)/bin BuildLog.htm; \
	../script/rm_temps $(ITL_LIBS_PATH) \*.ncb; \
	../script/rm_temps $(ITL_LIBS_PATH) \*.suo; \
	../script/rm_temps $(ITL_LIBS_PATH) \*.user;
	
bin_clean:
	rm $(ITL_LIBS_PATH)/bin/* ;

dox_clean:
	../script/rm_temps $(ITL_LIBS_PATH)/doc/html \*.html; \
	../script/rm_temps $(ITL_LIBS_PATH)/doc/html \*.png; \
	../script/rm_temps $(ITL_LIBS_PATH)/doc/html \*.css; \
	../script/rm_temps $(ITL_LIBS_PATH)/doc/html \*.gif; \

clean:
	make win_clean 
	make dox_clean
	
svn_clean:
	../script/rm_svn $(ITL_ROOT_PATH)
	
	
	