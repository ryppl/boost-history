//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
// This interface is inspired by Howard Hinnant's lock proposal.
// http://home.twcny.rr.com/hinnant/cpp_extensions/threads_move.html
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNC_SHARED_LOCK_HPP
#define BOOST_SYNC_SHARED_LOCK_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

//~ #include <boost/interprocess/detail/config_begin.hpp>
//~ #include <boost/interprocess/detail/workaround.hpp>
//~ #include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/sync/locks/lock_options.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
//~ #include <boost/interprocess/detail/mpl.hpp>
//~ #include <boost/interprocess/detail/type_traits.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <boost/interprocess/detail/posix_time_types_wrk.hpp>

//!\file
//!Describes the upgrade_lock class that serves to acquire the upgrade
//!lock of a mutex.

namespace boost {
using interprocess::lock_exception;

template <class Lockable>
class unique_lock;

template <class UpgradeLockable>
class upgrade_lock;

//!shared_lock is meant to carry out the tasks for shared-locking
//!(such as read-locking), unlocking, try-shared-locking and timed-shared-locking
//!(recursive or not) for the Lockable. The Lockable need not supply all of this
//!functionality. If the client of shared_lock<Lockable> does not use functionality which
//!the Lockable does not supply, no harm is done. Lockable ownership can be shared among
//!shared_locks, and a single upgrade_lock. shared_lock does not support
//!copy semantics. But shared_lock supports ownership transfer from an shared_lock,
//!upgrade_lock and unique_lock via transfer_lock syntax.*/
template <class SharedLockable>
class shared_lock
{
   public:
   typedef SharedLockable mutex_type;
   /// @cond
   private:
   typedef shared_lock<SharedLockable> this_type;
   explicit shared_lock(unique_lock<mutex_type>&);
   typedef bool this_type::*unspecified_bool_type;
   BOOST_MOVABLE_BUT_NOT_COPYABLE(shared_lock)
   /// @endcond
   public:

   //!Effects: Default constructs a shared_lock.
   //!Postconditions: owns_lock() == false and mutex() == 0.
   shared_lock()
      : mp_mutex(0), m_locked(false)
   {}

   //!Effects: m.lock_shared().
   //!Postconditions: owns_lock() == true and mutex() == &m. 
   //!Notes: The constructor will take shared-ownership of the mutex. If
   //!   another thread already owns the mutex with exclusive ownership
   //!   (unique_lock), this thread will block until the mutex is released.
   //!   If another thread owns the mutex with shared or upgrade ownership,
   //!   then no blocking will occur. Whether or not this constructor handles
   //!   recursive locking depends upon the mutex.
   explicit shared_lock(mutex_type& m)
      : mp_mutex(&m), m_locked(false)
   {  mp_mutex->lock_shared();   m_locked = true;  }

   //!Postconditions: owns_lock() == false, and mutex() == &m.
   //!Notes: The constructor will not take ownership of the mutex. There is no effect
   //!   required on the referenced mutex.
   shared_lock(mutex_type& m, defer_lock_t)
      : mp_mutex(&m), m_locked(false)
   {}

   //!Postconditions: owns_lock() == true, and mutex() == &m.
   //!Notes: The constructor will suppose that the mutex is already shared
   //!   locked. There is no effect required on the referenced mutex.
   shared_lock(mutex_type& m, adopt_lock_t)
      : mp_mutex(&m), m_locked(true)
   {}

   //!Effects: m.try_lock_shared()
   //!Postconditions: mutex() == &m. owns_lock() == the return value of the
   //!   m.try_lock_shared() executed within the constructor.
   //!Notes: The constructor will take shared-ownership of the mutex if it
   //!   can do so without waiting. Whether or not this constructor handles
   //!   recursive locking depends upon the mutex. If the mutex_type does not
   //!   support try_lock_shared, this constructor will fail at compile
   //!   time if instantiated, but otherwise have no effect.
   shared_lock(mutex_type& m, try_to_lock_t)
      : mp_mutex(&m), m_locked(false)
   {  m_locked = mp_mutex->try_lock_shared();   }

