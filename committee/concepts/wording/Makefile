# -*- makefile -*-

LATEX	= latex
PDFLATEX = pdflatex


DVIPS		= dvips

.SUFFIXES: .tex .dvi .ps .pdf .dot .eps


.tex.dvi:
	@ if test ! -f $*.ind; then echo "" > $*.ind; fi
	@ $(LATEX) $*
	@ if ( grep 'Writing index file' $*.log > /dev/null ); \
	then makeindex $* ; $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Citation' $*.log > /dev/null ); \
	then bibtex $* ; $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi
	@ if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(LATEX) $* ; fi

.dot.eps:
	dot -Tps $*.dot > $*.eps

# .dot.eps:
# 	dot -Tps $*.dot | sed 's/^endpage/%endpage/' > $*.tmp.ps
# 	ps2epsi $*.tmp.ps $*.eps
# 	/bin/rm $*.tmp.ps

.dvi.ps:
	$(DVIPS) -t letter -P cmz -o $*.ps $*

.eps.pdf:
	epstopdf $*.eps

.tex.pdf:
	@$(PDFLATEX) $*
	@if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null ); \
	then $(PDFLATEX) $* ; else :; fi
	@ if ( grep 'Writing index file' $*.log > /dev/null ); \
	then makeindex $* ; $(PDFLATEX) $* ; fi
	@-if ( grep 'LaTeX Warning: Citation' $*.log > /dev/null ); then \
	  bibtex $* ; \
	  $(PDFLATEX) $* ; \
	fi
	@if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' $*.log > /dev/null); \
	then $(PDFLATEX) $* ; else :; fi
	@if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' $*.log > /dev/null); \
	then $(PDFLATEX) $* ; else :; fi
	@if ( grep 'LaTeX Warning: Label(s) may' $*.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' $*.log > /dev/null); \
	then $(PDFLATEX) $* ; else :; fi


#
# Default rule
#

NAME		= wording

OTHER_TEX	=

PDF_FIGS        =

default: $(NAME).pdf


DOT	= 

DPS	= $(DOT:.dot=.ps)
DEPS	= $(DOT:.dot=.eps)
DPDF	= $(DOT:.dot=.pdf)

#
# LaTeX stuff
#
$(NAME).dvi: $(NAME).tex $(OTHER_TEX)
$(NAME).ps:  $(NAME).dvi
$(NAME).pdf: $(NAME).tex $(OTHER_TEX) $(DPDF) $(PDF_FIGS)

#
# Standard rules
#
clean:
	/bin/rm -f *.dvi *.log *.blg *.bbl *.ind *.aux *.toc *lof *.lot *-speedup.txt \
	$(NAME).ps $(NAME).pdf *.out $(NAME).pdfsync

squeaky: clean

