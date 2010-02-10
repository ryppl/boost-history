//  boost/chrono/t24_hours_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_T24_HOURS_FORMATTER_HPP
#define BOOST_CHRONO_T24_HOURS_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/t24_hours.hpp>
#include <boost/current_function.hpp>
#include <boost/chrono/detail/default_out.hpp>
#include <boost/chrono/detail/adaptive_string.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#define BOOST_CHRONO_24_HOURS_FORMAT_DEFAULT "%d day(s) %h:%m:%s.%n\n"

namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//


    template <
        typename CharT=char,
        typename Traits=std::char_traits<CharT>,
        class Alloc=std::allocator<CharT>
    >
    class basic_24_hours_formatter {
    public:

        typedef std::basic_string<CharT,Traits,Alloc> string_type;
        typedef CharT char_type;
        typedef std::basic_ostream<CharT,Traits> ostream_type;

        static ostream_type &  default_os();
        static const char_type* default_format();
        static string_type format(const char_type* s) {
            string_type res(s);
            res += boost::chrono::detail::adaptive_string(" : ");
            res += default_format();
            //res += boost::chrono::detail::adaptive_string(" tokes %d day(s) %h:%m:%s.%n\n");
            return res;
        }
        static int default_places() { return 3; }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_
            , const char_type* format, int places, ostream_type & os
            , system::error_code & ec)
        //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
        //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::duration duration_t;
            duration_t d = stopwatch_.elapsed( ec );

            if ( d < duration_t::zero() ) return;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            t24_hours dt(d);
            for ( ; *format; ++format ) {
                if ( (*format != '%') || (!*(format+1)) || (!std::strchr("dhmsn", *(format+1))) ) {
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
                        assert(0 && "basic_24_hours_formatter internal logic error");
                    }
                }
            }
        }
    };

namespace detail {
    template <typename CharT>
    struct basic_24_hours_formatter_default_format;
    template <>
    struct basic_24_hours_formatter_default_format<char> {
        static const char* apply() {return BOOST_CHRONO_24_HOURS_FORMAT_DEFAULT; }
    };
#ifndef BOOST_NO_STD_WSTRING
    template <>
    struct basic_24_hours_formatter_default_format<wchar_t> {
        static const wchar_t* apply() {return L"%d day(s) %h:%m:%s.%n\n"; }
    };

#endif
}

    template <typename CharT,typename Traits, class Alloc>
    const typename basic_24_hours_formatter<CharT,Traits,Alloc>::char_type*
    basic_24_hours_formatter<CharT,Traits,Alloc>::default_format() {
        return detail::basic_24_hours_formatter_default_format<CharT>::apply();
    }

    template <typename CharT,typename Traits, class Alloc>
    typename basic_24_hours_formatter<CharT,Traits,Alloc>::ostream_type &
    basic_24_hours_formatter<CharT,Traits,Alloc>::default_os()  {
        return detail::default_out<CharT,Traits>::apply();
    }

    typedef basic_24_hours_formatter<char> t24_hours_formatter;
    typedef basic_24_hours_formatter<wchar_t> wt24_hours_formatter;

  } // namespace chrono
} // namespace boost

#define BOOST_CHRONO_24_HOURS_FORMAT(F) boost::chrono::detail::adaptive_string(F " : "  BOOST_CHRONO_24_HOURS_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_CHRONO_24_HOURS_FUNCTION_FORMAT boost::chrono::t24_hours_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_24_HOURS_FUNCTION_FORMAT BOOST_CHRONO_24_HOURS_FORMAT(BOOST_CURRENT_FUNCTION)
#endif


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_T24_HOURS_FORMATTER_HPP
