//  stopwatch_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopwatch.hpp>
#include <cmath>
//#include <boost/thread.hpp>
#include "sleep_for.hpp"

using namespace boost::chrono;
double res;
void f1(long j)
{
    stopwatch<>::reporter _(BOOST_CHRONO_STOPWATCH_FUNCTION_FORMAT);
    for (long i =0; i< j; i+=1)
        res+=std::sqrt( res+123.456L+i );  // burn some time
    if (j!=0) f1(j-1);
    stopwatch<>::reporter::scoped_suspend s(_);
    boost::this_thread::sleep_for(boost::chrono::milliseconds(10));

}
int main()
{
  stopwatch<>::reporter _(BOOST_CHRONO_STOPWATCH_FUNCTION_FORMAT);

    res=0;
    for (long i =0; i< 3; ++i)
        f1(i*100);

    std::cout<< res << std::endl;
  return 0;
}
