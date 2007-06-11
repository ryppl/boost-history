#ifndef INCLUDE_DRIVER_H
#define INCLUDE_DRIVER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

///////////////////////////////////////////////////////////////////////////////
//  Include Wave itself
#include <boost/wave.hpp>

///////////////////////////////////////////////////////////////////////////////
// Include the lexer stuff
#include <boost/wave/cpplexer/cpp_lex_token.hpp>    // token class
#include <boost/wave/cpplexer/cpp_lex_iterator.hpp> // lexer class


class Driver {
    // copy-pasted from wave/examples/quick_start.cpp
    typedef boost::wave::cpplexer::lex_token<> token_t;
    typedef boost::wave::cpplexer::lex_iterator<token_t> lex_iter_t;
    typedef boost::wave::context<std::string::iterator, lex_iter_t>
       context_t;
    
    // (temporary) local storehouse for all the .mapfiles.
    
public:
    static int execute (int args, const char ** argv);
};



#endif
