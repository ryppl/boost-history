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
//
// (C) Copyright Ion Gazta�ga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#if defined(_MSC_VER) && (_MSC_VER == 1310)
#pragma warning (disable : 4275)
#pragma warning (disable : 4251)
#endif

#include <boost/thread/detail/config.hpp>

#include <boost/shmem/sync/shared_condition.hpp>
#include <boost/thread/thread.hpp>
#include <boost/shmem/sync/xtime.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>

namespace{

inline boost::xtime delay(int secs, int msecs=0, int nsecs=0)
{
    const int MILLISECONDS_PER_SECOND = 1000;
    const int NANOSECONDS_PER_SECOND = 1000000000;
    const int NANOSECONDS_PER_MILLISECOND = 1000000;

    boost::xtime xt;
    int ret = boost::xtime_get(&xt, boost::TIME_UTC);
    assert(ret == static_cast<int>(boost::TIME_UTC));(void)ret;
    nsecs += xt.nsec;
    msecs += nsecs / NANOSECONDS_PER_MILLISECOND;
    secs += msecs / MILLISECONDS_PER_SECOND;
    nsecs += (msecs % MILLISECONDS_PER_SECOND) * NANOSECONDS_PER_MILLISECOND;
    xt.nsec = nsecs % NANOSECONDS_PER_SECOND;
    xt.sec += secs + (nsecs / NANOSECONDS_PER_SECOND);

    return xt;
}

template <typename F, typename T>
class binder
{
public:
    binder(const F& func, const T& param)
        : func(func), param(param) { }
    void operator()() const { func(param); }

private:
    F func;
    T param;
};

template <typename F, typename T>
binder<F, T> bind(const F& func, const T& param)
{
    return binder<F, T>(func, param);
}
} // namespace

struct condition_test_data
{
    condition_test_data() : notified(0), awoken(0) { }

    boost::shmem::shared_mutex mutex;
    boost::shmem::shared_condition condition;
    int notified;
    int awoken;
};

void condition_test_thread(condition_test_data* data)
{
    boost::shmem::shared_mutex::scoped_lock lock(data->mutex);
    assert(lock ? true : false);
    while (!(data->notified > 0))
        data->condition.wait(lock);
    assert(lock ? true : false);
    data->awoken++;
}

struct cond_predicate
{
    cond_predicate(int& var, int val) : _var(var), _val(val) { }

    bool operator()() { return _var == _val; }

    int& _var;
    int _val;
};

void condition_test_waits(condition_test_data* data)
{
    boost::shmem::shared_mutex::scoped_lock lock(data->mutex);
    assert(lock ? true : false);

    // Test wait.
    while (data->notified != 1)
        data->condition.wait(lock);
    assert(lock ? true : false);
    assert(data->notified == 1);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate wait.
    data->condition.wait(lock, cond_predicate(data->notified, 2));
    assert(lock ? true : false);
    assert(data->notified == 2);
    data->awoken++;
    data->condition.notify_one();

    // Test timed_wait.
    boost::shmem::xtime xt;
    boost::xtime aux       = delay(10);
    xt.sec = aux.sec;
    xt.nsec = aux.nsec;

    while (data->notified != 3)
        data->condition.timed_wait(lock, xt);
    assert(lock ? true : false);
    assert(data->notified == 3);
    data->awoken++;
    data->condition.notify_one();

    // Test predicate timed_wait.
    aux = delay(10);
    xt.sec = aux.sec;
    xt.nsec = aux.nsec;
    cond_predicate pred(data->notified, 4);
    bool ret = data->condition.timed_wait(lock, xt, pred);
    assert(ret);(void)ret;
    assert(lock ? true : false);
    assert(pred());
    assert(data->notified == 4);
    data->awoken++;
    data->condition.notify_one();
}

void do_test_condition_notify_one()
{
    condition_test_data data;

    boost::thread thread(bind(&condition_test_thread, &data));
    {
        boost::shmem::shared_mutex::scoped_lock lock(data.mutex);
        assert(lock ? true : false);
        data.notified++;
        data.condition.notify_one();
    }

    thread.join();
    assert(data.awoken == 1);
}

void do_test_condition_notify_all()
{
    const int NUMTHREADS = 5;
    boost::thread_group threads;
    condition_test_data data;

    for (int i = 0; i < NUMTHREADS; ++i)
        threads.create_thread(bind(&condition_test_thread, &data));

    {
        boost::shmem::shared_mutex::scoped_lock lock(data.mutex);
        assert(lock ? true : false);
        data.notified++;
        data.condition.notify_all();
    }

    threads.join_all();
    assert(data.awoken == NUMTHREADS);
}

void do_test_condition_waits()
{
    condition_test_data data;

    boost::thread thread(bind(&condition_test_waits, &data));

    {
        boost::shmem::shared_mutex::scoped_lock lock(data.mutex);
        assert(lock ? true : false);

        boost::thread::sleep(delay(1));
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 1)
            data.condition.wait(lock);
        assert(lock ? true : false);
        assert(data.awoken == 1);

        boost::thread::sleep(delay(1));
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 2)
            data.condition.wait(lock);
        assert(lock ? true : false);
        assert(data.awoken == 2);

        boost::thread::sleep(delay(1));
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 3)
            data.condition.wait(lock);
        assert(lock ? true : false);
        assert(data.awoken == 3);

        boost::thread::sleep(delay(1));
        data.notified++;
        data.condition.notify_one();
        while (data.awoken != 4)
            data.condition.wait(lock);
        assert(lock ? true : false);
        assert(data.awoken == 4);
    }

    thread.join();
    assert(data.awoken == 4);
}

//Message queue simulation test
using namespace boost::shmem;

static shared_condition cond_not_empty;
static shared_condition cond_not_full;
static shared_mutex     mutex;
static volatile int     count = 0;

//Function that removes items from queue
static void condition_func(void)
{
   shared_mutex::scoped_lock lock(mutex);
   while(count==0)
      cond_not_empty.wait(lock);
   --count;
   cond_not_full.notify_one();
}

//Queue functions
void do_test_condition_queue(void)
{
   const int queue_size    = 20;
   const int thread_factor = 3;
   const int NumThreads    = thread_factor*queue_size;

   //Create threads that will decrease count
   {
      boost::thread_group thgroup;
      int i;
      for(i = 0; i< NumThreads; ++i){
         thgroup.create_thread(condition_func);
      }

      //Wait until all threads are ready
      boost::thread::sleep(delay(2));

      //Fill 20 elements one by one in the queue simulation
      //The sender will block if it fills the queue
      for(i = 0; i< NumThreads; ++i){
         shared_mutex::scoped_lock lock(mutex);
         while(count >= queue_size)
            cond_not_full.wait(lock);
         count++;
         cond_not_empty.notify_one();
      }
      thgroup.join_all();
      assert(count == 0);
   }

   //Create threads that will decrease count
   {
      boost::thread_group thgroup;
      int i;
      for(i = 0; i< NumThreads; ++i){
         thgroup.create_thread(condition_func);
      }

      //Wait until all threads are ready
      boost::thread::sleep(delay(2));

      //Fill queue to the max size and notify all several times
      for(i = 0; i < thread_factor; ++i){
         {
            shared_mutex::scoped_lock lock(mutex);      
            count = queue_size;
            cond_not_empty.notify_all();
         }
         //Wait until the queue is empty
         while(count){
            boost::thread::sleep(delay(1));
         }
      }
      thgroup.join_all();
      assert(count == 0);
   }
}

int main ()
{
   do_test_condition_notify_one();
   do_test_condition_notify_all();
   do_test_condition_waits();
   do_test_condition_queue();
   return 0;
}


