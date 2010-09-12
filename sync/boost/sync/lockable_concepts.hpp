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

#include <boost/sync/lockable_traits.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
//~ #include <boost/chrono/duration.hpp>
//~ #include <boost/chrono/time_point.hpp>

#include <boost/concept_check.hpp>
namespace boost {  namespace sync {

/**
 * LockableConcept object supports the basic features
 * required to delimit a critical region
 * Supports the basic lock, unlock and try_lock functions and
 * defines the lock traits
 */

//[LockableConcept
template <typename Lockable>
struct BasicLockableConcept {
    typedef typename category<Lockable>::type category;
    typedef typename kind<Lockable>::type kind;
    typedef typename reentrancy<Lockable>::type reentrancy;
    typedef typename scope<Lockable>::type  scope;
    typedef typename lifetime<Lockable>::type  lifetime;
    typedef typename naming<Lockable>::type  naming;

    BOOST_CONCEPT_USAGE(BasicLockableConcept) {
        l.lock();
        l.unlock();
    }
    Lockable& l;
};
//]
/**
 * LockableConcept object supports the basic features
 * required to delimit a critical region
 * Supports the basic lock, unlock and try_lock functions and
 * defines the lock traits
 */

//[LockableConcept
template <typename Lockable>
struct TryLockableConcept {
    BOOST_CONCEPT_ASSERT((BasicLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(TryLockableConcept) {
        b=l.try_lock();
    }
    Lockable& l;
    bool b;
};
//]
/**
 * TimedLockableConcept object extends LockableConcept
 * with timed lock functions: try_lock_until and try_lock_for and the exception based lock_until and lock_for 
 */

//[TimedLockableConcept
template <typename Lockable>
struct TimedLockableConcept {
    BOOST_CONCEPT_ASSERT((TryLockableConcept<Lockable>));

    BOOST_CONCEPT_USAGE(TimedLockableConcept) {
        //~ l.lock_until(t);
        //~ l.lock_for(d);
        b=l.try_lock_until(t);
        b=l.try_lock_for(d);
    }
    Lockable& l;
    //~ boost::chrono::system_clock::time_point t;
    //~ boost::chrono::system_clock::duration d;
    boost::posix_time::ptime t;
    boost::posix_time::time_duration d;
    bool b;
};
//]


//[ConditionLockableConcept
template <typename Lockable>
struct ConditionLockableConcept {
    static bool pred();
    BOOST_CONCEPT_ASSERT((TimedLockableConcept<Lockable>));
    typedef typename Lockable::condition Condition;

    BOOST_CONCEPT_USAGE(ConditionLockableConcept) {
        l.lock_when(c, pred);
        l.lock_when_until(c, pred, t);
        l.lock_when_for(c, pred, d);
        l.relock_on(c);
        l.relock_on_until(c, t);
        l.relock_on_for(c, d);
        l.relock_when(c, pred);
        l.relock_when_until(c, pred, t);
        l.relock_when_for(c, pred, d);
        c.notify_one();
        c.notify_all();
    }
    Lockable& l;
    Condition& c;
    //~ boost::chrono::system_clock::time_point t;
    //~ boost::chrono::system_clock::duration d;
    boost::posix_time::ptime t;
    boost::posix_time::time_duration d;
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
        l.lock_shared();
        //~ l.lock_shared_until(t);
        //~ l.lock_shared_for(d);
        b=l.try_lock_shared();
        b=l.try_lock_shared_until(t);
        b=l.try_lock_shared_for(d);
        l.unlock_shared();
    }
    Lockable& l;
    //~ boost::chrono::system_clock::time_point t;
    //~ boost::chrono::system_clock::duration d;
    boost::posix_time::ptime t;
    boost::posix_time::time_duration d;
    bool b;
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
        l.lock_upgrade();
        l.unlock_upgrade();
        //~ l.lock_upgrade_until(t);
        //~ l.lock_upgrade_for(d);
        b=l.try_lock_upgrade();
        b=l.try_lock_upgrade_until(t);
        b=l.try_lock_upgrade_for(d);
        l.unlock_upgrade_and_lock();
        l.unlock_and_lock_upgrade();
        l.unlock_and_lock_shared();
        l.unlock_upgrade_and_lock_shared();
        //~ b=try_unlock_upgrade_and_lock();
        //~ b=try_unlock_upgradable_and_lock_for(d);
        //~ b=try_unlock_upgradable_and_lock_until(t);
        //~ b=try_unlock_share_and_lock();
        //~ b=try_unlock_share_and_lock_upgrade();
        
    }
    Lockable& l;
    //~ boost::chrono::system_clock::time_point t;
    //~ boost::chrono::system_clock::duration d;
    boost::posix_time::ptime t;
    boost::posix_time::time_duration d;
    bool b;

};
//]

}
}
#endif
