MAIN=bindo
#MAIN=gcc_bug.nullary_variadic_functor
YES_NOT=yes
#Purpose:
#  compile/run $(MAIN).cpp with one_of:
#    1) variadic bind, when YES_NOT=yes
#    2) phoenix3 bind, when YES_NOT=not

DIR.root := $(shell dirup_dir_file.pl $(PWD) root.imk)

HOW.base=gcc4_6n
BINDV.yes=-I$(vartmpl.root) $(COMPILER.gcc.opts.variadic) -DUSE_VARIADIC_BIND
BINDV.not=-I$(phoenix3.root) $(COMPILER.gcc.opts.all) -std=gnu++0x
COMPILE.$(HOW.base).yes=$(COMPILER.$(HOW.base)) -c $(BINDV.yes) $(INCS)
COMPILE.$(HOW.base).not=$(COMPILER.$(HOW.base)) -c $(BINDV.not) $(INCS) 

LINK.$(HOW.base).yes=$(LINK.$(HOW.base))
LINK.$(HOW.base).not=$(LINK.$(HOW.base))

HOW=$(HOW.base).$(YES_NOT)

include $(DIR.root)/root.imk

all:
	for YES_NOT in yes not ; do \
	  make -W$(MAIN).cpp YES_NOT=$$YES_NOT cmp_run ; \
	done ; \
	#

cmp_run:
	$(COMPILE.$(HOW)) $(MAIN).cpp -o $(MAIN).$(YES_NOT).o
	$(LINK.$(HOW)) $(MAIN).$(YES_NOT).o -o $(MAIN).$(YES_NOT).x
	./$(MAIN).$(YES_NOT).x

show:
	@echo "COMPILE.HOW.base.yes=" $(COMPILE.$(HOW.base).yes)
	@echo "COMPILE.HOW.base.not=" $(COMPILE.$(HOW.base).not)
