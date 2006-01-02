//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_NAMED_MUTEX_HPP
#define BOOST_NAMED_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/noncopyable.hpp>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>

#else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <unistd.h>     //close
#include <string>       //std::string
#include <pthread.h>    //sem_* family, SEM_VALUE_MAX
#include <boost/shmem/sync/posix/pthread_helpers.hpp>
#include <boost/shmem/shared_memory.hpp>   //for shared_memory

#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

/*!\file
   Describes a named mutex class for inter-process synchronization
*/

namespace boost {

namespace shmem {

/*!A mutex with a global name, so it can be found from different 
   processes. This mutex can't be placed in shared memory, and
   each process should have it's own mutex.*/
class named_mutex : private boost::noncopyable
{
 public:
   /*!Initializes member variables. Does not throw*/
   named_mutex();

   /*!Calls close(). Does not throw*/
  ~named_mutex();

   /*!Creates a global mutex with a name. 
      It will return an false if the mutex is already created. 
      Does not throw*/
   bool create(const char *name);

   /*!Opens or creates a global mutex with a name. 
      If the mutex is created, this call is equivalent to create(). 
      If the mutex is already created, this call is equivalent to open(). 
      Does not throw*/
   bool open_or_create(const char *name);

   /*!Opens a global mutex with a name if that mutex is previously.
      created. If it is not previously created this function return false.
      Does not throw*/
   bool open(const char *name);

   /*!Closes the mutex. Does not throw*/
   void close();

   /*!Unlocks a previously locked mutex.*/
   void unlock();

   /*!Locks mutex, sleeps when mutex is already locked.
      Throws process_resource_exception if a severe error is found*/
   void lock();

   /*! Tries to lock the mutex, returns false when mutex 
      is already locked, returns true when success.
      Throws process_resource_exception if a severe error is found*/
   bool try_lock();

   /*! Tries to lock the mutex until time xt,
      Returns false when timeout expires, returns true when locks.
      Throws process_resource_exception if a severe error is found*/
   bool timed_lock(const xtime& xt);

 private:
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   void*          mp_mut;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   sem_t *        mp_mut;
   shared_memory  m_shmem;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

}  //namespace shmem {

}  //namespace boost {

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/named_mutex.hpp>
#else 
#  include <boost/shmem/sync/posix/named_mutex.hpp>
#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_NAMED_MUTEX_HPP
