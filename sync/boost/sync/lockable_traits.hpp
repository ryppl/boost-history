//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCH_LOCKABLE_TRAITS__HPP
#define BOOST_SYNCH_LOCKABLE_TRAITS__HPP

#include <boost/type_traits.hpp>
#include <boost/mpl/or.hpp>

namespace boost { namespace sync {

namespace sync_detail {
template <typename B, typename D>
struct is_same_or_is_base_and_derived
    : mpl::or_<is_same<B,D>, is_base_and_derived<B,D> >
{};
}

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
 * inherit from the helper lockable_base or
 * have a nested type scope or
 * specialize the scope_tag template class
 */

//[scope_tag
template <typename Lockable>
struct scope {
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
    : sync_detail::is_same_or_is_base_and_derived<
        mono_threaded_tag,
        typename scope<Lockable>::type
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
    : sync_detail::is_same_or_is_base_and_derived<
        multi_threaded_tag,
        typename scope<Lockable>::type
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
    : sync_detail::is_same_or_is_base_and_derived<
        multi_process_tag,
        typename scope<Lockable>::type
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
 * inherit from the helper lockable_base or
 * have a nested type lifetime or
 * specialize the lifetime_tag template class
 */

template <typename Lockable>
struct lifetime {
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
 * inherit from the helper lockable_base or
 * have a nested type naming or
 * specialize the naming_tag template class
 */

template <typename Lockable>
struct naming {
    typedef typename Lockable::naming type;
};

/**
 * a category_tag forms a hierarchy
 * exclusive_lock_tag <- sharable_lock_tag <- upgradable_lock_tag
 */
//[category_tag_hierarchy
struct exclusive_basic_lockable_tag {};
struct exclusive_try_lockable_tag : exclusive_basic_lockable_tag {};
struct exclusive_timed_lockable_tag : exclusive_try_lockable_tag {};
struct exclusive_condition_lockable_tag : exclusive_timed_lockable_tag {};

struct sharable_basic_lockable_tag : exclusive_timed_lockable_tag {};
struct sharable_try_lockable_tag : sharable_basic_lockable_tag {};
struct sharable_timed_lockable_tag : sharable_try_lockable_tag {};

struct upgradable_basic_lockable_tag : sharable_basic_lockable_tag  {};
struct upgradable_try_lockable_tag : upgradable_basic_lockable_tag, sharable_try_lockable_tag  {};
struct upgradable_timed_lockable_tag : upgradable_try_lockable_tag, sharable_timed_lockable_tag  {};

//]
/**
 * A lockable implementer must either
 * inherit from the helper lockable_base or
 * have a nested type category or
 * specialize the category_tag template class
 */

template <typename Lockable>
struct category {
    typedef typename Lockable::category type;
};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then exclusive_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_exclusive
    : sync_detail::is_same_or_is_base_and_derived<
        exclusive_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then sharable_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_sharable
    : sync_detail::is_same_or_is_base_and_derived<
        sharable_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};

/**
 * Inherits: If Lockable has a category_tag that inherits from
 * then upgradable_lock_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_upgradable
    : sync_detail::is_same_or_is_base_and_derived<
        upgradable_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};

    
/**
 * Inherits: If Lockable has a cathegory that inherits from
 * then XXX then inherits from true_type,
 * otherwise inherits from false_type.
 */

template <typename Lockable>
struct is_exclusive_basic_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        exclusive_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};


template <typename Lockable>
struct is_exclusive_try_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        exclusive_try_lockable_tag,
        typename category<Lockable>::type
    >
{};

template <typename Lockable>
struct is_exclusive_timed_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        exclusive_timed_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_exclusive_condition_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        exclusive_condition_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_sharable_basic_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        sharable_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_sharable_try_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        sharable_try_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_sharable_timed_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        sharable_timed_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_upgradable_basic_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        upgradable_basic_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_upgradable_try_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        upgradable_try_lockable_tag,
        typename category<Lockable>::type
    >
{};
    
template <typename Lockable>
struct is_upgradable_timed_lockable
    : sync_detail::is_same_or_is_base_and_derived<
        upgradable_timed_lockable_tag,
        typename category<Lockable>::type
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
 * inherit from the helper lockable_base or
 * have a nested type reentrancy or
 * specialize the reentrancy_tag template class
 */

template <typename Lockable>
struct reentrancy {
    typedef typename Lockable::reentrancy type;
};

/**
 * Inherits: If Lockable has a reentrancy_tag that inherits from
 * then recursive_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */
template <typename Lockable>
struct is_recursive
    : sync_detail::is_same_or_is_base_and_derived<
        recursive_tag,
        typename reentrancy<Lockable>::type
    >
{};

/**
 * a ownership tags formas a hierarchy
 * owned_tag <- unowned_tag
 */
//[ownership_tags
struct owned_tag {};
struct unowned_tag {};
//]

/**
 * A lockable implementer must either
 * inherit from the helper lockable_base or
 * have a nested type ownership or
 * specialize the ownership template class
 */

template <typename Lockable>
struct ownership {
    typedef typename Lockable::ownership type;
};

/**
 * Inherits: If Lockable has a reentrancy_tag that inherits from
 * then recursive_tag then inherits from true_type,
 * otherwise inherits from false_type.
 */
template <typename Lockable>
struct is_owned
    : sync_detail::is_same_or_is_base_and_derived<
        owned_tag,
        typename ownership<Lockable>::type
    >
{};

    
/** */

template <typename Lockable>
struct best_condition {
    typedef typename Lockable::best_condition_type type;
};

template <typename Lockable>
struct best_condition_any {
    typedef typename Lockable::best_condition_any_type type;
};

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

//[lockable_base_defaults
template<
    typename Scope=multi_threaded_tag,
    typename Category=exclusive_try_lockable_tag,
    typename Reentrancy=non_recursive_tag,
    typename Ownership=owned_tag,
    //~ typename Lifetime=typename default_lifetime<Scope>::type,
    typename Lifetime=process_lifetime_tag,
    typename Naming=anonymous_tag,
    typename Base=void
> struct lockable_base;
//]
/**
 * lockable_base is helper class that servs as a base class of lockables.
 */

//[lockable_base
template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename Ownership,
    typename Lifetime,
    typename Naming,
    typename Base
>
struct lockable_base : Base {
    // TODO add constraints on typenames
    typedef Scope scope;
    typedef Category category;
    typedef Reentrancy reentrancy;
    typedef Ownership ownership;
    typedef Lifetime lifetime;
    typedef Naming naming;
};
//]

//[lockable_base_void
template<
    typename Scope,
    typename Category,
    typename Reentrancy,
    typename Ownership,
    typename Lifetime,
    typename Naming
>
struct lockable_base<Scope, Category, Reentrancy, Ownership, Lifetime, Naming, void> {
    typedef Scope scope;
    typedef Category category;
    typedef Reentrancy reentrancy;
    typedef Ownership ownership;
    typedef Lifetime lifetime;
    typedef Naming naming;
};
//]


}
}


#endif

