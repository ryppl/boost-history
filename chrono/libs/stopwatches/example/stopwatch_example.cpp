//  stopwatch_example.cpp  ---------------------------------------------------//

//  Copyright Beman Dawes 2006, 2008
//  Copyright 2009/2010 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <iostream>
#include <boost/stopwatches/stopwatches.hpp>
#include <cmath>

using namespace boost::chrono;
using namespace boost::stopwatches;
int f1(long j)
{
  stopwatch_reporter<stopwatch<> > _(BOOST_STOPWATCHES_STOPWATCH_FUNCTION_FORMAT);

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  stopwatch_reporter<stopwatch<> > _(BOOST_STOPWATCHES_STOPWATCH_FUNCTION_FORMAT);

  f1(1000);
  f1(2000);
  f1(3000);
  return 0;
}
