//  boost run_timer_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/process_times.hpp>
#include <boost/chrono/timer.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <locale>

using boost::chrono::run_timer;
using boost::system::error_code;

namespace
{
  
  void run_timer_constructor_overload_test()
  {
    // exercise each supported combination of constructor arguments

    std::ostream &             os   = std::cout;
    int                        pl   = 6;
    boost::system::error_code  ec;

    run_timer t1;
    run_timer t2( os );
    run_timer t3( ec );
    run_timer t4( os, ec );
    run_timer t5( pl );
    run_timer t6( os, pl );
    run_timer t7( pl, ec );
    run_timer t8( os, pl, ec );
    run_timer t9( "t9, default places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t10( os, "t10, default places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t11( "t11, default places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t12( os, "t12, default places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t13( pl, "t13, 6 places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t14( "t14, 6 places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    run_timer t15( os, pl, "t15, 6 places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t16( os, "t16, 6 places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    run_timer t17( pl, "t17, 6 places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t18( "t18, 6 places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );
    run_timer t19( os, pl, "t19, 6 places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t20( os, "t20, 6 places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );

    std::cout << "Burn some time so run_timers have something to report...";
    boost::chrono::timer<boost::chrono::high_resolution_clock> t;
    while ( t.elapsed() < boost::chrono::seconds(1) ) {}
    std::cout << "\n";
    std::cout << "default places is " << run_timer::default_places() << "\n";

  }
}

int main( int argc, char * argv[] )
{
  //std::locale loc( "" );
  //std::cout.imbue( loc );

  run_timer_constructor_overload_test();
  
  //run_timer timer(6);
  //run_timer timer2("\nrtc %r sec, utilization %p%\n");
  //run_timer timer3("\nrtc %r sec, total cpu %c sec, utilization %p%\n", 3);

  //long count = 0;
  //times_t times;
  //times.real = 0;
  //microsecond_t timeout
  //  = microsecond_t(500000); // default .5 seconds

  //if ( argc > 1 ) timeout = microsecond_t(std::atol( argv[1] ));

  //while ( times.real < timeout )
  //{
  //  //  The point of this code is to burn both kernal and user cpu time,
  //  //  with the total less than wall clock time.
  //  ++count;
  //  timer.elapsed( times );
  //  //    sleep(1);
  //  std::cout << "iteration " << count << ", "
  //    << times.real << " real, "
  //    << times.user << " user, "
  //    << times.system << " system microsecs"
  //    << std::endl;
  //}

  //std::cout << count << " iterations\n";
  return 0;
}

