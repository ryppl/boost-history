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
#include <boost/shmem/sync/read_write_lock.hpp>
#include <assert.h>

namespace boost {

namespace shmem {
/*
template <typename TimedReadWriteMutex>
class scoped_timed_read_write_lock : private noncopyable
{
public:

    typedef TimedReadWriteMutex mutex_type;

    scoped_timed_read_write_lock(
        TimedReadWriteMutex& mx,
        read_write_lock_state::read_write_lock_state_enum initial_state)
        : m_mutex(mx), m_state(read_write_lock_state::unlocked)
    {
        if (initial_state == read_write_lock_state::read_locked)
            read_lock();
        else if (initial_state == read_write_lock_state::write_locked)
            write_lock();
    }

    ~scoped_timed_read_write_lock()
    {
        if (m_state != read_write_lock_state::unlocked)
            unlock();
    }

    void read_lock()
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        read_write_lock_ops<TimedReadWriteMutex>::read_lock(m_mutex);
        m_state = read_write_lock_state::read_locked;
    }

    bool try_read_lock()
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        if(read_write_lock_ops<TimedReadWriteMutex>::try_read_lock(m_mutex))
        {
            m_state = read_write_lock_state::read_locked;
            return true;
        }
        return false;
    }

    bool timed_read_lock(const xtime &xt)
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        if(read_write_lock_ops<TimedReadWriteMutex>::timed_read_lock(m_mutex,xt))
        {
            m_state = read_write_lock_state::read_locked;
            return true;
        }
        return false;
    }

    void write_lock()
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        read_write_lock_ops<TimedReadWriteMutex>::write_lock(m_mutex);
        m_state = read_write_lock_state::write_locked;
    }

    bool try_write_lock()
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        if(read_write_lock_ops<TimedReadWriteMutex>::try_write_lock(m_mutex))
        {
            m_state = read_write_lock_state::write_locked;
            return true;
        }
        return false;
    }

    bool timed_write_lock(const xtime &xt)
    {
        if (m_state != read_write_lock_state::unlocked) 
            throw lock_exception();
        if(read_write_lock_ops<TimedReadWriteMutex>::timed_write_lock(m_mutex,xt))
        {
            m_state = read_write_lock_state::write_locked;
            return true;
        }
        return false;
    }

    void unlock()
    {
        if (m_state == read_write_lock_state::unlocked) 
            throw lock_exception();
        if (m_state == read_write_lock_state::read_locked)
            read_write_lock_ops<TimedReadWriteMutex>::read_unlock(m_mutex);
        else //(m_state == read_write_lock_state::write_locked)
            read_write_lock_ops<TimedReadWriteMutex>::write_unlock(m_mutex);

        m_state = read_write_lock_state::unlocked;
    }

    void demote(void)
    {
        if (m_state != read_write_lock_state::write_locked) 
            throw lock_exception();
        read_write_lock_ops<TimedReadWriteMutex>::demote(m_mutex);
        m_state = read_write_lock_state::read_locked;
    }

    bool try_demote(void)
    {
        if (m_state != read_write_lock_state::write_locked) 
            throw lock_exception();
        return read_write_lock_ops<TimedReadWriteMutex>::try_demote(m_mutex) ? (m_state = read_write_lock_state::read_locked, true) : false;
    }

    bool timed_demote(const xtime &xt)
    {
        if (m_state != read_write_lock_state::write_locked) 
            throw lock_exception();
        return read_write_lock_ops<TimedReadWriteMutex>::timed_demote(m_mutex, xt) ? (m_state = read_write_lock_state::read_locked, true) : false;
    }

    void promote(void)
    {
        if (m_state != read_write_lock_state::read_locked) 
            throw lock_exception();
        read_write_lock_ops<TimedReadWriteMutex>::promote(m_mutex);
        m_state = read_write_lock_state::write_locked;
    }

    bool try_promote(void)
    {
        if (m_state != read_write_lock_state::read_locked) 
            throw lock_exception();
        return read_write_lock_ops<TimedReadWriteMutex>::try_promote(m_mutex) ? (m_state = read_write_lock_state::write_locked, true) : false;
    }

    bool timed_promote(const xtime &xt)
    {
        if (m_state != read_write_lock_state::read_locked) 
            throw lock_exception();
        return read_write_lock_ops<TimedReadWriteMutex>::timed_promote(m_mutex, xt) ? (m_state = read_write_lock_state::write_locked, true) : false;
    }

    //If allow_unlock = true, set_lock always succeedes and
    //the function result indicates whether an unlock was required.
    //If allow_unlock = false, set_lock may fail;
    //the function result indicates whether it succeeded.
    bool set_lock(read_write_lock_state::read_write_lock_state_enum ls, bool allow_unlock = true)
    {
        bool result = !allow_unlock;

        if (m_state != ls)
        {
            if (m_state == read_write_lock_state::unlocked)
            {
                if (ls == read_write_lock_state::read_locked)
                    read_lock();
                else //(ls == read_write_lock_state::write_locked)
                    write_lock();
            }
            else //(m_state == read_write_lock_state::read_locked || m_state == read_write_lock_state::write_locked)
            {
                if (ls == read_write_lock_state::read_locked)
                    demote();
                else if (ls == read_write_lock_state::write_locked)
                {
                    if (!try_promote())
                    {
                        if (allow_unlock)
                        {
                            result = true;
                            unlock();
                            write_lock();
                        }
                        else
                            result = false;
                    }
                }
                else //(ls == read_write_lock_state::unlocked)
                    unlock();
            }
        }

        return result;
    }

    bool try_set_lock(read_write_lock_state::read_write_lock_state_enum ls)
    {
        if (m_state != ls)
        {
            if (m_state == read_write_lock_state::unlocked)
            {
                if (ls == read_write_lock_state::read_locked)
                    return try_read_lock();
                else // (ls == read_write_lock_state::write_locked)
                    return try_write_lock();
            }
            else //(m_state == read_write_lock_state::read_locked || m_state == read_write_lock_state::write_locked)
            {
                if (ls == read_write_lock_state::read_locked)
                    return try_demote();
                else if (ls == read_write_lock_state::write_locked)
                    return try_promote();
                else //(ls == read_write_lock_state::unlocked)
                    return unlock(), true;
            }
        }
        else //(m_state == ls) 
            return true;
    }

    bool timed_set_lock(read_write_lock_state::read_write_lock_state_enum ls, const xtime &xt)
    {
        if (m_state != ls)
        {
            if (m_state == read_write_lock_state::unlocked)
            {
                if (ls == read_write_lock_state::read_locked)
                    return timed_read_lock(xt);
                else // (ls == read_write_lock_state::write_locked)
                    return timed_write_lock(xt);
            }
            else //(m_state == read_write_lock_state::read_locked || m_state == read_write_lock_state::write_locked)
            {
                if (ls == read_write_lock_state::read_locked)
                    return timed_demote(xt);
                else if (ls == read_write_lock_state::write_locked)
                    return timed_promote(xt);
                else //(ls == read_write_lock_state::unlocked)
                    return unlock(), true;
            }
        }
        else //(m_state == ls)
            return true;
    }
  
    bool locked() const
    {
        return m_state != read_write_lock_state::unlocked;
    }
  
    bool read_locked() const
    {
        return m_state == read_write_lock_state::read_locked;
    }
  
    bool write_locked() const
    {
        return m_state != read_write_lock_state::write_locked;
    }

    operator const void*() const
    {
        return (m_state != read_write_lock_state::unlocked) ? this : 0; 
    }

    read_write_lock_state::read_write_lock_state_enum state() const
    {
        return m_state;
    }

private:

    TimedReadWriteMutex& m_mutex;
    read_write_lock_state::read_write_lock_state_enum m_state;
};
*/
/*!An object of class shared_read_write_mutex is a read_write_mutex that
   can be placed in shared memory so that it can be used to synchronize
   threads of different processes.*/
class shared_read_write_lock
{
   enum shared_rw_lock_policy
   {
      reader_priority,               //Prefer readers; can starve writers
      writer_priority,               //Prefer writers; can starve readers
   //   rw_alternating_many_reads,        //Alternate readers and writers; before a writer, release all queued readers 
   //   rw_alternating_single_read        //Alternate readers and writers; before a writer, release only on queued reader
   };

