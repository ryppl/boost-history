//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_READ_WRITE_LOCK_HPP
#define BOOST_SHMEM_READ_WRITE_LOCK_HPP
        
#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/sync/lock.hpp>
#include <boost/shmem/sync/shared_condition.hpp>
#include <assert.h>
#include <cstddef>

namespace boost {

namespace shmem {

/*!An object of class shared_read_write_mutex is a read_write_mutex that
   can be placed in shared memory so that it can be used to synchronize
   threads of different processes.*/
class shared_read_write_lock
{
   enum shared_rw_lock_policy
   {
      reader_priority,               //Prefer readers; can starve writers
      writer_priority,               //Prefer writers; can starve readers
      alternating,
      max_policy
   //   rw_alternating_many_reads,        //Alternate readers and writers; before a writer, release all queued readers 
   //   rw_alternating_single_read        //Alternate readers and writers; before a writer, release only on queued reader
   };

   public:
   shared_read_write_lock(shared_rw_lock_policy policy = reader_priority)
   :  m_policy (policy), 
      m_r_active_completed_shared_count(0), m_r_wait_shared_count(0), m_w_wait_exclusive_count(0), m_w_active(0),
      m_completed_shared_count(0), m_shared_count(0), m_exclusive_count(0)
   {}

   ~shared_read_write_lock()
      {}

   void readlock()
      {  this->do_readlock(false, 0); }

   bool readtrylock()
      {  return this->do_readlock(true, 0); }

   bool readtimedlock(const xtime &xt)
      {  return this->do_readlock(true, &xt); }

   void readunlock()
   {
      assert(m_policy < max_policy);
      switch(m_policy){
         case reader_priority:
         case writer_priority:
            {
               scoped_lock<shared_timed_mutex>lock(m_mutex_exclusive_mutex);
               --m_r_active_completed_shared_count;
               if(m_r_active_completed_shared_count == 0 && m_w_wait_exclusive_count > 0){
                  m_writer_cond.notify_one();
               }
            }
         break;
         case alternating:
            {
               do_alt_priority_unlock();
            }
         break;
      }
   }

   void writelock()
      {  this->do_writelock(false, 0);  }

   bool writetrylock()
      {  return this->do_writelock(true, 0);  }

   bool writetimedlock(const xtime &xt)
      {  return this->do_writelock(true, &xt);  }

   void writeunlock()
   {
      switch(m_policy){
         case reader_priority:
            {
               scoped_lock<shared_timed_mutex> lock(m_mutex_exclusive_mutex);
               m_w_active = 0;
               if(m_r_wait_shared_count > 0){
                  m_reader_shared_cond.notify_all();
               }
               else if (m_w_wait_exclusive_count > 0){
                  m_writer_cond.notify_one();
               }
            }
         case writer_priority:
            {
               scoped_lock<shared_timed_mutex> lock(m_mutex_exclusive_mutex);
               m_w_active = 0;
               if(m_w_wait_exclusive_count > 0){
                  m_writer_cond.notify_one();
               }
               else if (m_r_wait_shared_count > 0){
                  m_reader_shared_cond.notify_all();
               }
            }
         break;
         case alternating:
            do_alt_priority_unlock();
         break;
      }
   }

   private:

   //Anti-exception RAII
   struct counter_decrementer
   {
      counter_decrementer(std::size_t &count)
         : m_count(count){}
      ~counter_decrementer()
         {  --m_count;  }
      std::size_t &m_count;
   };

   struct a_priority_wlock_rollback
   {
      a_priority_wlock_rollback(shared_read_write_lock *lock)
      :  m_lock(lock){}

      ~a_priority_wlock_rollback()
      {
         if(m_lock){
            m_lock->m_shared_count = -m_lock->m_completed_shared_count;
            m_lock->m_completed_shared_count = 0;
         }
      }

      void release(){   m_lock = 0;   }

      private:
      shared_read_write_lock *m_lock;
   };


   bool do_readlock(bool timed, const xtime *pxt)
   {
      assert(m_policy < max_policy);
      switch(m_policy){
         case reader_priority:
            {
               scoped_lock<shared_timed_mutex>lock(m_mutex_exclusive_mutex);
               if(m_w_active && !this->do_rw_priority_r_wait(lock, timed, pxt)){
                  return false;
               }
               ++m_r_active_completed_shared_count;
            }
         break;
         case writer_priority:
            {
               scoped_lock<shared_timed_mutex>lock(m_mutex_exclusive_mutex);
               if((m_w_active || m_w_wait_exclusive_count > 0) &&
                  !this->do_rw_priority_r_wait(lock, timed, pxt)){
                  return false;
               }
               ++m_r_active_completed_shared_count;
            }
         break;
         //Hau errepasatu!!!
         case alternating:
            {
               if(timed && !pxt){
                  scoped_try_lock<shared_timed_mutex>lock
                     (m_mutex_exclusive_mutex);
                  if(!lock.locked())   return false;
                  
                  if(++m_shared_count == INT_MAX){
                     //Is better to throw in this case?
                     //m_mutex_shared_completed.do_lock();
                     scoped_lock<shared_timed_mutex>lock(m_mutex_shared_completed);
                     m_shared_count          -= m_completed_shared_count;
                     m_completed_shared_count = 0;
                  }
               }
               else if(timed){
                  scoped_timed_lock<shared_timed_mutex>lock
                     (m_mutex_exclusive_mutex, *pxt);
                  if(!lock.locked()) return false;
                  if(++m_shared_count == INT_MAX){
                     //Is better to throw in this case?
                     //m_mutex_shared_completed.do_lock();
                     scoped_timed_lock<shared_timed_mutex> lock
                        (m_mutex_shared_completed, *pxt);
                     if(!lock.locked()){
	                     ++m_completed_shared_count;
                        return false;
                     }
                     m_shared_count          -= m_completed_shared_count;
                     m_completed_shared_count = 0;
                     return true;
                  }
               }
               else{
                  scoped_lock<shared_timed_mutex>lock
                     (m_mutex_exclusive_mutex);
                  if(++m_shared_count == INT_MAX){
                     //Is better to throw in this case?
                     //m_mutex_shared_completed.do_lock();
                     scoped_lock<shared_timed_mutex>lock
                        (m_mutex_shared_completed);
                     m_shared_count          -= m_completed_shared_count;
                     m_completed_shared_count = 0;
                     return true;
                  }
               }
            }
         break;
      }
   }

