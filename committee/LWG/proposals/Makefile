all: exported-concept-maps.html taxonomy-of-concepts-and-maps.html

exported-concept-maps.html: exported-concept-maps.rst exported-concept-maps.css
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet exported-concept-maps.rst exported-concept-maps.html

taxonomy-of-concepts-and-maps.html: taxonomy-of-concepts-and-maps.rst exported-concept-maps.css
	rst2html.py --stylesheet=exported-concept-maps.css --embed-stylesheet taxonomy-of-concepts-and-maps.rst taxonomy-of-concepts-and-maps.html