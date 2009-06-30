// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_ASSERT_HPP
#define BOOST_FUSION_SUPPORT_ASSERT_HPP

#include <boost/config.hpp>

#ifdef BOOST_NO_STATIC_ASSERT
#   include <boost/mpl/assert.hpp>
#   include <boost/static_assert.hpp>

#   define BOOST_FUSION_MPL_ASSERT(PRED,MESSAGE) BOOST_MPL_ASSERT(PRED)
#   define BOOST_FUSION_MPL_ASSERT_NOT(PRED,MESSAGE) BOOST_MPL_ASSERT_NOT(PRED)
#   define BOOST_FUSION_MPL_ASSERT_RELATION(X,REL,Y,MESSAGE)\
        BOOST_MPL_ASSERT_RELATION(X,REL,Y)
#   define BOOST_FUSION_STATIC_ASSERT(PRED,MESSAGE) BOOST_STATIC_ASSERT(PRED)
#else
#   define BOOST_FUSION_MPL_ASSERT(PRED,MESSAGE)\
        static_assert(PRED::value,MESSAGE)
#   define BOOST_FUSION_MPL_ASSERT_NOT(PRED,MESSAGE)\
        static_assert(!PRED::value,MESSAGE)
#   define BOOST_FUSION_MPL_ASSERT_RELATION(X,REL,Y,MESSAGE)\
        static_assert(X REL Y,MESSAGE)
#   define BOOST_FUSION_STATIC_ASSERT(PRED,MESSAGE)\
        static_assert(PRED,MESSAGE)
#endif

#define BOOST_FUSION_INDEX_CHECK(INDEX,MAX)\
    BOOST_FUSION_MPL_ASSERT_RELATION(INDEX, >=, 0, "index out of range")\
    BOOST_FUSION_MPL_ASSERT_RELATION(INDEX, <, MAX, "index out of range")

#endif
