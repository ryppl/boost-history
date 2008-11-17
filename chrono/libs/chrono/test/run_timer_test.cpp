//  boost run_timer_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/process_times.hpp>
#include <boost/chrono/timer.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <sstream>
#include <locale>
#include <ctime>

using boost::chrono::run_timer;
using boost::system::error_code;

#include <boost/test/minimal.hpp>

#define CHECK_REPORT(Timer,String_Stream,R,U,S,Expected_String) \
  check_report(Timer, String_Stream, R, U, S, Expected_String, __LINE__)


namespace
{
  typedef boost::chrono::nanoseconds ns;

  bool check_report( run_timer & tmr, std::stringstream & ss,
    run_timer::duration r, run_timer::duration u, run_timer::duration s, 
    const std::string & expected, int line )
  {
    tmr.test_report(r,u,s);
    bool result(true);
    if ( ss.str() != expected )
    {
      std::cout << "run_timer_test.cpp(" << line << ") : error: actual output \""
        << ss.str() << "\" != expected \"" << expected << "\"\n";
      result = false;
    }
    return result;
  }
  
  void run_timer_constructor_overload_test()
  {
    // exercise each supported combination of constructor arguments

    std::ostream &             os   = std::cout;
    const int                  pl   = 9;
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
    run_timer t13( pl, "t13, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t14( "t14, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    run_timer t15( os, pl, "t15, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n" );
    run_timer t16( os, "t16, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    run_timer t17( pl, "t17, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t18( "t18, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );
    run_timer t19( os, pl, "t19, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    run_timer t20( os, "t20, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );

    std::cout << "Burn some time so run_timers have something to report...";
    boost::chrono::timer<boost::chrono::high_resolution_clock> t;
    while ( t.elapsed() < boost::chrono::seconds(1) ) {}
    std::cout << "\n";
    std::cout << run_timer::default_places() << " default places\n";
    std::cout << pl << " explicitly coded places\n";

  }
}

int test_main( int argc, char * argv[] )
{
  std::locale loc( "" );     // test with appropriate locale
  std::cout.imbue( loc );

  {
    std::stringstream ss;
    run_timer t(ss);
    BOOST_CHECK( CHECK_REPORT(t, ss, ns(0), ns(0), ns(0),
      "\nreal 0.000s, cpu 0.000s (0.0%), user 0.000s, system 0.000s\n" ) );
  }

  {
    std::stringstream ss;
    run_timer t(ss);
    BOOST_CHECK( CHECK_REPORT(t, ss, ns(3000000000), ns(2000000000), ns(1000000000),
      "\nreal 3.000s, cpu 3.000s (100.0%), user 2.000s, system 1.000s\n" ) );
  }

  {
    std::stringstream ss;
    run_timer t( ss, "9 places: r %r, c %c, p %p, u %u, s %s", 9 );
    BOOST_CHECK( CHECK_REPORT(t, ss, ns(3000000003), ns(2000000002), ns(1000000001),
      "9 places: "
      "r 3.000000003, c 3.000000003, p 100.0, u 2.000000002, s 1.000000001" ) );
  }

  run_timer_constructor_overload_test();

  // accuracy test

  long timeout_in_secs = 1;
  if ( argc > 1 ) timeout_in_secs = std::atol( argv[1] );

  std::clock_t timeout_in_clock_t = std::clock();
  timeout_in_clock_t += (timeout_in_secs * CLOCKS_PER_SEC);

  std::cout << "accuracy test for " << timeout_in_secs << " second(s)...";

  boost::chrono::system_timer           sys;
  boost::chrono::monotonic_timer        mono;
  boost::chrono::high_resolution_timer  hires;
  boost::chrono::process_timer          process;
  
  std::clock_t now;
  do
  {
    now = std::clock();
  } while ( now < timeout_in_clock_t );

  boost::chrono::system_timer::duration sys_dur = sys.elapsed();
  boost::chrono::monotonic_timer::duration mono_dur = mono.elapsed();
  boost::chrono::high_resolution_timer::duration hires_dur = hires.elapsed();
  boost::chrono::process_times times;
  process.elapsed( times );

  std::cout << std::endl;

  ns timeout_in_nanoseconds( static_cast<long long>(timeout_in_secs) * 1000000000LL );
  ns maximum_delta ( static_cast<long long>(timeout_in_nanoseconds.count() * 0.02 ) );  // 2% leeway

  std::cout << timeout_in_nanoseconds.count() << " timeout_in_nanoseconds\n";
  std::cout << maximum_delta.count() << " maximum_delta\n";

  std::cout << sys_dur.count() << " sys_dur\n";

  BOOST_CHECK( sys_dur > timeout_in_nanoseconds - maximum_delta
    && sys_dur < timeout_in_nanoseconds + maximum_delta );

  std::cout << mono_dur.count() << " mono_dur\n";

  BOOST_CHECK( mono_dur > timeout_in_nanoseconds - maximum_delta
    && mono_dur < timeout_in_nanoseconds + maximum_delta );

  std::cout << hires_dur.count() << " hires_dur\n";

  BOOST_CHECK( hires_dur > timeout_in_nanoseconds - maximum_delta
    && hires_dur < timeout_in_nanoseconds + maximum_delta );

  std::cout << times.real.count() << " times.real\n";

  BOOST_CHECK( times.real > timeout_in_nanoseconds - maximum_delta
    && times.real < timeout_in_nanoseconds + maximum_delta );
  
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

