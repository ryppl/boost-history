/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#include <boost/detail/lightweight_test.hpp>

#include <boost/act/consumer_allocator/immediate_consumer_allocator.hpp>

struct test_fun
{
  test_fun( int& value_init ) : value( value_init ) {}

  void operator ()() const
  {
    ++value;
  }

  int& value;
};

void test_scoped_consumer_allocator()
{
  using namespace boost::act;

  typedef concurrent_consumer_allocator<> allocator_type;
  typedef allocator_type::scoped_consumer< test_fun
                                         , execute
                                         >
                                         scoped_consumer_type;

  int test_value = 0;

  scoped_consumer_type consumer;

  consumer.consume( test_fun( test_value ) );
  consumer.consume( test_fun( test_value ) );
  consumer.consume( test_fun( test_value ) );
  consumer.consume( test_fun( test_value ) );

  if( test_value != 4 )
    BOOST_ERROR( "immediate_consumer_allocator general execution failure." );
}

int main()
{
  test_scoped_consumer_allocator();

  return boost::report_errors();
}
