//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
// Parts of the pthread code come from Boost Threads code:
//
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2001-2003
// William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_RECURSIVE_SHARED_MUTEX_HPP
#define BOOST_RECURSIVE_SHARED_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shmem/sync/lock.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/sync/shared_recursive_mutex.hpp>
#include <boost/shmem/sync/xtime.hpp>
#include <assert.h>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>
#else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <pthread.h>
#  include <errno.h>
#  include <boost/shmem/sync/posix/pthread_helpers.hpp>   
#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

/*!\file
   Describes shared_recursive_mutex and shared_recursive_try_mutex classes
*/

namespace boost {

namespace shmem {

struct xtime;

/*!Wraps a mutex that can be placed in shared memory and can be 
   shared between processes. Allows several locking calls by the same 
   process.*/
class shared_recursive_mutex : private boost::noncopyable
{
 public:

   /*!Creates mutex resources, throws process_resource_error
      if there are available resources */
   shared_recursive_mutex();

   /*!Destructs resources, does not throw */
   ~shared_recursive_mutex();

   // Friend classes
   friend class boost::shmem::lock_ops<shared_recursive_mutex>;
   typedef boost::shmem::scoped_lock<shared_recursive_mutex> scoped_lock;

 private:

   /*!Locks mutex, sleeps when mutex is already locked.
      Throws process_resource_exception if a severe error is found*/
   void do_lock(void);

   /*! Unlocks the mutex */
   void do_unlock(void);

   //For compatibility with thread locks
   typedef char cv_state;

   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   shared_mutex         m_shared_mutex;
   unsigned int         m_nLockCount;
   unsigned long        m_nOwner;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   pthread_mutex_t      m_mut;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

/*!Wraps a mutex that can be placed in shared memory and can be 
   shared between processes. Allows several locking calls by the same 
   process. Allows lock tries*/
class shared_recursive_try_mutex : private boost::noncopyable
{
 public:
   /*!Creates mutex resources, throws process_resource_error
      if there are available resources */
   shared_recursive_try_mutex();

   /*!Destructs resources, does not throw */
  ~shared_recursive_try_mutex();

   // Friend classes
   friend class boost::shmem::lock_ops<shared_recursive_try_mutex>;
   typedef boost::shmem::scoped_lock<shared_recursive_try_mutex> 
      scoped_lock;
   typedef boost::shmem::scoped_try_lock<shared_recursive_try_mutex> 
      scoped_try_lock;

 private:

   /*!Locks mutex, sleeps when mutex is already locked.
      Throws process_resource_exception if a severe error is found*/
   void do_lock(void);

   /*! Tries to lock the mutex, returns false when mutex 
      is already locked, returns true when success.
      Throws process_resource_exception if a severe error is found*/
   bool do_trylock(void);

   /*! Unlocks the mutex */
   void do_unlock(void);

   //For compatibility with thread locks
   typedef char cv_state;

   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   shared_try_mutex     m_shared_try_mutex;
   unsigned int         m_nLockCount;
   unsigned long        m_nOwner;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   pthread_mutex_t      m_mut;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

/*!Wraps a mutex that can be placed in shared memory and can be 
   shared between processes. Allows several locking calls by the same 
   process. Allows timed lock tries*/
class shared_recursive_timed_mutex : private boost::noncopyable
{
public:
   friend class boost::shmem::lock_ops<shared_recursive_timed_mutex>;
   friend class shared_recursive_try_mutex;
   friend class shared_condition;
   typedef boost::shmem::scoped_lock
      <shared_recursive_timed_mutex>               scoped_lock;
   typedef boost::shmem::scoped_try_lock
      <shared_recursive_timed_mutex>               scoped_try_lock;
   typedef boost::shmem::scoped_timed_lock
      <shared_recursive_timed_mutex>               scoped_timed_lock;

   shared_recursive_timed_mutex();
  ~shared_recursive_timed_mutex();

private:
   //For compatibility with thread locks
   typedef char cv_state;

   /*!Locks mutex, sleeps when mutex is already locked.
      Throws process_resource_exception if a severe error is found*/
   void do_lock(void);

   /*! Tries to lock the mutex, returns false when mutex 
      is already locked, returns true when success.
      Throws process_resource_exception if a severe error is found*/
   bool do_trylock(void);

   /*! Tries to lock the mutex, if mutex can't be locked before
      xt time, returns false.
      Throws process_resource_exception if a severe error is found*/
   bool do_timedlock(const xtime& xt);

   /*! Unlocks the mutex */
   void do_unlock(void);

   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   shared_timed_mutex   m_shared_timed_mutex;
   unsigned int         m_nLockCount;
   unsigned long        m_nOwner;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   pthread_mutex_t m_mut;
   pthread_cond_t m_unlocked;
   pthread_t m_thread_id;
   bool m_valid_id;
   unsigned m_count;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

}  //namespace shmem {

}  //namespace boost {

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/shared_recursive_mutex.hpp>
#else 
#  include <boost/shmem/sync/posix/shared_recursive_mutex.hpp>
#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_RECURSIVE_SHARED_MUTEX_HPP
