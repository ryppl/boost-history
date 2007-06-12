//  Copyright (c) 2006, Matthew Calabrese
//
//  This code may be used under either of the following two licences:
//
//    Permission is hereby granted, free of charge, to any person
//    obtaining a copy of this software and associated documentation
//    files (the "Software"), to deal in the Software without
//    restriction, including without limitation the rights to use,
//    copy, modify, merge, publish, distribute, sublicense, and/or
//    sell copies of the Software, and to permit persons to whom the
//    Software is furnished to do so, subject to the following
//    conditions:
//
//    The above copyright notice and this permission notice shall be
//    included in all copies or substantial portions of the Software.
//
//    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
//    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
//    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
//    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
//    OTHER DEALINGS IN THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//    Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//    http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_ACT_TRIGGER_HPP
#define BOOST_ACT_TRIGGER_HPP

#include <boost/noncopyable.hpp>

#include <boost/config.hpp>

#include <boost/thread/condition.hpp>

#include "detail/unspecified_bool.hpp"

// ToDo: Check safety of using unsigned int volatile
namespace boost
{
namespace act
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

#endif
