#ifndef INCLUDE_GENERATOR_H
#define INCLUDE_GENERATOR_H

#include "../driver/output.h"
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class


// copy-pasted from wave/examples/quick_start.cpp
typedef boost::wave::cpplexer::lex_token<> token_t;
typedef boost::wave::cpplexer::lex_iterator<token_t> lex_iter_t;
typedef boost::wave::context<std::string::iterator, lex_iter_t>  context_t;


class Generator {
	context_t& c;
	OutputDelegate& d;

public:
	Generator (context_t& ctx, OutputDelegate& del) : c(ctx), d(del) {}

	// returns a list of the modules defined (in part or whole) int
	// this file.
	std::vector<std::string>  execute ();
};

#endif
