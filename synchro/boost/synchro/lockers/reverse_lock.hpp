//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_REVERSE_LOCKER__HPP
#define BOOST_SYNCHRO_REVERSE_LOCKER__HPP

#include <boost/mpl/bool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/locker_concepts.hpp>

namespace boost { namespace synchro {



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
