/*=============================================================================
    Wave: A Standard compliant C++ preprocessor library

    Sample: IDL oriented preprocessor
            Explicit instantiation of the predefined_macros_grammar template

    Copyright (c) 2001-2004 Hartmut Kaiser
    http://spirit.sourceforge.net/

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#include "idl.hpp"

#if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

#include <string>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/cpplexer/cpp_lex_token.hpp>

#include "idllexer/idl_lex_iterator.hpp"

#include <boost/wave/grammars/cpp_predef_macros_grammar.hpp>

///////////////////////////////////////////////////////////////////////////////
//  
//  Explicit instantiation of the predefined_macros_grammar_gen template 
//  with the correct token type. This instantiates the corresponding pt_parse 
//  function, which in turn instantiates the cpp_predefined_macros_grammar 
//  object (see wave/grammars/cpp_predef_macros_grammar.hpp)
//
///////////////////////////////////////////////////////////////////////////////

typedef boost::wave::idllexer::lex_iterator<
        boost::wave::cpplexer::lex_token<> >
    lex_t;
template struct boost::wave::grammars::predefined_macros_grammar_gen<lex_t>;

#endif // #if BOOST_WAVE_SEPARATE_GRAMMAR_INSTANTIATION != 0

