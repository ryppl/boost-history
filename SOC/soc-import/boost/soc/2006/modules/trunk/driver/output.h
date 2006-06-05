#ifndef INCLUDE_OUTPUT_H
#define INCLUDE_OUTPUT_H

#include <iterator>
#include <algorithm>

//
// Once the driver's selected where to emit the files,
// this delegate wraps the output streams for easy use.
class OutputDelegate {
	std::ostream&  header;
	std::ostream&  source;
public:
	OutputDelegate (std::ostream& h, std::ostream& s)
	 : header(h), source(s) {}

	// we'll see how useful these interfaces are for
	// the parser.
	template<class Iter>
	void header_emit (Iter start, Iter end) {
		std::copy(start, end, std::ostream_iterator<char>(header));
	}
	
	template<class Iter>
	void source_emit (Iter start, Iter end) {
		std::copy(start, end, std::ostream_iterator<char>(source));
	}
};


#endif
