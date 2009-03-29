//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_CONCEPTS_HPP
#define BOOST_SYNCHRO_LOCKABLE_CONCEPTS_HPP

#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/lockable/functions.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread/thread_time.hpp>
#include <boost/chrono/chrono.hpp>

#include <boost/concept_check.hpp>
namespace boost {  namespace synchro {

/**
 * LockableConcept object supports the basic features
 * required to delimit a critical region
 * Supports the basic lock, unlock and try_lock functions and
 * defines the lock traits
 */

//[LockableConcept
template <typename Lockable>
struct LockableConcept {
    typedef typename category_tag<Lockable>::type category;
    typedef typename timed_interface_tag<Lockable>::type timed_interface;
    typedef typename reentrancy_tag<Lockable>::type reentrancy;
    typedef typename scope_tag<Lockable>::type  scope;
    typedef typename lifetime_tag<Lockable>::type  lifetime;
    typedef typename naming_tag<Lockable>::type  naming;

    BOOST_CONCEPT_USAGE(LockableConcept) {
        lockable::lock(l);
        lockable::unlock(l);
        lockable::try_lock(l);
    }
    Lockable& l;
};
//]
/**
 * TimedLockableConcept object extends LockableConcept
 * with timed lock functions: try_lock_until and try_lock_for and the exception based lock_until and lock_for 
 */

//[TimedLockableConcept
template <typename Lockable>
struct TimedLockableConcept {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(TimedLockableConcept) {
        lockable::lock_until(l, t);
        lockable::lock_for(l, boost::chrono::seconds(1));
        lockable::try_lock_until(l, t);
        lockable::try_lock_for(l, boost::chrono::seconds(1));
    }
    Lockable& l;
    boost::chrono::system_clock::time_point t;
};
//]
bool pred();
//[ConditionLockableConcept
template <typename Lockable>
struct ConditionLockableConcept {
    BOOST_CONCEPT_ASSERT((TimedLockableConcept<Lockable>));
    typedef typename Lockable::condition Condition;

    BOOST_CONCEPT_USAGE(ConditionLockableConcept) {
        l.lock_when(c, pred);
        l.lock_when_until(c, pred, t);
        l.lock_when_for(c, pred, boost::chrono::seconds(1));
        l.relock_on(t);
        l.relock_on_until(c, t);
        l.relock_on_for(c, boost::chrono::seconds(1));
        l.relock_when(c, pred);
        l.relock_when_until(c, pred, t);
        l.relock_when_for(c, pred, boost::chrono::seconds(1));
        c.notify_one();
        c.notify_all();
    }
    Lockable& l;
    Condition c;
    boost::chrono::system_clock::time_point t;
};
      

//]
/**
 * ShareLockableConcept object extends TimedLockableConcept
 * with the lock_shared, lock_shared_until, lock_shared_for, try_lock_shared_until, try_lock_shared
 * and unlock_shared functions
 */
//[ShareLockableConcept
template <typename Lockable>
struct ShareLockableConcept {
    BOOST_CONCEPT_ASSERT((TimedLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(ShareLockableConcept) {
        lockable::lock_shared(l);
        lockable::lock_shared_until(l, t);
        lockable::lock_shared_for(l, boost::chrono::seconds(1));
        lockable::try_lock_shared(l);
        lockable::try_lock_shared_until(l, t);
        lockable::try_lock_shared_for(l, boost::chrono::seconds(1));
        lockable::unlock_shared(l);
    }
    Lockable& l;
    boost::chrono::system_clock::time_point t;
};
//]


/**
 * UpgradeLockableConcept object extends SharableLockableConcept
 * with the lock_upgrade, lock_upgrade_until, unlock_upgrade_and_lock,
 * unlock_and_lock_shared and unlock_upgrade_and_lock_shared functions
 */

//[UpgradeLockableConcept
template <typename Lockable>
struct UpgradeLockableConcept {
    BOOST_CONCEPT_ASSERT((ShareLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(UpgradeLockableConcept) {
        lockable::lock_upgrade(l);
        //lockable::lock_upgrade_until(l, t);
        //lockable::lock_upgrade_for(l, boost::chrono::seconds(1));
        lockable::try_lock_upgrade(l);
        //lockable::try_lock_upgrade_until(l, t);
        //lockable::try_lock_upgrade_for(l,boost::chrono::seconds(1));
        lockable::unlock_upgrade_and_lock(l);
        lockable::unlock_and_lock_upgrade(l);
        lockable::unlock_and_lock_shared(l);
        lockable::unlock_upgrade_and_lock_shared(l);
    }
    Lockable& l;
    boost::chrono::system_clock::time_point t;
};
//]

}
}
#endif
