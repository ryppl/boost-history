//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include "../errored_clock.hpp"
#include "../check_clock_now.hpp"
#include <boost/detail/lightweight_test.hpp>

int main()
{
	check_clock_now_err<errored_clock>(1);
	check_clock_now_ec_err<errored_clock>(1);

    return boost::report_errors();
}
