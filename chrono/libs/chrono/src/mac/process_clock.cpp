//  boost process_timer.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE

#include <boost/chrono/config.hpp>
#include <boost/chrono/process_times.hpp>
#include <cassert>

#include <sys/time.h> //for gettimeofday and timeval
# include <unistd.h>

namespace
{
  long tick_factor()        // multiplier to convert ticks
                            //  to nanoseconds; -1 if unknown
  {
    static long factor = 0;
    if ( !factor )
    {
      if ( (factor = ::sysconf( _SC_CLK_TCK )) <= 0 )
        factor = -1;
      else
      {
        assert( factor <= 1000000l ); // doesn't handle large ticks
        factor = 1000000l / factor;  // compute factor
        if ( !factor ) factor = -1;
      }
    }
    return factor;
  }
}

#else
# error unknown API
#endif

namespace boost
{
  namespace chrono
  {

    void process_clock::now( process_times & times_, system::error_code & ec )
    {

      tms tm;
      clock_t c = ::times( &tm );
      if ( c == -1 ) // error
      {
        assert( 0 && "error handling not implemented yet" );

        ec = system::error_code( errno, system::system_category );
        times_.real = times_.system = times_.user = nanoseconds(-1);
      }
      else
      {
        times_.real = microseconds(c);
        times_.system = microseconds(tm.tms_stime + tm.tms_cstime);
        times_.user = microseconds(tm.tms_utime + tm.tms_cutime);
        if ( tick_factor() != -1 )
        {
          times_.real *= tick_factor();
          times_.user *= tick_factor();
          times_.system *= tick_factor();
        }
        else
        {
        assert( 0 && "error handling not implemented yet" );
          ec = system::error_code( errno, system::system_category );
          times_.real = times_.user = times_.system = nanoseconds(-1);
        }
      }

    }
  } // namespace chrono
} // namespace boost
