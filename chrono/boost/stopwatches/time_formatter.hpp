//  boost/chrono/time_formatter.hpp  ------------------------------------------------------------//
//  Copyright 2009-2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//  See http://www.boost.org/libs/stopwatches for documentation.

#ifndef BOOST_STOPWATCHES_TIME_FORMATTER_HPP
#define BOOST_STOPWATCHES_TIME_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
#include <boost/current_function.hpp>
#include <boost/stopwatches/detail/default_out.hpp>
#include <boost/stopwatches/detail/adaptive_string.hpp>
#include <boost/system/error_code.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>
#include <boost/assert.hpp>

#define BOOST_STOPWATCHES_TIME_FORMAT_DEFAULT "real %rs, cpu %cs (%p%), user %us, system %ss\n"

namespace boost { namespace stopwatches  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//


    template <
        typename CharT=char,
        typename Traits=std::char_traits<CharT>,
        class Alloc=std::allocator<CharT>
    >
    class basic_time_formatter {
    public:

        typedef std::basic_string<CharT,Traits,Alloc> string_type;
        typedef CharT char_type;
        typedef std::basic_ostream<CharT,Traits> ostream_type;

        static ostream_type &  default_os();
        static const char_type* default_format();
        static string_type format(const char_type* s) {
            string_type res(s);
            res += boost::stopwatches::detail::adaptive_string(" : ");
            res += default_format();
            //res += boost::stopwatches::detail::adaptive_string(" spent real %rs, cpu %cs (%p%), user %us, system %ss\n");
            return res;
        }
        static int default_places() { return 3; }

        template <class Stopwatch >
        static void show_time( typename Stopwatch::duration::rep const & times
            , const char_type* format, int places, ostream_type & os
            , system::error_code & ec
        )
          //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
          //  be as low as 10, although will be 15 for many common platforms.
          {
        if (!BOOST_CHRONO_IS_THROWS(ec))  ec.clear();
            typedef typename Stopwatch::duration duration;
            typedef typename duration::rep rep;
            if ( times.real < 0 ) return;
            if ( places > 9 )
              places = 9;  // sanity check
            else if ( places < 0 )
              places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            chrono::nanoseconds total = chrono::nanoseconds(times.system + times.user);

            for ( ; *format; ++format )
            {
              if ( (*format != '%') || (!*(format+1)) || (!std::strchr("rcpus", *(format+1))) )
                os << *format;
              else
              {
                ++format;
                switch ( *format )
                {
                case 'r':
                  os << boost::chrono::duration<double>(chrono::nanoseconds(times.real)).count();
                  break;
                case 'u':
                  os << boost::chrono::duration<double>(chrono::nanoseconds(times.user)).count();
                  break;
                case 's':
                  os << boost::chrono::duration<double>(chrono::nanoseconds(times.system)).count();
                  break;
                case 'c':
                  os << boost::chrono::duration<double>(total).count();
                  break;
                case 'p':
                  {
                    boost::io::ios_precision_saver ips( os );
                    os.precision( 1 );
                    if ( times.real && total.count() )
                      os << boost::chrono::duration<double>(total).count()
                           /boost::chrono::duration<double>(chrono::nanoseconds(times.real)).count() * 100.0;
                    else
                      os << 0.0;
                  }
                  break;
                default:
                  BOOST_ASSERT(0 && "basic_time_formatter internal logic error");
                }
              }
            }
          }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_
            , const char_type* format, int places, ostream_type & os
            , system::error_code & ec)
          //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
          //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::duration duration;
            typedef typename duration::rep rep;
            duration d = stopwatch_.elapsed( ec );
        if (!BOOST_CHRONO_IS_THROWS(ec)) {
        if (ec) return;
        }
            rep times=d.count();
            show_time<Stopwatch>(times, format, places, os, ec);
        }
    };

namespace detail {
    template <typename CharT>
    struct basic_time_formatter_default_format;
    template <>
    struct basic_time_formatter_default_format<char> {
        static const char* apply() {return BOOST_STOPWATCHES_TIME_FORMAT_DEFAULT; }
    };
#ifndef BOOST_NO_STD_WSTRING
    template <>
    struct basic_time_formatter_default_format<wchar_t> {
        static const wchar_t* apply() {return L"real %rs, cpu %cs (%p%), user %us, system %ss\n"; }
    };

#endif
}

    template <typename CharT,typename Traits, class Alloc>
    const typename basic_time_formatter<CharT,Traits,Alloc>::char_type*
    basic_time_formatter<CharT,Traits,Alloc>::default_format() {
        return detail::basic_time_formatter_default_format<CharT>::apply();
    }

    template <typename CharT,typename Traits, class Alloc>
    typename basic_time_formatter<CharT,Traits,Alloc>::ostream_type &
    basic_time_formatter<CharT,Traits,Alloc>::default_os()  {
        return detail::default_out<CharT,Traits>::apply();
    }

    typedef basic_time_formatter<char> time_formatter;
    typedef basic_time_formatter<wchar_t> wtime_formatter;

  } // namespace stopwatches
} // namespace boost

#define BOOST_STOPWATCHES_TIME_FORMAT(F) boost::stopwatches::detail::adaptive_string(F " : " BOOST_STOPWATCHES_TIME_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_STOPWATCHES_TIME_FUNCTION_FORMAT boost::stopwatches::time_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_STOPWATCHES_TIME_FUNCTION_FORMAT BOOST_STOPWATCHES_TIME_FORMAT(BOOST_CURRENT_FUNCTION)
#endif

#endif // BOOST_STOPWATCHES_TIME_FORMATTER_HPP
