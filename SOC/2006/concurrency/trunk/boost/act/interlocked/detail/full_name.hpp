/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME_HPP
#define BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME_HPP

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/control/if.hpp>

#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_full_fence 0
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_acquire    1
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_release    2
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_raw        3
#define BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_dummy      0

#define BOOST_ACT_INTERLOCKED_DETAIL_FULL_NAME( name, semantics )              \
BOOST_PP_IF( BOOST_PP_CAT( BOOST_ACT_INTERLOCKED_DETAIL_AFFIX_                 \
                         , semantics                                           \
                         )                                                     \
           , BOOST_PP_CAT                                                      \
             (                                                                 \
               name                                                            \
             , BOOST_PP_CAT( _                                                 \
                           , semantics                                         \
                           )                                                   \
             )                                                                 \
           , name                                                              \
           )

#endif
