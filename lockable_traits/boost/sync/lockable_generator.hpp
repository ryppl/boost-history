//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synch for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCH_LOCK_GENERATOR__HPP
#define BOOST_SYNCH_LOCK_GENERATOR__HPP

#include <boost/sync/lockable_traits.hpp>
#include <boost/interprocess/sync/null_mutex.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>
#include <boost/interprocess/sync/interprocess_upgradable_mutex.hpp>
#include <boost/mpl/and.hpp>
#include <boost/type_traits.hpp>

namespace boost {
namespace synch {

template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename Kind,
    typename Lifetime,
    typename Naming
>
struct find_best_lock;

template<
typename Lockable,
typename Category,
typename Scope,
typename Reentrancy,
typename Kind,
typename Lifetime,
typename Naming
>
struct lock_matches_features : mpl::and_<
mpl::and_<
    boost::is_base_and_derived<Category, typename category_tag<Lockable>::type>,
    boost::is_base_and_derived<Scope, typename scope_tag<Lockable>::type>,
    boost::is_base_and_derived<Reentrancy, typename reentrancy_tag<Lockable>::type> >,

    boost::is_base_and_derived<Kind, typename kind<Lockable>::type>,
    boost::is_base_and_derived<Lifetime, typename lifetime_tag<Lockable>::type>,
    boost::is_base_and_derived<Naming, typename naming_tag<Lockable>::type>
> {};

template<
      typename Lockables,
    , typename constraint
struct fill_matches_features : mpl::copy_if<Lockables, constraint>

//template<
//    typename Lockables,
//    typename Lockables,
//struct filter : mpl::and_<

template<typename Scope>
struct default_lifetime;

template<>
struct default_lifetime<multi_threaded_tag> {
    typedef process_lifetime_tag type;
};

template<>
struct default_lifetime<multi_process_tag> {
    typedef kernel_lifetime_tag type;
};


template<
    typename Scope=multi_threaded_tag,
    typename Category=exclusive_lock_tag,
    typename Reentrancy=non_recursive_tag,
    typename Kind=timed_lockable_tag,
    typename Lifetime=typename default_lifetime<Scope>,
    typename Naming=anonymous_tag
>
struct find_best_lock;

template<
    typename Category,
    typename Reentrancy,
    typename Kind,
    typename Lifetime,
    typename Naming
>
struct find_best_lock<
    mono_threaded_tag,
    Category,
    Reentrancy,
    TimedInterface,
    Lifetime,
    Naming
> {
  typedef boost::interprocess::sync::null_mutex type;
};

template<>
struct find_best_lock<
    multi_threaded_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    timed_lockable_tag,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::mutex type;
};

template<>
struct find_best_lock<
    multi_threaded_tag,
    exclusive_lock_tag,
    recursive_tag,
    timed_lockable_tag,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::recursive_mutex type;
};

template<>
struct find_best_lock<
    multi_threaded_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    timed_lockable_tag,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::timed_mutex type;
};

template<>
struct find_best_lock<
    multi_threaded_tag,
    exclusive_lock_tag,
    recursive_tag,
    timed_lockable_tag,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::recursive_timed_mutex type;
};

template <
    typename Category,
    typename Kind
>
struct find_best_lock<
    multi_threaded_tag,
    Category,
    non_recursive_tag,
    Kind,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::shared_mutex type;
};

//////////////////////////

template <
    typename Kind
>
struct find_best_lock<
    multi_process_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    Kind,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::interprocess::sync::interprocess_mutex type;
};

template <
    typename Kind
>
struct find_best_lock<
    multi_process_tag,
    exclusive_lock_tag,
    recursive_tag,
    Kind,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::interprocess::sync::interprocess_recursive_mutex type;
};


template <
    typename Category,
    typename Kind
>
struct find_best_lock<
    multi_process_tag,
    Category,
    non_recursive_tag,
    Kind,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::interprocess::sync::interprocess_upgradable_mutex type;
};


}
}

#endif
