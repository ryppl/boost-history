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
// shared_barrier is a modified version of Boost Threads barrier
//
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2002-2003
// David Moore, William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.

#ifndef BOOST_SHARED_BARRIER_HPP
#define BOOST_SHARED_BARRIER_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/sync/shared_condition.hpp>
#include <stdexcept>

namespace boost {

namespace shmem {

/*!An object of class shared_barrier is a synchronization primitive that 
   can be placed in shared memory used to cause a set of threads from 
   different processes to wait until they each perform a certain 
   function or each reach a particular point in their execution.*/
class shared_barrier
{
 public:
   /*!Constructs a barrier object that will cause count threads 
      to block on a call to wait(). */
   shared_barrier(unsigned int count);
   /*!Destroys *this. If threads are still executing their wait() 
      operations, the behavior for these threads is undefined.*/
  ~shared_barrier(){};
   /*!Effects: Wait until N threads call wait(), where N equals the count 
      provided to the constructor for the barrier object.
      Note that if the barrier is destroyed before wait() can return, 
      the behavior is undefined.
      Returns: Exactly one of the N threads will receive a return value 
      of true, the others will receive a value of false. Precisely which 
      thread receives the return value of true will be implementation-defined. 
      Applications can use this value to designate one thread as a leader that 
      will take a certain action, and the other threads emerging from the barrier 
      can wait for that action to take place.*/
   bool wait();

 private:
   shared_mutex m_mutex;
   shared_condition m_cond;
   unsigned int m_threshold;
   unsigned int m_count;
   unsigned int m_generation;
};

inline shared_barrier::shared_barrier(unsigned int count)
   : m_threshold(count), m_count(count), m_generation(0)
{
    if (count == 0)
        throw std::invalid_argument("count cannot be zero.");
}

inline bool shared_barrier::wait()
{
   boost::shmem::shared_mutex::scoped_lock lock(m_mutex);
   unsigned int gen = m_generation;

   if (--m_count == 0){
      m_generation++;
      m_count = m_threshold;
      m_cond.notify_all();
      return true;
   }

   while (gen == m_generation){
      m_cond.wait(lock);
   }
   return false;
}

}  // namespace shmem

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif
