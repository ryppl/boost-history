//  boost/chrono/clock_suspender.hpp  -----------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_CLOCK_SUSPENDER_CLOCK_HPP
#define BOOST_STOPWATCHES_CLOCK_SUSPENDER_CLOCK_HPP

#include <boost/system/error_code.hpp>
#include <boost/mpl/bool.hpp>

namespace boost { namespace stopwatches {


    template <class Clock>
    struct is_suspendible : mpl:: false_ {};

    template <class Clock>
    class clock_suspender {
    public:
        clock_suspender(system::error_code & ec = BOOST_CHRONO_THROWS) {
            ec.clear();
        }
        ~clock_suspender() {}
    private:
        //~ clock_suspender(); // = delete;
        clock_suspender(const clock_suspender&); // = delete;
        clock_suspender& operator=(const clock_suspender&); // = delete;
    };


} // namespace stopwatches
} // namespace boost


#endif  // BOOST_CHRONO_PROCESS_CPU_CLOCKS_HPP
