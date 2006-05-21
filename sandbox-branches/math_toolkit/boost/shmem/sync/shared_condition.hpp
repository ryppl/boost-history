//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHARED_CONDITION_HPP
#define BOOST_SHARED_CONDITION_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>
#include <boost/shmem/sync/xtime.hpp>
#include <boost/shmem/sync/lock.hpp>
#include <boost/shmem/sync/shared_condition.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/exceptions.hpp>
#include <boost/limits.hpp>
#include <cassert>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>
#else
#  include <errno.h>
#  include <pthread.h>
#endif

/*!\file
   Describes process-shared variables condition class
*/

namespace boost {

namespace shmem {

struct xtime;

class shared_condition : private noncopyable
{
 public:
   /*!Constructs a shared_condition*/
   shared_condition();

   /*!Destroys *this*/
   ~shared_condition();

   /*!If there is a thread waiting on *this, change that 
      thread's state to ready. Otherwise there is no effect.*/
   void notify_one();

   /*!Change the state of all threads waiting on *this to ready.
      If there are no waiting threads, notify_all() has no effect.*/
   void notify_all();

   /*!Releases the lock on the mutex object associated with lock, blocks 
      the current thread of execution until readied by a call to 
      this->notify_one() or this->notify_all(), and then reacquires the lock.*/
   template <typename L>
   void wait(L& lock)
   {
      if (!lock)
         throw lock_exception();
      do_wait(lock.m_mutex);
   }

   /*!The same as: while (!pred()) wait(lock)*/
   template <typename L, typename Pr>
   void wait(L& lock, Pr pred)
   {
      if (!lock)
         throw lock_exception();

      while (!pred())
         do_wait(lock.m_mutex);
   }

   /*!Releases the lock on the mutex object associated with lock, blocks 
      the current thread of execution until readied by a call to 
      this->notify_one() or this->notify_all(), or until time xt is reached, 
      and then reacquires the lock.
      Returns: false if time xt is reached, otherwise true.*/
   template <typename L>
   bool timed_wait(L& lock, const xtime& xt)
   {
      if (!lock)
            throw lock_exception();

      return do_timed_wait(xt, lock.m_mutex);
   }

   /*!The same as:   while (!pred()) { 
                        if (!timed_wait(lock, xt)) return false; 
                     } return true;*/
   template <typename L, typename Pr>
   bool timed_wait(L& lock, const xtime& xt, Pr pred)
   {
      if (!lock)
            throw lock_exception();

      while (!pred()){
            if (!do_timed_wait(xt, lock.m_mutex))
               return false;
      }

      return true;
   }

 private:
   template<class SharedMutex>
   void do_wait(SharedMutex &mut);
   template<class SharedMutex>
   bool do_timed_wait(const xtime& xt, SharedMutex &mut);
#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   enum { SLEEP, NOTIFY_ONE, NOTIFY_ALL };
   shared_try_mutex m_enter_mut;
   shared_mutex     m_check_mut;
   volatile long    m_command;
   volatile long    m_num_waiters;
   template<class SharedMutex>
   bool do_timed_wait(bool tout_enabled, const xtime& xt, SharedMutex &mut);
   void notify(long command);
#else
   pthread_cond_t   m_condition;
#endif
};

}  //namespace shmem

}  // namespace boost

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/shared_condition.hpp>
#else 
#  include <boost/shmem/sync/posix/shared_condition.hpp>
#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <boost/shmem/detail/config_end.hpp>

#endif // BOOST_SHARED_CONDITION_HPP
