
src = \
	total_geodesic.tex \
	mean_geodesic.tex \
	mean_geodesic_undirected.tex \
	mean_geodesic_directed.tex \
	inverse_geodesic.tex \
	closeness.tex \
	norm_closeness.tex
dvi = $(src:%.tex=%.dvi)
ps = $(src:%.tex=%.ps)
png = $(src:%.tex=%.png)
aux = $(src:%.tex=%.aux)
log = $(src:%.tex=%.log)

%.png: %.ps
	convert -density 120 $< $@

%.ps: %.dvi
	dvips -E $< -o $@

%.dvi: %.tex
	latex --interaction=nonstopmode $<

all: $(png)

clean:
	rm -f $(png) $(ps) $(dvi) $(aux) $(log)