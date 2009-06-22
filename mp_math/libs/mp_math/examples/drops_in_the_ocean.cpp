// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// calculate the number of water drops in earth's oceans

#include <boost/timer.hpp>
#include <boost/mp_math/integer.hpp>
#include <boost/mp_math/integer/gmp_integer.hpp>

typedef boost::mp_math::integer<> int_type;


int main(int, char**)
{
  // assume that the oceans have a volume of 1.3 billion km^3
  // assume that one drop has a volume of 0.05 ml
  const unsigned long ocean_vol = 1300000000UL;
  const unsigned long drops_per_ml = 20;

  // there are 1000 ml in 1 liter and 1 liter = 1 dm^3

  int_type num_drops = 1; // start with one drop

  num_drops *= drops_per_ml; // now we have the number of drops per ml
  num_drops *= 1000;         // now we have the number of drops per liter
  num_drops *= 1000;         // 1  m^3 = 1000 dm^ 3
  num_drops *= 1000000000UL; // 1 km^3 = 1000000000 m^3
  num_drops *= ocean_vol;

  std::cout << "Earth's oceans consist of " << num_drops << " water drops!"
            << std::endl;


  int_type counter = num_drops;

  boost::timer t;
  while (t.elapsed() < 2.0)
    --counter;

  //counter /= 2; // FIXME!! this makes a larger number out of counter
  //c1 = 25999999999999999995604718
  //c2 = 26000000005912711854358254

  const int_type difference = (num_drops - counter) / 2;
  int_type years = num_drops / difference;

  years /= 60;
  years /= 60;
  years /= 24;
  years /= 365;

  std::cout << "This computer can count about " << difference << " water drops "
            << "per second.\n"
            << "Counting all water drops would take about " << years
            << " years on this computer." << std::endl;

  return 0;
}



