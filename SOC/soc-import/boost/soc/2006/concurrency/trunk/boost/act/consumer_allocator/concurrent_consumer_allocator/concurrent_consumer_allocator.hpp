/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_ALLOC_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_ALLOC_HPP

#include <cstddef>

#include <boost/act/consumer_allocator/concurrent_consumer_allocator/concurrent_consumer_allocator_fwd.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/always.hpp>

#include <boost/act/consumer_allocator/consumer_arguments.hpp>

namespace boost
{
namespace act
{

// ToDo: Make default BufferSize toggleable
template< typename ThreadAllocator, ::std::size_t BufferSize >
class concurrent_consumer_allocator
{
public:
  typedef ThreadAllocator thread_allocator;
  static ::std::size_t const buffer_size_c = BufferSize;
public:/*
  template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL() >
  class consumer;
*/
  template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL() >
  class scoped_consumer;
/*
  template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL() >
  class safe_consumer;

  template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS_DECL() >
  class safe_scoped_consumer;*/
public:
  template< typename ValueType = void >
  class tracker;
/*
  template< typename ValueType = void >
  class safe_tracker;*/
};

}
}

#endif
