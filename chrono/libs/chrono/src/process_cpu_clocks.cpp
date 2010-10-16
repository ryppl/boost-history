//  boost process_cpu_clocks.cpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

//--------------------------------------------------------------------------------------//

// define BOOST_CHRONO_SOURCE so that <boost/chrono/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_CHRONO_SOURCE

#include <boost/chrono/detail/inlined/process_cpu_clocks.hpp>

#if 0


#include <boost/chrono/config.hpp>
#include <boost/version.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/process_times.hpp>
#include <cassert>

namespace boost { namespace chrono {

    process_real_cpu_clock::time_point process_real_cpu_clock::now(system::error_code & ec) {
        process_times t;
        process_clock::now(t, ec);
        return process_real_cpu_clock::time_point(t.real);
    }

    process_user_cpu_clock::time_point process_user_cpu_clock::now(system::error_code & ec){
        process_times t;
        process_clock::now(t, ec);
        return process_user_cpu_clock::time_point(t.user);
    }

    process_system_cpu_clock::time_point process_system_cpu_clock::now(system::error_code & ec) {
        process_times t;
        process_clock::now(t, ec);
        return process_system_cpu_clock::time_point(t.system);
    }
    
    process_cpu_clock::time_point process_cpu_clock::now( system::error_code & ec ) {
        process_times t;
        process_clock::now(t,ec);
        time_point::rep r(t.real.count(), t.user.count(), t.system.count()); 
        return time_point(duration(r));
    }
    
} // namespace chrono
} // namespace boost


#endif
