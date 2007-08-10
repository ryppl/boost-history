
dot_src = \
	connected_components_before.dot \
	connected_components_after.dot \
	geodesic.dot \
	information_network.dot
circo_src = \
	distribution_undirected.circo \
	distribution_directed.circo
neato_src = \
	social_network.neato

dot_png = $(dot_src:%.dot=%.png)
circo_png = $(circo_src:%.circo=%.png)
neato_png = $(neato_src:%.neato=%.png)

%.png: %.dot
	dot -Tpng $< > $@
	convert $@ -resize 80% $@

%.png: %.circo
	circo -Tpng $< > $@
	convert $@ -resize 80% $@

%.png: %.neato
	neato -Tpng $< > $@
	convert $@ -resize 80% $@

all: $(dot_png) $(circo_png) $(neato_png)

clean:
	rm -rf $(dot_png)
	rm -rf $(circo_png)
