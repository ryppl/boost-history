//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKER_CONCEPTS_HPP
#define BOOST_SYNCHRO_LOCKER_CONCEPTS_HPP

#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/concept_check.hpp>
#include <boost/thread/thread_time.hpp>

namespace boost {  namespace synchro {

/**
 * LockerConcept object supports the basic features
 * required to delimit a critical region
 * Supports the basic lock, unlock and try_lock functions and
 * defines the lock traits
 */

template <typename Locker>
struct BasicLockerConcept {
    typedef typename lockable_type<Locker>::type lockable_type;
    
    BOOST_CONCEPT_USAGE(BasicLockerConcept) {
        const Locker l1(mtx_);
        if (l1.is_locking(mtx_)) return;
        if (l1.owns_lock()) return;
        if (l1) return;
        if (!l1) return;
    }
    lockable_type mtx_;
    system_time t;
};

template <typename Locker>
struct LockerConcept {
    BOOST_CONCEPT_ASSERT((BasicLockerConcept<Locker>));
    typedef typename lockable_type<Locker>::type lockable_type;
    
    BOOST_CONCEPT_USAGE(LockerConcept) {
        Locker l2(mtx_, defer_lock);
        Locker l3(mtx_, adopt_lock);
        Locker l4(mtx_, try_to_lock);
        l2.lock();
        if (l2.try_lock()) return;
        l2.unlock();
        //l2.release();
    }
    lockable_type mtx_;
    system_time t;
};

template <typename Locker>
struct TimedLockerConcept {
    BOOST_CONCEPT_ASSERT((LockerConcept<Locker>));
    typedef typename lockable_type<Locker>::type lockable_type;

    BOOST_CONCEPT_USAGE(TimedLockerConcept) {
        Locker l1(mtx_, t);
        Locker l2(mtx_, boost::posix_time::seconds(1));
        Locker l3(mtx_, t, throw_timeout);
        Locker l4(mtx_, boost::posix_time::seconds(1), throw_timeout);
        Locker l5(t, mtx_);
        Locker l6(boost::posix_time::seconds(1), mtx_);
        Locker l7(nothrow_timeout, t, mtx_);
        Locker l8(nothrow_timeout, boost::posix_time::seconds(1), mtx_);
        l5.lock_until(t);
        l5.lock_for(boost::posix_time::seconds(1));
        if (l5.try_lock_until(t)) return;
        if (l5.try_lock_for(boost::posix_time::seconds(1))) return;
    }
    lockable_type mtx_;
    system_time t;
};


template <typename Locker>
struct UniqueLockerConcept {
    BOOST_CONCEPT_ASSERT((TimedLockerConcept<Locker>));
    
    BOOST_CONCEPT_USAGE(UniqueLockerConcept) {
        
    }
};

template <typename Locker>
struct SharedLockerConcept {
    BOOST_CONCEPT_ASSERT((TimedLockerConcept<Locker>));

    BOOST_CONCEPT_USAGE(SharedLockerConcept) {
    }
};

template <typename Locker>
struct UpgradeLockerConcept {
    BOOST_CONCEPT_ASSERT((TimedLockerConcept<Locker>));

    BOOST_CONCEPT_USAGE(UpgradeLockerConcept) {
    }
};

/**
 * MovableLockerConcept object extends LockerConcept
 * with the timed_lock function
 */

template <typename Locker>
struct MovableLockerConcept {
    typedef typename lockable_type<Locker>::type lockable_type;
    BOOST_CONCEPT_ASSERT((LockerConcept<lockable_type>));

    BOOST_CONCEPT_USAGE(MovableLockerConcept) {
        Locker l1(mtx_);
        Locker& l2(l1);
        Locker l3(boost::move(l1));
        BOOST_ASSERT((l2.mutex()!=&mtx_));
        l3.lock();
    }
    lockable_type mtx_;
};


}
}
#endif
