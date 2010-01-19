//  static_assert.hpp  --------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#ifndef BOOST_CHRONO_DETAIL_STATIC_ASSERT__HPP
#define BOOST_CHRONO_DETAIL_STATIC_ASSERT__HPP

#include <boost/chrono/config.hpp>

#ifndef BOOST_NO_STATIC_ASSERT
#define BOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(BOOST_CHRONO_USES_STATIC_ASSERT)
#include <boost/static_assert.hpp>
#define BOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) BOOST_STATIC_ASSERT(CND)
#elif defined(BOOST_CHRONO_USES_MPL_ASSERT)
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#define BOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    BOOST_MPL_ASSERT_MSG(boost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#elif defined(BOOST_CHRONO_USES_ARRAY_ASSERT)
#define BOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES) static char BOOST_JOIN(__boost_chrono_test_,__LINE__)[CND];
#else
#define BOOST_CHRONO_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#endif // BOOST_CHRONO_DETAIL_STATIC_ASSERT__HPP
