// Copyright Howard Hinnant 2007. Distributed under the Boost
// Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)

// mutex.cpp

#include "mutex"
#include "system_error"
#undef NDEBUG
#include <cassert>

namespace std
{

using namespace roundhouse;

///////////
// mutex //
///////////

mutex::mutex()
{
    error_code::value_type ec = pthread_mutex_init(&m_, 0);
    if (ec)
        throw system_error(ec, native_category, "mutex constructor failed");
}

mutex::~mutex()
{
    int e = pthread_mutex_destroy(&m_);
    assert(e == 0);
}

void
mutex::lock()
{
    error_code::value_type ec = pthread_mutex_lock(&m_);
    if (ec)
        throw system_error(ec, native_category, "mutex lock failed");
}

void
mutex::unlock()
{
    int e = pthread_mutex_unlock(&m_);
    assert(e == 0);
}

bool
mutex::try_lock()
{
    return pthread_mutex_trylock(&m_) == 0;
}

/////////////////////
// recursive_mutex //
/////////////////////

recursive_mutex::recursive_mutex()
{
    pthread_mutexattr_t attr;
    error_code::value_type ec = pthread_mutexattr_init(&attr);
    if (ec)
        goto fail;
    ec = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    if (ec)
    {
        pthread_mutexattr_destroy(&attr);
        goto fail;
    }
    ec = pthread_mutex_init(&m_, &attr);
    if (ec)
    {
        pthread_mutexattr_destroy(&attr);
        goto fail;
    }
    ec = pthread_mutexattr_destroy(&attr);
    if (ec)
    {
        pthread_mutex_destroy(&m_);
        goto fail;
    }
    return;
fail:
    throw system_error(ec, native_category, "recursive_mutex constructor failed");
}

recursive_mutex::~recursive_mutex()
{
    int e = pthread_mutex_destroy(&m_);
    assert(e == 0);
}

void
recursive_mutex::lock()
{
    error_code::value_type ec = pthread_mutex_lock(&m_);
    if (ec)
        throw system_error(ec, native_category, "recursive_mutex lock failed");
}

void
recursive_mutex::unlock()
{
    int e = pthread_mutex_unlock(&m_);
    assert(e == 0);
}

bool
recursive_mutex::try_lock()
{
    return pthread_mutex_trylock(&m_) == 0;
}

/////////////////
// timed_mutex //
/////////////////

timed_mutex::timed_mutex()
    : locked_(false)
{
}

void
timed_mutex::lock()
{
    unique_lock<mutex> lk(mut_);
    while (locked_)
        gate1_.wait(lk);
    locked_ = true;
}

bool
timed_mutex::try_lock()
{
    unique_lock<mutex> lk(mut_, try_to_lock);
    if (lk.owns() && !locked_)
    {
        locked_ = true;
        return true;
    }
    return false;
}

bool
timed_mutex::timed_lock(const system_time& abs_time)
{
    unique_lock<mutex> lk(mut_);
    bool timed_out = get_system_time() > abs_time;
    while (!timed_out && locked_)
        timed_out = gate1_.timed_wait(lk, abs_time);
    if (!locked_)
    {
        locked_ = true;
        return true;
    }
    return false;
}

void
timed_mutex::unlock()
{
    {
    lock_guard<mutex> _(mut_);
    locked_ = false;
    }
    gate1_.notify_one();
}

///////////////////////////
// recursive_timed_mutex //
///////////////////////////

recursive_timed_mutex::recursive_timed_mutex()
    : state_(0),
      id_(0)
{
}

void
recursive_timed_mutex::lock()
{
    pthread_t id = pthread_self();
    unique_lock<mutex> lk(mut_);
    if (pthread_equal(id, id_))
    {
        if (state_ == numeric_limits<unsigned>::max())
            throw system_error(EAGAIN, native_category, "recursive_timed_mutex lock failed");
        ++state_;
        return;
    }
    while (state_ != 0)
        gate1_.wait(lk);
    state_ = 1;
    id_ = id;
}

bool
recursive_timed_mutex::try_lock()
{
    pthread_t id = pthread_self();
    unique_lock<mutex> lk(mut_, try_to_lock);
    if (lk.owns() && (state_ == 0 || pthread_equal(id, id_)))
    {
        if (state_ == numeric_limits<unsigned>::max())
            return false;
        ++state_;
        id_ = id;
        return true;
    }
    return false;
}

bool
recursive_timed_mutex::timed_lock(const system_time& abs_time)
{
    pthread_t id = pthread_self();
    unique_lock<mutex> lk(mut_);
    if (pthread_equal(id, id_))
    {
        if (state_ == numeric_limits<unsigned>::max())
            return false;
        ++state_;
        return true;
    }
    bool timed_out = get_system_time() > abs_time;
    while (!timed_out && state_ != 0)
        timed_out = gate1_.timed_wait(lk, abs_time);
    if (state_ == 0)
    {
        state_ = 1;
        id_ = id;
        return true;
    }
    return false;
}

void
recursive_timed_mutex::unlock()
{
    unique_lock<mutex> lk(mut_);
    if (--state_ == 0)
    {
        id_ = 0;
        lk.unlock();
        gate1_.notify_one();
    }
}

///////////////////
// lock commands //
///////////////////

const defer_lock_t  defer_lock = defer_lock_t();
const try_to_lock_t try_to_lock = try_to_lock_t();
const adopt_lock_t  adopt_lock = adopt_lock_t();

}  // std
