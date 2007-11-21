// Copyright Howard Hinnant 2007. Distributed under the Boost
// Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)

// mutex.cpp

#define __STDC_LIMIT_MACROS

#include "mutex"
#include "system_error"
#undef NDEBUG
#include <cassert>

#include <stdint.h>

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

///////////////
// call_once //
///////////////

// This is a variation of the Mike Burrows fast_pthread_once algorithm

// Copyright (c) 2007, Google Inc. 
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
// 
// 	*	Redistributions of source code must retain the above copyright
// 	notice, this list of conditions and the following disclaimer.
// 	*	Redistributions in binary form must reproduce the above
// 	copyright notice, this list of conditions and the following
// 	disclaimer in the documentation and/or other materials provided with
// 	the distribution.
// 	*	Neither the name of Google Inc. nor the names
// 	of its contributors may be used to endorse or promote products
// 	derived from this software without specific prior written
// 	permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
// IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
// PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
// OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Using pthread_getspecific as a poor man's thread_local
// Making non-portable assumption that a sig_atomic_t can be stored in a void*

pthread_mutex_t __call_once_mut = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  __call_once_cv  = PTHREAD_COND_INITIALIZER;
static once_flag global_epoch(SIG_ATOMIC_MAX);

once_flag& __get_call_once_global_epoch()
{
    return global_epoch;
}

static pthread_key_t key;
static pthread_once_t call_once_key_init = PTHREAD_ONCE_INIT;

extern "C"
{
void call_once_init_key()
{
    pthread_key_create(&key, 0);
}
} // extern "C"


static
pthread_key_t
get_once_key()
{
    pthread_once(&call_once_key_init, call_once_init_key);
    return key;
}

void
__set_once_per_thread_epoch(once_flag flag)
{
    pthread_setspecific(get_once_key(), (void*)flag);
}

once_flag
__get_once_per_thread_epoch()
{
    once_flag f = (once_flag)pthread_getspecific(get_once_key());
    if (f == once_flag(0))
        f = once_flag(SIG_ATOMIC_MAX);
    return f;
}

}  // std
