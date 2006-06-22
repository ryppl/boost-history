/* boost random_test.cpp various tests
 *
 * Copyright Jens Maurer 2000
 * Distributed under the Boost Software License, Version 1.0. (See
 * accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * $Id$
 */

#if defined(BOOST_MSVC) && BOOST_MSVC <= 1300
#pragma warning( disable : 4786 )
#endif

#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <iterator>
#include <vector>
#include <boost/random.hpp>
#include <boost/config.hpp>
#include <boost/random/parallel.hpp>
#include <boost/random/sprng.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/test/included/test_exec_monitor.hpp>


template<class PRNG>
void test(const std::string & name, const PRNG &)
{
  using namespace boost::random;
  std::cout << "Testing " << name << ": ";
  PRNG rng;  // default ctor
  for(int i = 0; i < 10000; i++)
    rng();
  typename PRNG::result_type val = rng();
  
  PRNG rng2(stream_number=0, global_seed=0, total_streams=1);
  for(int i = 0; i < 10000; i++)
    rng2();
  typename PRNG::result_type val2 = rng2();

  PRNG rng3(stream_number=1, total_streams=2);
  for(int i = 0; i < 10000; i++)
    rng3();
  typename PRNG::result_type val3 = rng3();

  PRNG rng4;
  parallel::seed(rng4,1,2,0);
  for(int i = 0; i < 10000; i++)
    rng4();
  typename PRNG::result_type val4 = rng4();
  
  PRNG rng5;
  std::vector<unsigned int> buffer(1,0);
  std::vector<unsigned int>::iterator it=buffer.begin();
  parallel::seed(rng5,1,2,it,buffer.end());
  for(int i = 0; i < 10000; i++)
    rng5();
  typename PRNG::result_type val5 = rng5();

  bool result = (val==val2) && (val != val3) && (val3==val4) && 
                (val4==val5);
  std::cout << val << " " << val2 << " " << val3 << " " << val4 <<  std::endl;
  BOOST_CHECK(result);
}

void test_all()
{
  test("sprng::lfg", boost::random::sprng::lfg());
  test("sprng::lcg", boost::random::sprng::lcg());
  test("sprng::lcg64", boost::random::sprng::lcg64());
  test("sprng::cmrg", boost::random::sprng::cmrg());
  test("sprng::mlfg", boost::random::sprng::mlfg());
//  test("sprng::pmlcg", boost::random::sprng::pmlcg());
}


int test_main(int, char*[])
{

  test_all();
  return 0;
}
