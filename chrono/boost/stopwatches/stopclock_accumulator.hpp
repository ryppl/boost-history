//  boost/stopwatches/stopclock.hpp  ------------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_STOPCLOCK_ACCUMULATOR_HPP
#define BOOST_STOPWATCHES_STOPCLOCK_ACCUMULATOR_HPP

#include <boost/stopwatches/config.hpp>
#include <boost/stopwatches/detail/static_assert.hpp>

#include <boost/stopwatches/stopwatch_reporter.hpp>
#include <boost/stopwatches/stopwatch_accumulator.hpp>
#include <boost/stopwatches/stopwatch_accumulator_formatter.hpp>
#include <boost/stopwatches/stopwatch_accumulator_time_formatter.hpp>
#include <boost/stopwatches/suspendible_clock.hpp>
#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/type_traits/is_same.hpp>

namespace boost { namespace stopwatches  {

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

    template <class Clock, typename Features, typename Weight>
    struct stopwatch_reporter_default_formatter<stopwatch_accumulator<Clock, Features,Weight> > {
        typedef stopwatch_accumulator_formatter type;
    };

    template <class Clock, typename Features, typename Weight>
    struct wstopwatch_reporter_default_formatter<stopwatch_accumulator<Clock, Features,Weight> > {
        typedef wstopwatch_accumulator_formatter type;
    };

    template <typename Features, typename Weight>
    struct stopwatch_reporter_default_formatter<stopwatch_accumulator<chrono::process_cpu_clock,Features,Weight> > {
        typedef stopwatch_accumulator_time_formatter type;
    };

    template <typename Features, typename Weight>
    struct wstopwatch_reporter_default_formatter<stopwatch_accumulator<chrono::process_cpu_clock, Features,Weight> > {
        typedef wstopwatch_accumulator_time_formatter type;
    };

    template <typename Features, typename Weight>
    struct stopwatch_reporter_default_formatter<stopwatch_accumulator<stopwatches::suspendible_clock<chrono::process_cpu_clock>, Features,Weight> > {
        typedef stopwatch_reporter_default_formatter<stopwatch_accumulator<chrono::process_cpu_clock> >::type  type;
    };

    template <typename Features, typename Weight>
    struct wstopwatch_reporter_default_formatter<stopwatch_accumulator<stopwatches::suspendible_clock<chrono::process_cpu_clock>, Features,Weight> > {
        typedef wstopwatch_reporter_default_formatter<stopwatch_accumulator<chrono::process_cpu_clock> >::type  type;
    };


    template <class Clock, class Formatter>
    class basic_stopclock_accumulator : public basic_stopwatch_reporter<stopwatch_accumulator<Clock>, Formatter> {
        typedef basic_stopwatch_reporter<stopwatch_accumulator<Clock>, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef stopwatch_accumulator<Clock> stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit basic_stopclock_accumulator( system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(ec) { }
        explicit basic_stopclock_accumulator( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, ec) { }

        explicit basic_stopclock_accumulator( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, ec) { }

        explicit basic_stopclock_accumulator( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, ec) { }

        basic_stopclock_accumulator( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, ec) { }

        basic_stopclock_accumulator( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, places, ec) { }

        basic_stopclock_accumulator( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, ec) { }

        basic_stopclock_accumulator( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, format, ec) { }

        basic_stopclock_accumulator( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, places, ec) { }

        basic_stopclock_accumulator( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, format, ec) { }


        typedef stopwatch_runner<basic_stopclock_accumulator> scoped_run;
        typedef stopwatch_stopper<basic_stopclock_accumulator> scoped_stop;
        typedef stopwatch_suspender<basic_stopclock_accumulator> scoped_suspend;
        typedef stopwatch_resumer<basic_stopclock_accumulator> scoped_resume;
    };

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<basic_stopclock_accumulator<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock=chrono::high_resolution_clock, class Formatter=typename stopwatch_reporter_default_formatter<stopwatch_accumulator<Clock> >::type>
    class stopclock_accumulator;

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<stopclock_accumulator<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock, class Formatter>
    class stopclock_accumulator : public basic_stopclock_accumulator<Clock, Formatter> {
        typedef basic_stopclock_accumulator<Clock, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef stopwatch_accumulator<Clock> stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit stopclock_accumulator( system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(ec) { }
        explicit stopclock_accumulator( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, ec) { }

        explicit stopclock_accumulator( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, ec) { }

        explicit stopclock_accumulator( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, ec) { }

        stopclock_accumulator( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, ec) { }

        stopclock_accumulator( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, places, ec) { }

        stopclock_accumulator( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, ec) { }

        stopclock_accumulator( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, format, ec) { }

        stopclock_accumulator( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, places, ec) { }

        stopclock_accumulator( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, format, ec) { }


        typedef stopwatch_runner<stopclock_accumulator> scoped_run;
        typedef stopwatch_stopper<stopclock_accumulator> scoped_stop;
        typedef stopwatch_suspender<stopclock_accumulator> scoped_suspend;
        typedef stopwatch_resumer<stopclock_accumulator> scoped_resume;
    };

