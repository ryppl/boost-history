//  boost/chrono/stopwatch_accumulator_time_formatter.hpp  ------------------------------------------------------------//

//  Copyright 2009-2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See http://www.boost.org/libs/system for documentation.

#ifndef BOOST_CHRONO_STOPWATCH_ACCUMULATOR_TIMES_FORMATTER_HPP
#define BOOST_CHRONO_STOPWATCH_ACCUMULATOR_TIMES_FORMATTER_HPP

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
#include <boost/chrono/time_formatter.hpp>

#include <boost/config/abi_prefix.hpp> // must be the last #include

#define BOOST_CHRONO_ACCUMULATOR_TIME_FORMAT_DEFAULT "%c times, sum=%ss, min=%ms, max=%Ms, mean=%as, frequency=%fHz, lifetime=%ls, percentage=%p%\n"


namespace boost { namespace chrono  {

//--------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------//

    template <
        typename CharT=char,
        typename Traits=std::char_traits<CharT>,
        class Alloc=std::allocator<CharT>
    >
    class basic_stopwatch_accumulator_time_formatter {
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
            duration_t lt= stopwatch_.lifetime();

            //if ( d < duration_t::zero() ) return;
            if ( places > 9 )
                places = 9;  // sanity check
            else if ( places < 0 )
                places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            const char_type* format2;
            for (format2=format ; *format2; ++format2 ) {
                if (*format2=='|') {++format2; break;}
            }

            for ( ; *format; ++format ) {
                if ( (*format != '%') || (!*(format+1)) || (!std::strchr("acflmMps", *(format+1))) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 's':
                        //~ os << boost::chrono::duration<double>(duration_t(accumulators::sum(acc))).count();
                        time_formatter::show_time<process_cpu_clock>(accumulators::sum(acc), format2, places, os, ec);
                        //~ os << accumulators::sum(acc);
                        break;
                    case 'm':
                        //~ os << boost::chrono::duration<double>(duration_t((accumulators::min)(acc))).count();
                        time_formatter::show_time<process_cpu_clock>((accumulators::min)(acc), format2, places, os, ec);
                        //~ os << (accumulators::min)(acc);
                        break;
                    case 'M':
                        //~ os << boost::chrono::duration<double>(duration_t((accumulators::max)(acc))).count();
                        time_formatter::show_time<process_cpu_clock>((accumulators::max)(acc), format2, places, os, ec);
                        //~ os << (accumulators::max)(acc);
                        break;
                    case 'a':
                        if (accumulators::count(acc)>0)
                         //? os << boost::chrono::duration<double>(duration_t(typename duration_t::rep(accumulators::mean(acc)))).count()
                                //~ os << boost::chrono::duration<double>(duration_t(accumulators::sum(acc))).count() / accumulators::count(acc)
                            os << accumulators::sum(acc) / accumulators::count(acc);
                        else
                            os << 0;
                        break;
                    case 'c':
                        os << accumulators::count(acc);
                        break;
                    case 'f':
                        if (accumulators::count(acc)>0)
                            //~ os << accumulators::count(acc)/boost::chrono::duration<double>(lt);
                            os << accumulators::count(acc) << "/" << lt.count();
                        else
                            os <<  0;
                        break;
                    case 'l':
                        os << lt.count();
                        break;
                    case 'p':
                        //~ os << int(boost::chrono::duration<double>(duration_t(accumulators::sum(acc))).count()*100/boost::chrono::duration<double>(lt).count());
                        os << accumulators::sum(acc)*100 << "/" << lt.count();
                        break;
                    default:
                        assert(0 && "basic_stopwatch_accumulator_time_formatter internal logic error");
                    }
                }
            }
        }
    };

namespace detail {
    template <typename CharT>
    struct basic_stopwatch_accumulator_time_formatter_default_format;
    template <>
    struct basic_stopwatch_accumulator_time_formatter_default_format<char> {
        static const char* apply() {return BOOST_CHRONO_ACCUMULATOR_TIME_FORMAT_DEFAULT; }
    };
#ifndef BOOST_NO_STD_WSTRING
    template <>
    struct basic_stopwatch_accumulator_time_formatter_default_format<wchar_t> {
        static const wchar_t* apply() {return L"%c times, sum=%ss, min=%ms, max=%Ms, mean=%as, frequency=%fHz, lifetime=%ls, percentage=%p%\n"; }
    };

#endif
}

    template <typename CharT,typename Traits, class Alloc>
    const typename basic_stopwatch_accumulator_time_formatter<CharT,Traits,Alloc>::char_type*
    basic_stopwatch_accumulator_time_formatter<CharT,Traits,Alloc>::default_format() {
        return detail::basic_stopwatch_accumulator_time_formatter_default_format<CharT>::apply();
    }

    template <typename CharT,typename Traits, class Alloc>
    typename basic_stopwatch_accumulator_time_formatter<CharT,Traits,Alloc>::ostream_type &
    basic_stopwatch_accumulator_time_formatter<CharT,Traits,Alloc>::default_os()  { return detail::default_out<CharT,Traits>::apply(); }

    typedef basic_stopwatch_accumulator_time_formatter<char> stopwatch_accumulator_time_formatter;
    typedef basic_stopwatch_accumulator_time_formatter<wchar_t> wstopwatch_accumulator_time_formatter;




} // namespace chrono
} // namespace boost

#define BOOST_CHRONO_ACCUMULATOR_TIME_FORMAT(F) boost::chrono::detail::adaptive_string(F " : " BOOST_CHRONO_ACCUMULATOR_FORMAT_DEFAULT)
#ifdef __GNUC__
#define BOOST_CHRONO_ACCUMULATOR_TIME_FUNCTION_FORMAT boost::chrono::stopwatch_accumulator_time_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_ACCUMULATOR_TIME_FUNCTION_FORMAT BOOST_CHRONO_ACCUMULATOR_TIME_FORMAT(BOOST_CURRENT_FUNCTION)
#endif


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