   bool do_writelock(bool timed, const xtime *pxt)
   {   
      assert(m_policy < max_policy);
      switch(m_policy){
         case reader_priority:
         case writer_priority:
            {
               scoped_lock<shared_timed_mutex>lock(m_mutex_exclusive_mutex);
               if((m_w_active || m_r_active_completed_shared_count > 0) && 
                  !this->do_rw_priority_w_wait(lock, timed, pxt)){
                  return false;
               }
               m_w_active = 1;
            }
         break;

         case alternating:
            {
               //Initialize scoped locks
               scoped_timed_lock<shared_timed_mutex>lock_exclusive
                  (m_mutex_exclusive_mutex, false);
               scoped_timed_lock<shared_timed_mutex>lock_shared
                  (m_mutex_shared_completed, false);
               //Do initialization depending on lock type
               if(timed && !pxt){
                  if(!lock_exclusive.try_lock())return false;
                  if(!lock_shared.try_lock())   return false;
               }
               else if(timed){
                  if(!lock_exclusive.timed_lock(*pxt))return false;
                  if(!lock_shared.timed_lock(*pxt))   return false;
               }
               else{
                  lock_exclusive.lock();
                  lock_shared.lock();
               }
               //Now do common logic
               if(!do_alt_priority_w_wait(lock_shared, timed, pxt))
                  return false;
               //We left the mutexes locked
               lock_shared.release();
               lock_exclusive.release();
               return true;
            }
         break;
      }
   }

   bool do_rw_priority_r_wait(scoped_lock<shared_timed_mutex> &lock, bool timed, const xtime *pxt)
   {
      if(timed && !pxt) return false;
      //We should block
      ++m_r_wait_shared_count;
      counter_decrementer decrementer(m_r_wait_shared_count);
      do{
         if(timed){
            if(!m_reader_shared_cond.timed_wait(lock, *pxt))
               return false;
         }
         else{
            m_reader_shared_cond.wait(lock);
         }
      }while(m_w_active);
      return true;
   }

   bool do_rw_priority_w_wait(scoped_lock<shared_timed_mutex> &lock, bool timed, const xtime *pxt)
   {
      if(timed && !pxt) return false;
      //We should block
      ++m_w_wait_exclusive_count;
      counter_decrementer decrementer(m_w_wait_exclusive_count);
      do{
         if(timed && !m_writer_cond.timed_wait(lock, *pxt)){
            return false;
         }
         else{
            m_writer_cond.wait(lock);
         }
      }while(m_w_active || m_r_active_completed_shared_count > 0);
      return true;
   }

   bool do_alt_priority_w_wait(scoped_timed_lock<shared_timed_mutex> &lock_shared, bool timed, const xtime *pxt)
   {
      //This would be a try lock
      if(timed && !pxt) return false;

      if(m_exclusive_count == 0){
         if(m_completed_shared_count > 0){
            m_shared_count          -= m_completed_shared_count;
            m_completed_shared_count = 0;
         }
         if(m_shared_count > 0){
            //This logic is for timed a infinite waits
	         m_completed_shared_count = -m_shared_count;
            a_priority_wlock_rollback rollback(this);
	         do{
               if(timed){
                  if(!m_reader_shared_cond.timed_wait(lock_shared, *pxt)){
                     return false;
                  }
               }
               else{
                  m_reader_shared_cond.wait(lock_shared);
               }
	         }while (m_completed_shared_count < 0);
            m_shared_count = 0;
            rollback.release();
         }
      }/*
      else{
         //Is this case correct?
         lock_shared.release();
         lock_exclusive.release();
         return false;
      }*/

      ++m_exclusive_count;
      return true;
   }

   void do_alt_priority_unlock()
   {
      if (m_exclusive_count == 0){
         scoped_lock<shared_timed_mutex> lock(m_mutex_exclusive_mutex);
         if(++m_completed_shared_count == 0) {
            m_reader_shared_cond.notify_one();
         }
      }
      else{
         --m_exclusive_count;
         m_mutex_shared_completed.do_unlock();
         m_mutex_exclusive_mutex.do_unlock();
      }
   }

   private:
   const shared_rw_lock_policy   m_policy;
   //shared by all methods
   shared_timed_mutex            m_mutex_exclusive_mutex;
   shared_condition              m_reader_shared_cond;
   std::size_t                   m_r_active_completed_shared_count;
   std::size_t                   m_r_wait_shared_count;
   std::size_t                   m_w_wait_exclusive_count;
   //Only for reader-writer      
   shared_condition              m_writer_cond;
   std::size_t                   m_w_active;
   //Only for alternating
   shared_timed_mutex            m_mutex_shared_completed;
   int                           m_completed_shared_count;
   int                           m_shared_count;
   int                           m_exclusive_count;
};

}  // namespace shmem

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif

