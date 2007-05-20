/*=============================================================================
    Copyright (c) 2006, 2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_CONSUMER_POLICY_DETAIL_CONCURRENT_TRACKING_INFO_HPP
#define BOOST_ACT_CONSUMER_POLICY_DETAIL_CONCURRENT_TRACKING_INFO_HPP

#include <boost/act/config/interlocked/has_interlocked_operations.hpp>

#ifdef BOOST_ACT_HAS_INTERLOCKED_OPERATIONS

#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy.hpp>

#include <new>

#include <boost/noncopyable.hpp>

#include <boost/act/detail/raw_storage.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

#include <boost/act/interlocked/assign_if_was.hpp>
#include <boost/act/interlocked/safe_get.hpp>
#include <boost/act/interlocked/assign.hpp>

namespace boost { namespace act { namespace detail {

template< typename ValueType >
class concurrent_tracking_info;

// ToDo: Use intrusive reference counting
class unknown_concurrent_tracking_info
{
public:
  virtual ~unknown_concurrent_tracking_info() = 0;
public:
  bool is_signalled() const
  {
    return interlocked::safe_get( signal_info_m ) == is_signalled_val;
  }

  void wait() const
  {
    // ToDo: Yield in body of while
    while( !is_signalled() );
  }
public:
  template< typename ValueType, typename FactoryType >
  bool signal_factory( FactoryType& factory )
  {
    return static_cast< concurrent_tracking_info< ValueType >& >( *this )
             .signal_factory( factory );
  }

  template< typename ValueType, typename FunctionType >
  bool signal_function( FunctionType& function )
  {
    return static_cast< concurrent_tracking_info< ValueType >& >( *this )
             .signal_function( function );
  }

  template< typename ValueType >
  bool signal( ValueType const& value )
  {
    return static_cast< concurrent_tracking_info< ValueType >& >( *this )
             .signal( value );
  }
protected:
  enum { not_signalled_val, being_signalled_val, is_signalled_val };
protected:
  interlocked::int_fast32_t signal_info_m;
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
    wait();
  }

  template< typename FunctionType >
  bool signal_function( FunctionType& function )
  {
    if( interlocked::compare_and_set_acquire( signal_info_m
                                            , being_signalled_val
                                            , not_signalled_val
                                            )
        == not_signalled_val
      )
    {
      function();
      interlocked::set_release( signal_info_m, is_signalled_val );
      return true;
    }
    else
      return false;
  }

  bool signal()
  {
    return    interlocked::compare_and_set( signal_info_m
                                          , is_signalled_val
                                          , not_signalled_val
                                          )
           == not_signalled_val;
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
    if( is_signalled() )
      value_no_lock().~value_type();
  }
public:
  value_type wait_for_value() const
  {
    // ToDo: Change memory barrier to acquire
    wait();
    return value();
  }

  template< typename FactoryType >
  void signal_factory( FactoryType& factory )
  {
    if( interlocked::compare_and_set_acquire( signal_info_m
                                            , being_signalled_val_val
                                            , not_signalled_val
                                            )
        == not_signalled_val
      )
    {
      factory.template apply< value_type >( &value_m );
      interlocked::set_release( signal_info_m, is_signalled_val );
      return true;
    }
    else
      return false;
  }

  template< typename FunctionType >
  void signal_function( FunctionType& function )
  {
    if( interlocked::compare_and_set_acquire( signal_info_m
                                            , being_signalled_val
                                            , not_signalled_val
                                            )
        == not_signalled_val
      )
    {
      new( &value_m ) value_type( function() );
      interlocked::set_release( signal_info_m, is_signalled_val );
      return true;
    }
    else
      return false;
  }

  void signal( value_type const& value )
  {
    if( interlocked::compare_and_set_acquire( signal_info_m
                                            , being_signalled_val
                                            , not_signalled_val
                                            )
        == not_signalled_val
      )
    {
      new( &value_m ) value_type( value );
      interlocked::set_release( signal_info_m, is_signalled_val );
      return true;
    }
    else
      return false;
  }
private:
  value_type& value()
  {
    return reinterpret_cast< value_type& >( value_m );
  }

  value_type const& value() const
  {
    return reinterpret_cast< value_type const& >( value_m );
  }
private:
  typename detail::raw_storage< value_type >::type value_m;
};

} } }

#else
/*
#include <boost/act/consumer_policy/concurrent_consumer_policy/concurrent_consumer_policy.hpp>

#include <new>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>

#include <boost/noncopyable.hpp>

#include <boost/act/detail/raw_storage.hpp>

#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_void.hpp>

namespace boost { namespace act { namespace detail {

template< typename ValueType >
class concurrent_tracking_info;

// ToDo: Use function pointer instead of virtual function
class unknown_concurrent_tracking_info
{
public:
  unknown_concurrent_tracking_info() : is_signalled_m( false ) {}

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
protected:
  mutable mutex mutex_m;
  mutable condition condition_m;
  bool is_signalled_m;
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
    mutex::scoped_lock lock( void_concurrent_tracking_info::mutex_m );
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

} } }
*/
#endif

namespace boost { namespace act { namespace detail {

template< typename ValueType >
class concurrent_tracking_info
  : public mpl::if_< is_void< ValueType >
                   , void_concurrent_tracking_info< ValueType >
                   , nonvoid_concurrent_tracking_info< ValueType >
                   >
                   ::type
{
};

} } }

#endif
