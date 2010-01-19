//  boost/chrono/stopclock.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPCLOCK_HPP
#define BOOST_CHRONO_STOPCLOCK_HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/detail/static_assert.hpp>
#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/stopwatch.hpp>
//#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/chrono/time_formatter.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#ifndef BOOST_CHRONO_USES_MPL_ASSERT
#define BOOST_CHRONO_S_STOPWATCH_CLOCK_MUST_BE_CLOCK        "Stopwatch::clock must be the same as Clock"
#endif

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
    class stopclock;

    template <class Clock, class Stopwatch, class Formatter>
    struct stopwatch_reporter_default_formatter<stopclock<Clock,Stopwatch, Formatter> > {
        typedef typename stopwatch_reporter_default_formatter<Stopwatch>::type type;
    };

    template <class Clock, class Stopwatch, class Formatter>
    class stopclock : public stopwatch_reporter<Stopwatch, Formatter> {
        BOOST_CHRONO_STATIC_ASSERT((boost::is_same<typename Stopwatch::clock, Clock>::value),
            BOOST_CHRONO_S_STOPWATCH_CLOCK_MUST_BE_CLOCK, (Stopwatch)(Stopwatch::clock)(Clock));
        
        typedef stopwatch_reporter<Stopwatch, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef Stopwatch stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit stopclock( system::error_code & ec = system::throws )
        : base_type(ec) { }
        explicit stopclock( ostream_type & os,
                    system::error_code & ec = system::throws )
        : base_type(os, ec) { }

        explicit stopclock( const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(format, ec) { }

        explicit stopclock( int places,
                    system::error_code & ec = system::throws )
        : base_type(places, ec) { }

        stopclock( ostream_type & os, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, format, ec) { }

        stopclock( const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(format, places, ec) { }

        stopclock( ostream_type & os, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, places, ec) { }

        stopclock( int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(places, format, ec) { }

        stopclock( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, format, places, ec) { }

        stopclock( ostream_type & os, int places, const string_type & format,
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
