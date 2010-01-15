//  boost/chrono/timer.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_REPORTER_HPP
#define BOOST_CHRONO_STOPWATCH_REPORTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/stopwatch_scoped.hpp>
#include <boost/system/error_code.hpp>
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
        explicit stopwatch_reporter( system::error_code & ec = system::throws )
        : m_places(Formatter::m_default_places), m_os(Formatter::m_cout()), m_format(Formatter::default_format), m_reported(false) { }
        explicit stopwatch_reporter( std::ostream & os,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::m_default_places), m_os(os), m_format(Formatter::default_format), m_reported(false) { }

        explicit stopwatch_reporter( const std::string & format,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::m_default_places), m_os(Formatter::m_cout()), m_format(format), m_reported(false) {}

        explicit stopwatch_reporter( int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::m_cout()), m_format(Formatter::default_format), m_reported(false) { }

        stopwatch_reporter( std::ostream & os, const std::string & format,
                    system::error_code & ec = system::throws )
        : m_places(Formatter::m_default_places), m_os(os), m_format(format), m_reported(false) { }

        stopwatch_reporter( const std::string & format, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::m_cout()), m_format(format), m_reported(false) { }
        
        stopwatch_reporter( std::ostream & os, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(os), m_format(Formatter::default_format), m_reported(false) { }
        
        stopwatch_reporter( int places, const std::string & format,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(Formatter::m_cout()), m_format(format), m_reported(false) { }
        
        stopwatch_reporter( std::ostream & os, const std::string & format, int places,
                    system::error_code & ec = system::throws )
        : m_places(places), m_os(os), m_format(format), m_reported(false) { }

        stopwatch_reporter( std::ostream & os, int places, const std::string & format,
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
        
    private:
        int             m_places;
        std::ostream &  m_os;
        std::string     m_format;
        bool            m_reported;

    
        //stopwatch_reporter(); // = delete;
        stopwatch_reporter(const stopwatch_reporter&); // = delete;
        stopwatch_reporter& operator=(const stopwatch_reporter&); // = delete;
    };
    
    template <class Stopwatch, class Formatter> 
    void stopwatch_reporter<Stopwatch, Formatter>::report( system::error_code & ec ) {
        m_reported = true;
        if ( m_format.empty() ) m_format = Formatter::default_format;

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
