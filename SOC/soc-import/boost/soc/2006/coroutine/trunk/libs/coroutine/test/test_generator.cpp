//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <vector>
#include <iterator>
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/coroutine/generator.hpp>
#include <boost/test/unit_test.hpp>

namespace coroutines = boost::coroutines;
using coroutines::generator;

typedef generator<int> generator_type;

int count(generator_type::self& self, int rmin, int rmax) {
  while(rmin < rmax -1) {
    self.yield(rmin++);
  }
  return (rmin);
}

int count2(generator_type::self& self, int rmin, int rmax) {
  while(rmin < rmax) {
    self.yield(rmin++);
  }
  self.exit();
}


void test_generator() {
  {
    generator_type gen(boost::bind(count, _1, 10, 20));
    std::vector<int> vec;
    std::copy(gen, generator_type(), std::back_inserter(vec));
    BOOST_CHECK(vec.size() == 10);
  }
  {
    generator_type gen(boost::bind(count2, _1, 10, 20));
    std::vector<int> vec;
    std::copy(gen, generator_type(), std::back_inserter(vec));
    BOOST_CHECK(vec.size() == 10);
  }
}

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("generator test");

    test->add(BOOST_TEST_CASE(&test_generator));

    return test;
}
