//  boost stopclock_constructor_overload_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopclock.hpp>
#include <cstdlib> // for atol()
#include <iostream>
#include <sstream>
#include <locale>
#include <ctime>
#include <cmath>  // for sqrt(), used to burn time

using boost::chrono::stopclock;
using boost::chrono::time_formatter;
using boost::system::error_code;

#include <boost/test/minimal.hpp>

namespace
{

  void stopclock_constructor_overload_test()
  {
    // exercise each supported combination of constructor arguments

    std::ostream &             os   = std::cout;
    const int                  pl   = 9;
    boost::system::error_code  ec;

    stopclock<> t1;
    stopclock<> t2( os );
    stopclock<> t3( ec );
    stopclock<> t4( os, ec );
    stopclock<> t5( pl );
    stopclock<> t6( os, pl );
    stopclock<> t7( pl, ec );
    stopclock<> t8( os, pl, ec );
    stopclock<> t9( "t9, default places,  r %r, c %c, p %p, u %u, s %s\n" );
    stopclock<> t10( os, "t10, default places,  r %r, c %c, p %p, u %u, s %s\n" );
    stopclock<> t11( "t11, default places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    stopclock<> t12( os, "t12, default places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    stopclock<> t13( pl, "t13, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n" );
    stopclock<> t14( "t14, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    stopclock<> t15( os, pl, "t15, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n" );
    stopclock<> t16( os, "t16, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl );
    stopclock<> t17( pl, "t17, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    stopclock<> t18( "t18, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );
    stopclock<> t19( os, pl, "t19, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", ec );
    stopclock<> t20( os, "t20, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n", pl, ec );

    std::cout << "Burn some time so run_timers have something to report...";
    boost::chrono::stopwatch<boost::chrono::high_resolution_clock> t;
    while ( t.elapsed() < boost::chrono::seconds(1) ) {}
    std::cout << "\n";
    std::cout << time_formatter::default_places() << " default places\n";
    std::cout << pl << " explicitly coded places\n";
  }

}

int test_main( int argc, char * argv[] )
{
  std::locale loc( "" );     // test with appropriate locale
  std::cout.imbue( loc );

  stopclock_constructor_overload_test();
  
  return 0;
}

