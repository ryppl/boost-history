#
#  Makefile for GNU make >= 3.77
#


# compiler settings

GCC = g++
LD  = g++
ASTYLE = astyle
INDENT = indent

DRIVE_E = /cygdrive/e

BINDIR = $(DRIVE_E)/roland/bin
BOOSTDIR = ../../../boost
DETAILDIR = $(BOOSTDIR)/tupple/detail

OPTS  = -w -O2 -g0
PREFLAGS = -E -C -P
FLAGS =
INCLUDE = -I$(DRIVE_E)/lib/boost_1_31_0 -I.
LINK  =

ASTYLE_OPTS = -s2 -b -N -P
INDENT_OPTS = --no-tabs -bl -bli0 -nce -npcs -nsaf -nsai -nsaw -prs -hnl

all: tupple.hpp iterator_tupple.hpp container_tupple.hpp

tupple.hpp:
	@echo " "
	-$(GCC) $(PREFLAGS) generate_tupple_hpp.cpp -o tupple.ii -DTUPPLE_PARTIAL_SPEC $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) tupple.ii -o $(DETAILDIR)/tupple_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/tupple_part_spec.hpp
	@echo " "
	-$(GCC) $(PREFLAGS) generate_tupple_hpp.cpp -o tupple_no_part_spec.ii $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) tupple_no_part_spec.ii -o $(DETAILDIR)/tupple_no_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/tupple_no_part_spec.hpp
	rm *.ii
	rm $(DETAILDIR)/*.orig


iterator_tupple.hpp:
	@echo " "
	-$(GCC) $(PREFLAGS) generate_iterator_tupple_hpp.cpp -o iterator_tupple.ii -DTUPPLE_PARTIAL_SPEC $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) iterator_tupple.ii -o $(DETAILDIR)/iterator_tupple_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/iterator_tupple_part_spec.hpp
	@echo " "
	-$(GCC) $(PREFLAGS) generate_iterator_tupple_hpp.cpp -o iterator_tupple.ii $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) iterator_tupple.ii -o $(DETAILDIR)/iterator_tupple_no_part_spec.ii
# indent does occasionally format template types "x< y<z> >" as "x< y<z >>"; thus, replace >> by > >:
	sed 's/>>/> >/' $(DETAILDIR)/iterator_tupple_no_part_spec.ii > $(DETAILDIR)/iterator_tupple_no_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/iterator_tupple_no_part_spec.hpp
	rm $(DETAILDIR)/iterator_tupple_no_part_spec.ii
	rm *.ii
	rm $(DETAILDIR)/*.orig

container_tupple.hpp:
	@echo " "
	-$(GCC) $(PREFLAGS) generate_container_tupple_hpp.cpp -o container_tupple.ii -DTUPPLE_PARTIAL_SPEC $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) container_tupple.ii -o $(DETAILDIR)/container_tupple_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/container_tupple_part_spec.hpp
	@echo " "
	-$(GCC) $(PREFLAGS) generate_container_tupple_hpp.cpp -o container_tupple.ii $(OPTS) $(INCLUDE)
	$(INDENT) $(INDENT_OPTS) container_tupple.ii -o $(DETAILDIR)/container_tupple_no_part_spec.hpp
	$(ASTYLE) $(ASTYLE_OPTS) $(DETAILDIR)/container_tupple_no_part_spec.hpp
	rm *.ii
	rm $(DETAILDIR)/*.orig

# Make object in BINDIR:
$(BINDIR)/%.o: %.cpp
	@echo " "
	@-$(GCC) -MM $< $(INCLUDE)
	-$(GCC) -c $< -o $@ $(OPTS) $(FLAGS) $(INCLUDE)

# Link exe in BINDIR:
%.exe: $(BINDIR)/%.o
	@echo " "
	-$(GCC) $< -o $(BINDIR)/$* $(OPTS) $(LINK)
