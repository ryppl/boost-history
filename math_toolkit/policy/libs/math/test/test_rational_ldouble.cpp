
#include <boost/test/floating_point_comparison.hpp>
#include <boost/test/test_exec_monitor.hpp>
#include <boost/array.hpp>
#include <boost/math/tools/rational.hpp>
#include <iostream>

#include "test_rational.hpp"

void test_spots(long double t, const char* n)
{
   std::cout << "Testing basic sanity checks for type " << n << std::endl;
   do_test_spots(t, int(0));
   do_test_spots(t, unsigned(0));
#ifdef BOOST_HAS_LONG_LONG
   do_test_spots(t, (unsigned long long)(0));
#endif
   do_test_spots(t, float(0));
   do_test_spots(t, static_cast<long double>(0));
}
