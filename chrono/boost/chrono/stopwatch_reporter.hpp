//  boost/chrono/stopwatch_reporter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_REPORTER_HPP
#define BOOST_CHRONO_STOPWATCH_REPORTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/stopwatch_scoped.hpp>
#include <boost/system/error_code.hpp>
#include <boost/chrono/suspendable_clock.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>

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


    template <class Stopwatch>
    struct stopwatch_reporter_default_formatter {
        typename Stopwatch::formatter type;
    };

    template <class Stopwatch, class Formatter=typename stopwatch_reporter_default_formatter<Stopwatch>::type>
    class stopwatch_reporter;

    template <class Stopwatch, class Formatter>
    struct stopwatch_reporter_default_formatter<stopwatch_reporter<Stopwatch, Formatter> > {
        typedef typename stopwatch_reporter_default_formatter<Stopwatch>::type type;
    };


    template <class Stopwatch, class Formatter>
    class stopwatch_reporter : public Stopwatch {
    public:
        typedef typename Stopwatch::clock clock;
        typedef Stopwatch stopwatch;
        typedef Formatter formatter;
        typedef typename Formatter::string_type string_type;
        typedef typename Formatter::char_type char_type;
        typedef typename Formatter::ostream_type ostream_type;
    
        explicit stopwatch_reporter( system::error_code & ec = system::throws )
        : m_places(Formatter::default_places()), m_os(Formatter::default_os()), m_format(Formatter::default_format()), m_reported(false) { }
        explicit stopwatch_reporter( ostream_type & os,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::default_places()), m_os(os), m_format(Formatter::default_format()), m_reported(false) { }

        explicit stopwatch_reporter( const string_type & format,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::default_places()), m_os(Formatter::default_os()), m_format(format), m_reported(false) {}

        explicit stopwatch_reporter( int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::default_os()), m_format(Formatter::default_format()), m_reported(false) { }

        stopwatch_reporter( ostream_type & os, const string_type & format,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::default_places()), m_os(os), m_format(format), m_reported(false) { }

        stopwatch_reporter( const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::default_os()), m_format(format), m_reported(false) { }

        stopwatch_reporter( ostream_type & os, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(os), m_format(Formatter::default_format()), m_reported(false) { }

        stopwatch_reporter( int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::default_os()), m_format(format), m_reported(false) { }

        stopwatch_reporter( ostream_type & os, const string_type & format, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(os), m_format(format), m_reported(false) { }

        stopwatch_reporter( ostream_type & os, int places, const string_type & format,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(os), m_format(format), m_reported(false) { }

        ~stopwatch_reporter() {// never throws
            system::error_code ec;
            //this->stop(ec);
            if ( !reported() ) {
                this->report( ec );
            }
        }


        inline void report( system::error_code & ec = system::throws );
        bool reported() const { return m_reported; }


        typedef stopwatch_runner<stopwatch_reporter<Stopwatch> > scoped_run;
        typedef stopwatch_suspender<stopwatch_reporter<Stopwatch> > scoped_suspend;
        typedef stopwatch_resumer<stopwatch_reporter<Stopwatch> > scoped_resume;

    protected:
        int             m_places;
        ostream_type &  m_os;
        string_type     m_format;
        bool            m_reported;


        //stopwatch_reporter(); // = delete;
        stopwatch_reporter(const stopwatch_reporter&); // = delete;
        stopwatch_reporter& operator=(const stopwatch_reporter&); // = delete;
    };

    template <class Stopwatch, class Formatter>
    void stopwatch_reporter<Stopwatch, Formatter>::report( system::error_code & ec ) {
        chrono::scoped_suspend<typename Stopwatch::clock> _(ec);
        m_reported = true;
        if ( m_format.empty() ) m_format = Formatter::default_format();

        //typename Stopwatch::duration d = this->elapsed( ec );
        if ( &ec == &system::throws ) {
            Formatter::show_time( *this, m_format.c_str(), m_places, m_os, ec);
        } else {// non-throwing
            try {
                Formatter::show_time( *this, m_format.c_str(), m_places, m_os, ec );
                ec = system::error_code();
            } catch (...) { // eat any exceptions
                assert( 0 && "error reporting not fully implemented yet" );
                //ec = error_code( EIO, errno_ecat );
            }
        }

    }


  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
