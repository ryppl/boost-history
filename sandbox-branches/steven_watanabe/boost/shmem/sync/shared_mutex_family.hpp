//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_MUTEX_FAMILY_HPP
#define BOOST_SHMEM_MUTEX_FAMILY_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/sync/shared_recursive_mutex.hpp>
#include <boost/shmem/sync/null_mutex.hpp>

/*!\file
   Describes a shared mutex family fit algorithm used to allocate objects in shared memory.
*/

namespace boost {

namespace shmem {

/*!Describes mutex family to use with Shmem frameworks 
   based on boost::shmem synchronization objects.*/
struct shared_mutex_family
{
   typedef boost::shmem::shared_mutex                 mutex_t;
   typedef boost::shmem::shared_try_mutex             try_mutex_t;
   typedef boost::shmem::shared_timed_mutex           timed_mutex_t;
   typedef boost::shmem::shared_recursive_mutex       recursive_mutex_t;
   typedef boost::shmem::shared_recursive_try_mutex   recursive_try_mutex_t;
   typedef boost::shmem::shared_recursive_timed_mutex recursive_timed_mutex_t;
};

/*!Describes mutex family to use with Shmem frameworks 
   based on null operation synchronization objects.*/
struct null_mutex_family
{
   typedef boost::shmem::null_mutex                   mutex_t;
   typedef boost::shmem::null_mutex                   try_mutex_t;
   typedef boost::shmem::null_mutex                   timed_mutex_t;
   typedef boost::shmem::null_mutex                   recursive_mutex_t;
   typedef boost::shmem::null_mutex                   recursive_try_mutex_t;
   typedef boost::shmem::null_mutex                   recursive_timed_mutex_t;
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_MUTEX_FAMILY_HPP


