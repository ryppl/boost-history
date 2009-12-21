//  boost process_clocks.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2007, 2008
//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_PROCESS_CLOCKS_HPP
#define BOOST_PROCESS_CLOCKS_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
//#include <boost/cstdint.hpp>
//#include <string>
//#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {

    class BOOST_CHRONO_DECL process_clocks
    {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_clocks>    time_point;
        static const bool is_monotonic =             true;

        struct durations
        {
          process_clocks::duration                       real;    // real (i.e wall clock) time
          process_clocks::duration                       user;    // user cpu time
          process_clocks::duration                       system;  // system cpu time
        };
        struct time_points
        {
          process_clocks::time_point                       real;    // real (i.e wall clock) time
          process_clocks::time_point                       user;    // user cpu time
          process_clocks::time_point                       system;  // system cpu time
        };
    
        static void now( durations & times,
                         system::error_code & ec = system::throws );
        static void now( time_points & times,
                         system::error_code & ec = system::throws );
    };


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP
