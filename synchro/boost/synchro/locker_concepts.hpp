//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_LOCKER_CONCEPTS_HPP
#define BOOST_SYNCHRO_LOCKER_CONCEPTS_HPP

#include "boost/synchro/lockable_traits.hpp"
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/concept_check.hpp>
namespace boost {  namespace synchro {

/**
 * LockerConcept object supports the basic features
 * required to delimit a critical region
 * Supports the basic lock, unlock and try_lock functions and
 * defines the lock traits
 */

template <typename Locker>
struct LockerConcept {
    typedef typename lockable_type<Locker>::type lockable_type;
    BOOST_CONCEPT_USAGE(LockerConcept) {
        Locker l(mtx_);
    }
    lockable_type mtx_;
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
        Locker l3(l1.move());
        BOOST_ASSERT((l2.mutex()!=&mtx_));
        l3.lock();
        l2 = l3.move();
    }
    lockable_type mtx_;
};


}
}
#endif
