//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <boost/test/unit_test.hpp>
#include <boost/ref.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#include <boost/none.hpp>
#include <boost/function.hpp>
#include <boost/coroutine/coroutine.hpp>
#include <boost/coroutine/future.hpp>


namespace coroutines = boost::coroutines;
using coroutines::coroutine;

typedef coroutine<void()> coroutine_type;

class a_pipe {
public:
  
  void send(int x) {
    m_callback (x);
  }

  template<typename Callback>
  void listen(Callback c) {
    m_callback = c;
  }
private:
  boost::function<void(int)> m_callback;
};


void coro_body(a_pipe& my_pipe, coroutine_type self)  {
  coroutines::future<int> future;
  //= coroutines::call(boost::bind(&a_pipe::listen, my_pipe, _1), self);
  my_pipe.listen(coroutines::make_callback(future, self));
  coroutines::wait(self, future);
  BOOST_CHECK(future);
  BOOST_CHECK(*future == 1);
  future = boost::none;
  coroutines::wait(self, future);
  BOOST_CHECK(*future == 2);
  future = boost::none;
  coroutines::wait(self, future);
  BOOST_CHECK(*future == 3);
  future = boost::none;
  return;  
}

void test_call() {
  a_pipe my_pipe;
  coroutine_type coro(boost::bind(coro_body, boost::ref(my_pipe), _1));
  coro();
  my_pipe.send(1);
  my_pipe.send(2);
  my_pipe.send(3);
		      }

boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    boost::unit_test::test_suite *test = BOOST_TEST_SUITE("call coroutine test");

    test->add(BOOST_TEST_CASE(&test_call));

    return test;
}
