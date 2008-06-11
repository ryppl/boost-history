# -*- makefile -*-

PDFLATEX = pdflatex

basic: wording.pdf

wording.pdf: wording.tex macros.tex
	@$(PDFLATEX) wording.tex

final:
	@$(PDFLATEX) wording.tex
	@if ( grep 'LaTeX Warning: Label(s) may' wording.log > /dev/null ); \
	then $(PDFLATEX) wording.tex ; else :; fi
	@ if ( grep 'Writing index file' wording.log > /dev/null ); \
	then makeindex wording.tex ; $(PDFLATEX) wording.tex ; fi
	@-if ( grep 'LaTeX Warning: Citation' wording.log > /dev/null ); then \
	  bibtex wording ; \
	  $(PDFLATEX) wording.tex ; \
	fi
	@if ( grep 'LaTeX Warning: Label(s) may' wording.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' wording.log > /dev/null); \
	then $(PDFLATEX) wording.tex ; else :; fi
	@if ( grep 'LaTeX Warning: Label(s) may' wording.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' wording.log > /dev/null); \
	then $(PDFLATEX) wording.tex ; else :; fi
	@if ( grep 'LaTeX Warning: Label(s) may' wording.log > /dev/null || \
	      grep 'LaTeX Warning: Citation' wording.log > /dev/null); \
	then $(PDFLATEX) wording.tex ; else :; fi

#
# Standard rules
#
clean:
	/bin/rm -f *.dvi *.log *.blg *.bbl *.ind *.aux *.toc *lof *.lot *-speedup.txt \
	$(NAME).ps $(NAME).pdf *.out $(NAME).pdfsync

squeaky: clean

