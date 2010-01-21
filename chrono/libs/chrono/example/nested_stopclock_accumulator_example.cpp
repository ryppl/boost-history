//  stopwatch_accumulator_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/stopwatches.hpp>
#include <cmath>
#include <boost/thread.hpp>

using namespace boost::chrono;
void f1()
{
    static stopclock_accumulator<> acc(stopwatch_accumulator_formatter::format(BOOST_STRINGIZE(__LINE__)));
    stopclock_accumulator<>::scoped_run _(acc);
    boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    {
        static stopclock_accumulator<> acc(stopwatch_accumulator_formatter::format(BOOST_STRINGIZE(__LINE__)));
        stopclock_accumulator<>::scoped_run _(acc);
        boost::this_thread::sleep(boost::posix_time::milliseconds(200));
    }

}
int main()
{
  static stopclock_accumulator<> acc(stopwatch_accumulator_formatter::format(BOOST_STRINGIZE(__LINE__)));
  stopclock_accumulator<>::scoped_run _(acc);

  f1();
  f1();
  f1();
  return 0;
}
