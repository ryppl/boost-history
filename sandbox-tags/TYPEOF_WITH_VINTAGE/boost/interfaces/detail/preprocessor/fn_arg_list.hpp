// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_FUNCTION_ARGS_HPP_INCLUDED
#define BOOST_IDL_DETAIL_FUNCTION_ARGS_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/config.hpp> // HAS_NAMED_PARAMS
#include <boost/preprocessor/array/size.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>

//
// Macro: BOOST_IDL_ARG_LIST(args)
// Parameters:
//     args - an array (arg0, ...., argn) of tuples consisting of 
//            tuples (type, name) if HAS_NAMED_PARAMS() is true and
//            typenames otherwise.
// Expands to: xxx_0, ..., xxx_n
//
#define BOOST_IDL_FN_ARG_LIST(args) \
    BOOST_PP_ENUM_PARAMS(BOOST_PP_ARRAY_SIZE(args), xxx_) \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_FUNCTION_ARGS_HPP_INCLUDED
