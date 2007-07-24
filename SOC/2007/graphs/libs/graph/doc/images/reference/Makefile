
dot_src = \
	connected_components_before.dot \
	connected_components_after.dot \
	geodesic.dot
circo_src = \
	distribution_undirected.circo \
	distribution_directed.circo

dot_png = $(dot_src:%.dot=%.png)
circo_png = $(circo_src:%.circo=%.png)

%.png: %.dot
	dot -Tpng $< > $@
	convert $@ -resize 80% $@

%.png: %.circo
	circo -Tpng $< > $@
	convert $@ -resize 80% $@

all: $(dot_png) $(circo_png)

clean:
	rm -rf $(dot_png)
	rm -rf $(circo_png)
