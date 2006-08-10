//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/coroutine/coroutine.hpp>
#include <boost/test/unit_test.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

//
// Test coroutine capability to handle non default constructible
// types both as result type and as arguments.

class ndc {
public:
  explicit 
  ndc(int) {}

  ndc(const ndc&) {}

  ndc& operator=(const ndc&) {
    return *this;
  }
private:
};

typedef coroutine<ndc(ndc)> coroutine_type;

ndc body(coroutine_type::self& self, ndc x) {
  self.yield(x);  
  self.yield(x);  
  return ndc (0);
}

void test_ndc() {
  coroutine_type coro(body);
  ndc result (coro(ndc(0)));
  result = (coro(result));
}


boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("non-default-constructible-types coroutine test");

    test->add(BOOST_TEST_CASE(&test_ndc));

    return test;
}
