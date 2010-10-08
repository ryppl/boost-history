//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_CHRONO_CHECK_CLOCK_INVARIANTS_HPP
#define BOOST_CHRONO_CHECK_CLOCK_INVARIANTS_HPP

#include <boost/type_traits/is_same.hpp>

#if !defined(BOOST_NO_STATIC_ASSERT)
#define NOTHING ""
#endif

template <typename Clock>
void check_clock_invariants()
{
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<typename Clock::rep, typename Clock::duration::rep>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<typename Clock::period, typename Clock::duration::period>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT((boost::is_same<typename Clock::duration, typename Clock::time_point::duration>::value), NOTHING, ());
    BOOST_CHRONO_STATIC_ASSERT(Clock::is_monotonic || !Clock::is_monotonic, NOTHING, ());
    // to be replaced by has static member bool is_monotonic
}

#endif
