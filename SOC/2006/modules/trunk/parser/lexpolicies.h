#ifndef INCLUDE_LEXPOLICIES_H
#define INCLUDE_LEXPOLICIES_H

#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class


// copy-pasted from wave/examples/quick_start.cpp
typedef boost::wave::cpplexer::lex_token<> token_t;
typedef boost::wave::cpplexer::lex_iterator<token_t> lex_iter_t;
typedef boost::wave::context<std::string::iterator, lex_iter_t>  context_t;
typedef context_t::iterator_type context_iter_t;

#endif
