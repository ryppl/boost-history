//  Copyright 2010 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#include <boost/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include "../check_clock_invariants.hpp"

int main()
{
    check_clock_invariants<boost::chrono::process_real_cpu_clock>();
    check_clock_invariants<boost::chrono::process_user_cpu_clock>();
    check_clock_invariants<boost::chrono::process_system_cpu_clock>();
    check_clock_invariants<boost::chrono::process_cpu_clock>();
    return 0;    
}
