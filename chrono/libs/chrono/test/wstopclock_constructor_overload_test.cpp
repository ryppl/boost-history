//  boost run_timer_test.cpp  -----------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008
//  Copyright 2009 Vicente J. Botet Escriba
//  Copyright 2010 Tan Zhi(Tom)

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopclock.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/stopwatch.hpp>

#include <cstdlib> // for atol()
#include <iostream>
#include <sstream>
#include <locale>
#include <ctime>
#include <cmath>  // for sqrt(), used to burn time

#include <string>

using boost::chrono::stopwatch;
using boost::chrono::stopclock;
using boost::chrono::process_cpu_clock;
using boost::chrono::wtime_formatter;
using boost::chrono::time_formatter;
using boost::system::error_code;
using boost::chrono::detail::adaptive_string;
#include <boost/test/minimal.hpp>


namespace util
{    
    
    /**
     * this class return std::cout or std::wcout according to 
     * the context(e.g. type on the left side of '=' operator)
     */
    struct console
    {
        
        template <
            typename Traits
        >
        operator std::basic_ostream<char,Traits>&() const
        {
            return std::cout;
        }
#ifndef BOOST_NO_STD_WSTRING
        template <
            typename Traits
        >
        operator std::basic_ostream<wchar_t,Traits>&() const
        {
            return std::wcout;
        } 
#endif        
    };

} // namespace util


    // overload TR for streaming operation with cout/wcout
    template<class charT>
    std::basic_ostream<charT>& operator << (std::basic_ostream<charT>& os, const boost::chrono::detail::adaptive_string& s)
    {
        os << std::basic_string<charT>(s);
        return os;
    }    


namespace
{
    using namespace util;
    
    template<class stopclock_type>
    void stopclock_constructor_overload_test()
    {
        // exercise each supported combination of constructor arguments

        typename stopclock_type::ostream_type& os    = console();
        const int                  pl       = 9;
        boost::system::error_code  ec;

        stopclock_type t1;
        stopclock_type t2( os );
        stopclock_type t3( ec );
        stopclock_type t4( os, ec );
        stopclock_type t5( pl );
        stopclock_type t6( os, pl );
        stopclock_type t7( pl, ec );
        stopclock_type t8( os, pl, ec );
        stopclock_type t9( adaptive_string("t9, default places,  r %r, c %c, p %p, u %u, s %s\n") );
        stopclock_type t10( os, adaptive_string("t10, default places,  r %r, c %c, p %p, u %u, s %s\n") );
        stopclock_type t11( adaptive_string("t11, default places,  r %r, c %c, p %p, u %u, s %s\n"), ec );
        stopclock_type t12( os, adaptive_string("t12, default places,  r %r, c %c, p %p, u %u, s %s\n"), ec );
        stopclock_type t13( pl, adaptive_string("t13, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n") );
        stopclock_type t14( adaptive_string("t14, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), pl );
        stopclock_type t15( os, pl, adaptive_string("t15, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n") );
        stopclock_type t16( os, adaptive_string("t16, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), pl );
        stopclock_type t17( pl, adaptive_string("t17, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), ec );
        stopclock_type t18( adaptive_string("t18, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), pl, ec );
        stopclock_type t19( os, pl, adaptive_string("t19, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), ec );
        stopclock_type t20( os, adaptive_string("t20, explicitly code places,  r %r, c %c, p %p, u %u, s %s\n"), pl, ec );

        std::cout << adaptive_string("Burn some time so run_timers have something to report...");

        boost::chrono::stopwatch<boost::chrono::high_resolution_clock> t;
        while ( t.elapsed() < boost::chrono::seconds(1) ) {}

        std::cout << adaptive_string("\n");
        std::cout << time_formatter::default_places() << adaptive_string(" default places\n");
        std::cout << pl << adaptive_string(" explicitly coded places\n");
    }
  
    typedef stopclock <> stopclock_t;
    typedef stopclock <
            process_cpu_clock, 
            stopwatch<process_cpu_clock>, 
            boost::chrono::wtime_formatter
            > wstopclock_t; 
}

int test_main( int argc, char * argv[] )
{
  std::locale loc( "" );     // test with appropriate locale
  std::cout.imbue( loc );
   
  stopclock_constructor_overload_test<stopclock_t>(); // test for char
#ifndef BOOST_NO_STD_WSTRING
  stopclock_constructor_overload_test<wstopclock_t>(); // test for wchar_t 
#endif    
  return 0;
}

