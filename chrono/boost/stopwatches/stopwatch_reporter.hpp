//  boost/stopwatches/stopwatch_reporter.hpp  ------------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_STOPWATCH_REPORTER_HPP
#define BOOST_STOPWATCHES_STOPWATCH_REPORTER_HPP

#if !defined(BOOST_ENABLE_WARNINGS) && !defined(BOOST_STOPWATCHES_ENABLE_WARNINGS) 
#if defined __GNUC__
#pragma GCC system_header
#elif defined __SUNPRO_CC
#pragma disable_warn
#elif defined _MSC_VER
#pragma warning(push, 1)
#endif
#endif

#include <boost/chrono/chrono.hpp>
#include <boost/stopwatches/stopwatch_scoped.hpp>
#include <boost/system/error_code.hpp>
#include <boost/stopwatches/clock_suspender.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>

#include <boost/config/abi_prefix.hpp> // must be the last #include

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
 *      stopwatch_reporter<stopwatch<> > _;
 *      // ...
 * }
 */
/* void f2()
 * {
 *      stopwatch<>::reporter _;
 *      // ...
 * }
 */
/* void f3()
 * {
 *      static stopwatch_reporter<stopwatch_accumulator<> > t;
 *      stopwatch_reporter<stopwatch_accumulator<> >::scoped_run _(t);
 *      // ...
 * }
 */
/* void f4()
 * {
 *      static stopwatch_accumulator<>::reporter t;
 *      stopwatch_accumulator<>::reporter::scoped_run _(t);
 *      // ...
 * }
 */
