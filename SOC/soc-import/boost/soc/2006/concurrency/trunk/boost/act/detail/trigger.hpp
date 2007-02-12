/*=============================================================================
    Copyright (c) 2006-2007 Matthew Calabrese

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_ACT_DETAIL_TRIGGER_HPP
#define BOOST_ACT_DETAIL_TRIGGER_HPP

#include <boost/noncopyable.hpp>

#include <boost/config.hpp>

#include <boost/thread/condition.hpp>

#include <boost/act/detail/unspecified_bool.hpp>

// ToDo: Check safety of using unsigned int volatile
namespace boost
{
namespace act
{
namespace detail
{
// ToDo: Rewrite with OpenMP optimized atomic implementation
// ToDo: Add event implementation
class trigger
  : ::boost::noncopyable
{
private:
  struct unspecified_bool_tag;
public:
  explicit trigger( bool initial_state = false )
    : value_m( initial_state )
  {
  }
public:
  trigger& operator =( bool value )
  {
    reset( value );

    return *this;
  }
public:
  void reset( bool value )
  {
    mutex::scoped_lock lock( mutex_m );

    if( value && !value_m )
      condition_m.notify_all();

    value_m = value;
  }

  void wait() const
  {
    mutex::scoped_lock lock( mutex_m );

    if( !value_m )
      condition_m.wait( lock );
  }

  operator BOOST_ACT_DETAIL_UNSPECIFIED_BOOL((unspecified_bool_tag))() const
  {
    return   check()
           ? BOOST_ACT_DETAIL_UNSPECIFIED_TRUE((unspecified_bool_tag))
           : BOOST_ACT_DETAIL_UNSPECIFIED_FALSE((unspecified_bool_tag));
  }
private:
  bool check() const
  {
    // Note: Aggressive locking.
    mutex::scoped_lock lock( mutex_m );
    return value_m;
  }
private:
  mutable mutex mutex_m;
  mutable condition condition_m;
  mutable bool value_m;
};
}
}
}

#endif
