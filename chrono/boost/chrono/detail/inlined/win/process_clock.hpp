//  boost process_timer.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//
#ifndef BOOST_CHRONO_DETAIL_INLINED_WIN_PROCESS_CLOCK_HPP
#define BOOST_CHRONO_DETAIL_INLINED_WIN_PROCESS_CLOCK_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/system_clocks.hpp>
#include <boost/chrono/process_times.hpp>
#include <cassert>

#include <boost/detail/win/time.hpp>

namespace boost
{
  namespace chrono
  {
    
    void process_clock::now( process_times & times_, system::error_code & ec )
    {

      //  note that Windows uses 100 nanosecond ticks for FILETIME
      boost::detail::win32::FILETIME_ creation, exit, user_time, system_time;

      times_.real = duration( monotonic_clock::now().time_since_epoch().count() );

      if ( boost::detail::win32::GetProcessTimes( 
    		  boost::detail::win32::GetCurrentProcess(), &creation, &exit,
             &system_time, &user_time ) )
      {
        ec.clear();
        times_.user   = duration(
          ((static_cast<time_point::rep>(user_time.dwHighDateTime) << 32)
            | user_time.dwLowDateTime) * 100 );

        times_.system = duration(
          ((static_cast<time_point::rep>(system_time.dwHighDateTime) << 32)
            | system_time.dwLowDateTime) * 100 );
      }
      else
      {
        //~ assert( 0 && "error handling not implemented yet" );
#if ((BOOST_VERSION / 100000) < 2) && ((BOOST_VERSION / 100 % 1000) < 44)
        ec.assign( boost::detail::win32::GetLastError(), system::system_category );
#else
        ec.assign( boost::detail::win32::GetLastError(), system::system_category() );
#endif          
        times_.real = times_.system = times_.user = nanoseconds(-1);
      }

    }
  } // namespace chrono
} // namespace boost

#endif          
