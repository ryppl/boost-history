//  stopwatch_accumulator_example.cpp  ---------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/stopwatches for documentation.

#include <boost/stopwatches/stopclock_accumulator.hpp>

#include <cmath>
#include "sleep_for.hpp"

using namespace boost::chrono;
using namespace boost::stopwatches;
template <typename Clock>
int f1(long j)
{
  static stopclock_accumulator<Clock> acc(BOOST_STOPWATCHES_ACCUMULATOR_FUNCTION_FORMAT);
  typename stopclock_accumulator<Clock>::scoped_run _(acc);

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time
  boost::this_thread::sleep_for(boost::chrono::milliseconds(20));
  

  return 0;
}
int main()
{
  static stopclock_accumulator<process_cpu_clock> acc(BOOST_STOPWATCHES_ACCUMULATOR_TIME_FUNCTION_FORMAT);
  stopclock_accumulator<process_cpu_clock>::scoped_run _(acc);

  f1<high_resolution_clock>(1000);
  f1<high_resolution_clock>(2000);
  f1<high_resolution_clock>(3000);
  return 0;
}