   //!Effects: m.timed_lock_shared(abs_time) 
   //!Postconditions: mutex() == &m. owns_lock() == the return value of the
   //!   m.timed_lock_shared() executed within the constructor.
   //!Notes: The constructor will take shared-ownership of the mutex if it
   //!   can do so within the time specified. Whether or not this constructor
   //!   handles recursive locking depends upon the mutex. If the mutex_type
   //!   does not support timed_lock_shared, this constructor will fail at
   //!   compile time if instantiated, but otherwise have no effect.
   shared_lock(mutex_type& m, const boost::posix_time::ptime& abs_time)
      : mp_mutex(&m), m_locked(false)
   {  m_locked = mp_mutex->timed_lock_shared(abs_time);  }

   //!Postconditions: mutex() == upgr.mutex(). owns_lock() == the value of upgr.owns_lock()
   //!   before the construction. upgr.owns_lock() == false after the construction.
   //!Notes: If the upgr shared_lock owns the mutex, ownership is moved to this
   //!   shared_lock with no blocking. If the upgr shared_lock does not own the mutex, then
   //!   neither will this shared_lock. Only a moved shared_lock's will match this
   //!   signature. An non-moved shared_lock can be moved with the expression:
   //!   "boost::interprocess::move(lock);". This constructor does not alter the state of the mutex,
   //!   only potentially who owns it.
   shared_lock(BOOST_RV_REF(shared_lock<mutex_type>) upgr)
      : mp_mutex(0), m_locked(upgr.owns_lock())
   {  mp_mutex = upgr.release(); }

   //!Effects: If upgr.owns_lock() then calls unlock_upgrade_and_lock_shared() on the
   //!   referenced mutex.
   //!Postconditions: mutex() == the value upgr.mutex() had before the construction.
   //!   upgr.mutex() == 0 owns_lock() == the value of upgr.owns_lock() before construction.
   //!   upgr.owns_lock() == false after the construction. 
   //!Notes: If upgr is locked, this constructor will lock this shared_lock while
   //!   unlocking upgr. Only a moved shared_lock's will match this
   //!   signature. An non-moved upgrade_lock can be moved with the expression:
   //!   "boost::interprocess::move(lock);".*/
   template<class T>
   shared_lock(BOOST_RV_REF(upgrade_lock<T>) upgr
      , typename enable_if< is_same<T, SharedLockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      upgrade_lock<mutex_type> &u_lock = upgr;
      if(u_lock.owns_lock()){
         u_lock.mutex()->unlock_upgrade_and_lock_shared();
         m_locked = true;
      }
      mp_mutex = u_lock.release();
   }

   //!Effects: If scop.owns_lock() then calls unlock_and_lock_shared() on the
   //!   referenced mutex.
   //!Postconditions: mutex() == the value scop.mutex() had before the construction.
   //!   scop.mutex() == 0 owns_lock() == scop.owns_lock() before the constructor. After the
   //!   construction, scop.owns_lock() == false.
   //!Notes: If scop is locked, this constructor will transfer the exclusive ownership
   //!   to a shared-ownership of this shared_lock. 
   //!   Only a moved unique_lock's will match this
   //!   signature. An non-moved unique_lock can be moved with the expression:
   //!   "boost::interprocess::move(lock);".
   template<class T>
   shared_lock(BOOST_RV_REF(unique_lock<T>) scop
               , typename enable_if< is_same<T, SharedLockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      unique_lock<mutex_type> &e_lock = scop;
      if(e_lock.owns_lock()){
         e_lock.mutex()->unlock_and_lock_shared();
         m_locked = true;
      }
      mp_mutex = e_lock.release();
   }

   //!Effects: if (owns_lock()) mp_mutex->unlock_shared().
   //!Notes: The destructor behavior ensures that the mutex lock is not leaked.
   ~shared_lock()
   {
      try{
         if(m_locked && mp_mutex)   mp_mutex->unlock_shared();
      }
      catch(...){}
   }

   //!Effects: If owns_lock() before the call, then unlock_shared() is called on mutex().
   //!   *this gets the state of upgr and upgr gets set to a default constructed state. 
   //!Notes: With a recursive mutex it is possible that both this and upgr own the mutex
   //!   before the assignment. In this case, this will own the mutex after the assignment
   //!   (and upgr will not), but the mutex's lock count will be decremented by one.
   shared_lock &operator=(BOOST_RV_REF(shared_lock<mutex_type>) upgr)
   {  
      if(this->owns_lock())
         this->unlock();
      m_locked = upgr.owns_lock();
      mp_mutex = upgr.release();
      return *this;
   }

