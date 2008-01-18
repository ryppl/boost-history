/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_REFCOUNTED_HPP
#define BOOST_FLYWEIGHT_REFCOUNTED_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <algorithm>
#include <boost/detail/atomic_count.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/flyweight/refcounted_fwd.hpp>
#include <boost/flyweight/tracking_tag.hpp>

/* Refcounting tracking policy: values have an embedded ref count,
 * when this goes down to zero the element is erased from the
 * factory.
 */

namespace boost{

namespace flyweights{

namespace detail{

template<typename Value>
class refcounted_value
{
public:
  explicit refcounted_value(const Value& x_):
    x(x_),ref(0)
  {}
  
  refcounted_value(const refcounted_value& r):
    x(r.x),ref(0)
  {}
  
  refcounted_value& operator=(const refcounted_value& r)
  {
    x=r.x;
    return *this;
  }
  
  operator const Value&()const{return x;}
    
#if !defined(BOOST_NO_MEMBER_TEMPLATE_FRIENDS)
private:
  template<typename,typename> friend class refcounted_handle;
#endif

  long count()const{return ref;}
  void add_ref()const{++ref;}
  bool release()const{return (--ref==0);}
  
private:  
  Value                               x;
  mutable boost::detail::atomic_count ref;
};

template<typename Handle,typename TrackingHelper>
class refcounted_handle
{
public:
  explicit refcounted_handle(const Handle& h_):h(h_)
  {
    TrackingHelper::entry(*this).add_ref();
  }
  
  refcounted_handle(const refcounted_handle& x):h(x.h)
  {
    TrackingHelper::entry(*this).add_ref();
  } 

  refcounted_handle& operator=(const refcounted_handle& x)
  {
    refcounted_handle tmp(x);
    std::swap(h,tmp.h);
    return *this;
  }
  
  ~refcounted_handle()
  {
    if(TrackingHelper::entry(*this).release()){
      TrackingHelper::erase(*this,check_erase);
    }
  }

  operator const Handle&()const{return h;}
  
private:
  static bool check_erase(const refcounted_handle& x)
  {
    return TrackingHelper::entry(x).count()==0;
  }

  Handle h;
};

} /* namespace flyweights::detail */

struct refcounted:tracking_marker
{
  struct entry_type
  {
    template<typename Value>
    struct apply
    {
      typedef detail::refcounted_value<Value> type;
    };
  };

  struct handle_type
  {
    template<typename Handle,typename TrackingHelper>
    struct apply
    {
      typedef detail::refcounted_handle<Handle,TrackingHelper> type;
    };
  };
};

} /* namespace flyweights */

} /* namespace boost */

#endif
