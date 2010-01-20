//  stopclock_example.cpp  ---------------------------------------------------//

//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org/libs/chrono for documentation.

#include <boost/chrono/scoped_stopclock.hpp>
#include <cmath>

using namespace boost::chrono;
int f1(long j)
{
  scoped_stopclock<> _(BOOST_CURRENT_FUNCTION);   

  for ( long i = 0; i < j; ++i )
    std::sqrt( 123.456L );  // burn some time

  return 0;
}
int main()
{
  scoped_stopclock<> _(BOOST_CURRENT_FUNCTION);   

  f1(100000);
  f1(200000);
  f1(300000);
  return 0;
}