   public:
   shared_read_write_lock(shared_rw_lock_policy policy = reader_priority)
   {
      m_policy  = policy;
      m_r_active  = 0;
      m_r_wait    = 0;
      m_w_active  = 0;
   }

   ~shared_read_write_lock()
      {}

   void readlock()
      {  do_readlock(false, 0); }

   bool readtrylock()
   {
      scoped_lock<shared_mutex>lock(m_mutex);
      if(m_w_active){
         return false;
      }
      else{
         m_r_active++;
      }
   }

   bool readtimedlock(const xtime &xt)
      {  return do_readlock(true, &xt); }

   bool do_readlock(bool timed, const xtime *pxt)
   {
      scoped_lock<shared_mutex>lock(m_mutex);

      //Check blocking policy 
      if((m_policy == reader_priority) ?
          m_w_active != 0 : m_w_active || m_w_wait > 0){
         //We should block
         m_r_wait++;
         while(m_w_active){
            if(timed){
               if(!m_reader_cond.timed_wait(lock, *pxt)){
                  m_r_wait--;
                  return false;
               }
            }
            else{
               m_reader_cond.wait(lock);
            }
         }
         m_r_wait--;
      }
      m_r_active++;
   }

   void readunlock()
   {
      scoped_lock<shared_mutex>lock(m_mutex);
      m_r_active--;
      if(m_r_active == 0 && m_w_wait > 0){
         m_writer_cond.notify_one();
      }
   }

