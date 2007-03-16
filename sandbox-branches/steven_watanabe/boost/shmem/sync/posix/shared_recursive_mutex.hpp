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
{
   detail::mutexattr_wrapper mut_attr(true);
   detail::mutex_initializer mut(m_mut, mut_attr);
   mut.release();
}

inline shared_recursive_mutex::~shared_recursive_mutex()
{
   int res = pthread_mutex_destroy(&m_mut);
   assert(res == 0);(void)res;
}

inline void shared_recursive_mutex::do_lock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    if (res == EDEADLK) throw lock_exception();
    assert(res == 0);
}

inline void shared_recursive_mutex::do_unlock()
{
    int res = 0;
    res = pthread_mutex_unlock(&m_mut);
    if (res == EPERM) throw lock_exception();
    assert(res == 0);
}

inline shared_recursive_try_mutex::shared_recursive_try_mutex()
{
   detail::mutexattr_wrapper mut_attr(true);
   detail::mutex_initializer mut(m_mut, mut_attr);
   mut.release();
}

inline shared_recursive_try_mutex::~shared_recursive_try_mutex()
{
   int res = pthread_mutex_destroy(&m_mut);
   assert(res == 0);(void)res;
}

inline void shared_recursive_try_mutex::do_lock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    if (res == EDEADLK) throw lock_exception();
    assert(res == 0);
}

inline bool shared_recursive_try_mutex::do_trylock()
{
    int res = 0;
    res = pthread_mutex_trylock(&m_mut);
    if (res == EDEADLK) throw lock_exception();
    assert(res == 0 || res == EBUSY);
    return res == 0;
}

inline void shared_recursive_try_mutex::do_unlock()
{
    int res = 0;
    res = pthread_mutex_unlock(&m_mut);
    if (res == EPERM) throw lock_exception();
    assert(res == 0);
}

inline shared_recursive_timed_mutex::shared_recursive_timed_mutex()
    : m_valid_id(false), m_count(0)
{
   //Mutex init
   detail::mutexattr_wrapper mut_attr;
   detail::mutex_initializer mut(m_mut, mut_attr);

   //Condition init
   detail::condattr_wrapper cond_attr;
   detail::condition_initializer cond(m_unlocked, cond_attr);

   mut.release(); 
   cond.release(); 
}

inline shared_recursive_timed_mutex::~shared_recursive_timed_mutex()
{
    int res = 0;
    res = pthread_mutex_destroy(&m_mut);
    assert(res == 0);

    res = pthread_cond_destroy(&m_unlocked);
    assert(res == 0);
}

inline void shared_recursive_timed_mutex::do_lock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    pthread_t tid = pthread_self();
    if (m_valid_id && pthread_equal(m_thread_id, tid))
        ++m_count;
    else
    {
        while (m_valid_id)
        {
            res = pthread_cond_wait(&m_unlocked, &m_mut);
            assert(res == 0);
        }

        m_thread_id = tid;
        m_valid_id = true;
        m_count = 1;
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
}

inline bool shared_recursive_timed_mutex::do_trylock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    bool ret = false;
    pthread_t tid = pthread_self();
    if (m_valid_id && pthread_equal(m_thread_id, tid))
    {
        ++m_count;
        ret = true;
    }
    else if (!m_valid_id)
    {
        m_thread_id = tid;
        m_valid_id = true;
        m_count = 1;
        ret = true;
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
    return ret;
}

inline bool shared_recursive_timed_mutex::do_timedlock(const xtime& xt)
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    bool ret = false;
    pthread_t tid = pthread_self();
    if (m_valid_id && pthread_equal(m_thread_id, tid))
    {
        ++m_count;
        ret = true;
    }
    else
    {
        timespec ts;
        xtime_to_timespec(xt, ts);

        while (m_valid_id)
        {
            res = pthread_cond_timedwait(&m_unlocked, &m_mut, &ts);
            if (res == ETIMEDOUT)
                break;
            assert(res == 0);
        }

        if (!m_valid_id)
        {
            m_thread_id = tid;
            m_valid_id = true;
            m_count = 1;
            ret = true;
        }
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
    return ret;
}

inline void shared_recursive_timed_mutex::do_unlock()
{
    int res = 0;
    res = pthread_mutex_lock(&m_mut);
    assert(res == 0);

    pthread_t tid = pthread_self();
    if (m_valid_id && !pthread_equal(m_thread_id, tid))
    {
        res = pthread_mutex_unlock(&m_mut);
        assert(res == 0);
        throw lock_exception();
    }

    if (--m_count == 0)
    {
        assert(m_valid_id);
        m_valid_id = false;

        res = pthread_cond_signal(&m_unlocked);
        assert(res == 0);
    }

    res = pthread_mutex_unlock(&m_mut);
    assert(res == 0);
}

}  //namespace shmem {

}  //namespace boost {

