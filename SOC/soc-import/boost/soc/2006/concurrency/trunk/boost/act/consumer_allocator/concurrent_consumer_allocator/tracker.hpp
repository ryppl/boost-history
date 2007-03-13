/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_TRACKER_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_TRACKER_HPP

#include <boost/act/consumer_allocator/concurrent_consumer_allocator/detail/concurrent_tracking_info.hpp>
#include <boost/act/consumer_allocator/concurrent_consumer_allocator/concurrent_consumer_allocator.hpp>
#include <boost/act/detail/unspecified_bool.hpp>

#include <cstddef>

#include <boost/shared_ptr.hpp>

namespace boost
{
namespace act
{

// ToDo: Template for buffer type
template< typename ThreadAllocator, ::std::size_t BufferSize >
template< typename ValueType >
class concurrent_consumer_allocator< ThreadAllocator, BufferSize >::tracker
{
  struct unspecified_bool_tag;
public:
  typedef ValueType value_type;
private:
  typedef detail::unknown_concurrent_tracking_info unknown_tracking_info;
  typedef detail::concurrent_tracking_info< value_type > tracking_info;
public:
  tracker() {}/*
  explicit tracker( signaller const& source )
    : tracking_info_m( source.tracking_info_m ) {}
*/
// private:
  explicit tracker
  ( shared_ptr< unknown_tracking_info > const& tracking_info_init )
    : tracking_info_m
      ( static_pointer_cast< tracking_info >( tracking_info_init ) ) {}
public:
  bool is_signalled() const
  {
    return tracking_info_m->is_signalled();
  }

  void wait() const
  {
    tracking_info_m->wait();
  }

  value_type value() const
  {
    return tracking_info_m->wait_for_value();
  }
public:
  operator BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_T( (unspecified_bool_tag) )() const
  {
    return BOOST_ACT_DETAIL_UNSPECIFIED_BOOL_VALUE_T( (unspecified_bool_tag)
                                                    , is_signalled()
                                                    );
  }
private:
  shared_ptr< tracking_info > tracking_info_m;
};

}
}

#endif
