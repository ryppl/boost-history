// (C) Copyright 2009 Vicente J. Botet Escriba
// (C) Copyright 2008 Anthony Williams
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/unit_test.hpp>
#include <boost/synchro/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/locks.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/synchro/lockables/functions.hpp>
#include <boost/synchro/thread/thread_to_lockable_adapter.hpp>
#include <boost/synchro/is_lockable.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/synchro/thread/locks.hpp>
#include <boost/synchro/timeout_exception.hpp>
#include <boost/synchro/lockers/array_locker.hpp>
#include <boost/synchro/synchronized.hpp>

struct dummy_mutex : boost::synchro::lock_traits_base<>
{
    bool is_locked;

    dummy_mutex():
        is_locked(false)
    {}

    void lock()
    {
        is_locked=true;
    }

    bool try_lock()
    {
        if(is_locked)
        {
            return false;
        }
        is_locked=true;
        return true;
    }

    void unlock()
    {
        is_locked=false;
    }

    template <class Clock, class Duration >
    bool try_lock_until(boost::chrono::time_point<Clock, Duration> const& abs_time) {
        if(is_locked)
        {
            return false;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        is_locked=true;
        return true;
    }

    template <class Rep, class Period >
    void try_lock_for(boost::chrono::duration<Rep, Period> const& rel_time) {
        if(is_locked)
        {
            return false;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        is_locked=true;
        return true;
    }

    template <class Clock, class Duration >
    void lock_until(boost::chrono::time_point<Clock, Duration> const& abs_time) {
        if(is_locked)
        {
            throw boost::synchro::timeout_exception();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        is_locked=true;
    }

    template <class Rep, class Period >
    void lock_for(boost::chrono::duration<Rep, Period> const& rel_time) {
        if(is_locked)
        {
            throw boost::synchro::timeout_exception();
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        is_locked=true;
    }
    
#if 0
    bool timed_lock(::boost::system_time const& absolute_time)
    {
        if(is_locked)
        {
            return false;
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(10));
        is_locked=true;
        return true;
    }
#endif
};

namespace boost { namespace synchro
{
    template<>
    struct is_lockable<dummy_mutex>
    {
        BOOST_STATIC_CONSTANT(bool, value = true);
    };
}}


void test_lock_two_uncontended()
{
    boost::synchro::thread_mutex m1,m2;

    boost::synchro::unique_locker<boost::synchro::thread_mutex> l1(m1,boost::synchro::defer_lock),
        l2(m2,boost::synchro::defer_lock);

    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());

    boost::synchro::lockables::lock(l1,l2);

    BOOST_CHECK(l1.owns_lock());
    BOOST_CHECK(l2.owns_lock());
}

void test_array_lock_two_uncontended()
{
    boost::synchro::thread_mutex m1,m2;
    boost::synchro::unique_array_locker<boost::synchro::thread_mutex,2> al(m1,m2,boost::synchro::defer_lock);
                    
    BOOST_CHECK(!al.owns_lock());
    al.lock();
    BOOST_CHECK(al.owns_lock());
}

#if 1
void test_synchronize_two_uncontended()
{
    boost::mutex m1,m2;
    defer_synchronize_var(l1,m1) defer_synchronize_var(l2,m2) 
    {
        BOOST_CHECK(!l1.owns_lock());
        BOOST_CHECK(!l2.owns_lock());

        boost::synchro::lockables::lock(l1,l2);

        BOOST_CHECK(l1.owns_lock());
        BOOST_CHECK(l2.owns_lock());
    }
    boost::synchro::lockables::lock(m1,m2);
    adopt_synchronize_var(l1,m1) adopt_synchronize_var(l2,m2) 
    {
        BOOST_CHECK(l1.owns_lock());
        BOOST_CHECK(l2.owns_lock());
    }
    synchronize_var(l1,m1) synchronize_var(l2,m2) 
    {
        BOOST_CHECK(l1.owns_lock());
        BOOST_CHECK(l2.owns_lock());
    }
}
#endif

struct wait_data
{
    boost::synchro::thread_mutex m;
    bool flag;
    boost::condition_variable cond;

    wait_data():
        flag(false)
    {}

    void wait()
    {
        boost::synchro::unique_locker<boost::synchro::thread_mutex> l(m);
        while(!flag)
        {
            cond.wait(l);
        }
    }

    template<typename Duration>
    bool timed_wait(Duration d)
    {
        boost::system_time const target=boost::get_system_time()+d;

        boost::synchro::unique_locker<boost::synchro::thread_mutex> l(m);
        while(!flag)
        {
            if(!cond.timed_wait(l,target))
            {
                return flag;
            }
        }
        return true;
    }

    void signal()
    {
        boost::synchro::unique_locker<boost::synchro::thread_mutex> l(m);
        flag=true;
        cond.notify_all();
    }
};


void lock_mutexes_slowly(boost::synchro::thread_mutex* m1,boost::synchro::thread_mutex* m2,wait_data* locked,wait_data* quit)
{
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l1(*m1);
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l2(*m2);
    locked->signal();
    quit->wait();
}

void lock_pair(boost::synchro::thread_mutex* m1,boost::synchro::thread_mutex* m2)
{
    boost::synchro::lockables::lock(*m1,*m2);
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l1(*m1,boost::adopt_lock),
        l2(*m2,boost::adopt_lock);
}

void test_lock_two_other_thread_locks_in_order()
{
    boost::synchro::thread_mutex m1,m2;
    wait_data locked;
    wait_data release;

    boost::thread t(lock_mutexes_slowly,&m1,&m2,&locked,&release);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    boost::thread t2(lock_pair,&m1,&m2);
    BOOST_CHECK(locked.timed_wait(boost::posix_time::seconds(1)));

    release.signal();

    BOOST_CHECK(t2.timed_join(boost::posix_time::seconds(1)));

    t.join();
}

void test_lock_two_other_thread_locks_in_opposite_order()
{
    boost::synchro::thread_mutex m1,m2;
    wait_data locked;
    wait_data release;

    boost::thread t(lock_mutexes_slowly,&m1,&m2,&locked,&release);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    boost::thread t2(lock_pair,&m2,&m1);
    BOOST_CHECK(locked.timed_wait(boost::posix_time::seconds(1)));

    release.signal();

    BOOST_CHECK(t2.timed_join(boost::posix_time::seconds(1)));

    t.join();
}

void test_lock_five_uncontended()
{
    boost::synchro::thread_mutex m1,m2,m3,m4,m5;

    boost::synchro::unique_locker<boost::synchro::thread_mutex> l1(m1,boost::synchro::defer_lock),
        l2(m2,boost::synchro::defer_lock),
        l3(m3,boost::synchro::defer_lock),
        l4(m4,boost::synchro::defer_lock),
        l5(m5,boost::synchro::defer_lock);

    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());
    BOOST_CHECK(!l3.owns_lock());
    BOOST_CHECK(!l4.owns_lock());
    BOOST_CHECK(!l5.owns_lock());

    boost::synchro::lockables::lock(l1,l2,l3,l4,l5);

    BOOST_CHECK(l1.owns_lock());
    BOOST_CHECK(l2.owns_lock());
    BOOST_CHECK(l3.owns_lock());
    BOOST_CHECK(l4.owns_lock());
    BOOST_CHECK(l5.owns_lock());
}

void lock_five_mutexes_slowly(boost::synchro::thread_mutex* m1,boost::synchro::thread_mutex* m2,boost::synchro::thread_mutex* m3,boost::synchro::thread_mutex* m4,boost::synchro::thread_mutex* m5,
                              wait_data* locked,wait_data* quit)
{
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l1(*m1);
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l2(*m2);
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l3(*m3);
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l4(*m4);
    boost::this_thread::sleep(boost::posix_time::milliseconds(500));
    boost::synchro::unique_locker<boost::synchro::thread_mutex> l5(*m5);
    locked->signal();
    quit->wait();
}

void lock_five(boost::synchro::thread_mutex* m1,boost::synchro::thread_mutex* m2,boost::synchro::thread_mutex* m3,boost::synchro::thread_mutex* m4,boost::synchro::thread_mutex* m5)
{
    boost::synchro::lockables::lock(*m1,*m2,*m3,*m4,*m5);
    m1->unlock();
    m2->unlock();
    m3->unlock();
    m4->unlock();
    m5->unlock();
}

void test_lock_five_other_thread_locks_in_order()
{
    boost::synchro::thread_mutex m1,m2,m3,m4,m5;
    wait_data locked;
    wait_data release;

    boost::thread t(lock_five_mutexes_slowly,&m1,&m2,&m3,&m4,&m5,&locked,&release);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    boost::thread t2(lock_five,&m1,&m2,&m3,&m4,&m5);
    BOOST_CHECK(locked.timed_wait(boost::posix_time::seconds(3)));

    release.signal();

    BOOST_CHECK(t2.timed_join(boost::posix_time::seconds(3)));

    t.join();
}

void test_lock_five_other_thread_locks_in_different_order()
{
    boost::synchro::thread_mutex m1,m2,m3,m4,m5;
    wait_data locked;
    wait_data release;

    boost::thread t(lock_five_mutexes_slowly,&m1,&m2,&m3,&m4,&m5,&locked,&release);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    boost::thread t2(lock_five,&m5,&m1,&m4,&m2,&m3);
    BOOST_CHECK(locked.timed_wait(boost::posix_time::seconds(3)));

    release.signal();

    BOOST_CHECK(t2.timed_join(boost::posix_time::seconds(3)));

    t.join();
}
void lock_n(boost::synchro::thread_mutex* mutexes,unsigned count)
{
    boost::synchro::lockables::lock(mutexes,mutexes+count);
    for(unsigned i=0;i<count;++i)
    {
        mutexes[i].unlock();
    }
}


void test_lock_ten_other_thread_locks_in_different_order()
{
    unsigned const num_mutexes=10;

    boost::synchro::thread_mutex mutexes[num_mutexes];
    wait_data locked;
    wait_data release;

    boost::thread t(lock_five_mutexes_slowly,&mutexes[6],&mutexes[3],&mutexes[8],&mutexes[0],&mutexes[2],&locked,&release);
    boost::this_thread::sleep(boost::posix_time::milliseconds(10));

    boost::thread t2(lock_n,mutexes,num_mutexes);
    BOOST_CHECK(locked.timed_wait(boost::posix_time::seconds(3)));

    release.signal();

    BOOST_CHECK(t2.timed_join(boost::posix_time::seconds(3)));

    t.join();
}



void test_lock_five_in_range()
{
    unsigned const num_mutexes=5;
    dummy_mutex mutexes[num_mutexes];

    boost::synchro::lockables::lock(mutexes,mutexes+num_mutexes);

    for(unsigned i=0;i<num_mutexes;++i)
    {
        BOOST_CHECK(mutexes[i].is_locked);
    }
}

void test_lock_ten_in_range()
{
    unsigned const num_mutexes=10;
    dummy_mutex mutexes[num_mutexes];

    boost::synchro::lockables::lock(mutexes,mutexes+num_mutexes);

    for(unsigned i=0;i<num_mutexes;++i)
    {
        BOOST_CHECK(mutexes[i].is_locked);
    }
}

void test_try_lock_two_uncontended()
{
    dummy_mutex m1,m2;

    int const res=boost::synchro::lockables::try_lock(m1,m2);

    BOOST_CHECK(res==-1);
    BOOST_CHECK(m1.is_locked);
    BOOST_CHECK(m2.is_locked);
}
void test_try_lock_two_first_locked()
{
    dummy_mutex m1,m2;
    m1.lock();

    boost::synchro::unique_locker<dummy_mutex> l1(m1,boost::synchro::defer_lock),
        l2(m2,boost::synchro::defer_lock);

    int const res=boost::synchro::lockables::try_lock(l1,l2);

    BOOST_CHECK(res==0);
    BOOST_CHECK(m1.is_locked);
    BOOST_CHECK(!m2.is_locked);
    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());
}
void test_try_lock_two_second_locked()
{
    dummy_mutex m1,m2;
    m2.lock();

    boost::synchro::unique_locker<dummy_mutex> l1(m1,boost::synchro::defer_lock),
        l2(m2,boost::synchro::defer_lock);

    int const res=boost::synchro::lockables::try_lock(l1,l2);

    BOOST_CHECK(res==1);
    BOOST_CHECK(!m1.is_locked);
    BOOST_CHECK(m2.is_locked);
    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());
}

