//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_FILE_LOCK_HPP
#define BOOST_INTERPROCESS_FILE_LOCK_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <assert.h>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/detail/posix_time_types_wrk.hpp>

/*!\file
   Describes a class that wraps file locking capabilities.
*/

namespace boost {
namespace interprocess {

/*!A file lock, is a mutual exclusion utility similar to a mutex using a
   file. A file lock has sharable and exclusive locking capabilities and
   can be used with scoped_lock and sharable_lock classes.*/
class file_lock
{
   /// @cond
   //Non-copyable
   file_lock();
   file_lock(const file_lock &);
   file_lock &operator=(const file_lock &);
   /// @endcond
   public:
   /*!Opens a file lock. Throws interprocess_exception if the file does not
      exist or there are no operating system resources.*/
   file_lock(const char *name);

   /*!Closes a file lock. Does not throw.*/
   ~file_lock();
   
   //Exclusive locking

   /*!Effects: The calling thread tries to obtain exclusive ownership of the mutex,
         and if another thread has exclusive, or sharable ownership of
         the mutex, it waits until it can obtain the ownership.
      Throws: interprocess_exception on error.*/
   void lock();

   /*!Effects: The calling thread tries to acquire exclusive ownership of the mutex
         without waiting. If no other thread has exclusive, or sharable
         ownership of the mutex this succeeds.
      Returns: If it can acquire exclusive ownership immediately returns true.
         If it has to wait, returns false.
      Throws: interprocess_exception on error.*/
   bool try_lock();

   /*!Effects: The calling thread tries to acquire exclusive ownership of the mutex
         waiting if necessary until no other thread has has exclusive, or sharable
         ownership of the mutex or abs_time is reached.
      Returns: If acquires exclusive ownership, returns true. Otherwise returns false. 
      Throws: interprocess_exception on error.*/
   bool timed_lock(const boost::posix_time::ptime &abs_time);

   /*!Precondition: The thread must have exclusive ownership of the mutex. 
      Effects: The calling thread releases the exclusive ownership of the mutex. 
      Throws: An exception derived from interprocess_exception on error.*/
   void unlock();

   //Sharable locking

   /*!Effects: The calling thread tries to obtain sharable ownership of the mutex,
         and if another thread has exclusive ownership of the mutex, waits until
         it can obtain the ownership.
      Throws: interprocess_exception on error.*/
   void lock_sharable();

   /*!Effects: The calling thread tries to acquire sharable ownership of the mutex
         without waiting. If no other thread has has exclusive ownership of the
         mutex this succeeds. 
      Returns: If it can acquire sharable ownership immediately returns true. If it
         has to wait, returns false. 
      Throws: interprocess_exception on error.*/
   bool try_lock_sharable();

   /*!Effects: The calling thread tries to acquire sharable ownership of the mutex
         waiting if necessary until no other thread has has exclusive ownership of
         the mutex or abs_time is reached. 
      Returns: If acquires sharable ownership, returns true. Otherwise returns false. 
      Throws: interprocess_exception on error.*/
   bool timed_lock_sharable(const boost::posix_time::ptime &abs_time);

   /*!Precondition: The thread must have sharable ownership of the mutex. 
      Effects: The calling thread releases the sharable ownership of the mutex. 
      Throws: An exception derived from interprocess_exception on error.*/
   void unlock_sharable();
   /// @cond
   private:
   file_handle_t m_file_hnd;

   bool timed_acquire_file_lock
      (file_handle_t hnd, bool &acquired, const boost::posix_time::ptime &abs_time)
   {  
      //Obtain current count and target time
      boost::posix_time::ptime now = microsec_clock::universal_time();
      using namespace boost::detail;

      if(now >= abs_time) return false;

      do{
         if(!try_acquire_file_lock(hnd, acquired))
            return false;

         if(acquired)
            return true;
         else{
            now = microsec_clock::universal_time();

            if(now >= abs_time){
               acquired = false;
               return true;
            }
            // relinquish current time slice
            winapi::sched_yield();
         }
      }while (true);
   }

