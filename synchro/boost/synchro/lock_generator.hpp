//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCK_GENERATOR__HPP
#define BOOST_SYNCHRO_LOCK_GENERATOR__HPP

#include "boost/synchro/lock_traits.hpp"
#include "boost/synchro/null/mutex.hpp"
#include "boost/synchro/thread/mutex.hpp"
#include "boost/synchro/thread/recursive_mutex.hpp"
#include "boost/synchro/thread/shared_mutex.hpp"
#include "boost/synchro/process/mutex.hpp"
#include "boost/synchro/process/recursive_mutex.hpp"
#include "boost/synchro/process/upgradable_mutex.hpp"
#include "boost/mpl/and.hpp"
#include "boost/type_traits.hpp"

namespace boost {
namespace synchro {

template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename TimedInterface,
    typename Lifetime,
    typename Naming
>
struct find_best_lock;

template<
typename Lockable,
typename Category,
typename Scope,
typename Reentrancy,
typename TimedInterface,
typename Lifetime,
typename Naming
>
struct lock_matches_features : mpl::and_<
mpl::and_<
    boost::is_base_and_derived<Category, typename category_tag<Lockable>::type>,
    boost::is_base_and_derived<Scope, typename scope_tag<Lockable>::type>,
    boost::is_base_and_derived<Reentrancy, typename reentrancy_tag<Lockable>::type> >,

    boost::is_base_and_derived<TimedInterface, typename timed_interface_tag<Lockable>::type>,
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
    typename TimedInterface=has_timed_interface_tag,
    typename Lifetime=typename default_lifetime<Scope>,
    typename Naming=anonymous_tag
>
struct find_best_lock;

template<
    typename Category,
    typename Reentrancy,
    typename TimedInterface,
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
  typedef boost::synchro::null_mutex type;
};

template<>
struct find_best_lock<
    multi_threaded_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    hasnt_timed_interface_tag,
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
    hasnt_timed_interface_tag,
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
    has_timed_interface_tag,
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
    has_timed_interface_tag,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::recursive_timed_mutex type;
};

template <
    typename Category,
    typename TimedInterface
>
struct find_best_lock<
    multi_threaded_tag,
    Category,
    non_recursive_tag,
    TimedInterface,
    process_lifetime_tag,
    anonymous_tag
> {
  typedef boost::shared_mutex type;
};

//////////////////////////

template <
    typename TimedInterface
>
struct find_best_lock<
    multi_process_tag,
    exclusive_lock_tag,
    non_recursive_tag,
    TimedInterface,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::synchro::interprocess_mutex type;
};

template <
    typename TimedInterface
>
struct find_best_lock<
    multi_process_tag,
    exclusive_lock_tag,
    recursive_tag,
    TimedInterface,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::synchro::interprocess_recursive_mutex type;
};


template <
    typename Category,
    typename TimedInterface
>
struct find_best_lock<
    multi_process_tag,
    Category,
    non_recursive_tag,
    TimedInterface,
    kernel_lifetime_tag,
    anonymous_tag
> {
  typedef boost::synchro::interprocess_upgradable_mutex type;
};


}
}

#endif
