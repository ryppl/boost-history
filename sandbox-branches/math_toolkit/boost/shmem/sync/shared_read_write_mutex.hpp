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
// shared_barrier is a modified version of Boost Threads read_write_mutex
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
// primitive which can allow multiple readers or a single
// writer to have access to a shared resource.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_READ_WRITE_MUTEX_HPP
#define BOOST_SHMEM_READ_WRITE_MUTEX_HPP

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
#include <boost/shmem/sync/shared_read_write_mutex_impl.hpp>
#include <assert.h>

namespace boost {

namespace shmem {

/*!An object of class shared_read_write_mutex is a read_write_mutex that
   can be placed in shared memory so that it can be used to synchronize
   threads of different processes.*/
class shared_read_write_mutex : private noncopyable
{
 public:
   /*!Effects: Constructs a read_write_mutex object with a scheduling policy. 
      Postconditions: *this is in an unlocked state.*/
   shared_read_write_mutex
      (read_write_scheduling_policy::read_write_scheduling_policy_enum sp) 
      : m_impl(sp) { }
   /*!Effects: Destroys a read_write_mutex object.
      Requires: *this is in an unlocked state.
      Notes: Danger: Destruction of a locked mutex is a serious programming 
      error resulting in undefined behavior such as a program crash.*/
  ~shared_read_write_mutex() { }
   /*!Returns the scheduling policy used by this shared_read_write_mutex.*/
   read_write_scheduling_policy::read_write_scheduling_policy_enum policy() const 
      { return m_impl.m_sp; }

   friend class boost::shmem::read_write_lock_ops<shared_read_write_mutex>;
   typedef boost::shmem::scoped_read_write_lock
       <shared_read_write_mutex> scoped_read_write_lock;
   typedef boost::shmem::scoped_read_lock
       <shared_read_write_mutex> scoped_read_lock;
   typedef boost::shmem::scoped_write_lock
       <shared_read_write_mutex> scoped_write_lock;

 private:
   // Operations that will eventually be done only via lock types
   void do_write_lock()             {  m_impl.do_write_lock();    }
   void do_read_lock()              {  m_impl.do_read_lock();     }
   void do_write_unlock()           {  m_impl.do_write_unlock();  }
   void do_read_unlock()            {  m_impl.do_read_unlock();   }
   void do_demote_to_read_lock()    {  m_impl.do_demote_to_read_lock();    } 
   void do_promote_to_write_lock()  {  m_impl.do_promote_to_write_lock();  } 
   bool locked()                    {  return m_impl.locked();  }
   boost::shmem::read_write_lock_state::read_write_lock_state_enum state()
      {   return m_impl.state();  }
   detail::shared_read_write_mutex_impl<shared_mutex> m_impl; 
};

/*!An object of class shared_read_write_mutex is a read_write_try_mutex that
   can be placed in shared memory so that it can be used to synchronize
   threads of different processes.*/
class shared_try_read_write_mutex : private noncopyable
{
 public:
   /*!Effects: Constructs a shared_try_read_write_mutex object with a 
      scheduling policy. 
      Postconditions: *this is in an unlocked state.*/
   shared_try_read_write_mutex(read_write_scheduling_policy::read_write_scheduling_policy_enum sp) 
      : m_impl(sp) { }
   /*!Effects: Destroys a shared_try_read_write_mutex object.
      Requires: *this is in an unlocked state.
      Notes: Danger: Destruction of a locked mutex is a serious programming 
      error resulting in undefined behavior such as a program crash.*/
  ~shared_try_read_write_mutex() { }
   /*!Returns the scheduling policy used by this shared_try_read_write_mutex.*/
   read_write_scheduling_policy::read_write_scheduling_policy_enum policy() const 
      { return m_impl.m_sp; }

   friend class boost::shmem::read_write_lock_ops<shared_try_read_write_mutex>;

   typedef boost::shmem::scoped_read_write_lock
       <shared_try_read_write_mutex> scoped_read_write_lock;
   typedef boost::shmem::scoped_try_read_write_lock
       <shared_try_read_write_mutex> scoped_try_read_write_lock;
   typedef boost::shmem::scoped_read_lock
       <shared_try_read_write_mutex> scoped_read_lock;
   typedef boost::shmem::scoped_try_read_lock
       <shared_try_read_write_mutex> scoped_try_read_lock;
   typedef boost::shmem::scoped_write_lock
       <shared_try_read_write_mutex> scoped_write_lock;
   typedef boost::shmem::scoped_try_write_lock
       <shared_try_read_write_mutex> scoped_try_write_lock;

 private:
   // Operations that will eventually be done only via lock types
   void do_write_lock()       {  m_impl.do_write_lock();             }
   void do_read_lock()        {  m_impl.do_read_lock();              }
   void do_write_unlock()     {  m_impl.do_write_unlock();           }
   void do_read_unlock()      {  m_impl.do_read_unlock();            }
   bool do_try_write_lock()   {  return m_impl.do_try_write_lock();  }
   bool do_try_read_lock()    {  return m_impl.do_try_read_lock();   }

