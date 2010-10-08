//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#ifndef BOOST_CHRONO_CHECK_CLOCK_NOW_HPP
#define BOOST_CHRONO_CHECK_CLOCK_NOW_HPP

#include <boost/chrono.hpp>

template <typename Clock>
void check_clock_now()
{
	typename Clock::time_point t1 = Clock::now();
}

#endif
