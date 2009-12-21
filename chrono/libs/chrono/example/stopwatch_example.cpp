//  run_timer_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <iostream>
#include <boost/chrono/process_times.hpp>
#include <boost/chrono/stopwatch.hpp>
#include <boost/chrono/stopwatch_accumulator.hpp>
#include <boost/chrono/cpu_clocks.hpp>
#include <cmath>

//#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/weighted_sum.hpp>

int f1(long j)
{
  static boost::chrono::stopwatch_accumulator<boost::chrono::process_real_cpu_clock>::reporter t(
    "\nf1 Count=%c times Sum=%ss Min=%ms Max=%Ms Mean=%as\n");
  boost::chrono::stopwatch_accumulator<boost::chrono::process_real_cpu_clock>::reporter::scoped_run _(t);
  boost::chrono::stopwatch<boost::chrono::process_real_cpu_clock>::reporter x("\nF1 %dsec\n");
  boost::chrono::run_timer y;


  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  boost::chrono::stopwatch<boost::chrono::process_real_cpu_clock>::reporter _("\nMain %dsec\n");
  boost::chrono::run_timer z;


  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}
