//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_STM_AUTO_LOCK_H
#define BOOST_STM_AUTO_LOCK_H

#include <pthread.h>
#include <stdexcept>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/stm/detail/datatypes.hpp>

#ifdef PERFORMING_LATM
#include <boost/stm/transaction.hpp>
#define BOOST_STM_LOCK(a) boost::stm::transaction::lock_(a)
#define BOOST_STM_UNLOCK(a) boost::stm::transaction::unlock_(a)
#else
#define BOOST_STM_LOCK(a) boost::stm::lock(a)
#define BOOST_STM_UNLOCK(a) boost::stm::unlock(a)
#endif

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
namespace boost { namespace stm {


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
class timer_lock_exception : public std::exception
{
public:
   timer_lock_exception(char const * const what) : what_(what) {}

   //virtual char const * what() const { return what_; }

private:
   char const * const what_;
};


typedef timer_lock_exception timer_err;

class auto_lock
{
public:
   typedef std::pair<size_t const, Mutex*> ThreadedLockPair;
   typedef std::multimap<size_t const, Mutex*> ThreadedLockContainer;
   typedef ThreadedLockContainer::iterator ThreadedLockIter;

   auto_lock(Mutex &mutex) : hasLock_(false), lock_(0)
   {
      do_auto_lock(&mutex);
   }

   auto_lock(Mutex *mutex) : hasLock_(false), lock_(0)
   {
      do_auto_lock(mutex);
   }


   auto_lock(size_t timeOut, Mutex &mutex) : hasLock_(false), lock_(0)
   {
      do_timed_auto_lock(timeOut, &mutex);
   }

   auto_lock(size_t timeOut, Mutex *mutex) : hasLock_(false), lock_(0)
   {
      do_timed_auto_lock(timeOut, mutex);
   }

   ~auto_lock() { do_auto_unlock(); }

   bool has_lock() { return hasLock_; }

   void release_lock() { do_auto_unlock(); }

   static int VC6_fix()
   {
      static int VC6_fix = 0;
      return VC6_fix;
   }

private:

   void do_timed_auto_lock(size_t timeOut, Mutex *mutex)
   {
      lock_ = mutex;

      if (thread_has_lock(mutex)) return;

      for (size_t i = 0; i < timeOut; ++i)
      {
         if (0 == trylock(lock_))
         {
            hasLock_ = true;
            insert_into_threaded_lock_map(mutex);
            return;
         }

         SLEEP(1);
      }

      throw timer_lock_exception( "lock timed out" );
   }

   void insert_into_threaded_lock_map(Mutex* mutex)
   {
      lock(global_lock());
      threaded_locks().insert(ThreadedLockPair(THREAD_ID, mutex));
      unlock(global_lock());
   }

   void do_auto_lock(Mutex *mutex)
   {
      lock_ = mutex;
      if (thread_has_lock(mutex)) return;

      BOOST_STM_LOCK(mutex);
      hasLock_ = true;

      insert_into_threaded_lock_map(mutex);
   }

   void do_auto_unlock()
   {
      if (hasLock_)
      {
         hasLock_ = false;
         BOOST_STM_UNLOCK(lock_);
         remove_thread_has_lock(lock_);
      }
   }

   bool thread_has_lock(Mutex *rhs)
   {
      lock(global_lock());

      for (ThreadedLockIter i = threaded_locks().begin();
      i != threaded_locks().end(); ++i)
      {
         if (i->first == THREAD_ID && i->second == rhs)
         {
            unlock(global_lock());
            return true;
         }
      }

      unlock(global_lock());
      return false;
   }

   void remove_thread_has_lock(Mutex *rhs)
   {
      lock(global_lock());

      for (ThreadedLockIter i = threaded_locks().begin();
      i != threaded_locks().end(); ++i)
      {
         if (i->first == THREAD_ID && i->second == rhs)
         {
            threaded_locks().erase(i);
            break;
         }
      }

      unlock(global_lock());
   }

   static Mutex *global_lock()
   {
#ifndef BOOST_STM_USE_BOOST_MUTEX
      static Mutex globalLock = PTHREAD_MUTEX_INITIALIZER;
#else
      static Mutex globalLock;
#endif
      return &globalLock;
   }

   static ThreadedLockContainer &threaded_locks()
   {
      static ThreadedLockContainer localThreadedLocks;
      return localThreadedLocks;
   }

   //auto_lock(auto_lock const &);
   //auto_lock& operator=(auto_lock const &);

   bool hasLock_;
   Mutex *lock_;
};

#define use_lock(L) if (bool __end=false); else \
    for (boost::stm::auto_lock ___l(L); !__end; __end=true)
#define use_timed_lock(T, L) if (bool __end=false); else \
    for (boost::stm::auto_lock ___l(T, L); !__end; __end=false)
#define try_timed_lock(T, L)  if (bool __end=false); else \
    try { for (boost::stm::auto_lock ___l(T, L); !__end; __end=false)

#define catch_lock_timeout(E) } catch (std::timer_lock_exception &E)
#define lock_timeout } catch (std::timer_lock_exception &E)

} // core namespace
}



#endif // AUTO_LOCK_H









