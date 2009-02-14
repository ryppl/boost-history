//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_NESTED_REVERSE_LOCKER__HPP
#define BOOST_SYNCHRO_NESTED_REVERSE_LOCKER__HPP

#include <boost/mpl/bool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/locker_concepts.hpp>

namespace boost { namespace synchro {

/*
 *
 * unique_lock<mutex> lock(smtx);
 * // ... some read operations
 * { // non locked block
 *         nested_reverse_locker<unique_lock<mutex> > rlock(lock);
 *      // ... some code not needing
 * } // locked again
 * // ...
 */

template <typename Locker>
class nested_reverse_locker : boost::noncopyable
{
//    BOOST_CONCEPT_ASSERT((MovableLockConcept<Locker>));
    typedef typename lock_error_type<typename lockable_type<Locker>::type >::type lock_error;

public:
    explicit nested_reverse_locker(Locker& locker)
        : locker_(locker) /*< Store reference to locker >*/
        , tmp_locker_(locker.move()) /*< Move ownership to temporaty locker >*/
        , was_locked_(false)
    {
#ifndef BOOST_SYNCHRO_NESTED_REVERSE_LOCKER_DONT_CHECK_OWNERSHIP  /*< Define BOOST_SYNCHRO_NESTED_REVERSE_LOCKER_DONT_CHECK_OWNERSHIP if you don't want to check locker ownership >*/
        if (tmp_locker_.mutex()==0) {
            locker_=tmp_locker_.move(); /*< Rollback for coherency purposes >*/
            throw lock_error();
        }
#endif
        if (tmp_locker_) { /*< ensures it is unlocked >*/
            tmp_locker_.unlock();
            was_locked_=true;
        }
    }
    ~nested_reverse_locker() {
        if (was_locked_) {
            tmp_locker_.lock();
        }
        locker_=tmp_locker_.move(); /*< Move ownership to nesting locker >*/
    }

protected:
    Locker& locker_;
    Locker tmp_locker_;
    bool was_locked_;
    nested_reverse_locker();
};

}
}

#endif
