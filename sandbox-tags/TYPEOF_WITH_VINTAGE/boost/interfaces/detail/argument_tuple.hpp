// (C) Copyright Jonathan Turkanis 2004.
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Disclaimer: Not a Boost library.

#ifndef BOOST_IDL_DETAIL_ARGUMENT_TUPLE_HPP_INCLUDED
#define BOOST_IDL_DETAIL_ARGUMENT_TUPLE_HPP_INCLUDED

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/iteration/local.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/type_traits/add_reference.hpp>

#define BOOST_IDL_MAX_TUPLE_SIZE 10

#define BOOST_IDL_PARAM_REFERENCE(z, n, text) \
    BOOST_PP_COMMA_IF(n) typename add_reference<BOOST_PP_CAT(Arg, n)>::type \
    /**/
#define BOOST_IDL_TUPLE_PARAMS(n)  \
    BOOST_PP_REPEAT(n, BOOST_IDL_PARAM_REFERENCE, _) \
    /**/

namespace boost { namespace interfaces {

template<typename T>
struct argument_tuple;

#define BOOST_PP_LOCAL_MACRO(n) \
    template< typename Result BOOST_PP_COMMA_IF(n) \
              BOOST_PP_ENUM_PARAMS(n, typename Arg)> \
    struct argument_tuple<Result(BOOST_PP_ENUM_PARAMS(n, Arg))> { \
        typedef tuple<BOOST_IDL_TUPLE_PARAMS(n)> type; \
    }; \
    /**/
#define BOOST_PP_LOCAL_LIMITS (0, BOOST_IDL_MAX_TUPLE_SIZE - 1)
#include BOOST_PP_LOCAL_ITERATE()

} } // End namespaces interfaces, boost.

#endif // #ifndef BOOST_IDL_DETAIL_ARGUMENT_TUPLE_HPP_INCLUDED
