//  stopclock_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/stopclock.hpp>
#include <cmath>

using namespace boost::chrono;
int f1(long j)
{
  stopclock<> _(BOOST_CHRONO_TIME_FUNCTION_FORMAT);   

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  stopclock<> _(BOOST_CHRONO_TIME_FUNCTION_FORMAT);   

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}
