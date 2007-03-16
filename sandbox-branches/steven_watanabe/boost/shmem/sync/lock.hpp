//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_LOCK_HPP
#define BOOST_SHMEM_LOCK_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/exceptions.hpp>
#include <boost/utility.hpp>

/*!\file
   Describes the scoped_lock and scoped_try_lock classes needed to use shared 
   mutex familiy synchronization objects
*/

namespace boost {

namespace shmem
{
   class shared_condition;
   struct xtime;
}

namespace shmem {

template <typename Mutex>
class lock_ops : private noncopyable
{
private:
    lock_ops() { }

public:
    typedef typename Mutex::cv_state lock_state;
    friend class boost::shmem::shared_condition;

    static void lock(Mutex& m)
    {
        m.do_lock();
    }
    static bool trylock(Mutex& m)
    {
        return m.do_trylock();
    }
    static bool timedlock(Mutex& m, const xtime& xt)
    {
        return m.do_timedlock(xt);
    }
    static void unlock(Mutex& m)
    {
        m.do_unlock();
    }
    static void lock(Mutex& m, lock_state& state)
    {
        m.do_lock(state);
    }
    static void unlock(Mutex& m, lock_state& state)
    {
        m.do_unlock(state);
    }
};

template <typename Mutex>
class scoped_lock : private noncopyable
{
public:
   typedef Mutex mutex_type;
   friend class boost::shmem::shared_condition;

   explicit scoped_lock(Mutex& mx, bool initially_locked=true)
      : m_mutex(mx), m_locked(false)
      {  if (initially_locked) lock(); }
   ~scoped_lock()
      {  if (m_locked) unlock(); }

   void lock()
   {
      if (m_locked) throw boost::shmem::lock_exception();
      lock_ops<Mutex>::lock(m_mutex);
      m_locked = true;
   }

   void unlock()
   {
      if (!m_locked) throw boost::shmem::lock_exception();
      lock_ops<Mutex>::unlock(m_mutex);
      m_locked = false;
   }

   bool locked() const { return m_locked; }

   operator const void*() const { return m_locked ? this : 0; }

   void release()
      {  m_locked = false; }

   private:
   Mutex& m_mutex;
   bool m_locked;
};

template <typename TryMutex>
class scoped_try_lock : private noncopyable
{
public:
   typedef TryMutex mutex_type;
   friend class boost::shmem::shared_condition;

   explicit scoped_try_lock(TryMutex& mx)
      : m_mutex(mx), m_locked(false)
      {  try_lock(); }

   scoped_try_lock(TryMutex& mx, bool initially_locked)
      : m_mutex(mx), m_locked(false)
      {  if (initially_locked) lock(); }

   ~scoped_try_lock()
      {  if (m_locked) unlock(); }

   void lock()
   {
      if (m_locked) throw boost::shmem::lock_exception();
      lock_ops<TryMutex>::lock(m_mutex);
      m_locked = true;
   }

   bool try_lock()
   {
      if (m_locked) throw boost::shmem::lock_exception();
      return (m_locked = lock_ops<TryMutex>::trylock(m_mutex));
   }

   void unlock()
   {
      if (!m_locked) throw boost::shmem::lock_exception();
      lock_ops<TryMutex>::unlock(m_mutex);
      m_locked = false;
   }

   bool locked() const { return m_locked; }

   operator const void*() const { return m_locked ? this : 0; }

   void release()
      {  m_locked = false; }

   private:

   TryMutex& m_mutex;
   bool m_locked;
};

template <typename TimedMutex>
class scoped_timed_lock : private noncopyable
{
public:
   typedef TimedMutex mutex_type;

   scoped_timed_lock(TimedMutex& mx, const xtime& xt)
      : m_mutex(mx), m_locked(false)
   {
      timed_lock(xt);
   }
   scoped_timed_lock(TimedMutex& mx, bool initially_locked)
      : m_mutex(mx), m_locked(false)
   {
      if (initially_locked) lock();
   }
   ~scoped_timed_lock()
      {  if (m_locked) unlock(); }

   void lock()
   {
      if (m_locked) throw lock_exception();
      lock_ops<TimedMutex>::lock(m_mutex);
      m_locked = true;
   }

   bool try_lock()
   {
      if (m_locked) throw lock_exception();
      return (m_locked = lock_ops<TimedMutex>::trylock(m_mutex));
   }

   bool timed_lock(const xtime& xt)
   {
      if (m_locked) throw lock_exception();
      return (m_locked = lock_ops<TimedMutex>::timedlock(m_mutex, xt));
   }

   void unlock()
   {
      if (!m_locked) throw lock_exception();
      lock_ops<TimedMutex>::unlock(m_mutex);
      m_locked = false;
   }

   bool locked() const { return m_locked; }

   operator const void*() const { return m_locked ? this : 0; }

   void release()
      {  m_locked = false; }

   private:
   friend class boost::shmem::shared_condition;

   TimedMutex& m_mutex;
   bool m_locked;
};

} // namespace shmem
} // namespace boost

#include <boost/shmem/detail/config_end.hpp>

#endif // BOOST_SHMEM_LOCK_HPP
