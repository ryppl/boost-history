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
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/accumulators/framework/accumulator_set.hpp>
#include <boost/accumulators/statistics/sum.hpp>
#include <boost/accumulators/statistics/min.hpp>
#include <boost/accumulators/statistics/max.hpp>
#include <boost/accumulators/statistics/mean.hpp>
//#include <boost/chrono/stopwatch_accumulator.hpp>
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

    template <
        typename CharT=char, 
        typename Traits=std::char_traits<CharT>, 
        class Alloc=std::allocator<CharT>
    >
    class basic_stopwatch_accumulator_formatter {
    public:
        //~ typedef std::string string_type;
        //~ typedef string_type::value_type char_type;
        //~ typedef std::ostream ostream_type;
    
        typedef std::basic_string<CharT,Traits,Alloc> string_type;
        typedef CharT char_type;
        typedef std::basic_ostream<CharT,Traits> ostream_type;
    
        static ostream_type &  default_os();
        static const int m_default_places = 3;
        static const char_type* m_default_format;
        static const char_type* default_format() { return m_default_format; }
        static string_type format(const char_type* s) {
            string_type res(s);
            res += " called %c times, sum=%ss, min=%ms, max=%Ms, mean=%as\n";
            return res;
        }
        static int default_places() { return m_default_places; }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_, const char_type* format, int places, ostream_type & os, system::error_code & ec)
        //  NOTE WELL: Will truncate least-significant digits to LDBL_DIG, which may
        //  be as low as 10, although will be 15 for many common platforms.
        {
            typedef typename Stopwatch::accumulator accumulator;
            typedef typename Stopwatch::duration duration;
            accumulator& acc = stopwatch_.accumulated();

            //if ( d < duration(0) ) return;
            if ( places > 9 )
                places = 9;  // sanity check
            else if ( places < 0 )
                places = 0;

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            os << "***" ;
            for ( ; *format; ++format ) {
                if ( *format != '%' || !*(format+1) || !std::strchr("smMac", *(format+1)) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 's':
                        os << boost::chrono::duration<double>(duration(accumulators::sum(acc))).count();
                        break;
                    case 'm':
                        os << boost::chrono::duration<double>(duration((accumulators::min)(acc))).count();
                        break;
                    case 'M':
                        os << boost::chrono::duration<double>(duration((accumulators::max)(acc))).count();
                        break;
                    case 'a':
                        os << ((accumulators::count(acc)>0)
                                ? (boost::chrono::duration<double>(duration(accumulators::sum(acc))).count())/accumulators::count(acc)
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

    template <typename CharT,typename Traits, class Alloc>
    const typename basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::char_type* 
    basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::m_default_format ="%c times, sum=%ss, min=%ms, max=%Ms, mean=%as\n";

    template <typename CharT,typename Traits, class Alloc>
    typename basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::ostream_type &  
    basic_stopwatch_accumulator_formatter<CharT,Traits,Alloc>::default_os()  { return detail::default_out<CharT,Traits>::apply(); }

    typedef basic_stopwatch_accumulator_formatter<char> stopwatch_accumulator_formatter;
    typedef basic_stopwatch_accumulator_formatter<wchar_t> wstopwatch_accumulator_formatter;
    
} // namespace chrono
} // namespace boost

#define BOOST_CHRONO_ACCUMULATOR_FORMAT(F) F" called %c times, sum=%ss, min=%ms, max=%Ms, mean=%as\n"
#ifdef __GNUC__
#define BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT boost::chrono::stopwatch_accumulator_formatter::format(BOOST_CURRENT_FUNCTION)
#else
#define BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT BOOST_CHRONO_ACCUMULATOR_FORMAT(BOOST_CURRENT_FUNCTION)
#endif


#include <boost/config/abi_suffix.hpp> // pops abi_prefix.hpp pragmas

#endif
