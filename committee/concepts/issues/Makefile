CXX=g++

html: 
	make concepts_issues
	./concepts_issues

install: html
	mv *.html /l/osl/www/www.generic-programming.org/languages/conceptcpp/issues

concepts_issues: main.cpp date.cpp date.h
	$(CXX) -o concepts_issues main.cpp date.cpp date.h

clean:
	rm -f *.html concepts_issues
