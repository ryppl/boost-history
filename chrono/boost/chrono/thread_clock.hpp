//  boost/chrono/process_cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_THREAD_CLOCK_HPP
#define BOOST_CHRONO_THREAD_CLOCK_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/duration.hpp>
#include <boost/chrono/time_point.hpp>
#include <boost/system/error_code.hpp>
//~ #include <boost/throw_exception.hpp>
//~ #include <boost/system/system_error.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#if defined(BOOST_CHRONO_HAS_THREAD_CLOCK) 

namespace boost { namespace chrono {

class BOOST_CHRONO_DECL thread_clock {
public:
    typedef nanoseconds                          duration;
    typedef duration::rep                        rep;
    typedef duration::period                     period;
    typedef chrono::time_point<thread_clock>    time_point;
    static const bool is_monotonic =             BOOST_CHRONO_THREAD_CLOCK_IS_MONOTONIC;

    static time_point now( );
    static time_point now( system::error_code & ec );
};
} // namespace chrono
} // namespace boost

#endif

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_CHRONO_THREAD_CLOCK_HPP
