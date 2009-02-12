//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_IS_STRICT_LOCKER__HPP
#define BOOST_SYNCHRO_IS_STRICT_LOCKER__HPP

#include <boost/mpl/bool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/locker_concepts.hpp>

namespace boost { namespace synchro {

/**
 */

template <typename Lockable>
class reverse_locker
{
    reverse_locker(Lockable& mtx):  mtx_(mtx) {mtx_.unlock();}
    ~reverse_locker() {mtx_.lock();}

protected:
    Lockable& mtx_;
};


/*
 *
 * unique_lock<mutex> lock(smtx);
 * // ... some read operations
 * { // non locked block
 * 		reverse_lock<unique_lock<mutex> > rlock(lock);
 * 	 // ... some code not needing
 * } // locked again
 * // ...
 */

template <typename Locker>
class reverse_lock : boost::noncopyable
{
//    BOOST_CONCEPT_ASSERT((MovableLockConcept<Locker>));
    typedef typename lock_error_type<typename lockable_type<Locker>::type >::type lock_error;

public:
    explicit reverse_lock(Locker& locker)
        : locker_(locker) /*< Store reference to locker >*/
        , tmp_locker_(locker.move()) /*< Move ownership to temporaty locker >*/
        , was_locked_(false)
    {
#ifndef BOOST_SYNCHRO_REVERSE_LOCK_DONT_CHECK_OWNERSHIP  /*< Define BOOST_SYNCHRO_REVERSE_LOCK_DONT_CHECK_OWNERSHIP if you don't want to check locker ownership >*/
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
    ~reverse_lock() {
    	if (was_locked_) {
        	tmp_locker_.lock();
    	}
        locker_=tmp_locker_.move(); /*< Move ownership to nesting locker >*/
    }

protected:
	Locker& locker_;
	Locker tmp_locker_;
	bool was_locked_;
	reverse_lock();
};

/*
 *
 * shared_lock<mutex> lock(smtx);
 * // ... some read operations
 * { // non locked block
 * 		reverse_lock<shared_lock<mutex> > rlock(lock);
 * 	 // ... some code not needing
 * } // locked again
 * // ...
 */



template <typename SharableMutex>
class exclusive_lockable_adapter
{
	exclusive_lockable_adapter(SharableMutex& mtx):  mtx_(mtx) {}
    ~exclusive_lockable_adapter() {}
    void lock()
    {mtx_.lock_shared();}
    void unlock()
    {mtx_.unlock_shared();}
    bool try_lock()
    { return mtx_.try_lock_shared();}

protected:
	SharableMutex& mtx_;
};


/*
 * template <class Lockable>
 * void f(Lokable&l) const {
 * 	boos::shared_lock<Lokable> lock(smtx);
 * // ... some read operations
 * }
 * // elsewhere
 * 		boost::mutex mtx;
 *  //
 *   {
 * 		shared_lockable_adapter smtx(mtx);
 * 		f(smtx);
 * 		//..
 * }
 *
 */
template <typename Lockable>
class shared_lockable_adapter
{
    shared_lockable_adapter(Lockable& mtx): mtx_(mtx) {}
    ~shared_lockable_adapter() {}
    void lock_shared() {mtx_.lock();}
    void unlock_shared() {mtx_.unlock();}
    bool try_lock_shared() { return mtx_.try_lock();}
    void lock() {mtx_.lock();}
    void unlock() {mtx_.unlock();}
    bool try_lock() { return mtx_.try_lock();}
    // other functions ....
private:
    Lockable& mtx_;
};

}
}

#endif
