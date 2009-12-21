//  boost cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_CPU_CLOCKS_HPP
#define BOOST_CHRONO_CPU_CLOCKS_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono {

    class BOOST_CHRONO_DECL process_real_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_real_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };

    class process_user_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_user_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };

    class process_system_cpu_clock {
    public:
        typedef nanoseconds                          duration;
        typedef duration::rep                        rep;
        typedef duration::period                     period;
        typedef chrono::time_point<process_system_cpu_clock>    time_point;
        static const bool is_monotonic =             true;

        static time_point now( system::error_code & ec = system::throws );
    };
    
} // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif  // BOOST_PROCESS_TIMES_HPP
