//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_STRICT_LOCKER__HPP
#define BOOST_SYNCHRO_STRICT_LOCKER__HPP

#include <boost/assert.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/synchro/lockers/is_strict_locker.hpp>
#include <boost/synchro/syntactic_lock_traits.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/lockable_concepts.hpp>
#include <boost/synchro/locker_concepts.hpp>
#include <boost/thread/exceptions.hpp>

namespace boost {  namespace synchro {

/**
* An strict_locker is a scoped lock guard ensuring the mutex is locked on the
* scope of the lock, by locking the mutex on construction and unlocking it on
* destruction.
*
* Essentially, a strict_locker's role is only to live on the stack as an
* automatic variable. strict_locker must adhere to a non-copy and non-alias
* policy. strict_locker disables copying by making the copy constructor and the
* assignment operator private. While we're at it, let's disable operator new
* and operator delete; strict_lockers are not intended to be allocated on the
* heap. strict_locker avoids aliasing by using a slightly less orthodox and
* less well-known technique: disable address taking.
*/

template <typename Locker>
struct StrictLockerConcept {
    BOOST_CONCEPT_ASSERT((BasicLockerConcept<Locker>));
    BOOST_STATIC_ASSERT((is_strict_locker<Locker>::value));
    typedef typename lockable_type<Locker>::type lockable_type;


    BOOST_CONCEPT_USAGE(StrictLockerConcept) {
        {
        }
    }
    lockable_type lock;
};
#if 0
//[strict_locker_synopsis
template <typename Lockable>
class strict_locker {
public:
    typedef Lockable lockable_type;
    typedef unspecified bool_type;

    explicit strict_locker(lockable_type& obj);
    ~strict_locker();

    operator bool_type() const;
    bool operator!() const;
    bool owns_lock() const;
    bool is_locking(lockable_type* l) const; /*< strict locker specific function >*/

    BOOST_ADRESS_OF_DELETE(strict_locker)  /*< disable aliasing >*/
    BOOST_HEAP_ALLOCATION_DELETE() /*< disable heap allocation >*/
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(strict_locker) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(strict_locker) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(strict_locker) /*< disable copy asignement >*/
};
//]
#endif
//[strict_locker
template <typename Lockable>
class strict_locker
{

      BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));
public:
    typedef Lockable lockable_type;
    explicit strict_locker(lockable_type& obj)
        : obj_(obj) { obj.lock(); } /*< locks on construction >*/
    ~strict_locker() { obj_.unlock(); } /*< unlocks on destruction >*/

    typedef bool (strict_locker::*bool_type)() const; /*< safe bool idiom >*/
    operator bool_type() const { return &strict_locker::owns_lock;  }
    bool operator!() const { return false; } /*< always owned >*/
    bool owns_lock() const { return true; }
    const lockable_type* mutex() const { return &obj_; }
    bool is_locking(lockable_type* l) const { return l==mutex(); } /*< strict lockers specific function >*/


    BOOST_ADRESS_OF_DELETE(strict_locker) /*< disable aliasing >*/
    BOOST_HEAP_ALLOCATEION_DELETE(strict_locker) /*< disable heap allocation >*/
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(strict_locker) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(strict_locker) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(strict_locker) /*< disable copy asignement >*/

    /*< no possibility to unlock >*/

private:
    lockable_type& obj_;
};
//]
template <typename Lockable>
struct is_strict_locker<strict_locker<Lockable> > : mpl::true_ {};

/**
 * An nested strict locker is a scoped lock guard ensuring the mutex is locked on its
 * scope, by taking ownership of an nesting locker, and locking the mutex on construction if not already locked
 * and restoring the ownership to the nesting locker on destruction.
 */
//[nested_strict_locker
template <typename Locker >
class nested_strict_locker
    {
      BOOST_CONCEPT_ASSERT((MovableLockerConcept<Locker>));
public:
    typedef typename lockable_type<Locker>::type lockable_type; /*< Name the lockable type locked by Locker >*/
    typedef typename syntactic_lock_traits<lockable_type>::lock_error lock_error;

    nested_strict_locker(Locker& locker)
        : locker_(locker) /*< Store reference to locker >*/
        , tmp_locker_(locker.move()) /*< Move ownership to temporaty locker >*/
    {
        #ifndef BOOST_SYNCHRO_STRCIT_LOCKER_DONT_CHECK_OWNERSHIP  /*< Define BOOST_SYNCHRO_EXTERNALLY_LOCKED_DONT_CHECK_OWNERSHIP if you don't want to check locker ownership >*/
        if (tmp_locker_.mutex()==0) {
            locker_=tmp_locker_.move(); /*< Rollback for coherency purposes >*/
            throw lock_error();
        }
        #endif
        if (!tmp_locker_) tmp_locker_.lock(); /*< ensures it is locked >*/
    }
    ~nested_strict_locker() {
        locker_=tmp_locker_.move(); /*< Move ownership to nesting locker >*/
    }
    typedef bool (nested_strict_locker::*bool_type)() const;
    operator bool_type() const { return &nested_strict_locker::owns_lock; }
    bool operator!() const { return false; }
    bool owns_lock() const { return true; }
    const lockable_type* mutex() const { return tmp_locker_.mutex(); }
    bool is_locking(lockable_type* l) const { return l==mutex(); }

    BOOST_ADRESS_OF_DELETE(nested_strict_locker)
    BOOST_HEAP_ALLOCATEION_DELETE(nested_strict_locker)
    BOOST_DEFAULT_CONSTRUCTOR_DELETE(nested_strict_locker) /*< disable default construction >*/
    BOOST_COPY_CONSTRUCTOR_DELETE(nested_strict_locker) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(nested_strict_locker) /*< disable copy asignement >*/

private:
    Locker& locker_;
    Locker tmp_locker_;
};
//]

template <typename Locker>
struct is_strict_locker<nested_strict_locker<Locker> > : mpl::true_ {} ;

}
}
#endif
