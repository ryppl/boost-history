/*=============================================================================
    Boost.Wave: A Standard compliant C++ preprocessor library
    http://www.boost.org/

    Copyright (c) 2001-2006 Hartmut Kaiser. Distributed under the Boost
    Software License, Version 1.0. (See accompanying file
    LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/

#if !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)
#define BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED

#include <cstdio>
#include <ostream>
#include <string>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/wave/token_ids.hpp>
#include <boost/wave/util/macro_helpers.hpp>
#include <boost/wave/preprocessing_hooks.hpp>

///////////////////////////////////////////////////////////////////////////////
//  
//  The advanced_preprocessing_hooks policy class is used to register some
//  of the more advanced (and probably more rarely used hooks with the Wave
//  library.
//
//  This policy type is used as a template parameter to the boost::wave::context<>
//  object.
//
///////////////////////////////////////////////////////////////////////////////
class advanced_preprocessing_hooks
:   public boost::wave::context_policies::default_preprocessing_hooks
{
public:
    advanced_preprocessing_hooks() : need_comment(true) {}
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'found_directive' is called, whenever a preprocessor 
    //  directive was encountered, but before the corresponding action is 
    //  executed.
    //
    //  The parameter 'directive' is a reference to the token holding the 
    //  preprocessing directive.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename TokenT>
    void
    found_directive(TokenT const& directive)
    {
        // print the commented conditional directives
        using namespace boost::wave;
        token_id id = token_id(directive);
        switch (id) {
        case T_PP_IFDEF:
        case T_PP_IFNDEF:
        case T_PP_IF:
        case T_PP_ELIF:
            std::cout << "// " << directive.get_value() << " ";
            break;
            
        case T_PP_ELSE:
        case T_PP_ENDIF:
            std::cout << "// " << directive.get_value() << std::endl;
            need_comment = true;
            break;

        default:
            break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'evaluated_conditional_expression' is called, whenever a 
    //  conditional preprocessing expression was evaluated (the expression
    //  given to a #if, #ifdef or #ifndef directive)
    //
    //  The parameter 'expression' holds the non-expanded token sequence
    //  comprising the evaluated expression.
    //
    //  The parameter 'expression_value' contains the result of the evaluation 
    //  of the expression in the current preprocessing context.
    //
    ///////////////////////////////////////////////////////////////////////////
    template <typename ContainerT>
    void
    evaluated_conditional_expression(ContainerT const& expression, 
        bool expression_value)
    {
        // print the conditional expressions
        std::cout << boost::wave::util::impl::as_string(expression) << std::endl;
        need_comment = true;
    }
    
    ///////////////////////////////////////////////////////////////////////////
    //
    //  The function 'skipped_token' is called, whenever a token is about to be
    //  skipped due to a false preprocessor condition (code fragments to be
    //  skipped inside the not evaluated conditional #if/#else/#endif branches).
    //
    //  The parameter 'token' refers to the token to be skipped.
    //  
    ///////////////////////////////////////////////////////////////////////////
    template <typename TokenT>
    void
    skipped_token(TokenT const& token)
    {
        // prepend a comment at the beginning of all skipped lines
        using namespace boost::wave;
        if (need_comment) {
            std::cout << "// ";
            need_comment = false;
        }
        std::cout << token.get_value();
        if (token_id(token) == T_NEWLINE || token_id(token) == T_CPPCOMMENT) 
            need_comment = true;
    }

private:
    bool need_comment;
};

#endif // !defined(BOOST_WAVE_ADVANCED_PREPROCESSING_HOOKS_INCLUDED)
