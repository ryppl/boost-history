// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_TEMPLATE_DECL_HPP_INCLUDED
#define BOOST_IDL_DETAIL_TEMPLATE_DECL_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

//
// Macro: BOOST_IDL_TEMPLATE_DECL(arity, param)
// Parameters:
//     arity - the number of template parameters in the template declaration
//             to be constructed.
//     params - the prefix used to construct formal parameter names.
// Expands to: template< typename param0, ..., typename paramn > if arity is 
//     non-zero, and nothing otherwise.
//
#define BOOST_IDL_TEMPLATE_DECL(arity, param) \
    BOOST_PP_EXPR_IF(arity, template<) \
    BOOST_PP_ENUM_PARAMS(arity, typename param) \
    BOOST_PP_EXPR_IF(arity, >) \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_TEMPLATE_DECL_HPP_INCLUDED
