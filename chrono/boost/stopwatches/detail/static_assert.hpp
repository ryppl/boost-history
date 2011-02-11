//  boost/stopwatches/detail/static_assert.hpp  --------------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_STOPWATCHES_DETAIL_STATIC_ASSERT_HPP
#define BOOST_STOPWATCHES_DETAIL_STATIC_ASSERT_HPP

#include <boost/stopwatches/config.hpp>

#ifndef BOOST_NO_STATIC_ASSERT
#define BOOST_STOPWATCHES_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(BOOST_STOPWATCHES_USES_STATIC_ASSERT)
#include <boost/static_assert.hpp>
#define BOOST_STOPWATCHES_STATIC_ASSERT(CND, MSG, TYPES) BOOST_STATIC_ASSERT(CND)
#elif defined(BOOST_STOPWATCHES_USES_MPL_ASSERT)
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#define BOOST_STOPWATCHES_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    BOOST_MPL_ASSERT_MSG(boost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#elif defined(BOOST_STOPWATCHES_USES_ARRAY_ASSERT)
#define BOOST_STOPWATCHES_STATIC_ASSERT(CND, MSG, TYPES) static char BOOST_JOIN(boost_chrono_test_,__LINE__)[CND]
#else
#define BOOST_STOPWATCHES_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#endif // BOOST_STOPWATCHES_DETAIL_STATIC_ASSERT_HPP
