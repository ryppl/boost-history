#ifndef INCLUDE_OUTPUT_H
#define INCLUDE_OUTPUT_H

#include "../parser/lexpolicies.h"

#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <utility>
#include <iterator>
#include <algorithm>
#include <stack>
#include <boost/shared_ptr.hpp>

#include "../map/map.h"

//
// Two functions:
// 1. Keeps the output streams open for the generated text
// 2. Keeps track of where each source segment goes.
// pushing everything into a pimpl to keep the change sensitivity on generate.cpp small.
class OutputDelegate {
	struct Context;

	Context * m_impl;
	void check ();
	
	
	// not implemented.
	OutputDelegate ();
	OutputDelegate (const OutputDelegate& other);
public:
	OutputDelegate (std::ostream& h, std::ostream& s, MapManager& m);

	~OutputDelegate ();

	//
	// These interpret the token ranges given to them
	void module_export (const context_iter_t& start, const context_iter_t& end);
	void module_import (const context_iter_t& start, const context_iter_t& end);
	void go_public ();
	void go_private ();

	// Note: For export namespace {} statements, don't tell OutputDelegate to emit
	// the braces.  We'll take care of that here.  It needs a bit of special handling.
/*	
	void import_module (std::string & name);
	void begin_module (std::string & name);
	void end_module ();
	
	void go_public ();
	void go_private ();

	/// we'll do the module name lookup.
	void include_module (std::string module); 

	void out (context_iter_t start, context_iter_t end);
	void out (std::string text);
	void out (token_t single);
	
	/// emit () causes the files to be written.
	void emit ();
*/

	void out (token_t single);
	void emit ();
};


#endif
