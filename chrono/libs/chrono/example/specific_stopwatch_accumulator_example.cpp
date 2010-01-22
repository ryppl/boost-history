//  stopwatch_accumulator_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopwatches.hpp>
#include <cmath>

using namespace boost::chrono;
using namespace boost::accumulators;
typedef stopwatch_accumulator<process_real_cpu_clock, kind::running,
            accumulator_set<process_real_cpu_clock::rep,
                features<
                        tag::count,
                        tag::sum,
                        tag::min,
                        tag::max,
                        tag::mean 
                >
            >
  >::reporter my_stopwatch_accumulator_reporter;

int f1(long j)
{
  static my_stopwatch_accumulator_reporter acc(BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT);
  my_stopwatch_accumulator_reporter::scoped_run _(acc);

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}