   void writelock()
      {  do_writelock(false, 0);  }

   bool writetrylock()
   {
      scoped_lock<shared_mutex>lock(m_mutex);
      if(m_w_active || m_r_active > 0){
         return false;
      }
      else{
         m_w_active = 1;
      }
   }

   bool writetimedlock(const xtime &xt)
      {  return do_writelock(true, &xt);  }

   bool do_writelock(bool timed, const xtime *pxt)
   {
      scoped_lock<shared_mutex>lock(m_mutex);
      if(m_w_active || m_r_active > 0){
         m_w_wait++;
         //while((m_w_active || m_w_active > 0)){
         while(m_w_active || m_r_active > 0){
            if(timed){
               if(!m_writer_cond.timed_wait(lock, *pxt)){
                  m_w_wait--;
                  return false;
               }
            }
            else{
               m_writer_cond.wait(lock);
            }
         }
         m_w_wait--;
      }
      m_w_active = 1;
   }

   void writeunlock()
   {
      scoped_lock<shared_mutex>lock(m_mutex);
      m_w_active = 0;
      if(m_policy == reader_priority){
         if(m_r_wait > 0){
            m_reader_cond.notify_all();
         }
         else if (m_w_wait > 0){
            m_writer_cond.notify_one();
         }
      }
      else{
         if(m_w_wait > 0){
            m_writer_cond.notify_one();
         }
         else if (m_r_wait > 0){
            m_reader_cond.notify_all();
         }
      }
   }

   private:
   shared_mutex            m_mutex;
   shared_condition        m_reader_cond;
   shared_condition        m_writer_cond;
   shared_rw_lock_policy   m_policy;

   std::size_t             m_r_active;
   std::size_t             m_w_active;
   std::size_t             m_r_wait;
   std::size_t             m_w_wait;
};

}  // namespace shmem

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif

