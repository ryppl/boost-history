//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2007. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_NAMED_RECURSIVE_MUTEX_HPP
#define BOOST_INTERPROCESS_NAMED_RECURSIVE_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/detail/posix_time_types_wrk.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/detail/managed_open_or_create_impl.hpp>
#include <boost/interprocess/sync/interprocess_recursive_mutex.hpp>

/*!\file
   Describes a named interprocess_mutex class for inter-process synchronization
*/

namespace boost {
namespace interprocess {

/*!A recursive mutex with a global name, so it can be found from different 
   processes. This mutex can't be placed in shared memory, and
   each process should have it's own named_recursive_mutex.*/
class named_recursive_mutex
{
   /// @cond
   //Non-copyable
   named_recursive_mutex();
   named_recursive_mutex(const named_recursive_mutex &);
   named_recursive_mutex &operator=(const named_recursive_mutex &);
   /// @endcond
   public:
   /*!Creates a global interprocess_mutex with a name.*/
   named_recursive_mutex(create_only_t create_only, const char *name);

   /*!Opens or creates a global interprocess_mutex with a name. 
      If the interprocess_mutex is created, this call is equivalent to create(). 
      If the interprocess_mutex is already created, this call is equivalent to open(). 
      Does not throw*/
   named_recursive_mutex(open_or_create_t open_or_create, const char *name);

   /*!Opens a global interprocess_mutex with a name if that interprocess_mutex is previously.
      created. If it is not previously created this function return false.
      Does not throw*/
   named_recursive_mutex(open_only_t open_only, const char *name);

   /*!Destroys named interprocess_mutex. Does not throw*/
   ~named_recursive_mutex();

   /*!Unlocks a previously locked interprocess_mutex.*/
   void unlock();

   /*!Locks interprocess_mutex, sleeps when interprocess_mutex is already locked.
      Throws interprocess_exception if a severe error is found*/
   void lock();

   /*! Tries to lock the interprocess_mutex, returns false when interprocess_mutex 
      is already locked, returns true when success.
      Throws interprocess_exception if a severe error is found*/
   bool try_lock();

   /*! Tries to lock the interprocess_mutex until time abs_time,
      Returns false when timeout expires, returns true when locks.
      Throws interprocess_exception if a severe error is found*/
   bool timed_lock(const boost::posix_time::ptime &abs_time);

   /*! Erases a named recursive mutex from the system*/
   static bool remove(const char *name);

   /// @cond
   private:
   interprocess_recursive_mutex *mutex() const
   {  return static_cast<interprocess_recursive_mutex*>(m_shmem.get_address()); }

   detail::managed_open_or_create_impl<shared_memory_object> m_shmem;

   class construct_func_t;
   /// @endcond
};

/// @cond
class named_recursive_mutex::construct_func_t
{
   public:
   enum CreationType {  open_only, open_or_create, create_only  };

   construct_func_t(CreationType type)
      :  m_creation_type(type){}

   bool operator()(void *address, std::size_t, bool created) const
   {   
      switch(m_creation_type){
         case open_only:
            return true;
         break;
         case create_only:
         case open_or_create:
            if(created){
               new(address)interprocess_recursive_mutex;
            }
            return true;
         break;

         default:
            return false;
         break;
      }
      return true;
   }
   private:
   CreationType       m_creation_type;
};
/// @endcond

inline named_recursive_mutex::~named_recursive_mutex()
{}

inline named_recursive_mutex::named_recursive_mutex(create_only_t, const char *name)
   :  m_shmem  (create_only
               ,name
               ,sizeof(interprocess_recursive_mutex) +
                  detail::managed_open_or_create_impl<shared_memory_object>::
                     ManagedOpenOrCreateUserOffset
               ,read_write
               ,0
               ,construct_func_t(construct_func_t::create_only))
{}

inline named_recursive_mutex::named_recursive_mutex(open_or_create_t, const char *name)
   :  m_shmem  (open_or_create
               ,name
               ,sizeof(interprocess_recursive_mutex) +
                  detail::managed_open_or_create_impl<shared_memory_object>::
                     ManagedOpenOrCreateUserOffset
               ,read_write
               ,0
               ,construct_func_t(construct_func_t::open_or_create))
{}

inline named_recursive_mutex::named_recursive_mutex(open_only_t, const char *name)
   :  m_shmem  (open_only
               ,name
               ,read_write
               ,0
               ,construct_func_t(construct_func_t::open_only))
{}

inline void named_recursive_mutex::lock()
{  this->mutex()->lock();  }

inline void named_recursive_mutex::unlock()
{  this->mutex()->unlock();  }

inline bool named_recursive_mutex::try_lock()
{  return this->mutex()->try_lock();  }

inline bool named_recursive_mutex::timed_lock(const boost::posix_time::ptime &abs_time)
{  return this->mutex()->timed_lock(abs_time);  }

inline bool named_recursive_mutex::remove(const char *name)
{  return shared_memory_object::remove(name); }

}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //BOOST_INTERPROCESS_NAMED_RECURSIVE_MUTEX_HPP
