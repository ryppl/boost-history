//  stopwatch_accumulator_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopclock_accumulator.hpp>
//#include <boost/chrono/stopwatches.hpp>

#include <cmath>
//#include <boost/thread.hpp>
#include "sleep_for.hpp"

using namespace boost::chrono;
int f1(long j)
{
  static stopclock_accumulator<> acc(BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT);
  stopclock_accumulator<>::scoped_run _(acc);

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time
  boost::this_thread::sleep_for(boost::chrono::milliseconds(20));
  

  return 0;
}
int main()
{
  static stopclock_accumulator<> acc(BOOST_CHRONO_ACCUMULATOR_FUNCTION_FORMAT);
  stopclock_accumulator<>::scoped_run _(acc);

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}