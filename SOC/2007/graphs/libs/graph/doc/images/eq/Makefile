
src = \
	mean_geodesic.tex \
	graph_mean_geodesic.tex \
	closeness.tex \
	norm_closeness.tex \
	betweenness.tex \
	relative_betweenness.tex \
	central_point_dominance.tex
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