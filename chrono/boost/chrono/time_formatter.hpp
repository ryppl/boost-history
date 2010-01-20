//  boost/chrono/time_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_TIME_FORMATTER_HPP
#define BOOST_CHRONO_TIME_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/chrono/process_cpu_clocks.hpp>
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


#define BOOST_CHRONO_TIME_FORMAT_DEFAULT "real %rs, cpu %cs (%p%), user %us, system %ss\n"


namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//


    template <
        typename CharT=char, 
        typename Traits=std::char_traits<CharT>, 
        class Alloc=std::allocator<CharT>
    >
    class basic_time_formatter {
    public:
        //~ typedef std::string string_type;
        //~ typedef string_type::value_type char_type;
        //~ typedef std::ostream ostream_type;
    
        typedef std::basic_string<CharT,Traits,Alloc> string_type;
        typedef CharT char_type;
        typedef std::basic_ostream<CharT,Traits> ostream_type;

        static ostream_type &  default_os();
        static const char_type* default_format();
        static string_type format(const char_type* s) {
            string_type res(s);
            res += boost::chrono::detail::adaptive_string(" : ");
            res += default_format();
            //res += boost::chrono::detail::adaptive_string(" spent real %rs, cpu %cs (%p%), user %us, system %ss\n");
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
            typedef typename Stopwatch::duration duration;
            typedef typename duration::rep rep;
            duration d = stopwatch_.elapsed( ec );
            rep times=d.count();
            if ( times.real < 0 ) return;
            if ( places > 9 )
              places = 9;  // sanity check
            else if ( places < 0 )
              places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            nanoseconds total = nanoseconds(times.system + times.user);

            for ( ; *format; ++format )
            {
              if ( *format != '%' || !*(format+1) || !std::strchr("rcpus", *(format+1)) )
                os << *format;
              else
              {
                ++format;
                switch ( *format )
                {
                case 'r':
                  os << boost::chrono::duration<double>(nanoseconds(times.real)).count();
                  break;
                case 'u':
                  os << boost::chrono::duration<double>(nanoseconds(times.user)).count();
                  break;
                case 's':
                  os << boost::chrono::duration<double>(nanoseconds(times.system)).count();
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
                           /boost::chrono::duration<double>(nanoseconds(times.real)).count() * 100.0;
                    else
                      os << 0.0;
                  }
                  break;
                default:
                  assert(0 && "run_timer internal logic error");
                }
              }
            }
          }

    };

namespace detail {
    template <typename CharT>
    struct basic_time_formatter_default_format;
    template <>
    struct basic_time_formatter_default_format<char> {
        static const char* apply() {return BOOST_CHRONO_TIME_FORMAT_DEFAULT; }
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

    template <>
    struct stopwatch_reporter_default_formatter<stopwatch<process_cpu_clock> > {
        typedef time_formatter type;
    };

  } // namespace chrono
} // namespace boost

#define BOOST_CHRONO_TIME_FORMAT(F) boost::chrono::detail::adaptive_string(F " : " BOOST_CHRONO_TIME_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_CHRONO_TIME_FUNCTION_FORMAT boost::chrono::time_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_TIME_FUNCTION_FORMAT BOOST_CHRONO_TIME_FORMAT(BOOST_CURRENT_FUNCTION)
#endif

#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif // BOOST_CHRONO_TIME_FORMATTER_HPP
