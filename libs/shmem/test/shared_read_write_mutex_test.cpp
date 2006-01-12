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
// (C) Copyright Ion Gazta�aga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/thread.hpp>
#include <boost/thread/xtime.hpp>
#include <boost/shmem/sync/xtime.hpp>
#include <boost/shmem/sync/shared_read_write_mutex.hpp>
//#include <boost/shmem/sync/shared_read_write_lock.hpp>
#include <iostream>
//#include <cassert>
#include <assert.h>

namespace {

int shared_val = 0;

//boost::shmem::xtime xsecs(int secs)
boost::xtime xsecs(int secs)
{
    boost::xtime ret;
    boost::xtime_get(&ret, boost::TIME_UTC);
    ret.sec += secs;
    return ret;

//    boost::xtime aux;
//    boost::shmem::xtime ret;
//    boost::xtime_get(&aux, boost::TIME_UTC);
//    ret.sec  = aux.sec;
//    ret.nsec = aux.nsec;
//    ret.sec += secs;
//    return ret;
}

template <typename RW>
class thread_adapter
{
public:
    thread_adapter(void (*func)(void*,RW &), void* param1,RW &param2)
        : _func(func), _param1(param1) ,_param2(param2){ }
    void operator()() const { _func(_param1, _param2); }

private:
    void (*_func)(void*, RW &);
    void* _param1;
    RW& _param2;
};

template <typename RW>
struct data
{
    data(int id, RW &m, int secs=0)
        : m_id(id), m_value(-1), m_secs(secs), m_rw(m)
    {
    }
    int m_id;
    int m_value;
    int m_secs;

    RW& m_rw;           // Reader/Writer mutex
};

// plain_writer exercises the "infinite" lock for each
//   read_write_mutex type.

template<typename RW>
void plain_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    // cout << "-->W" << pdata->m_id << "\n";

    typename RW::scoped_read_write_lock l(rw, boost::shmem::read_write_lock_state::write_locked);

    boost::thread::sleep(xsecs(3));
    shared_val += 10;

    pdata->m_value = shared_val;
}

template<typename RW>
void plain_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    typename RW::scoped_read_write_lock l(rw, boost::shmem::read_write_lock_state::read_locked);

    pdata->m_value = shared_val;
}

template<typename RW>
void try_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    // cout << "-->W" << pdata->m_id << "\n";

    typename RW::scoped_try_read_write_lock l(rw, boost::shmem::read_write_lock_state::unlocked);

    if (l.try_write_lock())
    {

        boost::thread::sleep(xsecs(3));
        shared_val += 10;

        pdata->m_value = shared_val;
    }

}

template<typename RW>
void try_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    typename RW::scoped_try_read_write_lock l(rw, boost::shmem::read_write_lock_state::unlocked);

    if (l.try_read_lock())
    {
        pdata->m_value = shared_val;
    }
}

template<typename RW>
void timed_writer(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;

    boost::shmem::xtime xt;
    boost::xtime aux;
    aux = xsecs(pdata->m_secs);
    xt.sec  = aux.sec;
    xt.nsec = aux.nsec;
    typename RW::scoped_timed_read_write_lock l
      (rw,boost::shmem::read_write_lock_state::unlocked);

    if (l.timed_write_lock(xt))
    {
        boost::thread::sleep(xsecs(3));
        shared_val += 10;

        pdata->m_value = shared_val;
    }
}

template<typename RW>
void timed_reader(void *arg,RW &rw)
{
    data<RW> *pdata = (data<RW> *) arg;
    boost::shmem::xtime xt;
    boost::xtime aux;
    aux = xsecs(pdata->m_secs);
    xt.sec  = aux.sec;
    xt.nsec = aux.nsec;

    typename RW::scoped_timed_read_write_lock l
      (rw,boost::shmem::read_write_lock_state::unlocked);

    if (l.timed_read_lock(xt))
    {
        pdata->m_value = shared_val;
    }
}

template<typename RW>
void test_plain_read_write_mutex(RW &rw)
{
    shared_val = 0;
    data<RW> r1(1,rw);
    data<RW> r2(2,rw);
    data<RW> w1(1,rw);
    data<RW> w2(2,rw);

    // Writer one launches, holds the lock for 3 seconds.
    boost::thread tw1(thread_adapter<RW>(plain_writer,&w1,rw));

    // Writer two launches, tries to grab the lock, "clearly"
    //  after Writer one will already be holding it.
    boost::thread::sleep(xsecs(1));
    boost::thread tw2(thread_adapter<RW>(plain_writer,&w2,rw));

    // Reader one launches, "clearly" after writer two, and "clearly"
    //   while writer 1 still holds the lock
    boost::thread::sleep(xsecs(1));
    boost::thread tr1(thread_adapter<RW>(plain_reader,&r1,rw));
    boost::thread tr2(thread_adapter<RW>(plain_reader,&r2,rw));

    tr2.join();
    tr1.join();
    tw2.join();
    tw1.join();

    if (rw.policy() == boost::shmem::read_write_scheduling_policy::writer_priority)
    {
        assert(w1.m_value == 10);
        assert(w2.m_value == 20);
        assert(r1.m_value == 20);   // Readers get in after 2nd writer
        assert(r2.m_value == 20);
    }
    else if (rw.policy() == boost::shmem::read_write_scheduling_policy::reader_priority)
    {
        assert(w1.m_value == 10);
        assert(w2.m_value == 20);
        assert(r1.m_value == 10);   // Readers get in before 2nd writer
        assert(r2.m_value == 10);
    }
    else if (rw.policy() == boost::shmem::read_write_scheduling_policy::alternating_many_reads)
    {
        assert(w1.m_value == 10);
        assert(w2.m_value == 20);
        assert(r1.m_value == 10);   // Readers get in before 2nd writer
        assert(r2.m_value == 10);
    }
    else if (rw.policy() == boost::shmem::read_write_scheduling_policy::alternating_single_read)
    {
        assert(w1.m_value == 10);
        assert(w2.m_value == 20);

        // One Reader gets in before 2nd writer, but we can't tell
        // which reader will "win", so just check their sum.
        assert((r1.m_value + r2.m_value == 30));
    }
}

