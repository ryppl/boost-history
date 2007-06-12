//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#include "boost/act/action.hpp"
#include "boost/act/action/as_target.hpp"
#include "boost/act/action/as_function.hpp"
#include "boost/act/algorithm/for_each.hpp"
#include "boost/act/algorithm/copy.hpp"
#include "boost/act/algorithm/find.hpp"
#include "boost/act/algorithm/fill.hpp"
#include "boost/act/algorithm/count.hpp"
#include "boost/act/active.hpp"
#include "boost/act/active/queue_function.hpp"
#include "boost/act/active/utility.hpp"
#include "boost/act/active/active_interface.hpp"

#include "boost/act/common_act_models/immediate_act_model.hpp"
#include "boost/act/common_algo_models/parallel_algo_model.hpp"
#include "boost/act/common_act_models/concurrent_act_model.hpp"

#include "boost/act/active/queue_function.hpp"

#include <iostream>

#include "boost/act/atomic.hpp"

struct a
{
  int value;
  a()
    : value( 10 )
  {
  }

  a( a const& source )
    : value( source.value )
  {
    ::std::cout << "Copying a" << ::std::endl;
  }

  ~a()
  {
    ::std::cout << "Destroying a" << ::std::endl;
  }
};

struct b
{
  b( int )
  {
    ::std::cout << "Constructing b" << ::std::endl;
  }

  ~b()
  {
    ::std::cout << "Destroying b" << ::std::endl;
  }
};

namespace boost
{
namespace act
{

BOOST_ACT_ACTIVE_INTERFACE_SPEC( ::a )
{
public:
  BOOST_ACT_ACTIVE_MEM_FUN( (int), mem_test, ((int),left) ((int),right) )
  {
    inactive_this->value *= left + right;

    ::std::cout << "mem_test Called: " << inactive_this->value << ::std::endl;

    return inactive_this->value;
  }

  BOOST_ACT_ACTIVE_CONST_MEM_FUN( (int), mem_test2, ((),()) )
  {
    ::std::cout << "mem_test2 Called: " << inactive_this->value << ::std::endl;

    return inactive_this->value;
  }
};

}
}

struct a_function
{
  typedef void result_type;

  void operator ()( a* ) const
  {
    ::std::cout << "a_function called!" << ::std::endl;
  }
};

struct complex_function
  : ::boost::act::parallel_safe
{
  complex_function() {}

  void operator ()( int& val ) const
  {
    val = 15;
  }
};

int main()
{
  using namespace ::boost::act;

  BOOST_ATOMIC((::boost::iterator_difference< int* >::type)) atomic_int_test = 0;
  atomic_int_test += 10;

  ::std::cout << "Atomic value: " << value_of( atomic_int_test ) << ::std::endl;

  int array_test[10] = { 0 };

  // Loop splits up execution over several threads at runtime, then joins
  for_each[ parallel_algo_model() ]( array_test + 0, array_test + 10
                                   , complex_function()
                                   );

  int const calculated_count
    = count[ parallel_algo_model() ]( array_test + 0, array_test + 10
                                    , 15
                                    );

  ::std::cout << "Count: " << calculated_count << ::std::endl;

  int const val = 20;

  // Loop splits up execution over several threads at runtime, then joins
  fill[ parallel_algo_model() ]( array_test + 0, array_test + 10
                               , val
                               );

  // Runs for_each either serially or in parallel dependent on the default model
  action<> default_for_call( as_function( for_each() )
                           , array_test + 0, array_test + 10
                           , complex_function()
                           );

  // If the concurrent act model is used, the following my have varying results
  ::std::cout << array_test[0] << ::std::endl
              << array_test[1] << ::std::endl
              << array_test[2] << ::std::endl
              << array_test[3] << ::std::endl
              << array_test[4] << ::std::endl;

  // Forces the call to complete
  default_for_call.wait();

  // Reliable results
  ::std::cout << array_test[5] << ::std::endl
              << array_test[6] << ::std::endl
              << array_test[7] << ::std::endl
              << array_test[8] << ::std::endl
              << array_test[9] << ::std::endl;

  // Loop splits up execution over several threads at runtime, then joins
  copy[ parallel_algo_model() ]( array_test + 0, array_test + 10
                               , array_test + 0
                               );

  // Loop splits up execution over several threads at runtime, then joins
  find[ parallel_algo_model() ]( array_test + 0, array_test + 10
                               , val
                               );

  // Create an active qualified instance of a
  BOOST_ACTIVE((a)) active_test;

  // Call a_function in active_test's thread
  action<> test2( as_target( active_test )
                , a_function()
                );

  
  // Copy active_test asynchronously
  action< a > const test3 = active_test;

  test3.wait();

  // Call "mem_test" in active_test's thread (asynchronous to call-site)
  action< int > const mem_test = active_test.mem_test( 1, 4 );

  // Wait for above function to complete execution
  mem_test.wait();

  // queue a_function on active_test (same as test2 with different syntax)
  action<> wee = active_test.queue_function( a_function() );

  // wait for above function to complete
  wee.wait();

  // queue a_function on the result of the copy operation (test3) asynchronously
  test3->queue_function( a_function() );

  // call mem_test on the result of test3
  test3->mem_test( 2, 6 );

  // call mem_test2 on the result of test3
  test3->mem_test2();

  BOOST_ACTIVE_M((b),(concurrent_act_model)) concurrent_b( 4 );
}
