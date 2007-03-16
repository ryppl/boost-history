/* boost random_test.cpp various tests
 *
 * Copyright Matthias Troyer 2006
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#include <boost/random/buffered_generator.hpp>
#include <boost/random.hpp>
#include <iostream>

// A simple example simulation - usually it will be much more complex
double simulate(boost::buffered_generator<double>& gen)
{
  double sum=0;
  for (int i=0;i<100000;i++)
    sum += gen();
  return sum;
}

// create a buffered_generator
template <class RNG>
void simulate_it()
{
  typedef boost::variate_generator<RNG&,boost::normal_distribution<> > gen_type;
  RNG engine;
  boost::basic_buffered_generator<gen_type,double> 
    gen(gen_type(engine,boost::normal_distribution<>()));
    
  std::cout << simulate(gen) << "\n";
}

// call the simulation with two different generators
int main()
{
  simulate_it<boost::mt11213b>();
  simulate_it<boost::mt19937>();
}
