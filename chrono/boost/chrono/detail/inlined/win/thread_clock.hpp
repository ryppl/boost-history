//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
//#define BOOST_CHRONO_SOURCE

#include <boost/chrono/config.hpp>
#include <boost/chrono/thread_clock.hpp>
#include <cassert>

# include <windows.h>

namespace boost
{
  namespace chrono
  {

    thread_clock::time_point thread_clock::now( system::error_code & ec )
    {

      //  note that Windows uses 100 nanosecond ticks for FILETIME
      FILETIME creation, exit, user_time, system_time;

      if ( ::GetThreadTimes( ::GetCurrentThread (), &creation, &exit,
             &system_time, &user_time ) )
      {
        duration user   = duration(
          ((static_cast<duration::rep>(user_time.dwHighDateTime) << 32)
            | user_time.dwLowDateTime) * 100 );

        duration system = duration(
          ((static_cast<duration::rep>(system_time.dwHighDateTime) << 32)
            | system_time.dwLowDateTime) * 100 );
          
        ec.clear();
        return time_point(system+user);
          
      }
      else
      {
        //~ assert( 0 && "error handling not implemented yet" );
#if ((BOOST_VERSION / 100000) < 2) && ((BOOST_VERSION / 100 % 1000) < 44)
        ec.assign( ::GetLastError(), system::system_category );
#else
        ec.assign( ::GetLastError(), system::system_category() );
#endif          
        return thread_clock::time_point(duration(0));
      }

    }
    
    thread_clock::time_point thread_clock::now( )
    {

      //  note that Windows uses 100 nanosecond ticks for FILETIME
      FILETIME creation, exit, user_time, system_time;

      if ( ::GetThreadTimes( ::GetCurrentThread (), &creation, &exit,
             &system_time, &user_time ) )
      {
        duration user   = duration(
          ((static_cast<duration::rep>(user_time.dwHighDateTime) << 32)
            | user_time.dwLowDateTime) * 100 );

        duration system = duration(
          ((static_cast<duration::rep>(system_time.dwHighDateTime) << 32)
            | system_time.dwLowDateTime) * 100 );
          
        return time_point(system+user);
          
      }
      else
      {
        boost::throw_exception(
#if ((BOOST_VERSION / 100000) < 2) && ((BOOST_VERSION / 100 % 1000) < 44)
            system::system_error( ::GetLastError(), system::system_category, "chrono::monotonic_clock" ));
#else
            system::system_error( ::GetLastError(), system::system_category(), "chrono::monotonic_clock" ));
#endif          
      }

    }
    

    
  } // namespace chrono
} // namespace boost
