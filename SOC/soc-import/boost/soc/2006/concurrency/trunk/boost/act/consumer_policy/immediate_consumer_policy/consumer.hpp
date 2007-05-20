/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_IMMEDIATE_CONSUMER_CONSUMER_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_IMMEDIATE_CONSUMER_CONSUMER_HPP

#include <boost/act/consumer_allocator/immediate_consumer_allocator/immediate_consumer_allocator.hpp>

namespace boost
{
namespace act
{

template< typename ValueType, typename Process >
class immediate_consumer_allocator::consumer
{
public:
  typedef ValueType value_type;
public:
  void consume( value_type data )
  {
    process_m( data );
  }
private:
  Process process_m;
};

}
}

#endif