void test_try_lock_three()
{
    int const num_mutexes=3;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<dummy_mutex> l1(mutexes[0],boost::synchro::defer_lock),
            l2(mutexes[1],boost::synchro::defer_lock),
            l3(mutexes[2],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
        }
    }
}

void test_array_try_lock_three()
{
    int const num_mutexes=3;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_array_locker<dummy_mutex,3> al(mutexes[0],mutexes[1], mutexes[2], boost::synchro::try_to_lock);

        int const res=al.owns_lock();

        BOOST_CHECK((i==-1)?res:!res);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1) BOOST_CHECK(al.owns_lock());
        else      BOOST_CHECK(!al.owns_lock());
    }
}

void test_try_lock_four()
{
    int const num_mutexes=4;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<dummy_mutex> l1(mutexes[0],boost::synchro::defer_lock),
            l2(mutexes[1],boost::synchro::defer_lock),
            l3(mutexes[2],boost::synchro::defer_lock),
            l4(mutexes[3],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3,l4);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
            BOOST_CHECK(l4.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
            BOOST_CHECK(!l4.owns_lock());
        }
    }
}

void test_try_lock_five()
{
    int const num_mutexes=5;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<dummy_mutex> l1(mutexes[0],boost::synchro::defer_lock),
            l2(mutexes[1],boost::synchro::defer_lock),
            l3(mutexes[2],boost::synchro::defer_lock),
            l4(mutexes[3],boost::synchro::defer_lock),
            l5(mutexes[4],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3,l4,l5);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
            BOOST_CHECK(l4.owns_lock());
            BOOST_CHECK(l5.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
            BOOST_CHECK(!l4.owns_lock());
            BOOST_CHECK(!l5.owns_lock());
        }
    }
}

