//
//  collector_sp_counted_tagged_test.cpp
//
//  Copyright (c) 2003 Peter Dimov
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//-------------------------------------------------------------------
//ChangeLog(latest at top):
//  2005-06-08 Larry Evans
//    1) changed shared_ptr<X> to std_ptr<X>::shared_tagged
//    2) prepended collector_type:: to global gc function names used by 
//       collector_test.cpp
//  2005-05-28 Larry Evans
//    WHAT:
//      copied from libs/smart_ptr/test/collector_test.cpp
//    WHY:
//      adapt to test policy_ptr emulation of shared_ptr when
//      defined(BOOST_SP_ENABLE_DEBUG_HOOKS).
//-------------------------------------------------------------------
//

#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

// sp_collector.cpp exported functions

#include "boost/policy_ptr/std_ptr.hpp"

    typedef 
  boost::policy_ptr::detail::collector
  < boost::policy_ptr::detail::sp_counted_tagged
  >
collector_type
;
struct X
{
    void* fill[32];
    boost::std_ptr<X>::shared_tagged p;
};

void report()
{
    std::cout << "Calling find_unreachable_objects:\n";

    std::clock_t t = std::clock();

    std::size_t n = collector_type::find_unreachable_objects(false);

    t = std::clock() - t;

    std::cout << n << " unreachable objects.\n";
    std::cout << "  " << static_cast<double>(t) / CLOCKS_PER_SEC << " seconds.\n";
}

void free()
{
    std::cout << "Calling free_unreachable_objects:\n";

    std::clock_t t = std::clock();

    collector_type::free_unreachable_objects();

    t = std::clock() - t;

    std::cout << "  " << static_cast<double>(t) / CLOCKS_PER_SEC << " seconds.\n";
}

int main()
{
    typedef boost::std_ptr<X>::shared_tagged smart_ptr_type;
    
    std::vector< smart_ptr_type > v1, v2;

    int const n = 3;//256 * 1024;

    std::cout << "Filling v1 and v2\n";

    for(int i = 0; i < n; ++i)
    {
        v1.push_back(smart_ptr_type(new X));
        v2.push_back(smart_ptr_type(new X));
    }

    report();

    std::cout << "Creating the cycles\n";

    for(int i = 0; i < n - 1; ++i)
    {
        v2[i]->p = v2[i+1];
    }

    v2[n-1]->p = v2[0];

    report();

    std::cout << "Resizing v2 to size of 1\n";

    v2.resize(1);
    report();

    std::cout << "Clearing v2\n";

    v2.clear();
    report();

    std::cout << "Clearing v1\n";

    v1.clear();
    report();

    free();
    report();

}
