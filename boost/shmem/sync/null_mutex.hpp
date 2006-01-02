//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_NULL_MUTEX_HPP
#define BOOST_NULL_MUTEX_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/sync/lock.hpp>

/*!\file
   Describes null_mutex classes
*/

namespace boost {

namespace shmem {

struct xtime;
class shared_condition;

/*!Wraps a mutex that does nothing */
class null_mutex
{
   null_mutex(const null_mutex&);
   null_mutex &operator= (const null_mutex&);
 public:

   /*!Empty constructor */
   null_mutex(){}

   /*!Empty destructor */
  ~null_mutex(){}

   // Friend classes
   friend class boost::shmem::lock_ops<null_mutex>;
   typedef boost::shmem::scoped_lock<null_mutex> 
      scoped_lock;
   typedef boost::shmem::scoped_try_lock<null_mutex>   
      scoped_try_lock;
   friend class shared_condition;
 private:

   /*!Does nothing */
   void do_lock(){}

   /*!Returns true */
   bool do_trylock(){  return true;   }

   /*!Returns true */
   bool do_timedlock(const xtime &time){  return true;   }

   /*!Does nothing */
   void do_unlock(){}
   
   //For compatibility with thread locks
   typedef char cv_state;

#if BOOST_WORKAROUND(__BORLANDC__, BOOST_TESTED_AT(0x564))
 private:
    char dummy_; // BCB would by default use 8 B for empty structure
#endif
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_NULL_MUTEX_HPP
