//  boost/chrono/process_cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_THREAD_CLOCK_HPP
#define BOOST_CHRONO_THREAD_CLOCK_HPP

#include <time.h>
#include <pthread.h>

#include <boost/chrono/config.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/throw_exception.hpp>
#include <boost/system/system_error.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono {

#if defined(BOOST_CHRONO_POSIX_API) && defined(_POSIX_THREAD_CPUTIME)
class BOOST_CHRONO_DECL thread_clock {
public:
    typedef nanoseconds                          duration;
    typedef duration::rep                        rep;
    typedef duration::period                     period;
    typedef chrono::time_point<thread_clock>    time_point;
    static const bool is_monotonic =             true;

    static time_point now( ) {
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, clock_id);
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
    static time_point now( system::error_code & ec ) {
        // get the current thread
        pthread_t pth=pthread_self();
        // get the clock_id associated to the current thread
        clockid_t clock_id;
        pthread_getcpuclockid(pth, clock_id);
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
};
#endif
} // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_CHRONO_THREAD_CLOCK_HPP
