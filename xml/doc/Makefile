SYNOPSIS:= python synopsis.py
srcdir:= ..

vpath %.hpp $(srcdir)/boost/xml

doc: html

html: bxl.syn bxl.xref
	$(SYNOPSIS) html --output=$@ $<

bxl.syn: dom.syn reader.syn
	$(SYNOPSIS) link --output=$@ $^

bxl.xref: bxl.syn
	$(SYNOPSIS) xref --output=$@ $^

%.syn: %.hpp
	$(SYNOPSIS) parse --output=$@ \
          --base_path=$(srcdir) \
          cppflags="['-I$(srcdir)', '-I/usr/include/libxml2']" \
          $<

clean:
	rm -rf *.syn *.xref xref links html