   void do_demote_to_read_lock()       
      {  m_impl.do_demote_to_read_lock(); }
   bool do_try_demote_to_read_lock()   
      {  return m_impl.do_try_demote_to_read_lock();}
   void do_promote_to_write_lock()     
      {  m_impl.do_promote_to_write_lock();  }
   bool do_try_promote_to_write_lock() 
      {  return m_impl.do_try_promote_to_write_lock(); }
   bool locked()               {  return m_impl.locked();  }
   boost::shmem::read_write_lock_state::read_write_lock_state_enum state()
      {   return m_impl.state();  }

   detail::shared_read_write_mutex_impl<shared_try_mutex> m_impl; 
};

/*!An object of class shared_read_write_mutex is a read_write_timed_mutex that
   can be placed in shared memory so that it can be used to synchronize
   threads of different processes.*/
class shared_timed_read_write_mutex : private noncopyable
{
 public:
   /*!Effects: Constructs a shared_timed_read_write_mutex object with a 
      scheduling policy. 
      Postconditions: *this is in an unlocked state.*/
   shared_timed_read_write_mutex
      (read_write_scheduling_policy::read_write_scheduling_policy_enum sp) 
      : m_impl(sp) { }
   /*!Effects: Destroys a shared_timed_read_write_mutex object.
      Requires: *this is in an unlocked state.
      Notes: Danger: Destruction of a locked mutex is a serious programming 
      error resulting in undefined behavior such as a program crash.*/
  ~shared_timed_read_write_mutex() { }
   /*!Returns the scheduling policy used by this shared_timed_read_write_mutex.*/
   read_write_scheduling_policy::read_write_scheduling_policy_enum policy() const 
      { return m_impl.m_sp; }

   friend class boost::shmem::read_write_lock_ops
      <shared_timed_read_write_mutex>;

   typedef boost::shmem::scoped_read_write_lock
      <shared_timed_read_write_mutex> scoped_read_write_lock;
   typedef boost::shmem::scoped_try_read_write_lock
      <shared_timed_read_write_mutex> scoped_try_read_write_lock;
   typedef boost::shmem::scoped_timed_read_write_lock
      <shared_timed_read_write_mutex> scoped_timed_read_write_lock;
   typedef boost::shmem::scoped_read_lock
      <shared_timed_read_write_mutex> scoped_read_lock;
   typedef boost::shmem::scoped_try_read_lock
      <shared_timed_read_write_mutex> scoped_try_read_lock;
   typedef boost::shmem::scoped_timed_read_lock
      <shared_timed_read_write_mutex> scoped_timed_read_lock;
   typedef boost::shmem::scoped_write_lock
      <shared_timed_read_write_mutex> scoped_write_lock;
   typedef boost::shmem::scoped_try_write_lock
      <shared_timed_read_write_mutex> scoped_try_write_lock;
   typedef boost::shmem::scoped_timed_write_lock
      <shared_timed_read_write_mutex> scoped_timed_write_lock;

 private:
   // Operations that will eventually be done only via lock types
   void do_write_lock()       {  m_impl.do_write_lock();    }
   void do_read_lock()        {  m_impl.do_read_lock();     }
   void do_write_unlock()     {  m_impl.do_write_unlock();  }
   void do_read_unlock()      {  m_impl.do_read_unlock();   }
   bool do_try_write_lock()   {  return m_impl.do_try_write_lock(); }
   bool do_try_read_lock()    {  return m_impl.do_try_read_lock();}
   bool do_timed_write_lock(const xtime &xt)
      {  return m_impl.do_timed_write_lock(xt);  }
   bool do_timed_read_lock(const xtime &xt)
      {  return m_impl.do_timed_read_lock(xt);  }
   void do_demote_to_read_lock()       
      {  m_impl.do_demote_to_read_lock(); }
   bool do_try_demote_to_read_lock()   
      {  return m_impl.do_try_demote_to_read_lock();}
   bool do_timed_demote_to_read_lock(const xtime &xt)
      {  return m_impl.do_timed_demote_to_read_lock(xt);}
   void do_promote_to_write_lock()     
      {  m_impl.do_promote_to_write_lock();  }
   bool do_try_promote_to_write_lock() 
      {  return m_impl.do_try_promote_to_write_lock(); }
   bool do_timed_promote_to_write_lock(const xtime &xt)
      {  return m_impl.do_timed_promote_to_write_lock(xt); }
   bool locked()               {  return m_impl.locked();  }
   boost::shmem::read_write_lock_state::read_write_lock_state_enum state()
      {   return m_impl.state();  }

   detail::shared_read_write_mutex_impl<shared_timed_mutex> m_impl; 
};

}  // namespace shmem

}  // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif

