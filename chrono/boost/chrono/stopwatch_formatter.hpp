//  boost/chrono/stopwatch_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_FORMATTER_HPP
#define BOOST_CHRONO_STOPWATCH_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/current_function.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>

#include <boost/config/abi_prefix.hpp> // must be the last #include

namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//


    class stopwatch_formatter {
    public:
        static std::ostream &  default_os();
        static const int m_default_places = 3;
        static const char * m_default_format;
        static const char* default_format() { return m_default_format; }
        static std::string format(const char* s) {
            std::string res(s);
            res += " tokes %ds\n";
            return res;
        }
        static int default_places() { return m_default_places; }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_, const char * format, int places, std::ostream & os, system::error_code & ec)
        //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
        //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::duration duration;
            duration d = stopwatch_.elapsed( ec );

            if ( d < duration(0) ) return;
            if ( places > 9 )
                places = 9;  // sanity check
            else if ( places < 0 )
                places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            for ( ; *format; ++format ) {
                if ( *format != '%' || !*(format+1) || !std::strchr("d", *(format+1)) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 'd':
                        os << boost::chrono::duration<double>(d).count();
                        break;
                    default:
                        assert(0 && "run_timer internal logic error");
                    }
                }
            }
        }
    };
    const char * stopwatch_formatter::m_default_format ="%ds\n";

    std::ostream &  stopwatch_formatter::default_os()  { return std::cout; }

  } // namespace chrono
} // namespace boost

#define BOOST_CHRONO_STOPWATCH_FORMAT(F) F " tokes %ds\n"
#ifdef __GNUC__
#define BOOST_CHRONO_STOPWATCH_FUNCTION_FORMAT boost::chrono::stopwatch_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_STOPWATCH_FUNCTION_FORMAT BOOST_CHRONO_STOPWATCH_FORMAT(BOOST_CURRENT_FUNCTION)
#endif

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
