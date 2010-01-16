//  boost process_times.hpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2007, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_PROCESS_STOPWATCHED_HPP
#define BOOST_PROCESS_STOPWATCHED_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_clocks.hpp>
#include <boost/system/error_code.hpp>
//#include <boost/cstdint.hpp>
//#include <string>
//#include <ostream>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost
{
  namespace chrono
  {


    class BOOST_CHRONO_DECL process_stopwatches
    // BOOST_CHRONO_DECL is required to quiet compiler warnings even though
    // process_stopwatches has no dynamically linked members, because process_stopwatches is
    // used as a base class for process_stopwatches_reporter, which does have dynamically linked members.
    {
    public:

      typedef process_clocks                          clock;
      typedef process_clocks::duration                duration;
      typedef process_clocks::time_point              time_point;

      explicit process_stopwatches( system::error_code & ec = system::throws )
      {
        start(ec);
      }

     ~process_stopwatches() {}  // never throws

      void  start( system::error_code & ec = system::throws )
      {
        process_clocks::now( m_start, ec );
      }

      void  elapsed( process_clocks::durations & times, system::error_code & ec = system::throws )
      {
        process_clocks::durations end;
        process_clocks::now( end, ec );
        times.real  = end.real - m_start.real;
        times.user       = end.user - m_start.user;
        times.system     = end.system - m_start.system;
      }

    protected:
      process_clocks::durations   m_start;
    };


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP
