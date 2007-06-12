/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONFIG_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_DEFAULT_MAX_SIZE_HPP
#define BOOST_ACT_CONFIG_QUEUE_POLICY_BOUNDED_QUEUE_POLICY_DEFAULT_MAX_SIZE_HPP

#ifndef BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE

#include <boost/preprocessor/tuple/elem.hpp>

#ifndef BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE_INFO
#define BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE_INFO                   \
        ( <boost/act/detail/empty_header.hpp>, 8 )
#endif

#include BOOST_PP_TUPLE_ELEM( 2, 0, BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE_INFO )

#define BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE()                      \
BOOST_PP_TUPLE_ELEM( 2, 1                                                      \
                   , BOOST_ACT_DEFAULT_BOUNDED_QUEUE_POLICY_MAX_SIZE_INFO      \
                   )

#endif

#endif
