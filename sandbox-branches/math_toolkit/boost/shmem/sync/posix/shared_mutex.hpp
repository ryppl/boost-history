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

inline shared_mutex::shared_mutex()
{
   detail::mutexattr_wrapper mut_attr;
   detail::mutex_initializer mut(m_mut, mut_attr);
   mut.release();
}

inline shared_mutex::~shared_mutex() 
{
   int res = pthread_mutex_destroy(&m_mut);
   assert(res  == 0);(void)res;
}    

inline void shared_mutex::do_lock()
{
   int res = pthread_mutex_lock(&m_mut);
   if ( res == EDEADLK) 
      throw lock_exception();
   assert(res == 0);
}

inline void shared_mutex::do_unlock()
{
   int res = pthread_mutex_unlock(&m_mut);
   if (res == EPERM) 
      throw lock_exception();
   assert(res == 0);
}

inline shared_try_mutex::shared_try_mutex()
{
   detail::mutexattr_wrapper mut_attr;
   detail::mutex_initializer mut(m_mut, mut_attr);
   mut.release();
}

inline shared_try_mutex::~shared_try_mutex() 
{
   int res = pthread_mutex_destroy(&m_mut);
   assert(res  == 0);(void)res;
}

inline void shared_try_mutex::do_lock()
{
   if (pthread_mutex_lock(&m_mut) == EDEADLK) 
      throw lock_exception();
}

inline bool shared_try_mutex::do_trylock()
{
    int res = 0;
    res = pthread_mutex_trylock(&m_mut);
    if (res == EDEADLK) throw lock_exception();
    assert(res == 0 || res == EBUSY);
    return res == 0;
}

inline void shared_try_mutex::do_unlock()
{
    int res = 0;
    res = pthread_mutex_unlock(&m_mut);
    if (res == EPERM) throw lock_exception();
    assert(res == 0);
}

//---

inline shared_timed_mutex::shared_timed_mutex()
    : m_locked(false)
{
   //Mutex init
   detail::mutexattr_wrapper mut_attr;
   detail::mutex_initializer mut(m_mut, mut_attr);

   //Condition init
   detail::condattr_wrapper  cond_attr;
   detail::condition_initializer cond(m_cond, cond_attr);

   mut.release();
   cond.release();
}

inline shared_timed_mutex::~shared_timed_mutex()
{
    assert(!m_locked);
    int res = 0;
    res = pthread_mutex_destroy(&m_mut);
    assert(res == 0);

    res = pthread_cond_destroy(&m_cond);
    assert(res == 0);
}

inline void shared_timed_mutex::do_lock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    while (m_locked){
        res = pthread_cond_wait(&m_cond, &m_mut);
        assert(res == 0);
    }

    assert(!m_locked);
    m_locked = true;

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
}

inline bool shared_timed_mutex::do_trylock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    bool ret = false;
    if (!m_locked)
    {
        m_locked = true;
        ret = true;
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
    return ret;
}

inline bool shared_timed_mutex::do_timedlock(const xtime& xt)
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    timespec ts;
    xtime_to_timespec(xt, ts);

    while (m_locked)
    {
        res = pthread_cond_timedwait(&m_cond, &m_mut, &ts);
        assert(res == 0 || res == ETIMEDOUT);

        if (res == ETIMEDOUT)
            break;
    }

    bool ret = false;
    if (!m_locked)
    {
        m_locked = true;
        ret = true;
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
    return ret;
}

inline void shared_timed_mutex::do_unlock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    assert(m_locked);
    m_locked = false;

    res = pthread_cond_signal(&m_cond);
    assert(res == 0);

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
}

}  //namespace shmem {

}  //namespace boost {

