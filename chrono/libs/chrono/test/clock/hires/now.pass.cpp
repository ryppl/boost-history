//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include "../check_clock_now.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
{
	check_clock_now<boost::chrono::high_resolution_clock>();
	check_clock_now_ec<boost::chrono::high_resolution_clock>();
    return boost::report_errors();
}
