//  boost/chrono/stopwatch_accumulator_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_ACCUMULATOR_FORMATTER_HPP
#define BOOST_CHRONO_STOPWATCH_ACCUMULATOR_FORMATTER_HPP

#include <boost/chrono/chrono.hpp>
#include <boost/system/error_code.hpp>
#include <boost/current_function.hpp>
#include <boost/chrono/detail/default_out.hpp>
#include <boost/chrono/detail/adaptive_string.hpp>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/cstdint.hpp>
#include <string>
#include <iostream>
#include <boost/io/ios_state.hpp>
#include <cstring>
#include <cassert>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#define BOOST_CHRONO_ACCUMULATOR_FORMAT_DEFAULT "%c times, sum=%ss, min=%ms, max=%Ms, mean=%as\n"


namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

    template <
        typename CharT=char, 
        typename Traits=std::char_traits<CharT>, 
        class Alloc=std::allocator<CharT>
    >
    class basic_stopwatch_accumulator_formatter {
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
            return res;
        }
        static int default_places() { return 3; }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_, const char_type* format, int places, ostream_type & os, system::error_code & ec)
        //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
        //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::accumulator accumulator;
            typedef typename Stopwatch::duration duration_t;
            accumulator& acc = stopwatch_.accumulated();
            //duration_t d0((0));

            //if ( d < d0 ) return;
            if ( places > 9 )
                places = 9;  // sanity check
            else if ( places < 0 )
                places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            for ( ; *format; ++format ) {
                if ( *format != '%' || !*(format+1) || !std::strchr("smMac", *(format+1)) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 's':
                        os << boost::chrono::duration<double>(duration_t(accumulators::sum(acc))).count();
                        break;
                    case 'm':
                        os << boost::chrono::duration<double>(duration_t((accumulators::min)(acc))).count();
                        break;
                    case 'M':
                        os << boost::chrono::duration<double>(duration_t((accumulators::max)(acc))).count();
                        break;
                    case 'a':
                        os << ((accumulators::count(acc)>0)
                                ? (boost::chrono::duration<double>(duration_t(accumulators::sum(acc))).count())/accumulators::count(acc)
                                : 0);
                        break;
                    case 'c':
                        os << accumulators::count(acc);
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
    struct basic_stopwatch_accumulator_formatter_default_format;
    template <>
    struct basic_stopwatch_accumulator_formatter_default_format<char> {
        static const char* apply() {return BOOST_CHRONO_ACCUMULATOR_FORMAT_DEFAULT; }
    };
#ifndef BOOST_NO_STD_WSTRING
    template <>
    struct basic_stopwatch_accumulator_formatter_default_format<wchar_t> {
        static const wchar_t* apply() {return L"%c times, sum=%ss, min=%ms, max=%Ms, mean=%as\n"; }
    };
    
#endif    
}
       
    template <typename CharT,typename Traits, class Alloc>
    const typename basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::char_type* 
    basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::default_format() {
        return detail::basic_stopwatch_accumulator_formatter_default_format<CharT>::apply();
    }

    template <typename CharT,typename Traits, class Alloc>
    typename basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::ostream_type &  
    basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::default_os()  { return detail::default_out<CharT,Traits>::apply(); }

    typedef basic_stopwatch_accumulator_formatter<char> stopwatch_accumulator_formatter;
    typedef basic_stopwatch_accumulator_formatter<wchar_t> wstopwatch_accumulator_formatter;
    
} // namespace chrono
} // namespace boost

#define BOOST_CHRONO_ACCUMULATOR_FORMAT(F) boost::chrono::detail::adaptive_string(F " : " BOOST_CHRONO_ACCUMULATOR_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT boost::chrono::stopwatch_accumulator_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT BOOST_CHRONO_ACCUMULATOR_FORMAT(BOOST_CURRENT_FUNCTION)
#endif


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
