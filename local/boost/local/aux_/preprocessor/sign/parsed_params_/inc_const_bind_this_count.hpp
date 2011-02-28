
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0 (see accompanying file LICENSE_1_0.txt or a
// copy at http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INC_CONST_BIND_THIS_COUNT_HPP_
#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INC_CONST_BIND_THIS_COUNT_HPP_

#include "../params_unbind.hpp"
#include "../params_bind.hpp"
#include "../params_const_bind.hpp"
#include <boost/preprocessor/arithmetic/inc.hpp>

#define BOOST_LOCAL_AUX_PP_SIGN_PARSED_PARAMS_INC_CONST_BIND_THIS_COUNT( \
        params) \
    ( /* unbind params and defaults */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_UNBIND(params) \
    , /* const-bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND(params) \
    , /* const-bind `this` count */ \
        BOOST_PP_INC(BOOST_LOCAL_AUX_PP_SIGN_PARAMS_CONST_BIND_THIS_COUNT( \
                params)) /* increment */ \
    , /* bind names */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND(params) \
    , /* bind `this` count */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_BIND_THIS_COUNT(params) \
    , /* error message (if any) */ \
        BOOST_LOCAL_AUX_PP_SIGN_PARAMS_ERROR(params) \
    )

#endif // #include guard

