//  digital_time_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopwatch.hpp>
#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/digital_time_formatter.hpp>
#include <cmath>

using namespace boost::chrono;
int f1(long j)
{
  stopwatch_reporter<stopwatch<process_real_cpu_clock>, digital_time_formatter> hhmmss(BOOST_CHRONO_DIGITAL_TIME_FUNCTION_FORMAT);
  //stopwatch_reporter<stopwatch<process_real_cpu_clock>, digital_time_formatter> hhmmss;

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  stopwatch_reporter<stopwatch<process_real_cpu_clock>, digital_time_formatter> hhmmss(BOOST_CHRONO_DIGITAL_TIME_FUNCTION_FORMAT);

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}
