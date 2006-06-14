//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <iostream>
#include <boost/coroutine/coroutine.hpp>

namespace coroutines = boost::coroutines;
using coroutines::coroutine;

 
struct my_result {};
struct my_parm {};

typedef coroutine<my_result(my_parm)> coroutine_type;
typedef coroutine<void()> coroutine_vv_type;
typedef coroutine<int()> coroutine_iv_type;
typedef coroutine<void(int)> coroutine_vi_type;

typedef boost::tuple<my_result, my_result> result_tuple;
typedef boost::coroutines::tuple<my_result, my_result> result_tuple_tag;
typedef coroutine<result_tuple_tag (my_parm)> coroutine_tuple_type;

typedef boost::tuple<my_parm, my_parm> parm_tuple;
typedef coroutine<my_result(my_parm, my_parm)> coroutine_tuple2_type;

my_result foo(coroutine_type& self, my_parm parm) {
  int i = 10;
  my_result t;
  while(--i) {
    std::cout<<i <<", in coroutine, yielding\n";
    parm = self.yield(t);
  }
  std::cout<<i <<", in coroutine, exiting\n";
  return t;
}

struct foo_functor {
  typedef my_result result_type;
  my_result operator()(coroutine_type& self, my_parm parm) {
    return foo(self, parm);
  }
};


int bar(coroutine_iv_type& self) {
  self.yield(0);
  return 0;
}

result_tuple baz(coroutine_tuple_type& self, my_parm) {
  self.yield(my_result(), my_result()); 
  return boost::make_tuple(my_result(), my_result());
}

my_result barf(coroutine_tuple2_type& self, my_parm a, my_parm b) {
  boost::tie(a, b) = self.yield(my_result());
  return my_result();
}

void vi(coroutine_vi_type& self, int i) {
  i = self.yield();
}

void vv(coroutine_vv_type& self) {
  self.yield();
}

typedef coroutine<void(int&)> coroutine_ref_type;
void ref(coroutine_ref_type& self, int& x) {
  x = 10;
  self.yield();
}

int main() {
  coroutine <my_result(my_parm)> empty;
  coroutine <my_result(my_parm)> coro(foo);
  BOOST_ASSERT(!empty);
  empty = coro;
  BOOST_ASSERT(empty);

  coroutine_type coro_functor = coroutine_type(foo_functor());
  my_parm t;

  /* Void parameters are supported */
  coroutine_iv_type iv_coro(bar);
  /* Void results are supported */
 coroutine_vi_type vi_coro (vi);
  /* Void values and parameters are supported*/  
  coroutine_vv_type void_coro (vv); 
  /* Tuple result types are supported */
  coroutine_tuple_type tuple_coro(baz);
  /* Variable arity coroutines are supported */
  coroutine_tuple2_type tuple2_coro(barf);
  tuple2_coro(my_parm(), my_parm());
  /* references are supported */
  coroutine_ref_type ref_coro(ref);
  int x = 0;
  ref_coro(x);
  BOOST_ASSERT(x == 10);

  while(coro && coro_functor) {
    std::cout<<"in main, calling coro\n";
    coro(t);
    std::cout<<"in main, calling coro_functor\n";
    coro_functor(t);
    std::cout<<"in main!\n";
  }
  BOOST_ASSERT(!(coro && coro_functor));
  std::cout<<"test finished\n";
}