   //!Effects: If mutex() == 0 or already locked, throws a lock_exception()
   //!   exception. Calls lock_shared() on the referenced mutex.
   //!Postconditions: owns_lock() == true.
   //!Notes: The shared_lock changes from a state of not owning the
   //!   mutex, to owning the mutex, blocking if necessary.
   void lock()
   {  
      if(!mp_mutex || m_locked)
         throw lock_exception();
      mp_mutex->lock_shared();
      m_locked = true;
   }

   //!Effects: If mutex() == 0 or already locked, throws a lock_exception()
   //!   exception. Calls try_lock_shared() on the referenced mutex.
   //!Postconditions: owns_lock() == the value returned from
   //!   mutex()->try_lock_shared().
   //!Notes: The shared_lock changes from a state of not owning the mutex,
   //!   to owning the mutex, but only if blocking was not required. If the
   //!   mutex_type does not support try_lock_shared(), this function will
   //!   fail at compile time if instantiated, but otherwise have no effect.
   bool try_lock()
   {  
      if(!mp_mutex || m_locked)
         throw lock_exception();
      m_locked = mp_mutex->try_lock_shared();
      return m_locked;
   }

   //!Effects: If mutex() == 0 or already locked, throws a lock_exception()
   //!   exception. Calls timed_lock_shared(abs_time) on the referenced mutex.
   //!Postconditions: owns_lock() == the value returned from
   //!   mutex()->timed_lock_shared(elps_time).
   //!Notes: The shared_lock changes from a state of not owning the mutex,
   //!   to owning the mutex, but only if it can obtain ownership within the
   //!   specified time interval. If the mutex_type does not support
   //!   timed_lock_shared(), this function will fail at compile time if
   //!   instantiated, but otherwise have no effect.
   bool timed_lock(const boost::posix_time::ptime& abs_time)
   {  
      if(!mp_mutex || m_locked)
         throw lock_exception();
      m_locked = mp_mutex->timed_lock_shared(abs_time);
      return m_locked;
   }

   //!Effects: If mutex() == 0 or not locked, throws a lock_exception() exception.
   //!   Calls unlock_shared() on the referenced mutex.
   //!Postconditions: owns_lock() == false.
   //!Notes: The shared_lock changes from a state of owning the mutex, to
   //!   not owning the mutex.
   void unlock()
   {
      if(!mp_mutex || !m_locked)
         throw lock_exception();
      mp_mutex->unlock_shared();
      m_locked = false;
   }

   //!Effects: Returns true if this unique_lock has
   //!acquired the referenced mutex.
   bool owns_lock() const
   {  return m_locked && mp_mutex;  }

   bool owns() const
   {  return owns_lock();  }

   //!Conversion to bool.
   //!Returns owns_lock().
   operator unspecified_bool_type() const
   {  return m_locked? &this_type::m_locked : 0;   }

   //!Effects: Returns a pointer to the referenced mutex, or 0 if
   //!there is no mutex to reference.
   mutex_type* mutex() const
   {  return  mp_mutex;  }

   //!Effects: Returns a pointer to the referenced mutex, or 0 if there is no
   //!   mutex to reference.
   //!Postconditions: mutex() == 0 and owns_lock() == false.
   mutex_type* release()
   {
      mutex_type *mut = mp_mutex;
      mp_mutex = 0;
      m_locked = false;
      return mut;
   }

   //!Effects: Swaps state with moved lock. 
   //!Throws: Nothing.
   void swap(shared_lock<mutex_type> &other)
   {
      std::swap(mp_mutex, other.mp_mutex);
      std::swap(m_locked, other.m_locked);
   }

   /// @cond
   private:
   mutex_type *mp_mutex;
   bool        m_locked;
   /// @endcond
};

} // namespace boost

#include <boost/interprocess/detail/config_end.hpp>

#endif // BOOST_SYNC_SHARED_LOCK_HPP
