//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONC_MONITOR__HPP
#define BOOST_SYNCHRO_CONC_MONITOR__HPP

#include <boost/synchro/lockers/condition_locker.hpp>
#include <boost/synchro/lockable_concepts.hpp>
#include <boost/synchro/lockable_adapter.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/synchro/thread/shared_mutex.hpp>
#include <boost/synchro/thread/locks.hpp>

namespace boost { namespace synchro {

/**
 */
#if 0
//[monitor_1st_Synopsis
template <
    typename Lockable=thread_mutex
>
class exclusive_monitor : protected exclusive_lockable_adapter<Lockable> { /*< behaves like an ExclusiveLockable for the derived classes >*/
protected:
    typedef unspecified synchronizer; /*< is an strict lock guard >*/
};
//]
#endif

#if 0
//[exclusive_monitor
template <
    typename Lockable=thread_mutex,
    class Condition=typename best_condition<Lockable>::type
    , typename ScopeTag=typename scope_tag<Lockable>::type
>
class exclusive_monitor : protected lockable_adapter<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
protected:
    typedef Condition condition;
    typedef condition_unique_locker<Lockable, Condition, ScopeTag> synchronizer;
};
//]

//[shared_monitor
template <
    typename Lockable=thread_shared_mutex,
    class Condition=condition_safe<typename best_condition_any<Lockable>::type >,
    , typename ScopeTag=typename scope_tag<Lockable>::type
>
class shared_monitor : protected lockable_adapter<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
protected:
    typedef Condition condition;
    typedef condition_unique_locker<Lockable, Condition, ScopeTag> synchronizer;
    typedef condition_shared_locker<Lockable, Condition, ScopeTag> shared_synchronizer;
};
//]

#endif

template <
    typename Lockable=thread_mutex,
    class Condition=condition_safe<typename best_condition<Lockable>::type >
    , typename ScopeTag=typename scope_tag<Lockable>::type
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition<Lockable>::type >
>
class exclusive_monitor : protected lockable_adapter<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
protected:
    typedef Condition condition;
    //typedef ConditionBoosted condition_boosted;
    typedef condition_unique_locker<Lockable, Condition, ScopeTag
    //    , ConditionBoosted
    > synchronizer;
};

template <
    typename Lockable=thread_shared_mutex,
    class Condition=condition_safe<typename best_condition_any<Lockable>::type >
    , typename ScopeTag=typename scope_tag<Lockable>::type
    //, class ConditionBoosted=condition_safe_boosted<typename best_condition_any<Lockable>::type >
>
class shared_monitor : protected lockable_adapter<Lockable> {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
protected:
    typedef Condition condition;
    //typedef ConditionBoosted condition_boosted;
    typedef condition_unique_locker<Lockable, Condition, ScopeTag
        //, ConditionBoosted
    > synchronizer;
    typedef condition_shared_locker<Lockable, Condition, ScopeTag
        //, ConditionBoosted
    > shared_synchronizer;
};

//[shared_monitor2
//template <
//    typename Lockable=thread_shared_mutex,
//    typename Condition=condition_safe<best_condition<Lockable> >
//>
//class upgrade_monitor : protected lockable_adapter<Lockable> {
//    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
//protected:
//    typedef Condition condition;
//    typedef condition_unique_locker<Lockable, Condition> synchronizer;
//    typedef condition_shared_locker<Lockable, Condition> shared_synchronizer;
//    typedef condition_upgrade_locker<Lockable, Condition> upgrade_synchronizer;
//};
//]

//[monitor
template <
      typename Lockable=thread_mutex
    , typename lock_tag=typename category_tag<Lockable>::type
    , typename ScopeTag=typename scope_tag<Lockable>::type
> struct monitor;

template <typename Lockable, typename ScopeTag>
struct monitor<Lockable, exclusive_lock_tag, ScopeTag>
    : protected exclusive_monitor<Lockable, ScopeTag>
{};

template <typename Lockable, typename ScopeTag>
struct monitor<Lockable, sharable_lock_tag, ScopeTag>
    : protected shared_monitor<Lockable, ScopeTag>
{};

template <typename Lockable, typename ScopeTag>
struct monitor<Lockable, upgradable_lock_tag, ScopeTag>
    : protected shared_monitor<Lockable, ScopeTag>
{};
//]
}
}
#endif
