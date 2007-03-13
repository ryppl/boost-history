/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_SCOPED_CONCURRENT_CONSUMER_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_SCOPED_CONCURRENT_CONSUMER_HPP

#include <cstddef>

#include <boost/act/consumer_allocator/concurrent_consumer_allocator/concurrent_consumer_allocator.hpp>
#include <boost/act/consumer_allocator/concurrent_consumer_allocator/detail/scoped_concurrent_consumer_impl.hpp>

#include <boost/act/consumer_allocator/consumer_arguments.hpp>

namespace boost
{
namespace act
{

// ToDo: Template for buffer, not buffer size
//       Optimize away locking when possible (normalized type has trivial copy)
template< typename ThreadAllocator, ::std::size_t BufferSize >
template< BOOST_ACT_CONSUMER_TEMPLATE_PARAMS() >
class concurrent_consumer_allocator< ThreadAllocator, BufferSize >
::scoped_consumer
  : detail::scoped_concurrent_consumer_impl
    <
      ThreadAllocator
    , BufferSize
    , typename BOOST_ACT_CONSUMER_TRAITS()::processor_type
    , typename BOOST_ACT_CONSUMER_TRAITS()::restraint
    , typename BOOST_ACT_CONSUMER_TRAITS()::normalizer_type
    , typename BOOST_ACT_CONSUMER_TRAITS()::normalized_type
    >
{
public:
  BOOST_ACT_CONSUMER_TYPE_MEMBERS()
private:
  typedef detail::scoped_concurrent_consumer_impl
          <
            ThreadAllocator
          , BufferSize
          , typename BOOST_ACT_CONSUMER_TRAITS()::processor_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::restraint
          , typename BOOST_ACT_CONSUMER_TRAITS()::normalizer_type
          , typename BOOST_ACT_CONSUMER_TRAITS()::normalized_type
          >
          impl_base;
public:
  template< typename ParamType >
  struct tracker_from_param
    : impl_base::template tracker_from_param< ParamType > {};
public:
  explicit
  scoped_consumer( concurrent_consumer_allocator const& allocator_init )
    : impl_base( allocator_init ) {}

  explicit
  scoped_consumer( processor_type const& processor_init )
    : impl_base( processor_init ) {}

  scoped_consumer() {}
public:
  using impl_base::consume;
  using impl_base::tracked_consume;
};

}
}

#endif
