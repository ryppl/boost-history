//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_TRAITS__HPP
#define BOOST_SYNCHRO_LOCKABLE_TRAITS__HPP

#include <boost/type_traits.hpp>
#include <boost/mpl/or.hpp>
#include <boost/synchro/lockable_scope_traits.hpp>

namespace boost { namespace synchro {


template <typename B, typename D>
struct is_same_or_is_base_and_derived
    : mpl::or_<is_same<B,D>, is_base_and_derived<B,D> >
{};


/**
 * a scope_tag forms a hierarchy
 * mono_threaded_tag <- multi_threaded_tag <-  multi_process_tag
 */

//[scope_tag_hierarchy
struct mono_threaded_tag {};
struct multi_threaded_tag : mono_threaded_tag {};
struct multi_process_tag : multi_threaded_tag  {};
//]

/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type scope or
 * specialize the scope_tag template class
 */

//[scope_tag
template <typename Lockable>
struct scope_tag {
    typedef typename Lockable::scope type;
};
//]

/**
 * Inherits: If Lockable has a scope_tag that inherits from
 * then mono_threaded_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */
//[is_mono_threaded
template <typename Lockable>
struct is_mono_threaded
    : is_same_or_is_base_and_derived<
        mono_threaded_tag,
        typename scope_tag<Lockable>::type
    >
{};
//]

/**
 * Inherits: If Lockable has a scope_tag that inherits from
 * then multi_threaded_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */
//[is_multi_threaded
template <typename Lockable>
struct is_multi_threaded
    : is_same_or_is_base_and_derived<
        multi_threaded_tag,
        typename scope_tag<Lockable>::type
    >
{};
//]

/**
 * Inherits: If Lockable has a scope_tag that inherits from
 * then multi_process_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_multi_process
    : is_same_or_is_base_and_derived<
        multi_process_tag,
        typename scope_tag<Lockable>::type
    >
{};

/**
 * a lifetime_tag forms a hierarchy
 * process_lifetime_tag <- kernel_lifetime_tag <- filesystem_lifetime_tag
 */
//[lifetime_tag_hierarchy
struct process_lifetime_tag {};
struct kernel_lifetime_tag : process_lifetime_tag {};
struct filesystem_lifetime_tag : kernel_lifetime_tag {};
//]
/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type lifetime or
 * specialize the lifetime_tag template class
 */

template <typename Lockable>
struct lifetime_tag {
    typedef typename Lockable::lifetime type;
};

/**
 * a naming_tags forms a hierarchy
 * anonymous_tag <- named_tag
 */

struct anonymous_tag {};
struct named_tag : anonymous_tag {};

/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type naming or
 * specialize the naming_tag template class
 */

template <typename Lockable>
struct naming_tag {
    typedef typename Lockable::naming type;
};

/**
 * a category_tag forms a hierarchy
 * exclusive_lock_tag <- sharable_lock_tag <- upgradable_lock_tag
 */
//[category_tag_hierarchy
struct exclusive_lock_tag {};
struct sharable_lock_tag : exclusive_lock_tag {};
struct upgradable_lock_tag : sharable_lock_tag  {};
//]
/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type category or
 * specialize the category_tag template class
 */

template <typename Lockable>
struct category_tag {
    typedef typename Lockable::category type;
};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then exclusive_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_exclusive_lock
    : is_same_or_is_base_and_derived<
        exclusive_lock_tag,
        typename category_tag<Lockable>::type
    >
{};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then sharable_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_sharable_lock
    : is_same_or_is_base_and_derived<
        sharable_lock_tag,
        typename category_tag<Lockable>::type
    >
{};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then upgradable_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_upgradable_lock
    : is_same_or_is_base_and_derived<
        upgradable_lock_tag,
        typename category_tag<Lockable>::type
    >
{};

/**
 * a reentrancy_tag formas a hierarchy
 * non_recursive_tag <- recursive_tag
 */
//[reentrancy_tag_hierarchy
struct non_recursive_tag {};
struct recursive_tag : non_recursive_tag {};
//]

/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type reentrancy or
 * specialize the reentrancy_tag template class
 */

template <typename Lockable>
struct reentrancy_tag {
    typedef typename Lockable::reentrancy type;
};

/**
 * Inherits: If Lockable has a reentrancy_tag that inherits from
 * then recursive_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */
template <typename Lockable>
struct is_recursive_lock
    : is_same_or_is_base_and_derived<
        recursive_tag,
        typename reentrancy_tag<Lockable>::type
    >
{};

/**
 * a timed_interface_tag forms a hierarchy
 * hasnt_timed_interface_tag <- has_timed_interface_tag
 */

//[timed_interface_tag_hierarchy
struct hasnt_timed_interface_tag {};
struct has_timed_interface_tag : hasnt_timed_interface_tag {};
//]
/**
 * A lockable implementer must either
 * inherit from the helper lock_traits_base or
 * have a nested type timed_interface or
 * specialize the timed_interface_tag template class
 */

template <typename Lockable>
struct timed_interface_tag {
    typedef typename Lockable::timed_interface type;
};

/**
 * Inherits: If Lockable has a timed_interface_tag that inherits from
 * then has_timed_interface_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct has_timed_interface
    : is_same_or_is_base_and_derived<
        has_timed_interface_tag,
        typename timed_interface_tag<Lockable>::type
    >
{};

template <typename Locker>
struct lockable_type {
    typedef typename Locker::lockable_type type;
};

template <typename Lockable>
struct best_condition {
    typedef typename Lockable::best_condition_type type;
};

template <typename Lockable>
struct best_condition_any {
    typedef typename Lockable::best_condition_any_type type;
};

//[lock_lockers_traits
template <typename Lockable>
struct scoped_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::scoped_lock type;
};

template <typename Lockable>
struct unique_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::unique_lock type;
};

template <typename Lockable>
struct shared_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::shared_lock type;
};

template <typename Lockable>
struct upgrade_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::upgrade_lock type;
};

template <typename Lockable>
struct upgrade_to_unique_locker_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::upgrade_to_unique_locker type;
};

//]

//[lock_exception_traits
template <typename Lockable>
struct lock_error_type {
    typedef typename lockable_scope_traits<
        typename scope_tag<Lockable>::type, Lockable
    >::lock_error type;
};
//]

//[lock_movable_traits
template <typename Lockable>
struct move_object_type {
    template <typename T>
    struct moved_object : lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::template moved_object<T> {
        moved_object(T& t_): lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::template moved_object<T>(t_) {}
    };
};
//]
//[lockers_init_traits
template <typename Lockable>
struct defer_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::defer_lock_t type;
    static const type& value() {return lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::defer_lock();}
};

template <typename Lockable>
struct adopt_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::adopt_lock_t type;
    static const type& value(){return lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::adopt_lock();}
};

template <typename Lockable>
struct try_to_lock_type {
    typedef typename lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::try_to_lock_t type;
    static const type& value(){return lockable_scope_traits<
                typename scope_tag<Lockable>::type, Lockable>::try_to_lock();}
};

//]

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

//[lock_traits_base_defaults
template<
    typename Scope=multi_threaded_tag,
    typename Category=exclusive_lock_tag,
    typename Reentrancy=non_recursive_tag,
    typename TimedInterface=has_timed_interface_tag,
    typename Lifetime=typename default_lifetime<Scope>::type,
    typename Naming=anonymous_tag,
    typename Base=void
> struct lock_traits_base;
//]
/**
 * lock_traits_base is helper class that servs as a base class of lockables.
 */

//[lock_traits_base
template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename TimedInterface,
    typename Lifetime,
    typename Naming,
    typename Base
>
struct lock_traits_base : Base {
    // TODO add constraints on typenames
    typedef Scope scope;
    typedef Category category;
    typedef Reentrancy reentrancy;
    typedef TimedInterface timed_interface;
    typedef Lifetime lifetime;
    typedef Naming naming;
};
//]

//[lock_traits_base_void
template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename TimedInterface,
    typename Lifetime,
    typename Naming
>
struct lock_traits_base<Scope, Category, Reentrancy, TimedInterface, Lifetime, Naming, void> {
    typedef Scope scope;
    typedef Category category;
    typedef Reentrancy reentrancy;
    typedef TimedInterface timed_interface;
    typedef Lifetime lifetime;
    typedef Naming naming;
};
//]


}
}


#endif

