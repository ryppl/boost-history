// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_PARAM_TYPE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_PARAM_TYPE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/interfaces/detail/config/has_named_params.hpp>
#include <boost/preprocessor/control/iif.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

//
// Macro: BOOST_IDL_PARAM_TYPE(arg)
// Parameters:
//     arg - a tuple (type, name) if HAS_NAMED_PARAMS() is true,
//           and a typename type otherwise.
// Expands to: type.
//
# define BOOST_IDL_PARAM_TYPE(arg) \
    BOOST_PP_IIF( BOOST_IDL_HAS_NAMED_PARAMS(), \
                  BOOST_PP_TUPLE_ELEM(2, 0, arg), arg ) \
    /**/

#endif // #ifndef BOOST_IDL_DETAIL_PARAM_TYPE_HPP_INCLUDED