//--------------------------------------------------------------------------------------//

    template <class Stopwatch, class Formatter>
    class basic_stopwatch_reporter : public Stopwatch {
    public:
        typedef typename Stopwatch::clock clock;
        typedef Stopwatch stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        basic_stopwatch_reporter( )
        : m_places(Formatter::default_places()), m_os(Formatter::default_os()), m_format(Formatter::default_format()), m_reported(false)  {
        }
        explicit basic_stopwatch_reporter( system::error_code &  )
        : m_places(Formatter::default_places()), m_os(Formatter::default_os()), m_format(Formatter::default_format()), m_reported(false)  {
        }
        explicit basic_stopwatch_reporter( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(Formatter::default_places()), m_os(os), m_format(Formatter::default_format()), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        explicit basic_stopwatch_reporter( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(Formatter::default_places()), m_os(Formatter::default_os()), m_format(format), m_reported(false) { 
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        explicit basic_stopwatch_reporter( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(Formatter::default_os()), m_format(Formatter::default_format()), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(Formatter::default_places()), m_os(os), m_format(format), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(Formatter::default_os()), m_format(format), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(os), m_format(Formatter::default_format()), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(Formatter::default_os()), m_format(format), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(os), m_format(format), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        basic_stopwatch_reporter( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : m_places(places), m_os(os), m_format(format), m_reported(false) {  
        if (!BOOST_CHRONO_IS_THROWS(ec)) ec.clear();
        }

        ~basic_stopwatch_reporter() {// never throws
            system::error_code ec;
            //this->stop(ec);
            if ( !reported() ) {
                this->report( ec );
            }
        }


        inline void report( system::error_code & /*ec*/ = BOOST_CHRONO_THROWS );
        bool reported() const { return m_reported; }


        typedef stopwatch_runner<basic_stopwatch_reporter<Stopwatch,Formatter> > scoped_run;
        typedef stopwatch_stopper<basic_stopwatch_reporter<Stopwatch,Formatter> > scoped_stop;
        typedef stopwatch_suspender<basic_stopwatch_reporter<Stopwatch,Formatter> > scoped_suspend;
        typedef stopwatch_resumer<basic_stopwatch_reporter<Stopwatch,Formatter> > scoped_resume;

    protected:
        int             m_places;
        ostream_type &  m_os;
        string_type     m_format;
        bool            m_reported;


        //basic_stopwatch_reporter(); // = delete;
        basic_stopwatch_reporter(const basic_stopwatch_reporter&); // = delete;
        basic_stopwatch_reporter& operator=(const basic_stopwatch_reporter&); // = delete;
    };

    template <class Stopwatch, class Formatter>
    void basic_stopwatch_reporter<Stopwatch, Formatter>::report( system::error_code & ec ) {
        stopwatches::clock_suspender<typename Stopwatch::clock> _(ec);
    if (!BOOST_CHRONO_IS_THROWS(ec)) {
        if (ec) return;
    }
        if ( m_format.empty() ) m_format = Formatter::default_format();

        m_reported = true;
        Formatter::show_time( *this, m_format.c_str(), m_places, m_os, ec);
    }


    template <class Stopwatch>
    struct stopwatch_reporter_default_formatter;

    template <class Stopwatch, class Formatter=typename stopwatch_reporter_default_formatter<Stopwatch>::type>
    class stopwatch_reporter;

    template <class Stopwatch, class Formatter>
    struct stopwatch_reporter_default_formatter<stopwatch_reporter<Stopwatch, Formatter> > {
        typedef Formatter type;
    };


    template <class Stopwatch, class Formatter>
    class stopwatch_reporter : public basic_stopwatch_reporter<Stopwatch,Formatter> {
        typedef basic_stopwatch_reporter<Stopwatch,Formatter> base_type;
    public:
        typedef typename Stopwatch::clock clock;
        typedef Stopwatch stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        stopwatch_reporter( )
        : base_type() { }
        explicit stopwatch_reporter( system::error_code & ec  )
        : base_type(ec) { }
        explicit stopwatch_reporter( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, ec) { }

        explicit stopwatch_reporter( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, ec) { }

        explicit stopwatch_reporter( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, ec) { }

        stopwatch_reporter( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, ec) { }

        stopwatch_reporter( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, places, ec) { }

        stopwatch_reporter( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, ec) { }

        stopwatch_reporter( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, format, ec) { }

        stopwatch_reporter( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, places, ec) { }

        stopwatch_reporter( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, format, ec) { }

        typedef stopwatch_runner<stopwatch_reporter<Stopwatch,Formatter> > scoped_run;
        typedef stopwatch_stopper<stopwatch_reporter<Stopwatch,Formatter> > scoped_stop;
        typedef stopwatch_suspender<stopwatch_reporter<Stopwatch,Formatter> > scoped_suspend;
        typedef stopwatch_resumer<stopwatch_reporter<Stopwatch,Formatter> > scoped_resume;

    protected:


        //stopwatch_reporter(); // = delete;
        stopwatch_reporter(const stopwatch_reporter&); // = delete;
        stopwatch_reporter& operator=(const stopwatch_reporter&); // = delete;
    };


    template <class Stopwatch>
    struct wstopwatch_reporter_default_formatter;

    template <class Stopwatch, class Formatter=typename wstopwatch_reporter_default_formatter<Stopwatch>::type>
    class wstopwatch_reporter;

    template <class Stopwatch, class Formatter>
    struct wstopwatch_reporter_default_formatter<wstopwatch_reporter<Stopwatch, Formatter> > {
        typedef Formatter type;
    };


    template <class Stopwatch, class Formatter>
    class wstopwatch_reporter : public basic_stopwatch_reporter<Stopwatch,Formatter> {
        typedef basic_stopwatch_reporter<Stopwatch,Formatter> base_type;
    public:
        typedef typename Stopwatch::clock clock;
        typedef Stopwatch stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;

        explicit wstopwatch_reporter( system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(ec) { }
        explicit wstopwatch_reporter( ostream_type & os,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, ec) { }

        explicit wstopwatch_reporter( const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, ec) { }

        explicit wstopwatch_reporter( int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, ec) { }

        wstopwatch_reporter( ostream_type & os, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, ec) { }

        wstopwatch_reporter( const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(format, places, ec) { }

        wstopwatch_reporter( ostream_type & os, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, ec) { }

        wstopwatch_reporter( int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(places, format, ec) { }

        wstopwatch_reporter( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, format, places, ec) { }

        wstopwatch_reporter( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = BOOST_CHRONO_THROWS )
        : base_type(os, places, format, ec) { }

        typedef stopwatch_runner<wstopwatch_reporter<Stopwatch,Formatter> > scoped_run;
        typedef stopwatch_stopper<wstopwatch_reporter<Stopwatch,Formatter> > scoped_stop;
        typedef stopwatch_suspender<wstopwatch_reporter<Stopwatch,Formatter> > scoped_suspend;
        typedef stopwatch_resumer<wstopwatch_reporter<Stopwatch,Formatter> > scoped_resume;

    protected:

        //wstopwatch_reporter(); // = delete;
        wstopwatch_reporter(const wstopwatch_reporter&); // = delete;
        wstopwatch_reporter& operator=(const wstopwatch_reporter&); // = delete;
    };

  } // namespace stopwatches
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#if !defined(BOOST_ENABLE_WARNINGS) && !defined(BOOST_STOPWATCHES_ENABLE_WARNINGS)
#if defined __SUNPRO_CC
#pragma enable_warn
#elif defined _MSC_VER
#pragma warning(pop)
#endif
#endif


#endif
