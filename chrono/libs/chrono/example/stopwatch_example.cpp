//  run_timer_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <iostream>
#include <boost/chrono/process_times.hpp>
#include <boost/chrono/stopwatch.hpp>
#include <boost/chrono/stopwatch_accumulator.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/time_formatter.hpp>
#include <boost/chrono/digital_time_formatter.hpp>
#include <cmath>


using namespace boost::chrono;
int f1(long j)
{
  static stopwatch_accumulator<process_real_cpu_clock>::reporter t(
    "\nf1 Count=%c times Sum=%ss Min=%ms Max=%Ms Mean=%as\n");
  stopwatch_accumulator<process_real_cpu_clock>::reporter::scoped_run _(t);
  stopwatch<process_real_cpu_clock>::reporter x("\nF1 %dsec\n");
  
#if 0    
  run_timer y;   
#else    
  stopwatch_reporter<stopwatch<process_cpu_clock>, time_formatter> y;
#endif    
  stopwatch_reporter<stopwatch<process_real_cpu_clock>, digital_time_formatter> hhmmss;
    

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  stopwatch<boost::chrono::process_real_cpu_clock>::reporter _("\nMain %dsec\n");
  run_timer z;


  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}
