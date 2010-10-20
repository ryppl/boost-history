//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

// monotonic_clock

// static time_point now();

#include <boost/chrono.hpp>
#include "../check_clock_now.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
{
#ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
	check_clock_now<boost::chrono::monotonic_clock>();
	check_clock_now_ec<boost::chrono::monotonic_clock>();
    check_clock_now_throws<boost::chrono::monotonic_clock>();
	
#endif
    return boost::report_errors();
}

