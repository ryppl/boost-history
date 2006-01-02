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

namespace boost {

namespace shmem {

inline shared_recursive_mutex::shared_recursive_mutex() 
   : m_nLockCount(0), m_nOwner(0xffffffff){}

inline shared_recursive_mutex::~shared_recursive_mutex(){}

inline void shared_recursive_mutex::do_lock()
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner){
      m_nLockCount++;
   }
   else{
      m_shared_mutex.do_lock();
      m_nOwner = pNumber;
      m_nLockCount++;
   }
}

inline void shared_recursive_mutex::do_unlock()
{
   unsigned long pNumber = current_thread_id();
   assert(pNumber == m_nOwner);
   m_nLockCount--;
   if(m_nLockCount == 0){
      m_nOwner = 0xffffffff;
      m_shared_mutex.do_unlock();
   }
}

inline shared_recursive_try_mutex::shared_recursive_try_mutex() 
   : m_nLockCount(0), m_nOwner(0xffffffff){}

inline shared_recursive_try_mutex::~shared_recursive_try_mutex(){}

inline void shared_recursive_try_mutex::do_lock()
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner){
      m_nLockCount++;
   }
   else{
      m_shared_try_mutex.do_lock();
      m_nOwner = pNumber;
      m_nLockCount++;
   }
}

inline bool shared_recursive_try_mutex::do_trylock()
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner) {  // we own it
      m_nLockCount++;
      return true;
   }
   if(m_shared_try_mutex.do_trylock()){
      m_nOwner = pNumber;
      m_nLockCount++;
      return true;
   }
   return false;
}

inline void shared_recursive_try_mutex::do_unlock()
{
   unsigned long pNumber = current_thread_id();
   assert(pNumber == m_nOwner);
   m_nLockCount--;
   if(m_nLockCount == 0){
      m_nOwner = 0xffffffff;
      m_shared_try_mutex.do_unlock();
   }
}

inline shared_recursive_timed_mutex::shared_recursive_timed_mutex() 
   : m_nLockCount(0), m_nOwner(0xffffffff){}

inline shared_recursive_timed_mutex::~shared_recursive_timed_mutex(){}

inline void shared_recursive_timed_mutex::do_lock()
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner){
      m_nLockCount++;
   }
   else{
      m_shared_timed_mutex.do_lock();
      m_nOwner = pNumber;
      m_nLockCount++;
   }
}

inline bool shared_recursive_timed_mutex::do_trylock()
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner) {  // we own it
      m_nLockCount++;
      return true;
   }
   if(m_shared_timed_mutex.do_trylock()){
      m_nOwner = pNumber;
      m_nLockCount++;
      return true;
   }
   return false;
}

inline bool shared_recursive_timed_mutex::do_timedlock(const xtime &xt)
{
   unsigned long pNumber = current_thread_id();
   if(pNumber == m_nOwner) {  // we own it
      m_nLockCount++;
      return true;
   }
   if(m_shared_timed_mutex.do_timedlock(xt)){
      m_nOwner = pNumber;
      m_nLockCount++;
      return true;
   }
   return false;
}

inline void shared_recursive_timed_mutex::do_unlock()
{
   unsigned long pNumber = current_thread_id();
   assert(pNumber == m_nOwner);
   m_nLockCount--;
   if(m_nLockCount == 0){
      m_nOwner = 0xffffffff;
      m_shared_timed_mutex.do_unlock();
   }
}

}  //namespace shmem {

}  //namespace boost {

