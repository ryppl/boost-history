// Copyright Howard Hinnant 2007. Distributed under the Boost
// Software License, Version 1.0. (see http://www.boost.org/LICENSE_1_0.txt)

// thread.cpp

#include "thread"
#include <sys/sysctl.h>

namespace std
{

void
thread::join()
{
    error_code::value_type ec = pthread_join(id_, 0);
    if (ec)
        throw system_error(ec, native_category, "thread::join failed");
    id_ = 0;
}

int
thread::__detach()
{
    int ec = EINVAL;
    if (id_ != 0)
    {
        ec = pthread_detach(id_);
        if (ec == 0)
            id_ = 0;
    }
    return ec;
}

void
thread::detach()
{
    error_code::value_type ec = __detach();
    if (ec)
        throw system_error(ec, native_category, "thread::detach failed");
}

unsigned
thread::hardware_concurrency()
{
    int n;
    int mib[2] = {CTL_HW, HW_NCPU};
    std::size_t s = sizeof(n);
    sysctl(mib, 2, &n, &s, 0, 0);
    return n;
}

typedef void (*FP)(void*);

extern "C"
void*
__thread_proxy(void* p)
{
    FP fp = *static_cast<FP*>(p);
    fp(p);
    return 0;
}

namespace this_thread
{

void
sleep(const nanoseconds& rel_time)
{
    system_time limit = get_system_time() + rel_time;
    mutex mut;
    unique_lock<mutex> lk(mut);
    condition_variable cv;
    while (get_system_time() < limit)
        cv.timed_wait(lk, limit);
}

}  // this_thread

}  // std
