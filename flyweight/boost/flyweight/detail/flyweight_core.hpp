/* Copyright 2006-2007 Joaquín M López Muñoz.
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * See http://www.boost.org/libs/flyweight for library home page.
 */

#ifndef BOOST_FLYWEIGHT_DETAIL_FLYWEIGHT_CORE_HPP
#define BOOST_FLYWEIGHT_DETAIL_FLYWEIGHT_CORE_HPP

#if defined(_MSC_VER)&&(_MSC_VER>=1200)
#pragma once
#endif

#include <boost/config.hpp> /* keep it first to prevent nasty warns in MSVC */
#include <boost/detail/workaround.hpp>
#include <boost/flyweight/detail/handle_factory_adaptor.hpp>
#include <boost/flyweight/detail/prevent_eti.hpp>
#include <boost/mpl/apply.hpp>

#if BOOST_WORKAROUND(BOOST_MSVC,BOOST_TESTED_AT(1400))
#pragma warning(push)
#pragma warning(disable:4101)  /* unreferenced local vars */
#endif

/* flyweight_core provides the inner implementation of flyweight<> by
 * weaving together a flyweight factory, a holder for the factory,
 * a tracking policy and a locking policy.
 */

namespace boost{

namespace flyweights{

namespace detail{

template<
  typename Value,typename Tag,typename TrackingPolicy,
  typename FactorySpecifier,typename LockingPolicy,typename HolderSpecifier
>
class flyweight_core;

template<
  typename Value,typename Tag,typename TrackingPolicy,
  typename FactorySpecifier,typename LockingPolicy,typename HolderSpecifier
>
struct flyweight_core_tracking_helper
{
private:
  typedef flyweight_core<
    Value,Tag,TrackingPolicy,
    FactorySpecifier,LockingPolicy,
    HolderSpecifier
  >                                   core;
  typedef typename core::handle_type  handle_type;
  typedef typename core::entry_type   entry_type;
  
public:
  static const entry_type& entry(const handle_type& h)
  {
    return core::factory().entry(h);
  }

  template<typename Checker>
  static void erase(const handle_type& h,Checker check)
  {
    typedef typename core::lock_type lock_type;
    lock_type lock(core::mutex());
    if(check(h))core::factory().erase(h);
  }
};

template<
  typename Value,typename Tag,typename TrackingPolicy,
  typename FactorySpecifier,typename LockingPolicy,typename HolderSpecifier
>
class flyweight_core
{
public:
  typedef typename detail::prevent_eti<
    TrackingPolicy,
    typename mpl::apply1<
      typename TrackingPolicy::entry_type,
      Value
    >::type 
  >::type                                      entry_type;
  typedef typename detail::prevent_eti<
    FactorySpecifier,
    typename mpl::apply2<
      FactorySpecifier,
      entry_type,
      Value
    >::type 
  >::type                                      base_factory_type;
  typedef typename detail::prevent_eti<
    TrackingPolicy,
    typename mpl::apply2<
      typename TrackingPolicy::handle_type,
      typename base_factory_type::handle_type,
      flyweight_core_tracking_helper<
        Value,Tag,TrackingPolicy,
        FactorySpecifier,LockingPolicy,
        HolderSpecifier
      >
    >::type
  >::type                                      handle_type;  
  typedef handle_factory_adaptor<
    base_factory_type,
    handle_type,entry_type
  >                                            factory_type;
  typedef typename LockingPolicy::mutex_type   mutex_type;
  typedef typename LockingPolicy::lock_type    lock_type;

  static handle_type insert(const Value& x)
  {
    lock_type lock(mutex());
    return handle_type(factory().insert(entry_type(x)));
  }
    
  static const Value& value(const handle_type& h)
  {
    return factory().entry(h);
  }

  static factory_type& factory()
  {
    (void)static_force_holder_get;
    return holder_type::get().factory;
  }

  static mutex_type& mutex()
  {
    (void)static_force_holder_get;
    return holder_type::get().mutex;
  }
  
private:
  struct                                holder_arg
  {
    factory_type factory;
    mutex_type   mutex;
  };
  typedef typename detail::prevent_eti<
    HolderSpecifier,
    typename mpl::apply1<
      HolderSpecifier,
      holder_arg
    >::type
  >::type                               holder_type;

  static bool static_force_holder_get;
};

template<
  typename Value,typename Tag,typename TrackingPolicy,
  typename FactorySpecifier,typename LockingPolicy,typename HolderSpecifier
>
bool flyweight_core<
  Value,Tag,TrackingPolicy,
  FactorySpecifier,LockingPolicy,HolderSpecifier>::static_force_holder_get=
  &flyweight_core<
    Value,Tag,TrackingPolicy,
    FactorySpecifier,LockingPolicy,HolderSpecifier>::holder_type::get()!=0;

} /* namespace flyweights::detail */

} /* namespace flyweights */

} /* namespace boost */

#if BOOST_WORKAROUND(BOOST_MSVC,BOOST_TESTED_AT(1400))
#pragma warning(pop)
#endif

#endif
