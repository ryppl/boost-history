
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_BIND_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_BIND_HPP_

#include "../params_unbind.hpp"
#include "../params_bind.hpp"
#include "../params_const_bind.hpp"
#include <boost/preprocessor/list/append.hpp>

// bind_qualified_name: [&] name
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_APPEND_BIND( \
        params, bind_qualified_name) \
    ( /* unbind params and defaults */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(params) \
    , /* const-bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params) \
    , /* const-bind `this` count */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT(params) \
    , /* bind names */ \
        BOOST_PP_LIST_APPEND(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND(params), \
                (bind_qualified_name, BOOST_PP_NIL)) \
    , /* bind `this` count */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params) \
    , /* error message (if any) */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
    )

#endif // #include guard

