
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_HPP_

#include "parsed_params_/index.hpp"
#include <boost/preprocessor/tuple/elem.hpp>
#include <boost/preprocessor/list/adt.hpp> // For `IS_CONS`.

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_MAX, \
            BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_CONST_BIND, \
            params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND(params) \
    BOOST_PP_LIST_IS_CONS(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params))

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params) \
    BOOST_PP_TUPLE_ELEM(BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_MAX, \
            BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INDEX_CONST_BIND_THIS_COUNT, \
            params)

#define BOOST_LOCAL_AUX_PP_SIGN_PARAMS_HAVE_CONST_BIND_THIS(params) \
    BOOST_PP_GREATER(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT( \
            params), 0)

#endif // #include guard

