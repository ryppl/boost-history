
// Copyright (C) 2009-2011 Lorenzo Caminiti
// Use, modification, and distribution is subject to the
// Boost Software License, Version 1.0
// (see accompanying file LICENSE_1_0.txt or a copy at
// http://www.boost.org/LICENSE_1_0.txt).

#ifndef BOOST_DETAIL_PP_KEYWORD_PUBLIC_HPP_
#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_HPP_

#include "facility/is.hpp"
#include "facility/add.hpp"
#include "facility/remove.hpp"

// These are not local macros -- DO NOT #UNDEF.
#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_IS_public (1) /* unary */
#define public_BOOST_DETAIL_PP_KEYWORD_PUBLIC_IS (1) /* unary */
#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE_public /* nothing */
#define public_BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE /* nothing */

// Is.

#define BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_IS_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_PUBLIC_IS_)

#define BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_BACK(token) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_IS_BACK(token, \
            _BOOST_DETAIL_PP_KEYWORD_PUBLIC_IS)

// Rremove.

#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_REMOVE_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_FRONT, \
            BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE_)

#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE_BACK(tokens) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_REMOVE_BACK(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_BACK, \
            _BOOST_DETAIL_PP_KEYWORD_PUBLIC_REMOVE)

// Add.

#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_ADD_FRONT(tokens) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_ADD_FRONT(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_FRONT, public)

#define BOOST_DETAIL_PP_KEYWORD_PUBLIC_ADD_BACK(tokens) \
    BOOST_DETAIL_PP_KEYWORD_FACILITY_ADD_BACK(tokens, \
            BOOST_DETAIL_PP_KEYWORD_IS_PUBLIC_BACK, public)

#endif // #include guard