void test_lock_until_two_uncontended()
{
    dummy_mutex m1,m2;
    boost::synchro::thread_to_lockable_adapter<dummy_mutex> ml1(m1), ml2(m2);

    boost::synchro::lockables::lock_until(boost::chrono::system_clock::now()+boost::chrono::milliseconds(2),ml1,ml2);

    BOOST_CHECK(m1.is_locked);
    BOOST_CHECK(m2.is_locked);
}
void test_try_lock_until_two_first_locked()
{
    dummy_mutex m1,m2;
    boost::synchro::thread_to_lockable_adapter<dummy_mutex> ml1(m1), ml2(m2);
    m1.lock();

    boost::synchro::unique_locker<boost::synchro::thread_to_lockable_adapter<dummy_mutex> > l1(ml1,boost::synchro::defer_lock),
        l2(ml2,boost::synchro::defer_lock);
    bool res=false;
    try {
    boost::synchro::lockables::lock_until(boost::chrono::system_clock::now()+boost::chrono::milliseconds(2), l1,l2);
    } catch (boost::synchro::timeout_exception& ex) {
        res=true;
    }

    BOOST_CHECK(res);
    BOOST_CHECK(m1.is_locked);
    BOOST_CHECK(!m2.is_locked);
    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());
}

void test_try_lock_until_two_second_locked()
{
    dummy_mutex m1,m2;
    boost::synchro::thread_to_lockable_adapter<dummy_mutex> ml1(m1), ml2(m2);
    m2.lock();

    boost::synchro::unique_locker<boost::synchro::thread_to_lockable_adapter<dummy_mutex> > l1(ml1,boost::synchro::defer_lock),
        l2(ml2,boost::synchro::defer_lock);

    bool res=false;
    try {
    boost::synchro::lockables::lock_until(boost::chrono::system_clock::now()+boost::chrono::milliseconds(2),l1,l2);
    } catch (boost::synchro::timeout_exception& ex) {
        res=true;
    }

    BOOST_CHECK(res==1);
    BOOST_CHECK(!m1.is_locked);
    BOOST_CHECK(m2.is_locked);
    BOOST_CHECK(!l1.owns_lock());
    BOOST_CHECK(!l2.owns_lock());
}

