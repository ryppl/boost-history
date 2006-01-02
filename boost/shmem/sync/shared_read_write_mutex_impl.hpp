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
// shared_read_write_mutex_impl is a modified version 
// of Boost Threads shared_read_write_mutex_impl
//
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C)  2002-2003
// David Moore, William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  David Moore makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//
//////////////////////////////////////////////////////////////////////////////
//
// A Boost::threads implementation of a synchronization
//   primitive which can allow multiple readers or a single
//   writer to have access to a shared resource.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_READ_WRITE_MUTEX_IMPL_HPP
#define BOOST_SHMEM_READ_WRITE_MUTEX_IMPL_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/sync/lock.hpp>
#include <boost/shmem/sync/shared_condition.hpp>
#include <assert.h>

namespace boost {

namespace shmem {

namespace read_write_scheduling_policy {
   enum read_write_scheduling_policy_enum
   {
      writer_priority,               //Prefer writers; can starve readers
      reader_priority,               //Prefer readers; can starve writers
      alternating_many_reads,        //Alternate readers and writers; before a writer, release all queued readers 
      alternating_single_read        //Alternate readers and writers; before a writer, release only on queued reader
   };
} // namespace read_write_scheduling_policy

namespace detail {

/*!Common member and functions for all shared_read_write_mutex_impl
   bindings...*/
struct shared_read_write_mutex_impl_common
{
 public: //Typedefs
   typedef read_write_scheduling_policy::read_write_scheduling_policy_enum
      sched_t;
   typedef boost::shmem::read_write_lock_state::read_write_lock_state_enum
      state_t;
 public: //Members
   boost::shmem::shared_condition m_waiting_writers;
   boost::shmem::shared_condition m_waiting_readers;
   int m_num_waiting_writers;
   int m_num_waiting_readers;
   int m_num_readers_to_wake;
   boost::shmem::shared_condition m_waiting_promotion;
   bool m_state_waiting_promotion;
   int m_state;   // -1 = excl locked
                  // 0 = unlocked
                  // 1-> INT_MAX - shared locked
   const read_write_scheduling_policy::read_write_scheduling_policy_enum m_sp;
   bool m_readers_next;

 public: //Members
   shared_read_write_mutex_impl_common
      (sched_t sp)
    : m_num_waiting_writers(0),
      m_num_waiting_readers(0),
      m_num_readers_to_wake(0),
      m_state_waiting_promotion(false),
      m_state(0),
      m_sp(sp),
      m_readers_next(true) { }

   state_t state();
   bool locked();

 public: //Static functions
   static inline bool valid_lock(int state)
      {  return (state >= 0) || (state == -1);  }

   static inline bool valid_read_write_lock(int state)
      {  return state != 0;   }

   static inline bool valid_read_lock(int state)
      {  return state > 0; }

   static inline bool valid_read_lockable(int state)
      {  return state >= 0;  }

   static inline bool valid_write_lock(int state)
      {  return state == -1;  }

   static inline bool valid_write_lockable(int state)
      {  return state == 0;   }
};

inline bool shared_read_write_mutex_impl_common::locked()
{
   int state = m_state;
   assert(valid_lock(state));
   return state != 0;
}

inline read_write_lock_state::read_write_lock_state_enum 
shared_read_write_mutex_impl_common::state()
{
   int state = m_state;
   assert(valid_lock(state));

   if (state > 0){
      assert(valid_read_lock(state));
      return read_write_lock_state::read_locked;
   }
   else if (state == -1){
      assert(valid_write_lock(state));
      return read_write_lock_state::write_locked;
   }
   else
      return read_write_lock_state::unlocked;
}

/*!Shared implementation construct for explicit Scheduling Policies
   This implementation is susceptible to self-deadlock, though....*/
template<typename Mutex>
struct shared_read_write_mutex_impl 
   : public shared_read_write_mutex_impl_common
{
 public:
   typedef Mutex mutex_type;
   typedef boost::shmem::scoped_lock<Mutex>         scoped_lock;
   typedef boost::shmem::scoped_try_lock<Mutex>     scoped_try_lock;
   typedef boost::shmem::scoped_timed_lock<Mutex>   scoped_timed_lock;

 public:
   Mutex m_prot;

 public:
   shared_read_write_mutex_impl(sched_t sp)
      : shared_read_write_mutex_impl_common(sp){}
   void do_read_lock();
   void do_read_unlock();
   void do_write_lock();
   void do_write_unlock();
   bool do_try_write_lock();
   bool do_try_read_lock();
   bool do_timed_write_lock(const xtime &xt);
   bool do_timed_read_lock(const xtime &xt);

   void do_demote_to_read_lock();
   bool do_try_demote_to_read_lock();
   bool do_timed_demote_to_read_lock(const xtime &xt);

   void do_promote_to_write_lock();
   bool do_try_promote_to_write_lock();
   bool do_timed_promote_to_write_lock(const xtime &xt);

 private:
   void do_unlock_scheduling_impl();
   void do_timeout_scheduling_impl();
   void do_demote_scheduling_impl();
   void do_scheduling_impl();

   bool do_demote_to_read_lock_impl();
};

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_read_lock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_lock(m_state));

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: wait while write-locked
      while (m_state == -1){
         ++m_num_waiting_readers;
         m_waiting_readers.wait(l);
         --m_num_waiting_readers;
      }
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Writer priority: wait while write-locked or while writers are waiting
      while (m_state == -1 || m_num_waiting_writers > 0){
         ++m_num_waiting_readers;
         m_waiting_readers.wait(l);
         --m_num_waiting_readers;
      }
   }
   else{
      //Alternating priority: wait while write-locked or while not readers' turn
      while (m_state == -1 || m_num_readers_to_wake == 0){
         ++m_num_waiting_readers;
         m_waiting_readers.wait(l);
         --m_num_waiting_readers;
      }

      assert(m_num_readers_to_wake > 0);
      --m_num_readers_to_wake;
   }

