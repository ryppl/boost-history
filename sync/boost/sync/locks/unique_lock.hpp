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

#ifndef BOOST_SYNC_UNIQUE_LOCK_HPP
#define BOOST_SYNC_UNIQUE_LOCK_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

//~ #include <boost/interprocess/detail/config_begin.hpp>
//~ #include <boost/interprocess/detail/workaround.hpp>
//~ #include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/sync/lock_options.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_same.hpp>
//~ #include <boost/interprocess/detail/mpl.hpp>
//~ #include <boost/interprocess/detail/type_traits.hpp>
#include <boost/interprocess/detail/move.hpp>
#include <boost/interprocess/detail/posix_time_types_wrk.hpp>

//!\file
//!Describes the unique_lock class.

namespace boost {
using interprocess::lock_exception;

template <class SharedLockable>
class shared_lock;

template <class UpgradeLockable>
class upgrade_lock;

//!unique_lock is meant to carry out the tasks for locking, unlocking, try-locking
//!and timed-locking (recursive or not) for the Lockable. The Lockable need not supply all
//!of this functionality. If the client of unique_lock<Lockable> does not use
//!functionality which the Lockable does not supply, no harm is done. Lockable ownership
//!transfer is supported through the syntax of move semantics. Ownership transfer
//!is allowed both by construction and assignment. The unique_lock does not support
//!copy semantics. A compile time error results if copy construction or copy
//!assignment is attempted. Lockable ownership can also be moved from an
//!upgrade_lock and shared_lock via constructor. In this role, unique_lock
//!shares the same functionality as a write_lock.
template <class Lockable>
class unique_lock
{
   /// @cond
   private:
   typedef unique_lock<Lockable> this_type;
   BOOST_MOVABLE_BUT_NOT_COPYABLE(unique_lock)
   typedef bool this_type::*unspecified_bool_type;
   /// @endcond
   public:
#if BOOST_WORKAROUND(__SUNPRO_CC, < 0x5100) 
    unique_lock(const volatile unique_lock&); 
#endif
   typedef Lockable mutex_type;

   //!Effects: Default constructs a unique_lock.
   //!Postconditions: owns_lock() == false and mutex() == 0.
   unique_lock()
      : mp_mutex(0), m_locked(false)
   {}

   //!Effects: m.lock().
   //!Postconditions: owns_lock() == true and mutex() == &m.
   //!Notes: The constructor will take ownership of the mutex. If another thread
   //!   already owns the mutex, this thread will block until the mutex is released.
   //!   Whether or not this constructor handles recursive locking depends upon the mutex.
   explicit unique_lock(mutex_type& m)
      : mp_mutex(&m), m_locked(false)
   {  mp_mutex->lock();   m_locked = true;  }

   //!Postconditions: owns_lock() == false, and mutex() == &m.
   //!Notes: The constructor will not take ownership of the mutex. There is no effect
   //!   required on the referenced mutex.
   unique_lock(mutex_type& m, defer_lock_t)
      : mp_mutex(&m), m_locked(false)
   {}

   //!Postconditions: owns_lock() == true, and mutex() == &m.
   //!Notes: The constructor will suppose that the mutex is already locked. There
   //!   is no effect required on the referenced mutex.
   unique_lock(mutex_type& m, adopt_lock_t)
      : mp_mutex(&m), m_locked(true)
   {}

   //!Effects: m.try_lock(). 
   //!Postconditions: mutex() == &m. owns_lock() == the return value of the
   //!   m.try_lock() executed within the constructor.
   //!Notes: The constructor will take ownership of the mutex if it can do
   //!   so without waiting. Whether or not this constructor handles recursive
   //!   locking depends upon the mutex. If the mutex_type does not support try_lock,
   //!   this constructor will fail at compile time if instantiated, but otherwise
   //!   have no effect.
   unique_lock(mutex_type& m, try_to_lock_t)
      : mp_mutex(&m), m_locked(mp_mutex->try_lock())
   {}

   //!Effects: m.timed_lock(abs_time). 
   //!Postconditions: mutex() == &m. owns_lock() == the return value of the
   //!   m.timed_lock(abs_time) executed within the constructor.
   //!Notes: The constructor will take ownership of the mutex if it can do
   //!   it until abs_time is reached. Whether or not this constructor
   //!   handles recursive locking depends upon the mutex. If the mutex_type
   //!   does not support try_lock, this constructor will fail at compile
   //!   time if instantiated, but otherwise have no effect.
   unique_lock(mutex_type& m, const boost::posix_time::ptime& abs_time)
      : mp_mutex(&m), m_locked(mp_mutex->try_lock_until(abs_time))
   {}

   //!Postconditions: mutex() == the value scop.mutex() had before the
   //!   constructor executes. s1.mutex() == 0. owns_lock() == the value of
   //!   scop.owns_lock() before the constructor executes. scop.owns_lock().
   //!Notes: If the scop unique_lock owns the mutex, ownership is moved
   //!   to thisscoped_lock with no blocking. If the scop unique_lock does not
   //!   own the mutex, then neither will this unique_lock. Only a moved
   //!   unique_lock's will match this signature. An non-moved unique_lock
   //!   can be moved with the expression: "boost::interprocess::move(lock);". This
   //!   constructor does not alter the state of the mutex, only potentially
   //!   who owns it.
   unique_lock(BOOST_RV_REF(unique_lock) scop)
      : mp_mutex(0), m_locked(scop.owns_lock())
   {  mp_mutex = scop.release(); }

   //!Effects: If upgr.owns_lock() then calls unlock_upgrade_and_lock() on the
   //!   referenced mutex. upgr.release() is called. 
   //!Postconditions: mutex() == the value upgr.mutex() had before the construction.
   //!   upgr.mutex() == 0. owns_lock() == upgr.owns_lock() before the construction.
   //!   upgr.owns_lock() == false after the construction.
   //!Notes: If upgr is locked, this constructor will lock this unique_lock while
   //!   unlocking upgr. If upgr is unlocked, then this unique_lock will be
   //!   unlocked as well. Only a moved upgrade_lock's will match this
   //!   signature. An non-moved upgrade_lock can be moved with
   //!   the expression: "boost::interprocess::move(lock);" This constructor may block if
   //!   other threads hold a shared_lock on this mutex (shared_lock's can
   //!   share ownership with an upgrade_lock).
   template<class T>
   explicit unique_lock(BOOST_RV_REF(upgrade_lock<T>) upgr
      , typename enable_if< is_same<T, Lockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      upgrade_lock<mutex_type> &u_lock = upgr;
      if(u_lock.owns_lock()){
         u_lock.mutex()->unlock_upgrade_and_lock();
         m_locked = true;
      }
      mp_mutex = u_lock.release();
   }

   //!Effects: If upgr.owns_lock() then calls try_unlock_upgrade_and_lock() on the
   //!referenced mutex:
   //!   a)if try_unlock_upgrade_and_lock() returns true then mutex() obtains
   //!      the value from upgr.release() and owns_lock() is set to true. 
   //!   b)if try_unlock_upgrade_and_lock() returns false then upgr is
   //!      unaffected and this unique_lock construction as the same effects as  
   //!      a default construction. 
   //!   c)Else upgr.owns_lock() is false. mutex() obtains the value from upgr.release()
   //!      and owns_lock() is set to false 
   //!Notes: This construction will not block. It will try to obtain mutex
   //!   ownership from upgr immediately, while changing the lock type from a
   //!   "read lock" to a "write lock". If the "read lock" isn't held in the
   //!   first place, the mutex merely changes type to an unlocked "write lock".
   //!   If the "read lock" is held, then mutex transfer occurs only if it can
   //!   do so in a non-blocking manner.
   template<class T>
   unique_lock(BOOST_RV_REF(upgrade_lock<T>) upgr, try_to_lock_t
         , typename enable_if< is_same<T, Lockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      upgrade_lock<mutex_type> &u_lock = upgr;
      if(u_lock.owns_lock()){
         if((m_locked = u_lock.mutex()->try_unlock_upgrade_and_lock()) == true){
            mp_mutex = u_lock.release();
         }
      }
      else{
         u_lock.release();
      }
   }

   //!Effects: If upgr.owns_lock() then calls timed_unlock_upgrade_and_lock(abs_time)
   //!   on the referenced mutex:
   //!   a)if timed_unlock_upgrade_and_lock(abs_time) returns true then mutex()
   //!      obtains the value from upgr.release() and owns_lock() is set to true. 
   //!   b)if timed_unlock_upgrade_and_lock(abs_time) returns false then upgr
   //!      is unaffected and this unique_lock construction as the same effects
   //!      as a default construction.
   //!   c)Else upgr.owns_lock() is false. mutex() obtains the value from upgr.release()
   //!      and owns_lock() is set to false 
   //!Notes: This construction will not block. It will try to obtain mutex ownership
   //!   from upgr immediately, while changing the lock type from a "read lock" to a
   //!   "write lock". If the "read lock" isn't held in the first place, the mutex
   //!   merely changes type to an unlocked "write lock". If the "read lock" is held,
   //!   then mutex transfer occurs only if it can do so in a non-blocking manner.
   template<class T>
   unique_lock(BOOST_RV_REF(upgrade_lock<T>) upgr, boost::posix_time::ptime &abs_time
               , typename enable_if< is_same<T, Lockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      upgrade_lock<mutex_type> &u_lock = upgr;
      if(u_lock.owns_lock()){
         if((m_locked = u_lock.mutex()->timed_unlock_upgrade_and_lock(abs_time)) == true){
            mp_mutex = u_lock.release();
         }
      }
      else{
         u_lock.release();
      }
   }

   //!Effects: If shar.owns_lock() then calls try_unlock_shared_and_lock() on the
   //!referenced mutex. 
   //!   a)if try_unlock_shared_and_lock() returns true then mutex() obtains
   //!      the value from shar.release() and owns_lock() is set to true. 
   //!   b)if try_unlock_shared_and_lock() returns false then shar is
   //!      unaffected and this unique_lock construction has the same
   //!      effects as a default construction. 
   //!   c)Else shar.owns_lock() is false. mutex() obtains the value from
   //!      shar.release() and owns_lock() is set to false 
   //!Notes: This construction will not block. It will try to obtain mutex
   //!   ownership from shar immediately, while changing the lock type from a
   //!   "read lock" to a "write lock". If the "read lock" isn't held in the
   //!   first place, the mutex merely changes type to an unlocked "write lock".
   //!   If the "read lock" is held, then mutex transfer occurs only if it can
   //!   do so in a non-blocking manner.
   template<class T>
   unique_lock(BOOST_RV_REF(shared_lock<T>) shar, try_to_lock_t
      , typename enable_if< is_same<T, Lockable> >::type * = 0)
      : mp_mutex(0), m_locked(false)
   {
      shared_lock<mutex_type> &s_lock = shar;
      if(s_lock.owns_lock()){
         if((m_locked = s_lock.mutex()->try_unlock_shared_and_lock()) == true){
            mp_mutex = s_lock.release();
         }
      }
      else{
         s_lock.release();
      }
   }

   //!Effects: if (owns_lock()) mp_mutex->unlock().
   //!Notes: The destructor behavior ensures that the mutex lock is not leaked.*/
   ~unique_lock()
   {
      try{  if(m_locked && mp_mutex)   mp_mutex->unlock();  }
      catch(...){}
   }

   //!Effects: If owns_lock() before the call, then unlock() is called on mutex().
   //!   *this gets the state of scop and scop gets set to a default constructed state. 
   //!Notes: With a recursive mutex it is possible that both this and scop own
   //!   the same mutex before the assignment. In this case, this will own the
   //!   mutex after the assignment (and scop will not), but the mutex's lock
   //!   count will be decremented by one.
   unique_lock &operator=(BOOST_RV_REF(unique_lock) scop)
   {  
      if(this->owns_lock())
         this->unlock();
      m_locked = scop.owns_lock();
      mp_mutex = scop.release();
      return *this;
   }

   //!Effects: If mutex() == 0 or if already locked, throws a lock_exception()
   //!   exception. Calls lock() on the referenced mutex.
   //!Postconditions: owns_lock() == true.
   //!Notes: The unique_lock changes from a state of not owning the mutex, to
   //!   owning the mutex, blocking if necessary.
   void lock()
   {
      if(!mp_mutex || m_locked)
         throw lock_exception();
      mp_mutex->lock();
      m_locked = true;
   }

   //!Effects: If mutex() == 0 or if already locked, throws a lock_exception()
   //!   exception. Calls try_lock() on the referenced mutex. 
   //!Postconditions: owns_lock() == the value returned from mutex()->try_lock().
   //!Notes: The unique_lock changes from a state of not owning the mutex, to
   //!   owning the mutex, but only if blocking was not required. If the
   //!   mutex_type does not support try_lock(), this function will fail at
   //!   compile time if instantiated, but otherwise have no effect.*/
   bool try_lock()
   {
      if(!mp_mutex || m_locked)
         throw lock_exception();
      m_locked = mp_mutex->try_lock();
      return m_locked;
   }

   //!Effects: If mutex() == 0 or if already locked, throws a lock_exception()
   //!   exception. Calls timed_lock(abs_time) on the referenced mutex.
   //!Postconditions: owns_lock() == the value returned from mutex()-> timed_lock(abs_time).
   //!Notes: The unique_lock changes from a state of not owning the mutex, to
   //!   owning the mutex, but only if it can obtain ownership by the specified
   //!   time. If the mutex_type does not support timed_lock (), this function
   //!   will fail at compile time if instantiated, but otherwise have no effect.*/
   bool timed_lock(const boost::posix_time::ptime& abs_time)
   {
      if(!mp_mutex || m_locked)
         throw lock_exception();
      m_locked = mp_mutex->timed_lock(abs_time);
      return m_locked;
   }

   //!Effects: If mutex() == 0 or if not locked, throws a lock_exception()
   //!   exception. Calls unlock() on the referenced mutex.
   //!Postconditions: owns_lock() == false.
   //!Notes: The unique_lock changes from a state of owning the mutex, to not
   //!   owning the mutex.*/
   void unlock()
   {
      if(!mp_mutex || !m_locked)
         throw lock_exception();
      mp_mutex->unlock();
      m_locked = false;
   }

   //!Effects: Returns true if this unique_lock has acquired
   //!the referenced mutex.
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
   void swap( unique_lock<mutex_type> &other)
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

//~ #include <boost/interprocess/detail/config_end.hpp>

#endif // BOOST_SYNC_UNIQUE_LOCK_HPP
