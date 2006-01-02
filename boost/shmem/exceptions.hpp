//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_EXCEPTIONS_HPP
#define BOOST_SHMEM_EXCEPTIONS_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <stdexcept>
#include <new>


/*!\file
   Describes exceptions thrown by shmem classes
*/

namespace boost {

namespace shmem {

/*!This is the exception thrown by shared mutex family when a deadlock situation
   is detected or when using a shared_condition the mutex is not locked*/
class lock_exception : public std::exception
{
 public:
    virtual const char* what() const throw()
      {  return "boost::shmem::lock_exception";  }
};

/*!This is the exception thrown by named semaphore when a deadlock situation
   is detected or when an error is detected in the post/wait operation*/
class sem_exception : public std::exception
{
 public:
    virtual const char* what() const throw()
      {  return "boost::shmem::sem_exception";  }
};

/*!This exception is thrown by shared_mutex family and shared_condition 
   constructors if there are no operating system resources*/
class process_resource_exception : public std::exception
{
 public:
    virtual const char* what() const throw()
      {  return "boost::shmem::process_resource_exception";  }
};

/*!This exception is thrown when a memory request can't be fulfilled.*/
class bad_alloc : public std::bad_alloc
{
 public:
    virtual const char* what() const throw()
      {  return "boost::shmem::bad_alloc";  }
};

}  // namespace shmem {

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif // BOOST_SHMEM_EXCEPTIONS_HPP
