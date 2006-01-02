//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_PTHREAD_HELPERS_HPP
#define BOOST_SHMEM_PTHREAD_HELPERS_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <pthread.h>
#include <errno.h>   
#include <boost/shmem/exceptions.hpp>   

namespace boost {

namespace shmem {

namespace detail{

   /*!Makes pthread_mutexattr_t cleanup easy when using exceptions*/
   struct mutexattr_wrapper 
   {
      /*!Constructor*/
      mutexattr_wrapper(bool recursive = false)
      {
         if(pthread_mutexattr_init(&m_attr)!=0 ||
            pthread_mutexattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED)!= 0 ||
             (recursive &&
              pthread_mutexattr_settype(&m_attr, PTHREAD_MUTEX_RECURSIVE)!= 0 ))
            throw boost::shmem::process_resource_exception();
      }

      /*!Destructor*/
     ~mutexattr_wrapper()  {  pthread_mutexattr_destroy(&m_attr);  }

      /*!This allows using mutexattr_wrapper as pthread_mutexattr_t*/
      operator pthread_mutexattr_t&()  {  return m_attr;  }

      pthread_mutexattr_t m_attr;
   };

   /*!Makes pthread_condattr_t cleanup easy when using exceptions*/
   struct condattr_wrapper 
   {
      /*!Constructor*/
      condattr_wrapper()
      {
         if(pthread_condattr_init(&m_attr)!=0 ||
            pthread_condattr_setpshared(&m_attr, PTHREAD_PROCESS_SHARED)!= 0)
            throw boost::shmem::process_resource_exception();
      }

      /*!Destructor*/
     ~condattr_wrapper() { pthread_condattr_destroy(&m_attr); }

      /*!This allows using condattr_wrapper as pthread_condattr_t*/
      operator pthread_condattr_t&(){  return m_attr;  }

      pthread_condattr_t m_attr;
   };

   /*!Makes initialized pthread_mutex_t cleanup easy when using exceptions*/
   class mutex_initializer
   {
    public:
      /*!Constructor. Takes mutex attributes to initialize the mutex*/
      mutex_initializer(pthread_mutex_t &mut, pthread_mutexattr_t &mut_attr)
      : mp_mut(&mut)
      {
         if(pthread_mutex_init(mp_mut, &mut_attr) != 0)
            throw boost::shmem::process_resource_exception();
      }

     ~mutex_initializer() {  if(mp_mut) pthread_mutex_destroy(mp_mut);  }

      void release() {mp_mut = 0; }

    private:     
      pthread_mutex_t *mp_mut;
   };

   /*!Makes initialized pthread_cond_t cleanup easy when using exceptions*/
   class condition_initializer
   {
    public:
      condition_initializer(pthread_cond_t &cond, pthread_condattr_t &cond_attr) 
      : mp_cond(&cond)
      {
         if(pthread_cond_init(mp_cond, &cond_attr)!= 0)
            throw boost::shmem::process_resource_exception();
      }

     ~condition_initializer()   {  if(mp_cond) pthread_cond_destroy(mp_cond);  }

      void release()       { mp_cond = 0; }

    private:   
      pthread_cond_t *mp_cond;
   };

}//namespace detail

}//namespace shmem

}//namespace boost

#endif //ifdef BOOST_SHMEM_PTHREAD_HELPERS_HPP
