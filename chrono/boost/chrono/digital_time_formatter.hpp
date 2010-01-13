//  boost/chrono/timer.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_DIGITAL_TIME_FORMATTER_HPP
#define BOOST_CHRONO_DIGITAL_TIME_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/digital_time.hpp>
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
//--------------------------------------------------------------------------------------//


    class digital_time_formatter {
    public:
        static std::ostream &  m_cout();
        static const int m_default_places = 3;
        static const char * default_format;
        static int default_places() { return m_default_places; }

        template <class Stopwatch > 
        static void show_time( Stopwatch & stopwatch_
            , const char * format, int places, std::ostream & os
            , system::error_code & ec)
        //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
        //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::duration duration;
            duration d = stopwatch_.elapsed( ec );
          
            if ( d < duration(0) ) return;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            digital_time dt(d);
            for ( ; *format; ++format ) {
                if ( *format != '%' || !*(format+1) || !std::strchr("dhmsn", *(format+1)) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 'd':
                        os << dt.days_.count();
                        break;
                    case 'h': 
                    {
                        boost::io::ios_flags_saver ifs( os );
                        os.width(2); os.fill('0');
                        os << dt.hours_.count();
                        break;
                    }
                    case 'm':
                    {
                        boost::io::ios_flags_saver ifs( os );
                        os.width(2); os.fill('0');
                        os << dt.minutes_.count();
                        break;
                    }
                    case 's':
                    {
                        boost::io::ios_flags_saver ifs( os );
                        os.width(2); os.fill('0');
                        os << dt.seconds_.count();
                        break;
                    }
                    case 'n':
                    {
                        boost::io::ios_flags_saver ifs( os );
                        os.width(9); os.fill('0');
                        os << dt.nanoseconds_.count();
                        break;
                    }
                    default:
                        assert(0 && "digital_time_formatter internal logic error");
                    }
                }
            }
        }
    };  
    const char * digital_time_formatter::default_format ="\n%d days(s) %h:%m:%s.%n\n";
    
    std::ostream &  digital_time_formatter::m_cout()  { return std::cout; }

  } // namespace chrono
} // namespace boost

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_DIGITAL_TIME_FORMATTER_HPP
