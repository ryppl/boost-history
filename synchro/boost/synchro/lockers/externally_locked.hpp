//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_EXTERNALLY_LOCKED__HPP
#define BOOST_SYNCHRO_EXTERNALLY_LOCKED__HPP

#include "boost/synchro/lockers/is_strict_locker.hpp"
#include "boost/synchro/lockers/strict_locker.hpp"
#include "boost/synchro/syntactic_lock_traits.hpp"
#include "boost/synchro/lockable_traits.hpp"
#include "boost/static_assert.hpp"
#include "boost/type_traits/is_same.hpp"
#include "boost/synchro/lockable_concepts.hpp"
#include "boost/thread/exceptions.hpp"

namespace boost { namespace synchro {

/**
 * externally_locked cloaks an object of type T, and actually provides full
 * access to that object through the get and set member functions, provided you
 * pass a reference to a strict_locker<Owner> object
 */

//[externally_locked
template <typename  T, typename Lockable>
class externally_locked {
    BOOST_CONCEPT_ASSERT((LockableConcept<Lockable>));

//	/*<-*/ typedef boost::lock_error lock_error; /*->*/
    typedef typename syntactic_lock_traits<Lockable>::lock_error lock_error; /*< needed until Boost Thread and Interprocess unify the exceptions >*/
public:
    externally_locked(T& obj, Lockable& lockable)
        : obj_(obj)
        , lockable_(lockable)
    {}

    externally_locked(Lockable& lockable)
        : obj_()
        , lockable_(lockable)
    {}

    T& get(strict_locker<Lockable>& locker) {

#ifndef BOOST_SYNCHRO_EXTERNALLY_LOCKED_DONT_CHECK_SAME  /*< define BOOST_SYNCHRO_EXTERNALLY_LOCKED_DONT_CHECK_SAME if you don't want to check locker check the same lockable >*/
        if (!locker.is_locking(&lockable_)) throw lock_error(); /*< run time check throw if not locks the same >*/
#endif
        return obj_;
    }
    void set(const T& obj, Lockable& lockable) {
        obj_ = obj;
        lockable_=lockable;
    }
private:
    T obj_;
    Lockable& lockable_;
};
//]

//[externally_locked_any
template <typename  T, typename Lockable>
class externally_locked_any {
	/*<-*/ typedef typename syntactic_lock_traits<Lockable>::lock_error lock_error; /*->*/
//	/*<-*/ typedef boost::lock_error lock_error; /*->*/
public:
/*<-*/    externally_locked_any(T& obj, Lockable& lockable)
        : obj_(obj)
        , lockable_(lockable)
    {}

    externally_locked_any(Lockable& lockable)
        : obj_()
        , lockable_(lockable)
    {}/*->*/
    // ... as before
    template <class Locker>
    T& get(Locker& locker) {
        BOOST_CONCEPT_ASSERT((StrictLockerConcept<Locker>));

        BOOST_STATIC_ASSERT((is_strict_locker<Locker>::value)); /*< locker is a strict locker "sur parolle" >*/
        BOOST_STATIC_ASSERT((is_same<Lockable,
        		typename lockable_type<Locker>::type>::value)); /*< that locks the same type >*/
#ifndef BOOST_SYNCHRO_EXTERNALLY_LOCKED_DONT_CHECK_OWNERSHIP  /*< define BOOST_SYNCHRO_EXTERNALLY_LOCKED_NO_CHECK_OWNERSHIP if you don't want to check locker ownership >*/
        if (! locker ) throw lock_error(); /*< run time check throw if no locked >*/
#endif
#ifndef BOOST_SYNCHRO_EXTERNALLY_LOCKED_DONT_CHECK_SAME
        if (!locker.is_locking(&lockable_)) throw lock_error();
#endif
        return obj_;
    }
/*<-*/
    void set(const T& obj, Lockable& lockable) {
        obj_ = obj;
        lockable_=lockable;
    }
private:
    T obj_;
    Lockable& lockable_;
/*->*/
};
//]
}
}
#endif