   //Obtain a read lock
   assert(valid_read_lockable(m_state));
   ++m_state;

   //Set m_readers_next in the lock function rather than the 
   //unlock function to prevent thread starvation that can happen,
   //e.g., like this: if all writer threads demote themselves
   //to reader threads before unlocking, they will unlock using 
   //do_read_unlock() which will set m_readers_next = false;
   //if there are enough writer threads, this will prevent any
   //"true" reader threads from ever obtaining the lock.
   m_readers_next = false;
   assert(valid_read_lock(m_state));
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_write_lock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_lock(m_state));

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: wait while locked or while readers are waiting

      while (m_state != 0 || m_num_waiting_readers > 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         m_waiting_writers.wait(l);
         --m_num_waiting_writers;
      }
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Shut down extra readers that were scheduled only because of no waiting writers
      m_num_readers_to_wake = 0;

      //Writer priority: wait while locked
      while (m_state != 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         m_waiting_writers.wait(l);
         --m_num_waiting_writers;
      }
   }
   else{
      //Shut down extra readers that were scheduled only because of no waiting writers
      if(m_sp == read_write_scheduling_policy::alternating_single_read 
         && m_num_waiting_writers == 0)
         m_num_readers_to_wake = (m_readers_next && m_num_readers_to_wake > 0) ? 1 : 0;

      //Alternating priority: wait while locked or while not writers' turn
      while (m_state != 0 || m_num_readers_to_wake > 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         m_waiting_writers.wait(l);
         --m_num_waiting_writers;
      }
   }

   //Obtain a write lock
   assert(valid_write_lockable(m_state));
   m_state = -1;

   //See note in shared_read_write_mutex_impl<>::do_read_lock() as to why 
   //m_readers_next should be set here
   m_readers_next = true;
   assert(valid_write_lock(m_state));
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_try_read_lock()
{
   typename Mutex::scoped_try_lock l(m_prot);
   assert(valid_lock(m_state));

   if (!l.locked()) return false;

   bool fail;

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: fail if write-locked
      fail = (m_state == -1);
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Writer priority: fail if write-locked or if writers are waiting
      fail = (m_state == -1 || m_num_waiting_writers > 0);
   }
   else{
      //Alternating priority: fail if write-locked or if not readers' turn
      fail = (m_state == -1 || m_num_readers_to_wake == 0);

      if (!fail){
         assert(m_num_readers_to_wake > 0);
         --m_num_readers_to_wake;
      }
   }

   if (!fail){
      //Obtain a read lock
      assert(valid_read_lockable(m_state));
      ++m_state;

      //See note in shared_read_write_mutex_impl<>::do_read_lock() as to why 
      //m_readers_next should be set here
      m_readers_next = false;
      assert(valid_read_lock(m_state));
         //Should be read-locked
   }
   else{
      assert(valid_write_lock(m_state) || m_num_waiting_writers > 0);
      //Should be write-locked or
      //writer should be waiting
   }

   return !fail;
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_try_write_lock()
{
   typename Mutex::scoped_try_lock l(m_prot);
   assert(valid_lock(m_state));

   if (!l.locked()) return false;

   bool fail;

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: fail if locked or if readers are waiting
      fail = (m_state != 0 || m_num_waiting_readers > 0);
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Writer priority: fail if locked
      fail = (m_state != 0);
   }
   else{
      //Alternating priority: fail if locked or if not writers' turn
      fail = (m_state != 0 || m_num_readers_to_wake > 0);
   }

   if (!fail){
      //Obtain a write lock
      assert(valid_write_lockable(m_state));
      m_state = -1;

      //See note in shared_read_write_mutex_impl<>::do_read_lock() as to why 
      //m_readers_next should be set here
      m_readers_next = true;
      assert(valid_write_lock(m_state));
         //Should be write-locked
   }
   else{
      assert(valid_read_write_lock(m_state) || m_num_readers_to_wake > 0);
      //Should be read-locked or write-locked, or
      //reader should be waking
   }

   return !fail;
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::
   do_timed_read_lock(const boost::shmem::xtime &xt)
{
   typename Mutex::scoped_timed_lock l(m_prot, xt);
   assert(valid_lock(m_state));

   if (!l.locked()) return false;

   bool fail = false;

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: wait while write-locked
      while (m_state == -1){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_readers;
         if (!m_waiting_readers.timed_wait(l, xt)){
               --m_num_waiting_readers;
               fail = true; 
               break;
         }
         --m_num_waiting_readers;
      }
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Writer priority: wait while write-locked or while writers are waiting

      while (m_state == -1 || m_num_waiting_writers > 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_readers;
         if (!m_waiting_readers.timed_wait(l, xt)){
               --m_num_waiting_readers;
               fail = true; 
               break;
         }
         --m_num_waiting_readers;
      }
   }
      
   else if 
      ((assert(m_sp == read_write_scheduling_policy::alternating_single_read 
               || m_sp == read_write_scheduling_policy::alternating_many_reads)), 
               false){
   } 
   else{
      //Alternating priority: wait while write-locked or while not readers' turn
      while (m_state == -1 || m_num_readers_to_wake == 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_readers;
         if (!m_waiting_readers.timed_wait(l, xt)){
            --m_num_waiting_readers;
            fail = true; 
            break;
         }
         --m_num_waiting_readers;
      }

      if (!fail){
         assert(m_num_readers_to_wake > 0);
         --m_num_readers_to_wake;
      }
   }

   if (!fail){
      //Obtain a read lock
      assert(valid_read_lockable(m_state));
      ++m_state;

      //See note in read_write_mutex_impl<>::do_read_lock() as to why 
      //m_readers_next should be set here
      m_readers_next = false;
      assert(valid_read_lock(m_state));
      //Should be read-locked
   }
   else{
      if (m_num_readers_to_wake > 0){
         //If there were readers scheduled to wake, 
         //decrement the number in case we were that reader.
         //If only one was scheduled to wake, the scheduling
         //algorithm will schedule another if one is available;
         //if more than one, one fewer reader will run before
         //the scheduling algorithm is called again. This last
         //case is not ideal, especially if a lot of waiting
         //readers timeout, but without knowing whether
         //we were actually one of the readers that was
         //scheduled to wake it's difficult to come up
         //with a better plan.
         --m_num_readers_to_wake;
      }

      if (m_state == 0){
         //If there is no thread with a lock that will 
         //call do_scheduling_impl() when it unlocks, call it ourselves
         do_timeout_scheduling_impl();
      }
   }

   return !fail;
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::
   do_timed_write_lock(const boost::shmem::xtime &xt)
{
   typename Mutex::scoped_timed_lock l(m_prot, xt);
   assert(valid_lock(m_state));

   if (!l.locked())
      return false;

   bool fail = false;

   if (m_sp == read_write_scheduling_policy::reader_priority){
      //Reader priority: wait while locked or while readers are waiting

      while (m_state != 0 || m_num_waiting_readers > 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         if (!m_waiting_writers.timed_wait(l, xt)){
               --m_num_waiting_writers;
               fail = true;
               break;
         }
         --m_num_waiting_writers;
      }
   }
   else if (m_sp == read_write_scheduling_policy::writer_priority){
      //Shut down extra readers that were scheduled only because of no waiting writers
      m_num_readers_to_wake = 0;

      //Writer priority: wait while locked
      while (m_state != 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         if (!m_waiting_writers.timed_wait(l, xt)){
            --m_num_waiting_writers;
            fail = true;
            break;
         }
         --m_num_waiting_writers;
      }
   }
   else  if 
   ((assert(m_sp == read_write_scheduling_policy::alternating_single_read || 
            m_sp == read_write_scheduling_policy::alternating_many_reads)), 
            false){
   }
   else{
      //Shut down extra readers that were scheduled only because of no waiting writers
      if (m_sp == read_write_scheduling_policy::alternating_single_read 
         && m_num_waiting_writers == 0)
         m_num_readers_to_wake = (m_readers_next && m_num_readers_to_wake > 0) ? 1 : 0;

      //Alternating priority: wait while locked or while not writers' turn

      while (m_state != 0 || m_num_readers_to_wake > 0){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         ++m_num_waiting_writers;
         if (!m_waiting_writers.timed_wait(l, xt)){
               --m_num_waiting_writers;
               fail = true;
               break;
         }
         --m_num_waiting_writers;
      }
   }

   if (!fail){
      //Obtain a write lock
      assert(valid_write_lockable(m_state));
      m_state = -1;

      //See note in read_write_mutex_impl<>::do_read_lock() as to why 
      //m_readers_next should be set here
      m_readers_next = true;
      assert(valid_write_lock(m_state));
      //Should be write-locked
   }
   else{
      if (m_state == 0){
         //If there is no thread with a lock that will 
         //call do_scheduling_impl() when it unlocks, call it ourselves
         do_timeout_scheduling_impl();
      }
   }

   return !fail;
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_read_unlock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_read_lock(m_state));

   if (m_state > 0)
      --m_state;
   else //not read-locked
      throw lock_error();

   if (m_state == 0)
      do_unlock_scheduling_impl();

   assert(valid_lock(m_state));
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_write_unlock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_write_lock(m_state));

   if (m_state == -1)
      m_state = 0;
   else
      // Trying to release a reader-locked or unlocked mutex???
      throw lock_error();

   do_unlock_scheduling_impl();

   assert(valid_lock(m_state));
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_demote_to_read_lock_impl()
{
   assert(valid_write_lock(m_state));

   if (m_state == -1) {
      //Convert from write lock to read lock
      m_state = 1;

      //If the conditions are right, release other readers
      do_demote_scheduling_impl();

      //Lock demoted
      assert(valid_read_lock(m_state));
      return true;
   }
   else{
      //Lock is read-locked or unlocked can't be demoted
      throw lock_error();
      return false;
   }
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_demote_to_read_lock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_write_lock(m_state));

   do_demote_to_read_lock_impl();
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_try_demote_to_read_lock()
{
   typename Mutex::scoped_try_lock l(m_prot);
   assert(valid_write_lock(m_state));

   if (!l.locked())
      return false;
   else //(l.locked())
      return do_demote_to_read_lock_impl();
}


