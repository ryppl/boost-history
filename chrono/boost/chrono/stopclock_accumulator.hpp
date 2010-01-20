//  boost/chrono/stopclock.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPCLOCK_ACCUMULATOR__HPP
#define BOOST_CHRONO_STOPCLOCK_ACCUMULATOR__HPP

#include <boost/chrono/config.hpp>
#include <boost/chrono/detail/static_assert.hpp>

#include <boost/chrono/stopwatch_reporter.hpp>
#include <boost/chrono/stopwatch_accumulator.hpp>
#include <boost/chrono/stopwatch_accumulator_formatter.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/type_traits/is_same.hpp>

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
#ifndef BOOST_CHRONO_USES_MPL_ASSERT
#define BOOST_CHRONO_SA_STOPWATCH_CLOCK_MUST_BE_CLOCK        "Stopwatch::clock must be the same as Clock"
#endif
    template <class Clock=high_resolution_clock, class Stopwatch=stopwatch_accumulator<Clock>, class Formatter=typename stopwatch_reporter_default_formatter<Stopwatch>::type>
    class stopclock_accumulator;

    template <class Clock, class Stopwatch, class Formatter>
    struct stopwatch_reporter_default_formatter<stopclock_accumulator<Clock,Stopwatch, Formatter> > {
        typedef typename stopwatch_reporter_default_formatter<Stopwatch>::type type;
    };

    template <class Clock, class Stopwatch, class Formatter>
    class stopclock_accumulator : public stopwatch_reporter<Stopwatch, Formatter> {
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

        explicit stopclock_accumulator( system::error_code & ec = system::throws )
        : base_type(ec) { }
        explicit stopclock_accumulator( ostream_type & os,
                    system::error_code & ec = system::throws )
        : base_type(os, ec) { }

        explicit stopclock_accumulator( const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(format, ec) { }

        explicit stopclock_accumulator( int places,
                    system::error_code & ec = system::throws )
        : base_type(places, ec) { }

        stopclock_accumulator( ostream_type & os, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, format, ec) { }

        stopclock_accumulator( const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(format, places, ec) { }

        stopclock_accumulator( ostream_type & os, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, places, ec) { }

        stopclock_accumulator( int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(places, format, ec) { }

        stopclock_accumulator( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, format, places, ec) { }

        stopclock_accumulator( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, places, format, ec) { }


        typedef typename base_type::scoped_run scoped_run;
        typedef typename base_type::scoped_suspend scoped_suspend;
        typedef typename base_type::scoped_resume scoped_resume;
    };

    typedef stopclock_accumulator< boost::chrono::system_clock > system_stopclock_accumulator;
    #ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef stopclock_accumulator< boost::chrono::monotonic_clock > monotonic_stopclock_accumulator;
    #endif
    typedef stopclock_accumulator< boost::chrono::high_resolution_clock > high_resolution_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_real_cpu_clock > process_real_cpu_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_user_cpu_clock > process_user_cpu_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_system_cpu_clock > process_system_cpu_stopclock_accumulator;
    //typedef stopclock_accumulator< boost::chrono::process_cpu_clock > process_cpu_stopclock_accumulator;

  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_STOPCLOCK_HPP