void test_try_lock_until_three()
{
    int const num_mutexes=3;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];
        boost::synchro::thread_to_lockable_adapter<dummy_mutex> mutexes_l[]={mutexes[0], mutexes[1],mutexes[2]};

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<boost::synchro::thread_to_lockable_adapter<dummy_mutex> > l1(mutexes_l[0],boost::synchro::defer_lock),
            l2(mutexes_l[1],boost::synchro::defer_lock),
            l3(mutexes_l[2],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
        }
    }
}


void test_try_lock_until_four()
{
    int const num_mutexes=4;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];
        boost::synchro::thread_to_lockable_adapter<dummy_mutex> mutexes_l[]={mutexes[0], mutexes[1],mutexes[2],mutexes[3]};

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<boost::synchro::thread_to_lockable_adapter<dummy_mutex> > l1(mutexes_l[0],boost::synchro::defer_lock),
            l2(mutexes_l[1],boost::synchro::defer_lock),
            l3(mutexes_l[2],boost::synchro::defer_lock),
            l4(mutexes_l[3],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3,l4);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
            BOOST_CHECK(l4.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
            BOOST_CHECK(!l4.owns_lock());
        }
    }
}


void test_try_lock_until_five()
{
    int const num_mutexes=5;

    for(int i=-1;i<num_mutexes;++i)
    {
        dummy_mutex mutexes[num_mutexes];
        boost::synchro::thread_to_lockable_adapter<dummy_mutex> mutexes_l[]={mutexes[0], mutexes[1],mutexes[2],mutexes[3],mutexes[4]};

        if(i>=0)
        {
            mutexes[i].lock();
        }
        boost::synchro::unique_locker<boost::synchro::thread_to_lockable_adapter<dummy_mutex> > l1(mutexes_l[0],boost::synchro::defer_lock),
            l2(mutexes_l[1],boost::synchro::defer_lock),
            l3(mutexes_l[2],boost::synchro::defer_lock),
            l4(mutexes_l[3],boost::synchro::defer_lock),
            l5(mutexes_l[4],boost::synchro::defer_lock);

        int const res=boost::synchro::lockables::try_lock(l1,l2,l3,l4,l5);

        BOOST_CHECK(res==i);
        for(int j=0;j<num_mutexes;++j)
        {
            if((i==j) || (i==-1))
            {
                BOOST_CHECK(mutexes[j].is_locked);
            }
            else
            {
                BOOST_CHECK(!mutexes[j].is_locked);
            }
        }
        if(i==-1)
        {
            BOOST_CHECK(l1.owns_lock());
            BOOST_CHECK(l2.owns_lock());
            BOOST_CHECK(l3.owns_lock());
            BOOST_CHECK(l4.owns_lock());
            BOOST_CHECK(l5.owns_lock());
        }
        else
        {
            BOOST_CHECK(!l1.owns_lock());
            BOOST_CHECK(!l2.owns_lock());
            BOOST_CHECK(!l3.owns_lock());
            BOOST_CHECK(!l4.owns_lock());
            BOOST_CHECK(!l5.owns_lock());
        }
    }
}

