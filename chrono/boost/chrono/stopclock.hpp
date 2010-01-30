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

    template <class Clock, class Formatter>
    class basic_stopclock : public basic_stopwatch_reporter<chrono::stopwatch<Clock>, Formatter> {
        typedef basic_stopwatch_reporter<chrono::stopwatch<Clock>, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef chrono::stopwatch<Clock> stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit basic_stopclock( system::error_code & ec = system::throws )
        : base_type(ec) { }
        explicit basic_stopclock( ostream_type & os,
                    system::error_code & ec = system::throws )
        : base_type(os, ec) { }

        explicit basic_stopclock( const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(format, ec) { }

        explicit basic_stopclock( int places,
                    system::error_code & ec = system::throws )
        : base_type(places, ec) { }

        basic_stopclock( ostream_type & os, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, format, ec) { }

        basic_stopclock( const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(format, places, ec) { }

        basic_stopclock( ostream_type & os, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, places, ec) { }

        basic_stopclock( int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(places, format, ec) { }

        basic_stopclock( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, format, places, ec) { }

        basic_stopclock( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, places, format, ec) { }

        typedef stopwatch_runner<basic_stopclock> scoped_run;
        typedef stopwatch_stopper<basic_stopclock> scoped_stop;
        typedef stopwatch_suspender<basic_stopclock> scoped_suspend;
        typedef stopwatch_resumer<basic_stopclock> scoped_resume;

    };

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<basic_stopclock<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock=process_cpu_clock, class Formatter=typename stopwatch_reporter_default_formatter<stopwatch<Clock> >::type>
    class stopclock;

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<stopclock<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock, class Formatter>
    class stopclock : public basic_stopclock<Clock, Formatter> {
        typedef basic_stopclock<Clock, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef chrono::stopwatch<Clock> stopwatch;
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

        typedef stopwatch_runner<stopclock> scoped_run;
        typedef stopwatch_stopper<stopclock> scoped_stop;
        typedef stopwatch_suspender<stopclock> scoped_suspend;
        typedef stopwatch_resumer<stopclock> scoped_resume;

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

    template <class Clock=process_cpu_clock, class Formatter=typename wstopwatch_reporter_default_formatter<stopwatch<Clock> >::type>
    class wstopclock;

    template <class Clock, class Formatter>
    struct stopwatch_reporter_default_formatter<wstopclock<Clock,Formatter> > {
        typedef Formatter type;
    };

    template <class Clock, class Formatter>
    class wstopclock : public basic_stopclock<Clock, Formatter> {
        typedef basic_stopclock<Clock, Formatter> base_type;
    public:
        typedef Clock clock;
        typedef chrono::stopwatch<Clock> stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit wstopclock( system::error_code & ec = system::throws )
        : base_type(ec) { }
        explicit wstopclock( ostream_type & os,
                    system::error_code & ec = system::throws )
        : base_type(os, ec) { }

        explicit wstopclock( const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(format, ec) { }

        explicit wstopclock( int places,
                    system::error_code & ec = system::throws )
        : base_type(places, ec) { }

        wstopclock( ostream_type & os, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, format, ec) { }

        wstopclock( const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(format, places, ec) { }

        wstopclock( ostream_type & os, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, places, ec) { }

        wstopclock( int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(places, format, ec) { }

        wstopclock( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : base_type(os, format, places, ec) { }

        wstopclock( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : base_type(os, places, format, ec) { }

        typedef stopwatch_runner<wstopclock> scoped_run;
        typedef stopwatch_stopper<wstopclock> scoped_stop;
        typedef stopwatch_suspender<wstopclock> scoped_suspend;
        typedef stopwatch_resumer<wstopclock> scoped_resume;

    };

    typedef wstopclock< boost::chrono::system_clock > system_wstopclock;
    #ifdef BOOST_CHRONO_HAS_CLOCK_MONOTONIC
    typedef wstopclock< boost::chrono::monotonic_clock > monotonic_wstopclock;
    #endif
    typedef wstopclock< boost::chrono::high_resolution_clock > high_resolution_wstopclock;
    typedef wstopclock< boost::chrono::process_real_cpu_clock > process_real_cpu_wstopclock;
    typedef wstopclock< boost::chrono::process_user_cpu_clock > process_user_cpu_wstopclock;
    typedef wstopclock< boost::chrono::process_system_cpu_clock > process_system_cpu_wstopclock;
    //typedef wstopclock< boost::chrono::process_cpu_clock > process_cpu_wstopclock;


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_STOPCLOCK_HPP
