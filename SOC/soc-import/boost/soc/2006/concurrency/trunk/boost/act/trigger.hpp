//  Copyright (c) 2006, Matthew Calabrese
//  All rights reserved.
//  Distributed under the New BSD License
//  (See accompanying file ACT_BSD_LICENSE.txt)

#ifndef BOOST_ACT_TRIGGER_HPP
#define BOOST_ACT_TRIGGER_HPP

#include <boost/noncopyable.hpp>

#include <boost/config.hpp>

#include <boost/thread/condition.hpp>

// ToDo: Check safety of using unsigned int volatile
namespace boost
{
namespace act
{

class trigger
  : ::boost::noncopyable
{
public:
  explicit trigger( bool initial_state = false )
  {
    value_m = initial_state;
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
    // ToDo: Possibly rewrite to avoid ordering problems
    if( value )
      condition_m.notify_all();

    value_m = value; // PORTABILIY NOTE: Assumes atomic write
  }

  void wait() const
  {
    if( !value_m ) // PORTABILIY NOTE: Assumes atomic read
    {
      ::boost::mutex::scoped_lock lock( mutex_m );

      if( !value_m )
        condition_m.wait( lock );
    }
  }

  operator bool() const // ToDo: Change return
  {
    return check();
  }

  bool check() const
  {
    return value_m != 0;
  }
private:
  ::boost::condition condition_m;
  unsigned int volatile value_m;
};

}
}

#endif