#if 0
#endif
boost::unit_test_framework::test_suite* init_unit_test_suite(int, char*[])
{
    boost::unit_test_framework::test_suite* test =
        BOOST_TEST_SUITE("Boost.Threads: generic locks test suite");

    test->add(BOOST_TEST_CASE(&test_lock_two_uncontended));
    test->add(BOOST_TEST_CASE(&test_lock_two_other_thread_locks_in_order));
    test->add(BOOST_TEST_CASE(&test_lock_two_other_thread_locks_in_opposite_order));
    test->add(BOOST_TEST_CASE(&test_lock_five_uncontended));
    test->add(BOOST_TEST_CASE(&test_lock_five_other_thread_locks_in_order));
    test->add(BOOST_TEST_CASE(&test_lock_five_other_thread_locks_in_different_order));
    test->add(BOOST_TEST_CASE(&test_lock_five_in_range));
    test->add(BOOST_TEST_CASE(&test_lock_ten_in_range));
    test->add(BOOST_TEST_CASE(&test_lock_ten_other_thread_locks_in_different_order));

    test->add(BOOST_TEST_CASE(&test_try_lock_two_uncontended));
    test->add(BOOST_TEST_CASE(&test_try_lock_two_first_locked));
    test->add(BOOST_TEST_CASE(&test_try_lock_two_second_locked));
    test->add(BOOST_TEST_CASE(&test_try_lock_three));
    test->add(BOOST_TEST_CASE(&test_try_lock_four));
    test->add(BOOST_TEST_CASE(&test_try_lock_five));

    test->add(BOOST_TEST_CASE(&test_lock_until_two_uncontended));
    test->add(BOOST_TEST_CASE(&test_try_lock_until_two_first_locked));
    test->add(BOOST_TEST_CASE(&test_try_lock_until_two_second_locked));
    test->add(BOOST_TEST_CASE(&test_try_lock_until_three));
    test->add(BOOST_TEST_CASE(&test_try_lock_until_four));
    test->add(BOOST_TEST_CASE(&test_try_lock_until_five));
    
    test->add(BOOST_TEST_CASE(&test_array_lock_two_uncontended));
    test->add(BOOST_TEST_CASE(&test_array_try_lock_three));
    
    test->add(BOOST_TEST_CASE(&test_synchronize_two_uncontended));


    return test;
}

