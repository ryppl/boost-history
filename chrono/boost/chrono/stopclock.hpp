//  boost/chrono/stopclock.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPCLOCK_HPP
#define BOOST_CHRONO_STOPCLOCK_HPP

#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/stopwatch.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/time_formatter.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//~ provides a everything a Timer provides and it adds reporting capabilities that can be invoked in a single line of code. The reporting is controleed by two parameters:

    //~ * format : The output format
    //~ * places(precission): the number of decimal placess used.

//~ The default places is given by default_places and is 3. The default format is "\n%ts\n", where

    //~ * %t : the result of elapsed() when the reporting is done.

//~ The time is given using the suffix "s" following the System International d'Unites Std.

/* void f1()
 * {
 *      stopclock<> _;
 *      // ...
 * }
 */
//--------------------------------------------------------------------------------------//

    template <class Clock=process_cpu_clock, class Stopwatch=stopwatch<Clock>, class Formatter=typename stopwatch_reporter_default_formatter<Stopwatch>::type>
    class stopclock : public stopwatch_reporter<Stopwatch, Formatter> {
        typedef stopwatch_reporter<Stopwatch, Formatter> base_type;
    public:
        explicit stopclock( system::error_code & ec = system::throws )
        : base_type(ec) { }
        explicit stopclock( std::ostream & os,
                    system::error_code & ec = system::throws )
        : base_type(os, ec) { }

        explicit stopclock( const std::string & format,
                    system::error_code & ec = system::throws )
        : base_type(format, ec) { }

        explicit stopclock( int places,
                    system::error_code & ec = system::throws )
        : base_type(places, ec) { }

        stopclock( std::ostream & os, const std::string & format,
                    system::error_code & ec = system::throws )
        : base_type(os, format, ec) { }

        stopclock( const std::string & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(format, places, ec) { }

        stopclock( std::ostream & os, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, places, ec) { }

        stopclock( int places, const std::string & format,
                    system::error_code & ec = system::throws )
        : base_type(places, format, ec) { }

        stopclock( std::ostream & os, const std::string & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, format, places, ec) { }

        stopclock( std::ostream & os, int places, const std::string & format,
                    system::error_code & ec = system::throws )
        : base_type(os, places, format, ec) { }


        typedef typename base_type::scoped_run scoped_run;
        typedef typename base_type::scoped_suspend scoped_suspend;
        typedef typename base_type::scoped_resume scoped_resume;
    };

    typedef stopclock< boost::chrono::system_clock > system_stopclock;
    #ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef stopclock< boost::chrono::monotonic_clock > monotonic_stopclock;
    #endif
    typedef stopclock< boost::chrono::high_resolution_clock > high_resolution_stopclock;
    typedef stopclock< boost::chrono::process_real_cpu_clock > process_real_cpu_stopclock;
    typedef stopclock< boost::chrono::process_user_cpu_clock > process_user_cpu_stopclock;
    typedef stopclock< boost::chrono::process_system_cpu_clock > process_system_cpu_stopclock;
    //typedef stopclock< boost::chrono::process_cpu_clock > process_cpu_stopclock;

  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_STOPCLOCK_HPP