   bool timed_acquire_file_lock_sharable
      (file_handle_t hnd, bool &acquired, const boost::posix_time::ptime &abs_time)
   {  
      //Obtain current count and target time
      boost::posix_time::ptime now = microsec_clock::universal_time();
      using namespace boost::detail;

      if(now >= abs_time) return false;

      do{
         if(!try_acquire_file_lock_sharable(hnd, acquired))
            return false;

         if(acquired)
            return true;
         else{
            now = microsec_clock::universal_time();

            if(now >= abs_time){
               acquired = false;
               return true;
            }
            // relinquish current time slice
            winapi::sched_yield();
         }
      }while (true);
   }

   bool timed_acquire_file_lock
      (file_handle_t hnd, bool &acquired, const boost::posix_time::ptime &abs_time)
   {
      //Obtain current count and target time
      boost::posix_time::ptime now = microsec_clock::universal_time();
      using namespace boost::detail;

      if(now >= abs_time) return false;

      do{
         if(!try_acquire_file_lock(hnd, acquired))
            return false;

         if(acquired)
            return true;
         else{
            now = microsec_clock::universal_time();

            if(now >= abs_time){
               acquired = false;
               return true;
            }
            // relinquish current time slice
            sleep(0);
         }
      }while (true);
   }

   bool timed_acquire_file_lock_sharable
      (file_handle_t hnd, bool &acquired, const boost::posix_time::ptime &abs_time)
   {  
      //Obtain current count and target time
      boost::posix_time::ptime now = microsec_clock::universal_time();
      using namespace boost::detail;

      if(now >= abs_time) return false;

      do{
         if(!try_acquire_file_lock_sharable(hnd, acquired))
            return false;

         if(acquired)
            return true;
         else{
            now = microsec_clock::universal_time();

            if(now >= abs_time){
               acquired = false;
               return true;
            }
            // relinquish current time slice
            ::sleep(0);
         }
      }while (true);
   }
   /// @endcond
};

inline file_lock::file_lock(const char *name)
{
   m_file_hnd = detail::open_existing_file(name);

   if(m_file_hnd == detail::invalid_file()){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
}

inline file_lock::~file_lock()
{
   if(m_file_hnd != detail::invalid_file()){
      detail::close_file(m_file_hnd);
      m_file_hnd = detail::invalid_file();
   }
}

inline void file_lock::lock()
{
   if(!detail::acquire_file_lock(m_file_hnd)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
}

inline bool file_lock::try_lock()
{
   bool result;
   if(!detail::try_acquire_file_lock(m_file_hnd, result)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
   return result;
}

inline bool file_lock::timed_lock(const boost::posix_time::ptime &abs_time)
{
   bool result;
   if(!detail::timed_acquire_file_lock(m_file_hnd, result, abs_time)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
   return result;
}

inline void file_lock::unlock()
{
   if(!detail::release_file_lock(m_file_hnd)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
}

inline void file_lock::lock_sharable()
{
   if(!detail::acquire_file_lock_sharable(m_file_hnd)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
}

inline bool file_lock::try_lock_sharable()
{
   bool result;
   if(!detail::try_acquire_file_lock_sharable(m_file_hnd, result)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
   return result;
}

inline bool file_lock::timed_lock_sharable(const boost::posix_time::ptime &abs_time)
{
   bool result;
   if(!detail::timed_acquire_file_lock_sharable(m_file_hnd, result, abs_time)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
   return result;
}

inline void file_lock::unlock_sharable()
{
   if(!detail::release_file_lock_sharable(m_file_hnd)){
      error_info err(system_error_code());
      throw interprocess_exception(err);
   }
}

}  //namespace interprocess {

}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_FILE_LOCK_HPP