template<typename RW>
void test_try_read_write_mutex(RW &rw)
{
    data<RW> r1(1,rw);
    data<RW> w1(2,rw);
    data<RW> w2(3,rw);

    // We start with some specialized tests for "try" behavior

    shared_val = 0;

    // Writer one launches, holds the lock for 3 seconds.

    boost::thread tw1(thread_adapter<RW>(try_writer,&w1,rw));

    // Reader one launches, "clearly" after writer #1 holds the lock
    //   and before it releases the lock.
    boost::thread::sleep(xsecs(1));
    boost::thread tr1(thread_adapter<RW>(try_reader,&r1,rw));

    // Writer two launches in the same timeframe.
    boost::thread tw2(thread_adapter<RW>(try_writer,&w2,rw));

    tw2.join();
    tr1.join();
    tw1.join();

    assert(w1.m_value == 10);
    assert(r1.m_value == -1);        // Try would return w/o waiting
    assert(w2.m_value == -1);        // Try would return w/o waiting

    // We finish by repeating the plain tests with the try lock
    //  This is important to verify that try locks are proper read_write_mutexes as
    //    well.
    test_plain_read_write_mutex(rw);

}

template<typename RW>
void test_timed_read_write_mutex(RW &rw)
{
    data<RW> r1(1,rw,1);
    data<RW> r2(2,rw,3);
    data<RW> w1(3,rw,3);
    data<RW> w2(4,rw,1);

    // We begin with some specialized tests for "timed" behavior

    shared_val = 0;

    // Writer one will hold the lock for 3 seconds.
    boost::thread tw1(thread_adapter<RW>(timed_writer,&w1,rw));

    boost::thread::sleep(xsecs(1));
    // Writer two will "clearly" try for the lock after the readers
    //  have tried for it.  Writer will wait up 1 second for the lock.
    //  This write will fail.
    boost::thread tw2(thread_adapter<RW>(timed_writer,&w2,rw));

    // Readers one and two will "clearly" try for the lock after writer
    //   one already holds it.  1st reader will wait 1 second, and will fail
    //   to get the lock.  2nd reader will wait 3 seconds, and will get
    //   the lock.

    boost::thread tr1(thread_adapter<RW>(timed_reader,&r1,rw));
    boost::thread tr2(thread_adapter<RW>(timed_reader,&r2,rw));

    tw1.join();
    tr1.join();
    tr2.join();
    tw2.join();

    assert(w1.m_value == 10);
    assert(r1.m_value == -1);
    assert(r2.m_value == 10);
    assert(w2.m_value == -1);

    // We follow by repeating the try tests with the timed lock.
    //  This is important to verify that timed locks are proper try locks as
    //  well
    test_try_read_write_mutex(rw);

}

} // namespace

#include <boost/shared_ptr.hpp>

boost::shared_ptr<int> return_shared_ptr()
{
   return boost::shared_ptr<int>();
}

int main ()
{
   int i;
   for(i = (int) boost::shmem::read_write_scheduling_policy::writer_priority;
      i <= (int) boost::shmem::read_write_scheduling_policy::alternating_single_read;
      i++)
   {
      boost::shmem::shared_read_write_mutex plain_rw
         (static_cast<boost::shmem::read_write_scheduling_policy::
         read_write_scheduling_policy_enum>(i));
      boost::shmem::shared_try_read_write_mutex try_rw
         (static_cast<boost::shmem::read_write_scheduling_policy::
         read_write_scheduling_policy_enum>(i));
      boost::shmem::shared_timed_read_write_mutex timed_rw
         (static_cast<boost::shmem::read_write_scheduling_policy::
         read_write_scheduling_policy_enum>(i));

      std::cout << "plain test, sp=" << i << "\n";
      test_plain_read_write_mutex(plain_rw);

      std::cout << "try test, sp=" << i << "\n";
      test_try_read_write_mutex(try_rw);

      std::cout << "timed test, sp=" << i << "\n";
      test_timed_read_write_mutex(timed_rw);
   }
   return 0;
}

