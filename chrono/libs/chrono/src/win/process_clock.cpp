//  boost process_timer.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008

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

# include <windows.h>

namespace boost
{
  namespace chrono
  {

    void process_clock::now( process_times & times_, system::error_code & ec )
    {

      //  note that Windows uses 100 nanosecond ticks for FILETIME
      FILETIME creation, exit, user_time, system_time;

      times_.real = duration( monotonic_clock::now().time_since_epoch().count() );

      if ( ::GetProcessTimes( ::GetCurrentProcess(), &creation, &exit,
             &system_time, &user_time ) )
      {
        times_.user   = duration(
          ((static_cast<time_point::rep>(user_time.dwHighDateTime) << 32)
            | user_time.dwLowDateTime) * 100 );

        times_.system = duration(
          ((static_cast<time_point::rep>(system_time.dwHighDateTime) << 32)
            | system_time.dwLowDateTime) * 100 );
      }
      else
      {
        assert( 0 && "error handling not implemented yet" );
        //ec = error_code( ::GetLastError(), native_ecat );
        //times_.real = times_.system = times_.user = nanoseconds(-1);
      }

    }
  } // namespace chrono
} // namespace boost