template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_timed_demote_to_read_lock
   (const boost::shmem::xtime &xt)
{
    typename Mutex::scoped_timed_lock l(m_prot, xt);
    assert(valid_write_lock(m_state));

    if (!l.locked())
        return false;
    else //(l.locked())
        return do_demote_to_read_lock_impl();
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_promote_to_write_lock()
{
   typename Mutex::scoped_lock l(m_prot);
   assert(valid_read_lock(m_state));

   if (m_state == 1){
      //Convert from read lock to write lock
      m_state = -1;

      //Lock promoted
      assert(valid_write_lock(m_state));
   }
   else if (m_state <= 0){
      //Lock is write-locked or unlocked can't be promoted
      throw lock_error();
   }
   else if (m_state_waiting_promotion){
      //Someone else is already trying to promote. Avoid deadlock by throwing exception.
      throw lock_error();
   }
   else{
      ++m_num_waiting_writers;
      m_state_waiting_promotion = true;

      while (m_state > 1){
         //Check for invalid loop conditions (but will also detect spurious wakeups)
         m_waiting_promotion.wait(l);
      }

      m_state_waiting_promotion = false;
      --m_num_waiting_writers;
      
      assert(m_num_waiting_writers >= 0);
      assert(m_state == 1);

      //Convert from read lock to write lock
      m_state = -1;
      
      //Lock promoted
      assert(valid_write_lock(m_state));
   }
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::do_try_promote_to_write_lock()
{
   typename Mutex::scoped_try_lock l(m_prot);
   assert(valid_read_lock(m_state));

   if (!l.locked())
      return false;
   else{
      if (m_state == 1){
         //Convert from read lock to write lock
         m_state = -1;

         //Lock promoted
         assert(valid_write_lock(m_state));
         return true;
      }
      else if (m_state <= 0){
         //Lock is write-locked or unlocked can't be promoted
         throw lock_error();
      }
      else if (m_state_waiting_promotion){
         //Someone else is already trying to promote. Avoid deadlock by returning false.
         return false;
      }
      else{
         //There are other readers, so we can't promote
         return false;
      }
   }
}

template<typename Mutex>
bool shared_read_write_mutex_impl<Mutex>::
   do_timed_promote_to_write_lock(const boost::shmem::xtime &xt)
{
   typename Mutex::scoped_timed_lock l(m_prot, xt);
   assert(valid_read_lock(m_state));

   if (!l.locked()) return false;
   else{
      if (m_state == 1){
         //Convert from read lock to write lock
         m_state = -1;
         
         //Lock promoted
         assert(valid_write_lock(m_state));
         return true;
      }
      else if (m_state <= 0){
         //Lock is not read-locked and can't be promoted
         throw lock_error();
      }
      else if (m_state_waiting_promotion){
         //Someone else is already trying to promote. Avoid deadlock by returning false.
         return false;
      }
      else if((assert(m_state > 1 && !m_state_waiting_promotion)), false){
      }
      else{   
         ++m_num_waiting_writers;
         m_state_waiting_promotion = true;

         while (m_state > 1){
            if (!m_waiting_promotion.timed_wait(l, xt)){
               m_state_waiting_promotion = false;
               --m_num_waiting_writers;
               return false;
            }
         }

         m_state_waiting_promotion = false;
         --m_num_waiting_writers;
         
         assert(m_num_waiting_writers >= 0);
         assert(m_state == 1);

         //Convert from read lock to write lock
         m_state = -1;
         
         //Lock promoted
         assert(valid_write_lock(m_state));
         return true;
      }
   }
}


template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_unlock_scheduling_impl()
{
   assert(m_state == 0);
   do_scheduling_impl();
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_timeout_scheduling_impl()
{
   assert(m_state == 0);
   do_scheduling_impl();
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_demote_scheduling_impl()
{
   assert(m_state == 1);
   do_scheduling_impl();
}

template<typename Mutex>
void shared_read_write_mutex_impl<Mutex>::do_scheduling_impl()
{
   bool demotion = m_state > 0; //Releasing readers after lock demotion?
   assert(valid_read_lockable(m_state));

   if (m_num_waiting_writers > 0 && m_num_waiting_readers > 0){
      //Both readers and writers waiting: use scheduling policy
      if (m_sp == read_write_scheduling_policy::reader_priority){
         m_num_readers_to_wake = m_num_waiting_readers;
         m_waiting_readers.notify_all();
      }
      else if (m_sp == read_write_scheduling_policy::writer_priority){
         if (!demotion){
            if (m_state_waiting_promotion)
               m_waiting_promotion.notify_one();
            else
               m_waiting_writers.notify_one();
         }
      }
      else if (m_sp == read_write_scheduling_policy::alternating_single_read){
         if (m_num_readers_to_wake > 0){
            //Let the already woken threads work
         }
         else  if (m_readers_next){
            m_num_readers_to_wake = 1;
            m_waiting_readers.notify_one();
         }
         else{
            if (!demotion){
               if (m_state_waiting_promotion)
                  m_waiting_promotion.notify_one();
               else
                  m_waiting_writers.notify_one();
            }
         }
      }
      else{
         if (m_num_readers_to_wake > 0){
            //Let the already woken threads work
         }
         else if (m_readers_next){
            m_num_readers_to_wake = m_num_waiting_readers;
            m_waiting_readers.notify_all();
         }
         else{
            if (!demotion){
               if (m_state_waiting_promotion)
                  m_waiting_promotion.notify_one();
               else
                  m_waiting_writers.notify_one();
            }
         }
      }
   }
   else if (m_num_waiting_writers > 0){
      if (!demotion){
         //Only writers waiting--scheduling policy doesn't matter
         if (m_state_waiting_promotion)
               m_waiting_promotion.notify_one();
         else
               m_waiting_writers.notify_one();
      }
   }
   else if (m_num_waiting_readers > 0){
      //Only readers waiting--scheduling policy doesn't matter
      m_num_readers_to_wake = m_num_waiting_readers;
      m_waiting_readers.notify_all();
   }
}

}  // namespace detail {

}  // namespace shmem

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif
