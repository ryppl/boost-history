//  boost thread_clock.cpp  -----------------------------------------------------------//

//  Copyright Beman Dawes 1994, 2006, 2008
//  Copyright Vicente J. Botet Escriba 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE

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
            system::system_error( errno, system::system_category, "chrono::thread_clock" ));
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
          ec.assign( errno, system::system_category );
          return time_point();
        }
        ec.clear();
        // transform to nanoseconds
        return time_point(duration(
            static_cast<thread_clock::rep>( ts.tv_sec ) * 1000000000 + ts.tv_nsec));
             
    }
} }
