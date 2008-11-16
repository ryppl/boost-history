//  boost process_timer.cpp  ------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//---------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE 

#include <boost/chrono/config.hpp>
#include <boost/chrono/process_times.hpp>
#include <cassert>

#if defined(BOOST_CHRONO_WINDOWS_API)
# include <windows.h>
#elif defined(BOOST_CHRONO_POSIX_API)
# include <sys/times.h>
#else
# error unknown API
#endif

namespace boost
{
  namespace chrono
  {

    void process_clock::now( process_times & times, system::error_code & ec )
    {

#   if defined(BOOST_WINDOWS_API)

      //  note that Windows uses 100 nanosecond ticks for FILETIME
      FILETIME creation, exit, user_time, system_time;

      //FILETIME stopwatch_time;
      //::GetSystemTimeAsFileTime( &stopwatch_time );
      //times.real = duration(
      //  ((static_cast<time_point::rep>(stopwatch_time.dwHighDateTime) << 32)
      //    | stopwatch_time.dwLowDateTime) * 100 );

      times.real = duration( monotonic_clock::now().time_since_epoch().count() );

      if ( ::GetProcessTimes( ::GetCurrentProcess(), &creation, &exit,
             &system_time, &user_time ) )
      {
        times.user   = duration(
          ((static_cast<time_point::rep>(user_time.dwHighDateTime) << 32)
            | user_time.dwLowDateTime) * 100 );

        times.system = duration(
          ((static_cast<time_point::rep>(system_time.dwHighDateTime) << 32)
            | system_time.dwLowDateTime) * 100 );
      }
      else
      {
        assert( 0 && "error handling not implemented yet" );
        //ec = error_code( ::GetLastError(), native_ecat );
        //current.wall = current.system = current.user = microsecond_t(-1);
      }

#   else
      tms tm;
      clock_t c = ::times( &tm );
      if ( c == -1 ) // error
      {
        ec = error_code( errno, native_ecat );
        current.wall = current.system = current.user = microsecond_t(-1);
      }
      else
      {
        current.wall = microsecond_t(c);
        current.system = microsecond_t(tm.tms_stime + tm.tms_cstime);
        current.user = microsecond_t(tm.tms_utime + tm.tms_cutime);
        if ( tick_factor() != -1 )
        {
          current.wall *= tick_factor();
          current.user *= tick_factor();
          current.system *= tick_factor();
        }
        else
        {
          ec = error_code( errno, native_ecat );
          current.wall = current.user = current.system = microsecond_t(-1);
        }
      }
#   endif
    }
  } // namespace chrono
} // namespace boost
