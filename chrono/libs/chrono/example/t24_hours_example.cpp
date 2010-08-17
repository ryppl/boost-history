//  t24_hours_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopwatch.hpp>
#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/t24_hours_formatter.hpp>
#include <cmath>

using namespace boost::chrono;
int f1(long j)
{
  stopwatch_reporter<stopwatch<process_real_cpu_clock>, t24_hours_formatter> hhmmss(BOOST_CHRONO_24_HOURS_FUNCTION_FORMAT);
  //stopwatch_reporter<stopwatch<process_real_cpu_clock>, t24_hours_formatter> hhmmss;

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  stopwatch_reporter<stopwatch<process_real_cpu_clock>, t24_hours_formatter> hhmmss(BOOST_CHRONO_24_HOURS_FUNCTION_FORMAT);

  f1(1000);
  f1(2000);
  f1(3000);
  return 0;
}
