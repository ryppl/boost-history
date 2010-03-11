noexcept.html: noexcept.rst
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet noexcept.rst noexcept.html

all: exported-concept-maps.html taxonomy-of-concepts-and-maps.html explicit-refinement.html

exported-concept-maps.html: exported-concept-maps.rst exported-concept-maps.css
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet exported-concept-maps.rst exported-concept-maps.html

taxonomy-of-concepts-and-maps.html: taxonomy-of-concepts-and-maps.rst exported-concept-maps.css
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet taxonomy-of-concepts-and-maps.rst taxonomy-of-concepts-and-maps.html

explicit-refinement.html: explicit-refinement.rst exported-concept-maps.css
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet explicit-refinement.rst explicit-refinement.html