/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_SIGNALLER_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_CONCURRENT_CONSUMER_ALLOCATOR_SIGNALLER_HPP

#include <boost/act/consumer_allocator/concurrent_consumer_allocator/detail/concurrent_tracking_info.hpp>
#include <boost/act/consumer_allocator/concurrent_consumer_allocator/concurrent_consumer_allocator.hpp>

#include <boost/intrusive_ptr.hpp>

#include <boost/noncopyable.hpp>

#include <cstddef>

#include <boost/

namespace boost
{
namespace act
{

// ToDo: Handle void types
template< typename ThreadAllocator, ::std::size_t BufferSize >
template< typename ValueType >
class concurrent_consumer_allocator< ThreadAllocator, BufferSize >::signaller
  : noncopyable
{
  friend class tracker< value_type >;
public:
  typedef ValueType value_type;
private:
  typedef detail::concurrent_tracking_info< value_type > tracking_info;
public:
  signaller() : tracking_info_m( new tracking_info ) {}
public:
  template< typename InPlaceFactory >
  void signal( InPlaceFactory const& factory )
  {
    tracking_info_m->signal( factory );
  }

  void signal( value_type const value_init )
  {
    tracking_info_m->signal( value_init );
  }
public:
  tracker< value_type > obtain_tracker() const
  {
    return tracker< value_type >( *this );
  }
public:
  bool is_signalled() const
  {
    return tracking_info_m->is_signalled();
  }
private:
  intrusive_ptr< tracking_info > const tracking_info_m;
};

}
}

#endif
