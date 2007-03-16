//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_NAMED_SEMAPHORE_HPP
#define BOOST_NAMED_SEMAPHORE_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/noncopyable.hpp>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>

#else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <fcntl.h>      //O_CREAT, O_*... 
#include <unistd.h>     //close
#include <string>       //std::string
#include <semaphore.h>  //sem_* family, SEM_VALUE_MAX
#include <sys/stat.h>   //mode_t, S_IRWXG, S_IRWXO, S_IRWXU,
#include <boost/shmem/shared_memory.hpp>   //for shared_memory

#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

/*!\file
   Describes a named semaphore class for inter-process synchronization
*/

namespace boost {

namespace shmem {

/*!A semaphore with a global name, so it can be found from different 
   processes. Allows several resource sharing patterns and efficient 
   acknowledgment mechanisms.*/
class named_semaphore : private boost::noncopyable
{
 public:
   /*!Initializes member variables. Does not throw*/
   named_semaphore();

   /*!Calls close(). Does not throw*/
   ~named_semaphore();

   /*!Creates a global semaphore with a name, and an initial count. 
      It will return an false if the semaphore is already created. 
      Does not throw*/
   bool create(const char *name, int initialCount);

   /*!Opens or creates a global semaphore with a name, and an initial count. 
      If the semaphore is created, this call is equivalent to create(). 
      If the semaphore is already created, this call is equivalent to open()
      and initialCount is ignored. Does not throw*/
   bool open_or_create(const char *name, int initialCount);

   /*!Opens a global semaphore with a name if that semaphore is previously.
      created. If it is not previously created this function return false.
      Does not throw*/
   bool open(const char *name);

   /*!Frees system resources. Does not throw*/
   void close();

   /*!Increments semaphore count. If there is any waiting process, wakes
      up the process. If there is an error throws sem_exception*/
   void post();

   /*!Waits for the semaphore to the posted if count is 0. 
      If there is an error throws sem_exception*/
   void wait();

   /*!Waits for the semaphore to the posted if count is 0 or until
      timeout expires. Returns true if wait was ok, false if timeout
      expires. If there is an error throws sem_exception*/
   bool timed_wait(const xtime& xt);

   /*!Tries to wait for the semaphore to the posted if count is 0 or until
      timeout expires. Returns true if wait was ok, false if wait was not
      successful. If there is an error throws sem_exception*/
   bool try_wait();

 private:
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   void*          mp_sem;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   sem_t *        mp_sem;
   shared_memory  m_shmem;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

}  //namespace shmem {

}  //namespace boost {

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/named_semaphore.hpp>
#else 
#  include <boost/shmem/sync/posix/named_semaphore.hpp>
#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_NAMED_SEMAPHORE_HPP
