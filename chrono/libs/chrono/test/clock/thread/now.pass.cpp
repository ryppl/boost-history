//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt


#include "../check_clock_now.hpp"
#include <boost/chrono/thread_clock.hpp>
#include <boost/chrono.hpp>
#include <boost/detail/lightweight_test.hpp>

int main()
{
#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK)
	check_clock_now<boost::chrono::thread_clock>();
    check_clock_now_ec<boost::chrono::thread_clock>();
    check_clock_now_throws<boost::chrono::thread_clock>();
#endif
    return boost::report_errors();
}
