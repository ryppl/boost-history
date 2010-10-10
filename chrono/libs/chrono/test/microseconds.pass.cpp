//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/type_traits.hpp>
#include <limits>

#if !defined(BOOST_NO_STATIC_ASSERT) 
#define NOTHING ""
#endif

void test()
{
    typedef boost::chrono::microseconds D;
    typedef D::rep Rep;
    typedef D::period Period;
    BOOST_CHRONO_STATIC_ASSERT(boost::is_signed<Rep>::value, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(boost::is_integral<Rep>::value, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(std::numeric_limits<Rep>::digits >= 54, NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<Period, boost::micro>::value), NOTHING, ());
}
