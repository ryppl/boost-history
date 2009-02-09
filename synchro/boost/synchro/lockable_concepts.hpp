//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKABLE_CONCEPTS_HPP
#define BOOST_SYNCHRO_LOCKABLE_CONCEPTS_HPP

#include "boost/synchro/lockable_traits.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/thread/thread_time.hpp>
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
        l.lock();
        l.unlock();
        l.try_lock();
    }
    Lockable l;
};
//]
/**
 * TimedLockableConcept object extends ExclusiveLockConcept
 * with the timed_lock function
 */

//[TimedLockableConcept
template <typename Lockable>
struct TimedLockableConcept {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(TimedLockableConcept) {
        l.timed_lock(t);
//        l.timed_lock(1);
    }
    Lockable l;
    system_time t;
};
//]

/**
 * ShareLockableConcept object extends ExclusiveTimedLockConcept
 * with the lock_shared, timed_lock_shared, try_lock_shared
 * and unlock_shared functions
 */
//[ShareLockableConcept
template <typename Lockable>
struct ShareLockableConcept {
    BOOST_CONCEPT_ASSERT((TimedLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(ShareLockableConcept) {
        l.lock_shared();
        l.timed_lock_shared(t);
        l.try_lock_shared();
        l.unlock_shared();
    }
    Lockable l;
    system_time t;
};
//]


/**
 * UpgradeLockableConcept object extends SharableLockableConcept
 * with the lock_upgrade, timed_lock_upgrade, unlock_upgrade_and_lock,
 * unlock_and_lock_shared and unlock_upgrade_and_lock_shared functions
 */

//[UpgradeLockableConcept
template <typename Lockable>
struct UpgradeLockableConcept {
    BOOST_CONCEPT_ASSERT((ShareLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(UpgradeLockableConcept) {
        l.lock_upgrade();
        l.timed_lock_upgrade(t);
        l.unlock_upgrade_and_lock();
        l.unlock_and_lock_upgrade();
        l.unlock_and_lock_shared();
        l.unlock_upgrade_and_lock_shared();
    }
    Lockable l;
    system_time t;
};
//]

}
}
#endif
