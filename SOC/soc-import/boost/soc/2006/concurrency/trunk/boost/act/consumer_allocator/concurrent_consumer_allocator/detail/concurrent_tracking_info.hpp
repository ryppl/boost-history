/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_ALLOCATOR_DETAIL_CONCURRENT_TRACKING_INFO_HPP
#define BOOST_ACT_CONSUMER_ALLOCATOR_DETAIL_CONCURRENT_TRACKING_INFO_HPP

#include <boost/act/consumer_allocator/concurrent_consumer_allocator/concurrent_consumer_allocator.hpp>

#include <cstddef>
#include <new>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/noncopyable.hpp>

#include <boost/act/detail/raw_storage.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost
{
namespace act
{
namespace detail
{

template< typename ValueType >
class concurrent_tracking_info;

// ToDo: Use function pointer instead of virtual function
class unknown_concurrent_tracking_info
{/*
  friend void intrusive_ptr_add_ref( unknown_concurrent_tracking_info* const info )
  {
    mutex::scoped_lock lock( info->mutex_m );
    // ToDo: Assert if reference count is 0
    ++info->reference_count_m;
  }

  friend void intrusive_ptr_release( unknown_concurrent_tracking_info* const info )
  {
    bool destroy;

    {
      mutex::scoped_lock lock( info->mutex_m );

      destroy = ( --info->reference_count_m == 0 );
    }

    if( destroy )
      delete info;
  }*/
public:
  unknown_concurrent_tracking_info()
    : is_signalled_m( false ), reference_count_m( 1 ) {}

  virtual ~unknown_concurrent_tracking_info() = 0;
public:
  bool is_signalled() const
  {
    mutex::scoped_lock lock( mutex_m );
    return is_signalled_m;
  }

  void wait() const
  {
    mutex::scoped_lock lock( mutex_m );
    if( !is_signalled_m )
      condition_m.wait( lock );
  }
public:
  template< typename ValueType, typename FunctionType >
  void signal( FunctionType& function )
  {
    static_cast< concurrent_tracking_info< ValueType >& >( *this )
      .signal( function );
  }
private:
  typedef ::std::size_t ref_count_type;
protected:
  mutable mutex mutex_m;
  mutable condition condition_m;
  bool is_signalled_m;
  ref_count_type reference_count_m;
};

inline unknown_concurrent_tracking_info::~unknown_concurrent_tracking_info() {}

template< typename ValueType >
class void_concurrent_tracking_info
  : public unknown_concurrent_tracking_info
  , noncopyable
{
public:
  typedef ValueType value_type;
public:
  value_type wait_for_value() const
  {
    mutex::scoped_lock lock( mutex_m );
    if( !is_signalled_m )
      condition_m.wait( mutex_m );
  }

  template< typename FunctionType >
  void signal( FunctionType& function )
  {
    mutex::scoped_lock lock( mutex_m );
    function();
    is_signalled_m = true;
    condition_m.notify_all();
  }
};

template< typename ValueType >
class nonvoid_concurrent_tracking_info
  : public unknown_concurrent_tracking_info
  , noncopyable
{
public:
  typedef ValueType value_type;
public:
  ~nonvoid_concurrent_tracking_info()
  {
    if( is_signalled_m )
      value_no_lock().~value_type();
  }
public:
  value_type wait_for_value() const
  {
    mutex::scoped_lock lock( mutex_m );

    if( !is_signalled_m )
      condition_m.wait( lock );

    return value_no_lock();
  }

  template< typename FunctionType >
  void signal( FunctionType& function )
  {
    mutex::scoped_lock lock( mutex_m );
    new( &value_m ) value_type( function() );
    is_signalled_m = true;
    condition_m.notify_all();
  }
private:
  value_type& value_no_lock()
  {
    return reinterpret_cast< value_type& >( value_m );
  }

  value_type const& value_no_lock() const
  {
    return reinterpret_cast< value_type const& >( value_m );
  }
private:
  typename detail::raw_storage< value_type >::type value_m;
};

template< typename ValueType >
class concurrent_tracking_info
  : public mpl::if_< is_void< ValueType >
                   , void_concurrent_tracking_info< ValueType >
                   , nonvoid_concurrent_tracking_info< ValueType >
                   >
                   ::type
{
};

}
}
}

#endif
