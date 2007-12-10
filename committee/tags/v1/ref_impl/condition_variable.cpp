// Copyright Howard Hinnant 2007. Distributed under the Boost
// Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)

// condition_variable.cpp

#include "condition_variable"
#include "hdate_time"
#undef NDEBUG
#include <cassert>

namespace std
{

template <>
struct __make<timespec>
{
    static
    timespec
    from(const system_time& t)
    {
        timespec ts;
        ts.tv_sec = t.seconds_since_epoch();
        ts.tv_nsec = static_cast<long>(t.nanoseconds_since_epoch().count() % system_time::ticks_per_second);
        return ts;
    }

};

condition_variable::condition_variable()
{
    error_code::value_type ec = pthread_cond_init(&cv_, 0);
    if (ec)
        throw system_error(ec, native_category, "condition_variable constructor failed");
}

condition_variable::~condition_variable()
{
    int e = pthread_cond_destroy(&cv_);
    assert(e == 0);
}

void
condition_variable::notify_one()
{
    error_code::value_type ec = pthread_cond_signal(&cv_);
    if (ec)
        throw system_error(ec, native_category, "condition_variable notify_one failed");
}

void
condition_variable::notify_all()
{
    error_code::value_type ec = pthread_cond_broadcast(&cv_);
    if (ec)
        throw system_error(ec, native_category, "condition_variable notify_all failed");
}

void
condition_variable::__do_wait(pthread_mutex_t* mut)
{
    error_code::value_type ec = pthread_cond_wait(&cv_, mut);
    if (ec)
        throw system_error(ec, native_category, "condition_variable wait failed");
}

bool
condition_variable::__do_timed_wait(pthread_mutex_t* mut, const system_time& abs_time)
{
    timespec __tm = __make<timespec>::from(abs_time);
    error_code::value_type ec = pthread_cond_timedwait(&cv_, mut, &__tm);
    if (ec != 0 && ec != ETIMEDOUT)
        throw system_error(ec, native_category, "condition_variable timed_wait failed");
    return ec == 0;
}

}  // std
