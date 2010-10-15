//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright Vicente J. Botet Escriba 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

#include <boost/chrono/config.hpp>
#include <boost/chrono/thread_clock.hpp>
#include <cassert>

# include <sys/times.h>
# include <unistd.h>

namespace boost { namespace chrono {

	thread_clock::time_point thread_clock::now( ) {
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, &clock_id);
        // get the timespec associated to the thread clock
        struct timespec ts;
        if ( ::clock_gettime( clock_id, &ts ) )
        {
            boost::throw_exception(
#if ((BOOST_VERSION / 100000) < 2) && ((BOOST_VERSION / 100 % 1000) < 44)
            system::system_error( errno, system::system_category, "chrono::thread_clock" ));
#else
            system::system_error( errno, system::system_category(), "chrono::thread_clock" ));
#endif          
        }

        // transform to nanoseconds
        return time_point(duration(
            static_cast<thread_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));
             
    }
	thread_clock::time_point thread_clock::now( system::error_code & ec ) {
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, &clock_id);
        // get the timespec associated to the thread clock
        struct timespec ts;
        if ( ::clock_gettime( clock_id, &ts ) )
        {
#if ((BOOST_VERSION / 100000) < 2) && ((BOOST_VERSION / 100 % 1000) < 44)
          ec.assign( errno, system::system_category );
#else
          ec.assign( errno, system::system_category() );
#endif          
          return time_point();
        }
        ec.clear();
        // transform to nanoseconds
        return time_point(duration(
            static_cast<thread_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));
             
    }
} }
