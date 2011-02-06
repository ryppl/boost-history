//  boost/stopwatches/process_cpu_clocks.hpp  -----------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_STOPWATCHES_SCOPED_SUSPEND_CLOCK_HPP
#define BOOST_STOPWATCHES_SCOPED_SUSPEND_CLOCK_HPP

#include <boost/system/error_code.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace stopwatches {


    template <class Clock>
    struct is_suspendible : mpl:: false_ {};

    template <class Clock>
    class scoped_suspend {
    public:
        scoped_suspend(system::error_code & ec = BOOST_CHRONO_THROWS) {
            ec.clear();
        }
        ~scoped_suspend() {}
    private:
        //~ scoped_suspend(); // = delete;
        scoped_suspend(const scoped_suspend&); // = delete;
        scoped_suspend& operator=(const scoped_suspend&); // = delete;
    };


} // namespace stopwatches
} // namespace boost


#endif  // BOOST_STOPWATCHES_PROCESS_CPU_CLOCKS_HPP
