//  stopwatch_accumulator_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopwatches.hpp>
#include <cmath>
#include <boost/accumulators/statistics/variance.hpp>

using namespace boost::chrono;
using namespace boost::accumulators;

    class my_stopwatch_accumulator_formatter {
    public:
        typedef std::string string_type;
        typedef char char_type;
        typedef std::ostream ostream_type;

        static ostream_type &  default_os() {return std::cout;}
        static const char_type* default_format() {return "%c times, sum=%ss, mean=%as, variance=%vs\n";}
        static int default_places() { return 3; }

        template <class Stopwatch >
        static void show_time( Stopwatch & stopwatch_, const char_type* format,
                                int places, ostream_type & os, boost::system::error_code & ec)
        {
            typedef typename Stopwatch::accumulator accumulator_t;
            typedef typename Stopwatch::duration duration_t;
            accumulator_t& acc = stopwatch_.accumulated();

            boost::io::ios_flags_saver ifs( os );
            os.setf( std::ios_base::fixed, std::ios_base::floatfield );
            boost::io::ios_precision_saver ips( os );
            os.precision( places );

            for ( ; *format; ++format ) {
                if ( *format != '%' || !*(format+1) || !std::strchr("acsv", *(format+1)) ) {
                    os << *format;
                } else {
                    ++format;
                    switch ( *format ) {
                    case 's':
                        os << boost::chrono::duration<double>(duration_t(sum(acc))).count();
                        break;
                    case 'a':
                        os << boost::chrono::duration<double>(duration_t(typename duration_t::rep(mean(acc)))).count();
                        break;
                    case 'c':
                        os << count(acc);
                        break;
                    case 'v':
                        os << boost::chrono::duration<double>(duration_t(typename duration_t::rep(variance(acc)))).count();
                        break;
                    default:
                        assert(0 && "my_stopwatch_accumulator_formatter internal logic error");
                    }
                }
            }
        }
    };


typedef stopwatch_reporter<
        stopwatch_accumulator<process_real_cpu_clock,
            accumulator_set<process_real_cpu_clock::rep,
                features<
                        tag::count,
                        tag::sum,
                        tag::mean,
                        tag::variance(lazy)
                >
            >
        >
        , my_stopwatch_accumulator_formatter
    > my_stopwatch_accumulator_reporter;

int f1(long j)
{
  //static my_stopwatch_accumulator_reporter acc(BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT);
  static my_stopwatch_accumulator_reporter acc;
  my_stopwatch_accumulator_reporter::scoped_run _(acc);

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  static my_stopwatch_accumulator_reporter acc;

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}