    typedef stopclock_accumulator< boost::chrono::system_clock > system_stopclock_accumulator;
    #ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
    typedef stopclock_accumulator< boost::chrono::steady_clock > steady_stopclock_accumulator;
    #endif
    typedef stopclock_accumulator< boost::chrono::high_resolution_clock > high_resolution_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_real_cpu_clock > process_real_cpu_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_user_cpu_clock > process_user_cpu_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_system_cpu_clock > process_system_cpu_stopclock_accumulator;
    typedef stopclock_accumulator< boost::chrono::process_cpu_clock > process_cpu_stopclock_accumulator;

    template <class Clock=chrono::high_resolution_clock, class Formatter=typename wstopwatch_reporter_default_formatter<stopwatch_accumulator<Clock> >::type>
    class wstopclock_accumulator;

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<wstopclock_accumulator<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock, class Formatter>
    class wstopclock_accumulator : public basic_stopclock_accumulator<Clock, Formatter> {
        typedef basic_stopclock_accumulator<Clock, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef stopwatch_accumulator<Clock> stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit wstopclock_accumulator( system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(ec) { }
        explicit wstopclock_accumulator( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, ec) { }

        explicit wstopclock_accumulator( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, ec) { }

        explicit wstopclock_accumulator( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, ec) { }

        wstopclock_accumulator( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, ec) { }

        wstopclock_accumulator( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, places, ec) { }

        wstopclock_accumulator( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, ec) { }

        wstopclock_accumulator( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, format, ec) { }

        wstopclock_accumulator( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, places, ec) { }

        wstopclock_accumulator( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, format, ec) { }


        typedef stopwatch_runner<wstopclock_accumulator> scoped_run;
        typedef stopwatch_stopper<wstopclock_accumulator> scoped_stop;
        typedef stopwatch_suspender<wstopclock_accumulator> scoped_suspend;
        typedef stopwatch_resumer<wstopclock_accumulator> scoped_resume;
    };

    typedef wstopclock_accumulator< boost::chrono::system_clock > system_wstopclock_accumulator;
    #ifdef BOOST_CHRONO_HAS_CLOCK_STEADY
    typedef wstopclock_accumulator< boost::chrono::steady_clock > steady_wstopclock_accumulator;
    #endif
    typedef wstopclock_accumulator< boost::chrono::high_resolution_clock > high_resolution_wstopclock_accumulator;
    typedef wstopclock_accumulator< boost::chrono::process_real_cpu_clock > process_real_cpu_wstopclock_accumulator;
    typedef wstopclock_accumulator< boost::chrono::process_user_cpu_clock > process_user_cpu_wstopclock_accumulator;
    typedef wstopclock_accumulator< boost::chrono::process_system_cpu_clock > process_system_cpu_wstopclock_accumulator;
    typedef wstopclock_accumulator< boost::chrono::process_cpu_clock > process_cpu_wstopclock_accumulator;


  } // namespace stopwatches
} // namespace boost

#endif // BOOST_STOPWATCHES_STOPCLOCK_HPP
